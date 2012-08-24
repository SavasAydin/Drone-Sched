/*!
*   @file sched_robin_simulating.c
*   
*   @brief The idea of simulating round-robin algorithm 
*				   is to create a structure for each process 
*				   and assign name, CPU_burst, working and waiting time 
*				   to be used  for round robin algorithm 
*
*   @author Savas Aydin
*
*		@reference 
*						
*   @date 2011-03-29
*
*   @history    2011-03-29 - initial version													
*								2011-03-31 - comments added and changes done according to
*														 coding standards
*								2011-04-06 - changes on schedling algorithm implementation
*														 make this file more or less useless
*								2011-04-13 - added flags
*/

#ifdef PC
#include <stdio.h>
#include <stdlib.h>
/* PC headers */

#elif ARDUINO
#include "WProgram.h"
#endif /* PC ARDUINO */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "sched_robin_simulating.h"

PROCESS *proc;
   
/* Simulating stabilization interface */
int16_t stab(void)
{   
/* Allocate memory for proc*/
 proc = (PROCESS*)malloc(sizeof(PROCESS));
 if(proc == NULL)
 {
  printf("ERROR!\n");
  return 1;
 }  
 proc->name = "STAB";
 /*  sched_round_robin(proc); */
 printf("inside %s\n",proc->name);
 /*  free(proc); */
 return 0;
  
}

/* Simulating stabilization interface */
int16_t ca(void)
{
 proc = (PROCESS*)malloc(sizeof(PROCESS));
 if(proc == NULL)
 {
  printf("ERROR!\n");
  return 1;
 }    
 proc->name = "COLLESION AVOIDENCE";
 /*  sched_round_robin(proc); */
 printf("inside %s\n",proc->name);
 /*  free(proc); */
 return 0;
  
}

int16_t motor(void)
{
 proc = (PROCESS*)malloc(sizeof(PROCESS));
 if(proc == NULL)
 {
  printf("ERROR!\n");
  return 1;
 }  
 proc->name = "MOTOR";
 printf("inside %s\n",proc->name);
 /* sched_round_robin(proc); */
 /*  free(proc); */
 return 0;
 
}

int16_t mov(void)
{
 proc = (PROCESS*)malloc(sizeof(PROCESS));
 if(proc == NULL)
 {
  printf("ERROR!\n");
  return 1;
 }  
 proc->name = "MOVEMENT";
 printf("inside %s\n",proc->name);
 /* sched_round_robin(proc); */
 /*  free(proc); */
 return 0;

}

/* 
 * According to algorithm, process CPU_burst time cant be more than 
 * quantum time. If CPU_burst time less than quantum than process time 
 * will be CPU_burst time instead quantum. 
 * Quantum time is pre-fix time for each process. 
 * Here is the algorithm analysis:
 * http://choices.cs.uiuc.edu/~f-kon/RoundRobin/node2.html 
 * http://www.springerlink.com/content/7tx6672754685622/    
 */

void sched_round_robin(PROCESS *temp)
{  
 static int count=0;
 if((temp->CPU_burst <=QUANTUM) && (temp->CPU_burst != 0))
 {
  printf("process %s from %d to %d\n",temp->name, count,(count + proc->CPU_burst));
  count += proc->CPU_burst;
 }
 else
 {
  printf("process %s from %d to %d\n",temp->name, count,(count + QUANTUM));    
  count += QUANTUM;
 }
 free(temp);
 
}

