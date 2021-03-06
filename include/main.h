/**
 * \file main.h
 *
 * This application perform the routing of messages between Ground Station
 * Simulator and Autopilot Board. 
 *
 *
 *
 * \author Luigi Pannocchi, <l.pannocchi@gmail.com>
 */


// -----------------------------------------------------------------------
//   Includes
// -----------------------------------------------------------------------

#include <stdio.h>
#include <cstdlib>
#include <unistd.h>
#include <cmath>
#include <string.h>
#include <inttypes.h>
#include <fstream>
#include <signal.h>
#include <time.h>
#include <sys/time.h>
#include <stdint.h>
#include <vector>

using namespace std;

#include <common/mavlink.h>

#include "simulator_interface.h"
#include "sim_manager.h"
#include "gs_interface.h"
#include "ue_interface.h"
#include "autopilot_interface.h"
#include "ma_manager.h"

extern "C" {
#include <ptask.h>
#include <tstat.h>
#include <pbarrier.h>
}

#include "../sfav_config.h"


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Threads Time properties
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
tspec Inflow_Thread_Period = tspec_from(INFLOWTHR_T, MILLI); /// Period of the Inflow Thread
int Inflow_Thread_Priority = INFLOWTHR_PRIO; /// Priority of the Inflow Thread

tspec Simulation_Thread_Period = tspec_from(SIMTHR_T, MILLI); /// Period of the Simulation Thread
int Simulation_Thread_Priority = SIMTHR_PRIO; /// Priority of the Simulation Thread

tspec GroundStation_Thread_Period = tspec_from(GSTHR_T, MILLI); /// Period of the Groundstation Thread
int GroundStation_Thread_Priority = GSTHR_PRIO; /// Priority of the Groundstation Thread

tspec UnrealEngine_Thread_Period = tspec_from(UETHR_T, MILLI); /// Period of the UnrealEngine Thread
int UnrealEngine_Thread_Priority = UETHR_PRIO; /// Priority of the UnrealEngine Thread


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Ports for the communication with Debug Application
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char* dbg_ip = (char *)DBG_IP; /// IP of the Debug Application
unsigned int dbg_port = DBG_PORT; /// Starting Write Port of the Debug Application

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Ports for the communication with GS
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char* gs_ip = (char *)GS_IP; /// IP of the Groundstation
unsigned int gs_r_port = GS_RPORT; /// Read Port of the Groundstation
unsigned int gs_w_port = GS_WPORT; /// Write Port of the Groundstation

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Ports for the communication with Unreal Engine
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
char* ue_ip = (char *)UE_IP; /// IP of the Unreal Engine
unsigned int ue_w_port = UE_WPORT; /// Read Port of the Unreal Engine
unsigned int ue_r_port = UE_RPORT; /// Write Port of the Unreal Engine
unsigned int ue_v_port = UE_VPORT; /// Video Port of the Unreal Engine

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Port for the communication with the Boards
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//char* uart_name = (char *)UART_NAME; /// Name of the uart for serial communication
//int baudrate = BAUDRATE; /// Serial connection baudrate

// Threads Indexes
int gsT_id; // Ground station thread index
int ueT_id; // Unreal Engine thread index


/** 
 * Struct with the coordinates of the HOME
 */
struct HomePoint
{
    double Latitude;
    double Longitude;
    double Altitude;
};

/**
 * Struct for the Unreal Engine Connection
 */
struct UEConnData
{
    bool active;      /// Active Flag
    
    char ip[15];         /// IP address
    
    uint32_t br_port; /// Base Reading port 
    uint32_t bw_port; /// Base Writing port
    
    uint32_t bv_port; /// Base Video port
};

/**
 * Struct with the pointes to the interfaces
 * to be used as arguments for the threads
 */

/*!
 * Struct for the Launch Thread
 */
struct LaunchArg
{
    tpars params; /// Ptask Thread parameters
    MA_Manager* ma; /// Pointer to the MultiAgent Interface Class
    Sim_Manager* sm; /// Pointer to the Simulators Manager Interface Class
    GS_Interface* gs; /// Pointer to the GroundStation Interface Class
    struct UEConnData* ue; /// Pointer to the UnrealEngine Interface Class
    
    CommInterfaceType commType; /// Connection type (UDP, SERIAL)
    char ip[16]; /// Ip Address of the vehicle
    uint32_t r_port; /// Reading port 
    uint32_t w_port; /// Writing port
    int baudrate;   /// Serial port baudrate
    char* uart_name; /// Serial port device
    bool synch; /// Activation flag for the board driven sync
    
    float init_pos[3]; /// Initial position of the vehicle
};

/*!
 * Struct for the Inflow Threads
 */
struct InflowArg 
{
    int ThreadId; /// Thread id
    tpars params; /// Ptask Thread parameters
    GS_Interface* gs;   /// Pointer to the GroundStation Interface Class
    Autopilot_Interface* aut;   /// Pointer to the Autopilot Interface Class
};
/// Vector with the structures for the inflow threads
vector< struct InflowArg* > VectInflowArg;

/*!
 * Struct for the Simulation Threads
 */
struct SimThreadArg
{
    int ThreadId; /// Thread id
    tpars params; /// Ptask Thread parameters
    Autopilot_Interface* aut; /// Pointer to the Autopilot Interface Class
    Simulator_Interface* sim; /// Pointer to the Simulator Interface Class
};
/// Vector with the structure for the simualation threads
vector<struct SimThreadArg* > VectSimThreadArg;

/*!
 * Struct for the Ground Station Threads
 */
struct GsThreadArg
{
    int ThreadId; /// Thread id
    tpars params; /// Ptask Thread parameters
    MA_Manager* ma; /// Pointer to the MultiAgent Interface Class
    GS_Interface* gs; /// Pointer to the GroundStation Interface Class
};

/*!
 * Struct for the Unreal Engine Threads
 */
struct UeThreadArg
{
    int ThreadId; /// Thread id
    tpars params; /// Ptask Thread parameters
    UE_Interface* ue; /// Pointer to the UnrealEngine Interface Class
    Simulator_Interface* sim; /// Pointer to the Simulator Interface Class
    
    float pos_offset[3];
};
/// Vector with the structure for the simualation threads
vector<struct UeThreadArg* > VectUEThreadArg;

/**
 * Flags
 * 
 */
/*! Flag for the activation of the Ground 
 * Station Thread
 */
bool gs_thread_active = false;

/*! Flag for the activation of the Unreal 
 * Engine Thread
 */
bool ue_thread_active = false;


/** 
 * Time variables
 */
tspec sim_period; /*!< Period of the simulator thread */
tspec aut_period; /*!< Period of the inflow thread */
tspec gs_period;  /*!< Period of the ground station thread */
tspec ue_period;  /*!< Period of the unreal engine thread */


/**
 * File handler
 */
FILE *outfile; /// File for output



// -----------------------------------------------------------------------
//   Functions Prototypes
// -----------------------------------------------------------------------
int main(int argc, char **argv);


void routing_messages(mavlink_message_t *msg, struct InflowArg* p);

// Threads Bodies
//
/** 
 * Launch thread: it launch the inflow thread and the simulation thread for a given connected uav.
 */
void lauch_thread();

/** 
 * Inflow thread: It reads data from the uav.
 */
void inflow_thread();

/** 
 * Simulation thread: It simulates the dynamics of the vehicle using the actuation command 
 * and compute the value of the sensors. 
 */
void simulator_thread();

/** 
 * Ground station thread: Perform the data exchange between the ground station and the PC.
 */
void gs_thread();

/** 
 * Unreal Engine thread: Perform the data exchange between the ground station and the Unreal Engine
 * application.
 */
void ue_thread();

/** 
 * Quit handler
 */
void quit_handler( int sig );




