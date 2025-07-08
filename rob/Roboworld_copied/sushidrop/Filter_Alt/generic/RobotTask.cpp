/*
 * RobotTask.h
 *
 *  Created on: Apr 22, 2020
 *      Author: mx
 */


#include "RobotTask.h"


RobotTask::RobotTask(const char* name, const char* Params_file, int policy, int priority, void* (*threadRoutine)(void*)) :RobotThread(name, policy, priority, threadRoutine)
{
	if (Params_file!=NULL) strcpy(PARAMS_FILENAME, Params_file);
	
}


RobotTask::~RobotTask()
{

}


int RobotTask::read_config()
{
	FILE* f;
	char name_dir_file[256];
	char value[100], value2[100];
	double val;

	sprintf(name_dir_file, "%s%s", CONFIGURATION_FILES_DIRECTORY, PARAMS_FILENAME);

	if ((f = fopen(name_dir_file, "r")) != NULL)
	{
		while (!feof(f))
		{
			fscanf(f, "%s %s %lf\n", value, value2, &val);

			parse_config(value, val);
		}

		fclose(f);

		return 0;
	}

	else
	{
		printf("%s read_config - File error: %s\n", threadName, name_dir_file);
		return -1;
	}
}



int RobotTask::write_config()
{
	FILE* f;
	char name_dir_file[256];

	sprintf(name_dir_file, "%s%s", CONFIGURATION_FILES_DIRECTORY, PARAMS_FILENAME);

	if ((f = fopen(name_dir_file, "w")) != NULL)
	{
		set_config(f);
		


		fclose(f);

		return 0;
	}

	else
	{
		printf("%s read_config - File error: %s\n", threadName, name_dir_file);
		return -1;
	}
}
