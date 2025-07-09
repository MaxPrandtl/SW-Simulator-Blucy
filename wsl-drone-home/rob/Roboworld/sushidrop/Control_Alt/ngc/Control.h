/*
 * Control.h
 *
 *  Created on: May 26, 2020
 *      Author: mx
 */

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include <semaphore.h>
#include "../generic/RobotThread.h"
#include "../generic/define.h"
#include "../generic/custom_types.h"
#include "../generic/Variable.h"
#include "../generic/RobotTask.h"


void* start_control(void* arg);


class Control:public RobotTask
{
	protected:
		

	public:
		Control(const char* name, const char* Params_file);
		~Control();

		virtual void compute()=0;
		virtual void init() {}
		
		virtual void execute();
};






#endif /* _CONTROL_H_ */
