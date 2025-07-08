/*
 * Control.cpp
 *
 *  Created on: May 26, 2020
 *      Author: mx
 */


#include "Control.h"


Control::Control(const char* name, const char* Params_file) :RobotTask(name, Params_file, SCHED_FIFO, NGC_THREAD_PRIORITY, start_control)
{

}


Control::~Control()
{

}


void Control::execute()
{
	timespec tSleep;
	tSleep.tv_sec = NGC_SLEEP_SEC;
	tSleep.tv_nsec = NGC_SLEEP_NSEC;

	Task_execution task_exec;
	Time_status clock;

	while (running)
	{
		clock = time_access->get();
		task_exec = task_access->get();

		set_params();

		if (task_exec.command.updated == 1)
		{
			if (task_exec.command.value == TASK_GET_PARAMS) get_params();
			else task_exec.status.copyValueFrom(task_exec.command);

			task_exec.command.updated = 0;
		}

		task_exec.status.timeStamp = clock.timeStamp.value;
		task_exec.status.updated = 1;
		task_access->set(task_exec);



		if (task_exec.status.value == TASK_INIT)
		{
			init();
		}
		if (task_exec.status.value == TASK_RUNNING)
		{
			compute();
		}
		
		nanosleep(&tSleep, NULL);
	}
}



void* start_control(void* arg)
{
	Control* pThread = (Control*)((ThreadInfo*)arg)->pThread;
	pThread->execute();
	return NULL;
}

