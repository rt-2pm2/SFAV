/**
 * \file: main.cpp
 *
 * \author: Luigi Pannocchi <l.pannocchi@sssup.it>
 */

#include <fstream>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "json/json.h"

#include "main.h"

int time_to_exit = 0;
const char *RT_MEMORY_ALLOCATION_ERROR = "memory allocation error";

struct HomePoint home;

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



/**
 * Convert relative position to lat/lot 
 */
void rel2latlon(struct HomePoint hp, float init_pos[3], float* lat, float* lon, float* alt)
{
    float latitude;
    float longitude;
    
    *alt = init_pos[2];
    
    latitude = hp.Latitude + init_pos[0] / 111111;
    longitude = hp.Longitude + init_pos[1] / (111111 * cos(latitude));
    
    *lat = latitude;
    *lon = longitude;
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
                        bool synch, float init_pos[3])
{
    int Agent_Id = -1; 
    Autopilot_Interface* pA;
    Simulator_Interface* pS;
    UE_Interface* pUe;
    
    float lat = 0.0;
    float lon = 0.0;
    float alt = 0.0;
    
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
    rel2latlon(home, init_pos, &lat, &lon, &alt);
    pS = sm->add_simulator(Agent_Id, lat, lon, dbg_ip, dbg_port);
    
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
    pUe = new UE_Interface(ue_ip, ue_r_port, ue_w_port, ue_v_port);
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
                        char*& uart_name, int baudrate, bool synch, float init_pos[3]))
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
    rel2latlon(home, init_pos, &lat, &lon, &alt);
    pS = sm->add_simulator(Agent_Id, lat, lon, dbg_ip, dbg_port);
    
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
    pUe = new UE_Interface(ue_ip, ue_r_port, ue_w_port, ue_v_port);
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
                   UE_Interface* ue, unsigned int portr, unsigned int portw, 
                   bool sync, char* ip_addr, float init_pos[3])
{
    int i;
    struct LaunchArg* larg = new (struct LaunchArg);
    
    larg->ma = ma; // Pointer to the Multiagent Manager Interface Class
    larg->sm = sm; // Pointer to the Simulators Manager Interface Class
    larg->gs = gs; // Pointer to the GroundStation Interface Class
    larg->ue = ue; // Pointer to the UnrealEngine Interface Class
    larg->params = TASK_SPEC_DFL;
    larg->params.act_flag = NOW;
    larg->params.arg = (void *) larg;
    larg->ip = ip_addr;
    larg->r_port = portr;
    larg->w_port = portw;
    larg->commType = UDP;
    larg->synch = sync;
    
    for (i = 0; i < 3; i++)
        larg->init_pos[i] = init_pos[i];
    
    ptask_create_param(lauch_thread, &larg->params);
}

void fill_launcher_serial(MA_Manager* ma, Sim_Manager* sm, GS_Interface* gs, 
                   UE_Interface* ue, int baudrate, bool sync, char* uart_name, 
                   float init_pos[3])
{
    int i;
    
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
    
    for (i = 0; i < 3; i++)
        larg->init_pos[i] = init_pos[i];
    
    ptask_create_param(lauch_thread, &larg->params);
}

/*
 * This function defines the variables for communicating with the agents and sets up the 
 * connection with each of them.
 */

int Init_Managers(std::ifstream* cfg, MA_Manager* ma, Sim_Manager* sm, 
                  GS_Interface* gs, UE_Interface* ue)
{
    Json::Value root;
    Json::CharReaderBuilder builder;
    JSONCPP_STRING parse_errs;
        
    std::string tmp_str;
    int tmp_int;
    struct in_addr* tmp_ip = nullptr; // Structure used for the check of the address format
    
    int N_UDP_vehicles = 0;
    int N_SERIAL_vehicles = 0;

    // Temp variables
    char ip_addr_uav[16];
    bool synch_udp;                 /// Vector containing the synchronization type flag
    unsigned int uav_port_r;        /// Vector containing the read port for communicating with UAV
    unsigned int uav_port_w;        /// Vector containing the write port for communicating with UAV
    char serial_dev_uav[20];        /// Vector containing the Serial Devices strings
    int uav_baud;                   /// Vector containing the baud rates of the serial interfaces
    bool synch_serial;              /// Vector containing the synchronization type flag

    builder["rejectDupKeys"] = true;

    bool parsingSuccessful =  parseFromStream(builder, *cfg, &root, &parse_errs);
    if ( !parsingSuccessful ) {
        // report to the user the failure and their locations in the document.
        printf("Failed to parse configuration.\n");
        return 1;
    }

    // Ground Station
    Json::Value js_gs = root["GS"];
    if (!js_gs.empty()) {
        printf("The configuration file includes a ground station.\n");

        // Dump GS object    
        //std::cout << "[INFO] " << gs << std::endl;

        std::string tmp_str;
        int tmp_pw, tmp_pr;
        
        // ToDo: Get without default
        tmp_str = js_gs.get("ConnectionType", "None").asString();
        printf("Groud Station - ConnectionType: %s \n", tmp_str.c_str());
        
        if (tmp_str == "UDP") {
            
            tmp_pw = js_gs.get("PortWrite", GS_WPORT).asInt();
            printf("Groud Station - PortWrite: ", tmp_pw);
            gs->setWritePort(tmp_pw);

            tmp_pr = js_gs.get("PortRead", GS_RPORT).asInt();
            printf("Groud Station - PortRead: ", tmp_pr);
            gs->setReadPort(tmp_pr);        

            tmp_str = js_gs.get("IPAddress", GS_IP).asString();
            if(inet_aton(tmp_str.c_str(), tmp_ip)) {    // The ip is valid 
                gs->setUDP((char *)tmp_str.c_str(), tmp_pr, tmp_pw);
                printf("Groud Station - IPAddress: %s \n", tmp_str.c_str());
            } else {
                printf("Failed to parse Ground Station IP address!\n");
                return 1;
            }
 
        } else {
            printf("Only UDP connection with Ground Station supported.");
            return 1;
        }
    }
    
    // Unreal Engine
    Json::Value js_ue = root["UE"];
    if (!js_ue.empty()) {
        printf("The configuration file includes an unreal engine.\n");

        // Dump UE object    
        //std::cout << "[INFO] " << ue << std::endl;

        std::string tmp_str;
        int tmp_int;
            
        // ToDo: Get without default!
        tmp_str = js_ue.get("ConnectionType", "UDP").asString();
        printf("Unreal Engine - ConnectionType: %s \n", tmp_str.c_str());
        
        if (tmp_str == "UDP") {
            
            tmp_str = js_ue.get("IPAddress", UE_IP).asString();
            if(inet_aton(tmp_str.c_str(), tmp_ip)) {    // The ip is valid 
                ue->setIP((char *)tmp_str.c_str());
                printf("Unreal Engine - IPAddress: %s \n", tmp_str.c_str());
            } else {
                printf("Failed to parse Ground Station IP address!\n");
                return 1;
            }

            tmp_int = js_ue.get("PortWriteBase", UE_WPORT).asInt();
            printf("Unreal Engine - PortWriteBase:  %d \n", tmp_int);
            ue->setBaseWritePort(tmp_int);

            tmp_int = js_ue.get("PortReadBase", UE_RPORT).asInt();
            printf("Unreal Engine - PortReadBase:  %d \n", tmp_int);
            ue->setBaseReadPort(tmp_int);        

            tmp_int = js_ue.get("PortVideoRead", UE_RPORT).asInt();
            printf("Unreal Engine - PortVideoBase:  %d \n", tmp_int);
            ue->setBaseVideoPort(tmp_int);        
 
        } else {
            printf("Only UDP connection with Ground Station supported.\n");
            return 1;
        }
    }
    
    // Vehicles
    Json::Value ve = root["Vehicle"];
    int nv = ve.size();
    //std::cout << "Number of Vehicles - " << nv << std::endl;    
    
    if (nv <= 0) {
        printf("The configuration file defines %d% vehicles, which is an invalid number.\n", nv);
        return 1;
    }
 
    int tmp_id;
    for ( int index = 0; index < nv; ++index ) {
        // Dump Vehicle
        //std::cout << "Vehicle[" << index << "]: " << ve[index] << std::endl;

        if(!ve[index].isMember("ID")) {
            printf("The configuration file includes a vehicle without ID.\n");
            return 1;
        } else if (!ve[index]["ID"].isNumeric()) {
            printf("The configuration file includes a vehicle with wrong ID.\n");
            return 1;
        } else {
            tmp_id = ve[index]["ID"].asInt();
            printf("Vehicle[%d] - ID: %d\n", index, tmp_id);
        }
        
        // ToDo: Not currently used!
        tmp_int = ve[index].get("TypeID", VH_TYPEID).asInt();
        printf("Vehicle[%d] - TypeID: %d\n", index, tmp_int);

        // ToDo: Not currently used!
        tmp_str = ve[index].get("IPAddressCamera", VH_IPCAM).asString();
        printf("Vehicle[%d] - IPAddressCamera: %s\n", index, tmp_str.c_str());
        
        if(ve[index].isMember("ConnectionType")) {
            std::string tmp_ct;
            tmp_ct = ve[index]["ConnectionType"].asString();
            //std::cout << "Vehicle[" << index << "] - ConnectionType: " << tmp_ct << std::endl;
                    
            if(tmp_ct == "UDP") {
            
                printf("Vehicle[%d] - ConnectionType: UDP\n", index);
                            
                tmp_str = ve[index].get("IPAddress", VH_IPADDR).asString();
                if(inet_aton(tmp_str.c_str(), tmp_ip)) {    // The ip is valid 
                    strcpy(ip_addr_uav, (char *)tmp_str.c_str());
                    printf("Vehicle[%d] - IPAddress: %s\n", index, tmp_str.c_str());
                } else {
                    printf("Failed to parse Ground Station IP address!\n");
                    return 1;
                }

                uav_port_w = ve[index].get("PortWrite", VH_PORTBASE_WRITE + ve[index]["ID"].asInt()*2).asInt();
                printf("Vehicle[%d] - PortWrite: %d\n", index, uav_port_w);

                uav_port_r = ve[index].get("PortRead", VH_PORTBASE_READ + ve[index]["ID"].asInt()*2).asInt();
                printf("Vehicle[%d] - PortRead: %d\n", index, uav_port_r);

                synch_udp = ve[index].get("Synch", VH_SYNCH).asInt();
                printf("Vehicle[%d] - Synch: %d\n", index, synch_udp);

                fill_launcher_udp(ma, sm, gs, ue, uav_port_r, uav_port_w, synch_udp, ip_addr_uav);   // All the parameters are read and we can add the vehicle
                N_UDP_vehicles++; 
                
            } else if(tmp_ct == "Serial") {
            
                printf("Vehicle[%d] - ConnectionType: Serial\n", index);

                tmp_str = ve[index].get("Device", VH_DEVICE).asString();
                strcpy(serial_dev_uav, (char *)tmp_str.c_str());
                printf("Vehicle[%d] - Device: %s\n", index, tmp_str.c_str());

                uav_baud = ve[index].get("Baudrate", VH_DEVICE_BAUDRATE).asInt();
                printf("Vehicle[%d] - Baudrate: %d\n", index, uav_baud);
                
                synch_serial = ve[index].get("Synch", VH_SYNCH).asInt();
                printf("Vehicle[%d] - Synch: %d\n", index, synch_serial);
                
                fill_launcher_serial(ma, sm, gs, ue, uav_baud, synch_serial, serial_dev_uav);
                N_SERIAL_vehicles++;
                
            } else {
                printf("The configuration file includes a vehicle with unknown ConnectionType.\n");
                return 1;
            }
        } else {
            printf("The configuration file includes a vehicle without ConnectionType.\n");
            return 1;
        }
    }
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
    
    // Check the number of parameters
    if (argc < 2) {
        printf("Usage: %s <config_file>.json \n", argv[0]);
        return 1;
    }

    std::string configfilename = argv[1];
    std::ifstream configfile;
    configfile.open(configfilename);
    if (!configfile.is_open()) {
        printf("Error while parsing configuration file %s \n", configfilename); 
        return 1;
    }

    // File to save data to
    //outfile = fopen("outfile.txt", "w");
       

    // -----------------------------
    //    INSTANTIATE CLASSES
    // -----------------------------
    home.Latitude = 43.8148386; 
    home.Longitude = 10.3192456; 
    home.Altitude = 0.0;
    
    /*
     * Instantiate a ground station interface object
     * This object handles the communication with the ground station.
     * The communication is accomplished over the UDP. 
     */
    GS_Interface gs_interface;

    /*
     * Instatiate the interface for the management of the 
     * Unreal Engine. The communication is accomplished over the UDP.
     */ 
    UE_Interface ue_interface;

    /*
     * Instatiate the interfaces for the management of the 
     * Multi Agent system.
     */ 
    MA_Manager ma_manager;   // Autopilot Interfaces
    Sim_Manager sim_manager; // Simulation Interfaces

    /*
     * Initialize the threads
     */
    Init_ThreadsEnvironment();

    /*
     * Parse the configuration file to get the information to initialize the managers
     */
    //parse_commandline(argc, argv, gs_ip, gs_r_port, gs_w_port, ue_ip, ue_r_port, ue_w_port);
    Init_Managers(&configfile, &ma_manager, &sim_manager, &gs_interface, &ue_interface);

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

    // Instantiate and start the Ground Station Thread
    GsThreadArg GSarg; 
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

    //fclose(outfile);
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
            add_agent_to_system(p->ma, p->sm, p->gs, p->ip, p->r_port, p->w_port, 
                                p->synch, p->init_pos);
            break;
        case SERIAL:
            add_agent_to_system(p->ma, p->sm, p->gs, p->uart_name, p->baudrate, p->synch, 
                                p->init_pos);
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
