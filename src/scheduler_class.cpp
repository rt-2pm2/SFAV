/**
* @file: scheduler_class.cpp
*/

#include "scheduler_class.h"
#include <stdlib.h>
#include <stdio.h>

/**
 * Class constructor
 * 
 */
MKscheduler::MKscheduler(int M, int K)
{
    int i;
    
    setM(M);
    setK(K);
    
    accumulator = 0;
    
    act_buff = new bool[K];
    
    for (i  = 0; i < K; i++)
        act_buff[i] = true;
}

/**
 * Class destructor
 */
MKscheduler::~MKscheduler()
{
    delete act_buff;
}

int MKscheduler::getK()
{
    return K;
}

void MKscheduler::setK(int k)
{
    K = k;
}

int MKscheduler::getM()
{
    return M;
}

void MKscheduler::setM(int m)
{
    M = m;
}




bool MKscheduler::sched_query()
{
    int i;
    double r;
    
    // Shift the actuation buffer and count the
    // number of deadline misses in the window.
    accumulator = 0;
    for (i = 0; i < K-1; i++)
    {
        act_buff[i] = act_buff[i + 1];
        if (act_buff[i] == false)
            accumulator++; 
    }
    
    // Check whether the max number of deadline misses
    // could be exceded.
    if (accumulator >= M)
    {
        act_buff[K-1] = true; // Job hit
    }
    else
    {
        // I could skip the job
        r = (double)(rand())/RAND_MAX;
        if (r >= (1 / M) + (accumulator / M))
            act_buff[K-1] = false; // Skip the job
        else
            act_buff[K-1] = true; // Job hit
    }
    
    return act_buff[K-1];
}


