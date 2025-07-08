/*
 * AngVelControl.h
 *
 *  Created on: May 26, 2020
 *      Author: mx
 */


#include "AngVelControl.h"


AngVelControl::AngVelControl(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<AngVelControl_params>* Params_access) :
	Control(name, ANGVEL_CONTROL_PARAMS_FILE)
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


AngVelControl::~AngVelControl()
{
	
}


void AngVelControl::parse_config(char *value, double val)
{
	Time_status clock;
	clock = time_access->get();

	if (strcmp(value, "Kp") == 0)	{ params.actual.kp.value = val;		params.actual.kp.updated = 1;	params.actual.kp.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Ki") == 0)	{ params.actual.ki.value = val;		params.actual.ki.updated = 1;	params.actual.ki.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Kd") == 0)	{ params.actual.kd.value = val;		params.actual.kd.updated = 1; 	params.actual.kd.timeStamp = clock.timeStamp.value; }
}


void AngVelControl::set_config(FILE* f)
{
	fprintf(f, "Kp = %lf\n", params.actual.kp.value);
	fprintf(f, "Ki = %lf\n", params.actual.ki.value);
	fprintf(f, "Kd = %lf\n", params.actual.kd.value);
}



void AngVelControl::get_params()
{
	params = params_access->get();
	read_config();
	params_access->set(params);
}


void AngVelControl::set_params()
{
	params = params_access->get();

	if (params.reference.kp.updated==1 || params.reference.ki.updated == 1 || params.reference.kd.updated == 1 )
	{
		if (params.reference.kp.updated == 1)	{ params.actual.kp.copyValueFrom(params.reference.kp);	params.reference.kp.updated = 0; }
		if (params.reference.ki.updated == 1)	{ params.actual.ki.copyValueFrom(params.reference.ki);	params.reference.ki.updated = 0; }
		if (params.reference.kd.updated == 1)	{ params.actual.kd.copyValueFrom(params.reference.kd);	params.reference.kd.updated = 0; }
		

		write_config();


		params_access->set(params);
	}
}



void AngVelControl::init()
{	
	Task_execution ts;
	ts = task_access->get();
	ts.status.value = TASK_RUNNING;
	task_access->set(ts);

	ie = 0.0;

	NGC_status ngc_status;
	ngc_status = ngc_access->get();
	DoubleVariable r = ngc_status.velocityBody.r.actual;
	DoubleVariable rRef = ngc_status.velocityBody.r.reference;

	ep = rRef.value - r.value;
}




void AngVelControl::compute()
{
	params = params_access->get();

	double kp = params.actual.kp.value;
	double ki = params.actual.ki.value;
	double kd = params.actual.kd.value;
	
	NGC_status ngc_status;
	ngc_status = ngc_access->get();
	DoubleVariable r = ngc_status.velocityBody.r.actual;
	DoubleVariable rRef = ngc_status.velocityBody.r.reference;
	double tr;

	Time_status time_status;
	time_status = time_access->get();
	double dt = time_status.dt.value;

	double k1 = 1204.0;
	double k2 = 98936.0;
	double a = 0.86;


	double e = rRef.value - r.value;


	ie += e * time_status.dt.value;

	if (labs(time_status.timeStamp.value - rRef.timeStamp) <= 150)
		ie = 0.0;

	double de = (e - ep) / time_status.dt.value;
	ep = e;

	double ff = kp * e + ki * ie + kd * de + k1*rRef.value + k2* rRef.value*fabs(rRef.value);
	if (ff >= 0.0) tr = sqrt(ff);
	else tr = -sqrt(fabs(ff / a));


	ngc_status.force.tr.automatic.value = tr;
	ngc_status.force.tr.automatic.valid = 1;
	ngc_status.force.tr.automatic.updated = 1;
	ngc_status.force.tr.automatic.timeStamp = time_status.timeStamp.value;
	
	ngc_access->set(ngc_status);
}



