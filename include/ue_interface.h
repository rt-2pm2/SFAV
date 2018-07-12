/**
 *
 *  \file ue_interface.h
 *
 *  \brief Unreal Engine interface class definition
 * 
 *  This code constitutes the interface for the communication with the
 *  Unreal Engine Application
 * 
 * \author Luigi Pannocchi, <l.pannocchi@gmail.com>
 *
 */

#ifndef UE_INTERFACE_H_
#define UE_INTERFACE_H_

#include "udp_port.h"
#include <time.h>
#include <poll.h>
#include <vector>

extern "C" {
#include "ptask.h"
}


struct UE_SendData
{
	// Drone Index
	int Id;

	// Position
	float X;
	float Y;
	float Z;

	// Orientation
	float r;
	float p;
	float y;
};

struct UE_RecData
{
	// Drone Index
	int Id;

	// Impact Normal 
	float Nx;
	float Ny;
	float Nz;

	// Penetration 
	float PenDepth;
};




class UE_Interface {

public:

    // Constructor
    UE_Interface();
    UE_Interface(char* ip, uint32_t r_port, uint32_t w_port, uint32_t v_port);

    // Destructor
    ~UE_Interface();

    // Settings
    int setIP(char* ip);
    int setBaseReadPort(unsigned int port);
    int setBaseWritePort(unsigned int port);
    int setBaseVideoPort(unsigned int port);

    // Communication
    int sendData(struct UE_SendData);

    int receiveData();
    int getData(struct UE_RecData* data);
    int getCollision(float impV[3], float* pen);

    int started;

    int add_port(int offset);

    bool active;
    
private:

    int NVehicles;
    char net_ip[15];
    uint32_t r_port, w_port, v_port;

    pthread_mutex_t mut_sendData;
    pthread_mutex_t mut_recData;

    struct pollfd fdsR[1];
    struct pollfd fdsW[1];

    Udp_Port* ComPort; /*!< Communication port */

    char rbuff[512];
    
    struct UE_SendData UEDataOut;
    
    struct UE_RecData UEDataIn;
};

#endif // UE_INTERFACE_H_
