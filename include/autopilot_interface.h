/**
 * \file autopilot_interface.h
 *
 * \brief Autopilot interface class definition
 *
 * Functions for sending and receiving commands to an autopilot via MAVlink
 *
 * \author Luigi Pannocchi, <l.pannocchi@gmail.com>
 *
 */


#ifndef AUTOPILOT_INTERFACE_H_
#define AUTOPILOT_INTERFACE_H_


// -----------------------------------------------------------------------
//   Includes
// -----------------------------------------------------------------------

//#include "serial_port.h"
#include "udp_port.h"
#include "serial_port.h"

#include <signal.h>
#include <sys/time.h>
#include <stdint.h>
#include <common/mavlink.h>
#include <queue>
#include <poll.h>

extern "C" {
#include <ptask.h>
}

// ------------------------------------------------------------------------
//   Data Structures
// ------------------------------------------------------------------------

/**
 * UAV Category (Network or Serial connected)
 */
enum CommInterfaceType {UDP, SERIAL};

/** Struct containing information on the MAV we are currently connected to
 *  and the queue of received messages
 *
 */
struct Mavlink_Messages {

    int sysid; /*!< System Identifier */
    int compid /*!< Component Identifier */;

    std::queue<mavlink_message_t> messages; /*!< Queue of the received messages */
};


/// Structure of the simulated sensor data
struct SensorData_str {
    float      xacc; /*!< X acceleration in body Frame [m/s^2] */
    float      yacc; /*!< Y acceleration in body Frame [m/s^2] */
    float      zacc; /*!< Z acceleration in body Frame [m/s^2] */
    float      xgyro; /*!< X angular speed in body Frame [rad/s] */
    float      ygyro; /*!< Y angular speed in body Frame [rad/s] */
    float      zgyro; /*!< Z angular speed in body Frame [rad/s] */
    float      xmag; /*!< X magnetic field in body Frame [Gauss] */
    float      ymag; /*!< Y magnetic field in body Frame [Gauss] */
    float      zmag; /*!< Z magnetic field in body Frame [Gauss] */
    float      abs_pressure; /*!< Absolute atmospheric pressure [milliBar] */
    float      diff_pressure; /*!< Differential atmospheric pressure [milliBar] */
    float      pressure_alt; /*!< Altitude calculated from pressure [m] */
    float      temperature; /*!< Temperature [Celsius] */
    uint32_t   fields_updated; /*!< Bitmask of the updated fields */
    uint64_t   time_usec; /*!< Timestamp [us] */
};

/// Structure of the simulated GSP sensor data
struct GPSData_str {
    uint8_t    fix_type; /*!< Fix Type [0-1: No Fix; 2: 2D Fix; 3: 3D Fix] */
    int32_t    lat; /*!<  Latitude [deg * 1e7] */
    int32_t    lon; /*!<  Longitude [deg * 1e7] */
    int32_t    alt; /*!<  Altitude [m * 1000] */
    uint16_t   eph; /*!<  GPS HDOP [cm] */
    uint16_t   epv; /*!<  GPS VDOP [cm] */
    uint16_t   vel; /*!<  GPS Ground Speed [cm/s] */
    int16_t    vn; /*!<  GPS Velocity North [cm/s] */
    int16_t    ve; /*!< GPS Velocity East [cm/s] */
    int16_t    vd; /*!<  GPS Velocityy Down [cm/s] */
    int16_t    cog; /*!< Course over ground [deg * 100] */
    uint8_t    satellites_visible; /*!< Number of visible satellites */

    uint64_t   time_usec; /*!< Timestamp [us] */
};

/// Structure of the control actuator data
struct Actuator_str {
    float act[4]; /*!< Control signal to the motors [0 - 1] */
};

// ---------------------------------------------------------------------
//   Autopilot Interface Class
// ---------------------------------------------------------------------
/*!
 * Autopilot Interface Class
 *
 */
class Autopilot_Interface
{

public:
    /// Base Constructor
    Autopilot_Interface();

    /// Advanced Constructor
    /*!
     * Instantiate the class configuring the UDP communication port
     *  \param ip Target Ip String
     *  \param r_port Local Port for reading
     *  \param w_port Remote Port for sending
     */
    Autopilot_Interface(char* ip, uint32_t r_port,
                        uint32_t w_port, bool synch);

    /*!
     * Instantiate the class configuring the SERIAL communication port
     * \param uart_name_ Path to usb device
     * \param baudrate Local Port for reading
     */
    Autopilot_Interface(char *&uart_name_, int baudrate,
                        bool synch);

    /// Base Destructor
    ~Autopilot_Interface();


    // COMMUNICATION

    // Function to read messages from the Serial Port, manage them and
    // retrieve them.
    int fetch_data();
    int get_message(mavlink_message_t* req_mess);

    // Write mavlink message to the drone
    int send_message(mavlink_message_t* msg);

    /**
     * Interface Handles
     *
     */

    /*! Set the autopilot board in HIL mode */
    void start_hil();

    /*! Unset the autopilot board in HIL mode */
    void stop_hil();

    /*! Return if the autopilot board is in HIL mode */
    bool is_hil();

    /*! Return if the autopilot is connected to the PC */
    bool isConnected();

    /*! Wait until an heartbeat is received */
    void waitNextHeartbeat();

    /*! Enable the offboard control mode */
    void enable_offboard_control();

    /*! Disable the offboard control mode */
    void disable_offboard_control();

    /*! Toggle the offboard control mode */
    int toggle_offboard_control( bool flag );

    /*! Update the state of the vehicle using the info of
     * the heartbeat message
     */
    void update_vehicle_status(mavlink_heartbeat_t heartbeat);

    /// Data Exchange functions

    /*! Send Sensor data to the autopilot board*/
    void sendSensorData(float xacc, float yacc, float zacc,
                        float xgyro, float ygyro, float zgyro,
                        float xmag, float ymag, float zmag,
                        float abs_pressure, float diff_pressure, float pressure_alt,
                        float temperature, ptime timestamp);

    /*! Send gps data to the autopilot board */
    void sendGpsData(float gpslat, float gpslon, float gpsalt, float gpseph, float gpsepv,
                     float gpsvmod, float gpsvn, float gpsve, float gpsvd,
                     float gpscog);

    /*! Get control values */
    uint64_t getActuator(float act[4]);

    /*! Get control timestamp */
    uint64_t getActuatorTimestamp();

    /*! Get the AUV system id */
    int getSystemId();

    /*! Send raw bytes to vehicle */
    int writeBytes(char * data, unsigned int len);

    /*! Read raw bytes from the vehicle */
    int readBytes(char* data, unsigned int len);

    /*! Set the Index of the System */
    int setSystemIndex(int index);

    /*! Get the Index of the System */
    int getSystemIndex();

    /*! Get if the autopilot board drives the synchronization */
    bool getSynchActive();

    long int t_ctr;

private:

    int system_Index; /*!< Index of the vehicle in the simulation framework*/

    /// Information on the real vehicle
    ///
    int system_id; /*!< System Identifier */
    int component_id; /*!< Component Identifier */
    int autopilot_id; /*!< Autopilot Identifier */
    int companion_id; /*!< Companion PC Identifier */
    uint8_t mav_type; /*!< Vehicle Type */
    uint8_t system_status; /*!<  Status of the Autopilot System */
    uint32_t custom_mode; /*!< Custom Mode */
    uint8_t base_mode; /*!< Base Mode */


    /**
     * Type of communication interface (SERIAL or UDP)
     */
    CommInterfaceType commType;

    int control_status;

    bool Connected; /*!< Flag for the autopilot connection state */

    pthread_cond_t cond_synch; /*!< Synchronization pthread condition */
    bool synch_active; /*!< Flag for the synchronization type */
    bool synchronized; /*!< Synchronization variable */

    // Mutex
    pthread_mutex_t mut_controls; /*!< Mutex for the access to the control values */

    /// Structure to stock the messages
    Mavlink_Messages current_messages; /*!< Received Messages */

    float hil_ctr[4]; /*!< Control Data */


    /*! Timestamp of the received control data */
    uint64_t hil_ctr_time;

    /*!
     * Variables to assure mutual exclusion when checking the running
     * communication with the vehicle
     */
    bool updated_heartbeat; /*!< Flag for the update of the vehicle status*/
    pthread_mutex_t mut_heartbeat; /*!< Mutex for accessing the receiving_heartbeat variable */
    pthread_cond_t cond_heartbeat; /*!< Condition Variable for accessing the receiving_heartbeat variable */
    bool hil_mode; /*!< Flag representing the Hil Mode status of the autopilot */

    /*!
     *  Communication Part
     */
    //Serial_Port uart_port;
    Udp_Port* udp_port; /*!< Reference to the UDP port */
    Serial_Port* serial_port; /*!< Reference to the Serial port */

    struct pollfd fdsR[1]; /*!< File descriptor associated to the reading port */
    struct pollfd fdsW[1]; /*!< File descriptor associated to the writing port */

    /// Private Functions
    ///
    int handle_message(mavlink_message_t message);

    /*! Unpack the information on the motor actuation values */
    void unpack_ctr_mess(mavlink_message_t msg);

    /*! Initialization function for the class */
    void initialize_class(bool synch);

    /*! Set the value of the System ID*/
    void setSystemId(int id);
};
#endif // AUTOPILOT_INTERFACE_H_


