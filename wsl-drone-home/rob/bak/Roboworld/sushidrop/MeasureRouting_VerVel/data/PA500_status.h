/*
 * PA500_status.h
 *
 *  Created on: Apr 09, 2020
 *      Author: mx
 */

#ifndef PA500_STATUS_H_
#define PA500_STATUS_H_

#include <math.h>
#include "../generic/custom_types.h"
#include "../generic/Variable.h"
#include <vector>
#include <string>



class PA500_status
{
	private:

		void copy_data(const PA500_status&d)
		{
			altitude = d.altitude;

			device_status = d.device_status;
			powered = d.powered;
		}


	public:

		DoubleVariable altitude;
		IntVariable device_status,powered;
		

		PA500_status()
		{
			altitude.zero();
			powered.zero();
			device_status.zero();
		}


		PA500_status(PA500_status&d)
		{
			copy_data(d);
		}


		~PA500_status(){}


		PA500_status& operator=(const PA500_status&d)
		{
			copy_data(d);
			return *this;
		}



};


#endif /* PA500_STATUS_H_ */
