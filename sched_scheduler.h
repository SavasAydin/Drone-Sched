/*!
*   @file sched_scheduler.h
*   
*   @brief header file for sched_scheduler.c
*
*   @author Savas Aydin
*
*		@reference Anders Treptow
*							 It is written by Anders, and I just customized and updated 
*							 to use it for my implementation.
*
*/

#ifndef SCHED_SCHEDULER_H
#define SCHED_SCHEDULER_H

/* type Fun_t is type "function that returns int and takes no arguments */
// TODO: make it possible to generalize a functionpointer
typedef int(*Fun_t)(void);

/* struct Task points to function which perfoms given task */
typedef struct tagTask
{
 char *name;
 int duration;
 Fun_t functionPointer;
 struct tagTask *nextTask;
}Task;

/* struct Process consists of 1...n number of Tasks put into an array */
typedef struct tagProcess
{
 char *name;
 int priority;
 int no_tasks;
 Task *firstTask;
 Task *lastTask;
 Task *idleTask;
}Process;

Process* sched_createProcess(char *name);
void sched_endProcess(Process *process);
Task* sched_createTask(char *name, Fun_t functionPointer, int duration); 
void sched_removeProcessTasks(Task *task);
void sched_enqueueTask(Process *process, Task *task);
void sched_dequeueTask(Process *process, Task *task);
void sched_runIdleTask(Process *process);

#endif
