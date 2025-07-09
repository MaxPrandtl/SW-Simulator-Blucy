/*
 * VerPosFilter.h
 *
 *  Created on: May 12, 2020
 *      Author: mx
 */

#ifndef VERPOS_FILTER_H_
#define VERPOS_FILTER_H_

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
#include "VerPosFilter_params.h"


class VerPosFilter :public Filter
{
	private:

		DataAccess<NGC_status>* ngc_access = NULL;
		DataAccess<VerPosFilter_params>* params_access = NULL;

		VerPosFilter_params params;
		

		Matrix P, Q, R, H, K, OMEGA, A, B;
		Matrix state, measure, in;

		bool start = true;

		const double convergenceThr = 10.0;

	public:
		VerPosFilter(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<VerPosFilter_params>* Params_access);
		~VerPosFilter();

		virtual void init();
		virtual void compute();
		double filter();
		
		virtual void parse_config(char* value, double val);
		void set_config(FILE* f);

		virtual void get_params();
		virtual void set_params();
};

void* start_filter(void* arg);

#endif /* VERPOS_FILTER_H_ */
