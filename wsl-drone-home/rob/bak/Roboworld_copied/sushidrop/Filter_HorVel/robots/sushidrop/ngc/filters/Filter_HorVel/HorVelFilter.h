/*
 * HorVelFilter.h
 *
 *  Created on: May 11, 2020
 *      Author: mx
 */

#ifndef HORVEL_FILTER_H_
#define HORVEL_FILTER_H_

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
#include "HorVelFilter_params.h"


class HorVelFilter :public Filter
{
	private:

		DataAccess<NGC_status>* ngc_access = NULL;
		DataAccess<HorVelFilter_params>* params_access = NULL;

		HorVelFilter_params params;
		

		Matrix P, Q, R, H, K, OMEGA, A, B;
		Matrix state, measure, in;

		bool start = true;

		const double convergenceThr = 50.0;

	public:
		HorVelFilter(const char* name, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access, DataAccess<HorVelFilter_params>* Params_access);
		~HorVelFilter();

		virtual void init();
		virtual void compute();
		double filter();
		double inv_thrust_curve(double v, double a, double b);
		
		virtual void parse_config(char* value, double val);
		void set_config(FILE* f);

		virtual void get_params();
		virtual void set_params();
};

void* start_filter(void* arg);

#endif /* HORVEL_FILTER_H_ */
