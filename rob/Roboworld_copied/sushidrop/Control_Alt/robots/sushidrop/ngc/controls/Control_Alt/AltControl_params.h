/*
 * AltControl_params.h
 *
 *  Created on: Jun 10, 2020
 *      Author: mx
 */

#ifndef ALT_CONTROL_PARAMS_H_
#define ALT_CONTROL_PARAMS_H_


#include "../../../../../generic/Variable.h"
#include "../../../../../data/Task_execution.h"

 
class AltControl_param_set
{
	private:
		void copy_data(const AltControl_param_set& d)
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

		AltControl_param_set()
		{
			zero();
		}


		AltControl_param_set(AltControl_param_set& d)
		{
			copy_data(d);
		}


		~AltControl_param_set() {}


		void zero()
		{
			kp_vel.zero();	ki_vel.zero();
			kd_vel.zero();

			kp_force.zero();	ki_force.zero();
			kd_force.zero();
		}


		AltControl_param_set& operator=(const AltControl_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class AltControl_params
{
	private:
		void copy_data(const AltControl_params& d)
		{
			reference = d.reference;
			actual = d.actual;
			vel_control = d.vel_control;
		}


	public:
		AltControl_param_set reference, actual;
		Task_execution vel_control;

		AltControl_params()
		{
			reference.zero();
			actual.zero();
			vel_control.status.zero();
		}


		AltControl_params(AltControl_params& d)
		{
			copy_data(d);
		}


		~AltControl_params() {}


		AltControl_params& operator=(const AltControl_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* ALT_CONTROL_PARAMS_H_ */
