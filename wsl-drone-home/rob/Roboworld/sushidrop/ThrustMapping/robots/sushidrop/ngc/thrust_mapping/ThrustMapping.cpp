/*
 * ThrustMapping.h
 *
 *  Created on: Apr 24, 2020
 *      Author: mx
 */


#include "ThrustMapping.h"


ThrustMapping::ThrustMapping(const char* name, DataAccess<SushidropMotors_status>* Motors_access, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access) : RobotTask(name, NULL,SCHED_FIFO, CTD_THREAD_PRIORITY, start_thrust_mapping_europe),
	versor(3, 8), pos(3, 8), J(3,4), Jpinv(4, 3)
{
	time_access = Time_access;
	task_access = Task_access;
	motors_access = Motors_access;
	ngc_access = NGC_access;

	
	ang_motor = M_PI / 4.0;


	versor(0, 0) = 0.0; 			  	versor(1, 0) = 0.0; 				versor(2, 0) = 1.0;  // VFL
	versor(0, 1) = 0.0; 			  	versor(1, 1) = 0.0; 				versor(2, 1) = 1.0;  // VFR
	versor(0, 2) = 0.0; 			  	versor(1, 2) = 0.0; 				versor(2, 2) = 1.0;  // VRL
	versor(0, 3) = 0.0; 			  	versor(1, 3) = 0.0; 				versor(2, 3) = 1.0;  // VRR

	versor(0, 4) = cos(ang_motor);		versor(1, 4) = sin(ang_motor);		versor(2, 4) = 0.0;  // HFL
	versor(0, 5) = cos(ang_motor); 		versor(1, 5) = -sin(ang_motor); 	versor(2, 5) = 0.0;  // HFR
	versor(0, 6) = -cos(ang_motor); 	versor(1, 6) = sin(ang_motor); 		versor(2, 6) = 0.0;  // HRL
	versor(0, 7) = -cos(ang_motor); 	versor(1, 7) = -sin(ang_motor); 	versor(2, 7) = 0.0;  // HRR


	pos(0, 0) = 0.4; 	pos(1, 0) = -0.2; 	pos(2, 0) = -0.4;  // VFL
	pos(0, 1) = 0.4; 	pos(1, 1) = 0.2; 	pos(2, 1) = -0.4;  // VFR
	pos(0, 2) = -0.4; 	pos(1, 2) = -0.2; 	pos(2, 2) = -0.4;  // VRL
	pos(0, 3) = -0.4; 	pos(1, 3) = 0.2; 	pos(2, 3) = -0.4;  // VRR

	pos(0, 4) = 0.4;	pos(1, 4) = -0.2;	pos(2, 4) = 0.0;  // HFL
	pos(0, 5) = 0.4; 	pos(1, 5) = 0.2; 	pos(2, 5) = 0.0;  // HFR
	pos(0, 6) = -0.4; 	pos(1, 6) = -0.2; 	pos(2, 6) = 0.0;  // HRL
	pos(0, 7) = -0.4; 	pos(1, 7) = 0.2; 	pos(2, 7) = 0.0;  // HRR


	a_pos = 0.0543;
	b_pos = 0.0032;

	a_neg = -a_pos;
	b_neg = -b_pos;


	Matrix vref(8, 1);
	Matrix force(6, 1);

	vref(0, 0) = 0.0; vref(1, 0) = 0.0; vref(2, 0) = 0.0; vref(3, 0) = 0.0;
	vref(4, 0) = 10.0; vref(5, 0) = 10.0; vref(6, 0) = -10.0; vref(7, 0) = -10.0;
	computeForceTorque(vref, versor, pos, force);
	max_fu = force(0, 0);

	vref(0, 0) = 0.0; vref(1, 0) = 0.0; vref(2, 0) = 0.0; vref(3, 0) = 0.0;
	vref(4, 0) = 10.0; vref(5, 0) = -10.0; vref(6, 0) = 10.0; vref(7, 0) = -10.0;
	computeForceTorque(vref, versor, pos, force);
	max_fv = force(1, 0);

	vref(0, 0) = 10.0; vref(1, 0) = 10.0; vref(2, 0) = 10.0; vref(3, 0) = 10.0;
	vref(4, 0) = 0.0; vref(5, 0) = 0.0; vref(6, 0) = 0.0; vref(7, 0) = 0.0;
	computeForceTorque(vref, versor, pos, force);
	max_fw = force(2, 0);

	vref(0, 0) = 0.0; vref(1, 0) = 0.0; vref(2, 0) = 0.0; vref(3, 0) = 0.0;
	vref(4, 0) = 10.0; vref(5, 0) = -10.0; vref(6, 0) = -10.0; vref(7, 0) = 10.0;
	computeForceTorque(vref, versor, pos, force);
	max_tr = force(5, 0);


	double alpha_hfl, alpha_hfr, alpha_hrl, alpha_hrr;
	double beta_hfl, beta_hfr, beta_hrl, beta_hrr;
	double gamma_hfl, gamma_hfr, gamma_hrl, gamma_hrr;

	
	Vector phfl, phfr, phrl, phrr, vhfl, vhfr, vhrl, vhrr;

	phfl(0) = pos(0, 4);		phfl(1) = pos(1, 4);		phfl(2) = pos(2, 4);
	phfr(0) = pos(0, 5);		phfr(1) = pos(1, 5);		phfr(2) = pos(2, 5);
	phrl(0) = pos(0, 6);		phrl(1) = pos(1, 6);		phrl(2) = pos(2, 6);
	phrr(0) = pos(0, 7);		phrr(1) = pos(1, 7);		phrr(2) = pos(2, 7);

	vhfl(0) = versor(0, 4);		vhfl(1) = versor(1, 4);		vhfl(2) = versor(2, 4);
	vhfr(0) = versor(0, 5);		vhfr(1) = versor(1, 5);		vhfr(2) = versor(2, 5);
	vhrl(0) = versor(0, 6);		vhrl(1) = versor(1, 6);		vhrl(2) = versor(2, 6);
	vhrr(0) = versor(0, 7);		vhrr(1) = versor(1, 7);		vhrr(2) = versor(2, 7);

	J(0, 0) = versor(0, 4);			J(0, 1) = versor(0, 5);			J(0, 2) = versor(0, 6);			J(0, 3) = versor(0, 7);
	J(1, 0) = versor(1, 4);			J(1, 1) = versor(1, 5);			J(1, 2) = versor(1, 6);			J(1, 3) = versor(1, 7);
	J(2, 0) = (phfl % vhfl)(2);		J(2, 1) = (phfr % vhfr)(2);		J(2, 2) = (phrl % vhrl)(2);		J(2, 3) = (phrr % vhrr)(2);

	Matrix JJ = J * J.t();
	Jpinv = J.t() * JJ.inv();

}


ThrustMapping::~ThrustMapping()
{
	
}


void ThrustMapping::execute()
{
	timespec tSleep;
	tSleep.tv_sec = NGC_SLEEP_SEC;
	tSleep.tv_nsec = NGC_SLEEP_NSEC;

	Time_status clock;
	NGC_status ngc;
	SushidropMotors_status motors;

	while (running)
	{
		motors = motors_access->get();
		ngc = ngc_access->get();
		clock = time_access->get();

		double fu, fv, fw, tr;
		double vf, vr, hl, hr;
		double bow, stern;

		double tfu, tfv, tfw, ttr;

		fu = sat(ngc.force.fu.reference.value, -100.0, 100.0);
		fv = sat(ngc.force.fv.reference.value, -100.0, 100.0);
		fw = sat(ngc.force.fw.reference.value, -100.0, 100.0);
		tr = sat(ngc.force.tr.reference.value, -100.0, 100.0);

		tfu = fu;
		
		tfw = -fw;
		ttr = -tr;
	

		// heave mapping
		vf = sat(3000*tfw/100.0, -3000.0, 3000.0);
		vr = sat(3000 * tfw / 100.0, -3000.0, 3000.0);

		// surge mapping
		hl = sat(3000 * tfu / 100.0, -3000.0, 3000.0);
		hr = sat(3000 * -tfu / 100.0, -3000.0, 3000.0);

		
		

		// compute max available tr + fv combination (tr always satisfied, remaining effort used for fv)
		double delta = 100.0 - fabs(tr);
		if (fabs(fv) > delta) fv = sign(fv) * delta;

		tfv = -fv;

		bow = sat(3000 * (tfv+ttr) / 100.0, -3000.0, 3000.0);
		stern = sat(3000 * (tfv - ttr) / 100.0, -3000.0, 3000.0);


		
		ngc.force.fu.actual.value = fu;		ngc.force.fu.actual.valid = 1;		ngc.force.fu.actual.timeStamp = clock.timeStamp.value;		ngc.force.fu.actual.updated = 1;
		ngc.force.fv.actual.value = fv;		ngc.force.fv.actual.valid = 1;		ngc.force.fv.actual.timeStamp = clock.timeStamp.value;		ngc.force.fv.actual.updated = 1;
		ngc.force.fw.actual.value = fw;		ngc.force.fw.actual.valid = 1;		ngc.force.fw.actual.timeStamp = clock.timeStamp.value;		ngc.force.fw.actual.updated = 1;
		ngc.force.tr.actual.value = tr;		ngc.force.tr.actual.valid = 1;		ngc.force.tr.actual.timeStamp = clock.timeStamp.value;		ngc.force.tr.actual.updated = 1;

		

		// set voltage to motors
		motors.hl.velocity_reference.value = hl;							motors.hl.velocity_reference.valid = 1;
		motors.hl.velocity_reference.timeStamp = clock.timeStamp.value;		motors.hl.velocity_reference.updated = 1;

		motors.hr.velocity_reference.value = hr;							motors.hr.velocity_reference.valid = 1;
		motors.hr.velocity_reference.timeStamp = clock.timeStamp.value;		motors.hr.velocity_reference.updated = 1;

		motors.vf.velocity_reference.value = vf;							motors.vf.velocity_reference.valid = 1;
		motors.vf.velocity_reference.timeStamp = clock.timeStamp.value;		motors.vf.velocity_reference.updated = 1;

		motors.vr.velocity_reference.value = vr;							motors.vr.velocity_reference.valid = 1;
		motors.vr.velocity_reference.timeStamp = clock.timeStamp.value;		motors.vr.velocity_reference.updated = 1;

		motors.bow.velocity_reference.value = bow;							motors.bow.velocity_reference.valid = 1;
		motors.bow.velocity_reference.timeStamp = clock.timeStamp.value;	motors.bow.velocity_reference.updated = 1;

		motors.stern.velocity_reference.value = stern;						motors.stern.velocity_reference.valid = 1;
		motors.stern.velocity_reference.timeStamp = clock.timeStamp.value;	motors.stern.velocity_reference.updated = 1;
		
		motors_access->set(motors);
		ngc_access->set(ngc);
		 

		nanosleep(&tSleep, NULL);
	}
}


void ThrustMapping::computeForceTorque(Matrix& v, Matrix& versor, Matrix& pos, Matrix& force)
{
	for (int i = 0; i < 6; i++)
		force(i, 0) = 0.0;

	Vector f;
	for (int j = 0; j < 8; j++)
	{
		Vector vrs(versor(0, j), versor(1, j), versor(2, j));
		f += vrs * thrust_curve(v(j, 0));
	}

	Vector t, tt, vv;
	for (int j = 0; j < 8; j++)
	{
		Vector vrs(versor(0, j), versor(1, j), versor(2, j));
		Vector p(pos(0, j), pos(1, j), pos(2, j));
		vv = (vrs * thrust_curve(v(j, 0)));
		tt = p % vv;
		t += tt;

		//printf("%lf  %lf  %lf %lf\n",thrust_curve(v(4,0)),thrust_curve(v(5,0)),thrust_curve(v(6,0)),thrust_curve(v(7,0)));
	}

	//printf("%lf  %lf  %lf %lf\n", thrust_curve(v(4, 0)), thrust_curve(v(5, 0)), thrust_curve(v(6, 0)), thrust_curve(v(7, 0)));

	force(0, 0) = f(0);		force(1, 0) = f(1);		force(2, 0) = f(2);
	force(3, 0) = t(0);		force(4, 0) = t(1);		force(5, 0) = t(2);
}


double ThrustMapping::thrust_curve(double v)
{
	double th = 0.0;

	double vv = fabs(v);

	if (v >= 0.0) th = a_pos * vv * vv + b_pos * vv;
	else th = a_neg * vv * vv + b_neg * vv;

	return th;
}

double ThrustMapping::inv_thrust_curve(double v)
{
	double th = 0.0;

	double vv = fabs(v);

	if (v >= 0.0) th = (-b_pos + sqrt(b_pos * b_pos + 4.0 * a_pos * vv)) / (2.0 * a_pos);
	else th = -((-b_neg - sqrt(b_neg * b_neg - 4.0 * a_neg * vv)) / (2.0 * a_neg));

	return th;
}

	

void* start_thrust_mapping_europe(void* arg)
{
	ThrustMapping* pThread = (ThrustMapping*)((ThreadInfo*)arg)->pThread;
	pThread->execute();
	return NULL;
}
