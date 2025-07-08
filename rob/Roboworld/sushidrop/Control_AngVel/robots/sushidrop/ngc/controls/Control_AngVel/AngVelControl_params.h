/*
 * AngVelControl_params.h
 *
 *  Created on: May 26, 2020
 *      Author: mx
 */

#ifndef ANG_VEL_CONTROL_PARAMS_H_
#define ANG_VEL_CONTROL_PARAMS_H_


#include "../../../../../generic/Variable.h"

 
class AngVelControl_param_set
{
	private:
		void copy_data(const AngVelControl_param_set& d)
		{
			kp = d.kp;
			ki = d.ki;
			kd = d.kd;
		}


	public:
		DoubleVariable kp, ki, kd;
		

		AngVelControl_param_set()
		{
			zero();
		}


		AngVelControl_param_set(AngVelControl_param_set& d)
		{
			copy_data(d);
		}


		~AngVelControl_param_set() {}


		void zero()
		{
			kp.zero();	ki.zero();
			kd.zero();
		}


		AngVelControl_param_set& operator=(const AngVelControl_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class AngVelControl_params
{
	private:
		void copy_data(const AngVelControl_params& d)
		{
			reference = d.reference;
			actual = d.actual;
		}


	public:
		AngVelControl_param_set reference, actual;
		

		AngVelControl_params()
		{
			reference.zero();
			actual.zero();
		}


		AngVelControl_params(AngVelControl_params& d)
		{
			copy_data(d);
		}


		~AngVelControl_params() {}


		AngVelControl_params& operator=(const AngVelControl_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* ANG_VEL_CONTROL_PARAMS_H_ */
