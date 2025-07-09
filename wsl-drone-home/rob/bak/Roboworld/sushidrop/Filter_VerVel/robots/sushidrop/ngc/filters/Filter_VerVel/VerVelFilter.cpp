/*
 * VerVelFilter.h
 *
 *  Created on: May 12, 2020
 *      Author: mx
 */


#include "VerVelFilter.h"


VerVelFilter::VerVelFilter(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<VerVelFilter_params>* Params_access) :
	Filter(name, VERVEL_FILTER_PARAMS_FILE), P(1, 1), Q(1, 1), R(1, 1), H(1, 1), K(1, 1), OMEGA(1, 1), A(1, 1), B(1, 1), state(1, 1), measure(1, 1), in(1, 1)
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


VerVelFilter::~VerVelFilter()
{
	
}


void VerVelFilter::parse_config(char *value, double val)
{
	Time_status clock;
	clock = time_access->get();

	if (strcmp(value, "q_w") == 0)			{ params.actual.q_w.value = val;		params.actual.q_w.updated = 1;		params.actual.q_w.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "r_w") == 0)			{ params.actual.r_w.value = val;		params.actual.r_w.updated = 1;		params.actual.r_w.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "Kw") == 0)			{ params.actual.Kw.value = val;			params.actual.Kw.updated = 1;		params.actual.Kw.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "bw") == 0)			{ params.actual.bw.value = val;			params.actual.bw.updated = 1;		params.actual.bw.timeStamp = clock.timeStamp.value;	}
}


void VerVelFilter::set_config(FILE* f)
{
	fprintf(f, "q_w = %lf\n",		params.actual.q_w.value);
	fprintf(f, "r_w = %lf\n",		params.actual.r_w.value);
	fprintf(f, "Kw = %lf\n",		params.actual.Kw.value);
	fprintf(f, "bw = %lf\n",		params.actual.bw.value);
}


void VerVelFilter::get_params()
{
	params = params_access->get();
	read_config();
	params_access->set(params);
}


void VerVelFilter::set_params()
{
	params = params_access->get();

	if (params.reference.q_w.updated == 1 || params.reference.r_w.updated == 1 || params.reference.Kw.updated == 1 || params.reference.bw.updated == 1)
	{
		if (params.reference.q_w.updated == 1) { params.actual.q_w.copyValueFrom(params.reference.q_w);	params.reference.q_w.updated = 0; }
		if (params.reference.r_w.updated == 1) { params.actual.r_w.copyValueFrom(params.reference.r_w);	params.reference.r_w.updated = 0; }
		if (params.reference.Kw.updated == 1) { params.actual.Kw.copyValueFrom(params.reference.Kw);	params.reference.Kw.updated = 0; }
		if (params.reference.bw.updated == 1) { params.actual.bw.copyValueFrom(params.reference.bw);	params.reference.bw.updated = 0; }


		write_config();


		params_access->set(params);
	}
}



void VerVelFilter::init()
{
	if (start)
	{
		P.eye();  P = P * 3000.0;
		state.zeros();

		NGC_status ngc_status;
		ngc_status = ngc_access->get();
		state(0, 0) = ngc_status.velocityBody.w.raw.value;
		
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




void VerVelFilter::compute()
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



double VerVelFilter::filter()
{
	params = params_access->get(); 

	double q_w = params.actual.q_w.value;
	double r_w = params.actual.r_w.value;
	double Kw = params.actual.Kw.value;
	double bw = params.actual.bw.value;

	NGC_status ngc_status;
	ngc_status = ngc_access->get();

	DoubleVariable wRaw = ngc_status.velocityBody.w.raw;
	DoubleVariable fwRef = ngc_status.force.fw.actual;

	Time_status time_status;
	time_status = time_access->get();
	double dt = time_status.dt.value;


	Q.eye();
	Q(0, 0) = q_w;


	R.eye();
	R(0, 0) = r_w;


	double h = 0.0;
	if (wRaw.valid) h = 1.0;


	if (fabs(wRaw.value) > 5.0 * P(0, 0)) h = 0.0;

	H.zeros();
	H(0, 0) = h;


	OMEGA = H * P * H.t() + R;
	K = P * H.t() * OMEGA.inv();


	measure.zeros();
	if (h == 1.0)
	{
		measure(0, 0) = wRaw.value;
	}

	state = state + K * (measure - H * state);

	P = P - K * H * P;


	ngc_status.velocityBody.w.filtered.value = state(0, 0);
	ngc_status.velocityBody.w.filtered.std = P(0, 0);
	ngc_status.velocityBody.w.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.velocityBody.w.filtered.valid = 1;
	ngc_status.velocityBody.w.filtered.updated = 1;


	double phi = ngc_status.pose.phi.actual.value;
	double theta = ngc_status.pose.theta.actual.value;
	double w = state(0, 0);
	double zDot = w * cos(phi) * cos(theta);


	ngc_status.velocityInertial.zDot.filtered.value = zDot;
	ngc_status.velocityInertial.zDot.filtered.std = P(0, 0);
	ngc_status.velocityInertial.zDot.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.velocityInertial.zDot.filtered.valid = 1;
	ngc_status.velocityInertial.zDot.filtered.updated = 1;
	

	A.zeros();
	A(0, 0) = 1.0 - 2.0*Kw*fabs(state(0,0)) * dt;


	B.zeros();
	B(0, 0) = bw * dt;


	in(0, 0) = inv_thrust_curve(fwRef.value, 0.0543, 0.0032);


	state = A * state + B * in;
	P = A * P * A.t() + Q;

	ngc_access->set(ngc_status);

	return P.trace();
}


double VerVelFilter::inv_thrust_curve(double v, double a, double b)
{
	double a_pos = a;
	double b_pos = b;

	double a_neg = -a;
	double b_neg = -b;

	double th = 0.0;

	double vv = fabs(v);

	if (v >= 0.0) th = (-b_pos + sqrt(b_pos * b_pos + 4.0 * a_pos * vv)) / (2.0 * a_pos);
	else th = -((-b_neg - sqrt(b_neg * b_neg - 4.0 * a_neg * vv)) / (2.0 * a_neg));

	return th;
}