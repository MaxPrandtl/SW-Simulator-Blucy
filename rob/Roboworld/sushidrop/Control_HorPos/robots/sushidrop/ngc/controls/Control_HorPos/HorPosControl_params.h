/*
 * HorPosControl_params.h
 *
 *  Created on: May 27, 2020
 *      Author: mx
 */

#ifndef HOR_POS_CONTROL_PARAMS_H_
#define HOR_POS_CONTROL_PARAMS_H_


#include "../../../../../generic/Variable.h"
#include "../../../../../data/Task_execution.h"

 
class HorPosControl_param_set
{
	private:
		void copy_data(const HorPosControl_param_set& d)
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

		HorPosControl_param_set()
		{
			zero();
		}


		HorPosControl_param_set(HorPosControl_param_set& d)
		{
			copy_data(d);
		}


		~HorPosControl_param_set() {}


		void zero()
		{
			kp_vel.zero();	ki_vel.zero();
			kd_vel.zero();

			kp_force.zero();	ki_force.zero();
			kd_force.zero();
		}


		HorPosControl_param_set& operator=(const HorPosControl_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class HorPosControl_params
{
	private:
		void copy_data(const HorPosControl_params& d)
		{
			reference = d.reference;
			actual = d.actual;
			vel_control = d.vel_control;
		}


	public:
		HorPosControl_param_set reference, actual;
		Task_execution vel_control;

		HorPosControl_params()
		{
			reference.zero();
			actual.zero();
			vel_control.status.zero();
		}


		HorPosControl_params(HorPosControl_params& d)
		{
			copy_data(d);
		}


		~HorPosControl_params() {}


		HorPosControl_params& operator=(const HorPosControl_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* HOR_POS_CONTROL_PARAMS_H_ */
