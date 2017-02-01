/**
 * \file ue_interface.cpp
 * 
 * \author Luigi Pannocchi, <l.pannocchi@gmail.com>
 */

#include "ue_interface.h"

struct UE_SendData UEDataOut;
struct UE_RecData UEDataIn;


// ------------------------------------------------------------------
// Unreal Engine Interface
// ------------------------------------------------------------------
/*
 *                    +----------------+
 *  UDP (9000)  <-----|                |
 *                    |  UE (x.x.x.x)  |
 *  UDP (8000)  ----->|                |
 *                    +----------------+
 */
UE_Interface::UE_Interface():
	udp_port((const char *)"127.0.0.1", (uint32_t)9000, (uint32_t)8000) 
{
	int i;
    
    NVehicles = 0;

	// Inizialize UDP
	setReadPort(9000);
	setBaseWritePort(8000);

	// Define the structure for the polling
	fdsR[0].fd = udp_port.sock;
	fdsR[0].events = POLLIN;

	fdsW[0].fd = udp_port.sock;
	fdsW[0].events = POLLOUT;

	// Initialize Mutexes
	pthread_mutex_init(&mut_sendData, 0);
	pthread_mutex_init(&mut_recData, 0);
	started = 1;

	for (i = 0; i < 512; i++)
		rbuff[i] = 0;
}

UE_Interface::UE_Interface(char *ip, uint32_t r_port, uint32_t w_port):
	udp_port(ip, r_port, w_port)
{
	int i;
    
    NVehicles = 0;

	printf("UE_Interface: Read Port = %d | Write Port = %d\n", r_port, w_port);
	// Initialize UDP
    for (i = 0; i < strlen(ip); i++)
        net_ip[i] = ip[i];
    
	setReadPort(r_port);
	setBaseWritePort(w_port);

	// Define the structure for the polling
	fdsR[0].fd = udp_port.sock;
	fdsR[0].events = POLLIN;

	fdsW[0].fd = udp_port.sock;
	fdsW[0].events = POLLOUT;

	// Initilize Mutexes
	pthread_mutex_init(&mut_sendData, 0);
	pthread_mutex_init(&mut_recData, 0);
	started = 1;

	for (i = 0; i < 512; i++)
		rbuff[i] = 0;
}

UE_Interface::~UE_Interface()
{
	printf("UE Destructor\n");
}

// ----------------------------------------------------------------
//  FUNCTIONS
// ----------------------------------------------------------------

//
// setReadPort
//
int UE_Interface::setReadPort(unsigned int port)
{
    r_port = port;
    
    return r_port;
}

//
// setWritePort
//
int UE_Interface::setBaseWritePort(unsigned int port)
{
    w_port = port;
    
    return w_port;
}

//
// sendData
// Send Data to the UE through the UDP port 
//
int UE_Interface::sendData(int Id)
{
	int bytes_sent = -1;

	struct UE_SendData tempData;
    Udp_Port* port;

	// Comping the data in a temp structure
	pthread_mutex_lock(&mut_sendData);
	tempData = UEDataOut;
	pthread_mutex_unlock(&mut_sendData);
    
    port = getPortInstance(Id);
	// Sending the data
    if (port != NULL)
        bytes_sent = port->writeBytes((char* )&tempData, sizeof(struct UE_SendData));

	return bytes_sent;
}

//
// addPort
//
int UE_Interface::add_port(int index)
{
    // Create a new Autopilot Interface class and add it to the list
    Udp_Port* pP;
    pP = new Udp_Port();
    pP->InitializeOutputPort(net_ip, w_port + index);
    pPorts.push_back(pP);
    NVehicles++;
    return 1;
}


//
// getPortInstance
//
Udp_Port* UE_Interface::getPortInstance(int id)
{
    int i;
    int port;
    for (i = 0; i < NVehicles; i++)
    {
        port = pPorts.at(i)->getWport();
        if (port == (w_port + id))
            return pPorts.at(i);
    }   
    printf("Port Instance not found! | Requiring Id = %d\n", id);
    return NULL;
}



// 
// setData
// Set the data to send 
//
int UE_Interface::setData(struct UE_SendData Data)
{
	pthread_mutex_lock(&mut_sendData);
	UEDataOut = Data;
	pthread_mutex_unlock(&mut_sendData);
		
	return 1;
}


//
// receiveData
// Wait for data on the UDP and receive one message from the Unreal Engine 
//
int UE_Interface::receiveData()
{
	int i;
	uint16_t timeout = 0;  // ms
	int8_t read_bytes = 0;

	int ret = poll(fdsR, 1, timeout);

	if (ret < 0) 
	{ 
		printf("UE_Interface::receiveData : ERROR IN READING UDP PORT\n");
		return -1;
	}

	if (ret == 0)
	{
		//printf("UE_Interface::receiveData : NO DATA RETURNED\n");
		return 0;
	}
	else
	{
		// Receive data num bytes over UDP and put them at the sensors address
		read_bytes = udp_port.readBytes(rbuff, sizeof(struct UE_RecData));
		//printf("Got Data, %d Bytes\n", read_bytes);
		if (read_bytes == sizeof(UE_RecData))
		{
			memcpy((void*) &UEDataIn, (void* ) &rbuff[0], read_bytes);
			
		//	printf("Nx = %1.2f, Ny = %1.2f, Nz = %1.2f, Pen = %3.2f\n", UEDataIn.Nx, UEDataIn.Ny, UEDataIn.Nz, UEDataIn.PenDepth);
		}
	}
	return 1;
}

// 
// getData
//
int UE_Interface::getData(struct UE_RecData* data)
{
	pthread_mutex_lock(&mut_recData);
	memcpy((void*) data, (void*) &UEDataIn, sizeof(UE_RecData));
	pthread_mutex_unlock(&mut_recData);

	return 1;
}

