/*
 * HorVelFilter.h
 *
 *  Created on: May 11, 2020
 *      Author: mx
 */


#include "HorVelFilter.h"


HorVelFilter::HorVelFilter(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<HorVelFilter_params>* Params_access) :
	Filter(name, HORVEL_FILTER_PARAMS_FILE), P(4, 4), Q(4, 4), R(2, 2), H(2, 4), K(4, 2), OMEGA(2, 2), A(4, 4), B(4, 2), state(4, 1), measure(2, 1), in(2, 1)
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


HorVelFilter::~HorVelFilter()
{
	
}


void HorVelFilter::parse_config(char *value, double val)
{
	Time_status clock;
	clock = time_access->get();

	if (strcmp(value, "q_uv") == 0)			{ params.actual.q_uv.value = val;		params.actual.q_uv.updated = 1;			params.actual.q_uv.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "q_current ") == 0)	{ params.actual.q_current.value = val;	params.actual.q_current.updated = 1;	params.actual.q_current.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "r_uv_11") == 0)		{ params.actual.r_uv_11.value = val;	params.actual.r_uv_11.updated = 1;		params.actual.r_uv_11.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "r_uv_12") == 0)		{ params.actual.r_uv_12.value = val;	params.actual.r_uv_12.updated = 1;		params.actual.r_uv_12.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "r_uv_22") == 0)		{ params.actual.r_uv_22.value = val;	params.actual.r_uv_22.updated = 1;		params.actual.r_uv_22.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "Ku") == 0)			{ params.actual.Ku.value = val;			params.actual.Ku.updated = 1;			params.actual.Ku.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "bu") == 0)			{ params.actual.bu.value = val;			params.actual.bu.updated = 1;			params.actual.bu.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "Kv") == 0)			{ params.actual.Kv.value = val;			params.actual.Kv.updated = 1;			params.actual.Kv.timeStamp = clock.timeStamp.value;	}
	if (strcmp(value, "bv") == 0)			{ params.actual.bv.value = val;			params.actual.bv.updated = 1;			params.actual.bv.timeStamp = clock.timeStamp.value;	}
}


void HorVelFilter::set_config(FILE* f)
{
	fprintf(f, "q_uv = %lf\n",		params.actual.q_uv.value);
	fprintf(f, "q_current = %lf\n", params.actual.q_current.value);
	fprintf(f, "r_uv_11 = %lf\n",	params.actual.r_uv_11.value);
	fprintf(f, "r_uv_12 = %lf\n",	params.actual.r_uv_12.value);
	fprintf(f, "r_uv_22 = %lf\n",	params.actual.r_uv_22.value);
	fprintf(f, "Ku = %lf\n",		params.actual.Ku.value);
	fprintf(f, "bu = %lf\n",		params.actual.bu.value);
	fprintf(f, "Kv = %lf\n",		params.actual.Kv.value);
	fprintf(f, "bv = %lf\n",		params.actual.bv.value);
}


void HorVelFilter::get_params()
{
	params = params_access->get();
	read_config();
	params_access->set(params);
}


void HorVelFilter::set_params()
{
	params = params_access->get();

	if (params.reference.q_uv.updated == 1 || params.reference.q_current.updated == 1 || 
		params.reference.r_uv_11.updated == 1 || params.reference.r_uv_12.updated == 1 || params.reference.r_uv_22.updated == 1 ||
		params.reference.Ku.updated == 1 || params.reference.bu.updated == 1 || params.reference.Kv.updated == 1 || params.reference.bv.updated == 1)
	{
		if (params.reference.q_uv.updated == 1)			{ params.actual.q_uv.copyValueFrom(params.reference.q_uv);			params.reference.q_uv.updated = 0; }
		if (params.reference.q_current.updated == 1)	{ params.actual.q_current.copyValueFrom(params.reference.q_current);params.reference.q_current.updated = 0; }
		if (params.reference.r_uv_11.updated == 1)		{ params.actual.r_uv_11.copyValueFrom(params.reference.r_uv_11);	params.reference.r_uv_11.updated = 0; }
		if (params.reference.r_uv_12.updated == 1)		{ params.actual.r_uv_12.copyValueFrom(params.reference.r_uv_12);	params.reference.r_uv_12.updated = 0; }
		if (params.reference.r_uv_22.updated == 1)		{ params.actual.r_uv_22.copyValueFrom(params.reference.r_uv_22);	params.reference.r_uv_22.updated = 0; }
		if (params.reference.Ku.updated == 1)			{ params.actual.Ku.copyValueFrom(params.reference.Ku);				params.reference.Ku.updated = 0; }
		if (params.reference.bu.updated == 1)			{ params.actual.bu.copyValueFrom(params.reference.bu);				params.reference.bu.updated = 0; }
		if (params.reference.Kv.updated == 1)			{ params.actual.Kv.copyValueFrom(params.reference.Kv);				params.reference.Kv.updated = 0; }
		if (params.reference.bv.updated == 1)			{ params.actual.bv.copyValueFrom(params.reference.bv);				params.reference.bv.updated = 0; }


		write_config();


		params_access->set(params);
	}
}



void HorVelFilter::init()
{
	if (start)
	{
		P.eye();  P = P * 3000.0;
		state.zeros();

		NGC_status ngc_status;
		ngc_status = ngc_access->get();
		state(0, 0) = ngc_status.velocityBody.u.raw.value;
		state(1, 0) = ngc_status.velocityBody.v.raw.value;

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




void HorVelFilter::compute()
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



double HorVelFilter::filter()
{
	params = params_access->get(); 

	double q_uv = params.actual.q_uv.value;
	double q_current = params.actual.q_current.value;
	double r_uv_11 = params.actual.r_uv_11.value;
	double r_uv_12 = params.actual.r_uv_12.value;
	double r_uv_22 = params.actual.r_uv_22.value;
	double Ku = params.actual.Ku.value;
	double bu = params.actual.bu.value;
	double Kv = params.actual.Kv.value;
	double bv = params.actual.bv.value;

	NGC_status ngc_status;
	ngc_status = ngc_access->get();

	DoubleVariable uRaw = ngc_status.velocityBody.u.raw;
	DoubleVariable vRaw = ngc_status.velocityBody.v.raw;
	DoubleVariable fuRef = ngc_status.force.fu.actual;
	DoubleVariable fvRef = ngc_status.force.fv.actual;

	Time_status time_status;
	time_status = time_access->get();
	double dt = time_status.dt.value;


	Q.eye();
	Q(0, 0) = Q(1, 1) = q_uv;
	Q(2, 2) = Q(3, 3) = q_current;

	R.eye();
	R(0, 0) = r_uv_11;  R(0, 1) = r_uv_12;
	R(1, 0) = r_uv_12;  R(1, 1) = r_uv_22;


	double h = 0.0;
	if (uRaw.valid && vRaw.valid) h = 1.0;



	if (fabs(uRaw.value) > 5.0 * P(0, 0) || fabs(vRaw.value) > 5.0 * P(1, 1)) h = 0.0;

	H.zeros();
	H(0, 0) = h; H(1, 1) = h; H(0, 2) = h; H(1, 3) = h;


	OMEGA = H * P * H.t() + R;
	K = P * H.t() * OMEGA.inv();


	

	measure.zeros();
	if (h == 1.0)
	{
		measure(0, 0) = uRaw.value;
		measure(1, 0) = vRaw.value;
	}

	state = state + K * (measure - H * state);

	P = P - K * H * P;

	//printf("uRaw: %lf    u: %lf\n",uRaw.value,state(0,0));
	//printf("psi: %lf\n", ngc_status.pose.psi.actual.value);

	ngc_status.velocityBody.u.filtered.value = state(0, 0);
	ngc_status.velocityBody.u.filtered.std = P(0, 0);
	ngc_status.velocityBody.u.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.velocityBody.u.filtered.valid = 1;
	ngc_status.velocityBody.u.filtered.updated = 1;

	ngc_status.velocityBody.v.filtered.value = state(1, 0);
	ngc_status.velocityBody.v.filtered.std = P(1, 1);
	ngc_status.velocityBody.v.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.velocityBody.v.filtered.valid = 1;
	ngc_status.velocityBody.v.filtered.updated = 1;

	ngc_status.velocityBody.uSeaCurrent.filtered.value = state(2, 0);
	ngc_status.velocityBody.uSeaCurrent.filtered.std = P(2, 2);
	ngc_status.velocityBody.uSeaCurrent.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.velocityBody.uSeaCurrent.filtered.valid = 1;
	ngc_status.velocityBody.uSeaCurrent.filtered.updated = 1;

	ngc_status.velocityBody.vSeaCurrent.filtered.value = state(3, 0);
	ngc_status.velocityBody.vSeaCurrent.filtered.std = P(3, 3);
	ngc_status.velocityBody.vSeaCurrent.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.velocityBody.vSeaCurrent.filtered.valid = 1;
	ngc_status.velocityBody.vSeaCurrent.filtered.updated = 1;

	double psi = ngc_status.pose.psi.actual.value;
	double u = state(0, 0);
	double v = state(1, 0);
	double u_sea = state(2, 0);
	double v_sea = state(3, 0);
	double xDot = cos(psi) * (u + u_sea) - sin(psi) * (v + v_sea);
	double yDot = sin(psi) * (u + u_sea) + cos(psi) * (v + v_sea);
	double xDot_sea = cos(psi) * (u_sea)-sin(psi) * (v_sea);
	double yDot_sea = sin(psi) * (u_sea)+cos(psi) * (v_sea);
	double speed = sqrt(xDot * xDot + yDot * yDot);
	double course = atan2(yDot, xDot);


	ngc_status.velocityInertial.xDot.filtered.value = xDot;
	ngc_status.velocityInertial.yDot.filtered.value = yDot;
	ngc_status.velocityInertial.xDotSeaCurrent.filtered.value = xDot_sea;
	ngc_status.velocityInertial.yDotSeaCurrent.filtered.value = yDot_sea;
	ngc_status.velocityInertial.speed.filtered.value = speed;
	ngc_status.velocityInertial.course.filtered.value = course;

	ngc_status.velocityInertial.xDot.filtered.std = P(0, 0);
	ngc_status.velocityInertial.yDot.filtered.std = P(1, 1);
	ngc_status.velocityInertial.xDotSeaCurrent.filtered.std = P(2, 2);
	ngc_status.velocityInertial.yDotSeaCurrent.filtered.std = P(3, 3);
	ngc_status.velocityInertial.speed.filtered.std = P(0, 0);
	ngc_status.velocityInertial.course.filtered.std = P(1, 1);

	ngc_status.velocityInertial.xDot.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.velocityInertial.yDot.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.velocityInertial.xDotSeaCurrent.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.velocityInertial.yDotSeaCurrent.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.velocityInertial.speed.filtered.timeStamp = time_status.timeStamp.value;
	ngc_status.velocityInertial.course.filtered.timeStamp = time_status.timeStamp.value;

	ngc_status.velocityInertial.xDot.filtered.valid = 1;
	ngc_status.velocityInertial.yDot.filtered.valid = 1;
	ngc_status.velocityInertial.xDotSeaCurrent.filtered.valid = 1;
	ngc_status.velocityInertial.yDotSeaCurrent.filtered.valid = 1;
	ngc_status.velocityInertial.speed.filtered.valid = 1;
	ngc_status.velocityInertial.course.filtered.valid = 1;

	ngc_status.velocityInertial.xDot.filtered.updated = 1;
	ngc_status.velocityInertial.yDot.filtered.updated = 1;
	ngc_status.velocityInertial.xDotSeaCurrent.filtered.updated = 1;
	ngc_status.velocityInertial.yDotSeaCurrent.filtered.updated = 1;
	ngc_status.velocityInertial.speed.filtered.updated = 1;
	ngc_status.velocityInertial.course.filtered.updated = 1;


	A.zeros();
	A(0, 0) = 1.0 - Ku * dt;   A(1, 1) = 1.0 - Kv * dt;
	A(2, 2) = 1.0; A(3, 3) = 1.0;


	B.zeros();
	B(0, 0) = bu * dt;	  B(1, 1) = bv * dt;


	in(0, 0) = inv_thrust_curve(fuRef.value, 0.0543, 0.0032);
	in(1, 0) = inv_thrust_curve(fvRef.value, 0.0543, 0.0032);


	state = A * state + B * in;
	P = A * P * A.t() + Q;

	ngc_access->set(ngc_status);

	return P.trace();
}


double HorVelFilter::inv_thrust_curve(double v, double a, double b)
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