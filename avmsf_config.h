/**
 * \file: avmsf_config.h
 */

/*
 * Vector with the synchronization information
 */
bool synch[256];
char* ip_addr_uav[256];
unsigned int uav_port[256];

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threads Time properties
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
tspec Inflow_Thread_Period = tspec_from(0, MILLI);
int Inflow_Thread_Priority = 90;

tspec Simulation_Thread_Period = tspec_from(2, MILLI);
int Simulation_Thread_Priority = 93;

tspec GroundStation_Thread_Period = tspec_from(4, MILLI);
int GroundStation_Thread_Priority = 70;

tspec UnrealEngine_Thread_Period = tspec_from(20, MILLI);
int UnrealEngine_Thread_Priority = 60;

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Port for the communication with the Boards
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char* uart_name = (char *)"/dev/ttyUSB0";
int baudrate = 921600;



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Ports for the communication with GS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//char* gs_ip = (char *)"127.0.0.1";
char* gs_ip = (char *)"10.30.3.136";
unsigned int gs_r_port = 14551;
unsigned int gs_w_port = 14550;


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Ports for the communication with Unreal Engine
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char* ue_ip = (char *)"10.30.3.136";
unsigned int ue_w_port = 8000;
unsigned int ue_r_port = 9000;
