
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
#include "../../../../generic/define.h"
#include "../../../../data/DataAccess.h"
#include "../../data/SushidropMotors_status.h"
#include "../../../../data/NGC_status.h"
#include "../../../../data/Time_status.h"
#include "../../../../data/Task_execution.h"
#include "ThrustMapping.h"


#define PROCESS_NAME "THRUST_MAPPING"

#define COMMAND_QOS 0


bool running = true;
struct 	mosquitto* mosq = NULL;

char broker_address[BUF_SIZE];
int broker_port;
int keep_alive_time;
char mqtt_client[BUF_SIZE];

char environment[BUF_SIZE];
char robot[BUF_SIZE];



DataAccess<SushidropMotors_status> motors_access;
DataAccess<NGC_status> ngc_access;
DataAccess<Time_status> time_access;
DataAccess<Task_execution> task_access;


ThrustMapping* thrust_mapping = NULL;

void parse_topic_file(char* logic_name, char* topic_name, SushidropMotors_status& motors, NGC_status& ngc, Time_status& clock);
void execute();
void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg);

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

	SushidropMotors_status motors;
	motors = motors_access.get();
	
	NGC_status ngc;
	ngc = ngc_access.get();

	Time_status clock;
	clock = time_access.get();

	while (!f.eof())
	{
		getline(f, line);

		if (line.length() != 0 && line[0] != '#')
		{
			sscanf(line.c_str(), "%s %s\n", s1, s2);
			parse_topic_file(s1, s2, motors, ngc, clock);
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


	// subscript to simulation topics
	mosquitto_subscribe(mosq, NULL, ngc.force.fu.reference.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.force.fv.reference.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.force.fw.reference.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.force.tr.reference.topic_name, COMMAND_QOS);

	

	motors_access.set(motors);
	ngc_access.set(ngc);
	time_access.set(clock);
	// End register topic subscriptions

	mosquitto_message_callback_set(mosq, on_message_recv_callback);

	return 0;
}



void parse_topic_file(char* logic_name, char* topic_name, SushidropMotors_status& motors, NGC_status& ngc, Time_status& clock)
{
	if (strcmp(logic_name, "Broker-address:") == 0) strcpy(broker_address, topic_name);
	if (strcmp(logic_name, "Broker-port:") == 0) sscanf(topic_name, "%d", &broker_port);
	if (strcmp(logic_name, "Keep-alive-time:") == 0) sscanf(topic_name, "%d", &keep_alive_time);

	if (strcmp(logic_name, "Environment:") == 0)		strcpy(environment, topic_name);	if (environment[0] == '/') environment[0] = '\0';
	if (strcmp(logic_name, "Robot:") == 0)				strcpy(robot, topic_name);			if (robot[0] == '/') robot[0] = '\0';

	
	if (strcmp(logic_name, "timeStamp:") == 0)					sprintf(clock.timeStamp.topic_name, "%s%s%s", environment, robot, topic_name);


	if (strcmp(logic_name, "ngc-force-fu-ref:") == 0)			sprintf(ngc.force.fu.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fu-act:") == 0)			sprintf(ngc.force.fu.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fv-ref:") == 0)			sprintf(ngc.force.fv.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fv-act:") == 0)			sprintf(ngc.force.fv.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fw-ref:") == 0)			sprintf(ngc.force.fw.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fw-act:") == 0)			sprintf(ngc.force.fw.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-tr-ref:") == 0)			sprintf(ngc.force.tr.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-tr-act:") == 0)			sprintf(ngc.force.tr.actual.topic_name, "%s%s%s", environment, robot, topic_name);


	if (strcmp(logic_name, "thruster-HL-ref:") == 0)		sprintf(motors.hl.velocity_reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "thruster-HR-ref:") == 0)		sprintf(motors.hr.velocity_reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "thruster-VF-ref:") == 0)		sprintf(motors.vf.velocity_reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "thruster-VR-ref:") == 0)		sprintf(motors.vr.velocity_reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "thruster-BOW-ref:") == 0)		sprintf(motors.bow.velocity_reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "thruster-STERN-ref:") == 0)		sprintf(motors.stern.velocity_reference.topic_name, "%s%s%s", environment, robot, topic_name);
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

	
	thrust_mapping = new ThrustMapping(PROCESS_NAME, &motors_access, &ngc_access,&time_access,&task_access);

	

	if (init_mqtt() != 0)
	{
		printf("Failed mqtt init.\n");
		return -1;
	}


	thrust_mapping->create();


	int loop_ret;
	while (running)
	{
		loop_ret = mqtt_loop();
		if (loop_ret == 0) execute();
	}

	thrust_mapping->terminate();

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}


void checkMessage(Variable& dv, const struct mosquitto_message* msg)
{
	bool match = false;
	mosquitto_topic_matches_sub(dv.topic_name, msg->topic, &match);
	if (match) { dv.fromString((char*)(msg->payload)); }
}


void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
	Time_status clock;
	clock = time_access.get();

	NGC_status ngc;
	ngc = ngc_access.get();

	

	checkMessage(clock.timeStamp,msg);

	checkMessage(ngc.force.fu.reference, msg);
	checkMessage(ngc.force.fv.reference, msg);
	checkMessage(ngc.force.fw.reference, msg);
	checkMessage(ngc.force.tr.reference, msg);

	

	time_access.set(clock);
	ngc_access.set(ngc);
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


void execute()
{
	char message[BUF_SIZE];

	SushidropMotors_status motors;
	motors = motors_access.get();

	NGC_status ngc;
	ngc = ngc_access.get();

	
	publishMessage(motors.hl.velocity_reference);
	publishMessage(motors.hr.velocity_reference);
	publishMessage(motors.vf.velocity_reference);
	publishMessage(motors.vr.velocity_reference);
	publishMessage(motors.bow.velocity_reference);
	publishMessage(motors.stern.velocity_reference);

	publishMessage(ngc.force.fu.actual);
	publishMessage(ngc.force.fv.actual);
	publishMessage(ngc.force.fw.actual);
	publishMessage(ngc.force.tr.actual);

	
	motors_access.set(motors);
	ngc_access.set(ngc);
}