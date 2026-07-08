# __Simple Serial Transport Layer - SSTL__

This simple point-to-point protocol allows to transfer packets on a serial link, like a UART peripheral. It's a master/slave protocol that supports both synchronous and asynchronous packets, and it's tailored specifically to multi-channel data streaming applications.

Synchronous packets are bi-directional, but they always start with a request from the master, followed by a response from the slave.

Asynchronous packets are sent sent by the slave (data source) to the master.

This SSTL implementation is built on top of the ASPEP protocol layer (Asymmetric Serial Packet Exchange).

The SSTL protocol takes care of the segmentation and reassembly of synchronous packets that are too big to fit in lower level payload (ASPEP protocol). To maintain a certain level of efficiency, the segmentation and reassembly feature is not supported for asynchronous packets.
Maximum ASPEP payload for async packet is 8192 Bytes, but it may vary depending on the device capabilities, and it's negotiated during the handshake phase at the beginning.


## Serial protocol

This section describes the communication protocol between a client (master) and a target board (slave).

At the very beginning of the interaction between a datalog client and a target board which runs datalogging firmware, there is a need to discover supported capabilities.

Communication happens over the serial line (virtual COM) and it usually involves the client sending PING or BEACON packets until a valid response is received from the target.

The serial communication between client and vanilla datalogger follows these rules:

- Vanilla datalogging firmware communicates over serial link according to the ASPEP protocol and indicates a SSTL type of payload (by using reserved bits in the packet headers, as explained in the next sections)
- When using ASPEP protocol the payload must start with a SSTL specific header

ASPEP packets have a 32-bit header, followed by variable length payload and an optional payload CRC. Max payload size is negotiated at connection setup, when master and slave exchange beacon packets. Communication can only occur after a successful initial negotiation phase (connection setup).

ASPEP then defines the following other packet types:

- DATA or ASYNC packet
- RESPONSE packet (to answer a DATA packet)

## Conventions

Multiple-byte fields are drawn with the less significant bytes towards the left and the most significant bytes towards the right.

The multiple-byte fields in the ASPEP/SSTL headers are transmitted in little endian Byte order (following the architecture of the most used MCU/MPU), with more significant bytes being transferred after less-significant (low-order) bytes.

The UART peripheral is configured in LSB mode: for each Byte the least significant bit is the first to be sent

# __ASPEP - Asymmetric Serial Packet Exchange__

## BEACON packet

The exchange of ASPEP beacon packets establishes the initial connection between master and slave and allows negotiating data buffer dimensions; its structure is shown below.

```
+-----------+---------+----------+--------+----------+----------+----------+-----------------+
|  3 bits   |  1 bit  |  3 bits  | 1 bit  |  6 bits  |  7 bits  |  7 bits  |     4 bits      |
+-----------+---------+----------+--------+----------+----------+----------+-----------------+
| Type (5)  | Parity  | Version  |  CRC   | RXS Max  | TXS Max  | TXA Max  |      CRCH       |
+-----------+---------+----------+--------+----------+----------+----------+-----------------+
0           3         4          7        8          14         21         28               31
```
**Table 1: ASPEP beacon packet structure.**

Just as an example, below is the representation of the Beacon packet in memory, written in C code.
```C
typedef struct
{
       uint32_t type     : 3;
       uint32_t parity   : 1;
       uint32_t version  : 3;
       uint32_t CRC      : 1;
       uint32_t RXS      : 6;
       uint32_t TXS      : 7;
       uint32_t TXA      : 7;
       uint32_t CRCH     : 4;
} ASPEP_Beacon_Header
```

## ASPEP Capabilities

- **RXS_MAX (2048 Bytes):** max payload size of a DATA packet the slave can receive
- **TXS_MAX (4096 Bytes):** max payload size of a RESPONSE packet the slave can send
- **TXA_MAX (8128 Bytes):** max payload size of an ASYNC packet the slave can send

The ASPEP protocol specifies other types of packets as summarized below.

### DATA/ASYNC
```
+------------------+---------+---------------------+-----------+--------+-----------------+
|      3 bits      |  1 bit  |      13 bits        |  11 bits  | 4 bits |    N*8 bits     |
+------------------+---------+---------------------+-----------+--------+-----------------+
|     Type (1)     | Parity  | Payload Length (N)  | Reserved  | CRC-4  |     Payload     |
+------------------+---------+---------------------+-----------+--------+-----------------+
0                  3         4                    17          28       32  
```
This packet type is used to transmit data in both directions.

### RESPONSE
```
+------------------+---------+---------------------+-----------+--------+-----------------+
|      3 bits      |  1 bit  |      13 bits        |  11 bits  | 4 bits |    N*8 bits     |
+------------------+---------+---------------------+-----------+--------+-----------------+
|     Type (2)     | Parity  | Payload Length (N)  | Reserved  | CRC-4  |     Payload     |
+------------------+---------+---------------------+-----------+--------+-----------------+
0                  3         4                    17          28       32  
```
This packet is a response to a DATA packet.

### PING
```
+----------+---------+-----+-----+-----+-----+------+-----------------+
|  3 bits  |   1 b   | 1 b | 1 b | 1 b | 1 b |4 bits|    8*N bits     |
+----------+---------+-----+-----+-----+-----+------+-----------------+
| Type (T) | Parity  |  C  |  C  |  N  |  N  | LIID | Packet Number   |
+----------+---------+-----+-----+-----+-----+------+-----------------+
0          3         4     5     6     7     8      12
```
This packet is used to verify the connectivity between master and slave. Master sends a ping packet and slave responds with the same packet. If master does not receive a reply within a time limit, it can infer that the slave is no longer available.

For a complete reference of the ASPEP protocol, see [MCSDK](https://www.st.com/en/embedded-software/x-cube-mcsdk.html) documentation.

Before the actual datalogging session can be started, the client must associate a valid device model which describes the types of data (telemetry), parameters and commands that can be accepted by the datalogging firmware that runs on the target board.

# __Simple Serial Transport Layer (SSTL)__

SSTL serial protocol is introduced to transmit specific payload types on top of ASPEP frames.

SSTL manages different message types:

- PnPL requests (JSON format, derives from a device model)
- PnPL responses (same as above)
- Raw/binary as well as textual data (Raw sensor data, derived from ST's High Speed Datalogger protocol)

Both synchronous and asynchronous messages are supported. SSTL takes care of the segmentation and reassembly of synchronous higher-level packets that are too big to fit in lower level ASPEP payload, which is 4096 Bytes. To maintain a certain level of efficiency, the segmentation and reassembly feature is not supported for asynchronous packets (max ASPEP payload size for async packets is 8192 Bytes as opposed to 4096 Bytes for synchronous packets).

The SSTL header has 32bits and is placed at the beginning of the ASPEP DATA/ASYNC payload. Its structure is described below.

## Single frame type

This SSTL header is used for synchronous request, responses, and for asynchronous data in all cases where the payload size fits into a single packet (4kB max). These packets will have a ‘FIN’ bit set to one.

**Standard SSTL 32-bits frame header**
```
+--------+--------+--------+-------+----------+-------------------+
| 2 bits | 3 bits | 3 bits | 1 bit |  7 bits  |      16 bits      |
+========+========+========+=======+==========+===================+
|   vv   |   RRR  |   cr   | FIN=1 | Channel# |     Sequence#     |
+--------+--------+--------+-------+----------+-------------------+
0        2        5        8       9         16                  31
```
## Segmented frame types

In case of packet segmentation into multiple frames, it’s important that the receiver knows the total size of the packet, so that it can allocate a suitable memory buffer. In this case, the packet being transmitted will have a ‘FIN’ bit set to zero and the following header bits will encode the total packet size (see Figure 3). The following packets will use the following scheme with the ‘FIN’ bit still set to zero, until the last packet which will have the ‘FIN’ bit set to 1.

**SSTL 32-bits frame header in case of segmentation (first packet of the sequence)**
```
+--------+--------+--------+-------+------------------------------+
| 2 bits | 3 bits | 3 bits | 1 bit |           23 bits            |
+========+========+========+=======+==============================+
|   vv   |   RRR  |   cr   | FIN=0 |    Total Packet Size (8MB)   |
+--------+--------+--------+-------+------------------------------+
0        2        5        8       9                             31
```
**SSTL 32-bits frame header in case of segmentation (from second to N-1 packet)**
```
+--------+--------+--------+-------+----------+-------------------+
| 2 bits | 3 bits | 3 bits | 1 bit |  7 bits  |      16 bits      |
+========+========+========+=======+==========+===================+
|   vv   |   RRR  |   cr   | FIN=0 | Channel# |     Sequence#     |
+--------+--------+--------+-------+----------+-------------------+
0        2        5        8       9         16                  31
```
**SSTL 32-bits frame header in case of segmentation (last packet)**
```
+--------+--------+--------+-------+----------+-------------------+
| 2 bits | 3 bits | 3 bits | 1 bit |  7 bits  |      16 bits      |
+========+========+========+=======+==========+===================+
|   vv   |   RRR  |   cr   | FIN=1 | Channel# |     Sequence#     |
+--------+--------+--------+-------+----------+-------------------+
0        2        5        8       9         16                  31
```

In case of segmentation, SSTL uses a simple stop and wait scheme, where each fragment must be acknowledged by the receiver before the next fragment is transmitted. SSTL header fields have the meaning described in the table below.

**SSTL header fields**
```
| Field     | Value   | Meaning                                                   |
|-----------|---------|-----------------------------------------------------------|
| vv        | 00      | Protocol version                                          |
| RRR       | 000     | Reserved bits                                             |
| cr        | 000     | Binary data - HSD Protocol                                |
|           | 001     | Command / Request                                         |
|           | 010     | Response                                                  |
|           | 011     | Async property change / notification                      |
|           | 100     | Binary data - Custom Data Structure                       |
|           | 111     | Textual data                                              |
| FIN       | 1       | Last frame                                                |
|           | 0       | Continuation frame                                        |
| Channel#  | 0-127   | Logical channel number (only supported for binary data)   |
| Sequence# | 0-65535 | Current frame number when segmentation is used            |
```

## SSTL single frame message examples

Note: ASPEP headers are not shown in the figures below.

### Single frame Command: 0x01800000
```
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     | JSONpayload(DTDLcommand) |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 001 |  1  | 0000000 | 00000000 00000000 |                          |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```

### Single frame Response: 0x02800000
```
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     | JSONpayload(DTDLcommand) |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 010 |  1  | 0000000 | 00000000 00000000 |                          |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```
Commands and responses are “PnPL” messages, according to the device model definitions. Single frame responses are also used to acknowledge the correct reception of fragments in case segmentation is used.

### Single frame binary ASYNC data (on channel 12): 0x008C0000
```
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |    Binary data payload   |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 000 |  1  | 0001100 | 00000000 00000000 |                          |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```

### Single frame textual ASYNC data (on channel 12): 0x098C0000
```
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |   Textual data payload   |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 111 |  1  | 0001100 | 00000000 00000000 |                          |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```

### ??????????????????????? Single frame textual ASYNC PnPL property change: 0x03800000
```
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |   Textual data payload   |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 011 |  1  | 0000000 | 00000000 00000000 |                          |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```

## Message examples with segmentation (total packet size = 10000Bytes in the examples below)

The following example refers to the client sending a PnPL command (like “get_status_all”), that results in a response larger than the ASPEP max frame length. The symbol → indicates a message being sent from the client (master) to the target board (slave). Vice versa, the symbol ← indicates a message being sent from the target board (slave) to the client (master).

### Request (→): 0x01800000 + PnPL command payload
```
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |   PnPL Request Payload   |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 001 |  1  | 0000000 | 00000000 00000000 |                          |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```

### Response (← First frame): 0x02002710 + PnPL response payload
```
+----+-----+-----+-----+-----------------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |  Total Packet Size (10000)  |  PnPL Response Payload   |
+====+=====+=====+=====+=============================+===== = = = = = = = = ====+
| 00 | 000 | 010 |  0  |  0000000 00100111 00010000  |     1st segment 4kB      |
+----+-----+-----+-----+-----------------------------+----- - - - - - - - - ----+
```

### Ack first frame (→): 0x01800000
```
+----+-----+-----+-----+---------+-------------------+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |
+====+=====+=====+=====+=========+===================+
| 00 | 000 | 001 |  1  | 0000000 | 00000000 00000000 |
+----+-----+-----+-----+---------+-------------------+
```

### Response (← Second frame): 0x02000001
```
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |  PnPL Response Payload   |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 010 |  0  | 0000000 | 00000000 00000001 |     2nd segment 4kB      |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```

### Ack second frame (→): 0x01800001
```
+----+-----+-----+-----+---------+-------------------+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |
+====+=====+=====+=====+=========+===================+
| 00 | 000 | 001 |  1  | 0000000 | 00000000 00000001 |
+----+-----+-----+-----+---------+-------------------+
```

### Response (← Last frame): 0x02800002
```
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |  PnPL Response Payload   |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 010 |  1  | 0000000 | 00000000 00000010 |     2nd segment 2kB      |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```

To avoid dealing with large response packets, the client can request the status of single PnPL components (i.e. avoid “get_status_all” command).


## Message examples with segmentation on both side (total packet size = 10000Bytes in the examples below)

The following example refers to the client sending a PnPL command (like “get_status_all”) larger than RXS_SLAVE_MAX length. In this case, also messages from host must be segmented. The response also from the device is larger than the ASPEP max frame length. The symbol → indicates a message being sent from the client (master) to the target board (slave). Vice versa, the symbol ← indicates a message being sent from the target board (slave) to the client (master).

### Request (→): Header with request total message size + PnPL command payload - First frame
```
+----+-----+-----+-----+-----------------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |  Total Packet Size (10000)  |   PnPL Request Payload   |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 001 |  0  |  0000000 00100111 00010000  |                          |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```

### Ack first frame (←): Header with sequence#
```
+----+-----+-----+-----+---------+-------------------+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |
+====+=====+=====+=====+=========+===================+
| 00 | 000 | 010 |  1  | 0000000 | 00000000 00000000 |
+----+-----+-----+-----+---------+-------------------+
```

### Request (→): Header with sequence# + PnPL command payload - Second frame
```
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |   PnPL Request Payload   |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 001 |  0  | 0000000 | 00000000 00000001 |                          |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```

### Ack second frame (←): Header with sequence#
```
+----+-----+-----+-----+---------+-------------------+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |
+====+=====+=====+=====+=========+===================+
| 00 | 000 | 010 |  1  | 0000000 | 00000000 00000001 |
+----+-----+-----+-----+---------+-------------------+
```

### Request (→): Header with sequence# + PnPL command payload - last frame
```
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |   PnPL Request Payload   |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 001 |  1  | 0000000 | 00000000 00000010 |                          |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```

### Response (← First frame): Request has been completely received ---> start responding: received 0x02002710 + PnPL response payload
```
+----+-----+-----+-----+-----------------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |  Total Packet Size (10000)  |  PnPL Response Payload   |
+====+=====+=====+=====+=============================+===== = = = = = = = = ====+
| 00 | 000 | 010 |  0  |  0000000 00100111 00010000  |     1st segment 4kB      |
+----+-----+-----+-----+-----------------------------+----- - - - - - - - - ----+
```

### Ack first frame (→): 0x01800000
```
+----+-----+-----+-----+---------+-------------------+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |
+====+=====+=====+=====+=========+===================+
| 00 | 000 | 001 |  1  | 0000000 | 00000000 00000000 |
+----+-----+-----+-----+---------+-------------------+
```

### Response (← Second frame): 0x02000001
```
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |  PnPL Response Payload   |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 010 |  0  | 0000000 | 00000000 00000001 |     2nd segment 4kB      |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```

### Ack second frame (→): 0x01800001
```
+----+-----+-----+-----+---------+-------------------+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |
+====+=====+=====+=====+=========+===================+
| 00 | 000 | 001 |  1  | 0000000 | 00000000 00000001 |
+----+-----+-----+-----+---------+-------------------+
```

### Response (← Last frame): 0x02800002
```
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
| vv | RRR |  cr | FIN |   Ch#   |     Sequence#     |  PnPL Response Payload   |
+====+=====+=====+=====+=========+===================+===== = = = = = = = = ====+
| 00 | 000 | 010 |  1  | 0000000 | 00000000 00000010 |     2nd segment 2kB      |
+----+-----+-----+-----+---------+-------------------+----- - - - - - - - - ----+
```

To avoid dealing with large response packets, the client can request the status of single PnPL components (i.e. avoid “get_status_all” command).

