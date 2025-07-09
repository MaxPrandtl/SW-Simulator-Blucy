/*
 * AngPosControl_params.h
 *
 *  Created on: May 27, 2020
 *      Author: mx
 */

#ifndef ANG_POS_CONTROL_PARAMS_H_
#define ANG_POS_CONTROL_PARAMS_H_


#include "../../../../../generic/Variable.h"

 
class AngPosControl_param_set
{
	private:
		void copy_data(const AngPosControl_param_set& d)
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

		AngPosControl_param_set()
		{
			zero();
		}


		AngPosControl_param_set(AngPosControl_param_set& d)
		{
			copy_data(d);
		}


		~AngPosControl_param_set() {}


		void zero()
		{
			kp_vel.zero();	ki_vel.zero();
			kd_vel.zero();

			kp_force.zero();	ki_force.zero();
			kd_force.zero();
		}


		AngPosControl_param_set& operator=(const AngPosControl_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class AngPosControl_params
{
	private:
		void copy_data(const AngPosControl_params& d)
		{
			reference = d.reference;
			actual = d.actual;
			vel_control = d.vel_control;
		}


	public:
		AngPosControl_param_set reference, actual;
		Task_execution vel_control;

		AngPosControl_params()
		{
			reference.zero();
			actual.zero();
			vel_control.status.zero();
		}


		AngPosControl_params(AngPosControl_params& d)
		{
			copy_data(d);
		}


		~AngPosControl_params() {}


		AngPosControl_params& operator=(const AngPosControl_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* ANG_POS_CONTROL_PARAMS_H_ */
