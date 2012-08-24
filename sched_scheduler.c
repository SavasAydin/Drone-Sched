/*!
*   @file sched_scheduler.c
*   
*   @brief Functions for the robin scheduler implementation.
*
*   @author Savas Aydin
*
*		@reference Anders Treptow
*							 It is written by Anders, and I just customized and updated 
*							 to use it for my implementation.
*
*/

#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <stdint.h>

#include "sched_scheduler.h"

/* TODO: Fix const construction of processes */
Process* sched_createProcess(char *name)
{
 int strLen = strlen(name) + 1;
 Process *process = (Process*)malloc(sizeof(Process));
 memset(process, 0, sizeof(Process));
 process->name = (char*)malloc(strLen);
 memset(process->name, 0, sizeof(strLen));
 strncpy(process->name, name, strLen);
 return process;
 
}

/* Terminates a process completely */
void sched_endProcess(Process *process)
{
 sched_removeProcessTasks(process->firstTask);
 free(process->name);
 free(process);
 
}

/* TODO: Fix const construction of tasks */
Task* sched_createTask(char *name, Fun_t functionPointer, int duration)
{
 int strLen = strlen(name) + 1;
 Task *task = (Task*)malloc(sizeof(Task));
 memset(task, 0, sizeof(Task));
 task->name = (char*)malloc(strLen);
 memset(task->name, 0, strLen);
 strncpy(task->name, name, strLen);
 task->functionPointer = functionPointer;
 task->duration = duration;
 return task;
 
}

/* Removes all enqueued tasks for a process */
void sched_removeProcessTasks(Task *task)
{
 if(task->nextTask != NULL)
 {
	sched_removeProcessTasks(task->nextTask);
	free(task->name);
	free(task);
 }
 else
 {
	free(task->name);
	free(task);
 }
 
}

/* Enqueues a task to a structer at the last position of queue */
void sched_enqueueTask(Process *process, Task *task)
{
 Task *tmpTask = process->lastTask;
 if(tmpTask != NULL)
 {
	while(tmpTask != NULL)
  {
	 tmpTask = tmpTask->nextTask;
	}
	tmpTask = task;
	process->lastTask = task;
 }
 else
 {
	process->firstTask = task;
	process->lastTask = task;
	process->idleTask = task;
 }
	process->no_tasks++;
	
}

// TODO
void sched_dequeueTask(Process *process, Task *task)
{ }

/* Runs the idle task for the process */
void sched_runIdleTask(Process *process)
{
 if(process->firstTask != NULL)
 {
	Task *tmpTask = process->idleTask;
	Fun_t fp = tmpTask->functionPointer;

	if(tmpTask->nextTask != NULL)
	{
	 process->idleTask = tmpTask->nextTask;
	}
	else
	{
	 process->idleTask = process->firstTask;
	}
	fp();
 }
 
}

