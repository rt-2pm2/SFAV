/**
 *
 *  \file gs_interface.h
 *  \brief Ground Station Interface Class definition
 * 
 * 
 *  This code constitutes the interface for the communication with a Sim
 *  instance.
 *
 * \author Luigi Pannocchi, <l.pannocchi@gmail.com>
 */

#ifndef GS_INTERFACE_H_
#define GS_INTERFACE_H_

#include "udp_port.h"
#include <time.h>
#include "common/mavlink.h"
#include <poll.h>
#include <queue>


extern "C" {
#include "ptask.h"
}

/*!
 * Structure containing the queue of message to the 
 * Ground Station, the Id of the sending UAV and a 
 * mutex for mutual exclusion for the access to the 
 * resoures.
 * 
 */
struct UavConnection{
    /// Id of the connected UAV 
    int sysId; 
    /// Queue of the messages to be sent to the GS
    std::queue<mavlink_message_t> sendQueue;
    /// Mutex for the mutual exclusion
    pthread_mutex_t mut_sendQueue;
};

class GS_Interface {

    public:

        /*! 
         * Default Constructor 
         */
        GS_Interface();
        
        /*!
         * Advanced Constructor
         * \param ip Ip Address of the machine hosting the Ground Station
         * \param r_port Base Value for the Local Port for reading
         * \param w_port Base Value for the Remote Port for sending
         */
        GS_Interface(char* ip, uint32_t r_port, uint32_t w_port); 

        /*!
         * Default Destructor
         */
        ~GS_Interface(); 

        // Settings
        /**
         * Set the ReadPort variable
         * \param port Reading Port
         */
        int setReadPort(unsigned int port);
        
        /**
         * Set the WritePort variable
         * \param port Writing Port
         */
        int setWritePort(unsigned int port);
        
        /**
         * Update the vector of queues adding a new connected uav
         * \param uavId Uav index to be added
         */
        int addUav(int uavId);
        
        /**
         * Update the vector of queues removing a disconnected uav
         * \param uavId Uav index to be removed
         */
        int removeUav(int uavId);

        // Communication 
        int sendMessages();
		int sendMessage(mavlink_message_t* message);
        int receiveMessage();
		int receiveMessages();

        int pushMessage(mavlink_message_t* message);
        
        /**
         * Get a message from the receiving queue
         * \param message Pointer to the mavlink message object to return 
         *                the message
         * The function return the system Id of the interested Uav
         */
        int getMessage(mavlink_message_t* message);
        
        /**
         * Get the pointer to the connection structure
         */
        struct UavConnection* getConnectionStructure(int id);
        
        /**
         * Get the number of connected Connected AUVS
         */
        int getNumberOfConnectedUAVs();

        /**
         * Return the number of message in the recevign queue
         */
        int getDimrecQueue();

        /**
         * Set the "started" flag to true
         */
        void setGSActive();
        
        /**
         * Get the state of the "started" flag
         */
        bool getGSActive();

        /**
         * UDP port class
         */
        Udp_Port udp_port;
        

    private:

        /**
         * Flag representing that the application is started
         */
        bool started;
        
        /**
         * Communication port with the application
         */
        uint32_t r_port, w_port;
        
        /**
         * Number of connected connectedUAVs
         */
        int connectedUAVs;
        
        /**
         * Vector of queues for sending (one for each connected uav)
         */
        std::vector< struct UavConnection* > VectConnecteUAVs;
        
        
        /**
         * Receiving queue to stock the messages from the GroundStation
         * Note: I have only one queue since I don't have other threads 
         * accessing the resource
         */
        std::queue<mavlink_message_t> recQueue;
    
        /** 
         * Structures for the poll on the UDP connection to the GroundStation application
         */
        struct pollfd fdsR[1];
        struct pollfd fdsW[1];
		
		char rbuff[512];

};

#endif // GS_INTERFACE_H_
