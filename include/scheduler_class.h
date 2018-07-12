/** @file: scheduler_class.h
*
* This class represent a scheduler
*/

class MKscheduler
{
public:
    
    // Basic Constructor/Destructor
    /**
     * Constructor
     */
    MKscheduler(int M, int K);
    
    /**
     * Destructor
     */
    ~MKscheduler();

    // Methods
    void setM(int M);
    void setK(int K);
    
    int getM();
    int getK();
    
    bool sched_query();

private:
    int M; /// Max allowed deadline misses in a window
    int K; /// Scheduling window
    
    int accumulator; /// Counter of the accumulated misses in a window K
    bool* act_buff; /// Pointer to a buffer for the actuation pattern of the moving window
};

