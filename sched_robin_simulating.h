/*!
*   @file sched_robin_simulating.h
*   
*   @brief header file for scheD_robin_simulating.c
*
*   @author Savas Aydin
*
*		@reference 
*						
*   @date 2011-03-30
*
*   @history    2011-03-30 - initial version													
*								2011-03-31 - comments added and changes done according to
*														 coding standards
*/


#ifndef sched_simulating_functions_h
#define sched_simulating_functions_h

#include <stdint.h>

#define QUANTUM 5
#define TRUE 0
#define FALSE ~TRUE

/* each process has a name, burst, waiting and working time */
typedef struct sched_process{
  char *name;
  int CPU_burst;
  int OTC; // Original time slice (millisecond)
  int PC; // Priority component
  int SC; // Shortness Component based on CPU burst time
  int CSC; // Context switch component
  int ITS; // Intelligence time slice
}PROCESS;

/* other group interface */
int16_t stab();
int16_t mov();
int16_t ca();
int16_t motor();
void sched_round_robin(PROCESS *proc);

#endif
