/**
 * \file ma_manager.cpp
 * 
 * \brief Multi Agent ma_manager
 * 
 */

// ------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------
#include "ma_manager.h"



MA_Manager::MA_Manager()
{
    pthread_mutex_init(&ma_mutex, 0);
    
    ConnectedAgents = 0;
}

MA_Manager::~MA_Manager()
{
    int i;
    int NElements = pAI.size();
    
    // Delete the Allocated Memory for the Classes
    for (i = 0; i < NElements; i++)
    {
        delete pAI.at(i);    // Destroy the Class
        pAI.pop_back();
    }
    
    // Clear the List
    pAI.clear();
}


Autopilot_Interface* MA_Manager::add_agent(char* ip, uint32_t r_port,
                          uint32_t w_port, bool synch)
{
    // Create a new Autopilot Interface class and add it to the
    // vector of the connected agents
    Autopilot_Interface* pAi;
    pAi = new Autopilot_Interface(ip, r_port, w_port, synch);

    pthread_mutex_lock(&ma_mutex);
    pAI.push_back(pAi);
    ConnectedAgents++;
    pAi->setSystemIndex(ConnectedAgents);
    pthread_mutex_unlock(&ma_mutex);
    
    return pAi;
}

Autopilot_Interface* MA_Manager::add_agent(char*& uart_name, int baudrate, bool synch)
{
    // Create a new Autopilot Interface class and add it to the
    // vector of the connected agents
    Autopilot_Interface* pAi = NULL;
    pAi = new Autopilot_Interface(uart_name, baudrate, synch);

    pthread_mutex_lock(&ma_mutex);
    pAI.push_back(pAi);
    ConnectedAgents++;
    pAi->setSystemIndex(ConnectedAgents);
    pthread_mutex_unlock(&ma_mutex);

    return pAi;
}

int MA_Manager::remove_agent(int Id)
{
    int i, N;
    std::vector<Autopilot_Interface*>::iterator iter;
    pthread_mutex_lock(&ma_mutex);
    iter = pAI.begin();
    if (getNumberOfAgents() == 0)
    {
        printf("Empty List!\n");
        return -1;
    }
    else
    {
        N = pAI.size();
        for (i = 0; i < N; i++)
        {
            if (pAI.at(i)->getSystemId() == Id)
            {
                delete pAI.at(i);    // Destroy the Class
                pAI.erase(iter + i); // Remove the vector element
                ConnectedAgents--;
                pthread_mutex_unlock(&ma_mutex);
                return 1;
            }
        }
    }
    pthread_mutex_unlock(&ma_mutex);
    printf("System not found!\n");
    return -1;
}

int MA_Manager::getNumberOfAgents()
{
    return ConnectedAgents;
}

Autopilot_Interface* MA_Manager::getAutopilotInstance(int id)
{
    int i, N;

    pthread_mutex_lock(&ma_mutex);
    N = getNumberOfAgents();
    for (i = 0; i < N; i++)
    {
        
        if (pAI.at(i)->getSystemId() == id)
        {
            pthread_mutex_unlock(&ma_mutex);
            return pAI.at(i);
        }
    }
    printf("Autopilot Instance not found!\n");
    pthread_mutex_unlock(&ma_mutex);
    return NULL;
}

Autopilot_Interface* MA_Manager::getLastAutopilotInstance()
{
    int N;
    Autopilot_Interface* p;
    pthread_mutex_lock(&ma_mutex);
    N = getNumberOfAgents();
    if (N == 0)
    {
        printf("No Autopilot Instance added to the System!\n");
        p = NULL;
    }
    else
    {
        p = pAI.back();
    }
    pthread_mutex_unlock(&ma_mutex);
    return p;
}


int MA_Manager::sendMessage2UAV(mavlink_message_t* message, int uavId)
{
    int i, N, bytesent;
    //printf("Sending Message %d to UAV %d\n", message->msgid, uavId);
    
    if (getNumberOfAgents() == 0)
    {
        printf("Empty List!\n");
        return -1;
    }
    else
    {
        N = pAI.size();
        if (uavId == 255)
        {
            for (i = 0; i < N; i++)
                bytesent = pAI.at(i)->send_message(message);
        }
        else
        {
            for (i = 0; i < N; i++)
            {
                if (pAI.at(i)->getSystemId() == uavId)
                {
                    bytesent = pAI.at(i)->send_message(message);
                    return bytesent;
                }
            }
            printf("UAV not found!\n");
            return -1;
        }
        return 1;
    }
}
