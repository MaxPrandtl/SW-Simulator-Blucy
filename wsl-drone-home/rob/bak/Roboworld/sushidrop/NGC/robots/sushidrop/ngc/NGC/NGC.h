/*
 * NGC.h
 *
 *  Created on: Dec 31, 2020
 *      Author: Mx
 */

#ifndef NGC_H_
#define NGC_H_

#include "../../../../generic/RobotThread.h"
#include "../../../../data/DataAccess.h"
#include "../../../../data/Time_status.h"
#include "../../../../data/NGC_status.h"
#include "../../../../data/Task_execution.h"



class NGC :public RobotThread
{
	private:
		DataAccess<Time_status>* time_access = NULL;
		DataAccess<NGC_status>* ngc_access = NULL;
		DataAccess<Task_execution>* control_angvel_access = NULL;
		DataAccess<Task_execution>* control_angpos_access = NULL;
		DataAccess<Task_execution>* control_horvel_access = NULL;
		DataAccess<Task_execution>* control_horpos_access = NULL;
		DataAccess<Task_execution>* control_vervel_access = NULL;
		DataAccess<Task_execution>* control_verpos_access = NULL;
		DataAccess<Task_execution>* control_alt_access = NULL;
		DataAccess<Task_execution>* pf_ua_access = NULL;
		
		
	public:
		NGC(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Control_angvel_access, DataAccess<Task_execution>* Control_angpos_access,
			DataAccess<Task_execution>* Control_horvel_access, DataAccess<Task_execution>* Control_horpos_access, 
			DataAccess<Task_execution>* Control_vervel_access, DataAccess<Task_execution>* Control_verpos_access, DataAccess<Task_execution>* Control_alt_access,
			DataAccess<Task_execution>* PF_ua_access);
		~NGC();

		virtual void execute();
};

void* start_ngc(void* arg);

#endif /* NGC_H_ */