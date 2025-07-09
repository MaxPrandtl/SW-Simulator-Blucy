/*
 * AngFilter.h
 *
 *  Created on: Apr 21, 2020
 *      Author: mx
 */

#ifndef ANG_FILTER_H_
#define ANG_FILTER_H_

#include <unistd.h>
#include <string>
#include "../../../../../generic/define.h"
#include "../../../../../generic/custom_types.h"
#include "../../../../../generic/RobotMath.h"
#include "../../../../../generic/matrix.h"
#include "../../../../../generic/vector.h"
#include "../../../../../generic/RobotThread.h"
#include "../../../../../data/Time_status.h"
#include "../../../../../data/NGC_status.h"
#include "../../../../../data/Task_execution.h"
#include "../../../../../data/DataAccess.h"
#include "../../../../../ngc/Filter.h"
#include "AngFilter_params.h"


class AngFilter :public Filter
{
	private:

		DataAccess<NGC_status>* ngc_access = NULL;
		DataAccess<AngFilter_params>* params_access = NULL;

		AngFilter_params params;
		

		Matrix P, Q, R, H, K, OMEGA, A, B;
		Matrix state, measure, in;

		bool start = true;

		const double convergenceThr = 10.0;

	public:
		AngFilter(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<AngFilter_params>* Params_access);
		~AngFilter();

		virtual void init();
		virtual void compute();
		double filter();
		double inv_thrust_curve(double v, double a, double b);
		
		virtual void parse_config(char* value, double val);
		virtual void set_config(FILE* f);

		virtual void get_params();
		virtual void set_params();
};

void* start_filter(void* arg);

#endif /* ANG_FILTER_H_ */
