/*
 * SwampLogger.h
 *
 *  Created on: Feb 25, 2021
 *      Author: Mx
 */

#include "SwampLogger.h"

SwampLogger::SwampLogger(const char* name, DataAccess<Actuator_swamp_status>* FL_access, DataAccess<Actuator_swamp_status>* FR_access,
	                                   DataAccess<Actuator_swamp_status>* RL_access, DataAccess<Actuator_swamp_status>* RR_access,
	                                   DataAccess<NGC_status>* NGC_access, DataAccess<Thrust_mapping_status>* Thrust_mapping_access,
									   DataAccess < GPS_AHRS_status>* Gps_ahrs_access, DataAccess <ECHOLOGGER_status>* Echo_access,
									   DataAccess <UBLOX_status>* Ublox_access, DataAccess<Time_status>* Time_access) : RobotThread(name, SCHED_FIFO, LOGGER_THREAD_PRIORITY, start_swamp_logger)
{
	time_access = Time_access;
	ngc_access = NGC_access;
	fl_access = FL_access;
	fr_access = FR_access;
	rl_access = RL_access;
	rr_access = RR_access;
	gps_ahrs_access = Gps_ahrs_access;
	echo_access = Echo_access;
	ublox_access = Ublox_access;
	thrust_mapping_access = Thrust_mapping_access;
}


SwampLogger::~SwampLogger()
{
	fclose(f);
}



void SwampLogger::execute()
{
	timespec tSleep;
	tSleep.tv_sec = CORE_SAMPLE_TIME_SEC;
	tSleep.tv_nsec = CORE_SAMPLE_TIME_NSEC;

	Time_status clock;
	Actuator_swamp_status fl_status;
	Actuator_swamp_status fr_status;
	Actuator_swamp_status rl_status;
	Actuator_swamp_status rr_status;
	NGC_status ngc;
	GPS_AHRS_status gps_ahrs;
	ECHOLOGGER_status echo;
	UBLOX_status ublox;
	Thrust_mapping_status thrust_mapping;

	char logName[BUF_SIZE];
	time_t localTime;
	struct tm* pTimeStruct;
	time(&localTime);
	pTimeStruct = localtime(&localTime);
	
	sprintf(logName, "SWAMP-LOG-%02d%02d%02d-%02d%02d%02d.txt",
		pTimeStruct->tm_year - 100,
		pTimeStruct->tm_mon + 1,
		pTimeStruct->tm_mday,
		pTimeStruct->tm_hour,
		pTimeStruct->tm_min,
		pTimeStruct->tm_mday);

	f = fopen(logName, "w");
	
	while (running)
	{
		clock = time_access->get();
		ngc = ngc_access->get();
		gps_ahrs = gps_ahrs_access->get();
		echo = echo_access->get();
		ublox = ublox_access->get();
		fl_status = fl_access->get();
		fr_status = fr_access->get();
		rl_status = rl_access->get();
		rr_status = rr_access->get();
		thrust_mapping = thrust_mapping_access->get();
		
		fprintf(f, "%lld %s %lf ", clock.timeStamp.value, clock.date_time.value.c_str(), clock.dt.value);

		fprintf(f, "%lf %lf ", ngc.force.fu.reference.value, ngc.force.fu.actual.value);
		fprintf(f, "%lf %lf ", ngc.force.fv.reference.value, ngc.force.fv.actual.value);
		fprintf(f, "%lf %lf ", ngc.force.tr.reference.value, ngc.force.tr.actual.value);

		fprintf(f, "%lf %lf ", ngc.velocityBody.u.reference.value, ngc.velocityBody.u.actual.value);
		fprintf(f, "%lf %lf ", ngc.velocityBody.v.reference.value, ngc.velocityBody.v.actual.value);
		fprintf(f, "%lf %lf ", ngc.velocityBody.r.reference.value, ngc.velocityBody.r.actual.value);

		fprintf(f, "%lf %lf ", ngc.velocityInertial.xDot.reference.value, ngc.velocityInertial.xDot.actual.value);
		fprintf(f, "%lf %lf ", ngc.velocityInertial.yDot.reference.value, ngc.velocityInertial.yDot.actual.value);
		fprintf(f, "%lf %lf ", ngc.velocityInertial.speed.reference.value, ngc.velocityInertial.speed.actual.value);
		fprintf(f, "%lf %lf ", ngc.velocityInertial.course.reference.value, ngc.velocityInertial.course.actual.value);

		fprintf(f, "%lf %lf ", ngc.pose.x.reference.value, ngc.pose.x.actual.value);
		fprintf(f, "%lf %lf ", ngc.pose.y.reference.value, ngc.pose.y.actual.value);
		fprintf(f, "%lf %lf ", ngc.pose.psi.reference.value, ngc.pose.psi.actual.value);
		fprintf(f, "%lf %lf ", ngc.pose.a.reference.value, ngc.pose.a.actual.value);

		fprintf(f, "%lld %lld %lf %lf ", fl_status.thr_power.value, fl_status.thr_enable.value, fl_status.thr_ref.value, fl_status.thr_act.value);
		fprintf(f, "%lld %lld %lf %lf ", fr_status.thr_power.value, fr_status.thr_enable.value, fr_status.thr_ref.value, fr_status.thr_act.value);
		fprintf(f, "%lld %lld %lf %lf ", rl_status.thr_power.value, rl_status.thr_enable.value, rl_status.thr_ref.value, rl_status.thr_act.value);
		fprintf(f, "%lld %lld %lf %lf ", rr_status.thr_power.value, rr_status.thr_enable.value, rr_status.thr_ref.value, rr_status.thr_act.value);

		fprintf(f, "%lld %lld %lf %lf ", fl_status.azm_power.value, fl_status.azm_enable.value, fl_status.azm_ref.value, fl_status.azm_act.value);
		fprintf(f, "%lld %lld %lf %lf ", fr_status.azm_power.value, fr_status.azm_enable.value, fr_status.azm_ref.value, fr_status.azm_act.value);
		fprintf(f, "%lld %lld %lf %lf ", rl_status.azm_power.value, rl_status.azm_enable.value, rl_status.azm_ref.value, rl_status.azm_act.value);
		fprintf(f, "%lld %lld %lf %lf ", rr_status.azm_power.value, rr_status.azm_enable.value, rr_status.azm_ref.value, rr_status.azm_act.value);

		fprintf(f, "%lld %lld ", thrust_mapping.command.value, thrust_mapping.status.value);

		fprintf(f, "%lf %lf ", gps_ahrs.latitude.value, gps_ahrs.longitude.value);
		fprintf(f, "%lf ", echo.altitude.value);

		fprintf(f, "%lf %lf %lf %lf ", ublox.latitude.value, ublox.longitude.value, ublox.x.value, ublox.y.value);

		fprintf(f, "\n");

		
		/*
		ngc_access->set(ngc);
		fl_access->set(fl_status);
		fr_access->set(fr_status);
		rl_access->set(rl_status);
		rr_access->set(rr_status);
		thrust_mapping_access->set(thrust_mapping);
		gps_ahrs_access->set(gps_ahrs);
		echo_access->set(echo);
		ublox_access->set(ublox);
		*/

		nanosleep(&tSleep, NULL);
	}
}





void* start_swamp_logger(void* arg)
{
	SwampLogger* pThread = (SwampLogger*)((ThreadInfo*)arg)->pThread;
	pThread->execute();
	return NULL;
}
