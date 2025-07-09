/*
 * clock.cpp
 *
 *  Created on: Mar 24, 2020
 *      Author: mx
 */

#include "Clock.h"



Clock::Clock(const char* name, DataAccess<Time_status>* Time_access):RobotThread(name, SCHED_FIFO, CLOCK_THREAD_PRIORITY, start_clock)
{
	time_access = Time_access;
}


Clock::~Clock()
{

}



void Clock::execute()
{
	timespec tSleep;
	tSleep.tv_sec = CORE_SAMPLE_TIME_SEC;
	tSleep.tv_nsec = CORE_SAMPLE_TIME_NSEC;

	Time_status ts;

	clock_gettime(CLOCK_REALTIME, &tStart);

	while (running)
	{
		nanosleep(&tSleep, NULL);

		ts=time_access->get();

		clock_gettime(CLOCK_REALTIME, &tDelay);
		int64 msec = (tDelay.tv_sec - tStart.tv_sec) * 1000 + (tDelay.tv_nsec - tStart.tv_nsec) / 1000000;

		struct tm* timeinfo;
		char buffer_time[80];
		char buf[256];

		timeinfo = gmtime(&(tDelay.tv_sec));

		strftime(buffer_time, 80, "%F %T", timeinfo);
		sprintf(buf, "%s.%d", buffer_time, tDelay.tv_nsec / 1000000);
		//sscanf(buf, "%d-%d-%d %d:%d:%d.%d", &(ts.val.year), &(ts.val.month), &(ts.val.day), &(ts.val.hour), &(ts.val.min), &(ts.val.sec), &(ts.val.msec));

		//printf("%d-%d-%d %d:%d:%d.%d\n", (ts.year), (ts.month), (ts.day), (ts.hour), (ts.min), (ts.sec), (ts.msec));
		//printf("%u Local time: %s\n", tDelay.tv_sec, buf);


		ts.timeStamp.value += msec;					ts.timeStamp.updated = 1;
		ts.dt.value = ((double)msec) / 1000.0;		ts.dt.updated = 1;
		ts.date_time.value=buf;						ts.date_time.updated = 1;
		//strcpy(ts.date_time.value, buf);			ts.date_time.updated = 1;
		//ts.set_string_from_value();



		ts.updated = true;

		time_access->set(ts);

		//printf("ts: %lf\n",ts.dt);


		tStart.tv_sec = tDelay.tv_sec;
		tStart.tv_nsec = tDelay.tv_nsec;
	}
}



void* start_clock(void* arg)
{
	Clock* pThread = (Clock*)((ThreadInfo*)arg)->pThread;
	pThread->execute();
	return NULL;
}



