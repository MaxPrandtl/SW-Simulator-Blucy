/*
 * Task_execution.h
 *
 *  Created on: Apr 22, 2020
 *      Author: mx
 */

#ifndef TASK_EXECUTION_H_
#define TASK_EXECUTION_H_

#include "../generic/custom_types.h"
#include "../generic/Variable.h"

enum TaskStatus
{
	TASK_OFF = 0,
	TASK_INIT = 2,
	TASK_RUNNING = 3,
	TASK_WARNING = 4,
	TASK_FAULT = 5,

	TASK_GET_PARAMS=6,
};


class Task_execution
{
private:

	void copy_data(const Task_execution& d)
	{
		command = d.command;
		status = d.status;
	}


public:

	IntVariable status,command;
	

	bool updated;

	Task_execution()
	{
		command.zero();
		status.zero();
	}


	Task_execution(Task_execution& d)
	{
		copy_data(d);
	}


	~Task_execution() {}


	Task_execution& operator=(const Task_execution& d)
	{
		copy_data(d);
		return *this;
	}

};

#endif /* TIME_STATUS_H_ */
