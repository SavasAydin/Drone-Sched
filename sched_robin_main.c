/*!
*   @file sched_robin_main.c
*   
*   @brief main file of sched module to schedule the tasks of basic processes
*					 Assumed a cycle is 50 ms since motor needs a pulse every 50 ms.
*	 				 tasks list consists of;
*	 				 motor, ca_task1, ca_tasks2, movement, stab_task1, stab_task2 
*	 				 If there is enough time left to perform a task according to 
* 					 task order except motor task (since once in 50 ms)
*					 then perform the idle task
*					 Damn I couldnt explain the algorithm behind :)   
*
*   @author Savas Aydin
*
*		@reference 
*						
*   @date 2011-03-29
*
*   @history    2011-03-29 - initial version of scheduling tasks of other 
*														 groups													
*								2011-03-31 - comments added and changes done according to
*														 coding standards
*								2011-04-06 - integrated with Anders scheduler implementation
*								2011-04-13 - added flags
*								2011-15-17 - final review - Reason to final review of code 
*														 even though we didnt integrate, it will use to
*														 be graded from project.											
*						
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
#include <time.h>
#include <stdint.h>

/* header from Anders for simulation  */
#include "sched_scheduler.h"

/* to simulate other modules */
#include "sched_robin_simulating.h"

#define NO_PROCESSES 6

Process *g_processArr[NO_PROCESSES];
/*it is temporary array to switch tasks with g_processArr */
Process *waiting_processArr[NO_PROCESSES];

/* Sets up all the processes and tasks */
void sched_setup(void)
{
 Process *process = sched_createProcess("Motor Process");
 Task *task = sched_createTask("runMotor", &motor, 2);
 sched_enqueueTask(process, task);
 g_processArr[0] = process;

 process = sched_createProcess("CA Process");
 task = sched_createTask("runCA1", &ca, 7);
 sched_enqueueTask(process, task);
 g_processArr[1] = process;

 process = sched_createProcess("CA Process");
 task = sched_createTask("runCA2", &ca, 3);
 sched_enqueueTask(process, task);
 g_processArr[2] = process;
	
 process = sched_createProcess("Move Process");
 task = sched_createTask("runMove", &mov, 8);
 sched_enqueueTask(process, task);
 g_processArr[3] = process;

 process = sched_createProcess("Stability Process");
 task = sched_createTask("runStab1", &stab, 5 );
 sched_enqueueTask(process, task);
 g_processArr[4] = process;
	
 process = sched_createProcess("Stability Process");
 task = sched_createTask("runStab2", &stab, 10);
 sched_enqueueTask(process, task);
 g_processArr[5] = process;

}

/* In current implementation Loop will only run once 
* i.e. Run one task for each process */
void sched_loop(void)
{
 time_t start, end;
 int i=0; 
 int remaining_time; // time left to complete a cycle
 int total_burst=0; // counter to calculate total execution time of tasks 
 int tracker = 0; // counter for each cycle to track the next task 
 int j=0;
 while(1) //endless loop
 {
	Process *process = g_processArr[i];
	Process *nextProc = g_processArr[i+1];
	start = time(NULL);
	sched_runIdleTask(process);
	if(time(NULL) - start >= process->idleTask->duration)
	{
	 sleep(start);
	 start = time(NULL);
	}
	total_burst  += process->idleTask->duration;
	remaining_time = 50 - total_burst;
	printf("remaining time for a cycle is = %d\n",remaining_time);

/* while it is not first cycle and waiting list is not empty 
   temporary list holds tasks left from previous cycle */
	while(tracker > 0 && waiting_processArr[i] != NULL)
	{
	 printf("next process = %s and execution time of next process %d\n", 
	 				g_processArr[i]->idleTask->name, 
	 				g_processArr[i]->idleTask->duration);						
   sched_runIdleTask(waiting_processArr[i]);
	 /*remaining time calculation performs after every task is done */
	 total_burst  += waiting_processArr[i]->idleTask->duration;
	 remaining_time = 50 - total_burst;	
	 printf("remaining time for a cycle is = %d\n",remaining_time);
	 i++;
	}	
	/* free static array to use it for next cycle */
	for(j=0;j<NO_PROCESSES;j++)
	{
	 waiting_processArr[j] = NULL;
	}	
	/* to eleminate the segmentation fault */
	if(tracker > 0)
	{
	 i=5;
	}
	i++;
	/* To complete cycle of 50 ms we may perform more tasks then in list*/
	if(i >= NO_PROCESSES)
	{
   i=1;
   /* if there is time left then perform next idle task */
	 while(remaining_time > g_processArr[i]->idleTask->duration)
	 {	
		printf("next process = %s and execution time of next process %d\n",
					 g_processArr[i]->idleTask->name, 
					 g_processArr[i]->idleTask->duration);				
    sched_runIdleTask(g_processArr[i]);
		total_burst += g_processArr[i]->idleTask->duration;
		remaining_time = 50 - total_burst;
		printf("remaining time for a cycle is = %d\n",remaining_time);
		i++;
		/* 
		 * Of course we need to go back to array 
		 * to stop segmentation fault 
		 */
		if(i >= NO_PROCESSES)
		{
		 i=1;
		}		
	 }
	 j=0;
	 /* add remaining tasks into waiting list to perform at next cycle */
	 for(i; i < NO_PROCESSES; i++)
	 {
		waiting_processArr[j] = g_processArr[i];
		printf("remaining process to complete a cycle is %s \n", 
					 waiting_processArr[j]->name);
		j++;
   }		
	 /* 
	  * if there is not enough time left to perform next task
		*	then sleep as much as remaining time and reset the calculaters
		*/
	 sleep(1);
	 total_burst=0;
	 i=0;
	 tracker++;
	 printf("\n\n");
	}
 }
 
}

void sched_endProcesses(void)
{
 int i;
 for(i = 0; i < NO_PROCESSES; i++)
 {
	sched_endProcess(g_processArr[i]);
 }
 
}


int sched_run(void)
{
 sched_setup();
 sched_loop();
 sched_endProcesses();
 return 0;

}

/* needed main fun. to test */
int main(void)
{
 sched_run();
 
 return 0;
}
