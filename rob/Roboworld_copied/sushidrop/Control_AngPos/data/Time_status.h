/*
 * Time_status.h
 *
 *  Created on: Mar 24, 2020
 *      Author: mx
 */

#ifndef TIME_STATUS_H_
#define TIME_STATUS_H_

#include "../generic/custom_types.h"
#include "../generic/Variable.h"

/*
struct Time_value
{
	uint64 timeStamp;
	double dt;
	int32 year, month, day, hour, min, sec, msec;

	void zero()
	{
		timeStamp = 0;
		dt = 0.0;
		year = 0; month = 0; day = 0;
		hour = 0; min = 0; sec = 0; msec = 0;
	}

	void copy(Time_value d)
	{
		timeStamp = d.timeStamp;
		dt = d.dt;
		year = d.year; month = d.month; day = d.day;
		hour = d.hour; min = d.min; sec = d.sec; msec = d.msec;
	}
};


struct Time_string
{
	char timeStamp[BUF_SIZE];
	char dt[BUF_SIZE];
	char date_time[BUF_SIZE];

	void zero()
	{
		strcpy(timeStamp, "");  strcpy(dt, "");  strcpy(date_time, "");
	}

	void copy(Time_string d)
	{
		strcpy(timeStamp, d.timeStamp);
		strcpy(dt, d.dt);
		strcpy(date_time, d.date_time);
	}
};


class Time_status
{
	private:

		void copy_data(const Time_status &d)
		{
			val.copy(d.val);
			str.copy(d.str);
			updated = d.updated;
		}


	public:

		Time_value val;
		Time_string str;

		bool updated;

		Time_status()
		{
			val.zero();
			str.zero();
			updated = false;
		}


		Time_status(Time_status &d)
		{
			copy_data(d);
		}


		~Time_status(){}


		Time_status& operator=(const Time_status &d)
		{
			copy_data(d);
			return *this;
		}


		void set_string_from_value()
		{
			sprintf(str.timeStamp, "%lli", val.timeStamp);
			sprintf(str.dt, "%lf", val.dt);
			sprintf(str.date_time, "%d-%02d-%02d %02d:%02d:%02d.%d", val.year, val.month, val.day, val.hour, val.min, val.sec, val.msec);
		}

		void set_value_from_string()
		{
			sscanf(str.timeStamp, "%lli", &(val.timeStamp));
			sscanf(str.dt, "%lf", &(val.dt));
			sscanf(str.date_time, "%d-%02d-%02d %02d:%02d:%02d.%d", &(val.year), &(val.month), &(val.day), &(val.hour), &(val.min), &(val.sec), &(val.msec));
		}

};
*/


class Time_status
{
private:

	void copy_data(const Time_status& d)
	{
		timeStamp = d.timeStamp;
		dt = d.dt;
		date_time = d.date_time;
		updated = d.updated;
	}


public:

	IntVariable timeStamp;
	DoubleVariable dt;
	StringVariable date_time;

	bool updated;

	Time_status()
	{
		timeStamp.zero();
		dt.zero();
		date_time.zero();
		updated = false;
	}


	Time_status(Time_status& d)
	{
		copy_data(d);
	}


	~Time_status() {}


	Time_status& operator=(const Time_status& d)
	{
		copy_data(d);
		return *this;
	}

};

#endif /* TIME_STATUS_H_ */
