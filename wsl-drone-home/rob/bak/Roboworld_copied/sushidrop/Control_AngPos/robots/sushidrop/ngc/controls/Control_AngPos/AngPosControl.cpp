/*
 * AngPosControl.h
 *
 *  Created on: May 27, 2020
 *      Author: mx
 */


#include "AngPosControl.h"


AngPosControl::AngPosControl(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<AngPosControl_params>* Params_access) :
	Control(name, ANGPOS_CONTROL_PARAMS_FILE)
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


AngPosControl::~AngPosControl()
{
	
}


void AngPosControl::parse_config(char *value, double val)
{
	Time_status clock;
	clock = time_access->get();

	if (strcmp(value, "Kp_vel") == 0)	{ params.actual.kp_vel.value = val;		params.actual.kp_vel.updated = 1;	params.actual.kp_vel.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Ki_vel") == 0)	{ params.actual.ki_vel.value = val;		params.actual.ki_vel.updated = 1;	params.actual.ki_vel.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Kd_vel") == 0)	{ params.actual.kd_vel.value = val;		params.actual.kd_vel.updated = 1; 	params.actual.kd_vel.timeStamp = clock.timeStamp.value; }

	if (strcmp(value, "Kp_force") == 0) { params.actual.kp_force.value = val;		params.actual.kp_force.updated = 1;	params.actual.kp_force.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Ki_force") == 0) { params.actual.ki_force.value = val;		params.actual.ki_force.updated = 1;	params.actual.ki_force.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Kd_force") == 0) { params.actual.kd_force.value = val;		params.actual.kd_force.updated = 1; 	params.actual.kd_force.timeStamp = clock.timeStamp.value; }
}


void AngPosControl::set_config(FILE* f)
{
	fprintf(f, "Kp_vel = %lf\n", params.actual.kp_vel.value);
	fprintf(f, "Ki_vel = %lf\n", params.actual.ki_vel.value);
	fprintf(f, "Kd_vel = %lf\n", params.actual.kd_vel.value);

	fprintf(f, "Kp_force = %lf\n", params.actual.kp_force.value);
	fprintf(f, "Ki_force = %lf\n", params.actual.ki_force.value);
	fprintf(f, "Kd_force = %lf\n", params.actual.kd_force.value);
}



void AngPosControl::get_params()
{
	params = params_access->get();
	read_config();
	params_access->set(params);
}


void AngPosControl::set_params()
{
	params = params_access->get();

	if (params.reference.kp_vel.updated==1 || params.reference.ki_vel.updated == 1 || params.reference.kd_vel.updated == 1 ||
		params.reference.kp_force.updated == 1 || params.reference.ki_force.updated == 1 || params.reference.kd_force.updated == 1)
	{
		if (params.reference.kp_vel.updated == 1)	{ params.actual.kp_vel.copyValueFrom(params.reference.kp_vel);	params.reference.kp_vel.updated = 0; }
		if (params.reference.ki_vel.updated == 1)	{ params.actual.ki_vel.copyValueFrom(params.reference.ki_vel);	params.reference.ki_vel.updated = 0; }
		if (params.reference.kd_vel.updated == 1)	{ params.actual.kd_vel.copyValueFrom(params.reference.kd_vel);	params.reference.kd_vel.updated = 0; }
		
		if (params.reference.kp_force.updated == 1) { params.actual.kp_force.copyValueFrom(params.reference.kp_force);	params.reference.kp_force.updated = 0; }
		if (params.reference.ki_force.updated == 1) { params.actual.ki_force.copyValueFrom(params.reference.ki_force);	params.reference.ki_force.updated = 0; }
		if (params.reference.kd_force.updated == 1) { params.actual.kd_force.copyValueFrom(params.reference.kd_force);	params.reference.kd_force.updated = 0; }

		write_config();


		params_access->set(params);
	}
}



void AngPosControl::init()
{	
	Task_execution ts;
	ts = task_access->get();
	ts.status.value = TASK_RUNNING;
	task_access->set(ts);

	ie = 0.0;

	NGC_status ngc_status;
	ngc_status = ngc_access->get();
	DoubleVariable psiRef = ngc_status.pose.psi.reference;

	psiRef_prec = psiRef.value;
}




void AngPosControl::compute()
{
	params = params_access->get();

	double kp_vel = params.actual.kp_vel.value;
	double ki_vel = params.actual.ki_vel.value;
	double kd_vel = params.actual.kd_vel.value;

	double kp_force = params.actual.kp_force.value;
	double ki_force = params.actual.ki_force.value;
	double kd_force = params.actual.kd_force.value;
	
	NGC_status ngc_status;
	ngc_status = ngc_access->get();
	DoubleVariable r = ngc_status.velocityBody.r.actual;
	DoubleVariable rRef = ngc_status.velocityBody.r.reference;
	DoubleVariable psi = ngc_status.pose.psi.actual;
	DoubleVariable psiRef = ngc_status.pose.psi.reference;
	
	double tr;
	double rStar;

	Time_status time_status;
	time_status = time_access->get();
	double dt = time_status.dt.value;


	double e = modpi(psiRef.value - psi.value);
	ie += e * time_status.dt.value;
	double de = ((psiRef.value - psiRef_prec)/ time_status.dt.value) - r.value;
	psiRef_prec = psiRef.value;

	if (labs(time_status.timeStamp.value - psiRef.timeStamp) <= 150)
		ie = 0.0;


	if (params.vel_control.status.value == TASK_RUNNING)
	{
		rStar = kp_vel * e + ki_vel * ie + kd_vel * de;
		ngc_status.velocityBody.r.automatic.value = rStar;
		ngc_status.velocityBody.r.automatic.valid = 1;
		ngc_status.velocityBody.r.automatic.updated = 1;
		ngc_status.velocityBody.r.automatic.timeStamp = time_status.timeStamp.value;
	}
	else
	{
		tr = kp_force * e + ki_force * ie + kd_force * de;
		ngc_status.force.tr.automatic.value = tr;
		ngc_status.force.tr.automatic.valid = 1;
		ngc_status.force.tr.automatic.updated = 1;
		ngc_status.force.tr.automatic.timeStamp = time_status.timeStamp.value;
	}

	
	
	ngc_access->set(ngc_status);
}



