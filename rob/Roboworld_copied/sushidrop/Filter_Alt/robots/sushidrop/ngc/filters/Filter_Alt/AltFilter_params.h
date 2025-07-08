/*
 * AltFilter_params.h
 *
 *  Created on: May 12, 2020
 *      Author: mx
 */

#ifndef ALT_FILTER_PARAMS_H_
#define ALT_FILTER_PARAMS_H_


#include "../../../../../generic/Variable.h"

 
class AltFilter_param_set
{
	private:
		void copy_data(const AltFilter_param_set& d)
		{
			q_a = d.q_a;	r_a = d.r_a;
		}


	public:
		DoubleVariable q_a, r_a;

		AltFilter_param_set()
		{
			zero();
		}


		AltFilter_param_set(AltFilter_param_set& d)
		{
			copy_data(d);
		}


		~AltFilter_param_set() {}


		void zero()
		{
			q_a.zero();		r_a.zero();
		}


		AltFilter_param_set& operator=(const AltFilter_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class AltFilter_params
{
	private:
		void copy_data(const AltFilter_params& d)
		{
			reference = d.reference;
			actual = d.actual;
		}


	public:
		AltFilter_param_set reference, actual;
		

		AltFilter_params()
		{
			reference.zero();
			actual.zero();
		}


		AltFilter_params(AltFilter_params& d)
		{
			copy_data(d);
		}


		~AltFilter_params() {}


		AltFilter_params& operator=(const AltFilter_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* ALT_FILTER_PARAMS_H_ */
