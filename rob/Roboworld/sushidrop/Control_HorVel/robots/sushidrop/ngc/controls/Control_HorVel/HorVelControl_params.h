/*
 * HorVelControl_params.h
 *
 *  Created on: May 26, 2020
 *      Author: mx
 */

#ifndef HOR_VEL_CONTROL_PARAMS_H_
#define HOR_VEL_CONTROL_PARAMS_H_


#include "../../../../../generic/Variable.h"

 
class HorVelControl_param_set
{
	private:
		void copy_data(const HorVelControl_param_set& d)
		{
			kp_u = d.kp_u;
			ki_u = d.ki_u;
			kd_u = d.kd_u;
			
			kp_v = d.kp_v;
			ki_v = d.ki_v;
			kd_v = d.kd_v;
		}


	public:
		DoubleVariable kp_u, ki_u, kd_u;
		DoubleVariable kp_v, ki_v, kd_v;
		

		HorVelControl_param_set()
		{
			zero();
		}


		HorVelControl_param_set(HorVelControl_param_set& d)
		{
			copy_data(d);
		}


		~HorVelControl_param_set() {}


		void zero()
		{
			kp_u.zero();	ki_u.zero();
			kd_u.zero();	

			kp_v.zero();	ki_v.zero();
			kd_v.zero();
		}


		HorVelControl_param_set& operator=(const HorVelControl_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class HorVelControl_params
{
	private:
		void copy_data(const HorVelControl_params& d)
		{
			reference = d.reference;
			actual = d.actual;
		}


	public:
		HorVelControl_param_set reference, actual;
		

		HorVelControl_params()
		{
			reference.zero();
			actual.zero();
		}


		HorVelControl_params(HorVelControl_params& d)
		{
			copy_data(d);
		}


		~HorVelControl_params() {}


		HorVelControl_params& operator=(const HorVelControl_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* HOR_VEL_CONTROL_PARAMS_H_ */
