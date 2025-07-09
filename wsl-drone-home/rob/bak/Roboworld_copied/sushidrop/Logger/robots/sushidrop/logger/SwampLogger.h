/*
 * SwampLogger.h
 *
 *  Created on: Feb 25, 2021
 *      Author: Mx
 */

#ifndef SWAMPLOGGER_H_
#define SWAMPLOGGER_H_

#include "../../../generic/RobotThread.h"
#include "../../../data/DataAccess.h"
#include "../../../data/Time_status.h"
#include "../../../data/NGC_status.h"
#include "../../../data/GPS_AHRS_status.h"
#include "../../../data/ECHOLOGGER_status.h"
#include "../../../data/UBLOX_status.h"
#include "../../../data/Actuator_swamp_status.h"
#include "../../../data/Thrust_mapping_status.h"


class SwampLogger :public RobotThread
{
	private:
		DataAccess<Time_status>* time_access = NULL;
		DataAccess<NGC_status>* ngc_access = NULL;
		DataAccess<GPS_AHRS_status>* gps_ahrs_access = NULL;
		DataAccess<ECHOLOGGER_status>* echo_access = NULL;
		DataAccess<UBLOX_status>* ublox_access = NULL;
		DataAccess<Actuator_swamp_status>* fl_access = NULL;
		DataAccess<Actuator_swamp_status>* fr_access = NULL;
		DataAccess<Actuator_swamp_status>* rl_access = NULL;
		DataAccess<Actuator_swamp_status>* rr_access = NULL;
		DataAccess<Thrust_mapping_status>* thrust_mapping_access = NULL;

		FILE* f = NULL;

		
	public:
		SwampLogger(const char* name, DataAccess<Actuator_swamp_status>* FL_access, DataAccess<Actuator_swamp_status>* FR_access,
			                                 DataAccess<Actuator_swamp_status>* RL_access, DataAccess<Actuator_swamp_status>* RR_access,
											 DataAccess<NGC_status>* NGC_access, DataAccess<Thrust_mapping_status>* Thrust_mapping_access,
											 DataAccess <GPS_AHRS_status>* Gps_ahrs_access, DataAccess <ECHOLOGGER_status>* Echo_access,
											 DataAccess <UBLOX_status>* Ublox_access, DataAccess<Time_status>* Time_access);
		~SwampLogger();

		virtual void execute();
};

void* start_swamp_logger(void* arg);

#endif /* SWAMPLOGGER_H_ */