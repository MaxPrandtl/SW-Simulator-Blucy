/*
 * USBL_status.h
 *
 *  Created on: May 13, 2020
 *      Author: mx
 */

#ifndef USBL_STATUS_H_
#define USBL_STATUS_H_

#include <math.h>
#include "../generic/custom_types.h"
#include "../generic/Variable.h"
#include <vector>
#include <string>


enum AcousticLinkStatus
{
	SEND = 0,
	SENDING,
	SENT,
	SEND_ERROR,
	RECEIVE,
	RECEIVING,
	RECEIVED
};




class USBL_status
{
	private:

		void copy_data(const USBL_status&d)
		{
			x = d.x;		y = d.y;		z = d.z;
			device_status = d.device_status;
			comm_status = d.comm_status;
			powered = d.powered;
			msgSend = d.msgSend;
			msgRecv = d.msgRecv;
			
			pinger_to_usbl = d.pinger_to_usbl;
			usbl_to_pinger = d.usbl_to_pinger;
		}


	public:

		DoubleVariable x,y,z;
		IntVariable device_status, powered,comm_status;
		StringVariable msgSend, msgRecv;
		
		StringVariable pinger_to_usbl, usbl_to_pinger;

		USBL_status()
		{
			x.zero();		y.zero();		z.zero();
			device_status.zero();
			powered.zero();
			comm_status.zero();
			msgSend.zero();
			msgRecv.zero();
			
			pinger_to_usbl.zero();
			usbl_to_pinger.zero();
		}


		USBL_status(USBL_status&d)
		{
			copy_data(d);
		}


		~USBL_status(){}


		USBL_status& operator=(const USBL_status&d)
		{
			copy_data(d);
			return *this;
		}
};


#endif /* USBL_STATUS_H_ */
