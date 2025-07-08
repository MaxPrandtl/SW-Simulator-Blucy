/*
 * IO_sushidrop_status.h
 *
 *  Created on: Apr 07, 2021
 *      Author: mx
 */

#ifndef IO_SUSHIDROP_STATUS_H_
#define IO_SUSHIDROP_STATUS_H_

#include <math.h>
#include "../generic/custom_types.h"
#include "../generic/Variable.h"
#include <vector>
#include <string>


#define SUSHIDROP_DIO_MAX_CHANNELS 48
#define SUSHIDROP_AD_MAX_CHANNELS 8
#define SUSHIDROP_DA_MAX_CHANNELS 8

#define SUSHIDROP_DA_MOTOR_HL_VREF 2
#define SUSHIDROP_DA_MOTOR_HR_VREF 1
#define SUSHIDROP_DA_MOTOR_VF_VREF 3
#define SUSHIDROP_DA_MOTOR_VR_VREF 4
#define SUSHIDROP_DA_MOTOR_BOW_VREF 5
#define SUSHIDROP_DA_MOTOR_STERN_VREF 6

#define SUSHIDROP_DIO_MOTOR_HL_POWER 0
#define SUSHIDROP_DIO_MOTOR_HR_POWER 1
#define SUSHIDROP_DIO_MOTOR_VF_POWER 2
#define SUSHIDROP_DIO_MOTOR_VR_POWER 3
#define SUSHIDROP_DIO_MOTOR_BOW_POWER 4
#define SUSHIDROP_DIO_MOTOR_STERN_POWER 5



enum MOTOR_STATUS
{
	MOTOR_OFF = 0,
	MOTOR_POWERED = 1,
	MOTOR_ENABLED = 2,
	MOTOR_FAULT = 3
};








class IO_sushidrop_status
{
	private:

		void copy_data(const IO_sushidrop_status&d)
		{
			for (int i = 0; i < SUSHIDROP_DIO_MAX_CHANNELS; i++)
			{
				digitalStatus[i] = d.digitalStatus[i];
				digitalCommand[i] = d.digitalCommand[i];
			}

			for (int i = 0; i < SUSHIDROP_AD_MAX_CHANNELS; i++)
				analogStatus[i] = d.analogStatus[i];

			for (int i = 0; i < SUSHIDROP_DA_MAX_CHANNELS; i++)
			{
				analogCommand[i] = d.analogCommand[i];
			}
			
		
			autoStartStop_command = d.autoStartStop_command;
			autoStartStop_status = d.autoStartStop_status;

			device_status = d.device_status;
		}


	public:

		DoubleVariable analogStatus[SUSHIDROP_AD_MAX_CHANNELS];
		DoubleVariable analogCommand[SUSHIDROP_DA_MAX_CHANNELS];
		IntVariable digitalStatus[SUSHIDROP_DIO_MAX_CHANNELS];
		IntVariable digitalCommand[SUSHIDROP_DIO_MAX_CHANNELS];
		IntVariable autoStartStop_command;
		IntVariable autoStartStop_status;
		IntVariable device_status;
		

		IO_sushidrop_status()
		{
			for (int i = 0; i < SUSHIDROP_DIO_MAX_CHANNELS; i++)
			{
				digitalStatus[i].zero();
				digitalCommand[i].zero();
			}
			
			

			for (int i = 0; i < SUSHIDROP_AD_MAX_CHANNELS; i++)
			{
				analogStatus[i].zero();
			}

			for (int i = 0; i < SUSHIDROP_DA_MAX_CHANNELS; i++)
			{
				analogCommand[i].zero(); 
			}

				
			autoStartStop_command.zero();
			autoStartStop_status.zero();

			device_status.zero();
		}


		IO_sushidrop_status(IO_sushidrop_status&d)
		{
			copy_data(d);
		}


		~IO_sushidrop_status(){}


		IO_sushidrop_status& operator=(const IO_sushidrop_status&d)
		{
			copy_data(d);
			return *this;
		}



};


#endif /* IO_SUSHIDROP_STATUS_H_ */
