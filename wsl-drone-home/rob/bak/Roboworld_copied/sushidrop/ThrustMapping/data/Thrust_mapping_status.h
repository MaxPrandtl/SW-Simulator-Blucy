/*
 * Thrust_mapping_status.h
 *
 *  Created on: Jan 27, 2021
 *      Author: mx
 */

#ifndef THUST_MAPPING_STATUS_H_
#define THUST_MAPPING_STATUS_H_

#include "../generic/custom_types.h"
#include "../generic/Variable.h"

enum Thrust_mapping_modes
{
	_4WD = 0,
	DP = 1,
	PINV = 2,
};


class Thrust_mapping_status
{
private:

	void copy_data(const Thrust_mapping_status& d)
	{
		command = d.command;
		status = d.status;
	}


public:

	IntVariable status,command;
	

	bool updated;

	Thrust_mapping_status()
	{
		command.zero();
		status.zero();
	}


	Thrust_mapping_status(Thrust_mapping_status& d)
	{
		copy_data(d);
	}


	~Thrust_mapping_status() {}


	Thrust_mapping_status& operator=(const Thrust_mapping_status& d)
	{
		copy_data(d);
		return *this;
	}

};

#endif /* THUST_MAPPING_STATUS_H_ */
