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
    UE_Interface(char* ip, uint32_t r_port, uint32_t w_port);

    // Destructor
    ~UE_Interface();

    // Settings
    int setReadPort(unsigned int port);
    int setBaseWritePort(unsigned int port);

    // Communication
    int sendData(int Id);
    int setData(struct UE_SendData);

    int receiveData();
    int getData(struct UE_RecData* data);
    
    Udp_Port* getPortInstance(int id);

    int started;

    int add_port(int offset);
    Udp_Port udp_port;


private:

    int NVehicles;
    char net_ip[15];
    uint32_t r_port, w_port;

    pthread_mutex_t mut_sendData;
    pthread_mutex_t mut_recData;

    struct pollfd fdsR[1];
    struct pollfd fdsW[1];

    std::vector<Udp_Port*> pPorts; /*!< Pointer to the dynamically allocated communication ports */

    char rbuff[512];
};

#endif // UE_INTERFACE_H_
