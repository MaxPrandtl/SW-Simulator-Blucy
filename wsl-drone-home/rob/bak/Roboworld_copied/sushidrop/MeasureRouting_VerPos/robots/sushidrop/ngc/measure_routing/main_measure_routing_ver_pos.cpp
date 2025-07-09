
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
#include "../../../../generic/Variable.h"
#include "../../../../data/Pinger_status.h"
#include "../../../../data/CTD_status.h"
#include "../../../../data/Pinger_status.h"
#include "../../../../data/NGC_status.h"
#include "../../../../data/DataAccess.h"


#define PROCESS_NAME "MEASURE_ROUTING_VER_POS"

#define COMMAND_QOS 0


bool running = true;
struct 	mosquitto* mosq = NULL;

char broker_address[BUF_SIZE];
int broker_port;
int keep_alive_time;
char mqtt_client[BUF_SIZE];

char environment[BUF_SIZE];
char robot[BUF_SIZE];

IntVariable sensor, measure_select;
DataAccess<CTD_status> ctd_access;
DataAccess<Pinger_status> pinger_access;
DataAccess<NGC_status> ngc_access;



void parse_topic_file(char* logic_name, char* topic_name, CTD_status& ctd, Pinger_status& pinger, NGC_status& ngc);
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


	CTD_status ctd;
	Pinger_status pinger;
	NGC_status ngc;


	ctd = ctd_access.get();
	pinger = pinger_access.get();
	ngc = ngc_access.get();

	while (!f.eof())
	{
		getline(f, line);

		if (line.length() != 0 && line[0] != '#')
		{
			sscanf(line.c_str(), "%s %s\n", s1, s2);
			parse_topic_file(s1, s2, ctd, pinger, ngc);
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
	mosquitto_subscribe(mosq, NULL, sensor.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, measure_select.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ctd.depth.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, pinger.z.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ngc.pose.z.filtered.topic_name, 0);

	// End register topic subscriptions



	ctd_access.set(ctd);
	pinger_access.set(pinger);
	ngc_access.set(ngc);

	mosquitto_message_callback_set(mosq, on_message_recv_callback);

	return 0;
}



void parse_topic_file(char* logic_name, char* topic_name, CTD_status& ctd, Pinger_status& pinger, NGC_status& ngc)
{
	if (strcmp(logic_name, "Broker-address:") == 0) strcpy(broker_address, topic_name);
	if (strcmp(logic_name, "Broker-port:") == 0) sscanf(topic_name, "%d", &broker_port);
	if (strcmp(logic_name, "Keep-alive-time:") == 0) sscanf(topic_name, "%d", &keep_alive_time);

	if (strcmp(logic_name, "Environment:") == 0)		strcpy(environment, topic_name);	if (environment[0] == '/') environment[0] = '\0';
	if (strcmp(logic_name, "Robot:") == 0)				strcpy(robot, topic_name);			if (robot[0] == '/') robot[0] = '\0';



	if (strcmp(logic_name, "current-ver-pos-sensor:") == 0)			sprintf(sensor.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-ver-pos-measure:") == 0)		sprintf(measure_select.topic_name, "%s%s%s", environment, robot, topic_name);


	if (strcmp(logic_name, "CTD-depth:") == 0)						sprintf(ctd.depth.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "PINGER-z:") == 0)						sprintf(pinger.z.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-pose-z-raw:") == 0)					sprintf(ngc.pose.z.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-z-filt:") == 0)				sprintf(ngc.pose.z.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-z-act:") == 0)					sprintf(ngc.pose.z.actual.topic_name, "%s%s%s", environment, robot, topic_name);

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

	sensor.value = SENSOR_CTD;
	measure_select.value = MEASURE_RAW;

	publishMessage(sensor);
	publishMessage(measure_select);


	int loop_ret;
	while (running)
	{
		loop_ret = mqtt_loop();
		//if (loop_ret == 0) execute();
	}



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
	//if (dv.updated == 1)
	{
		dv.toString(message);
		mosquitto_publish(mosq, NULL, dv.topic_name, strlen(message), message, 0, false);
		dv.updated = 0;
	}
}


void set_var(DoubleVariable& sourceVar, DoubleVariable& destVar, int mode, bool publish_source = true)
{
	if (publish_source) publishMessage(sourceVar);

	if (measure_select.value == mode)
	{
		destVar.copyValueFrom(sourceVar);
		publishMessage(destVar);
	}
}





void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
	checkMessage(sensor, msg);
	checkMessage(measure_select, msg);

	CTD_status ctd;
	Pinger_status pinger;
	NGC_status ngc;

	ctd = ctd_access.get();
	pinger = pinger_access.get();
	ngc = ngc_access.get();

	NGCvariable* ngc_var = NULL;

	checkMessage(sensor, msg);
	checkMessage(measure_select, msg);


	if (checkMessage(ctd.depth, msg) == true && sensor.value == SENSOR_CTD)
	{
		ngc_var = &(ngc.pose.z);	ngc_var->raw.copyValueFrom(ctd.depth);	ngc_var->raw.value = ngc_var->raw.value-10.13; set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
	}

	
	if (checkMessage(pinger.z, msg) == true && sensor.value == SENSOR_USBL)
	{ ngc_var = &(ngc.pose.z);	ngc_var->raw.copyValueFrom(pinger.z);	set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); }
	


	ngc_var = &(ngc.pose.z); if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	
	ctd_access.set(ctd);
	pinger_access.set(pinger);
	ngc_access.set(ngc);
}






void execute()
{

}