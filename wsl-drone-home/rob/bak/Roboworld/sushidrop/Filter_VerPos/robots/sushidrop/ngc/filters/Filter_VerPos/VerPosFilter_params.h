/*
 * VerPosFilter_params.h
 *
 *  Created on: May 12, 2020
 *      Author: mx
 */

#ifndef VERPOS_FILTER_PARAMS_H_
#define VERPOS_FILTER_PARAMS_H_


#include "../../../../../generic/Variable.h"

 
class VerPosFilter_param_set
{
	private:
		void copy_data(const VerPosFilter_param_set& d)
		{
			q_z = d.q_z;	r_z = d.r_z;
		}


	public:
		DoubleVariable q_z, r_z;

		VerPosFilter_param_set()
		{
			zero();
		}


		VerPosFilter_param_set(VerPosFilter_param_set& d)
		{
			copy_data(d);
		}


		~VerPosFilter_param_set() {}


		void zero()
		{
			q_z.zero();		r_z.zero();
		}


		VerPosFilter_param_set& operator=(const VerPosFilter_param_set& d)
		{
			copy_data(d);
			return *this;
		}

};



class VerPosFilter_params
{
	private:
		void copy_data(const VerPosFilter_params& d)
		{
			reference = d.reference;
			actual = d.actual;
		}


	public:
		VerPosFilter_param_set reference, actual;
		

		VerPosFilter_params()
		{
			reference.zero();
			actual.zero();
		}


		VerPosFilter_params(VerPosFilter_params& d)
		{
			copy_data(d);
		}


		~VerPosFilter_params() {}


		VerPosFilter_params& operator=(const VerPosFilter_params& d)
		{
			copy_data(d);
			return *this;
		}

};

#endif /* VERPOS_FILTER_PARAMS_H_ */
