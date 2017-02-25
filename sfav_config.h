/**
 * \file: avmsf_config.h
 */

// Ground Station
#ifndef GS_IP
#define GS_IP ("10.30.3.136")
#endif
#ifndef GS_RPORT
#define GS_RPORT (14551)
#endif
#ifndef GS_WPORT
#define GS_WPORT (14550)
#endif

// Unreal Engine
#ifndef UE_IP
#define UE_IP ("10.30.3.136")
#endif
#ifndef UE_RPORT
#define UE_RPORT (9000)
#endif
#ifndef UE_WPORT
#define UE_WPORT (8000)
#endif

// Serial Connection
#ifndef UART_NAME
#define UART_NAME ("/dev/ttyUSB0")
#endif
#ifndef BAUDRATE
#define BAUDRATE (921600)
#endif

// Threads
#ifndef INFLOWTHR_PRIO
#define INFLOWTHR_PRIO (90)
#endif
#ifndef INFLOWTHR_T
#define INFLOWTHR_T (0)
#endif

#ifndef SIMTHR_PRIO
#define SIMTHR_PRIO (93)
#endif
#ifndef SIMTHR_T
#define SIMTHR_T (2)
#endif

#ifndef GSTHR_PRIO
#define GSTHR_PRIO (70)
#endif
#ifndef GSTHR_T
#define GSTHR_T (4)
#endif

#ifndef UETHR_PRIO
#define UETHR_PRIO (60)
#endif
#ifndef UETHR_T
#define UETHR_T (5)
#endif


// Vehicles Connections
#ifndef NCONNECTED_VEHICLES
#define NCONNECTED_VEHICLES (19)
#endif

#ifndef V1_IP
#define V1_IP ("192.168.1.2")
#define V1_PORT (4000)
#endif

#ifndef V2_IP
#define V2_IP ("192.168.1.3")
#define V2_PORT (4002)
#endif

#ifndef EXTPC_IP
#define EXTPC_IP ("10.30.3.24")
#endif
