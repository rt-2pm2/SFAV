/**
 * \file: main.cpp
 *
 * \author: Luigi Pannocchi <l.pannocchi@sssup.it>
 */

#include <fstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>

#include "main.h"

int time_to_exit = 0;
const char *RT_MEMORY_ALLOCATION_ERROR = "memory allocation error";


/* Start to retrieve data from the Autopilot Interface
 * Lock the resource autopilot_connected, then wait for the first heartbeat
 * before going on. It is necessary to wait because the heartbeat message
 * contains information about the current state of the vehicle, which is 
 * necessary when calling the "start_hil()" method.
*/
int start_inflow_thread(struct InflowArg* arg)
{
    int inflowT_id = 0;

    int id = 0;
    
    //printf("Creating the inflow_thread...\n");
    inflowT_id = ptask_create_param(inflow_thread, &arg->params);

    if (inflowT_id == -1)
    {
        printf("Inflow Thread not created!");
        return -1;
    }
    if (!arg->aut->isConnected())
    {
        //printf("Waiting for autopilot connection...\n");
        arg->aut->waitNextHeartbeat();
    }

    // Save the thread_id in the structure
    arg->ThreadId = inflowT_id;
    
    // Get the autopilot mavlink_id
    id = arg->aut->getSystemId();
    
    printf("Autopilot [%d] Connected!\n", id);
    
    // Add the UAV to the connection list of the Ground Station
    arg->gs->addUav(id);

    // Switch to HIL mode
    arg->aut->start_hil();

    return id;
}

int start_sim_thread(struct SimThreadArg* arg)
{
    int simT_id = 0;
    int id = arg->aut->getSystemId();
    printf("Creating the simuation_thread [%d] ....\n", id);
    simT_id = ptask_create_param(simulator_thread, &arg->params);
    if(simT_id == -1)
    { 
        printf("Error creating the simulator thread\n");
    }
    arg->ThreadId = simT_id;
    
    return simT_id;
}

int start_gs_thread(struct GsThreadArg* GSarg, class MA_Manager* ma_manager, class GS_Interface* gs_interface)
{
    int gsT_id;

    // Change the priority of the task related to the Ground Station
    GSarg->params = TASK_SPEC_DFL;
    GSarg->params.period = gs_period;
    GSarg->params.rdline = gs_period;
    GSarg->params.priority = GroundStation_Thread_Priority;
    GSarg->params.act_flag = NOW;
    GSarg->params.measure_flag = 0;
    GSarg->params.processor = 0;

    // Prepare the arg structure
    GSarg->gs = gs_interface;
    GSarg->ma = ma_manager;
    GSarg->params.arg = GSarg;
    
    printf("Creating the groundstation_thread....\n");
    gsT_id = ptask_create_param(gs_thread, &GSarg->params);
    
    if(gsT_id == -1)
    {
        printf("Error creating the Ground Station thread\n");
    }
    
    GSarg->ThreadId = gsT_id;
    
    return gsT_id;
}

int start_ue_thread(struct UeThreadArg* UEarg)
{
    int ueT_id = 0;
    
    ueT_id = ptask_create_param(ue_thread, &(UEarg->params));
    
    if(ueT_id == -1)
    {
        printf("Error creating the Unreal Engine thread\n");
    }
    UEarg->ThreadId = ueT_id;
    
    return ueT_id;
}


// add_agent_to_system
/* Add agent to system : 
 * 1) Instantiate: Autopilot interface class, Simulation interface class, UnrealEngine interface class 
 * 2) Start the Inflow thread to receive data from the board.
 * 3) Start the Simulation thread
 * 3) Start the VirtualEnvironment thread
 */
int add_agent_to_system(MA_Manager* ma, Sim_Manager* sm, GS_Interface* gs, 
                        char* ip, uint32_t r_port, uint32_t w_port,
                        bool synch)
{
    int Agent_Id = -1; 
    Autopilot_Interface* pA;
    Simulator_Interface* pS;
    UE_Interface* pUe;
    
    struct InflowArg* IArg;
    
    // 
    // 1) Add the UAV to the MultiAgent Manager class
    pA = ma->add_agent(ip, r_port, w_port, synch);
    
    //
    // 1b) Set up the arguments to be passed to the InflowThread
    // Structure to pass arguments to the thread
    
    IArg = new struct InflowArg;
    // Set the thread parameters
    IArg->ThreadId = -1;
    IArg->params = TASK_SPEC_DFL;
    IArg->params.period = aut_period;
    IArg->params.rdline = aut_period;
    IArg->params.priority = Inflow_Thread_Priority;
    IArg->params.act_flag = NOW;
    IArg->params.measure_flag = 0;
    IArg->params.processor = 0;
    
    // Load the pointers to the Interface Classes
    IArg->gs = gs;
    IArg->aut = pA;
    IArg->params.arg = (void *) IArg;

    // Start the InflowThread
    Agent_Id = start_inflow_thread(IArg);
    
    // Save the address of the allocated memory
    VectInflowArg.push_back(IArg);

    //
    // 2a) Add a Simulation_Interface class
    pS = sm->add_simulator(Agent_Id, 43.8148386, 10.3192456, dbg_ip, dbg_port);
    
    //
    // 2b) Set up the arguments to be passed to the SimulationThread
    // Structure to pass arguments to the thread
    struct SimThreadArg* SArg;
    SArg = new struct SimThreadArg;
    
    // Defining the Structure containing the thread Properties 
    SArg->ThreadId = -1;
    SArg->params = TASK_SPEC_DFL;
    SArg->params.period = sim_period;
    SArg->params.rdline = sim_period;
    SArg->params.priority = Simulation_Thread_Priority;
    SArg->params.act_flag = NOW;
    SArg->params.measure_flag = 0;
    SArg->params.processor = 0;
    SArg->params.arg = (void *) SArg;
    
    // Load the pointers to the Interface Classes
    SArg->aut = pA;
    SArg->sim = pS;
    
    // Save the address of the allocated memory
    VectSimThreadArg.push_back(SArg);

    start_sim_thread(SArg);
    
    //
    // 3a) Add a UnrealEngine interface class and set up the communication port
    pUe = new UE_Interface(ue_ip, ue_r_port, ue_w_port);
    pUe->add_port(Agent_Id);
    
    //
    // 3b) Set up the arguments to be passed to the UnrealThread
    // Structure to pass arguments to the thread
    struct UeThreadArg* UEArg;
    UEArg = new struct UeThreadArg;
    
    // Defining the Structure containing the thread Properties 
    UEArg->ThreadId = -1;
    UEArg->params = TASK_SPEC_DFL;
    UEArg->params.period = ue_period;
    UEArg->params.rdline = ue_period;
    UEArg->params.priority = UnrealEngine_Thread_Priority;
    UEArg->params.act_flag = NOW;
    UEArg->params.measure_flag = 0;
    UEArg->params.processor = 0;
    UEArg->params.arg = (void *) UEArg;
    
    // Load the pointers to the Interface Classes
    UEArg->ue = pUe;
    UEArg->sim = pS;
    
    // Save the address of the allocated memory
    VectUEThreadArg.push_back(UEArg);
    
    start_ue_thread(UEArg);
    
    return Agent_Id;
}




// add_agent_to_system
// Add agent to system : Instantiate a new Autopilot interface class and start the 
// Inflow thread to receive data from the board.
int add_agent_to_system(MA_Manager* ma, Sim_Manager* sm, GS_Interface* gs, 
                        char*& uart_name, int baudrate, bool synch)
{
    int Agent_Id = -1;    
    Autopilot_Interface* pA;
    Simulator_Interface* pS;
    UE_Interface*  pUe;
    
    struct InflowArg* IArg;
    
    // 
    // 1) Add the UAV to the MultiAgent Manager class
    pA = ma->add_agent(uart_name, baudrate, synch);
    
    //
    // 1b) Set up the arguments to be passed to the InflowThread
    // Structure to pass arguments to the thread
    IArg = new struct InflowArg;
    
    // Set the thread parameters
    IArg->ThreadId = -1;
    IArg->params = TASK_SPEC_DFL;
    IArg->params.period = aut_period;
    IArg->params.rdline = aut_period;
    IArg->params.priority = Inflow_Thread_Priority;
    IArg->params.act_flag = NOW;
    IArg->params.measure_flag = 0;
    IArg->params.processor = 0;
    
    // Load the pointers to the Interface Classes
    IArg->gs = gs;
    IArg->aut = pA;
    IArg->params.arg = (void *) IArg;
    
    // Put the Pointer to the Structure in the Vector and set the thread parameter to point to that
    VectInflowArg.push_back(IArg);
    
    // Start the InflowThread
    Agent_Id = start_inflow_thread(IArg);
    
    //
    // 2a) Add a Simulation_Interface class
    pS = sm->add_simulator(Agent_Id, INITIAL_COORDINATE_LAT, INITIAL_COORDINATE_LON, dbg_ip, dbg_port);
    
    //
    // 2b) Set up the arguments to be passed to the SimulationThread
    // Structure to pass arguments to the thread
    struct SimThreadArg* SArg;
    SArg = new struct SimThreadArg;
    
    // Defining the Structure containing the thread Properties 
    SArg->ThreadId = -1;
    SArg->params = TASK_SPEC_DFL;
    SArg->params.period = sim_period;
    SArg->params.rdline = sim_period;
    SArg->params.priority = Simulation_Thread_Priority;
    SArg->params.act_flag = NOW;
    SArg->params.measure_flag = 0;
    SArg->params.processor = 0;
    SArg->params.arg = (void *) SArg;

    // Load the pointers to the Interface Classes
    SArg->aut = pA;
    SArg->sim = pS;
    
    // Save the pointer to memory
    VectSimThreadArg.push_back(SArg);
    
    start_sim_thread(SArg);
    
    //
    // 3a) Add a communication port to the Synthetic Environment
    pUe = new UE_Interface(ue_ip, ue_r_port, ue_w_port);
    pUe->add_port(Agent_Id);
    
    //
    // 3b) Set up the arguments to be passed to the UnrealThread
    // Structure to pass arguments to the thread
    struct UeThreadArg* UEArg;
    UEArg = new struct UeThreadArg;
    
    // Defining the Structure containing the thread Properties 
    UEArg->ThreadId = -1;
    UEArg->params = TASK_SPEC_DFL;
    UEArg->params.period = sim_period;
    UEArg->params.rdline = sim_period;
    UEArg->params.priority = UnrealEngine_Thread_Priority;
    UEArg->params.act_flag = NOW;
    UEArg->params.measure_flag = 0;
    UEArg->params.processor = 0;
    UEArg->params.arg = (void *) UEArg;
    
    // Load the pointers to the Interface Classes
    UEArg->ue = pUe;
    UEArg->sim = pS;
    
    // Save the address of the allocated memory
    VectUEThreadArg.push_back(UEArg);
    
    start_ue_thread(UEArg);
    
    return Agent_Id;
}

/* 
 * This function launch the "launch thread", which will start the remaining threads 
 * involved in the data exchange with the single vehicle.
 */
void fill_launcher_udp(MA_Manager* ma, Sim_Manager* sm, GS_Interface* gs, 
                   UE_Interface* ue, unsigned int port, bool sync, char* ip_addr)
{
    struct LaunchArg* larg = new (struct LaunchArg);
    
    larg->ma = ma; // Pointer to the Multiagent Manager Interface Class
    larg->sm = sm; // Pointer to the Simulators Manager Interface Class
    larg->gs = gs; // Pointer to the GroundStation Interface Class
    larg->ue = ue; // Pointer to the UnrealEngine Interface Class
    larg->params = TASK_SPEC_DFL;
    larg->params.act_flag = NOW;
    larg->params.arg = (void *) larg;
    larg->ip = ip_addr;
    larg->r_port = port;
    larg->w_port = port + 1;
    larg->commType = UDP;
    larg->synch = sync;
    ptask_create_param(lauch_thread, &larg->params);
}

void fill_launcher_serial(MA_Manager* ma, Sim_Manager* sm, GS_Interface* gs, 
                   UE_Interface* ue, int baudrate, bool sync, char* uart_name)
{
    struct LaunchArg* larg = new (struct LaunchArg);
    
    larg->ma = ma; // Pointer to the Multiagent Manager Interface Class
    larg->sm = sm; // Pointer to the Simulators Manager Interface Class
    larg->gs = gs; // Pointer to the GroundStation Interface Class
    larg->ue = ue; // Pointer to the UnrealEngine Interface Class
    larg->params = TASK_SPEC_DFL;
    larg->params.act_flag = NOW;
    larg->params.arg = (void *) larg;
    larg->ip = NULL;
    larg->r_port = 0;
    larg->w_port = 0;
    larg->commType = SERIAL;
    larg->baudrate = baudrate;
    larg->uart_name = uart_name;
    larg->synch = sync;
    ptask_create_param(lauch_thread, &larg->params);
}

/*
 * This function defines the variables for communicating with the agents and sets up the 
 * connection with each of them.
 */
int Init_Managers(MA_Manager* ma, Sim_Manager* sm, GS_Interface* gs, UE_Interface* ue)
{
    int i, res, a;
    int addr_cntr;
    
    int N_UDP_vehicles = NCONNECTED_VEHICLES;
    int N_SERIAL_vehicles = NSERIAL_VEHICLES;
    
    int Nchars;
    int Nread;
    
    struct in_addr* inp = nullptr; // Structure used for the check of the address format
    
    int PortVal;
    
    Nchars = 30;
    char buffer[Nchars]; // Temp buffer
    
    
     // Create a input stream file object
    ifstream ifs("AutopilotAddressFile.txt", ifstream::in);
    
    
    // Read information
    addr_cntr = 0;
    while(ifs.good() && addr_cntr < 256)
    {
        // Reset the buffer
        for (i = 0; i < Nchars; i++)
            buffer[i] = 0;
        
        ifs.getline(buffer, Nchars);
        Nread = ifs.gcount();

        if (Nread > 0)
        {
            //Parsing the address
            res = sscanf(buffer, "Addr: %s", ip_addr_uav[addr_cntr]);
            if (res > 0) // The parsing was ok
            {
                if(inet_aton(ip_addr_uav[addr_cntr], inp)) // The ip is valid 
                    printf("Looking for an autopilot @ %s\n", ip_addr_uav[addr_cntr]);
                else
                    printf("Init_Managers: Error parsing the IP address!\n");
            }
            
            // Parsing the Port
            res =  sscanf(buffer, "Port: %d", &uav_port[addr_cntr]);
            if (res > 0)
                printf("At Port  %d\n", uav_port[addr_cntr]);
             
            // Parsing the Synchronization method
            res =  sscanf(buffer, "Synch: %d", &a);
            if (res > 0)
            {
                if (a > 0)
                {
                    synch[addr_cntr] = true;
                    printf("Synchronization ON\n");
                }
                else
                {
                    synch[addr_cntr] = false;
                    printf("Synchronization OFF\n");
                }
                    
                addr_cntr++; // All the parameters are read and we can add another vehicle
            }
        }
    
    }
	
	if (addr_cntr == 0)
        printf("Init_Managers: No vehicle were added!\n");
	
    ifs.close();
    
    // Allocating and setting up the structures
    for (i = 0; i < addr_cntr; i++)
    {
        fill_launcher_udp(ma, sm, gs, ue, uav_port[i], synch[i], ip_addr_uav[i]);
    }
    
    for (i = 0; i < N_SERIAL_vehicles; i++)
    {
        fill_launcher_serial(ma, sm, gs, ue, baudrate, false, uart_name);
    }
    
    return 0;
}
    
void Init_ThreadsEnvironment()
{
    ptask_init(SCHED_FIFO, GLOBAL, NO_PROTOCOL);
    
    // Setting periods
    aut_period = Inflow_Thread_Period;
    sim_period = Simulation_Thread_Period;
    gs_period = GroundStation_Thread_Period;
    ue_period = UnrealEngine_Thread_Period;

    tspec_init();
}


// *************************************************************************************************
// *************************************************************************************************
//                     MAIN
// *************************************************************************************************
// *************************************************************************************************
int main(int argc, char *argv[])
{
    int N_size, i;
    
    // File to save data to
    outfile = fopen("outfile.txt", "w");
    
    // Structure for the Ground Station Thread
    GsThreadArg GSarg;
    
    // Parse the Command Line to set information about communication
    // with external application
    parse_commandline(argc, argv, gs_ip, gs_r_port, gs_w_port, 
                                    ue_ip, ue_r_port, ue_w_port);

    // -----------------------------
    //    INSTANTIATE CLASSES
    // -----------------------------

    /*
     * Instantiate a ground station interface object
     * This object handles the communication with the ground station.
     * The communication is accomplished over the UDP. 
     */
    GS_Interface gs_interface(gs_ip, gs_r_port, gs_w_port);

    /*
     * Instatiate the interface for the management of the 
     * Unreal Engine. The communication is accomplished over the UDP.
     */ 
    UE_Interface ue_interface(ue_ip, ue_r_port, ue_w_port);

    /*
     * Instatiate the interfaces for the management of the 
     * Multi Agent system.
     */ 
    MA_Manager ma_manager;   // Autopilot Interfaces
    Sim_Manager sim_manager; // Simulation Interfaces

    // ======================================================================
    // Initialization
    // ======================================================================
    Init_ThreadsEnvironment();
    Init_Managers(&ma_manager, &sim_manager, &gs_interface, &ue_interface);


    /*
     * Setup interrupt signal handler
     *
     * Responds to early exits signaled with Ctrl-C.  The handler will 
     * command the UAV to return the standar operative mode, and close 
     * threads and the port.
     *
     */
    signal(SIGINT,quit_handler);

    //======================================================================
    //    THREADS  
    //======================================================================

    start_gs_thread(&GSarg, &ma_manager, &gs_interface);
    
    
    // Wait for the GS thread to finish
    pthread_join(ptask_get_threadid(gsT_id), 0);
    
    // Wait for the Inflow threads to finish
    N_size = VectInflowArg.size();
    for (i = 0; i < N_size; i++)
    {
        pthread_join(ptask_get_threadid(VectInflowArg.at(i)->ThreadId), 0);
    }
    
    // Wait for the simulation threads to finish
    N_size = VectSimThreadArg.size();
    for (i = 0; i < N_size; i++)
    {
        pthread_join(ptask_get_threadid(VectSimThreadArg.at(i)->ThreadId), 0);
    }
    
    // Wait for the Synthetic Environment threads to finish
    N_size = VectSimThreadArg.size();
    for (i = 0; i < N_size; i++)
    {
        pthread_join(ptask_get_threadid(VectUEThreadArg.at(i)->ThreadId), 0);
    }

    N_size = VectInflowArg.size();
    for (i = 0; i < N_size; i++)
    {
        delete VectInflowArg.at(i)->aut;
        delete VectInflowArg.at(i);
    }
    
    N_size = VectSimThreadArg.size();
    for (i = 0; i < N_size; i++)
    {
        delete VectSimThreadArg.at(i)->sim;
        delete VectSimThreadArg.at(i);
    }
    
    N_size = VectUEThreadArg.size();
    for (i = 0; i < N_size; i++)
    {
        delete VectUEThreadArg.at(i)->ue;
        delete VectUEThreadArg.at(i);
    }

    fclose(outfile);
    return 0;
}

// *************************************************************************************************
// *************************************************************************************************
// *************************************************************************************************
// *************************************************************************************************


// ----------------------------------------------------------------------
//    LAUNCH THREAD
// ----------------------------------------------------------------------
/*
 * This thread launches the inflow thread
 *
 */
void lauch_thread()
{
    struct LaunchArg* const p = (struct LaunchArg*)ptask_get_argument();

    //printf("Launching...\n");
    switch (p->commType)
    {
        case UDP:
            add_agent_to_system(p->ma, p->sm, p->gs, p->ip, p->r_port, p->w_port, p->synch);
            break;
        case SERIAL:
            add_agent_to_system(p->ma, p->sm, p->gs, p->uart_name, p->baudrate, p->synch);
            break;
            
        default:
            printf("Connection method not specified!\n");
            break;
    }
    delete p;
}





// ----------------------------------------------------------------------
//    INFLOW THREAD
// ----------------------------------------------------------------------
/*
 * This thread takes the information from the autopilot board 
 *
 */
void inflow_thread()
{
    struct InflowArg* const p = (struct InflowArg*)ptask_get_argument();

    int i, MessId;
    int NMessRead = 0;  // Number of messages read
    mavlink_message_t rec_message; // Current received mavlink message

    int tid = ptask_get_index();
    int system_id = p->aut->getSystemId();

    // The autopilot interface has been configured
    while (!time_to_exit)
    {
        // Access the Autopilot interface and obtain the 
        // number of read messages 
        NMessRead = p->aut->fetch_data();

        // If something has been found, then manage it:
        if (NMessRead > 0)
        {
            //printf("Autopilot[%d]::inflow_thread : Read %d messages\n", p->aut->getSystemId(), NMessRead);
            // Retrieve the messages
            for (i = 0; i < NMessRead; i++)
            {
                MessId = p->aut->get_message(&rec_message);
                if (MessId >= 0 && (p->aut->getSystemId() != -1))
                {
                    // Handle the message selecting the target 
                    routing_messages(&rec_message, p);
                }
            }
        }
        
    }
    p->aut->stop_hil();
    //delete p;
}

// -------------------------------------------------------
//  Function to manage the retrieved messages
//
//  Send messages to the GS or update the control signals
//
// -------------------------------------------------------
void routing_messages(mavlink_message_t *msg, struct InflowArg* const p)
{
    switch (msg->msgid) 
    {
        // OF NO INTEREST FOR THE GROUND STATION
        case MAVLINK_MSG_ID_HIL_CONTROLS:
            break;
		case MAVLINK_MSG_ID_HIL_ACTUATOR_CONTROLS:
            break;

        default:
        if (gs_thread_active)
            p->gs->pushMessage(msg);
            break;
    }
}


void extractSensors(struct SimOutput simout, struct SensorData_str* sensors)
{
    sensors->temperature = (float)simout.Temp;
    sensors->abs_pressure = (float)simout.Press;
    sensors->diff_pressure = (float)simout.diff_Pres;
    sensors->pressure_alt = (float)simout.Baro_Alt;

    sensors->xmag = (float)simout.Magn[0];
    sensors->ymag = (float)simout.Magn[1];
    sensors->zmag = (float)simout.Magn[2];

    sensors->xacc = (float)simout.Accelerometer[0];
    sensors->yacc = (float)simout.Accelerometer[1];
    sensors->zacc = (float)simout.Accelerometer[2];

    sensors->xgyro = (float)simout.Gyro[0];
    sensors->ygyro = (float)simout.Gyro[1];
    sensors->zgyro = (float)simout.Gyro[2];
    
    sensors->fields_updated = (uint32_t)0xFF;
}

void extractGpsData(struct SimOutput simout, struct GPSData_str* gps)
{
    gps->lat = (int32_t)(simout.Gps_Lat * 1e7);
    gps->lon = (int32_t)(simout.Gps_Lon * 1e7);
    gps->alt = (int32_t)(simout.Gps_Alt * 1e3);
    
    gps->vel = (uint16_t)(simout.Gps_V_Mod * 100); 
    
    gps->vn = (int16_t)(simout.Gps_V[0] * 100);
    gps->ve = (int16_t)(simout.Gps_V[1] * 100);
    gps->vd = (int16_t)(simout.Gps_V[2] * 100);
                     
    gps->cog = (int16_t)(simout.COG * 100);
    
    gps->satellites_visible = 8;
    
    gps->eph = 1;
    gps->epv = 1;
    
    gps->fix_type = 3;
}

// ----------------------------------------------------------------------
//    SIMULATION THREAD
// ----------------------------------------------------------------------
/* 
 *
 */
void simulator_thread()
{
    int ret_sens;
    int ret_gps;
    
    unsigned int scaler = 0;

    int tid = ptask_get_index();
    struct SimThreadArg* p = (struct SimThreadArg*)ptask_get_argument();
    
    uint8_t system_id = p->aut->getSystemId();
    uint8_t component_id = p->aut->getSystemId();

    mavlink_message_t sensor_msg, gps_msg; // Mavlink messages

    bool first = true;

    float pwm[NUM_FLOAT_ACT_CONTROLS];  // Structure to allocate simulation input
    struct SimOutput simout;            // Structure to allocate simualation output

    struct SensorData_str sensors;      // Structure to allocate simulated sensor data
    struct GPSData_str gps;             // Structure to allocate simulated gps data
    
    uint64_t   time_usec;
    
    ptime abs_deadlinetime;
    tspec abs_deadlinetime_tspec; 

    printf("***  Starting Simulator Thread [%d] ***\n", system_id);
    p->sim->simulationActive();
    
    ptime send_time = 0;
    ptime old_send_time = 0;
    ptime diff = 0;

    Udp_Port* ComPort = new Udp_Port();
    ComPort->InitializeOutputPort(dbg_ip, dbg_port + system_id);
    
    // Check the initialization of the necessary classes
    while (!time_to_exit)
    {
        /*
         * The synchronization part is still not implemented
         */
        if (first)
        {
            first = false;
            printf("Simulation Thread STARTED! \n");
        }

        // Get the Actuator Command
        time_usec = p->aut->getActuator(pwm);

        // Update the Inputs to the Simulator
        p->sim->updatePWM(pwm);

        // Simulation Step
        p->sim->simStep();

        // Get the Simulation Output
        p->sim->getSimOutput(&simout);
        
        // Send the state to the debug machine
        p->sim->DBGsendSimPosAtt();
        
        // Extract the Sensors and Gps Data
        extractSensors(simout, &sensors);
        extractGpsData(simout, &gps);
        

        // SEND
        if (p->aut->is_hil())
        {
            if (p->aut->getSynchActive())
            {
                send_time = ptask_gettime(MICRO);
                diff = send_time - p->aut->t_ctr;
                //printf("%ld \n", diff);
                //fprintf(outfile, "%ld \n", diff);
                
                // Sensors
                // Composition of the mavlink messages
                //  Sensors Message
                mavlink_msg_hil_sensor_pack(system_id, component_id, &sensor_msg, time_usec,
                                            sensors.xacc, sensors.yacc, sensors.zacc,
                                            sensors.xgyro, sensors.ygyro, sensors.zgyro,
                                            sensors.xmag, sensors.ymag, sensors.zmag,
                                            sensors.abs_pressure, sensors.diff_pressure,
                                            sensors.pressure_alt, sensors.temperature,
                                            sensors.fields_updated);
                ret_sens = p->aut->send_message(&sensor_msg);

                if ((send_time - old_send_time) > 100000)
                {
                    //  GPS Message
                    mavlink_msg_hil_gps_pack(system_id, component_id, &gps_msg,
                                             time_usec, gps.fix_type, gps.lat, gps.lon, gps.alt, gps.eph, gps.epv, gps.vel, gps.vn, gps.ve, gps.vd, gps.cog, gps.satellites_visible);
                    ret_gps = p->aut->send_message(&gps_msg);
                    old_send_time = send_time;
                }
            }
            else
            {
                if ((scaler % 2) == 0)
                {
                    // Sensors
                    mavlink_msg_hil_sensor_pack(system_id, component_id, &sensor_msg, time_usec,
                                            sensors.xacc, sensors.yacc, sensors.zacc,
                                            sensors.xgyro, sensors.ygyro, sensors.zgyro,
                                            sensors.xmag, sensors.ymag, sensors.zmag,
                                            sensors.abs_pressure, sensors.diff_pressure,
                                            sensors.pressure_alt, sensors.temperature,
                                            sensors.fields_updated);
                    ret_sens = p->aut->send_message(&sensor_msg);

                    // GPS
                    if ((scaler % 50) == 0)
                    {
                        //  GPS Message
                        mavlink_msg_hil_gps_pack(system_id, component_id, &gps_msg,
                                                 time_usec, gps.fix_type, gps.lat, gps.lon, gps.alt, gps.eph, gps.epv, gps.vel, gps.vn, gps.ve, gps.vd, gps.cog, gps.satellites_visible);
                        //printf("Sending GPS message to %d\n", system_id);
                        ret_gps = p->aut->send_message(&gps_msg);
                    }

                }
                if (ptask_deadline_miss())
                {
                    abs_deadlinetime = task_absdl(tid);
                    abs_deadlinetime_tspec = tspec_from(abs_deadlinetime, MILLI);

                    printf("Simulator Thread [%d] Deadline Miss!  Abs deadline = %ld [ms]|| Time = %ld [ms]\n",
                           system_id, tspec_to_rel(&abs_deadlinetime_tspec, MILLI), ptask_gettime(MILLI));
                }
                scaler++;
                ptask_wait_for_period();
            }
        }
    }
    delete ComPort;

}



// ----------------------------------------------------------------------
//    GROUND STATION THREAD
// ----------------------------------------------------------------------
/*
 * This thread takes the information from the simulator and ground 
 * station and directs them towards the autopilot. 
 *
 *                         
 *
 */
void gs_thread()
{
    printf("***  Starting Ground Station Thread  ***\n");

    int tid = ptask_get_index();
    struct GsThreadArg* p = (struct GsThreadArg*)ptask_get_argument();

    mavlink_message_t msg_message;

    int uavId;

    int first = 1;
    
    ptime abs_deadlinetime;
    tspec abs_deadlinetime_tspec;
    
    p->gs->setGSActive();
    gs_thread_active = true;

    // Check the initialization of the necessary classes
    while (!time_to_exit)
    {
        if (first)
        {
            first = false;
            printf("GS Thread STARTED! \n");
        }
        // Send all the pending data to the Ground Station
        p->gs->sendMessages();

        //Wait for data from the Ground Station
        p->gs->receiveMessages();

        // Send data to the Autopilot
        // We send data until the recQueue is empty
        while (p->gs->getDimrecQueue() > 0)
        {
            // Retrieve message from the Ground Station
            uavId = p->gs->getMessage(&msg_message);

            // Send it to the connected UAV
            p->ma->sendMessage2UAV(&msg_message, uavId);
        }
        
        if (ptask_deadline_miss())
        {
            abs_deadlinetime = task_absdl(tid);
            abs_deadlinetime_tspec = tspec_from(abs_deadlinetime, MILLI);
            
            printf("GroundStation Thread Deadline Miss!  Abs deadline = %ld [ms]|| Time = %ld [ms]\n", 
                   tspec_to_rel(&abs_deadlinetime_tspec, MILLI), ptask_gettime(MILLI));
        }
        ptask_wait_for_period();
	}
    //delete p;
}


// ----------------------------------------------------------------------
//    UNREAL ENGINE THREAD
// ----------------------------------------------------------------------
/*
 * This thread sends data to the Unreal Engine for visualization purposes 
 *
 *
 *    Simulator (Pose) >----> UE 
 *    Collisions   >----> Simulator                        
 *
 */
void ue_thread()
{
    int tid = ptask_get_index();
    
    int SysId;
    struct UE_SendData dataOut;
    Simulator_Interface* SimInt = NULL;
    struct UeThreadArg* p = (struct UeThreadArg*)ptask_get_argument();
     
    printf("***  Starting UE Communicator Thread  ***\n");

    float X[3]; /// Position
    float A[3]; /// Attitude
    
    float impnorm[3]; /// Impact versor
    float pen; /// Impact penetration
    
    int first = 1;
    
    ue_thread_active = true;

    // Check the initialization of the necessary classes
    while (!time_to_exit)
    {
        if (first)
        {
            first = false;
            printf("UE Thread STARTED! \n");
        }
        
        SimInt = p->sim;
        SysId = SimInt->getSystemId();
            

        dataOut.Id = SysId;
        SimInt->getSimPosAtt(X, A);
        dataOut.X = X[0];
        dataOut.Y = X[1];
        dataOut.Z = X[2];

        dataOut.r = A[0];
        dataOut.p = A[1];
        dataOut.y = A[2];
            
        // Send all the pending data to Unreal Engine
        p->ue->sendData(dataOut);

        //printf("Z = %3.2f\n", dataOut.Z);

        if(p->ue->receiveData())
        {
            // Read data from the Synthetic Environment
            p->ue->getCollision(impnorm, &pen);
        }
        else
        {
            impnorm[0] = 0.0;
            impnorm[1] = 0.0;
            impnorm[2] = 0.0;
            pen = 0.0;
        }
        SimInt->updateCollision(impnorm, pen);

    //ptask_wait_for_period();
    }
//delete p;
}




// ----------------------------------------------------------------------
//   Parse Command Line
// ----------------------------------------------------------------------
void parse_commandline(int argc, char **argv, char*& gs_ip, unsigned int& gs_r_port, unsigned int& gs_w_port,
                                            char *& ue_ip, unsigned int & ue_r_port, unsigned int& ue_w_port)
{
	// string for command line usage
	const char *commandline_usage = "usage: SFAV -gs_ip <GSip> -gs_rp <GSreadPort> -gs_wp <GSwritePort> \
                                                    -ue_ip <UEip> -ue_rp <UEreadPort> -ue_wp <UEwritePort> ";

	// Read input arguments
	for (int i = 1; i < argc; i++) { // argv[0] is the name of the executable

		// -----------------------------------
        // HELP
        // -----------------------------------
		if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			printf("%s\n",commandline_usage);
			throw EXIT_FAILURE;
		}

		// -----------------------------------
		// GROUND STATION SETTINGS
		// -----------------------------------
		// Ground Station Instance IP
		if (strcmp(argv[i], "-gs_ip") == 0) {
			if (argc > i + 1) {
				gs_ip = argv[i + 1];
			}
			else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}
		// From Ground Station Read Port
		if (strcmp(argv[i], "-gs_rp") == 0) {
			if (argc > i + 1) {
				gs_r_port = (unsigned int) atoi(argv[i + 1]);
			}
			else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}
		// To Ground Station Write Port
		if (strcmp(argv[i], "-gs_wp") == 0) {
			if (argc > i + 1) {
				gs_w_port = (unsigned int) atoi(argv[i + 1]);
			}
			else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}
        
        // -----------------------------------
		// UNREAL ENGINE VISUALIZATOR SETTINGS
		// -----------------------------------
		// Unreal Engine Visualizator Instance IP
		if (strcmp(argv[i], "-ue_ip") == 0) {
			if (argc > i + 1) {
				ue_ip = argv[i + 1];
			}
			else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}
        // From Unreal Engine Visualizator Start Read Port
		if (strcmp(argv[i], "-ue_rp") == 0) {
			if (argc > i + 1) {
				ue_r_port = (unsigned int) atoi(argv[i + 1]);
			}
			else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}
		// To Unreal Engine Visualizator Start Write Port
		if (strcmp(argv[i], "-ue_wp") == 0) {
			if (argc > i + 1) {
				ue_w_port = (unsigned int) atoi(argv[i + 1]);
			}
			else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}
	}
	// end: for each input argument
	// Done!
	return;
}


// ----------------------------------------------------------------------
//   Quit Signal Handler
// ----------------------------------------------------------------------
// this function is called when you press Ctrl-C
void quit_handler( int sig )
{
	printf("\n");
	printf("TERMINATING AT USER REQUEST\n");
	printf("\n");

	time_to_exit = 1;

	try {
		// Check if it is in HIL mode

	} 
	catch (int error){}


	// end program here
	exit(0);

}
