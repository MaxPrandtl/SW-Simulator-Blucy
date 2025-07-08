/*
 * HorPosFilter_params.h
 *
 *  Created on: May 05, 2020
 *      Author: mx
 */

#ifndef HORPOS_FILTER_PARAMS_H_
#define HORPOS_FILTER_PARAMS_H_


#include "../../../../../generic/Variable.h"

 
class HorPosFilter_param_set
{
	private:
		void copy_data(const HorPosFilter_param_set& d)
		{
			q_xy = d.q_xy;
			r_xy_11 = d.r_xy_11;	r_xy_12 = d.r_xy_12;	r_xy_22 = d.r_xy_22;
		}


	public:
		DoubleVariable q_xy;
		DoubleVariable r_xy_11, r_xy_12, r_xy_22;


		HorPosFilter_param_set()
		{
			zero();
		}


		HorPosFilter_param_set(HorPosFilter_param_set& d)
		{
			copy_data(d);
		}


		~HorPosFilter_param_set() {}


		void zero()
		{
			q_xy.zero();
			r_xy_11.zero();		r_xy_12.zero();		r_xy_22.zero();
		}


		HorPosFilter_param_set& operator=(const HorPosFilter_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class HorPosFilter_params
{
	private:
		void copy_data(const HorPosFilter_params& d)
		{
			reference = d.reference;
			actual = d.actual;
		}


	public:
		HorPosFilter_param_set reference, actual;
		

		HorPosFilter_params()
		{
			reference.zero();
			actual.zero();
		}


		HorPosFilter_params(HorPosFilter_params& d)
		{
			copy_data(d);
		}


		~HorPosFilter_params() {}


		HorPosFilter_params& operator=(const HorPosFilter_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* HORPOS_FILTER_PARAMS_H_ */
