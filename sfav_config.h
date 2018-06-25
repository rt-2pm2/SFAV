/**
 * \file: avmsf_config.h
 */

// ==============================================================================
// ==============================================================================
// Interfaces
// ==============================================================================
// ==============================================================================

// Ground Station
/*
* GS_IP: IP address of the machine hosting the Ground Station
* default: ("127.0.0.1")
* 
* GS_RPORT: Port for the incoming data from the GS
* default: 14551
* 
* GS_WPORT: Port for the outcoming data from the GS
* default: 14550 
*/
#ifndef GS_IP
#define GS_IP ("127.0.0.1")
#endif
#ifndef GS_RPORT
#define GS_RPORT (14551)
#endif
#ifndef GS_WPORT
#define GS_WPORT (14550)
#endif

// Unreal Engine
/*
* UE_IP: IP address of the machine hosting the Unreal Engine
* default: ("127.0.0.1")
* 
* UE_RPORT: Port for the incoming data from the UE
* default: 9000
* 
* UE_WPORT: Port for the outcoming data from the UE
* default: 8000 
*/
#ifndef UE_IP
#define UE_IP ("10.30.3.136")
#endif
#ifndef UE_CRT_PORT
#define UE_CRT_PORT (7900)
#endif
#ifndef UE_RPORT
#define UE_RPORT (8000)
#endif
#ifndef UE_WPORT
#define UE_WPORT (9000)
#endif
#ifndef UE_VPORT
#define UE_VPORT (10000)
#endif

// Debug Interface
#ifndef DBG_IP
#define DBG_IP ("10.30.3.136")
#endif
#ifndef DBG_PORT
#define DBG_PORT (10000)
#endif

// Vehicle
#ifndef VH_TYPEID
#define VH_TYPEID (2)
#endif
#ifndef VH_SYNCH
#define VH_SYNCH (0)
#endif
#ifndef VH_IPCAM
#define VH_IPCAM ("10.30.3.36")
#endif
// Vehicle - UDP Connection
#ifndef VH_IPADDR
#define VH_IPADDR ("10.30.3.36")
#endif
#ifndef VH_PORTBASE_READ
#define VH_PORTBASE_READ (4000)
#endif
#ifndef VH_PORTBASE_WRITE
#define VH_PORTBASE_WRITE (4001)
#endif
// Vehicle - Serial Connection
/*
* UART_NAME: Name of the Serial Interface used for connecting autopilot board
* default: ("/dev/ttyUSB0")
* 
* BAUDRATE: Connection baud rate
* default: 921600
*/
#ifndef VH_DEVICE
#define VH_DEVICE ("/dev/ttyUSB0")
#endif
#ifndef VH_DEVICE_BAUDRATE
#define VH_DEVICE_BAUDRATE (921600)
#endif




// ==============================================================================
// ==============================================================================
// Threads
// ==============================================================================
// ==============================================================================
/*
* INFLOWTHR_PRIO: Priority of the Inflow Thread
* default: 90
* 
* INFLOWTHR_T: Period of the Inflow Thread (In ms)
* default: 0 (It has been implemented as aperiodic thread)
*/
#ifndef INFLOWTHR_PRIO
#define INFLOWTHR_PRIO (90)
#endif
#ifndef INFLOWTHR_T
#define INFLOWTHR_T (0)
#endif

/*
* SIMTHR_PRIO: Priority of the Simulation Thread
* default: 90
* 
* SIMTHR_T: Period of the Simulation Thread (In ms)
* default: 2 
*/
#ifndef SIMTHR_PRIO
#define SIMTHR_PRIO (93)
#endif
#ifndef SIMTHR_T
#define SIMTHR_T (2)
#endif

/*
* GSTHR_PRIO: Priority of the Ground Station Thread
* default: 70
* 
* GSTHR_T: Period of the Ground Station Thread (In ms)
* default: 4 
*/
#ifndef GSTHR_PRIO
#define GSTHR_PRIO (70)
#endif
#ifndef GSTHR_T
#define GSTHR_T (4)
#endif

/*
* UETHR_PRIO: Priority of the Unreal Engine Thread
* default: 60
* 
* UETHR_T: Period of the Ground Station Thread (In ms)
* default: 0 (It has been implemented as aperiodic)
*/
#ifndef UETHR_PRIO
#define UETHR_PRIO (60)
#endif
#ifndef UETHR_T
#define UETHR_T (0)
#endif





// ==============================================================================
// ==============================================================================
// Vehicles Connections
// ==============================================================================
// ==============================================================================
/*
 * NCONNECTED_VEHICLES: Number of vehicle connected via UDP
 * default: 1
 */
//#ifndef NCONNECTED_VEHICLES
//#define NCONNECTED_VEHICLES (1)
//#endif

/*
 * NSERIAL_VEHICLES: Number of vehicle connected via Serial Interface
 * default: 0
 */
//#ifndef NSERIAL_VEHICLES
//#define NSERIAL_VEHICLES (0)
//#endif

/*
 * V1_IP: IP address of the first autopilot board
 * default: "192.168.1.2"
 * 
 * V1_PORT: Port for communication with the autopilot board
 * default: 4000
 */
//#ifndef V1_IP
//#define V1_IP ("192.168.1.2")
//#define V1_PORT (4000)
//#endif

/*
 * V1_IP: IP address of the second autopilot board
 * default: "192.168.1.3"
 * 
 * V1_PORT: Port for communication with the autopilot board
 * default: 4002
 */
//#ifndef V2_IP
//#define V2_IP ("10.30.3.49")
//#define V2_PORT (4002)
//#endif

/*
 * EXTPC_IP: IP address of the PC running autopilot software
 * default: "127.0.0.1"
 * 
 */
//#ifndef EXTPC_IP
//#define EXTPC_IP ("10.30.3.24")
//#define EXTPC_IP ("127.0.0.1")
//#endif


// ==============================================================================
// ==============================================================================
// Enviroment 
// ==============================================================================
// ==============================================================================

// Coordinate
/*
 * INITIAL_COORDINATE_LAT: Initial Latitude of the vehicles
 * INITIAL_COORDINATE_LON: Initial Longitude of the vehicles
 */
#ifndef INITIAL_COORDINATE_LAT
#define INITIAL_COORDINATE_LAT (43.8148386)
#endif

#ifndef INITIAL_COORDINATE_LON
#define INITIAL_COORDINATE_LON (10.3192456)
#endif
