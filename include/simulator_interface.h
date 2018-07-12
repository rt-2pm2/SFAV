/**
 *
 *  \file simulator_interface.h
 *  \brief Simulation Interface class definition
 * 
 *  Class used as interface with the Simulation Model
 *
 *  \author Luigi Pannocchi, <l.pannocchi@gmail.com>
 */

#ifndef SIMULATION_INTERFACE_H_
#define SIMULATION_INTERFACE_H_


#define SIM_INTERFACE_DBG 1
#define NUM_FLOAT_ACT_CONTROLS 4
#define NUM_FLOAT_SENSORS 22

// -----------------------------------------------------------------------
//   Includes
// -----------------------------------------------------------------------
extern "C"
{
    #include "DynModel.h"
    #include "DynModel_private.h"
}
#include "udp_port.h"

struct SimOutput
{
    float Temp;                       /*!< Temperature [Celsius] */
    float Press;                      /*!< Pressure [mbar] */
    float diff_Pres;                  /*!< Differential pressure [mbar] */
    float Baro_Alt;                   /*!< Altitude from barometer data [m] */
    float Gps_Lat;                    /*!< Gps latitude data [deg] */
    float Gps_Lon;                    /*!< Gps longitude data [deg] */
    float Gps_Alt;                    /*!< Gps altitude data [m] */
    float Gps_V[3];                   /*!< Gps velocity data [m/s] */
    float Gps_V_Mod;                  /*!< Gps speed data [m/s] */
    float COG;                        /*!< Gps COG data [deg] */
    float Lat_Lon_Alt[3];             /*!< Real coordinate data [deg] */
    float Magn[3];                    /*!< Magnetometer data [Gauss] */
    float RPY[3];                     /*!< RPY orientation [rad] */
    float Accelerometer[3];           /*!< Accelerometer data [m/s^2] */
    float Gyro[3];                    /*!< Gyro data [rad/s] */
    float Quaternion[4];              /*!< Quaternion orientation */
    float Sonar;                      /*!< Sonar altitude [m] */
    float Forces[3];                  /*!< Forces generated by thrusters [N] */
    float Torques[3];                 /*!< Torques generated by thrusters [N*m] */
    float Thrusts[4];                 /*!< Rotor thrusts [N] */
    float Rotor_Speed[4];             /*!< Rotor speeds [rad/s] */
    float Xe[3];                      /*!< Position NED w.r.t the starting point [m] */
    float Freact[3];                  /*!< Reaction forces [N] */
};


class Simulator_Interface {

    public:
        /// Default Constructor
        Simulator_Interface();
        
        /// Simple Constructor
        Simulator_Interface(int Id);
        
        /// Advanced Constructor
        Simulator_Interface(int Id, char *ip, uint32_t w_port);

        /// Default Destructor
        ~Simulator_Interface();


        /**
         * Update the value of the actuator commands
         */
        void updateActCtr(float pwm[NUM_FLOAT_ACT_CONTROLS], 
                             float impnorm[3], float pen_collision, 
                             float Fext[3]);

        /*! Update PWM commands */
        void updatePWM(float pwm[NUM_FLOAT_ACT_CONTROLS]);
        
        /**
         * Get the simulation output data
         */
        void getSimOutput(struct SimOutput* simout);

        /**
         * Get the Position and Attitude of the simulated vehicle
         */
        void getSimPosAtt(float Xe[3], float Att[3]);
        
        /**
         * Send the Position and Attitude of the simulated vehicle
         * to the debug machine
         */
        void DBGsendSimPosAtt();
        
        /**
         * Send the Position and the Angular velocity of the simulated vehicle
         * to the debug machine
         */
        void DBGsendSimPosGyro();
        
        /**
         * Simulation Step
         */
        void simStep();
        
        /**
         * Signal the running simulation thread
         */
        void simulationActive();
        
        /**
         * Return the system Identifier of the simualated
         * UAV
         */
        int getSystemId();
        
        /**
         * Set the initial coordinate of the UAV
         */
        void setInitialCoordinates(float Lat, float Lon);
        
        /*! Update collision information */
        void updateCollision(float impnorm[3], float pen_collision);
        
    private:
        double Initial_LL[2]; 
        
        /*! Initialize the interface with default values */
        void Initialize_Interface();
        
        /*! Update external forces */
        void updateFext(float Fext[3]);
        
        /*! Value of the control for the actuators */
        float act_controls[NUM_FLOAT_ACT_CONTROLS]; 

        /*! Simulated sensor data */
        float sensors[NUM_FLOAT_SENSORS];
        
        /*! Pointer to the simulation structure */
        RT_MODEL_DynModel_T* SimModel;
        
        /*! Identifier of the vehicle being simulated */
        int sysId;
        
        /*! Flag to signal the simulation thread running */
        bool simulator_thread_active;
        
        /*!< Communication port */
        Udp_Port* ComPort; 
        
        /*! IP address of the debug machine */
        char net_ip[15];
        
        /*! UDP port for dubeg purpose */
        uint32_t udp_port;
        
};

#endif // SIMULATION_INTERFACE_H_
