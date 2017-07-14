/**
 * @file ma_manager.h
 *
 * @brief Multi Agent Manager
 *
 * Class for managing multiple agents
 *
 * @author Luigi Pannocchi, <l.pannocchi@gmail.com>
 *
 */

#ifndef MA_MANAGER_H_
#define MA_MANAGER_H_

// ------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------
#include "autopilot_interface.h"
#include <vector>

/*
extern "C" {
#include <ptask.h>
}
*/

class MA_Manager {
    
    public:
        MA_Manager(); /*!< Default Constructor */
        ~MA_Manager(); /*!< Default Destructor */
        
        /*!
         * Add agent to the list of connected Agents
         * \param ip Target Ip String
         * \param r_port Local Port for receiving
         * \param w_port Remote Port for Sending
         * \param synch Synchronization Mode (Synch/Asynch)
         */
        Autopilot_Interface* add_agent(char* ip, uint32_t r_port, 
                      uint32_t w_port, bool synch);
        
        /*!
         * Add agent to the list of connected Agents
         * \param uart_name Path to usb device
         * \param baudrate Local Port for reading
         */
        Autopilot_Interface* add_agent(char*& uart_name, int baudrate, 
                      bool synch);
        
        /*!
         * Remove the i-th agent from the list of connected
         * agents
         * 
         * \param i Index of the agent to be removed
         */
        int remove_agent(int i);
        
        /*! Return the number of connected agents */
        int getNumberOfAgents();
        
        /*! Get the Id-th instance of Autopilot Interface 
         * 
         * \param id Id of the required UAV class instance
         */
        Autopilot_Interface* getAutopilotInstance(int id);
        
        /*! Get the last added instance of Autopilot Interface 
         * 
         */
        Autopilot_Interface* getLastAutopilotInstance();
        
        /*! Sent a Mavlink Message to the connected AUV with a given ID 
         * 
         * \param message Message to be sent
         * \param uavId Id of the target connected UAV
         */
        int sendMessage2UAV(mavlink_message_t* message, int uavId);
    
    private:
        
        int ConnectedAgents; /*!< Number of connected Agents */
        pthread_mutex_t ma_mutex; /*!< Mutex for the mutual exclusion */
        std::vector<Autopilot_Interface*> pAI; /*!< Pointer to the dynamically allocated Autopilot Interface Classes */
    
};

#endif // MA_MANAGER_H_
