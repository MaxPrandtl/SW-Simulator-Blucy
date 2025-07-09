/*
 * SwampLogger.h
 *
 *  Created on: Feb 25, 2021
 *      Author: Mx
 */

#include "SushidropLogger.h"

SushidropLogger::SushidropLogger(const char* name, DataAccess<IO_sushidrop_status>* IO_access,
	DataAccess<NGC_status>* NGC_access,
	DataAccess <GPS_AHRS_status>* Gps_ahrs_access, DataAccess<Time_status>* Time_access) : RobotThread(name, SCHED_FIFO, LOGGER_THREAD_PRIORITY, start_sushidrop_logger)
{
	time_access = Time_access;
	ngc_access = NGC_access;
	io_access = IO_access;
	gps_ahrs_access = Gps_ahrs_access;
}


SushidropLogger::~SushidropLogger()
{
	fclose(f);
}



void SushidropLogger::execute()
{
	timespec tSleep;
	tSleep.tv_sec = CORE_SAMPLE_TIME_SEC;
	tSleep.tv_nsec = CORE_SAMPLE_TIME_NSEC;

	Time_status clock;
	IO_sushidrop_status io_status;
	NGC_status ngc;
	GPS_AHRS_status gps_ahrs;
	

	char logName[BUF_SIZE];
	time_t localTime;
	struct tm* pTimeStruct;
	time(&localTime);
	pTimeStruct = localtime(&localTime);
	
	sprintf(logName, "SUSHIDROP-LOG-%02d%02d%02d-%02d%02d%02d.txt",
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
		io_status = io_access->get();
		
		
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

		
		fprintf(f, "%lf %lf ", gps_ahrs.latitude.value, gps_ahrs.longitude.value);
		
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





void* start_sushidrop_logger(void* arg)
{
	SushidropLogger* pThread = (SushidropLogger*)((ThreadInfo*)arg)->pThread;
	pThread->execute();
	return NULL;
}
