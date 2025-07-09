/*
 * NGC_status.h
 *
 *  Created on: Apr 20, 2020
 *      Author: mx
 */

#ifndef NGC_STATUS_H_
#define NGC_STATUS_H_

#include <math.h>
#include "../generic/custom_types.h"
#include "../generic/Variable.h"
#include <vector>
#include <string>


class NGCvariable
{
private:
	void copy_data(const NGCvariable& d)
	{
		actual = d.actual;
		reference = d.reference;
		manual = d.manual;
		automatic = d.automatic;
		external = d.external;
		raw = d.raw;
		filtered = d.filtered;
	}

public:
	DoubleVariable actual, reference, manual, automatic, external, raw, filtered;
	
	NGCvariable() { zero(); }

	void zero()
	{
		actual.zero();
		reference.zero();
		manual.zero();
		automatic.zero();
		external.zero();
		raw.zero();
		filtered.zero();
	}

	NGCvariable& operator=(const NGCvariable& d)
	{
		copy_data(d);
		return *this;
	}
};


class Pose
{
	private:
		void copy_data(const Pose& d)
		{
			x = d.x;		y = d.y;			z = d.z;		a = d.a;
			phi = d.phi;	theta = d.theta;	psi = d.psi;
			lat = d.lat;	lon = d.lon;
		}

	public:
		NGCvariable x, y, z, a;
		NGCvariable phi, theta, psi;
		NGCvariable lat, lon;

		Pose() { zero(); }

		void zero()
		{
			x.zero();		y.zero();		z.zero();	a.zero();
			phi.zero();		theta.zero();	psi.zero();
			lat.zero();		lon.zero();
		}


		Pose& operator=(const Pose& d)
		{
			copy_data(d);
			return *this;
		}
};

class VelocityBody
{
private:
	void copy_data(const VelocityBody& d)
	{
		u = d.u;	v = d.v;	w = d.w;
		p = d.p;	q = d.q;	r = d.r;

		uSeaCurrent = d.uSeaCurrent;		vSeaCurrent = d.vSeaCurrent;
	}

public:
	NGCvariable u, v, w;
	NGCvariable p, q, r;
	NGCvariable uSeaCurrent, vSeaCurrent;

	VelocityBody() { zero(); }

	void zero()
	{
		u.zero();		v.zero();		w.zero();
		p.zero();		q.zero();		r.zero();

		uSeaCurrent.zero();
		vSeaCurrent.zero();
	}


	VelocityBody& operator=(const VelocityBody& d)
	{
		copy_data(d);
		return *this;
	}
};

class VelocityInertial
{
private:
	void copy_data(const VelocityInertial& d)
	{
		xDot = d.xDot;		yDot = d.yDot;		zDot = d.zDot;
		speed = d.speed;	course = d.course;
		xDotSeaCurrent = d.xDotSeaCurrent;	yDotSeaCurrent = d.yDotSeaCurrent;
	}

public:
	NGCvariable xDot, yDot, zDot;
	NGCvariable speed, course;
	NGCvariable xDotSeaCurrent, yDotSeaCurrent;

	VelocityInertial() { zero(); }

	void zero()
	{
		xDot.zero();		yDot.zero();		zDot.zero();
		speed.zero();		course.zero();
		xDotSeaCurrent.zero(); yDotSeaCurrent.zero();
	}


	VelocityInertial& operator=(const VelocityInertial& d)
	{
		copy_data(d);
		return *this;
	}
};

class Force
{
private:
	void copy_data(const Force& d)
	{
		fu = d.fu;		fv = d.fv;		fw = d.fw;
		tp = d.tp;		tq = d.tq;		tr = d.tr;
	}

public:
	NGCvariable fu, fv, fw;
	NGCvariable tp, tq, tr;

	Force() { zero(); }

	void zero()
	{
		fu.zero();		fv.zero();		fw.zero();
		tp.zero();		tq.zero();		tr.zero();
	}


	Force& operator=(const Force& d)
	{
		copy_data(d);
		return *this;
	}
};


class NGC_status
{
	private:

		void copy_data(const NGC_status&d)
		{
			pose = d.pose;
			velocityBody = d.velocityBody;
			velocityInertial = d.velocityInertial;
			force = d.force;
		}


	public:

		Pose pose;
		VelocityBody velocityBody;
		VelocityInertial velocityInertial;
		Force force;

		NGC_status()
		{
			pose.zero();
			velocityBody.zero();
			velocityInertial.zero();
			force.zero();
		}


		NGC_status(NGC_status&d)
		{
			copy_data(d);
		}


		~NGC_status(){}


		NGC_status& operator=(const NGC_status&d)
		{
			copy_data(d);
			return *this;
		}
};


#endif /* NGC_STATUS_H_ */
