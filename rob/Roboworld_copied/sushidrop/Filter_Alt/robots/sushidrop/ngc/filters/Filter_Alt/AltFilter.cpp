/*
 * AltFilter.h
 *
 *  Created on: May 12, 2020
 *      Author: mx
 */


#include "AltFilter.h"


AltFilter::AltFilter(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<AltFilter_params>* Params_access) :
	Filter(name, ALT_FILTER_PARAMS_FILE), P(1, 1), Q(1, 1), R(1, 1), H(1, 1), K(1, 1), OMEGA(1, 1), A(1, 1), B(1, 1), state(1, 1), measure(1, 1), in(1, 1)
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


AltFilter::~AltFilter()
{
	
}


void AltFilter::parse_config(char *value, double val)
{
	Time_status clock;
	clock = time_access->get();

	if (strcmp(value, "q_alt") == 0)		{ params.actual.q_a.value = val;	params.actual.q_a.updated = 1;	params.actual.q_a.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "r_alt") == 0)		{ params.actual.r_a.value = val;	params.actual.r_a.updated = 1;	params.actual.r_a.timeStamp = clock.timeStamp.value; }
}


void AltFilter::set_config(FILE* f)
{
	fprintf(f, "q_alt = %lf\n",		params.actual.q_a.value);
	fprintf(f, "r_alt = %lf\n",		params.actual.r_a.value);
}


void AltFilter::get_params()
{
	params = params_access->get();
	read_config();

	params_access->set(params);
}


void AltFilter::set_params()
{
	params = params_access->get();

	if (params.reference.q_a.updated == 1 || params.reference.r_a.updated == 1)
	{
		if (params.reference.q_a.updated == 1) { params.actual.q_a.copyValueFrom(params.reference.q_a);	params.reference.q_a.updated = 0; }
		if (params.reference.r_a.updated == 1) { params.actual.r_a.copyValueFrom(params.reference.r_a);	params.reference.r_a.updated = 0; }

		write_config();


		params_access->set(params);
	}
}



void AltFilter::init()
{
	if (start)
	{
		P.eye();  P = P * 3000.0;
		state.zeros();

		NGC_status ngc_status;
		ngc_status = ngc_access->get();
		state(0, 0) = ngc_status.pose.a.raw.value;
		
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




void AltFilter::compute()
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


double AltFilter::filter()
{
	params = params_access->get();

	double q_z = params.actual.q_a.value;

	NGC_status ngc_status;
	ngc_status = ngc_access->get();

	Time_status time_status;
	time_status = time_access->get();

	state(0, 0) = (1.0 - q_z) * state(0, 0) + q_z * ngc_status.pose.a.raw.value;
	//printf("alt: %lf\n",state(0,0));

	ngc_status.pose.a.filtered.value = state(0, 0);
	ngc_status.pose.a.filtered.std = 0.0;
	ngc_status.pose.a.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.pose.a.filtered.valid = 1;
	ngc_status.pose.a.filtered.updated = 1;
}

/*
double AltFilter::filter()
{
	params = params_access->get(); 

	double q_z = params.actual.q_a.value;
	double r_z = params.actual.r_a.value;
	
	NGC_status ngc_status;
	ngc_status = ngc_access->get();

	DoubleVariable aRaw = ngc_status.pose.a.raw;
	DoubleVariable zDot = ngc_status.velocityInertial.zDot.actual;

	Time_status time_status;
	time_status = time_access->get();
	double dt = time_status.dt.value;


	Q.eye();
	Q(0, 0) = q_z;


	R.eye();
	R(0, 0) = r_z;


	double h = 0.0;
	if (aRaw.valid) h = 1.0;


	if (fabs(aRaw.value) > 5.0 * P(0, 0)) h = 0.0;

	H.zeros();
	H(0, 0) = h;


	OMEGA = H * P * H.t() + R;
	K = P * H.t() * OMEGA.inv();


	measure.zeros();
	if (h == 1.0)
	{
		measure(0, 0) = aRaw.value;
	}

	state = state + K * (measure - H * state);

	P = P - K * H * P;


	ngc_status.pose.a.filtered.value = state(0, 0);
	ngc_status.pose.a.filtered.std = P(0, 0);
	ngc_status.pose.a.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.pose.a.filtered.valid = 1;
	ngc_status.pose.a.filtered.updated = 1;

	

	A.eye();

	B.zeros();
	B(0, 0) = -dt;

	in(0, 0) = zDot.value;

	state = A * state + B * in;
	P = A * P * A.t() + Q;

	ngc_access->set(ngc_status);

	return P.trace();
}
*/