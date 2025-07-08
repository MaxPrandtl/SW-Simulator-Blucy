/*
 * CTD_status.h
 *
 *  Created on: Apr 15, 2020
 *      Author: mx
 */

#ifndef CTD_STATUS_H_
#define CTD_STATUS_H_

#include <math.h>
#include "../generic/custom_types.h"
#include "../generic/Variable.h"
#include <vector>
#include <string>



class CTD_status
{
	private:

		void copy_data(const CTD_status&d)
		{
			conductivity = d.conductivity;
			temperature = d.temperature;
			depth = d.depth;
			
			device_status = d.device_status;
		}


	public:

		DoubleVariable conductivity,temperature,depth;
		IntVariable device_status;
		

		CTD_status()
		{
			conductivity.zero();
			temperature.zero();
			depth.zero();
			
			device_status.zero();
		}


		CTD_status(CTD_status&d)
		{
			copy_data(d);
		}


		~CTD_status(){}


		CTD_status& operator=(const CTD_status&d)
		{
			copy_data(d);
			return *this;
		}



};


#endif /* CTD_STATUS_H_ */
