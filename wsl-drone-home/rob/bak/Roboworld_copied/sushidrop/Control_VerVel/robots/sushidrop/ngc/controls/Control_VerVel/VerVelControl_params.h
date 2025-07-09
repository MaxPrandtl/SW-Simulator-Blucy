/*
 * VerVelControl_params.h
 *
 *  Created on: May 26, 2020
 *      Author: mx
 */

#ifndef ANG_VEL_CONTROL_PARAMS_H_
#define ANG_VEL_CONTROL_PARAMS_H_


#include "../../../../../generic/Variable.h"

 
class VerVelControl_param_set
{
	private:
		void copy_data(const VerVelControl_param_set& d)
		{
			kp = d.kp;
			ki = d.ki;
			kd = d.kd;
		}


	public:
		DoubleVariable kp, ki, kd;
		

		VerVelControl_param_set()
		{
			zero();
		}


		VerVelControl_param_set(VerVelControl_param_set& d)
		{
			copy_data(d);
		}


		~VerVelControl_param_set() {}


		void zero()
		{
			kp.zero();	ki.zero();
			kd.zero();
		}


		VerVelControl_param_set& operator=(const VerVelControl_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class VerVelControl_params
{
	private:
		void copy_data(const VerVelControl_params& d)
		{
			reference = d.reference;
			actual = d.actual;
		}


	public:
		VerVelControl_param_set reference, actual;
		

		VerVelControl_params()
		{
			reference.zero();
			actual.zero();
		}


		VerVelControl_params(VerVelControl_params& d)
		{
			copy_data(d);
		}


		~VerVelControl_params() {}


		VerVelControl_params& operator=(const VerVelControl_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* ANG_VEL_CONTROL_PARAMS_H_ */
