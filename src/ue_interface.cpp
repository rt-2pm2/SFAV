/**
 * \file ue_interface.cpp
 * 
 * \author Luigi Pannocchi, <l.pannocchi@gmail.com>
 */

#define DEBUG

#include "ue_interface.h"


// ------------------------------------------------------------------
// Unreal Engine Interface
// ------------------------------------------------------------------
/*
 *                    +----------------+
 *  UDP (8001)  <-----|                |
 *                    |  UE (x.x.x.x)  |
 *  UDP (8000)  ----->|                |
 *                    +----------------+
 */
UE_Interface::UE_Interface()
{
	int i;
    
    NVehicles = 0;

	// Inizialize UDP
	setBaseReadPort(8001);
	setBaseWritePort(8000);

	// Initialize Mutexes
	pthread_mutex_init(&mut_sendData, 0);
	pthread_mutex_init(&mut_recData, 0);
	started = 1;

	for (i = 0; i < 512; i++)
		rbuff[i] = 0;
    
    ComPort = NULL;
}

UE_Interface::UE_Interface(char *ip, uint32_t r_port, uint32_t w_port)
{
	int i;
    
    NVehicles = 0;
    
	// Initialize UDP
    for (i = 0; i < strlen(ip); i++)
        net_ip[i] = ip[i];
    
	setBaseReadPort(r_port);
	setBaseWritePort(w_port);

	// Initilize Mutexes
	pthread_mutex_init(&mut_sendData, 0);
	pthread_mutex_init(&mut_recData, 0);
	started = 1;

	for (i = 0; i < 512; i++)
		rbuff[i] = 0;
    
    ComPort = NULL;
}

UE_Interface::~UE_Interface()
{
    printf("UE Destructor\n");
}

// ----------------------------------------------------------------
//  FUNCTIONS
// ----------------------------------------------------------------

//
// setBaseReadPort
//
int UE_Interface::setBaseReadPort(unsigned int port)
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
int UE_Interface::sendData(struct UE_SendData data)
{
    int bytes_sent = -1;


    // Sending the data
    if (ComPort != NULL)
        bytes_sent = ComPort->writeBytes((char* )&data, sizeof(struct UE_SendData));

    return bytes_sent;
}

//
// addPort
//
int UE_Interface::add_port(int index)
{
    // Create a new Autopilot Interface class and add it to the list
#ifdef DEBUG
    printf("UE_Interface::add_port | Creating Port for UAV %d\n", index);
#endif

    ComPort = new Udp_Port(net_ip, r_port + index, w_port + index);

    if (ComPort != NULL)
    {
        fdsR[0].fd = ComPort->sock;
        fdsR[0].events = POLLIN;

        fdsW[0].fd = ComPort->sock;
        fdsW[0].fd = POLLOUT;
        return 1;
    }
    else
        return 0;
}

//
// receiveData
// Wait for data on the UDP and receive one message from the Unreal Engine 
//
int UE_Interface::receiveData()
{
    int ret;
    int8_t read_bytes = 0;
    int timeout = 500;
 
    if (ComPort != NULL)
    {
        ret = poll(fdsR, 1, timeout);
        if (ret > 0)
        {
        // Receive data num bytes over UDP and put them at the sensors address
        read_bytes = ComPort->readBytes(rbuff, sizeof(struct UE_RecData));
        //printf("Got Data, %d Bytes\n", read_bytes);
        }
        if (ret == 0)
            printf("UE_Interface::receiveData | Waiting for data TIMEOUT!\n");
    }
    
    if (read_bytes == sizeof(UE_RecData))
    {
        memcpy((void*) &UEDataIn, (void* ) &rbuff[0], read_bytes);
        //  printf("Nx = %1.2f, Ny = %1.2f, Nz = %1.2f, Pen = %3.2f\n", 
        //  UEDataIn.Nx, UEDataIn.Ny, UEDataIn.Nz, UEDataIn.PenDepth);
        return 1;
    }
    else
        return 0;
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

int UE_Interface::getCollision(float impV[3], float* pen)
{
    pthread_mutex_lock(&mut_recData);
    impV[0] = UEDataIn.Nx;
    impV[1] = UEDataIn.Ny;
    impV[2] = -UEDataIn.Nz;
    
    *pen = 1e-2 * UEDataIn.PenDepth;
    pthread_mutex_unlock(&mut_recData);
    return 1;
}

