/*
 * SushidropMotors_status.h
 *
 *  Created on: Apr 14, 2021
 *      Author: mx
 */

#ifndef SUSHIDROP_MOTORS_STATUS_H_
#define SUSHIDROP_MOTORS_STATUS_H_

#include <math.h>
#include "../../../generic/custom_types.h"
#include "../../../generic/Variable.h"
#include <vector>
#include <string>



class Motor_status
{
	private:

		void copy_data(const Motor_status&d)
		{
			velocity_reference = d.velocity_reference;
			enable_command = d.enable_command;
			enable_status = d.enable_status;
		}


	public:

		DoubleVariable velocity_reference;
		IntVariable enable_command,enable_status;
		

		Motor_status()
		{
			velocity_reference.zero();
			enable_command.zero();
			enable_status.zero();
		}


		Motor_status(Motor_status&d)
		{
			copy_data(d);
		}


		~Motor_status(){}


		Motor_status& operator=(const Motor_status&d)
		{
			copy_data(d);
			return *this;
		}
};


class SushidropMotors_status
{
private:

	void copy_data(const SushidropMotors_status& d)
	{
		hl = d.hl;
		hr = d.hr;
		vf = d.vf;
		vr = d.vr;
		bow = d.bow;
		stern = d.stern;
	}


public:

	Motor_status hl, hr, vf, vr, bow, stern;


	SushidropMotors_status()
	{
	
	}


	SushidropMotors_status(SushidropMotors_status& d)
	{
		copy_data(d);
	}


	~SushidropMotors_status() {}


	SushidropMotors_status& operator=(const SushidropMotors_status& d)
	{
		copy_data(d);
		return *this;
	}



};


#endif /* SUSHIDROP_MOTORS_STATUS_H_ */
