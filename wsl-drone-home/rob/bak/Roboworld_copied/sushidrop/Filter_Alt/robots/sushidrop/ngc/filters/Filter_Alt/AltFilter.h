/*
 * AltFilter.h
 *
 *  Created on: May 12, 2020
 *      Author: mx
 */

#ifndef ALT_FILTER_H_
#define ALT_FILTER_H_

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
#include "AltFilter_params.h"


class AltFilter :public Filter
{
	private:

		DataAccess<NGC_status>* ngc_access = NULL;
		DataAccess<AltFilter_params>* params_access = NULL;

		AltFilter_params params;
		

		Matrix P, Q, R, H, K, OMEGA, A, B;
		Matrix state, measure, in;

		bool start = true;

		const double convergenceThr = 10.0;

	public:
		AltFilter(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<AltFilter_params>* Params_access);
		~AltFilter();

		virtual void init();
		virtual void compute();
		double filter();
		
		virtual void parse_config(char* value, double val);
		void set_config(FILE* f);

		virtual void get_params();
		virtual void set_params();
};

void* start_filter(void* arg);

#endif /* ALT_FILTER_H_ */
