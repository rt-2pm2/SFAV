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


// -----------------------------------------------------------------------
//   Prototypes
// -----------------------------------------------------------------------
int main(int argc, char **argv);


/** This function parse the option used to launch the application
 * throws EXIT_FAILURE if could not open the port
 * Inputs
 * \param gs_ip    Ground Station IP <"x.x.x.x">
 * \param gs_r_port Port number for reading incoming data from GS <"x">
 * \param gs_w_port Port number for writing outgoing data to GS <"x">
 * \param ue_ip    UnrealEngine Visualizator IP <"x.x.x.x">
 * \param ue_r_port number for reading incoming data from Visualizator <"x">
 * \param ue_w_port Port number for writing outgoing data to Visualizator <"x">
 */
void parse_commandline(int argc, char **argv, char *&gs_ip, unsigned int &gs_r_port, unsigned int &gs_w_port, 
                        char *& ue_ip, unsigned int & ue_r_port, unsigned int& ue_w_port);

void routing_messages(mavlink_message_t *msg, struct InflowArg* p);

// Threads Bodies
//
void lauch_thread();
void inflow_thread();
void simulator_thread();
void gs_thread();
void ue_thread();

// Threads Indexes
int gsT_id;
int ueT_id;

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
    UE_Interface* ue; /// Pointer to the UnrealEngine Interface Class
    
    CommInterfaceType commType; /// Connection type (UDP, SERIAL)
    char* ip; /// Ip Address of the vehicle
    uint32_t r_port; /// Reading port 
    uint32_t w_port; /// Writing port
    int baudrate;   /// Serial port baudrate
    char* uart_name; /// Serial port device
    bool synch; /// Activation flag for the board driven sync
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
    tpars params; /// Ptask Thread parameters
    MA_Manager* ma; /// Pointer to the MultiAgent Interface Class
    GS_Interface* gs; /// Pointer to the GroundStation Interface Class
};

/*!
 * Struct for the Unreal Engine Threads
 */
struct UeThreadArg
{
    tpars params; /// Ptask Thread parameters
    UE_Interface* ue; /// Pointer to the UnrealEngine Interface Class
    Sim_Manager* sm; /// Pointer to the Simulators Manager Interface Class
};

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
FILE *outfile;

/** 
 * Quit handler
 */
void quit_handler( int sig );




