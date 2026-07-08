# ASPEP Slave

This is a slave implementation of the ASPEP protocol (Asymmetric Serial Packet Exchange).

## ASPEP Features

- Light serial transport protocol for applications that produce data
- Asymmetric:
  - A point-to-point protocol where both ends that have different roles and traffic shapes
  - Partial Master/Slave communication scheme
- Serial:
  - Designed with Serial buses (UART) in mind
- Packets: 
  - Master and Slave exchange packets and not frames
  - No start or end flags, no data escaping inside
  - Time based transmission error recovery mechanism
- Connected protocol
  - Master and Slave need to establish an ASPEP connection before they can exchange data
    - Master is the one who initiates the connection – A Master is never Slave and vice-versa.
  - ASPEP connection establishment procedure includes parameters negotiation
- Provides different communication channels multiplexed across the Serial link
  - Synchronous channel  --> Master sends, Slave responds
  - Asynchronous channel --> Slave sends, no answer


For a complete reference of the ASPEP protocol, see [MCSDK](https://www.st.com/en/embedded-software/x-cube-mcsdk.html) documentation.
Official documentation on ASPEP protocol can be found in X-CUBE-MCSDK installation folder also.

