/*
 * ThrustMapping.h
 *
 *  Created on: Apr 24, 2020
 *      Author: mx
 */

#ifndef THRUST_MAPPING_H_
#define THRUST_MAPPING_H_

#include <unistd.h>
#include <string>
#include "../../../../generic/define.h"
#include "../../../../generic/RobotTask.h"
#include "../../../../generic/RobotMath.h"
#include "../../../../generic/custom_types.h"
#include "../../data/SushidropMotors_status.h"
#include "../../../../data/NGC_status.h"
#include "../../../../data/DataAccess.h"
#include "../../../../generic/matrix.h"
#include "../../../../generic/vector.h"



class ThrustMapping :public RobotTask
{
	private:
		DataAccess<SushidropMotors_status>* motors_access = NULL;
		DataAccess<NGC_status>* ngc_access = NULL;
		
		double a_pos, b_pos, a_neg, b_neg;
		double ang_motor;
		Matrix versor;
		Matrix pos;
		Matrix J, Jpinv;
		double max_fu, max_fv, max_fw, max_tr;

	public:
		ThrustMapping(const char* name, DataAccess<SushidropMotors_status>* Motors_access, DataAccess<NGC_status>* NGC_access, DataAccess<Time_status>* Time_access, DataAccess<Task_execution>* Task_access);
		~ThrustMapping();

		virtual void execute();

		double thrust_curve(double v);
		double inv_thrust_curve(double v);
		void computeForceTorque(Matrix& v, Matrix& versor, Matrix& pos, Matrix& force);
};

void* start_thrust_mapping_europe(void* arg);

#endif /* THRUST_MAPPING_H_ */
