/*
 * HorVelControl.h
 *
 *  Created on: May 26, 2020
 *      Author: mx
 */


#include "HorVelControl.h"


HorVelControl::HorVelControl(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<HorVelControl_params>* Params_access) :
	Control(name, HORVEL_CONTROL_PARAMS_FILE)
{
	ngc_access = NGC_access;
	time_access = Time_access;
	task_access = Task_access;
	params_access = Params_access;

	
	get_params();

	

	Task_execution ts;
	ts = task_access->get();

	ts.status.value = TASK_OFF;

	task_access->set(ts);
}


HorVelControl::~HorVelControl()
{
	
}


void HorVelControl::parse_config(char *value, double val)
{
	Time_status clock;
	clock = time_access->get();

	if (strcmp(value, "Kp_u") == 0)	{ params.actual.kp_u.value = val;		params.actual.kp_u.updated = 1;		params.actual.kp_u.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Ki_u") == 0)	{ params.actual.ki_u.value = val;		params.actual.ki_u.updated = 1;		params.actual.ki_u.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Kd_u") == 0)	{ params.actual.kd_u.value = val;		params.actual.kd_u.updated = 1; 	params.actual.kd_u.timeStamp = clock.timeStamp.value; }

	if (strcmp(value, "Kp_v") == 0) { params.actual.kp_v.value = val;		params.actual.kp_v.updated = 1;		params.actual.kp_v.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Ki_v") == 0) { params.actual.ki_v.value = val;		params.actual.ki_v.updated = 1;		params.actual.ki_v.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Kd_v") == 0) { params.actual.kd_v.value = val;		params.actual.kd_v.updated = 1; 	params.actual.kd_v.timeStamp = clock.timeStamp.value; }
	
}


void HorVelControl::set_config(FILE* f)
{
	fprintf(f, "Kp_u = %lf\n", params.actual.kp_u.value);
	fprintf(f, "Ki_u = %lf\n", params.actual.ki_u.value);
	fprintf(f, "Kd_u = %lf\n", params.actual.kd_u.value);

	fprintf(f, "Kp_v = %lf\n", params.actual.kp_v.value);
	fprintf(f, "Ki_v = %lf\n", params.actual.ki_v.value);
	fprintf(f, "Kd_v = %lf\n", params.actual.kd_v.value);
}



void HorVelControl::get_params()
{
	params = params_access->get();
	read_config();
	params_access->set(params);
}


void HorVelControl::set_params()
{
	params = params_access->get();

	if (params.reference.kp_u.updated==1 || params.reference.ki_u.updated == 1 || params.reference.kd_u.updated == 1 || 
		params.reference.kp_v.updated == 1 || params.reference.ki_v.updated == 1 || params.reference.kd_v.updated == 1)
	{
		if (params.reference.kp_u.updated == 1)	{ params.actual.kp_u.copyValueFrom(params.reference.kp_u);	params.reference.kp_u.updated = 0; }
		if (params.reference.ki_u.updated == 1)	{ params.actual.ki_u.copyValueFrom(params.reference.ki_u);	params.reference.ki_u.updated = 0; }
		if (params.reference.kd_u.updated == 1)	{ params.actual.kd_u.copyValueFrom(params.reference.kd_u);	params.reference.kd_u.updated = 0; }
		
		if (params.reference.kp_v.updated == 1) { params.actual.kp_v.copyValueFrom(params.reference.kp_v);	params.reference.kp_v.updated = 0; }
		if (params.reference.ki_v.updated == 1) { params.actual.ki_v.copyValueFrom(params.reference.ki_u);	params.reference.ki_v.updated = 0; }
		if (params.reference.kd_v.updated == 1) { params.actual.kd_v.copyValueFrom(params.reference.kd_v);	params.reference.kd_v.updated = 0; }

		write_config();


		params_access->set(params);
	}
}



void HorVelControl::init()
{	
	Task_execution ts;
	ts = task_access->get();
	ts.status.value = TASK_RUNNING;
	task_access->set(ts);

	ieu = 0.0;
	iev = 0.0;

	NGC_status ngc_status;
	ngc_status = ngc_access->get();
	DoubleVariable u = ngc_status.velocityBody.u.actual;
	DoubleVariable uRef = ngc_status.velocityBody.u.reference;
	DoubleVariable v = ngc_status.velocityBody.v.actual;
	DoubleVariable vRef = ngc_status.velocityBody.v.reference;

	epu = uRef.value - u.value;
	epv = vRef.value - v.value;
}




void HorVelControl::compute()
{
	params = params_access->get();

	double kp_u = params.actual.kp_u.value;
	double ki_u = params.actual.ki_u.value;
	double kd_u = params.actual.kd_u.value;

	double kp_v = params.actual.kp_v.value;
	double ki_v = params.actual.ki_v.value;
	double kd_v = params.actual.kd_v.value;
	
	
	NGC_status ngc_status;
	ngc_status = ngc_access->get();
	DoubleVariable u = ngc_status.velocityBody.u.actual;
	DoubleVariable uRef = ngc_status.velocityBody.u.reference;
	DoubleVariable v = ngc_status.velocityBody.v.actual;
	DoubleVariable vRef = ngc_status.velocityBody.v.reference;
	double fu,fv;

	Time_status time_status;
	time_status = time_access->get();
	double dt = time_status.dt.value;

	double Ku = 0.2737;
	double bu = 0.0075;

	double Kv = 0.2724;
	double bv = 0.0040;


	double eu = uRef.value - u.value;
	ieu += eu * time_status.dt.value;
	double deu = (eu - epu) / time_status.dt.value;
	epu = eu;
	fu = kp_u * eu + ki_u * ieu + kd_u * deu + (Ku / bu) * uRef.value;


	double ev = vRef.value - v.value;
	iev += ev * time_status.dt.value;
	double dev = (ev - epv) / time_status.dt.value;
	epv = ev;
	fv = kp_v * ev + ki_v * iev + kd_v * dev + (Kv / bv) * vRef.value;

	if (labs(time_status.timeStamp.value - uRef.timeStamp) <= 150)
		ieu = 0.0;

	if (labs(time_status.timeStamp.value - vRef.timeStamp) <= 150)
		iev = 0.0;

	ngc_status.force.fu.automatic.value = fu;
	ngc_status.force.fu.automatic.valid = 1;
	ngc_status.force.fu.automatic.updated = 1;
	ngc_status.force.fu.automatic.timeStamp = time_status.timeStamp.value;

	ngc_status.force.fv.automatic.value = fv;
	ngc_status.force.fv.automatic.valid = 1;
	ngc_status.force.fv.automatic.updated = 1;
	ngc_status.force.fv.automatic.timeStamp = time_status.timeStamp.value;
	
	ngc_access->set(ngc_status);
}



