/**
 * \file autopilot_interface.cpp
 *
 * \brief Autopilot interface functions
 *
 * Functions for sending and recieving commands to an autopilot 
 * via MAVlink
 *
 * \author Luigi Pannocchi, <l.pannocchi@gmail.com>
 */


// ---------------------------------------------------------------------
//   Includes
// ---------------------------------------------------------------------

#include "autopilot_interface.h"

#define AUT_INTERFACE_DBG 0 



// ---------------------------------------------------------------------
//   Autopilot Interface Class
// ---------------------------------------------------------------------

// ---------------------------------------------------------------------
//   Con/Destructors
// ---------------------------------------------------------------------
Autopilot_Interface::Autopilot_Interface()
{
    initialize_class(false);
}

void Autopilot_Interface::setSystemId(int id)
{
    system_id    = id; // system id
    autopilot_id = id; // autopilot component id   
    
    current_messages.sysid  = system_id;
    current_messages.compid = autopilot_id;
}


void Autopilot_Interface::initialize_class(bool synch)
{
    int i;
    control_status = 0;
    
    synch_active = synch;
    synchronized = false;
    
    Connected = false;
    setSystemId(-1);
    
    companion_id = 255; // companion computer component id
    system_status = 0;
    mav_type = 2;

    base_mode = 0;
    custom_mode = 0;

    control_status = 0;    
    updated_heartbeat = false;

    hil_mode = false;

    // Controls
    pthread_mutex_init(&mut_controls, NULL);
    pthread_cond_init(&cond_synch, NULL);
    
    // Heartbeat / Connection
    pthread_mutex_init(&mut_heartbeat, NULL);
    pthread_cond_init(&cond_heartbeat, NULL);

    for (i = 0; i < 4; i++)
        hil_ctr[i] = 0.0;

    hil_ctr_time = 0;

    serial_port = NULL;
    udp_port = NULL;
}

Autopilot_Interface::Autopilot_Interface(char *&uart_name_, int baudrate, bool synch)
{
    //printf("Creating Autopilot Interface [SERIAL]\n");
    // Call the default Constructor
    initialize_class(synch);
    
    // Set the System Id
    //setSystemId(-1);
    
    commType = SERIAL;
    serial_port = new Serial_Port(uart_name_, baudrate);
    
    // Initialize the structure for the poll() 
    fdsR[0].fd = serial_port->fd;
    fdsR[0].events = POLLIN;

    fdsW[0].fd = serial_port->fd;
    fdsW[0].fd = POLLOUT;
}

Autopilot_Interface::Autopilot_Interface(char* ip, uint32_t r_port, uint32_t w_port, bool synch)
{
    //printf("Creating Autopilot Interface [UDP]\n");
    // Call the default Constructor
    initialize_class(synch);
    
    // Set the System Id
    //setSystemId(-1);
    
    commType = UDP;
    udp_port = new Udp_Port(ip, r_port, w_port);
    
    // Initialize the structure for the poll() 
    fdsR[0].fd = udp_port->sock;
    fdsR[0].events = POLLIN;

    fdsW[0].fd = udp_port->sock;
    fdsW[0].fd = POLLOUT;
}

Autopilot_Interface::~Autopilot_Interface() 
{
    if (udp_port)
        delete udp_port;
    if (serial_port)
        delete serial_port;
    
    printf("DESTRUCTOR\n");
}

// -------------------------------------------------------- 
//  FUNCTIONS
// --------------------------------------------------------

void Autopilot_Interface::unpack_ctr_mess(mavlink_message_t msg)
{   
	float controls[16];
    pthread_mutex_lock(&mut_controls);
	switch (msg.msgid)
	{
	case MAVLINK_MSG_ID_HIL_CONTROLS:
		hil_ctr_time = mavlink_msg_hil_controls_get_time_usec(&msg);
    	hil_ctr[0] = mavlink_msg_hil_controls_get_roll_ailerons(&msg);
    	hil_ctr[1] = mavlink_msg_hil_controls_get_pitch_elevator(&msg);
    	hil_ctr[2] = mavlink_msg_hil_controls_get_yaw_rudder(&msg);
    	hil_ctr[3] = mavlink_msg_hil_controls_get_throttle(&msg);
		break;
	case MAVLINK_MSG_ID_HIL_ACTUATOR_CONTROLS:
		mavlink_msg_hil_actuator_controls_get_controls(&msg, controls);
		hil_ctr_time = mavlink_msg_hil_actuator_controls_get_time_usec(&msg);
		//hil_ctr[0] = controls[1];
		//hil_ctr[1] = controls[3];
		//hil_ctr[2] = controls[0];
		//hil_ctr[3] = controls[2];
        hil_ctr[0] = (float)controls[0];
		hil_ctr[1] = (float)controls[1];
		hil_ctr[2] = (float)controls[2];
		hil_ctr[3] = (float)controls[3];
		break;
	default:
		printf("Control Message not recognized!\n");
		break;
	}
	
	if (synch_active)
    {
        t_ctr = ptask_gettime(MICRO);
        synchronized = true;
        pthread_cond_signal(&cond_synch);
    }
    
    pthread_mutex_unlock(&mut_controls);
}


bool Autopilot_Interface::isConnected()
{
    bool res;
    pthread_mutex_lock(&mut_heartbeat);
    res = Connected;
    pthread_mutex_unlock(&mut_heartbeat);
    return res;
}

//
// readBytes
//
int Autopilot_Interface::readBytes(char* rbuff, unsigned int NBytes)
{
    int nread = 0;

    if (commType == UDP)
    {
        nread = udp_port->readBytes(rbuff, NBytes);
    }
    else
    {
        nread = serial_port->readBytes(rbuff, NBytes);
    }
    
    return nread;
}

//
// writeBytes
//
int Autopilot_Interface::writeBytes(char * data, unsigned int len)
{
    int writtenB = 0;
    
    if (commType == UDP)
        writtenB = udp_port->writeBytes(data, len);
    else
        writtenB = serial_port->writeBytes(data, len);
    
    return writtenB;
}

//
// Fetch Data
//
int Autopilot_Interface::fetch_data()
{
    //printf("Autopilot_Interface::fetch_message() \n");
    int i = 0;
    int ret = 0;
    int nread = 0;
    int timeout = 200;
    
    // Allocate Space for the Reading Buffer
    unsigned int NBytes = 256;
    char rbuff[NBytes];

    // Allocate Mavlink message variables
    mavlink_status_t status;
    mavlink_message_t recMessage;

    // Flag for the Message reception
    int msgReceived = 0; 

    // Number of messages parsed in read operation
    int NMessages = 0;

    // Id of the received message
    int message_Id = -1;
     
    ret = poll(fdsR, 1, timeout);
    if (ret > 0)
    {
        // Until I don't receive a message...
        while (!msgReceived)
        {
            nread = readBytes(rbuff, NBytes);

            if (nread > 255)
                printf("[Autopilot_Interface[%d]::fetch_message()] \
                    Receiving buffer full\n", system_id);


            // Check for errors
            if (nread < 0)
            {
                //printf("Autopilot_Interface::read_message : ERROR IN READING UART\n");
                return -1;
            }

            // No data on the device
            if (nread == 0)
            {
                return 0;
            }

            // Seek for mavlink messages in the received data stream
            for (i = 0; i < nread; i++)
            {
                // Parse 1 byte at time
                if (mavlink_parse_char(system_Index, rbuff[i], &recMessage, &status))
                {
                    // Handle the message and save in the Stock Structure
                    message_Id = handle_message(recMessage);

                    NMessages++;
                    // Set the flag to 1 to signal that a full message has been retrieved
                    msgReceived = 1;
                }
            }
        }
    }

    /* Return the number of read messages */
    return NMessages;
}


void Autopilot_Interface::update_vehicle_status(mavlink_heartbeat_t heartbeat)
{
    base_mode = heartbeat.base_mode;
    custom_mode = heartbeat.custom_mode;
    mav_type = heartbeat.type;
    system_status = heartbeat.system_status;
    custom_mode = heartbeat.custom_mode;
}

void Autopilot_Interface::waitNextHeartbeat()
{
    pthread_mutex_lock(&mut_heartbeat);

    while (Connected == false)
        pthread_cond_wait(&cond_heartbeat, &mut_heartbeat);

    pthread_mutex_unlock(&mut_heartbeat);
}

//
//  handle_message 
//
// In the Autopilot interface the handling consists in stocking the data into 
// queues for a future retrieval. 
//
int Autopilot_Interface::handle_message(mavlink_message_t message)
{
    int message_id = message.msgid;
    // Push the message in the queue
    current_messages.messages.push(message);
    
    long int ctr_time;
    static long int old_ctr_time = 0;

    //printf("Number of messages in the queue %d\n",current_messages.messages.size());

    // Record the time
    //current_messages.time_stamps[message_id] = ptask_gettime(MICRO);

    // Handle Message ID
    switch (message_id)
    {
        case MAVLINK_MSG_ID_HEARTBEAT:
        {
            //printf("MAVLINK_MSG_HEARTBEAT\n");
            mavlink_heartbeat_t heartbeat;
            mavlink_msg_heartbeat_decode(&message, &heartbeat);

            pthread_mutex_lock(&mut_heartbeat);
            updated_heartbeat = true;
            Connected = true;
            setSystemId(message.sysid);
            pthread_cond_broadcast(&cond_heartbeat);
            pthread_mutex_unlock(&mut_heartbeat);

            update_vehicle_status(heartbeat);
        }
        break;
    
        case MAVLINK_MSG_ID_HIL_CONTROLS:
        {
            //printf("MAVLINK_MSG_ID_HIL_CONTROLS\n");
            unpack_ctr_mess(message);
        }
        break;

        case MAVLINK_MSG_ID_HIL_ACTUATOR_CONTROLS:
        {
            //printf("MAVLINK_MSG_ID_HIL_ACTUATOR_CONTROLS\n");
            unpack_ctr_mess(message);
        }

        default:
        break;
    }

    //printf("Message ID from AUV : %d\n", message_id);
    return message_id;
}

// 
// get_message
//
int Autopilot_Interface::get_message(mavlink_message_t* rqmsg)
{
    mavlink_message_t message;
    // Extract the message from the front of the queue
    if (current_messages.messages.size() > 0)
    {
        message = current_messages.messages.front();
        current_messages.messages.pop();
    
        //printf("Number of messages in the queue %d\n",current_messages.messages.size());
        *rqmsg = message;
        return message.msgid;
    }
    else
    {
        printf("[Autopilot_Interface::get_message] Empty message queue!\n");
        return -1;
    }
}


//
// send_message
//
int Autopilot_Interface::send_message(mavlink_message_t* message)
{
    uint16_t len = 0;
    int writtenB = 0;
    
    char buf[300];
    len = mavlink_msg_to_send_buffer((uint8_t *)buf, message);

    // Write buffer to serial port, locks port while writing
    writtenB = writeBytes(buf, len);
    
    if (len != writtenB)
    {
        //printf("Autopilot_Interface::write_message: ERROR WHILE WRITING \n %d instead of %d \n\n", writtenB, len); 
        return -1;
    }
    // Done!
    return writtenB;
}


void Autopilot_Interface::sendSensorData(float xacc, float yacc, float zacc, 
                                         float xgyro, float ygyro, float zgyro, 
                                         float xmag, float ymag, float zmag, 
                                         float abs_pressure, float diff_pressure, float pressure_alt,
                                         float temperature)
{
    // Compose the Mavlinkg sensor message 
    mavlink_message_t msg;
    mavlink_hil_sensor_t sensors;
    ptime timestamp = ptask_gettime(MICRO);
    sensors.time_usec = timestamp; 
    sensors.xacc = xacc; 
    sensors.yacc = yacc; 
    sensors.zacc = zacc;
    sensors.xgyro = xgyro;
    sensors.ygyro = ygyro;
    sensors.zgyro = zgyro;
    sensors.xmag = xmag;
    sensors.ymag = ymag;
    sensors.zmag = zmag;
    sensors.abs_pressure = abs_pressure;
    sensors.diff_pressure = diff_pressure;
    sensors.pressure_alt = pressure_alt;
    sensors.temperature = temperature;
    sensors.fields_updated = 0xFF;
    mavlink_msg_hil_sensor_encode(system_id, companion_id, &msg, &sensors);

    // Send Message
    send_message(&msg);

    return;
}

void Autopilot_Interface::sendGpsData(float gpslat, float gpslon, float gpsalt,
                                        float gpsvmod, float gpsv[3],
                                        float gpscog)
{
    mavlink_message_t gps_msg;
    
    uint8_t    fix_type;
    int32_t    lat;
    int32_t    lon;
    int32_t    alt;
    uint16_t   eph;
    uint16_t   epv;
    uint16_t   vel;
    int16_t    vn;
    int16_t    ve;
    int16_t    vd;
    int16_t    cog;
    uint8_t    satellites_visible;
    ptime timestamp = ptask_gettime(MICRO);

    // Conver the measurements
    fix_type = 3;
    lat = (int32_t)(gpslat * 1e7);
    lon = (int32_t)(gpslon * 1e7);
    alt = (int32_t)(gpsalt * 1e3);
    eph = 1;
    epv = 1;
    vel = (uint16_t)(gpsvmod * 100); // cm/s
    vn  = (int16_t)(gpsv[0] * 100); 
    ve  = (int16_t)(gpsv[1] * 100);
    vd  = (int16_t)(gpsv[2] * 100);
    cog = (int16_t)(gpscog * 100);  
    satellites_visible = 8;
        
    // Compose the Mavlink Message
    mavlink_msg_hil_gps_pack(system_id, component_id, &gps_msg, 
            timestamp, fix_type, lat, lon, alt, eph, epv, 
            vel, vn, ve, vd, cog, satellites_visible);
    
    // Send Message
    send_message(&gps_msg);
}

uint64_t Autopilot_Interface::getActuator(float act[4])
{
    int i;
	uint64_t timestamp = 0;

    pthread_mutex_lock(&mut_controls);
    
    if (synch_active)
    {
//         ptime t = ptask_gettime(MILLI);
//         printf("iD = %d \n time = %lu \n\n", system_id, t);
        if (!synchronized)
        {
            pthread_cond_wait(&cond_synch, &mut_controls);
            synchronized = false;
        }
    }
    for (i = 0; i < 4; i++)
    {
         
        /*
          if (hil_ctr[i] != 0.0)
             printf("HilCtr[%d] = %1.2f\n", i, hil_ctr[i]);
         */
        act[i] = hil_ctr[i];
    }
	timestamp = getActuatorTimestamp();
    pthread_mutex_unlock(&mut_controls);

	return timestamp;
}

uint64_t Autopilot_Interface::getActuatorTimestamp()
{
	return hil_ctr_time;
}

bool Autopilot_Interface::getSynchActive()
{
    return synch_active;
}

// -----------------------------------------------------------------------
//   Start Hil Mode
// -----------------------------------------------------------------------
void Autopilot_Interface::start_hil()
{
    char buf[300];
    uint8_t newBaseMode;
    uint32_t newCustomMode;
    int writtenB = 0; 

    newBaseMode = ( base_mode | MAV_MODE_FLAG_HIL_ENABLED );
    newBaseMode &= ~MAV_MODE_FLAG_SAFETY_ARMED;

    newBaseMode |= ( base_mode & MAV_MODE_FLAG_SAFETY_ARMED );
    
    newCustomMode = custom_mode;

    //printf("BaseMode    : %u \n", base_mode);
    //printf("newBaseMode : %u \n", newBaseMode);
    newBaseMode = 113;
    //newBaseMode = 32;
    mavlink_message_t msg;
    mavlink_msg_set_mode_pack(system_id, 0, &msg, (uint8_t)1, newBaseMode, newCustomMode);

    uint16_t len = mavlink_msg_to_send_buffer((uint8_t*)buf, &msg);
    //printf("Setting HIL mode \n");
    int attempts = 0;
    bool condition = 1;
    while( condition )
    {
        attempts++;
        
        // Write the request message to serial
        //printf("Sending Request #%d \n",attempts);
        
        // We must wait for an acknowledgment for the new state, that is
        // a new heartbeat message where the state of the AUV is 
        // contained
        pthread_mutex_lock(&mut_heartbeat);
        updated_heartbeat = false;
        
        writtenB = writeBytes(buf, len);
        
        pthread_cond_wait(&cond_heartbeat,&mut_heartbeat);
        
        if(!updated_heartbeat)
        {
            //printf("Waiting for update...\n");
        }

        // Check the condition for the re-try
        condition = (base_mode & MAV_MODE_FLAG_HIL_ENABLED) == 0;
        condition = 0;
        // Release the resource 
        pthread_mutex_unlock(&mut_heartbeat);
    }

    //printf("After Check BaseMode    : %u \n", base_mode);

    hil_mode = true;
    /*
    printf("Message = \n");
    printf("len     : %u \n", msg.len);
    printf("seq     : %u \n", msg.seq);
    printf("sysid   : %u \n", msg.sysid);
    printf("compid  : %u \n", msg.compid);
    printf("msgid   : %u \n", msg.msgid);
    char* pchar = (char*)(&msg.payload64[0]);
    printf("payload : %u %u %u %u %u %u \n", *(pchar+5), *(pchar+4), *(pchar+3), *(pchar+2), *(pchar+1), *(pchar+0));
    printf("HIL mode set!  [%d attempts]\n",attempts);
    */
}


// ----------------------------------------------------------------------
//   Stop Hil Mode
// ----------------------------------------------------------------------
void Autopilot_Interface::stop_hil()
{
    
    char buf[300];

    uint8_t newBaseMode;
    uint32_t newCustomMode = 0;
    
    int writtenB = 0; 
    
    newBaseMode = ( base_mode & ~MAV_MODE_FLAG_HIL_ENABLED );

    newBaseMode &= ~MAV_MODE_FLAG_SAFETY_ARMED;
    newBaseMode |= ( base_mode & MAV_MODE_FLAG_SAFETY_ARMED );

    printf("BaseMode    : %u \n", base_mode);
    printf("newBaseMode : %u \n", newBaseMode);

    // Create the message and put into the buffer
    mavlink_message_t msg;
    mavlink_msg_set_mode_pack(system_id, 0, &msg, (uint8_t)1, newBaseMode, newCustomMode);
    uint16_t len = mavlink_msg_to_send_buffer((uint8_t*)buf, &msg);

    printf("Unsetting HIL mode \n");
    int attempts = 0;
    bool condition = 1;
    while( condition )
    { 
        attempts++;

       // Write the request message to drone
        printf("Sending Request #%d \n",attempts);
        
        // We must wait for an acknowledgment for the new state, that is
        // a new heartbeat message where the state of the AUV is 
        // contained
        pthread_mutex_lock(&mut_heartbeat);
        updated_heartbeat = false;
        
        writtenB = writeBytes(buf, len);
        
        pthread_cond_wait(&cond_heartbeat,&mut_heartbeat);
        
        if(!updated_heartbeat)
        {
            printf("Waiting for update...\n");
        }
       
        // Check if it is necessary to re-try
        condition = (base_mode & MAV_MODE_FLAG_HIL_ENABLED) != 0;
        pthread_mutex_unlock(&mut_heartbeat);
    }
    
    printf("After Check BaseMode    : %u \n", base_mode);

    hil_mode = false;

    printf("Message = \n");
    printf("len     : %u \n", msg.len);
    printf("seq     : %u \n", msg.seq);
    printf("sysid   : %u \n", msg.sysid);
    printf("compid  : %u \n", msg.compid);
    printf("msgid   : %u \n", msg.msgid);
    char* pchar = (char*)(&msg.payload64[0]);
    printf("payload : %u %u %u %u %u %u \n", *(pchar+5), *(pchar+4), *(pchar+3), *(pchar+2), *(pchar+1), *(pchar+0));
    printf("HIL mode unset!  [%d attempts]\n",attempts);
}

// ----------------------------------------------------------------------
//   Check if it is in HIL mode
// ----------------------------------------------------------------------
bool Autopilot_Interface::is_hil()
{
    return hil_mode;    
}


// ----------------------------------------------------------------------
//   Get the system Id
// ----------------------------------------------------------------------
int Autopilot_Interface::getSystemId()
{
    return system_id;
}

// ----------------------------------------------------------------------
//   Get the system Index
// ----------------------------------------------------------------------
int Autopilot_Interface::getSystemIndex()
{
    return system_Index;
}

// ----------------------------------------------------------------------
//   Set the system Index
// ----------------------------------------------------------------------
int Autopilot_Interface::setSystemIndex(int index)
{
    system_Index = index;
    return system_Index;
}
        



// -----------------------------------------------------------------------
//   Start Off-Board Mode
// -----------------------------------------------------------------------
void Autopilot_Interface:: enable_offboard_control()
{
    // Should only send this command once
    if ( control_status == false )
    {
        printf("ENABLE OFFBOARD MODE\n");

        // --------------------------------------------------------------
        //   TOGGLE OFF-BOARD MODE
        // --------------------------------------------------------------

        // Sends the command to go off-board
        int success = toggle_offboard_control( true );

        // Check the command was written
        if ( success )
            control_status = true;
        else
        {
            fprintf(stderr,"Error: off-board mode not set, could not write message\n");
            //throw EXIT_FAILURE;
        }

        printf("\n");

    } // end: if not offboard_status
}


// ------------------------------------------------------------------------------
//   Stop Off-Board Mode
// ------------------------------------------------------------------------------
void Autopilot_Interface::disable_offboard_control()
{

    // Should only send this command once
    if ( control_status == true )
    {
        printf("DISABLE OFFBOARD MODE\n");

        // ----------------------------------------------------------------------
        //   TOGGLE OFF-BOARD MODE
        // ----------------------------------------------------------------------

        // Sends the command to stop off-board
        int success = toggle_offboard_control( false );

        // Check the command was written
        if ( success )
            control_status = false;
        else
        {
            fprintf(stderr,"Error: off-board mode not set, could not write message\n");
            //throw EXIT_FAILURE;
        }

        printf("\n");

    } // end: if offboard_status

}


// ------------------------------------------------------------------------------
//   Toggle Off-Board Mode
// ------------------------------------------------------------------------------
int Autopilot_Interface::toggle_offboard_control( bool flag )
{
    int writtenB = 0;
    
    // Prepare command for off-board mode
    mavlink_command_long_t com;
    com.target_system    = system_id;
    com.target_component = autopilot_id;
    com.command          = MAV_CMD_NAV_GUIDED_ENABLE;
    com.confirmation     = true;
    com.param1           = (float) flag; // flag >0.5 => start, <0.5 => stop

    // Encode
    mavlink_message_t message;
    mavlink_msg_command_long_encode(system_id, companion_id, &message, &com);

    // Send the message
    char buf[300];
    
    uint16_t len = mavlink_msg_to_send_buffer((uint8_t*)buf, &message);
    // Write buffer to serial port, locks port while writing
    
    writtenB = writeBytes(buf, len);

    // Done!
    return writtenB;
}


// End Autopilot_Interface
