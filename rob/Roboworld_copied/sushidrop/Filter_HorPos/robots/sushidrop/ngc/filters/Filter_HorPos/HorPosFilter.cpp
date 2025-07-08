/*
 * HorPosFilter.cpp
 *
 *  Created on: Apr 21, 2020
 *      Author: mx
 */


#include "HorPosFilter.h"


HorPosFilter::HorPosFilter(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<HorPosFilter_params>* Params_access) :
	Filter(name, HORPOS_FILTER_PARAMS_FILE), P(2, 2), Q(2, 2), R(2, 2), H(2, 2), K(2, 2), OMEGA(2, 2), A(2, 2), B(2, 2), state(2, 1), measure(2, 1), in(2, 1)
{
	ngc_access = NGC_access;
	time_access = Time_access;
	task_access = Task_access;
	params_access = Params_access;

	
	get_params();


	Task_execution ts;
	ts = task_access->get();

	ts.status.value = TASK_INIT;

	task_access->set(ts);
}


HorPosFilter::~HorPosFilter()
{
	
}


void HorPosFilter::parse_config(char *value, double val)
{
	Time_status clock;
	clock = time_access->get();

	if (strcmp(value, "q_xy") == 0)		{ params.actual.q_xy.value = val;		params.actual.q_xy.updated = 1;		params.actual.q_xy.timeStamp = clock.timeStamp.value;	    }
	if (strcmp(value, "r_xy_11") == 0)	{ params.actual.r_xy_11.value = val;	params.actual.r_xy_11.updated = 1;	params.actual.r_xy_11.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "r_xy_12") == 0)	{ params.actual.r_xy_12.value = val;	params.actual.r_xy_12.updated = 1;	params.actual.r_xy_12.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "r_xy_22") == 0)	{ params.actual.r_xy_22.value = val;	params.actual.r_xy_22.updated = 1;	params.actual.r_xy_22.timeStamp = clock.timeStamp.value;	}
}


void HorPosFilter::set_config(FILE* f)
{
	fprintf(f, "q_xy = %lf\n",		params.actual.q_xy.value);
	fprintf(f, "r_xy_11 = %lf\n",	params.actual.r_xy_11.value);
	fprintf(f, "r_xy_12 = %lf\n",	params.actual.r_xy_12.value);
	fprintf(f, "r_xy_22 = %lf\n",	params.actual.r_xy_22.value);
}


void HorPosFilter::get_params()
{
	params = params_access->get();
	read_config();
	params_access->set(params);
}


void HorPosFilter::set_params()
{
	params = params_access->get();

	if (params.reference.q_xy.updated == 1 || params.reference.r_xy_11.updated == 1 || params.reference.r_xy_12.updated == 1 || params.reference.r_xy_22.updated == 1)
	{
		if (params.reference.q_xy.updated == 1)		{ params.actual.q_xy.copyValueFrom(params.reference.q_xy);			params.reference.q_xy.updated = 0; }
		if (params.reference.r_xy_11.updated == 1)	{ params.actual.r_xy_11.copyValueFrom(params.reference.r_xy_11);	params.reference.r_xy_11.updated = 0; }
		if (params.reference.r_xy_12.updated == 1)	{ params.actual.r_xy_12.copyValueFrom(params.reference.r_xy_12);	params.reference.r_xy_12.updated = 0; }
		if (params.reference.r_xy_22.updated == 1)	{ params.actual.r_xy_22.copyValueFrom(params.reference.r_xy_22);	params.reference.r_xy_22.updated = 0; }
		

		write_config();


		params_access->set(params);
	}
}


void HorPosFilter::init()
{
	if (start)
	{
		P.eye();  P = P * 3000.0;
		state.zeros();

		NGC_status ngc_status;
		ngc_status = ngc_access->get();
		state(0, 0) = ngc_status.pose.x.raw.value;
		state(1, 0) = ngc_status.pose.y.raw.value;

		start = false;
	}

	double f = filter();

	if (f < convergenceThr)
	{
		Task_execution ts;
		ts = task_access->get();
		ts.status.value = TASK_RUNNING;
		task_access->set(ts);

		start = true;
	}

}




void HorPosFilter::compute()
{
	double f = filter();

	if (f >= convergenceThr)
	{
		Task_execution ts;
		ts = task_access->get();
		ts.status.value = TASK_INIT;
		task_access->set(ts);

		start = true;
	}
}



double HorPosFilter::filter()
{
	params = params_access->get(); 

	double q_xy = params.actual.q_xy.value;
	double r_xy_11 = params.actual.r_xy_11.value;
	double r_xy_12 = params.actual.r_xy_12.value;
	double r_xy_22 = params.actual.r_xy_22.value;
	

	NGC_status ngc_status;
	ngc_status = ngc_access->get();
	DoubleVariable xRaw = ngc_status.pose.x.raw;
	DoubleVariable yRaw = ngc_status.pose.y.raw;
	DoubleVariable xDot = ngc_status.velocityInertial.xDot.actual;
	DoubleVariable yDot = ngc_status.velocityInertial.yDot.actual;

	Time_status time_status;
	time_status = time_access->get();
	double dt = time_status.dt.value;


	Q.eye();
	Q(0, 0) = Q(1, 1) = q_xy;


	R.eye();
	R(0, 0) = r_xy_11;  R(0, 1) = r_xy_12;
	R(1, 0) = r_xy_12;  R(1, 1) = r_xy_22;


	double h = 0.0;
	if (xRaw.valid==1 && yRaw.valid==1) h = 1.0;

	if (fabs(xRaw.value) > 10.0 * P(0, 0) || fabs(yRaw.value) > 10.0 * P(1, 1)) h = 0.0;

	H.zeros();
	H(0, 0) = h; H(1, 1) = h;


	OMEGA = H * P * H.t() + R;
	K = P * H.t() * OMEGA.inv();


	

	measure.zeros();
	if (h == 1.0)
	{
		measure(0, 0) = xRaw.value;
		measure(1, 0) = yRaw.value;
	}

	state = state + K * (measure - H * state);

	P = P - K * H * P;


	ngc_status.pose.x.filtered.value = state(0, 0);
	ngc_status.pose.x.filtered.std = P(0, 0);
	ngc_status.pose.x.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.pose.x.filtered.valid = 1;
	ngc_status.pose.x.filtered.updated = 1;

	ngc_status.pose.y.filtered.value = state(1, 0);
	ngc_status.pose.y.filtered.std = P(1, 1);
	ngc_status.pose.y.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.pose.y.filtered.valid = 1;
	ngc_status.pose.y.filtered.updated = 1;

	


	A.eye();

	B.zeros();
	B(0, 0) = dt;	  B(1, 1) = dt;


	in(0, 0) = xDot.value;
	in(1, 0) = yDot.value;


	state = A * state + B * in;
	P = A * P * A.t() + Q;

	ngc_access->set(ngc_status);

	return P.trace();
}