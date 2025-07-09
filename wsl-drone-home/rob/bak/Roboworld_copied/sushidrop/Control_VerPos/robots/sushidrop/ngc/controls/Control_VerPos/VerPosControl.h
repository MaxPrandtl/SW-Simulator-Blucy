/*
 * VerPosControl.h
 *
 *  Created on: Jun 10, 2020
 *      Author: mx
 */

#ifndef VER_POS_CONTROL_H_
#define VER_POS_CONTROL_H_

#include <unistd.h>
#include <string>
#include "../../../../../generic/define.h"
#include "../../../../../generic/custom_types.h"
#include "../../../../../generic/RobotThread.h"
//#include "../../../../../generic/RobotMath.h"
#include "../../../../../data/Time_status.h"
#include "../../../../../data/NGC_status.h"
#include "../../../../../data/Task_execution.h"
#include "../../../../../data/DataAccess.h"
#include "../../../../../ngc/Control.h"
#include "VerPosControl_params.h"


class VerPosControl :public Control
{
	private:

		DataAccess<NGC_status>* ngc_access = NULL;
		DataAccess<VerPosControl_params>* params_access = NULL;

		VerPosControl_params params;

		double ie,zRef_prec;
		

	public:
		VerPosControl(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<VerPosControl_params>* Params_access);
		~VerPosControl();

		virtual void init();
		virtual void compute();
		
		virtual void parse_config(char* value, double val);
		virtual void set_config(FILE* f);

		virtual void get_params();
		virtual void set_params();
};

void* start_control(void* arg);

#endif /* VER_POS_CONTROL_H_ */
