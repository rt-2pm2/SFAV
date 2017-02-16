/**
 * 
 * \file udp_port.h
 * 
 * \brief UDP interface class definition
 * 
 * Functions for managing communication with UDP ports
 *
 * \author Luigi Pannocchi, <l.pannocchi@gmail.com>
 */

#ifndef UDP_PORT_H_
#define UDP_PORT_H_

// ---------------------------------------------------------------------------
// Includes
// ---------------------------------------------------------------------------

#include <stdio.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <fcntl.h>
#include <time.h>
#include <sys/time.h>


#include "common/mavlink.h"


#define BUFFER_LENGTH 2041



// ---------------------------------------------------------------------------
// UDP Port Manager Class
// ---------------------------------------------------------------------------
/**
 * UDP Port Class
 *
 *
 */
class Udp_Port
{

public:
	
	//! Basic Constructor
	Udp_Port();

	//! Advanced Constructor
	/*!
 	* 	\param target_ip Target Ip String
 	* 	\param r_port Local Port for receiving
 	* 	\param w_port Remote Port for Sending 
  	*/
	Udp_Port(const char* target_ip, uint32_t r_port, uint32_t w_port);

	//! Desctructor
	~Udp_Port();
 
    
    //! Initialization Function
    /*!
    *     \param target_ip Target Ip String
    *     \param w_port Remote Port for Sending
    */
    void InitializeOutputPort(const char* target_ip, uint32_t w_port);
    
	//! Send a Mavlink Message to the Remote Target 
	/*!
 	*	\param message Mavlink Message to send
 	*	\return Number of bytes sent 
 	*/ 
	int send_mav_mess(mavlink_message_t* message);

	//! Sent Raw Data Bytes	
	/*! 
 	*	\param data Pointer to the data array to be sent
 	*	\param len Nuber of bytes to be sent
 	*	\return Number of Bytes actually sent 
 	*/
	int writeBytes(char* data, unsigned int len);

	//! Receive Data Bytes
	/*!
	*	\param data Pointer to the data array where to copy the received bytes
	*	\param len Number of bytes received
	*	\return Number of received bytes
	*/ 
	int readBytes(char* data, unsigned int len);
    
    //! Return the output port
	/*!
	*	\return Number of the remote port
	*/ 
    int getWport();

	int sock; /*!< File descriptor that refers to the END point of the communication*/
private:
  
	char target_ip[100]; /*!< String containing the Remote Address IP*/

	struct sockaddr_in remAddr; /*!< Remote address*/
	struct sockaddr_in locAddr; /*!< Local address*/
	
	int read_port; /*! Read Port */
	int write_port; /*! Write Port */

	unsigned char buf[BUFFER_LENGTH]; /*!< Output data buffer*/   
};


#endif // UDP_PORT_H_
