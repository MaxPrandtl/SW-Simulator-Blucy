/*
 * VerVelFilter_params.h
 *
 *  Created on: May 12, 2020
 *      Author: mx
 */

#ifndef VERVEL_FILTER_PARAMS_H_
#define VERVEL_FILTER_PARAMS_H_


#include "../../../../../generic/Variable.h"

 
class VerVelFilter_param_set
{
	private:
		void copy_data(const VerVelFilter_param_set& d)
		{
			q_w = d.q_w;	r_w = d.r_w;
			Kw = d.Kw;		bw = d.bw;
		}


	public:
		DoubleVariable q_w, r_w;
		DoubleVariable Kw, bw;

		VerVelFilter_param_set()
		{
			zero();
		}


		VerVelFilter_param_set(VerVelFilter_param_set& d)
		{
			copy_data(d);
		}


		~VerVelFilter_param_set() {}


		void zero()
		{
			q_w.zero();		r_w.zero();
			Kw.zero();		bw.zero();
		}


		VerVelFilter_param_set& operator=(const VerVelFilter_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class VerVelFilter_params
{
	private:
		void copy_data(const VerVelFilter_params& d)
		{
			reference = d.reference;
			actual = d.actual;
		}


	public:
		VerVelFilter_param_set reference, actual;
		

		VerVelFilter_params()
		{
			reference.zero();
			actual.zero();
		}


		VerVelFilter_params(VerVelFilter_params& d)
		{
			copy_data(d);
		}


		~VerVelFilter_params() {}


		VerVelFilter_params& operator=(const VerVelFilter_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* VERVEL_FILTER_PARAMS_H_ */
