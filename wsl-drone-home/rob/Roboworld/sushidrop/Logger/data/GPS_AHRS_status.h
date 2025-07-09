/*
 * GPS_AHRS_status.h
 *
 *  Created on: Apr 07, 2020
 *      Author: mx
 */

#ifndef GPS_AHRS_STATUS_H_
#define GPS_AHRS_STATUS_H_

#include <math.h>
#include "../generic/custom_types.h"
#include "../generic/Variable.h"
#include <vector>
#include <string>



class GPS_AHRS_status
{
	private:

		void copy_data(const GPS_AHRS_status&d)
		{
			roll = d.roll;									pitch = d.pitch;						heading = d.heading;
			xGyro = d.xGyro;								yGyro = d.yGyro;						zGyro = d.zGyro;
			latitude = d.latitude;							longitude = d.longitude;
			heightAboveEllipsoid = d.heightAboveEllipsoid;	heightAboveMSL = d.heightAboveMSL;
			northVel = d.northVel;							eastVel = d.eastVel;					downVel = d.downVel;
			speed = d.speed;								groundSpeed = d.groundSpeed;			course = d.course;
			xAccel = d.xAccel;								yAccel = d.yAccel;						zAccel = d.zAccel;
			lat0 = d.lat0;									lon0 = d.lon0;
			x0 = d.x0;										y0 = d.y0;
			x = d.x;										y = d.y;
			u = d.u;										v = d.v;
			horizontalAccuracy = d.horizontalAccuracy;		verticalAccuracy = d.verticalAccuracy;
			speedAccuracy = d.speedAccuracy;				courseAccuracy = d.courseAccuracy;
			positionValidFlags = d.positionValidFlags;		velocityValidFlags = d.velocityValidFlags;

			gps_status = d.gps_status;
			ahrs_status = d.ahrs_status;
		}


	public:

		DoubleVariable roll,pitch,heading;
		DoubleVariable xGyro, yGyro, zGyro;
		DoubleVariable latitude, longitude;
		DoubleVariable heightAboveEllipsoid, heightAboveMSL;
		DoubleVariable northVel, eastVel, downVel;
		DoubleVariable speed, groundSpeed, course;
		DoubleVariable xAccel, yAccel, zAccel;
		DoubleVariable lat0, lon0, x0, y0, x, y, u, v;
		DoubleVariable horizontalAccuracy, verticalAccuracy, speedAccuracy, courseAccuracy;
		IntVariable positionValidFlags, velocityValidFlags;
		IntVariable gps_status,ahrs_status;
		

		GPS_AHRS_status()
		{
			roll.zero();					pitch.zero();			heading.zero();
			xGyro.zero();					yGyro.zero();			zGyro.zero();
			latitude.zero();				longitude.zero();
			heightAboveEllipsoid.zero();	heightAboveMSL.zero();
			northVel.zero();				eastVel.zero();			downVel.zero();
			speed.zero();					groundSpeed.zero();		course.zero();
			xAccel.zero();					yAccel.zero();			zAccel.zero();
			lat0.zero();					lon0.zero();
			x0.zero();						y0.zero();
			x.zero();						y.zero();
			u.zero();						v.zero();
			horizontalAccuracy.zero();		verticalAccuracy.zero();
			speedAccuracy.zero();			courseAccuracy.zero();
			positionValidFlags.zero();		velocityValidFlags.zero();

			gps_status.zero();
			ahrs_status.zero();
		}


		GPS_AHRS_status(GPS_AHRS_status&d)
		{
			copy_data(d);
		}


		~GPS_AHRS_status(){}


		GPS_AHRS_status& operator=(const GPS_AHRS_status&d)
		{
			copy_data(d);
			return *this;
		}



};


#endif /* GPS_AHRS_STATUS_H_ */
