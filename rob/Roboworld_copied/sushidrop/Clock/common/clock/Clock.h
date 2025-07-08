/*
 * clock.h
 *
 *  Created on: Mar 24, 2020
 *      Author: mx
 */

#ifndef CLOCK_H_
#define CLOCK_H_


#include "../../generic/define.h"
#include "../../generic/custom_types.h"
#include "../../generic/RobotThread.h"
#include "../../data/Time_status.h"
#include "../../data/DataAccess.h"




class Clock :public RobotThread
{
	private:
		DataAccess<Time_status>* time_access;

		timespec tDelay, tStart, tSleep;

	public:
		Clock(const char* name, DataAccess<Time_status>* Time_access);
		~Clock();

		virtual void execute();
};

void* start_clock(void* arg);


#endif /* CLOCK_H_ */
