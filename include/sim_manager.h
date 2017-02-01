/**
 * @file sim_manager.h
 *
 * @brief Multi Simulator Manager
 *
 * Class for managing multiple simulators
 *
 * @author Luigi Pannocchi, <l.pannocchi@gmail.com>
 *
 */

#ifndef SIM_MANAGER_H_
#define SIM_MANAGER_H_

#ifndef MAXAGENTS
    #define MAXAGENTS 500
#endif

// ------------------------------------------------------------------
//   Includes
// ------------------------------------------------------------------
#include "simulator_interface.h"
#include <vector>

class Sim_Manager {
    
    public:
        Sim_Manager(); /*!< Default Constructor */
        ~Sim_Manager(); /*!< Default Destructor */
        
        /*!
         * Add agent to the list of connected Agents
         * \param Id Index of the added system 
         */
        int add_simulator(int Id);
        
        /*!
         * Add agent to the list of connected Agents
         * specifying also the initial position
         * \param Sys_Id Index of the added system 
         * \param Lat Initial Latitude
         * \param Lon Initial Longitude
         */
        Simulator_Interface* add_simulator(int Sys_Id, float Lat, float Lon);
        
        /*!
         * Remove the i-th simulator from the list of active
         * simulators
         * 
         * \param i Index of the agent to be removed
         */
        int remove_simulator(int i);
        
        /*! Return the number of active simulator */
        int getNumberOfSimulator();
        
        /*! Return the system id of the system in position pos */
        int getSimulatorId(int pos);
        
        /*!
         * Get the reference to the i-th simulator class
         */
        Simulator_Interface* getSimulatorInstance(int id);
    
    private:
        
        int ConnectedSimulators; /*!< Number of connected Agents */
        
        std::vector<Simulator_Interface*> pSI; /*!< Pointer to the dynamically allocated Simulator_Interface Classes */
    
};

#endif // SIM_MANAGER_H_
