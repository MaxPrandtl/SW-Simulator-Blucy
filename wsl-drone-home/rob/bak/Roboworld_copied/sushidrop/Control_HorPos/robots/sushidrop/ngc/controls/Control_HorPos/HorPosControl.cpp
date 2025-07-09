/*
 * HorPosControl.h
 *
 *  Created on: May 27, 2020
 *      Author: mx
 */


#include "HorPosControl.h"


HorPosControl::HorPosControl(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<HorPosControl_params>* Params_access) :
	Control(name, HORPOS_CONTROL_PARAMS_FILE)
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


HorPosControl::~HorPosControl()
{
	
}


void HorPosControl::parse_config(char *value, double val)
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


void HorPosControl::set_config(FILE* f)
{
	fprintf(f, "Kp_vel = %lf\n", params.actual.kp_vel.value);
	fprintf(f, "Ki_vel = %lf\n", params.actual.ki_vel.value);
	fprintf(f, "Kd_vel = %lf\n", params.actual.kd_vel.value);

	fprintf(f, "Kp_force = %lf\n", params.actual.kp_force.value);
	fprintf(f, "Ki_force = %lf\n", params.actual.ki_force.value);
	fprintf(f, "Kd_force = %lf\n", params.actual.kd_force.value);
}



void HorPosControl::get_params()
{
	params = params_access->get();
	read_config();
	params_access->set(params);
}


void HorPosControl::set_params()
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



void HorPosControl::init()
{	
	Task_execution ts;
	ts = task_access->get();
	ts.status.value = TASK_RUNNING;
	task_access->set(ts);

	ie = 0.0;

	NGC_status ngc_status;
	ngc_status = ngc_access->get();
	
	DoubleVariable x = ngc_status.pose.x.actual;
	DoubleVariable xRef = ngc_status.pose.x.reference;
	DoubleVariable y = ngc_status.pose.y.actual;
	DoubleVariable yRef = ngc_status.pose.y.reference;

	double ex = xRef.value - x.value;
	double ey = yRef.value - y.value;

	double e = sqrt(ex * ex + ey * ey);
}




void HorPosControl::compute()
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

	DoubleVariable psi = ngc_status.pose.psi.actual;
	
	DoubleVariable x = ngc_status.pose.x.actual;
	DoubleVariable xRef = ngc_status.pose.x.reference;
	DoubleVariable y = ngc_status.pose.y.actual;
	DoubleVariable yRef = ngc_status.pose.y.reference;
	
	double fu,fv;
	double uStar,vStar,vectStar;

	Time_status time_status;
	time_status = time_access->get();
	double dt = time_status.dt.value;

	double ex= xRef.value - x.value;
	double ey = yRef.value - y.value;
	
	double e = sqrt(ex*ex + ey*ey);
	double course = atan2(ey, ex);
	ie += e * time_status.dt.value;
	double de = ((e - e_prec)/ time_status.dt.value);
	e_prec = e;

	if (labs(time_status.timeStamp.value - xRef.timeStamp) <= 150  || labs(time_status.timeStamp.value - yRef.timeStamp) <= 150)
		ie = 0.0;

	//printf("e: %lf         course: %lf\n",e,course*180.0/M_PI);

	double ang = modpi(course - psi.value);

	if (params.vel_control.status.value == TASK_RUNNING)
	{
		vectStar = kp_vel * e + ki_vel * ie + kd_vel * de;
		vectStar = sat(vectStar, -0.4, 0.4);
		
		uStar = vectStar * cos(ang) - vectStar * sin(ang);
		vStar = vectStar * sin(ang) + vectStar * cos(ang);

		ngc_status.velocityBody.u.automatic.value = uStar;
		ngc_status.velocityBody.u.automatic.valid = 1;
		ngc_status.velocityBody.u.automatic.updated = 1;
		ngc_status.velocityBody.u.automatic.timeStamp = time_status.timeStamp.value;

		ngc_status.velocityBody.v.automatic.value = vStar;
		ngc_status.velocityBody.v.automatic.valid = 1;
		ngc_status.velocityBody.v.automatic.updated = 1;
		ngc_status.velocityBody.v.automatic.timeStamp = time_status.timeStamp.value;
	}
	else
	{
		vectStar = kp_force * e + ki_force * ie + kd_force * de;
		vectStar = sat(vectStar, -100.0, 100.0);

		fu = vectStar * cos(ang) - vectStar * sin(ang);
		fv = vectStar * sin(ang) + vectStar * cos(ang);

		ngc_status.force.fu.automatic.value = fu;
		ngc_status.force.fu.automatic.valid = 1;
		ngc_status.force.fu.automatic.updated = 1;
		ngc_status.force.fu.automatic.timeStamp = time_status.timeStamp.value;

		ngc_status.force.fv.automatic.value = fv;
		ngc_status.force.fv.automatic.valid = 1;
		ngc_status.force.fv.automatic.updated = 1;
		ngc_status.force.fv.automatic.timeStamp = time_status.timeStamp.value;
	}

	
	
	ngc_access->set(ngc_status);
}



