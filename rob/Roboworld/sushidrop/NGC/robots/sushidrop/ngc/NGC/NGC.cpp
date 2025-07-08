/*
 * NGC.h
 *
 *  Created on: Dec 31, 2020
 *      Author: Mx
 */

#include "NGC.h"

NGC::NGC(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Control_angvel_access, DataAccess<Task_execution>* Control_angpos_access,
	DataAccess<Task_execution>* Control_horvel_access, DataAccess<Task_execution>* Control_horpos_access, 
	DataAccess<Task_execution>* Control_vervel_access, DataAccess<Task_execution>* Control_verpos_access, DataAccess<Task_execution>* Control_alt_access,
	DataAccess<Task_execution>* PF_ua_access) : RobotThread(name, SCHED_FIFO, NGC_THREAD_PRIORITY, start_ngc)
{
	time_access = Time_access;
	ngc_access = NGC_access;
	control_angvel_access = Control_angvel_access;
	control_angpos_access = Control_angpos_access;
	control_horvel_access = Control_horvel_access;
	control_horpos_access = Control_horpos_access;
	control_vervel_access = Control_vervel_access;
	control_verpos_access = Control_verpos_access;
	control_alt_access = Control_alt_access;
	pf_ua_access = PF_ua_access;
}


NGC::~NGC()
{

}



void NGC::execute()
{
	timespec tSleep;
	tSleep.tv_sec = NGC_SLEEP_SEC;
	tSleep.tv_nsec = NGC_SLEEP_NSEC;

	Time_status clock;
	NGC_status ngc;
	Task_execution control_angvel;
	Task_execution control_angpos;
	Task_execution control_horvel;
	Task_execution control_horpos;
	Task_execution control_vervel;
	Task_execution control_verpos;
	Task_execution control_alt;
	Task_execution pf_ua;
	
	
	while (running)
	{
		clock = time_access->get();
		ngc = ngc_access->get();
		control_angvel = control_angvel_access->get();
		control_angpos = control_angpos_access->get();
		control_horvel = control_horvel_access->get();
		control_horpos = control_horpos_access->get();
		control_vervel = control_vervel_access->get();
		control_verpos = control_verpos_access->get();
		control_alt = control_alt_access->get();
		pf_ua = pf_ua_access->get();
		
		

		// ANG
		if (control_angpos.status.value == TASK_RUNNING)
		{
			if (pf_ua.status.value == TASK_RUNNING)
			{
				ngc.pose.psi.reference.copyValueFrom(ngc.pose.psi.automatic);
				ngc.pose.psi.automatic.updated = 0;
			}
			else
			{
				ngc.pose.psi.reference.copyValueFrom(ngc.pose.psi.manual);
				ngc.pose.psi.manual.updated = 0;
			}
		}


		if (control_angvel.status.value == TASK_RUNNING)
		{
			if (control_angpos.status.value == TASK_RUNNING || pf_ua.status.value == TASK_RUNNING)
			{
				ngc.velocityBody.r.reference.copyValueFrom(ngc.velocityBody.r.automatic);
				ngc.velocityBody.r.automatic.updated = 0;
			}
			else
			{
				ngc.velocityBody.r.reference.copyValueFrom(ngc.velocityBody.r.manual);
				ngc.velocityBody.r.manual.updated = 0;
			}
		}


		if (control_angpos.status.value == TASK_RUNNING || control_angvel.status.value == TASK_RUNNING)
		{
			ngc.force.tr.reference.copyValueFrom(ngc.force.tr.automatic);
			ngc.force.tr.automatic.updated = 0;
		}
		else
		{
			ngc.force.tr.reference.copyValueFrom(ngc.force.tr.manual);
			ngc.force.tr.manual.updated = 0;
		}


		// HOR
		if (control_horpos.status.value == TASK_RUNNING)
		{
			ngc.pose.x.reference.copyValueFrom(ngc.pose.x.manual);
			ngc.pose.x.manual.updated = 0;

			ngc.pose.y.reference.copyValueFrom(ngc.pose.y.manual);
			ngc.pose.y.manual.updated = 0;
		}


		if (control_horvel.status.value == TASK_RUNNING)
		{

			if (control_horpos.status.value == TASK_RUNNING)
			{
				ngc.velocityBody.u.reference.copyValueFrom(ngc.velocityBody.u.automatic);
				ngc.velocityBody.u.automatic.updated = 0;

				ngc.velocityBody.v.reference.copyValueFrom(ngc.velocityBody.v.automatic);
				ngc.velocityBody.v.automatic.updated = 0;
			}
			else
			{
				ngc.velocityBody.u.reference.copyValueFrom(ngc.velocityBody.u.manual);
				ngc.velocityBody.u.manual.updated = 0;

				ngc.velocityBody.v.reference.copyValueFrom(ngc.velocityBody.v.manual);
				ngc.velocityBody.v.manual.updated = 0;
			}
		}


		if (control_horpos.status.value == TASK_RUNNING || control_horvel.status.value == TASK_RUNNING)
		{
			ngc.force.fu.reference.copyValueFrom(ngc.force.fu.automatic);
			ngc.force.fu.automatic.updated = 0;

			ngc.force.fv.reference.copyValueFrom(ngc.force.fv.automatic);
			ngc.force.fv.automatic.updated = 0;
		}
		else
		{
			ngc.force.fu.reference.copyValueFrom(ngc.force.fu.manual);
			ngc.force.fu.manual.updated = 0;

			ngc.force.fv.reference.copyValueFrom(ngc.force.fv.manual);
			ngc.force.fv.manual.updated = 0;
		}




		// VER
		if (control_verpos.status.value == TASK_RUNNING)
		{
			ngc.pose.z.reference.copyValueFrom(ngc.pose.z.manual);
			ngc.pose.z.manual.updated = 0;
		}


		if (control_vervel.status.value == TASK_RUNNING)
		{

			if (control_verpos.status.value == TASK_RUNNING)
			{
				ngc.velocityBody.w.reference.copyValueFrom(ngc.velocityBody.w.automatic);
				ngc.velocityBody.w.automatic.updated = 0;
			}
			else
			{
				ngc.velocityBody.w.reference.copyValueFrom(ngc.velocityBody.w.manual);
				ngc.velocityBody.w.manual.updated = 0;
			}
		}


		if (control_verpos.status.value == TASK_RUNNING || control_vervel.status.value == TASK_RUNNING)
		{
			ngc.force.fw.reference.copyValueFrom(ngc.force.fw.automatic);
			ngc.force.fw.automatic.updated = 0;
		}
		else
		{
			ngc.force.fw.reference.copyValueFrom(ngc.force.fw.manual);
			ngc.force.fw.manual.updated = 0;
		}
		

		ngc_access->set(ngc);

		nanosleep(&tSleep, NULL);
	}
}



void* start_ngc(void* arg)
{
	NGC* pThread = (NGC*)((ThreadInfo*)arg)->pThread;
	pThread->execute();
	return NULL;
}
