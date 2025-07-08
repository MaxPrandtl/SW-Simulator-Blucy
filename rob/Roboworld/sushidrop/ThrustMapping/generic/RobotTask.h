/*
 * RobotTask.h
 *
 *  Created on: Apr 22, 2020
 *      Author: mx
 */

#ifndef DRIVERS_ROBOTTASK_H_
#define DRIVERS_ROBOTTASK_H_

#include <semaphore.h>
#include "../generic/RobotThread.h"
#include "../generic/define.h"
#include "../generic/custom_types.h"
#include "../generic/Variable.h"
#include "../data/Task_execution.h"
#include "../data/Time_status.h"
#include "../data/DataAccess.h"



class RobotTask:public RobotThread
{
	protected:
		DataAccess<Task_execution> *task_access;
		DataAccess<Time_status>* time_access;
		char PARAMS_FILENAME[BUF_SIZE];

	public:
		RobotTask(const char* name, const char* Params_file, int policy, int priority, void* (*threadRoutine)(void*));

		~RobotTask();

	
		virtual void init() {}
		
		virtual void execute() = 0;


		int read_config();
		int write_config();

		virtual void get_params() {}
		virtual void set_params() {}

		virtual void parse_config(char* value, double val) {}
		virtual void set_config(FILE *f) {}

};

#endif /* DRIVERS_ROBOTTASK_H_ */
