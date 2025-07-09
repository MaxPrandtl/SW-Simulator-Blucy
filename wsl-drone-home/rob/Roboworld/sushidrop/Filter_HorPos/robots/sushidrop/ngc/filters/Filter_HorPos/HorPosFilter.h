/*
 * HorPosFilter.h
 *
 *  Created on: May 05, 2020
 *      Author: mx
 */

#ifndef HORPOS_FILTER_H_
#define HORPOS_FILTER_H_

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
#include "HorPosFilter_params.h"


class HorPosFilter :public Filter
{
	private:

		DataAccess<NGC_status>* ngc_access = NULL;
		DataAccess<HorPosFilter_params>* params_access = NULL;

		HorPosFilter_params params;
		

		Matrix P, Q, R, H, K, OMEGA, A, B;
		Matrix state, measure, in;

		bool start = true;

		const double convergenceThr = 10.0;

	public:
		HorPosFilter(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<HorPosFilter_params>* Params_access);
		~HorPosFilter();

		virtual void init();
		virtual void compute();
		double filter();
		
		virtual void parse_config(char* value, double val);
		void set_config(FILE* f);

		virtual void get_params();
		virtual void set_params();
};

void* start_filter(void* arg);

#endif /* HORPOS_FILTER_H_ */
