/**
 ******************************************************************************
 * @file    vdatalog.c
 * @brief   Platform independent application code for Vanilla Datalog
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include "vdatalog.h"
#include "App_model_Ism6hg256x_Mlc.h"
#include "custom_motion_sensors.h"
#include "custom_motion_sensors_ex.h"
#include "Ism6hg256x_reg.h"
#include "main.h"
#include "stdlib.h"
#include "simple_serial_tl.h"
#include "PnPL_init.h"
#include "timestamp.h"
#include "App_model.h"
#include "Ism6hg256x_acc.h"
#include "Ism6hg256x_gyro.h"
#include "Ism6hg256x_mlc_app.h"
//#include "flash_memory.h"
#include "Ism6hg256x_mlc.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

extern TIM_HandleTypeDef htim3;
extern TIM_HandleTypeDef htim5;

#define BOOT_TARGET_MAGIC_VALUE 0xB007A11FUL
#define BOOT_TARGET_SDATALOG    0x53444154UL
#define BOOT_TARGET_AI_INERTIAL 0x4149494EUL

__attribute__((section(".boot_shared"), used)) volatile uint32_t g_boot_target_shared[2];

static void boot_target_set(uint32_t target)
{
  g_boot_target_shared[0] = BOOT_TARGET_MAGIC_VALUE;
  g_boot_target_shared[1] = target;
  __DSB();
  __ISB();
}

static uint32_t boot_target_get(void)
{
  if (g_boot_target_shared[0] != BOOT_TARGET_MAGIC_VALUE) {
    return 0U;
  }
  return g_boot_target_shared[1];
}

static void prepare_for_firmware_handover(void)
{
  /* Conservative teardown: stop local periodic sources only.
   * Avoid sensor/bus deinit calls here because they may block on a bad bus state.
   */
  if (htim3.Instance != NULL) {
    (void)HAL_TIM_Base_Stop_IT(&htim3);
  }
  if (htim5.Instance != NULL) {
    (void)HAL_TIM_Base_Stop_IT(&htim5);
  }

  HAL_NVIC_DisableIRQ(A_G_INT1_EXTI_IRQn);
  HAL_NVIC_DisableIRQ(A_G_INT2_EXTI_IRQn);
  HAL_NVIC_DisableIRQ(TIM3_IRQn);
  HAL_NVIC_DisableIRQ(TIM5_IRQn);
}

/* Private typedef -----------------------------------------------------------*/
extern bool is_MLC_configuration_in_flash(void);
extern void MLC_init_flash_memory(void);

/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint32_t led1_start_time 	= 0;
static uint32_t led10_start_time  	= 0;
uint32_t 		led10_timer  		= LED10_SLOW_TIMER;

/* Communication protocol */
SimpleSerialTL_t *pSstl;
volatile double ACCTimeStamp;
volatile double GYROTimeStamp;

volatile bool command_received;                        /* Flag when command is received from USB. */
uint32_t command_buffer_size;                         /* Size of the command received from USB. */
char command_buffer_static[APP_RX_DATA_SIZE];         /* Buffer containing the data received from USB. */
char *command_buffer_ptr;                              /* Pointer to the beginning of the command buffer. */
char *command_buffer_write_ptr;                        /* Pointer to the current write position in the command buffer. */


#if VD_DUMMY_DATA == 1
static int16_t dummy_counter = 0;
#endif
#if (VD_DUMMY_DATA == 1)
#if (VD_COMBO_SENSOR == 0)
static void InjectDummyData(uint8_t *buffer1, uint32_t data_size);
#else
static void InjectDummyData(uint8_t *buffer1, uint8_t *buffer2, uint32_t data_size);
#endif
#endif

#ifndef NO_PRINTF_USB
/* printf redirection over USB CDC */
int _write(int file, char *ptr, int len)
{
    CDC_Transmit_FS((uint8_t*)ptr, len);
    return len;
}
#endif

/**
 * @brief  Main function for Vanilla datalog application
 *         This function will never return
 * @param  handle: pointer to the HW interface for sensors
 */
void vdatalog_init(void *handle)
{
  boot_target_set(BOOT_TARGET_SDATALOG);

  /* Timestamp Timer initialization */
  TS_TIM_VD_Init();
  TS_TIM_MLC_Init();

  /* Serial Protocol (SSTL) initialization
   * Incoming configuration packets (PnPL) are managed in ProtocolScheduler(void)
   **/
  pSstl = SSTL_Alloc();
  SSTL_Init(pSstl);

  /* Frame format on UART
   * Max length of Datalog Payload buffer is defined by SSTL_MAX_ASYNC_PAYLOAD macro
   * and it depends on the ASPEP capabilities.
   * +--------------+-------------+------------------------------------+
   * | ASPEP Header | SSTL Header |       Datalog Payload buffer       |
   * +--------------+-------------+------------------------------------+
   * |    4 Bytes   |   4 Bytes   |               N Bytes              |
   * +--------------+-------------+------------------------------------+
   **/
 /* Initialize custom motion sensors abstraction layer */
  CUSTOM_MOTION_SENSOR_Init(SENSOR_0, MOTION_ACCELERO | MOTION_GYRO); /* ISM6HG256X */
  CUSTOM_MOTION_SENSOR_Init(SENSOR_1, MOTION_ACCELERO); /* IIS2DULPX */

 /* reset sensor registrs to dft values */
  MY_CUSTOM_MOTION_SENSOR_Reset (SENSOR_0);
  MY_CUSTOM_MOTION_SENSOR_Reset (SENSOR_1);   /* to be extended for IIS2DULPX */
 
  /* PnPL components initialization */
  PnPLSetAllocationFunctions(malloc, free);
  json_set_escape_slashes(0);
  PnPL_Components_Alloc();
  PnPL_Components_Init();

  /* Sensors FSMs initialization */
  FsmInit(&AccFsm,  FSM_STATE_DISABLED, AccTransitions,  sizeof_Acc_transitions,  A_G_INT2_EXTI_IRQn);    // Acc INT2
  FsmInit(&GyroFsm, FSM_STATE_DISABLED, GyroTransitions, sizeof_Gyro_transitions, A_G_INT2_EXTI_IRQn);    // Gyro INT2
  FsmInit(&MlcFsm,  FSM_STATE_DISABLED, MlcTransitions,  sizeof_Mlc_transitions,  A_G_INT1_EXTI_IRQn);     // MLC INT1

#if 0  
/* this code is to load MLC configuration from rom array at startup just for debugging */
#ifndef NO_FLASH_MEM  
  /* check if MLC configuration is already loaded in flash */
  if (!is_MLC_configuration_in_flash())
  {
    /* init in flash MLC magic number, fname and model file with default one */
    MLC_init_flash_memory();
  }
#endif
#endif

  /* set the default INT data ready mode, FIXME consider it can be changed at runtime by the MLC program */
  #ifdef DATA_READY_PULSED
  uint8_t Data;
  CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_CTRL4, &Data);
  Data = Data | 0x02; /* set drdy pulsed */
  CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_CTRL4, Data);
  #else
  uint8_t Data;
  CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_CTRL4, &Data);
  Data = Data & ~0x02; /* set drdy latched */
  CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_CTRL4, Data);
  #endif

	memset((char *) command_buffer_static, 0, APP_RX_DATA_SIZE);
 	command_buffer_ptr = &command_buffer_static[0];
	command_buffer_write_ptr = &command_buffer_static[0];
	command_buffer_size = 0;
	command_received = false;

#ifndef NO_PRINTF_USB 
  /* USB Device initialization */
	MX_USB_Device_Init();

	/* USB initialization timeout (required to stabilize the USB peripheral voltage and to open the terminal). */
	HAL_Delay(500);

        printf ("Vanilla Datalog initialized.\r\n"); // printf over USB CDC
#endif

// for debug purposes uncomment to enable by default acc, gyro or mlc
//      lsm6dsv16x_acc_set_enable(true);  // uncomment to enable ACC by default 
//      lsm6dsv16x_gyro_set_enable(false);  // uncomment to enable gyro by default
//      lsm6dsv16x_mlc_set_enable(true);  // uncomment to enable mlc by default      
//      load_lsm6dsv16x_mlc_configuration(lsm6dsv16x_mlc_conf_0, MEMS_CONF_ARRAY_LEN(lsm6dsv16x_mlc_conf_0)); // uncomment to load dft mlc program at startup
//      log_controller_start_log(0);  // uncomment start logging at startup by default
}

void vdatalog_boot_resume_if_needed(void)
{
  /* Recovery path: hold USER button during reset to force SDatalog startup. */
  if (HAL_GPIO_ReadPin(B1_GPIO_Port, B1_Pin) == GPIO_PIN_RESET) {
    boot_target_set(BOOT_TARGET_SDATALOG);
    return;
  }

  if (boot_target_get() == BOOT_TARGET_AI_INERTIAL) {
    jump_to_bootloader();
  }
}

void vdatalog_main()
{
  /* Infinite loop */
  while(1)
  {
	/* Run the Protocol Scheduler to manage incoming messages */
    ProtocolScheduler();

    /* handle Start/Stop Log evts from PnPL cmds and from INT */
	  FsmHandleEvent(&AccFsm);
	  FsmHandleEvent(&GyroFsm);
    FsmHandleEvent(&MlcFsm);

#ifndef NO_PRINTF_USB
  /* dummy test of USB CDC COM sending back echo of each received data terminated with \CR\LF */
  	if (command_received) {         /* Check if command is received. */
 	  	command_received = false;
	  	printf("%s", command_buffer_ptr);               /* Send back on USB the command received. */
      printf ("\r\n");  /* send Carriage Return and Line Feed to terminate the line on terminal */
		  /* Resetting command buffer. */
		  command_buffer_size = command_buffer_write_ptr - command_buffer_ptr;    /* Size of the last command received. */
		  memset((char *) command_buffer_ptr, 0, command_buffer_size);            /* Resetting the command buffer. */
		  command_buffer_write_ptr = command_buffer_ptr;                          /* Move the write pointer to point to the initial position of the command buffer. */
		  command_buffer_size = 0;
	  }
#endif
    /* Go to sleep. The system wakes up on any IRQ (Systick as well) */
//    __WFI();
  }
}

/**
 * @brief  ProtocolScheduler
 *
 * In bare metal applications this function should be called periodically to check PnPL incoming msgs.
 * In an event driven application with a RTOS it could be invoked in a task when a new packet is received.
 */
void ProtocolScheduler(void)
{
  char *SerializedResponse = NULL;
  uint32_t rx_status;

  if(pSstl == NULL)
  {
    return;
  }
  /* Green LED flash once at GUI connection */
  if (HAL_GetTick() - led1_start_time >= LED1_TIMER)
  {
	  led1_start_time = 0;
//	  HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET);
  }
  /* Blue LED blinking slow indicate prog is running */
  if (HAL_GetTick() - led10_start_time >= led10_timer)
  {
	  led10_start_time = HAL_GetTick();
      HAL_GPIO_TogglePin(LD2_GPIO_Port, LD2_Pin);
  }

  rx_status = SSTL_ProcessRxFrame(pSstl, SSTL_NON_BLOCKING);
  if(rx_status == SSTL_NEW_PACKET_AVAILABLE)
  {
    uint8_t *p_rx_packet = NULL;
    uint32_t size = 0;
    if (SSTL_GetRxPacket(pSstl, &p_rx_packet, &size) == SSTL_OK)
    {
      /* PnPLCommand object declaration */
      PnPLCommand_t PnPLCommand;

      /* Parse received input message. (received_msg is the received input message to parse)
       * PnPLCommand object is filled by the PnPLParseCommand function.
       */
      PnPLParseCommand((char*) p_rx_packet, &PnPLCommand);

      if(PnPLCommand.comm_type == PNPL_CMD_SYSTEM_INFO)
      {
    	 led1_start_time = HAL_GetTick();
//         HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
      }
      /* Check PnPLCommand type:
       * - If it is a GET Message, generate a response with complete or partial status
       * - PNPL_CMD_SYSTEM_INFO is for board identification only (FW_ID, BOARD_ID)
       */
      if(PnPLCommand.comm_type == PNPL_CMD_GET || PnPLCommand.comm_type == PNPL_CMD_SYSTEM_INFO)
      {
        /* Declare Serialized JSON response message and size */
        uint32_t size;

        /* Serialize message response */
        PnPLSerializeResponse(&PnPLCommand, &SerializedResponse, &size, 0);

        /* Set buffer that needs to be sent and its size */
        SSTL_SetResponse(pSstl, (uint8_t*) SerializedResponse, size);

        /* Start the transmission of the buffer that was previously set */
        if (SSTL_TxResponse(pSstl, SSTL_BLOCKING) != SSTL_PARTIAL_PACKET)
        {
          /* Free the json response unless the packet was segmented */
          pnpl_free(SerializedResponse);
          SerializedResponse = NULL;
        } 
      }
      else if(PnPLCommand.comm_type == PNPL_CMD_SET || PnPLCommand.comm_type == PNPL_CMD_COMMAND)
      {
        SSTL_TxAck(pSstl, SSTL_BLOCKING);
      }
    }
  }
  else if (rx_status == SSTL_ACK_RECEIVED) /* An ACK was received. */
  {
    /* Call again SSTL_SetResponse to send another frame of the segmented packet */
    if (SSTL_TxResponse(pSstl, SSTL_BLOCKING) != SSTL_PARTIAL_PACKET)
    {
      pnpl_free(SerializedResponse);
      SerializedResponse = NULL;
    }
  }
    else if (rx_status == SSTL_PARTIAL_PACKET)
  {
    // Send ACK to host for each partial packet received
    SSTL_TxAck(pSstl, SSTL_BLOCKING);
  }    
}

/**
 * @brief  EXTI line detection callback.
 * @param  GPIO_Pin Specifies the port pin connected to corresponding EXTI line.
 * @retval None
 */
#if defined(STM32U5) || defined(STM32H5) || defined(STM32U0)
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
#elif defined(STM32F4) || defined(STM32L4) || defined(STM32G4)
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
#else
#warning "Add support for the current MCU family"
#endif
{
  switch(GPIO_Pin)
  {
/* INT1 carry the MLC and ODR triggered */
    case VD_SENSOR_INT1_PIN:
    {
//   	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_SET);
      /* INT1 can be from Acc/Gyro in ODR triggered */
      FsmSetEventFromINT(&AccFsm, FSM_EVT_INT);            
      FsmSetEventFromINT(&GyroFsm, FSM_EVT_INT);
      /* or from MLC in normal mode */
      FsmSetEventFromINT(&MlcFsm, FSM_EVT_INT);  

//      HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_RESET);    
      break;
    }
/* INT2 carry the Acc and Gyro int or the PWM output configured as GPIO input in ODR triggered mode*/
    case VD_SENSOR_INT2_PIN:
    { 
//   	  HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_SET);      
      FsmSetEventFromINT(&AccFsm, FSM_EVT_INT);            
 	    FsmSetEventFromINT(&GyroFsm, FSM_EVT_INT);      // Gyro INT on same pin
//    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_3, GPIO_PIN_RESET);          
    	break;
    }
  }
}

#if (VD_DUMMY_DATA == 1)
#if (VD_COMBO_SENSOR == 0)
static void InjectDummyData(uint8_t *buffer1, uint32_t data_size)
#else
static void InjectDummyData(uint8_t *buffer1, uint8_t *buffer2, uint32_t data_size)
#endif
{
  uint32_t i;
  int16_t *p16_s1 = (int16_t*) buffer1;
  int16_t *p16_s2 = (int16_t*) buffer2;
  for(i = 0; i < data_size; i++)
  {
    /* Read sensor data */
#if (VD_COMBO_SENSOR == 0)
    *p16_s1++ = dummy_counter++;
    *p16_s1++ = dummy_counter++;
    *p16_s1++ = dummy_counter++;
#else
    *p16_s1++ = *p16_s2++ = dummy_counter++;
    *p16_s1++ = *p16_s2++ = dummy_counter++;
    *p16_s1++ = *p16_s2++ = dummy_counter++;
#endif
  }
}
#endif
  
/*
 * Jump to bootloader.
 */
void jump_to_bootloader( void )
{
  static const uint32_t slot_sdatalog_base = 0x08000000UL;
  static const uint32_t slot_ai_inertial_base = 0x08020000UL;
  static const uint32_t flash_end_address = 0x08080000UL;
	uint32_t i;
  uint32_t current_vector_base;
  uint32_t current_pc;
  uint32_t target_base;
  uint32_t target_stack;
  uint32_t target_reset_handler;
  uint32_t jump_target;

  current_vector_base = SCB->VTOR;
  if (current_vector_base == slot_sdatalog_base) {
    target_base = slot_ai_inertial_base;
  } else if (current_vector_base == slot_ai_inertial_base) {
    target_base = slot_sdatalog_base;
  } else {
    /* Fallback: if VTOR is not one of the two slots, use current code location. */
    current_pc = (uint32_t)&jump_to_bootloader;
    target_base = ((current_pc >= slot_ai_inertial_base) && (current_pc < flash_end_address)) ?
              slot_sdatalog_base : slot_ai_inertial_base;
  }

  boot_target_set((target_base == slot_ai_inertial_base) ? BOOT_TARGET_AI_INERTIAL : BOOT_TARGET_SDATALOG);

	/* Stop application timing sources before changing execution context. */
	#if APP_AI_USE_TIM1
	HAL_TIM_Base_Stop_IT(&htim1);
	#if defined(TIM1_UP_IRQn)
	HAL_NVIC_DisableIRQ(TIM1_UP_IRQn);
	#elif defined(TIM1_UP_TIM10_IRQn)
	HAL_NVIC_DisableIRQ(TIM1_UP_TIM10_IRQn);
	#endif
	#endif

  /* Disable ICACHE before jumping to target app. */
	#if defined(HAL_ICACHE_MODULE_ENABLED)
	HAL_ICACHE_DeInit();
	#endif

	/* Stop SysTick before leaving the application context. */
	SysTick->CTRL = 0U;
	SysTick->LOAD = 0U;
	SysTick->VAL  = 0U;

  target_reset_handler = *(__IO uint32_t *)(target_base + 4U);
  target_stack = *(__IO uint32_t *)target_base;
  if ((target_stack & 0x2FFE0000UL) != 0x20000000UL) {
    printf("ERROR: invalid target MSP at 0x%08lX.\r\n", (unsigned long)target_base);
		return;
	}
  if ((target_reset_handler < slot_sdatalog_base) || (target_reset_handler >= flash_end_address) || ((target_reset_handler & 0x1UL) == 0U)) {
    printf("ERROR: invalid target reset vector at 0x%08lX.\r\n", (unsigned long)target_base);
		return;
	}

  printf("Switching firmware to slot at 0x%08lX...\r\n", (unsigned long)target_base);

  prepare_for_firmware_handover();

	__disable_irq();

  /* Reset HAL and clocks so target app starts from a clean MCU state. */
	HAL_DeInit();
	HAL_RCC_DeInit();

	/* Clear any pending/enabled interrupts left by the application. */
	for (i = 0U; i < 8U; i++) {
		NVIC->ICER[i] = 0xFFFFFFFFUL;
		NVIC->ICPR[i] = 0xFFFFFFFFUL;
	}

  SCB->VTOR = target_base;
	__DSB();
	__ISB();

  jump_target = target_reset_handler;

  /* Initialize target app stack pointer and branch to its reset handler.
   * Do not access stack variables after MSP is updated.
   */
	__set_CONTROL(0U);
	__set_PSP(0U);
  __set_MSP(target_stack);

  /* Reset-like interrupt state before branching to target app. */
	__enable_irq();
  __DSB();
  __ISB();
  __asm volatile ("bx %0" : : "r" (jump_target));

	while (1) {
    /* Should never return from target application. */
	}

}

