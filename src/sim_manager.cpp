/**
 * @file sim_manager.cpp
 * 
 * \brief Multi Agent ma_manager
 * 
 */

// ------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------
#include "sim_manager.h"
#include <stdio.h>


Sim_Manager::Sim_Manager()
{
    ConnectedSimulators = 0;
}

Sim_Manager::~Sim_Manager()
{
    int i;
    int NElements = pSI.size();
    
    // Delete the Allocated Memory for the Classes
    for (i = 0; i < NElements; i++)
    {
        delete pSI.at(i);       // Destroy the Class
        pSI.pop_back();
    }
    
    // Clear the List
    pSI.clear();
}


int Sim_Manager::add_simulator(int Sys_Id, char* ip, uint32_t port)
{
    if (getNumberOfSimulator() + 1 > MAXAGENTS)
    {
        printf("Exceding the maximum number of Agents!\n"); 
        return -1;
    }
    else
    {
        // Create a new Autopilot Interface class and add it to the list
        Simulator_Interface* pSi;
        pSi = new Simulator_Interface(Sys_Id, ip, port);
        pSI.push_back(pSi);
        ConnectedSimulators++;
        return 1;
    }
}

Simulator_Interface* Sim_Manager::add_simulator(int Sys_Id, float Lat, float Lon, char* ip, uint32_t port)
{
    if (getNumberOfSimulator() + 1 > MAXAGENTS)
    {
        printf("Exceding the maximum number of Agents!\n"); 
        return NULL;
    }
    else
    {
        // Create a new Autopilot Interface class and add it to the list
        Simulator_Interface* pSi;
        if (port != 0)
            pSi = new Simulator_Interface(Sys_Id, ip, port);
        else
            pSi = new Simulator_Interface(Sys_Id);
        
        pSi->setInitialCoordinates(Lat, Lon);
        pSI.push_back(pSi);
        ConnectedSimulators++;
        return pSi;
    }
}

int Sim_Manager::remove_simulator(int Id)
{
    int i, N;
    std::vector<Simulator_Interface*>::iterator iter;
    iter = pSI.begin();
    
    if (getNumberOfSimulator() == 0)
    {
        printf("Empty List!\n");
        return -1;
    }
    else
    {
        N = pSI.size();
        for (i = 0; i < N; i++)
        {
            if (pSI.at(i)->getSystemId()== Id)
            {
                delete pSI.at(i);       // Destroy the Class
                pSI.erase(i + iter);    // Remove the vector element
                ConnectedSimulators--;
                return 1;
            }
        }
    }
    printf("Simulator not found!\n");
    return -1;
}

int Sim_Manager::getNumberOfSimulator()
{
    return ConnectedSimulators;
}

int Sim_Manager::getSimulatorId(int pos)
{
    int N = 0;
    int ret = 0;
    N = getNumberOfSimulator();
    if (pos <= N)
        ret = pSI.at(pos)->getSystemId();
    
    return ret;
}

Simulator_Interface* Sim_Manager::getSimulatorInstance(int id)
{
    int i, N;
    N = getNumberOfSimulator();
    for (i = 0; i < N; i++)
        if (pSI.at(i)->getSystemId() == id)
            return pSI.at(i);
        
    printf("Autopilot Instance not found!\n");
    return NULL;
}
