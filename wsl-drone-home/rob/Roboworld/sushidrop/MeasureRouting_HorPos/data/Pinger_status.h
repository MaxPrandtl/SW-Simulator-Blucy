/*
 * Pinger_status.h
 *
 *  Created on: Apr 10, 2020
 *      Author: mx
 */

#ifndef PINGER_STATUS_H_
#define PINGER_STATUS_H_

#include <math.h>
#include "../generic/custom_types.h"
#include "../generic/Variable.h"
#include <vector>
#include <string>



class Pinger_status
{
	private:

		void copy_data(const Pinger_status&d)
		{
			x = d.x;		y = d.y;		z = d.z;
			//pinger_to_usbl = d.pinger_to_usbl;
			//usbl_to_pinger = d.usbl_to_pinger;
			device_status = d.device_status;
		}


	public:

		DoubleVariable x,y,z;
		//StringVariable pinger_to_usbl, usbl_to_pinger;
		IntVariable device_status;
		

		Pinger_status()
		{
			x.zero();		y.zero();		z.zero();
			//pinger_to_usbl.zero();
			//usbl_to_pinger.zero();
			device_status.zero();
		}


		Pinger_status(Pinger_status&d)
		{
			copy_data(d);
		}


		~Pinger_status(){}


		Pinger_status& operator=(const Pinger_status&d)
		{
			copy_data(d);
			return *this;
		}
};


#endif /* PINGER_STATUS_H_ */
