/**
 *
 * \file: simulator_interface.cpp
 * \author : Luigi Pannocchi <l.pannocchi@gmail.com>
 *
 */

#include "simulator_interface.h"
#include <stdio.h>
#include <string.h>

// --------------------------------------
// Simulation Interface Class
// --------------------------------------

Simulator_Interface::Simulator_Interface()
{
    Initialize_Interface();
}

Simulator_Interface::Simulator_Interface(int Id)
{
    int i;
    /// Allocate and Initialize the Simulation Structure
    Initialize_Interface();

    sysId = Id;
}

Simulator_Interface::Simulator_Interface(int Id, char *ip, uint32_t w_port)
{
    int i;
    /// Allocate and Initialize the Simulation Structure
    Initialize_Interface();

    sysId = Id;
    
    udp_port = w_port;
    
    ComPort = new Udp_Port();
   
    // Initialize UDP Communication for debug purpose
    for (i = 0; i < strlen(ip); i++)
        net_ip[i] = ip[i];
    ComPort->InitializeOutputPort(net_ip, udp_port + sysId);
}

Simulator_Interface::~Simulator_Interface()
{
    DynModel_terminate(SimModel);
    if (ComPort != NULL)
        delete ComPort;
    printf("Destructor of Simulator/n");
}



// ------------------------------------------------------
//  FUNCTIONS
// ------------------------------------------------------ 

void Simulator_Interface::Initialize_Interface()
{
    int i;
    
    /// Allocate and Initialize the Simulation Structure
    SimModel = DynModel();
    DynModel_initialize(SimModel);
    
    memset(&sensors, 0, NUM_FLOAT_SENSORS * sizeof(float));
    memset(&act_controls, 0, NUM_FLOAT_ACT_CONTROLS * sizeof(float));
    
    SimModel->ModelData.inputs->pen_collision = 0.0;
    for (i = 0; i < 3; i++)
    {
        SimModel->ModelData.inputs->Fext[i] = 0.0;
        SimModel->ModelData.inputs->n_collision[i] = 0.0;
    }
    sysId = 1;
    ComPort = NULL;
}


void Simulator_Interface::setInitialCoordinates(float Lat, float Lon)
{
    Initial_LL[0] = Lat;
    Initial_LL[1] = Lon;
    
    SimModel->ModelData.inputs->Initial_LL[0] = Initial_LL[0];
    SimModel->ModelData.inputs->Initial_LL[1] = Initial_LL[1]; 
}

// Set the active flag
void Simulator_Interface::simulationActive()
{
    simulator_thread_active = true;
}

// Return the Identifier of the simulated vehicle
int Simulator_Interface::getSystemId()
{
    return sysId;
}

// Update PWM commands
void Simulator_Interface::updatePWM(float pwm[NUM_FLOAT_ACT_CONTROLS])
{
    SimModel->ModelData.inputs->PWM1 = pwm[2];
    SimModel->ModelData.inputs->PWM2 = pwm[0];
    SimModel->ModelData.inputs->PWM3 = pwm[3];
    SimModel->ModelData.inputs->PWM4 = pwm[1];
}

// Update collision information
void Simulator_Interface::updateCollision(float impnorm[3], float pen_collision)
{
    int i;
    // Collision informations and external forces
    for (i = 0; i < 3; i++)
    {
        SimModel->ModelData.inputs->n_collision[i] = impnorm[i];
    }
    SimModel->ModelData.inputs->pen_collision = pen_collision;
}

// Update external forces
void Simulator_Interface::updateFext(float Fext[3])
{
    int i;
    // Collision informations and external forces
    for (i = 0; i < 3; i++)
    {
        SimModel->ModelData.inputs->Fext[i] = Fext[i];
    }
}

// Overall update function
void Simulator_Interface::updateActCtr(float pwm[NUM_FLOAT_ACT_CONTROLS], 
                             float impnorm[3], float pen_collision, 
                             float Fext[3])
{
    updatePWM(pwm);
    updateCollision(impnorm, pen_collision);
    updateFext(Fext);
}

// Retrieve simulated output data
void Simulator_Interface::getSimOutput(struct SimOutput* simout)
{
    memcpy(simout, SimModel->ModelData.outputs, sizeof(struct SimOutput));
}

// Retrieve simulated output data
void Simulator_Interface::getSimPosAtt(float Xe[3], float Att[3])
{
    memcpy(Xe, SimModel->ModelData.outputs->Xe, 3 * sizeof(float));
    memcpy(Att, SimModel->ModelData.outputs->RPY, 3 * sizeof(float));
}

// Send debug information
void Simulator_Interface::DBGsendSimPosAtt()
{
    float outvect[6];
    
    memcpy(outvect, SimModel->ModelData.outputs->Xe, 3 * sizeof(float));
    memcpy(&outvect[3], SimModel->ModelData.outputs->RPY, 3 * sizeof(float));
    
    if (ComPort)
        ComPort->writeBytes((char* )outvect, 6 * sizeof(float));

    else
        printf("No DBG port specified \n");
}


// Do a simulation step
void Simulator_Interface::simStep()
{    
    DynModel_step(SimModel);
}
