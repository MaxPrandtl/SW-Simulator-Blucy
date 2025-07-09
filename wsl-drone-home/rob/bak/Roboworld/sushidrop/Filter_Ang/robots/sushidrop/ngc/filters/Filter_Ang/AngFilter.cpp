/*
 * AngFilter.cpp
 *
 *  Created on: Apr 21, 2020
 *      Author: mx
 */


#include "AngFilter.h"


AngFilter::AngFilter(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<AngFilter_params>* Params_access) :
	Filter(name, ANG_FILTER_PARAMS_FILE), P(2, 2), Q(2, 2), R(2, 2), H(2, 2), K(2, 2), OMEGA(2, 2), A(2, 2), B(2, 1), state(2, 1), measure(2, 1), in(1, 1)
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


AngFilter::~AngFilter()
{
	
}


void AngFilter::parse_config(char *value, double val)
{
	Time_status clock;
	clock = time_access->get();

	if (strcmp(value, "q_psi") == 0)	{ params.actual.q_psi.value = val;	params.actual.q_psi.updated = 1;	params.actual.q_psi.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "q_r") == 0)		{ params.actual.q_r.value = val;	params.actual.q_r.updated = 1;		params.actual.q_r.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "r_psi") == 0)	{ params.actual.r_psi.value = val;	params.actual.r_psi.updated = 1; 	params.actual.r_psi.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "r_r") == 0)		{ params.actual.r_r.value = val;	params.actual.r_r.updated = 1;		params.actual.r_r.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Kr") == 0)		{ params.actual.Kr.value = val;		params.actual.Kr.updated = 1; 		params.actual.Kr.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "br") == 0)		{ params.actual.br.value = val;		params.actual.br.updated = 1;		params.actual.br.timeStamp = clock.timeStamp.value; }
}


void AngFilter::set_config(FILE* f)
{
	fprintf(f, "q_psi = %lf\n", params.actual.q_psi.value);
	fprintf(f, "q_r = %lf\n",	params.actual.q_r.value);
	fprintf(f, "r_psi = %lf\n", params.actual.r_psi.value);
	fprintf(f, "r_r = %lf\n",	params.actual.r_r.value);
	fprintf(f, "Kr = %lf\n",	params.actual.Kr.value);
	fprintf(f, "br = %lf\n",	params.actual.br.value);
}



void AngFilter::get_params()
{
	params = params_access->get();
	read_config();
	params_access->set(params);
}


void AngFilter::set_params()
{
	params = params_access->get();

	if (params.reference.q_psi.updated==1 || params.reference.q_r.updated == 1 || params.reference.r_psi.updated == 1 || params.reference.r_r.updated == 1 ||
		params.reference.Kr.updated == 1 || params.reference.br.updated == 1)
	{
		if (params.reference.q_psi.updated == 1)	{ params.actual.q_psi.copyValueFrom(params.reference.q_psi);	params.reference.q_psi.updated = 0; }
		if (params.reference.q_r.updated == 1)		{ params.actual.q_r.copyValueFrom(params.reference.q_r);		params.reference.q_r.updated = 0; }
		if (params.reference.r_psi.updated == 1)	{ params.actual.r_psi.copyValueFrom(params.reference.r_psi);	params.reference.r_psi.updated = 0; }
		if (params.reference.r_r.updated == 1)		{ params.actual.r_r.copyValueFrom(params.reference.r_r);		params.reference.r_r.updated = 0; }
		if (params.reference.Kr.updated == 1)		{ params.actual.Kr.copyValueFrom(params.reference.Kr);			params.reference.Kr.updated = 0; }
		if (params.reference.br.updated == 1)		{ params.actual.br.copyValueFrom(params.reference.br);			params.reference.br.updated = 0; }


		write_config();


		params_access->set(params);
	}
}



void AngFilter::init()
{
	if (start)
	{
		P.eye();  P = P * 3000.0;
		state.zeros();

		NGC_status ngc_status;
		ngc_status = ngc_access->get();
		state(0, 0) = ngc_status.pose.psi.raw.value;
		state(1, 0) = ngc_status.velocityBody.r.raw.value;

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




void AngFilter::compute()
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



double AngFilter::filter()
{
	params = params_access->get(); 

	double q_psi = params.actual.q_psi.value;
	double q_r = params.actual.q_r.value;
	double r_psi = params.actual.r_psi.value;
	double r_r = params.actual.r_r.value;
	double Kr = params.actual.Kr.value;
	double br = params.actual.br.value;

	NGC_status ngc_status;
	ngc_status = ngc_access->get();
	DoubleVariable heading = ngc_status.pose.psi.raw;
	DoubleVariable yawRate = ngc_status.velocityBody.r.raw;
	double torqueRef = ngc_status.force.tr.actual.value;

	Time_status time_status;
	time_status = time_access->get();
	double dt = time_status.dt.value;


	Q.eye();
	Q(0, 0) = q_psi;  Q(1, 1) = q_r;

	R.eye();
	R(0, 0) = r_psi;  R(1, 1) = r_r;


	H.eye();

	if (!heading.valid) H(0, 0) = 0.0;
	if (!yawRate.valid) H(1, 1) = 0.0;


	OMEGA = H * P * H.t() + R;
	K = P * H.t() * OMEGA.inv();

	measure.zeros();
	measure(0, 0) = heading.value; measure(1, 0) = yawRate.value;

	state = state + K * (measure - H * state);

	P = P - K * H * P;


	ngc_status.pose.psi.filtered.value = state(0, 0);
	ngc_status.pose.psi.filtered.std = P(0, 0);
	ngc_status.pose.psi.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.pose.psi.filtered.valid = 1;
	ngc_status.pose.psi.filtered.updated = 1;

	ngc_status.velocityBody.r.filtered.value = state(1, 0);
	ngc_status.velocityBody.r.filtered.std = P(1, 1);
	ngc_status.velocityBody.r.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.velocityBody.r.filtered.valid = 1;
	ngc_status.velocityBody.r.filtered.updated = 1;

	ngc_status.pose.phi.filtered = ngc_status.pose.phi.raw;					ngc_status.pose.phi.filtered.updated = 1;
	ngc_status.pose.theta.filtered = ngc_status.pose.theta.raw;				ngc_status.pose.theta.filtered.updated = 1;
	ngc_status.velocityBody.p.filtered = ngc_status.velocityBody.p.raw;		ngc_status.velocityBody.p.filtered.updated = 1;
	ngc_status.velocityBody.q.filtered = ngc_status.velocityBody.q.raw;		ngc_status.velocityBody.q.filtered.updated = 1;



	//printf("psi: %lf    kf: %lf\n",heading.value,state(0,0));

	A.eye();
	A(0, 1) = dt;	A(1, 1) = 1.0 - Kr*dt;


	B.zeros();
	B(1, 0) = dt * br;

	//in(0,0)=torqueRef;
	in(0, 0) = inv_thrust_curve(torqueRef, 0.0543, 0.0032);

	state = A * state + B * in;
	state(0, 0) = modpi(state(0, 0));
	P = A * P * A.t() + Q;

	ngc_access->set(ngc_status);

	return P.trace();
}


double AngFilter::inv_thrust_curve(double v, double a, double b)
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