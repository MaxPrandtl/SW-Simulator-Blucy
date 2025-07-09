/*
 * SushidropLogger.h
 *
 *  Created on: Apr 21, 2021
 *      Author: Mx
 */

#ifndef SUSHIDROPLOGGER_H_
#define SUSHIDROPLOGGER_H_

#include "../../../generic/RobotThread.h"
#include "../../../data/DataAccess.h"
#include "../../../data/Time_status.h"
#include "../../../data/NGC_status.h"
#include "../../../data/GPS_AHRS_status.h"
#include "../../../data/IO_sushidrop_status.h"


class SushidropLogger :public RobotThread
{
	private:
		DataAccess<Time_status>* time_access = NULL;
		DataAccess<NGC_status>* ngc_access = NULL;
		DataAccess<GPS_AHRS_status>* gps_ahrs_access = NULL;
		DataAccess<IO_sushidrop_status>* io_access = NULL;

		FILE* f = NULL;

		
	public:
		SushidropLogger(const char* name, DataAccess<IO_sushidrop_status>* IO_access,
											 DataAccess<NGC_status>* NGC_access,
											 DataAccess <GPS_AHRS_status>* Gps_ahrs_access, DataAccess<Time_status>* Time_access);
		~SushidropLogger();

		virtual void execute();
};

void* start_sushidrop_logger(void* arg);

#endif /* SUSHIDROPLOGGER_H_ */