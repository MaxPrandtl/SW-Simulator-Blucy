/*
 * FOG_status.h
 *
 *  Created on: Apr 07, 2020
 *      Author: mx
 */

#ifndef FOG_STATUS_H_
#define FOG_STATUS_H_

#include <math.h>
#include "../generic/custom_types.h"
#include "../generic/Variable.h"
#include <vector>
#include <string>



class FOG_status
{
	private:

		void copy_data(const FOG_status&d)
		{
			roll = d.roll;			pitch = d.pitch;			heading = d.heading;
			rollRate = d.rollRate;	pitchRate = d.pitchRate;	headingRate = d.headingRate;

			device_status = d.device_status;
		}


	public:

		DoubleVariable roll,pitch,heading;
		DoubleVariable rollRate, pitchRate, headingRate;
		IntVariable device_status;
		

		FOG_status()
		{
			roll.zero();		pitch.zero();		heading.zero();
			rollRate.zero();	pitchRate.zero();	headingRate.zero();

			device_status.zero();
		}


		FOG_status(FOG_status&d)
		{
			copy_data(d);
		}


		~FOG_status(){}


		FOG_status& operator=(const FOG_status&d)
		{
			copy_data(d);
			return *this;
		}



};


#endif /* FOG_STATUS_H_ */
