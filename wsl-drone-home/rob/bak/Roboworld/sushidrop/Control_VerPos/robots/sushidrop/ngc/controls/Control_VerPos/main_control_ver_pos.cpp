
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
#include "VerPosControl.h"

#define PROCESS_NAME "CONTROL_VER_POS"

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
DataAccess<VerPosControl_params> params_access;

Control* ver_pos_control = NULL;


void parse_topic_file(char* logic_name, char* topic_name, NGC_status &ngc, Time_status &clock, Task_execution& task, VerPosControl_params& params);
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

	VerPosControl_params params;
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

	mosquitto_subscribe(mosq, NULL, params.vel_control.status.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, params.reference.kp_vel.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, params.reference.ki_vel.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, params.reference.kd_vel.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, params.reference.kp_force.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, params.reference.ki_force.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, params.reference.kd_force.topic_name, 0);
	
	mosquitto_subscribe(mosq, NULL, ngc.pose.z.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.pose.z.reference.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.w.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.w.reference.topic_name, 0);

	
	
	
	// End register topic subscriptions


	ngc_access.set(ngc);
	time_access.set(clock);
	task_access.set(task);
	params_access.set(params);

	mosquitto_message_callback_set(mosq, on_message_recv_callback);

	return 0;
}



void parse_topic_file(char* logic_name, char* topic_name, NGC_status& ngc, Time_status& clock, Task_execution& task, VerPosControl_params& params)
{
	if (strcmp(logic_name, "Broker-address:") == 0) strcpy(broker_address, topic_name);
	if (strcmp(logic_name, "Broker-port:") == 0) sscanf(topic_name, "%d", &broker_port);
	if (strcmp(logic_name, "Keep-alive-time:") == 0) sscanf(topic_name, "%d", &keep_alive_time);

	if (strcmp(logic_name, "Environment:") == 0)		strcpy(environment, topic_name);	if (environment[0] == '/') environment[0] = '\0';
	if (strcmp(logic_name, "Robot:") == 0)				strcpy(robot, topic_name);			if (robot[0] == '/') robot[0] = '\0';


	if (strcmp(logic_name, "timeStamp:") == 0)							sprintf(clock.timeStamp.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "dt:") == 0)									sprintf(clock.dt.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "control-ver-pos-execution-command:") == 0)	sprintf(task.command.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-ver-pos-execution-status:") == 0)	sprintf(task.status.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "control-ver-vel-execution-status:") == 0)	sprintf(params.vel_control.status.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "control-ver-pos-Kp-vel-ref:") == 0)			sprintf(params.reference.kp_vel.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-ver-pos-Ki-vel-ref:") == 0)			sprintf(params.reference.ki_vel.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-ver-pos-Kd-vel-ref:") == 0)			sprintf(params.reference.kd_vel.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "control-ver-pos-Kp-vel-act:") == 0)			sprintf(params.actual.kp_vel.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-ver-pos-Ki-vel-act:") == 0)			sprintf(params.actual.ki_vel.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-ver-pos-Kd-vel-act:") == 0)			sprintf(params.actual.kd_vel.topic_name, "%s%s%s", environment, robot, topic_name);
	
	if (strcmp(logic_name, "control-ver-pos-Kp-force-ref:") == 0)		sprintf(params.reference.kp_force.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-ver-pos-Ki-force-ref:") == 0)		sprintf(params.reference.ki_force.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-ver-pos-Kd-force-ref:") == 0)		sprintf(params.reference.kd_force.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "control-ver-pos-Kp-force-act:") == 0)		sprintf(params.actual.kp_force.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-ver-pos-Ki-force-act:") == 0)		sprintf(params.actual.ki_force.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-ver-pos-Kd-force-act:") == 0)		sprintf(params.actual.kd_force.topic_name, "%s%s%s", environment, robot, topic_name);
	
	if (strcmp(logic_name, "ngc-pose-z-act:") == 0)						sprintf(ngc.pose.z.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-z-ref:") == 0)						sprintf(ngc.pose.z.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-w-act:") == 0)			sprintf(ngc.velocityBody.w.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-w-auto:") == 0)			sprintf(ngc.velocityBody.w.automatic.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-w-ref:") == 0)			sprintf(ngc.velocityBody.w.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fw-auto:") == 0)					sprintf(ngc.force.fw.automatic.topic_name, "%s%s%s", environment, robot, topic_name);
	
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


	ver_pos_control = new VerPosControl(PROCESS_NAME, &ngc_access, &time_access, &task_access,&params_access);
	ver_pos_control->create();

	
	int loop_ret;
	while (running)
	{
		loop_ret = mqtt_loop();
		if (loop_ret == 0) execute();
	}

	ver_pos_control->terminate();

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

	VerPosControl_params params;
	params = params_access.get();


	checkMessage(clock.timeStamp, msg);			checkMessage(clock.dt, msg);

	checkMessage(task.command, msg);

	checkMessage(params.vel_control.status, msg);

	checkMessage(params.reference.kp_vel, msg);
	checkMessage(params.reference.ki_vel, msg);
	checkMessage(params.reference.kd_vel, msg);

	checkMessage(params.reference.kp_force, msg);
	checkMessage(params.reference.ki_force, msg);
	checkMessage(params.reference.kd_force, msg);
	
	checkMessage(ngc.velocityBody.w.actual, msg);
	checkMessage(ngc.velocityBody.w.reference, msg);

	checkMessage(ngc.pose.z.actual, msg);
	checkMessage(ngc.pose.z.reference, msg);
	
	ngc_access.set(ngc);
	time_access.set(clock);
	task_access.set(task);
	params_access.set(params);
}




void execute()
{
	Task_execution task;
	task = task_access.get();

	VerPosControl_params params;
	params = params_access.get();

	NGC_status ngc;
	ngc = ngc_access.get();

	publishMessage(task.status);
	publishMessage(params.actual.kp_vel);
	publishMessage(params.actual.ki_vel);
	publishMessage(params.actual.kd_vel);
	
	publishMessage(params.actual.kp_force);
	publishMessage(params.actual.ki_force);
	publishMessage(params.actual.kd_force);
	
	publishMessage(ngc.velocityBody.w.automatic);
	publishMessage(ngc.force.fw.automatic);

	ngc_access.set(ngc);
	task_access.set(task);
	params_access.set(params);
}