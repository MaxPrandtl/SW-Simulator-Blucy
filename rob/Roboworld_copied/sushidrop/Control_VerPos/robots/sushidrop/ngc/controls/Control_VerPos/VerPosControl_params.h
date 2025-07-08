/*
 * VerPosControl_params.h
 *
 *  Created on: Jun 10, 2020
 *      Author: mx
 */

#ifndef VER_POS_CONTROL_PARAMS_H_
#define VER_POS_CONTROL_PARAMS_H_


#include "../../../../../generic/Variable.h"
#include "../../../../../data/Task_execution.h"

 
class VerPosControl_param_set
{
	private:
		void copy_data(const VerPosControl_param_set& d)
		{
			kp_vel = d.kp_vel;
			ki_vel = d.ki_vel;
			kd_vel = d.kd_vel;

			kp_force = d.kp_force;
			ki_force = d.ki_force;
			kd_force = d.kd_force;
		}


	public:
		DoubleVariable kp_vel, ki_vel, kd_vel;
		DoubleVariable kp_force, ki_force, kd_force;

		VerPosControl_param_set()
		{
			zero();
		}


		VerPosControl_param_set(VerPosControl_param_set& d)
		{
			copy_data(d);
		}


		~VerPosControl_param_set() {}


		void zero()
		{
			kp_vel.zero();	ki_vel.zero();
			kd_vel.zero();

			kp_force.zero();	ki_force.zero();
			kd_force.zero();
		}


		VerPosControl_param_set& operator=(const VerPosControl_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class VerPosControl_params
{
	private:
		void copy_data(const VerPosControl_params& d)
		{
			reference = d.reference;
			actual = d.actual;
			vel_control = d.vel_control;
		}


	public:
		VerPosControl_param_set reference, actual;
		Task_execution vel_control;

		VerPosControl_params()
		{
			reference.zero();
			actual.zero();
			vel_control.status.zero();
		}


		VerPosControl_params(VerPosControl_params& d)
		{
			copy_data(d);
		}


		~VerPosControl_params() {}


		VerPosControl_params& operator=(const VerPosControl_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* VER_POS_CONTROL_PARAMS_H_ */
