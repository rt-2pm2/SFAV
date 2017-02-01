/*--------------------------------------------------------------*/
/*		PTASK LIBRARY - HEADER FILE			*/
/*--------------------------------------------------------------*/

#ifndef __PTASK_H__
#define __PTASK_H__

#include <pthread.h>
#include <semaphore.h>
#include <ptime.h>
#include <rtmode.h>

/*--------------------------------------------------------------*/

#define	MAX_TASKS	50
#define	MAX_GROUPS	10

/* activation flag for task_create */
#define	DEFERRED	0
#define	NOW		1

typedef enum {PARTITIONED, GLOBAL} global_policy;
typedef enum {PRIO_INHERITANCE, PRIO_CEILING, NO_PROTOCOL} sem_protocol;
typedef enum {TASK_SUSPENDED, TASK_ACTIVE, TASK_WFP} ptask_state;

typedef void ptask;

/**
   This structure is used to simplify the creation of a task by
   setting standard arguments
 */
typedef struct {
    tspec period; 
    tspec rdline;
    int priority;              /*< from 0 to 99                         */
    int processor;             /*< processor id                         */
    int act_flag;              /*< NOW if the create activates the task */
    
    int measure_flag;          /*< if 1, activates measure of exec time */
    void *arg;                 /*< pointer to a task argument           */
    rtmode_t *modes;           /*< a pointer to the mode handler        */
    int mode_list[RTMODE_MAX_MODES];  /*< the maximum number of modes   */
    int nmodes;               /*< num of modes in which the task is act */
} tpars;

extern const tpars TASK_SPEC_DFL;

/*----------------------------------------------------------------------------*/
/*             SETTING PARAMETERS FOR CREATING A TASK                         */
/*----------------------------------------------------------------------------*/

#define ptask_param_init(s) \
    (s = TASK_SPEC_DFL)

#define ptask_param_period(s, p, u) \
    (s.period = tspec_from(p, u))

#define ptask_param_deadline(p, deadline, unit) \
    (p.rdline = tspec_from(deadline, unit))

#define ptask_param_priority(p, prio) \
    (p.priority = prio)

#define ptask_param_processor(p, processor) \
    (p.processor = processor)

#define ptask_param_activation(p, mode) \
    (p.act_flag = mode)

#define ptask_param_measure(p) \
    (p.measure_flag = 1)

#define ptask_param_argument(p, arg) \
    (p.arg = arg)

#define ptask_param_modes(p, modes, nmodes) \
    (p->modes = modes, p->nmodes = nmodes)

#define ptask_param_mode_add(p, n, mode_num) \
    (p->mode_list[n] = mode_num)


/* ------------------------------------------------------------------ */
/*                     GLOBAL FUNCTIONS                               */
/* ------------------------------------------------------------------ */
int  ptask_getnumcores(); /*< returns the number of available cores   */

/** The following function initializes the library. The policy can be:
    - SCHED_FIFO      fixed priority, fifo for same priority tasks
    - SCHED_RR        fixed priority, round robin for same priority tasks 
    - SCHED_OTHER     classical Linux scheduling policy (background) 
    
    The global policy can be:
    - GLOBAL          global scheduling with migration
    - PARTITIONED     partitioned scheduling (no migration)

    The semaphore protocol can be:
    - PRIO_INHERIT
    - PRIO_CEILING
*/ 
void  ptask_init(int policy,
		 global_policy global, 
		 sem_protocol protocol); 

/** Prints an error message on stderr and exits. The first string
    should contain the name of the failing function, the second string
    should contain a description of the error */
void  ptask_syserror(char *fun, char *msg); 


/*--------------------------------------------------------------------- */
/*			TASK CREATION                                   */
/*----------------------------------------------------------------------*/
/** 
    Creates a task with a certain task body, a period, a priority and an
    activation flag */
int  ptask_create(void (*task)(void),
		  int period, int prio, int aflag);

/** 
    Creates a task with a set of parameters. If tp is NULL, by default 
    the period and the relative deadline will be equal to 1 second, the 
    priority is 1 (lowest), and the activation flag is DEFERRED */
int  ptask_create_param(void (*task)(void), tpars *tp);

/*-------------------------------------------------------------------------- */
/*			TASK FUNCTIONS                                       */
/*---------------------------------------------------------------------------*/
void      ptask_wait_for_period();   /*< waits for next period or activation */
void	  ptask_wait_for_activation(); /*< waits for an exp. activation      */
int       ptask_get_index();        /*< returns the task own index           */
int	  ptask_deadline_miss();    /*< true is the task missed its deadline */
void *    ptask_get_argument();     /*< returns the task argument            */

/* Global functions on tasks */
int 	  ptask_activate(int i); /*< activates the task of idx i              */
int	  ptask_activate_at(int i, ptime off); /*< activates the task of idx i*/
pthread_t ptask_get_threadid(int i);    /*< returns the thread id of task i   */
ptask_state ptask_get_state(int i);  /*< return the current task state        */

/*----------------------------------------------------------------------------*/
/*             Dinamically changing parameters                                */
/*----------------------------------------------------------------------------*/

int	  ptask_get_deadline(int i, int unit); /*< return current rel. deadline */
void	  ptask_set_deadline(int i, int dline, int unit); /*< set rel. deadline */

long	task_absdl(int i); /*< return current abs. deadline */

int	  ptask_get_period(int i, int unit); /*< return current period        */
void	  ptask_set_period(int i, int per, int unit); /*< set period          */

int	  ptask_get_priority(int i); /*< return current priority              */
void	  ptask_set_priority(int i, int prio); /*< sets task priority         */

/** Returns the cpuid where the task has been allocated */
int       ptask_get_processor(int i);           
/** Migrate task to processor cpuid*/
int       ptask_migrate_to(int i, int cpuid); 


#endif

/*--------------------------------------------------------------*/

