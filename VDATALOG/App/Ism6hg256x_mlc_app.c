/**
  ******************************************************************************
  * @file           : lsm6dsv16x_mlc.c
  * @brief          : Application code for lsm6dsv16x_mlc
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
// ...existing code...
#include "App_model_Ism6hg256x_Acc.h"
#include "main.h"
#include "Ism6hg256x_reg.h"
#include "Ism6hg256x_mlc.h"
#include "Ism6hg256x_mlc_app.h"
#include "Ism6hg256x_acc.h"
#include "Ism6hg256x_gyro.h"
#include "custom_motion_sensors_ex.h"
#include "simple_serial_tl.h"
#include "stm32f4xx_hal.h"
#include "vdatalog_conf.h"
#include "vdatalog.h"
#include "timestamp.h"
#include "fsm.h"
#include "App_model.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>
#include <inttypes.h>
/* Private includes ----------------------------------------------------------*/
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define VD_BUFFER_SIZE_MLC   (ASPEP_HEADER_SIZE + SSTL_HEADER_SIZE + SAMPLES_PER_TIME_STAMP_MLC*BYTES_PER_SAMPLE_MLC+100)
extern void *MotionCompObj[CUSTOM_MOTION_INSTANCES_NBR];
extern SimpleSerialTL_t   *pSstl;
extern volatile uint8_t   mlc_tim_interrupt;
extern int32_t            send_telemetries_ism6hg256x_mlc(uint8_t mlc_id);
extern void               load_ism6hg256x_mlc_configuration(const struct mems_conf_op* ism6hg256x_mlc_configuration, uint32_t ucf_number_of_lines);
extern void               MLC_store_on_flash_memory(const char* fname, const struct mems_conf_op* ism6hg256x_mlc_configuration, uint32_t ucf_number_of_lines);
extern uint8_t          * Write_To_Flash_From_Memory (uint8_t * memory_addr, const uint8_t * flash_addr_arg, uint32_t size);
uint8_t ism6hg256x_mlc_load_model_app(const char * fname, int32_t size, const char *content);
extern char               mlc_fname[];
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static uint8_t            txbuffer_s3[VD_BUFFER_SIZE_MLC] __attribute__((aligned(4)));
uint8_t                   MlcSensorInUse = 0; // variable to track which sensor is used by the MLC model (0: none, 1: acc, 2: gyro, 4 magneto)
//static uint8_t            loaded_flag = 0;
// = &((struct mems_conf_op*) (ism6hg256x_mlc_conf_0))[0];            /* ISM6HG256X_MLC configuration in use. */
//static uint32_t           current_ism6hg256x_mlc_configuration_size = sizeof(ism6hg256x_mlc_conf_0)/sizeof(struct mems_conf_op);               /* ISM6HG256X_MLC configuration size. */

/** Define the FSM state transitions LUT
     current FSM state,             Input event,    next FSM state,                 FSM state action func **/
Transition MlcTransitions[] = {
  {FSM_STATE_DISABLED,       FSM_EVT_ENABLE,     FSM_STATE_ENABLED,         MlcDisabledStateActionEnable},

  {FSM_STATE_ENABLED,        FSM_EVT_LOAD,       FSM_STATE_LOADED,          MlcEnabledStateActionLoad},
  {FSM_STATE_ENABLED,        FSM_EVT_DISABLE,    FSM_STATE_DISABLED,        MlcEnabledStateActionDisable},    

  {FSM_STATE_LOADED,         FSM_EVT_LOAD,       FSM_STATE_LOADED,          MlcLoadedStateActionLoad},
  {FSM_STATE_LOADED,         FSM_EVT_DISABLE,    FSM_STATE_DISABLED,        MlcLoadedStateActionDisable}, 
  {FSM_STATE_LOADED,         FSM_EVT_START_LOG,  FSM_STATE_LOGGING_DATA,    MlcLoadedStateActionLogRun},

  {FSM_STATE_LOGGING_DATA,   FSM_EVT_INT,        FSM_STATE_LOGGING_DATA,    MlcLoggingDataStateActionInt},
  {FSM_STATE_LOGGING_DATA,   FSM_EVT_STOP_LOG,   FSM_STATE_LOADED,          MlcLoggingDataStateActionLogStop},
};

unsigned int sizeof_Mlc_transitions = sizeof(MlcTransitions) / sizeof(Transition);

/* allocate the FSM obj */
FSM MlcFsm;

/* Private function prototypes -----------------------------------------------*/

extern uint32_t led10_timer;
/* Define event handling user functions for the QVAR FSM */
/* The activation function from off state */
void MlcDisabledStateActionEnable(Event event, void* args)
{
  (void)event;
  (void)args;
}

void MlcEnabledStateActionLoad(Event event, void* args)
{
  (void)event;
  (void)args;
  if(!args || !((MlcModel_t *)args)->filename || ((MlcModel_t *)args)->size <= 0 || !((MlcModel_t *)args)->content)
  {
    return;
  } 

  MlcModel_t *mlc_model_to_load = (MlcModel_t *)args;
  /* store in mem and load on MLC the conf file */
  ism6hg256x_mlc_load_model_app(mlc_model_to_load->filename, mlc_model_to_load->size, mlc_model_to_load->content);

  /* determine which sensor (acc, gyro) is used by the loaded MLC model and set the related FSM EN, ODR, FS accordingly*/
  float_t Odr;
  CUSTOM_MOTION_SENSOR_GetOutputDataRate( SENSOR_0, MOTION_ACCELERO, &Odr);
  if (Odr > 0) {
    /* model uses accelero: set FSM EN, ODR/FS=modelODR/FS in order to make the Acc FSM sending the raw data*/
    CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_ACCELERO, Odr);  // align Acc ODR driver to MLC model ODR
    ism6hg256x_acc_set_enable(true);  // enable Acc to start sending raw data
    MlcSensorInUse = MlcSensorInUse | MOTION_ACCELERO;
  } else {
    /* model dont uses accelero, set FSM DIS, ODR/FS=dft */
    CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_ACCELERO, DEFAULT_ACC_ODR_HZ_FLOAT);  // align Acc ODR driver to MLC model ODR
    CUSTOM_MOTION_SENSOR_SetFullScale(SENSOR_0, MOTION_ACCELERO, DEFAULT_ACC_FS_G_INT);  // align Acc FS driver to MLC model FS        
    ism6hg256x_acc_set_enable(false);  // disable Acc to stop sending raw data (unless enabled later via GUI)   
    MlcSensorInUse = MlcSensorInUse & ~MOTION_ACCELERO;
  }

  CUSTOM_MOTION_SENSOR_GetOutputDataRate( SENSOR_0, MOTION_GYRO, &Odr);  
  if (Odr > 0) {
    /* model uses gyro: set FSM EN, ODR/FS=modelODR/FS in order to make the Gyro FSM sending the raw data*/
    CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_GYRO, Odr);
    ism6hg256x_gyro_set_enable(true);  // enable Gyro to start sending raw data
    MlcSensorInUse = MlcSensorInUse | MOTION_GYRO;
  } else {
    /* model dont uses gyro, set FSM DIS, ODR/FS=dft */
    CUSTOM_MOTION_SENSOR_SetOutputDataRate(SENSOR_0, MOTION_GYRO, DEFAULT_GYRO_ODR_HZ_FLOAT);  // align Gyro ODR driver to MLC model ODR
    CUSTOM_MOTION_SENSOR_SetFullScale(SENSOR_0, MOTION_GYRO, DEFAULT_GYRO_FS_DPS_INT);  // align Gyro FS driver to MLC model FS        
    ism6hg256x_gyro_set_enable(false);  // disable Gyro to stop sending raw data (unless enabled later via GUI)
    MlcSensorInUse = MlcSensorInUse & ~MOTION_GYRO;
  }
}

void MlcEnabledStateActionDisable(Event event, void* args)
{
  (void)event;
  (void)args;
  /* set Acc/Gyro EN, ODR, FS to dft */
  MlcSensorInUse = 0;    
  ism6hg256x_acc_set_enable(false);
  ism6hg256x_acc_set_odr( DEFAULT_ACC_ODR_HZ);
  ism6hg256x_acc_set_fs(DEFAULT_ACC_FS_G);
  ism6hg256x_gyro_set_enable(false);
  ism6hg256x_gyro_set_odr(DEFAULT_GYRO_ODR_HZ);
  ism6hg256x_gyro_set_fs(DEFAULT_GYRO_FS_DPS);
}

void MlcLoadedStateActionLoad(Event event, void* args)
{
  (void)event;
  (void)args;
  MlcEnabledStateActionLoad(event, args);
}   

void MlcLoadedStateActionDisable(Event event, void* args)
{
  (void)event;
  (void)args;
/* set Acc/Gyro EN, ODR, FS to dft */
  MlcSensorInUse = 0;
  ism6hg256x_acc_set_enable(false);
  ism6hg256x_acc_set_odr( DEFAULT_ACC_ODR_HZ);
  ism6hg256x_acc_set_fs(DEFAULT_ACC_FS_G);
  ism6hg256x_gyro_set_enable(false);
  ism6hg256x_gyro_set_odr(DEFAULT_GYRO_ODR_HZ);
  ism6hg256x_gyro_set_fs(DEFAULT_GYRO_FS_DPS);
} 

void MlcLoadedStateActionLogRun(Event event, void* args)
{
  (void)event;
  (void)args;

  /* As the MLC component INT is managed by the MLC program loaded we act on MCU INT1 en/dis */
  HAL_NVIC_EnableIRQ(A_G_INT1_EXTI_IRQn);
  led10_timer = LED10_FAST_TIMER; /* change  led blink timer to fast */
  TS_TIM_MLC_Start(); // start MLC polling timer when MLC is started from GUI
} 

void MlcLoggingDataStateActionLogStop(Event event, void* args)
{
  (void)event;
  (void)args;
  /* As the MLC component INT is managed by the MLC program loaded we act on MCU INT1 en/dis */
  HAL_NVIC_DisableIRQ(A_G_INT1_EXTI_IRQn);
  led10_timer = LED10_SLOW_TIMER; /* change  led blink timer to slow */
  TS_TIM_MLC_Stop(); // stop MLC polling timer when MLC is stopped from GUI
}

void MlcLoggingDataStateActionInt(Event event, void* args)
{
  (void)event;
  (void)args;
  /* read MLC data */
  static uint8_t latest_mlc_event = 0;
  uint8_t ism6hg256x_int_status_reg = 0;

  /* read MLC data */
  /* check for new MLC events */
  (void)CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0,  ISM6HG256X_MLC_STATUS_MAINPAGE, &ism6hg256x_int_status_reg);

  for (uint32_t i = 0; i <= 7; i++) // check the 8 MLC status bits
  {
    if ((ism6hg256x_int_status_reg & (1 << i)) != 0) {
      latest_mlc_event = i;
//    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_SET);            
      send_telemetries_ism6hg256x_mlc(i);    
//    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_RESET);                  
    }
  }

  // if TIM1 timeout occurred send latest mlc event value telemetry
   if (mlc_tim_interrupt)    
   {         
//    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_SET);                
    mlc_tim_interrupt = 0;             
    send_telemetries_ism6hg256x_mlc(latest_mlc_event);
//    HAL_GPIO_WritePin(GPIOG, GPIO_PIN_4, GPIO_PIN_RESET);            
  }    
} 


/* Function to convert a single hex character to its integer value */
uint8_t hex_char_to_int(char character) {
	if (isdigit(character)) {
		return character - '0';
	} else if (isxdigit(character)) {
		return tolower(character) - 'a' + 10;
	}
	return 0; /* Handle invalid input gracefully */
}

/* Function to convert a two-character hex string to an integer */
uint8_t hex_to_int(const char *hex) {
	return (hex_char_to_int(hex[0]) << 4) | hex_char_to_int(hex[1]);
}

uint8_t ism6hg256x_mlc_load_model_app(const char * fname, int32_t size, const char *content)
{
  char *line;
  struct mems_conf_op* current_ism6hg256x_mlc_configuration = NULL;
    if (fname == NULL || content == NULL || size == 0 || size %4 !=0) return 1; /* Error: invalid parameters */

    /* Calculate the number of configuration lines */
    uint32_t n_lines = size  / COMPRESSED_UCF_LINE_WIDTH;

    /* Allocating space for the ISM6HG256X_MLC configuration structure (uncompressed) */
    current_ism6hg256x_mlc_configuration = (struct mems_conf_op*) calloc(n_lines, sizeof(struct mems_conf_op));
    if (current_ism6hg256x_mlc_configuration == NULL) {
        return 1; /* Error: memory allocation failed */
    }

    /* Uncompress and convert to hex the UCF content and fill the ISM6HG256X_MLC configuration structure */
    for (uint32_t i = 0; i < n_lines; i++) {
        line = (char *)&(content[i * (COMPRESSED_UCF_LINE_WIDTH)]); 
        if (*(line+1) == 'w')
        {
          current_ism6hg256x_mlc_configuration[i].type = MEMS_CONF_OP_TYPE_DELAY; 
        } else {
          current_ism6hg256x_mlc_configuration[i].type = MEMS_CONF_OP_TYPE_WRITE; /* FIXME assuming MEMS_CONF_OP_WRITE  */          
        }
        current_ism6hg256x_mlc_configuration[i].address = hex_to_int(line);
        current_ism6hg256x_mlc_configuration[i].data = hex_to_int(line + 2);
    }
    /* Loading ISM6HG256X_MLC configuration into ISM6HG256X_MLC registers. */
    load_ism6hg256x_mlc_configuration(current_ism6hg256x_mlc_configuration, n_lines);
// to load MLC from dft rom file  
//    load_ism6hg256x_mlc_configuration(ism6hg256x_mlc_conf_0, MEMS_CONF_ARRAY_LEN(ism6hg256x_mlc_conf_0));

#ifndef NO_FLASH_MEM
    /* store MLC file and fname in flash */
    MLC_store_on_flash_memory(fname, current_ism6hg256x_mlc_configuration, n_lines);
#else
    /* store the MLC fname in RAM */
    strcpy(mlc_fname, fname);
    strcat(mlc_fname, "\0");
#endif
    free(current_ism6hg256x_mlc_configuration);
    return 0; /* Success */
  }

/*
 * Programming the registers inside the LSM6DSV16X_MLC.
 */
void load_ism6hg256x_mlc_configuration(const struct mems_conf_op* ism6hg256x_mlc_configuration, uint32_t ucf_number_of_lines) {
	/* Iterating over each line in the configuration until NULL is reached. */
	for (uint32_t i = 0; i < ucf_number_of_lines; i++) {
    if (ism6hg256x_mlc_configuration[i].type == MEMS_CONF_OP_TYPE_WRITE) {      
		  CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0,  ism6hg256x_mlc_configuration[i].address, ism6hg256x_mlc_configuration[i].data);
    } else if (ism6hg256x_mlc_configuration[i].type == MEMS_CONF_OP_TYPE_DELAY){
      HAL_Delay(ism6hg256x_mlc_configuration[i].data);
    }
	}
}

/*
 * Reading the content of the LSM6DSV16X_MLC output registers.
 */
void read_ism6hg256x_mlc_output(uint8_t* ism6hg256x_mlc_output_reg, uint8_t mlc_id){

	(void)CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_FUNC_CFG_ACCESS, 0x80);
  if (mlc_id == 0) {
    (void)CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_MLC1_SRC, ism6hg256x_mlc_output_reg);  
  } else if (mlc_id == 1) {
    (void)CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_MLC2_SRC, ism6hg256x_mlc_output_reg);  
  } else if (mlc_id == 2) {
    (void)CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_MLC3_SRC, ism6hg256x_mlc_output_reg);  
  } else if (mlc_id == 3) {
    (void)CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_MLC4_SRC, ism6hg256x_mlc_output_reg);  
  } else if (mlc_id == 4) {
    (void)CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_MLC5_SRC, ism6hg256x_mlc_output_reg);  
  } else if (mlc_id == 5) {
    (void)CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_MLC6_SRC, ism6hg256x_mlc_output_reg);  
  } else if (mlc_id == 6) {
    (void)CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_MLC7_SRC, ism6hg256x_mlc_output_reg);  
  } else if (mlc_id == 7) {
    (void)CUSTOM_MOTION_SENSOR_Read_Register(SENSOR_0, ISM6HG256X_MLC8_SRC, ism6hg256x_mlc_output_reg);  
  } else {
    /* invalid mlc_id, handle error if needed */
  }
	(void)CUSTOM_MOTION_SENSOR_Write_Register(SENSOR_0, ISM6HG256X_FUNC_CFG_ACCESS, 0x00);
}

/*
 * Sending LSM6DSV16X_MLC register outputs.
 */
int32_t send_telemetries_ism6hg256x_mlc(uint8_t mlc_id) 
{
	uint8_t ism6hg256x_mlc_output_reg[8] = {0};

	/* Reading LSM6DSV16X_MLC output. */
	read_ism6hg256x_mlc_output(&ism6hg256x_mlc_output_reg[mlc_id], mlc_id);

	/* Creating PnPL telemetry message. */
	uint32_t size;
	char *telemetry = NULL;
  
	ism6hg256x_mlc_create_telemetry((int)ism6hg256x_mlc_output_reg[mlc_id], &telemetry, &size);

	/* Checking null pointer exception. */
	if(telemetry){
    // copy PnPL telemetry message to TX buffer
    memcpy(&txbuffer_s3[ASPEP_HEADER_SIZE + SSTL_HEADER_SIZE], telemetry, size);

    /* freeing telemetry message memory */
    json_free_serialized_string(telemetry);

    // sending the textual telemetry message via SSTL with channel id 
    if (SSTL_TxAsync(pSstl, SSTL_ASYNC_CH_MLC, SSTL_ASYNC_PAYLOAD_TYPE_TEXTUAL, &txbuffer_s3[ASPEP_HEADER_SIZE + SSTL_HEADER_SIZE], size, SSTL_NON_BLOCKING) != SSTL_OK )
    {
  	  return BSP_ERROR_BUSY;
    }
  }   
  return BSP_ERROR_NONE;
}

#ifndef NO_FLASH_MEM

bool is_MLC_configuration_in_flash(void)
{
  if (strncmp ((char *)MLC_MAGIC_NUMBER_FLASH_ADDRESS, MLC_MAGIC_NUMBER, MLC_MAGIC_NUMBER_SIZE) == 0) return true; // "MLC!" in ASCII
  return false;
}

void MLC_store_on_flash_memory(const char* fname, const struct mems_conf_op* lsm6dsv16x_mlc_configuration, uint32_t ucf_number_of_lines)
{
  // write default MLC configuration fname and ucf file in flash
  uint32_t MLC_magicnum_fname_file_size = MLC_MAGIC_NUMBER_SIZE + MLC_FILE_NAME_MAX_LEN + MLC_PROGRAM_NUM_LINES_SIZE + sizeof(struct mems_conf_op)*ucf_number_of_lines;    
  uint8_t *MLC_magic_num_fname_file_addr = (uint8_t *)calloc(1, MLC_magicnum_fname_file_size);   
  /* initialize the magic number */   
  strncpy ((char *)MLC_magic_num_fname_file_addr, MLC_MAGIC_NUMBER, MLC_MAGIC_NUMBER_SIZE);
  /* initialize the MLC file name */
  memcpy(MLC_magic_num_fname_file_addr+MLC_MAGIC_NUMBER_SIZE, (uint8_t *)fname, strlen(fname)+1);    
  /* initialize MLC file number of lines */
  uint32_t nline = ucf_number_of_lines;
  memcpy(MLC_magic_num_fname_file_addr+MLC_MAGIC_NUMBER_SIZE+MLC_FILE_NAME_MAX_LEN, &nline, MLC_PROGRAM_NUM_LINES_SIZE);
  /* initialize the MLC file */
  memcpy(MLC_magic_num_fname_file_addr+MLC_MAGIC_NUMBER_SIZE+MLC_FILE_NAME_MAX_LEN+MLC_PROGRAM_NUM_LINES_SIZE, (uint8_t *)lsm6dsv16x_mlc_configuration, sizeof(struct mems_conf_op)*ucf_number_of_lines);
  /* write to flash magic number, fname and ucf file */
  Write_To_Flash_From_Memory ((uint8_t *)MLC_magic_num_fname_file_addr,  (const uint8_t *)MLC_MAGIC_NUMBER_FLASH_ADDRESS, MLC_magicnum_fname_file_size);    
  free(MLC_magic_num_fname_file_addr);
}

void MLC_init_flash_memory(void)
{
  MLC_store_on_flash_memory("asset_tracking_ism6hg256x_mlc.ucf", ism6hg256x_mlc_conf_0, sizeof(ism6hg256x_mlc_conf_0)/sizeof(ism6hg256x_mlc_conf_0[0]));
}

#endif

