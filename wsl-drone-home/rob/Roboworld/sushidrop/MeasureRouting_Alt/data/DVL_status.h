/*
 * DVL_status.h
 *
 *  Created on: Apr 08, 2020
 *      Author: mx
 */

#ifndef DVL_STATUS_H_
#define DVL_STATUS_H_

#include <math.h>
#include "../generic/custom_types.h"
#include "../generic/Variable.h"
#include <vector>
#include <string>

#define RANGE_ANGLE (M_PI/6.0)

class DVL_status
{
	private:

		void copy_data(const DVL_status&d)
		{
			range1 = d.range1;		range2 = d.range2;		range3 = d.range3;		range4 = d.range4;
			surge = d.surge;		sway = d.sway;			heave = d.heave;		error = d.error;
			altitude = d.altitude;
			valid = d.valid;		device_status = d.device_status;	powered = d.powered;
		}


	public:

		DoubleVariable range1, range2, range3, range4;
		DoubleVariable surge, sway, heave, error,altitude;
		IntVariable valid, device_status,powered;
		

		DVL_status()
		{
			range1.zero();		range2.zero();		range3.zero();		range4.zero();
			surge.zero();		sway.zero();		heave.zero();		error.zero();
			altitude.zero();
			valid.zero();		device_status.zero();	powered.zero();
		}


		DVL_status(DVL_status&d)
		{
			copy_data(d);
		}


		~DVL_status(){}


		DVL_status& operator=(const DVL_status&d)
		{
			copy_data(d);
			return *this;
		}


};


#endif /* DVL_STATUS_H_ */
