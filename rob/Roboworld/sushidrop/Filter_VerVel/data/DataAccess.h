/*
 * Data_status.h
 *
 *  Created on: Jan 21, 2019
 *      Author: mx
 */

#ifndef DATA_ACCESS_H_
#define DATA_ACCESS_H_

#include <semaphore.h>



template <class T>
class DataAccess
{
	protected:

		T data;

		sem_t dataLock;


	public:
		DataAccess(){ sem_init(&dataLock,0,1); }
		~DataAccess(){}

		void set(T &d){ lockData(); data=d; unlockData(); }
		T get(){T d; lockData(); d=data; unlockData(); return d; }

		void lockData() { sem_wait(&dataLock); }
		void unlockData() { sem_post(&dataLock); }

		void set_no_lock(T& d) { data = d;  }
		T get_no_lock() { T d; d = data; return d; }

		void set_and_unlock(T& d) { data = d; unlockData(); }
		T get_and_lock() { T d; lockData(); d = data; return d; }
};


#endif /* DATA_ACCESS_H_ */
