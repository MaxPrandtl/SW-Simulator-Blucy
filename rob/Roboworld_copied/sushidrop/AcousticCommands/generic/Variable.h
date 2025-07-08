/*
 * Data_status.h
 *
 *  Created on: Jan 21, 2019
 *      Author: mx
 */

#ifndef VARIABLE_H_
#define VARIABLE_H_

#include <stdio.h>
#include <string>
#include <cstring>
#include "custom_types.h"
#include "define.h"


class Variable
{
	public:
		char topic_name[BUF_SIZE];
		int updated;
		int valid;
		uint64 timeStamp;

		Variable()
		{
			zero();
		}


		virtual void zero() { updated = 0; valid = 0; timeStamp = 0; strcpy(topic_name, ""); }
		
		virtual void toString(char* s)=0;
		virtual void fromString(char* s, int v = 1, int up = 1) = 0;

		virtual void toString(std::string &s) = 0;
		virtual void fromString(std::string &s, int v = 1, int up = 1) = 0;
};



class IntVariable :public Variable
{
	public:
		int64 value;

		IntVariable()
		{
			zero();
		}


		IntVariable(IntVariable& v)
		{
			(*this).copyVariableFrom(v);
		}


		IntVariable& copyValueFrom(const IntVariable& p)
		{
			value = p.value;
			timeStamp = p.timeStamp;
			valid = p.valid;
			updated = p.updated;
			
			return *this;
		}

		IntVariable& copyVariableFrom(const IntVariable& p)
		{
			value = p.value;
			timeStamp = p.timeStamp;
			valid = p.valid;
			updated = p.updated;
			strcpy(topic_name, p.topic_name);

			return *this;
		}

		IntVariable& operator=(const IntVariable& p)
		{
			return copyVariableFrom(p);
		}


		virtual void zero()
		{
			value = 0;  timeStamp = 0; valid = 0; updated = 0; strcpy(topic_name, "");
		}

		
		virtual void toString(char* s)
		{
			sprintf(s,"%lli %llu %d",value, timeStamp, valid);
		}

		virtual void fromString(char* s, int v=1,int up=1)
		{
			value = 0;  valid = -1;  timeStamp = 0;
			sscanf(s, "%lli %llu %d", &value, &timeStamp, &valid);
			if (valid == -1) valid = v;
			updated = up;
		}
		

		virtual void toString(std::string &s)
		{
			char temp[BUF_SIZE];
			sprintf(temp, "%lli %llu %d", value, timeStamp, valid);
			s = temp;
		}

		virtual void fromString(std::string &s, int v = 1, int up = 1)
		{
			char temp[BUF_SIZE];
			value = 0;  valid = -1;  timeStamp = 0;
			sscanf(temp, "%lli %llu %d", &value, &timeStamp, &valid);
			s = temp;
			if (valid == -1) valid = v;
			updated = up;
		}

};


class DoubleVariable :public Variable
{
	public:
		double value;
		double std;

		DoubleVariable()
		{
			zero();
		}

		DoubleVariable(DoubleVariable& v)
		{
			(*this).copyVariableFrom(v);
		}

		DoubleVariable& copyValueFrom(const DoubleVariable& p)
		{
			value = p.value;
			std = p.std;
			timeStamp = p.timeStamp;
			valid = p.valid;
			updated = p.updated;
			
			return *this;
		}


		DoubleVariable& copyVariableFrom(const DoubleVariable& p)
		{
			value = p.value;
			std = p.std;
			timeStamp = p.timeStamp;
			valid = p.valid;
			updated = p.updated;
			strcpy(topic_name, p.topic_name);

			return *this;
		}

		DoubleVariable& operator=(const DoubleVariable& p)
		{
			return copyVariableFrom(p);
		}

		virtual void zero()
		{
			value = 0.0;  std = 0.0; timeStamp = 0; valid = 0; updated = 0; strcpy(topic_name, "");
		}

		
		virtual void toString(char* s)
		{
			sprintf(s, "%.8lf %llu %d %.8lf", value, timeStamp, valid, std);
		}
		

		virtual void fromString(char* s, int v = 1,int up=1)
		{
			value = 0.0;  std = 0.0;  valid = -1;  timeStamp = 0;
			sscanf(s, "%lf %llu %d %lf", &value, &timeStamp, &valid,&std);
			if (valid == -1) valid = v;
			updated = up;
		}
		

		virtual void toString(std::string &s)
		{
			char temp[BUF_SIZE];
			sprintf(temp, "%.8lf %llu %d %.8lf", value, timeStamp, valid, std);
			s = temp;
		}

		virtual void fromString(std::string& s, int v = 1, int up = 1)
		{
			char temp[BUF_SIZE];
			value = 0.0;  std = 0.0;  valid = -1;  timeStamp = 0;
			sscanf(temp, "%lf %llu %d %lf", &value, &timeStamp, &valid, &std);
			s = temp;
			if (valid == -1) valid = v;
			updated = up;
		}
};



class StringVariable :public Variable
{
public:
	//char value[BUF_SIZE];
	std::string value;

	StringVariable()
	{
		zero();
	}

	StringVariable(StringVariable& v)
	{
		(*this).copyVariableFrom(v);
	}

	StringVariable& copyValueFrom(const StringVariable& p)
	{
		//strcpy(value, p.value);
		value = p.value;
		timeStamp = p.timeStamp;
		valid = p.valid;
		updated = p.updated;

		return *this;
	}


	StringVariable& copyVariableFrom(const StringVariable& p)
	{
		//strcpy(value, p.value);
		value = p.value;
		timeStamp = p.timeStamp;
		valid = p.valid;
		updated = p.updated;
		strcpy(topic_name, p.topic_name);

		return *this;
	}

	StringVariable& operator=(const StringVariable& p)
	{
		return copyVariableFrom(p);
	}

	virtual void zero()
	{
		//strcpy(value, "");
		value = "";
		timeStamp = 0; valid = 0; updated = 0; strcpy(topic_name, "");
	}

	
	virtual void toString(char* s)
	{
		strcpy(s, value.c_str());
	}
	
	virtual void fromString(char* s, int v = 1, int up = 1)
	{
		value=s;
		if (valid == -1) valid = v;
		updated = up;
	}

	virtual void toString(std::string& s)
	{
		s = value;
	}

	virtual void fromString(std::string &s, int v = 1, int up = 1)
	{
		value=s;
		if (valid == -1) valid = v;
		updated = up;
	}
};



#endif /* VARIABLE_H_ */
