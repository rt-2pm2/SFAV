/**
 * \file: main.cpp
 *
 * \author: Luigi Pannocchi <l.pannocchi@sssup.it>
 */

#include "main.h"
#include "sfav_config.h"

int time_to_exit = 0;
const char *RT_MEMORY_ALLOCATION_ERROR = "memory allocation error";



int start_inflow_thread(struct InflowArg* arg)
{
    int inflowT_id = 0;

    int id = 0;
    // Start to retrieve data from the Autopilot Interface
    //
    // Lock the resource autopilot_connected, then wait for the first heartbeat
    // before going on. It is necessary to wait because the heartbeat message
    // contains information about the current state of the vehicle, which is 
    // necessary when calling the "start_hil()" method.
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
    
    return gsT_id;
}

int start_ue_thread(struct UeThreadArg* UEarg, Sim_Manager* sm, UE_Interface* ue)
{
    int ueT_id = 0;
    // Change the priority of the task related to the Unreal Engine
    UEarg->params = TASK_SPEC_DFL;
    UEarg->params.period = ue_period;
    UEarg->params.rdline = ue_period;
    UEarg->params.priority = UnrealEngine_Thread_Priority;
    UEarg->params.act_flag = NOW;
    UEarg->params.measure_flag = 0;
    UEarg->params.processor = 0;
    
    // Prepare the arg structure
    UEarg->sm = sm;
    UEarg->ue = ue;
    UEarg->params.arg = UEarg;
    
    ueT_id = ptask_create_param(ue_thread, &UEarg->params);
    
    if(ueT_id == -1)
    {
        printf("Error creating the Ground Station thread\n");
    }
    
    return ueT_id;
}


// add_agent_to_system
// Add agent to system : Instantiate a new Autopilot interface class and start the 
// Inflow thread to receive data from the board.
int add_agent_to_system(MA_Manager* ma, Sim_Manager* sm, 
                        GS_Interface* gs, UE_Interface* ue, 
                        char* ip, uint32_t r_port, uint32_t w_port,
                        bool synch)
{
    int Agent_Id = -1;    
    Autopilot_Interface* pA;
    Simulator_Interface* pS;
    
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
    IArg->params.priority = Inflow_Thread_Priority;// - (r_port - 4000);
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
    pS = sm->add_simulator(Agent_Id, 43.8148386 + 0.0001 * Agent_Id, 10.3192456 + 0.0001 * Agent_Id);
    
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
    SArg->params.priority = Simulation_Thread_Priority;// - (r_port - 4000);
    SArg->params.act_flag = NOW;
    SArg->params.measure_flag = 0;
    SArg->params.processor = 0;
    SArg->params.arg = (void *) SArg;
    
    // Load the pointers to the Interface Classes
    //SArg->aut = ma->getAutopilotInstance(Agent_Id);
    //SArg->sim = sm->getSimulatorInstance(Agent_Id);
    VectSimThreadArg.push_back(SArg);
    SArg->aut = pA;
    SArg->sim = pS;
    
    // start_sim_thread(VectSimThreadArg.back());
    start_sim_thread(SArg);
    
    //
    // 3) Add a communication port to the Synthetic Environment
    ue->add_port(Agent_Id);
    
    return Agent_Id;
}




// add_agent_to_system
// Add agent to system : Instantiate a new Autopilot interface class and start the 
// Inflow thread to receive data from the board.
int add_agent_to_system(MA_Manager* ma, Sim_Manager* sm, 
                        GS_Interface* gs, UE_Interface* ue, 
                        char*& uart_name, int baudrate, bool synch)
{
    int Agent_Id = -1;    
    Autopilot_Interface* pA;
    Simulator_Interface* pS;
    
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
    //VectInflowArg.push_back(IArg);
    
    // Start the InflowThread
    Agent_Id = start_inflow_thread(IArg);
    
    //
    // 2a) Add a Simulation_Interface class
    pS = sm->add_simulator(Agent_Id, 43.8148386, 10.3192456 + 0.001 * Agent_Id * (2-Agent_Id));
    
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
    //SArg->aut = ma->getAutopilotInstance(Agent_Id);
    //SArg->sim = sm->getSimulatorInstance(Agent_Id);
    //VectSimThreadArg.push_back(SArg);
    SArg->aut = pA;
    SArg->sim = pS;
    
    // start_sim_thread(VectSimThreadArg.back());
    start_sim_thread(SArg);
    
    //
    // 3) Add a communication port to the Synthetic Environment
    ue->add_port(Agent_Id);
    
    return Agent_Id;
}

void fill_launcher_udp(MA_Manager* ma, Sim_Manager* sm, GS_Interface* gs, 
                   UE_Interface* ue, unsigned int port, bool sync, char* ip_addr)
{
    struct LaunchArg* larg = new (struct LaunchArg);
    
    larg->ma = ma; /// Pointer to the Multiagent Manager Interface Class
    larg->sm = sm; /// Pointer to the Simulators Manager Interface Class
    larg->gs = gs; /// Pointer to the GroundStation Interface Class
    larg->ue = ue; /// Pointer to the UnrealEngine Interface Class
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
    
    larg->ma = ma; /// Pointer to the Multiagent Manager Interface Class
    larg->sm = sm; /// Pointer to the Simulators Manager Interface Class
    larg->gs = gs; /// Pointer to the GroundStation Interface Class
    larg->ue = ue; /// Pointer to the UnrealEngine Interface Class
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

int Init_Managers(MA_Manager* ma, Sim_Manager* sm, GS_Interface* gs, UE_Interface* ue)
{
    int i;
    int N_UDP_vehicles = 19;
    
    // Load the IPs and other dat...
    ip_addr_uav[0] = (char *)"192.168.1.2";
    uav_port[0] = 4000;
    synch[0] = false;

    ip_addr_uav[1]= (char *)"192.168.1.3";
    uav_port[1] = 4002;
    synch[1] = false;

    ip_addr_uav[2] = (char *)"10.30.3.24";
    uav_port[2] = 4004;
    synch[2] = false;

    ip_addr_uav[3] = (char *)"10.30.3.24";
    uav_port[3] = 4006;
    synch[3] = false;

    ip_addr_uav[4] = (char *)"10.30.3.24";
    uav_port[4] = 4008;
    synch[4] = false;

    ip_addr_uav[5] = (char *)"10.30.3.24";
    uav_port[5] = 4010;
    synch[5] = false;

    ip_addr_uav[6] = (char *)"10.30.3.24";
    uav_port[6] = 4012;
    synch[6] = false;

    ip_addr_uav[7] = (char *)"10.30.3.24";
    uav_port[7] = 4014;
    synch[7] = false;

    ip_addr_uav[8] = (char *)"10.30.3.24";
    uav_port[8] = 4016;
    synch[8] = false;

    ip_addr_uav[9] = (char *)"10.30.3.24";
    uav_port[9] = 4018;
    synch[9] = false;

    ip_addr_uav[10] = (char *)"10.30.3.24";
    uav_port[10] = 4020;
    synch[10] = false;

    ip_addr_uav[11] = (char *)"10.30.3.24";
    uav_port[11] = 4022;
    synch[11] = false;

    ip_addr_uav[12] = (char *)"10.30.3.24";
    uav_port[12] = 4024;
    synch[12] = false;

    ip_addr_uav[13] = (char *)"10.30.3.24";
    uav_port[13] = 4026;
    synch[13] = false;
    
    ip_addr_uav[14] = (char *)"10.30.3.24";
    uav_port[14] = 4028;
    synch[14] = false;
    
    ip_addr_uav[15] = (char *)"10.30.3.24";
    uav_port[15] = 4030;
    synch[15] = false;
    
    ip_addr_uav[16] = (char *)"10.30.3.24";
    uav_port[16] = 4032;
    synch[16] = false;
    
    ip_addr_uav[17] = (char *)"10.30.3.24";
    uav_port[17] = 4034;
    synch[17] = false;
    
    ip_addr_uav[18] = (char *)"10.30.3.24";
    uav_port[18] = 4036;
    synch[18] = false;
    
    ip_addr_uav[19] = (char *)"10.30.3.24";
    uav_port[19] = 4038;
    synch[19] = false;
    
    // Allocating and setting up the structures
    for (i = 0; i < N_UDP_vehicles; i++)
    {
        fill_launcher_udp(ma, sm, gs, ue, uav_port[i], synch[i], ip_addr_uav[i]);
    }
    
    
    //fill_launcher_serial(ma, sm, gs, ue, baudrate, false, uart_name);

    return 0;
}
    
void Init_ThreadsEnvironment()
{
    //ptask_init(SCHED_OTHER, GLOBAL, NO_PROTOCOL);
    ptask_init(SCHED_FIFO, GLOBAL, NO_PROTOCOL);
    //ptask_init(SCHED_FIFO, PARTITIONED, NO_PROTOCOL);
    
    // Setting periods
    aut_period = Inflow_Thread_Period;
    sim_period = Simulation_Thread_Period;
    gs_period = GroundStation_Thread_Period;
    ue_period = UnrealEngine_Thread_Period;

    tspec_init();
}


// ***********************************************************************
// ***********************************************************************
//                     MAIN
// ***********************************************************************
// ***********************************************************************
int main(int argc, char *argv[])
{
    int N_size, i;
    
    outfile = fopen("outfile.txt", "w");
    
    // Structure for the Ground Station Thread
    GsThreadArg GSarg;
    
    // Structure for the UE Thread
    UeThreadArg UEarg;
    
    // Parse the Command Line to set information about communication
    // with external application
    parse_commandline(argc, argv, gs_ip, gs_r_port, gs_w_port, 
                                    ue_ip, ue_r_port, ue_w_port);

    // -----------------------------
    //    INSTANTIATE CLASSES
    // -----------------------------

    /*
     * Instantiate an ground station interface object
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

    gsT_id = start_gs_thread(&GSarg, &ma_manager, &gs_interface);
    //start_ue_thread(&UEarg, &sim_ma
    
    /*
    for(;;)
    {
        usleep(500000);
    }
    */
    
//     N_size = VectInflowArg.size();
//     for (i = 0; i < N_size; i++)
//     {
//         pthread_join(ptask_get_threadid(VectInflowArg.at(i)->ThreadId), 0);
//     }
//     
//     N_size = VectSimThreadArg.size();
//     for (i = 0; i < N_size; i++)
//     {
//         pthread_join(ptask_get_threadid(VectSimThreadArg.at(i)->ThreadId), 0);
//     }
    
    pthread_join(ptask_get_threadid(gsT_id), 0);
    fclose(outfile);
    return 0;
}

// ***********************************************************************
// ***********************************************************************
// ***********************************************************************
// ***********************************************************************


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
            add_agent_to_system(p->ma, p->sm, p->gs, p->ue, p->ip, p->r_port, p->w_port, p->synch);
            break;
        case SERIAL:
            add_agent_to_system(p->ma, p->sm, p->gs, p->ue, p->uart_name, p->baudrate, p->synch);
            break;
            
        default:
            printf("Connection method not specified!\n");
            break;
    }
    
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
    //int system_id = p->aut->getSystemId();
    
    //ptime abs_deadlinetime;
    //tspec abs_deadlinetime_tspec; 

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
        
//         if (ptask_deadline_miss())
//         {
//             abs_deadlinetime = task_absdl(tid);
//             abs_deadlinetime_tspec = tspec_from(abs_deadlinetime, MILLI);
//             
//             printf("Inflow Thread [%d] Deadline Miss!  Abs deadline = %ld [ms]|| Time = %ld [ms]\n", 
//                    system_id, tspec_to_rel(&abs_deadlinetime_tspec, MILLI), ptask_gettime(MILLI));
//         }
//         ptask_wait_for_period();
        
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
    ptime diff = 0;

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

        // Extract the Sensors and Gps Data
        extractSensors(simout, &sensors);
        extractGpsData(simout, &gps);
        
        // Composition of the mavlink messages  
        //  Sensors Message
        mavlink_msg_hil_sensor_pack(system_id, component_id, &sensor_msg, time_usec, 
                sensors.xacc, sensors.yacc, sensors.zacc,
                sensors.xgyro, sensors.ygyro, sensors.zgyro,
                sensors.xmag, sensors.ymag, sensors.zmag,
                sensors.abs_pressure, sensors.diff_pressure,
                sensors.pressure_alt, sensors.temperature,
                sensors.fields_updated);
        //  GPS Message
        mavlink_msg_hil_gps_pack(system_id, component_id, &gps_msg,
                time_usec, gps.fix_type, gps.lat, gps.lon, gps.alt, gps.eph, gps.epv,
                gps.vel, gps.vn, gps.ve, gps.vd, gps.cog, gps.satellites_visible);

        // SEND
        if (p->aut->is_hil())
        {
//             if (p->aut->getSystemId() == 1)
//                 printf("Here\n");
            if (p->aut->getSynchActive())
            {
                send_time = ptask_gettime(MICRO);
                diff = send_time - p->aut->t_ctr;
                //printf("%ld \n", diff);
                fprintf(outfile, "%ld \n", diff);
                // Sensors
                ret_sens = p->aut->send_message(&sensor_msg);
                //ret_gps = p->aut->send_message(&gps_msg);
            }
            else
            {
                if ((scaler % 2) == 0)
                {
                    // Sensors
                    ret_sens = p->aut->send_message(&sensor_msg);

                    // GPS
                    if ((scaler % 32) == 0)
                    {
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
    //delete p;

}



// ----------------------------------------------------------------------
//    GROUND STATION THREAD
// ----------------------------------------------------------------------
/*
 * This thread takes the information from the simulator and ground 
 * station and directs them towards the PX4. 
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
 *    Drone (Pose) >----> UE 
 *    Collisions   >----> Drone                        
 *
 */
void ue_thread()
{
    int i, Ns;
    int SysId;
    int port;
	printf("***  Starting UE Communicator Thread  ***\n");

	struct UE_SendData dataOut;
	struct UE_RecData dataIn;
    
    Simulator_Interface* SimInt = NULL;
	
	int tid = ptask_get_index();
	struct UeThreadArg* p = (struct UeThreadArg*)ptask_get_argument();
		
	dataOut.Id = 1;
    
    float X[3]; /// Position
    float A[3]; /// Attitude
    
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
		
		Ns = p->sm->getNumberOfSimulator();
		for (i = 0; i < Ns; i++)
        {
            SysId = p->sm->getSimulatorId(i);
            SimInt = p->sm->getSimulatorInstance(SysId);
            
            dataOut.Id = SysId;
            SimInt->getSimPosAtt(X,A);
            dataOut.X = X[0];
            dataOut.Y = X[1];
            dataOut.Z = X[2];
	
            dataOut.r = A[0];
            dataOut.p = A[1];
            dataOut.y = A[2];

            p->ue->setData(dataOut);
            // Send all the pending data to Unreal Engine
            p->ue->sendData(SysId);
        }
		//printf("Z = %3.2f\n", dataOut.Z);
		/*
		if(p->ue->receiveData())
		{	
            // Read data from the Synthetic Environment
			p->ue->getData(&dataIn);
			printf("Nx = %1.2f | Ny = %1.2f | Nz = %1.2f\n", dataIn.Nx, dataIn.Ny, dataIn.Nz);
			printf("Reaction = [%2.2f | %2.2f | %2.2f]\n\n", 
					DynModel_Y.Freact[0], DynModel_Y.Freact[1], DynModel_Y.Freact[2]);
            
            
			DynModel_U.n_collision[0] = dataIn.Nx;
			DynModel_U.n_collision[1] = dataIn.Ny;
			DynModel_U.n_collision[2] = - dataIn.Nz;
			DynModel_U.pen_collision = 1e-2 * dataIn.PenDepth;	
		}
		else
		{
			DynModel_U.n_collision[0] = 0.0;
			DynModel_U.n_collision[1] = 0.0;
			DynModel_U.n_collision[2] = 0.0;
			DynModel_U.pen_collision = 0.0; 

		}
		*/
        ptask_wait_for_period();
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
	const char *commandline_usage = "usage: routing -gs_ip <GSip> -gs_rd <GSreadPort> -gs_wr <GSwritePort> \
                                                    -ue_ip <UEip> -ue_rd <UEreadPort> -ue_wr <UEwritePort> ";

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
        // From Unreal Engine Visualizator Read Port
		if (strcmp(argv[i], "-ue_rp") == 0) {
			if (argc > i + 1) {
				ue_r_port = (unsigned int) atoi(argv[i + 1]);
			}
			else {
				printf("%s\n",commandline_usage);
				throw EXIT_FAILURE;
			}
		}
		// To Unreal Engine Visualizator Write Port
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
