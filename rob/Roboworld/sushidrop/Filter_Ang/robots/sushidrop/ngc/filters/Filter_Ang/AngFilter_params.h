/*
 * AngFilter_params.h
 *
 *  Created on: Apr 22, 2020
 *      Author: mx
 */

#ifndef ANG_FILTER_PARAMS_H_
#define ANG_FILTER_PARAMS_H_


#include "../../../../../generic/Variable.h"

 
class AngFilter_param_set
{
	private:
		void copy_data(const AngFilter_param_set& d)
		{
			q_psi = d.q_psi;	r_psi = d.r_psi;
			q_r = d.q_r;		r_r = d.r_r;
			Kr = d.Kr;			br = d.br;
		}


	public:
		DoubleVariable q_psi, r_psi;
		DoubleVariable q_r, r_r;
		DoubleVariable Kr, br;


		AngFilter_param_set()
		{
			zero();
		}


		AngFilter_param_set(AngFilter_param_set& d)
		{
			copy_data(d);
		}


		~AngFilter_param_set() {}


		void zero()
		{
			q_psi.zero();	r_psi.zero();
			q_r.zero();		r_r.zero();
			Kr.zero();		br.zero();
		}


		AngFilter_param_set& operator=(const AngFilter_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class AngFilter_params
{
	private:
		void copy_data(const AngFilter_params& d)
		{
			reference = d.reference;
			actual = d.actual;
		}


	public:
		AngFilter_param_set reference, actual;
		

		AngFilter_params()
		{
			reference.zero();
			actual.zero();
		}


		AngFilter_params(AngFilter_params& d)
		{
			copy_data(d);
		}


		~AngFilter_params() {}


		AngFilter_params& operator=(const AngFilter_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* ANG_FILTER_PARAMS_H_ */
