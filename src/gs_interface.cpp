/**
 * \file: gs_interface.cpp
 *
 */

#include "gs_interface.h"


// ------------------------------------------------------------------
// Ground Station Interface
// ------------------------------------------------------------------
/*
 *                     +----------------+
 *  UDP (14551)  <-----|                |
 *                     |  GS (x.x.x.x)  |
 *  UDP (14550)  ----->|                |
 *                     +----------------+
 */
GS_Interface::GS_Interface():
	udp_port((const char *)"127.0.0.1", (uint32_t)14551, (uint32_t)14550) 
{
	int i;
    
    connectedUAVs = 0;

	// Inizialize UDP
	setReadPort(14551);
	setWritePort(14550);

	// Define the structure for the polling
	fdsR[0].fd = udp_port.sock;
	fdsR[0].events = POLLIN;

	fdsW[0].fd = udp_port.sock;
	fdsW[0].events = POLLOUT;

	started = false;

	for (i = 0; i < 512; i++)
		rbuff[i] = 0;
}

GS_Interface::GS_Interface(char *ip, uint32_t r_port, uint32_t w_port):
	udp_port(ip, r_port, w_port)
{
	int i;
    
    connectedUAVs = 0;

	// Initialize UDP
	setReadPort(r_port);
	setWritePort(w_port);

	// Define the structure for the polling
	fdsR[0].fd = udp_port.sock;
	fdsR[0].events = POLLIN;

	fdsW[0].fd = udp_port.sock;
	fdsW[0].events = POLLOUT;

	started = false;

	for (i = 0; i < 512; i++)
		rbuff[i] = 0;
}

GS_Interface::~GS_Interface()
{
    int i;
    int NElements;
	
    printf("GS Destructor\n");
    
    NElements = getNumberOfConnectedUAVs();
    
    // Delete the Allocated Memory for the Classes
    for (i = 0; i < NElements; i++)
    {
        VectConnecteUAVs.pop_back();
    }
}

// ----------------------------------------------------------------
//  FUNCTIONS
// ----------------------------------------------------------------

// ToDo: Check!!!!!!!

//
// setReadPort
//
int GS_Interface::setReadPort(unsigned int port)
{
    r_port = port;
    return 1;
}

//
// setWritePort
//
int GS_Interface::setWritePort(unsigned int port)
{
    w_port = port;
    return 1;
}

//
// setIP
//
int GS_Interface::setUDP(char* ip, unsigned int port_r, unsigned int port_w)
{
    r_port = port_r;
    w_port = port_w;
    
	// Initialize UDP
    //ToDo: Reinitialize UDP port!
    
    return 1;
}

//
// setGSActive
//
void GS_Interface::setGSActive()
{
    started = true;
}

// getGSActive
bool GS_Interface::getGSActive()
{
    return started;
}

//
// addUav
//
int GS_Interface::addUav(int uavId)
{    
    struct UavConnection* pUavConn;
    
    pUavConn = new struct UavConnection;
    pUavConn->sysId = uavId;
    pthread_mutex_init(&(pUavConn->mut_sendQueue), 0);
    
    // Insert the pointer to the allocated structure in the vector
    VectConnecteUAVs.push_back(pUavConn);
    
    connectedUAVs++;
    return 1;
}

// 
// removeUav
//
int GS_Interface::removeUav(int uavId)
{
    int i, N;
    std::vector< struct UavConnection* >::iterator iter;
    
    iter = VectConnecteUAVs.begin();
    N = VectConnecteUAVs.size();
    for (i = 0; i < N; i++)
    {
        if (VectConnecteUAVs.at(i)->sysId == uavId)
        {
            delete VectConnecteUAVs.at(i);     // Destroy the class
            VectConnecteUAVs.erase(iter + i);  // Remove the Vector element
            connectedUAVs--;
            return 1;
        }
    }
    printf("Uav Connection not found!\n");
    return -1;
}


//
// sendMessage
// Send message directly to the Ground Station
//
int GS_Interface::sendMessage(mavlink_message_t* sendMessage)
{
	int bytes_sent;

    bytes_sent = udp_port.send_mav_mess(sendMessage);

	return bytes_sent;
}

//
// sendMessages
// Send message from the message queue to the Ground Station
//
int GS_Interface::sendMessages()
{
    int N, i;
    int bytes_sent;
    mavlink_message_t msg;

    // Take the message from the queues
    N = getNumberOfConnectedUAVs();
    for (i = 0; i < N; i++)
    {
        pthread_mutex_lock(&VectConnecteUAVs.at(i)->mut_sendQueue);
        while (!VectConnecteUAVs.at(i)->sendQueue.empty())
        {
            msg = VectConnecteUAVs.at(i)->sendQueue.front();
            VectConnecteUAVs.at(i)->sendQueue.pop();

            //printf("Sending Message ID = %d\n", msg.msgid);
            //printf("sendQueue # = %lu\n", VectConnecteUAVs.at(i)->sendQueue.size());
            bytes_sent = sendMessage(&msg);
        }
        pthread_mutex_unlock(&VectConnecteUAVs.at(i)->mut_sendQueue);
    }
    return 1;
}


//
// receiveMessages
// Wait for data on the UDP and receive messages until there is no more data
//
int GS_Interface::receiveMessages()
{
	int i;
	uint16_t timeout = 0;  // ms
	int8_t read_bytes = 0;
	mavlink_message_t recMessage;
	mavlink_status_t status;

	int ret = poll(fdsR, 1, timeout);

	if (ret < 0) 
	{ 
		printf("GS_Interface::receiveMessage : ERROR IN READING UDP PORT\n");
		return -1;
	}

	if (ret == 0)
	{
		//printf("GS_Interface::receiveMessage : NO DATA RETURNED\n");
		return 0;
	}
	else
	{
		while (ret > 0)
		{
			// Receive data num bytes over UDP and put them at the sensors address
			read_bytes = udp_port.readBytes(rbuff, sizeof(mavlink_message_t));
			for (i = 0; i < read_bytes; i++)
			{
				// Parse 1 byte at time
				if (mavlink_parse_char(MAVLINK_COMM_2, rbuff[i], &recMessage, &status))
				{
					recQueue.push(recMessage);
					//printf("recQueue # = %d\n", recQueue.size());
					//printf("Message id %d\n", recMessage.msgid);
				}
			}
			ret = poll(fdsR, 1, timeout);
		}
	}
	return 1;
}


//
// pushMessage 
// Push the message into the sending queue towards the Ground Station
//
int GS_Interface::pushMessage(mavlink_message_t* msg)
{
    int uavId;
    
    // Identify the sending uav
    uavId = msg->sysid;
    

    struct UavConnection* pConn = getConnectionStructure(uavId);

    if (pConn != NULL)
    {
        //printf("Pushing message from UAV %d\n", uavId);
        // Push the message in the correspondend queue
        pthread_mutex_lock(&pConn->mut_sendQueue);
        pConn->sendQueue.push(*msg);
        pthread_mutex_unlock(&pConn->mut_sendQueue);
    }
    else
        printf("[GS_Interface::pushMessage] Unknown source | MsgId = %d\n", msg->msgid);

    return 1;
}


// 
// getMessage
//
int GS_Interface::getMessage(mavlink_message_t* msg)
{
    int sysId;
    mavlink_message_t message;
	if (!recQueue.empty())
	{
		message = recQueue.front();
		recQueue.pop();
	}
   
	*msg = message;
    
	//printf("recQueue # = %d\n", recQueue.size());
	sysId = message.sysid;
	return sysId;
}

struct UavConnection* GS_Interface::getConnectionStructure(int id)
{
    int i, N;
    struct UavConnection* preturn;
    N = getNumberOfConnectedUAVs();

    for (i = 0; i < N; i++)
    {
        if (VectConnecteUAVs.at(i)->sysId == id)
        {
            preturn = VectConnecteUAVs.at(i);
            return preturn;
        }
    }
        
    printf("[GS_Interface::getConnectionStructure] Connection Structure not found! SysId = %d \n", id);
    return NULL;
}


//
// getDimrecQueue
//
int GS_Interface::getDimrecQueue()
{
	return recQueue.size();
}

//
// getNumberOfConnectedUAVs
//
int GS_Interface::getNumberOfConnectedUAVs()
{
    return connectedUAVs;
}
