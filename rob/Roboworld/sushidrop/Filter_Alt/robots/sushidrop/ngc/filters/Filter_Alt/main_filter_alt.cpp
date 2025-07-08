
#define __MAIN__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include "mosquitto.h"
#include "../../../../../generic/Variable.h"
#include "../../../../../data/DataAccess.h"
#include "../../../../../data/NGC_status.h"
#include "../../../../../data/Time_status.h"
#include "../../../../../data/Task_execution.h"
#include "AltFilter.h"

#define PROCESS_NAME "FILTER_ALT"

#define COMMAND_QOS 0


bool running = true;
struct 	mosquitto* mosq = NULL;

char broker_address[BUF_SIZE];
int broker_port;
int keep_alive_time;
char mqtt_client[BUF_SIZE];

char environment[BUF_SIZE];
char robot[BUF_SIZE];

DataAccess<NGC_status> ngc_access;
DataAccess<Time_status> time_access;
DataAccess<Task_execution> task_access;
DataAccess<AltFilter_params> params_access;

Filter* alt_filter = NULL;


void parse_topic_file(char* logic_name, char* topic_name, NGC_status &ngc, Time_status &clock, Task_execution& task, AltFilter_params& params);
void execute();
void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg);
void publishMessage(Variable& dv);

void ctrl_c_handler(int dummy)
{
	running = false;
}


int init_mqtt()
{
	char s1[BUF_SIZE], s2[BUF_SIZE];

	char topics_file[BUF_SIZE];
	sprintf(topics_file, "%s%s", CONFIGURATION_FILES_DIRECTORY, CONFIGURATION_TOPICS_FILE);

	// read config file
	std::ifstream f(topics_file, std::ifstream::in);
	std::string line;

	if (f.fail()) return -1;


	NGC_status ngc;
	ngc = ngc_access.get();

	Time_status clock;
	clock = time_access.get();
	
	Task_execution task;
	task = task_access.get();

	AltFilter_params params;
	params = params_access.get();

	while (!f.eof())
	{
		getline(f, line);

		if (line.length() != 0 && line[0] != '#')
		{
			sscanf(line.c_str(), "%s %s\n", s1, s2);
			parse_topic_file(s1, s2, ngc,clock,task,params);
		}
	}

	// end read config file

	sprintf(mqtt_client, "%s_%s_%s", environment, robot, PROCESS_NAME);

	// Open MQTT connection
	mosquitto_lib_init();
	mosq = mosquitto_new(mqtt_client, true, NULL);

	int conn_ret = mosquitto_connect(mosq, broker_address, broker_port, keep_alive_time);
	if (conn_ret != MOSQ_ERR_SUCCESS)
	{
		printf("%s - connection error on: %s:%d\n", mqtt_client, broker_address, broker_port);
		return(-1);
	}
	// End open MQTT connection



	// Register topic subscriptions
	
	mosquitto_subscribe(mosq, NULL, clock.timeStamp.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, clock.dt.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, task.command.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, params.reference.q_a.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, params.reference.r_a.topic_name, 0);
	
	
	mosquitto_subscribe(mosq, NULL, ngc.pose.a.raw.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.zDot.actual.topic_name, 0);

	
	// End register topic subscriptions

	ngc_access.set(ngc);
	time_access.set(clock);
	task_access.set(task);
	params_access.set(params);

	mosquitto_message_callback_set(mosq, on_message_recv_callback);

	return 0;
}



void parse_topic_file(char* logic_name, char* topic_name, NGC_status& ngc, Time_status& clock, Task_execution& task, AltFilter_params& params)
{
	if (strcmp(logic_name, "Broker-address:") == 0) strcpy(broker_address, topic_name);
	if (strcmp(logic_name, "Broker-port:") == 0) sscanf(topic_name, "%d", &broker_port);
	if (strcmp(logic_name, "Keep-alive-time:") == 0) sscanf(topic_name, "%d", &keep_alive_time);

	if (strcmp(logic_name, "Environment:") == 0)		strcpy(environment, topic_name);	if (environment[0] == '/') environment[0] = '\0';
	if (strcmp(logic_name, "Robot:") == 0)				strcpy(robot, topic_name);			if (robot[0] == '/') robot[0] = '\0';

	
	if (strcmp(logic_name, "timeStamp:") == 0)							sprintf(clock.timeStamp.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "dt:") == 0)									sprintf(clock.dt.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "filter-alt-execution-command:") == 0)		sprintf(task.command.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "filter-alt-execution-status:") == 0)		sprintf(task.status.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "filter-alt-q-a-ref:") == 0)					sprintf(params.reference.q_a.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "filter-alt-r-a-ref:") == 0)					sprintf(params.reference.r_a.topic_name, "%s%s%s", environment, robot, topic_name);
	
	if (strcmp(logic_name, "filter-alt-q-a-act:") == 0)					sprintf(params.actual.q_a.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "filter-alt-r-a-act:") == 0)					sprintf(params.actual.r_a.topic_name, "%s%s%s", environment, robot, topic_name);
	

	if (strcmp(logic_name, "ngc-pose-a-raw:") == 0)						sprintf(ngc.pose.a.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-a-filt:") == 0)					sprintf(ngc.pose.a.filtered.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-velocity-inertial-zDot-act:") == 0)		sprintf(ngc.velocityInertial.zDot.actual.topic_name, "%s%s%s", environment, robot, topic_name);
} 



int mqtt_loop()
{
	int loop_ret;
	loop_ret = mosquitto_loop(mosq, MAX_MQTT_LOOP_WAIT, 1);
	if (loop_ret != 0) loop_ret = mosquitto_reconnect(mosq);
	return loop_ret;
}


int main(int argc, char* argv[])
{
	printf("%s starting\n", PROCESS_NAME);

	if (argc != 2)
	{
		printf("Wrong parameter number.\n");
		return -1;
	}

	strcpy(CONFIGURATION_FILES_DIRECTORY, argv[1]);

	strcpy(CONFIGURATION_SIM_FILES_DIRECTORY, argv[1]);
	strcat(CONFIGURATION_SIM_FILES_DIRECTORY, "sim/");

	signal(SIGINT, ctrl_c_handler);



	if (init_mqtt() != 0)
	{
		printf("Failed mqtt init.\n");
		return -1;
	}


	alt_filter = new AltFilter(PROCESS_NAME, &ngc_access, &time_access, &task_access,&params_access);
	alt_filter->create();

	
	int loop_ret;
	while (running)
	{
		loop_ret = mqtt_loop();
		if (loop_ret == 0) execute();
	}

	alt_filter->terminate();

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}


bool checkMessage(Variable& dv, const struct mosquitto_message* msg)
{
	bool match = false;
	mosquitto_topic_matches_sub(dv.topic_name, msg->topic, &match);
	if (match) { dv.fromString((char*)(msg->payload)); }
	return match;
}



void publishMessage(Variable& dv)
{
	char message[BUF_SIZE];
	if (dv.updated == 1)
	{
		dv.toString(message);
		mosquitto_publish(mosq, NULL, dv.topic_name, strlen(message), message, 0, false);
		dv.updated = 0;
	}
}





void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
	NGC_status ngc;
	ngc = ngc_access.get();

	Time_status clock;
	clock = time_access.get();

	Task_execution task;
	task = task_access.get();

	AltFilter_params params;
	params = params_access.get();

	
	checkMessage(clock.timeStamp, msg);			checkMessage(clock.dt, msg);

	checkMessage(task.command, msg);

	checkMessage(params.reference.q_a, msg);		checkMessage(params.reference.r_a, msg);
	
	checkMessage(ngc.pose.a.raw, msg);
	checkMessage(ngc.velocityInertial.zDot.actual, msg);
	
	ngc_access.set(ngc);
	time_access.set(clock);
	task_access.set(task);
	params_access.set(params);
}




void execute()
{
	Task_execution task;
	task = task_access.get();

	AltFilter_params params;
	params = params_access.get();

	NGC_status ngc;
	ngc = ngc_access.get();

	publishMessage(task.status);

	
	publishMessage(params.actual.q_a);
	publishMessage(params.actual.r_a);

	
	
	publishMessage(ngc.pose.a.filtered);					
	

	ngc_access.set(ngc);
	task_access.set(task);
	params_access.set(params);
}