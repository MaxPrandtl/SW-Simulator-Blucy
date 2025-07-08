/*
 * Filter.h
 *
 *  Created on: Apr 22, 2020
 *      Author: mx
 */

#ifndef _FILTER_H_
#define _FILTER_H_

#include <semaphore.h>
#include "../generic/RobotThread.h"
#include "../generic/define.h"
#include "../generic/custom_types.h"
#include "../generic/Variable.h"
#include "../generic/RobotTask.h"


void* start_filter(void* arg);


class Filter:public RobotTask
{
	protected:
		

	public:
		Filter(const char* name, const char* Params_file);
		~Filter();

		virtual void compute()=0;
		virtual void init() {}
		
		virtual void execute();
};






#endif /* _FILTER_H_ */
