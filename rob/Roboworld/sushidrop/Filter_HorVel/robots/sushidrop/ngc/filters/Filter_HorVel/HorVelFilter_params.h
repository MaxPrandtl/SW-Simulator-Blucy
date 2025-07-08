/*
 * HorVelFilter_params.h
 *
 *  Created on: May 11, 2020
 *      Author: mx
 */

#ifndef HORVEL_FILTER_PARAMS_H_
#define HORVEL_FILTER_PARAMS_H_


#include "../../../../../generic/Variable.h"

 
class HorVelFilter_param_set
{
	private:
		void copy_data(const HorVelFilter_param_set& d)
		{
			q_uv = d.q_uv;			q_current = d.q_current;
			r_uv_11 = d.r_uv_11;	r_uv_12 = d.r_uv_12;	r_uv_22 = d.r_uv_22;
			Ku = d.Ku;				bu = d.bu;
			Kv = d.Kv;				bv = d.bv;
		}


	public:
		DoubleVariable q_uv, q_current;
		DoubleVariable r_uv_11, r_uv_12, r_uv_22;
		DoubleVariable Ku, bu, Kv, bv;

		HorVelFilter_param_set()
		{
			zero();
		}


		HorVelFilter_param_set(HorVelFilter_param_set& d)
		{
			copy_data(d);
		}


		~HorVelFilter_param_set() {}


		void zero()
		{
			q_uv.zero();		q_current.zero();
			r_uv_11.zero();		r_uv_12.zero();		r_uv_22.zero();
			Ku.zero();			bu.zero();
			Kv.zero();			bv.zero();
		}


		HorVelFilter_param_set& operator=(const HorVelFilter_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class HorVelFilter_params
{
	private:
		void copy_data(const HorVelFilter_params& d)
		{
			reference = d.reference;
			actual = d.actual;
		}


	public:
		HorVelFilter_param_set reference, actual;
		

		HorVelFilter_params()
		{
			reference.zero();
			actual.zero();
		}


		HorVelFilter_params(HorVelFilter_params& d)
		{
			copy_data(d);
		}


		~HorVelFilter_params() {}


		HorVelFilter_params& operator=(const HorVelFilter_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* HORVEL_FILTER_PARAMS_H_ */
