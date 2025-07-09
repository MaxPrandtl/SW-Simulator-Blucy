/*
 * VerVelControl.h
 *
 *  Created on: May 26, 2020
 *      Author: mx
 */


#include "VerVelControl.h"


VerVelControl::VerVelControl(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<VerVelControl_params>* Params_access) :
	Control(name, VERVEL_CONTROL_PARAMS_FILE)
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


VerVelControl::~VerVelControl()
{
	
}


void VerVelControl::parse_config(char *value, double val)
{
	Time_status clock;
	clock = time_access->get();

	if (strcmp(value, "Kp") == 0)	{ params.actual.kp.value = val;		params.actual.kp.updated = 1;	params.actual.kp.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Ki") == 0)	{ params.actual.ki.value = val;		params.actual.ki.updated = 1;	params.actual.ki.timeStamp = clock.timeStamp.value; }
	if (strcmp(value, "Kd") == 0)	{ params.actual.kd.value = val;		params.actual.kd.updated = 1; 	params.actual.kd.timeStamp = clock.timeStamp.value; }
}


void VerVelControl::set_config(FILE* f)
{
	fprintf(f, "Kp = %lf\n", params.actual.kp.value);
	fprintf(f, "Ki = %lf\n", params.actual.ki.value);
	fprintf(f, "Kd = %lf\n", params.actual.kd.value);
}



void VerVelControl::get_params()
{
	params = params_access->get();
	read_config();
	params_access->set(params);
}


void VerVelControl::set_params()
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



void VerVelControl::init()
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




void VerVelControl::compute()
{
	params = params_access->get();

	double kp = params.actual.kp.value;
	double ki = params.actual.ki.value;
	double kd = params.actual.kd.value;
	
	NGC_status ngc_status;
	ngc_status = ngc_access->get();
	DoubleVariable w = ngc_status.velocityBody.w.actual;
	DoubleVariable wRef = ngc_status.velocityBody.w.reference;
	double fw;

	Time_status time_status;
	time_status = time_access->get();
	double dt = time_status.dt.value;
	
	double Kww = 0.7332;
	double bw = 0.0054;


	double e = wRef.value - w.value;


	ie += e * time_status.dt.value;

	if (labs(time_status.timeStamp.value - wRef.timeStamp) <= 150)
		ie = 0.0;

	double de = (e - ep) / time_status.dt.value;
	ep = e;

	fw = kp * e + ki * ie + kd * de + (Kww/bw)*wRef.value*fabs(wRef.value);



	ngc_status.force.fw.automatic.value = fw;
	ngc_status.force.fw.automatic.valid = 1;
	ngc_status.force.fw.automatic.updated = 1;
	ngc_status.force.fw.automatic.timeStamp = time_status.timeStamp.value;
	
	ngc_access->set(ngc_status);
}



