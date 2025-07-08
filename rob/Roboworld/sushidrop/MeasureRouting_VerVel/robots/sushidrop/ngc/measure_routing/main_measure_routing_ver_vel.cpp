
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
#include "../../../../data/CTD_status.h"
#include "../../../../data/DVL_status.h"
#include "../../../../data/PA500_status.h"
#include "../../../../data/Pinger_status.h"
#include "../../../../data/NGC_status.h"
#include "../../../../data/DataAccess.h"


#define PROCESS_NAME "MEASURE_ROUTING_VER_VEL"

#define COMMAND_QOS 0


bool running = true;
struct 	mosquitto* mosq = NULL;

char broker_address[BUF_SIZE];
int broker_port;
int keep_alive_time;
char mqtt_client[BUF_SIZE];

char environment[BUF_SIZE];
char robot[BUF_SIZE];

IntVariable sensor,measure_select;
DataAccess<CTD_status> ctd_access;
DataAccess<DVL_status> dvl_access;
DataAccess<PA500_status> pa500_access;
DataAccess<Pinger_status> pinger_access;
DataAccess<NGC_status> ngc_access;

double zp=0.0;
uint64 tp = 0;


void parse_topic_file(char* logic_name, char* topic_name, CTD_status& ctd, DVL_status& dvl, PA500_status& pa500, Pinger_status& pinger, NGC_status& ngc);
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
	DVL_status dvl;
	PA500_status pa500;
	Pinger_status pinger;
	NGC_status ngc;

	
	ctd = ctd_access.get();
	dvl = dvl_access.get();
	pa500 = pa500_access.get();
	pinger = pinger_access.get();
	ngc = ngc_access.get();

	while (!f.eof())
	{
		getline(f, line);

		if (line.length() != 0 && line[0] != '#')
		{
			sscanf(line.c_str(), "%s %s\n", s1, s2);
			parse_topic_file(s1, s2, ctd,dvl,pa500,pinger,ngc);
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
	mosquitto_subscribe(mosq, NULL, dvl.heave.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, pa500.altitude.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, pinger.z.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ngc.pose.phi.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.pose.theta.actual.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.w.filtered.topic_name, 0);	
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.zDot.filtered.topic_name, 0);
	
	
	// End register topic subscriptions


	
	ctd_access.set(ctd);
	dvl_access.set(dvl);
	pa500_access.set(pa500);
	pinger_access.set(pinger);
	ngc_access.set(ngc);

	mosquitto_message_callback_set(mosq, on_message_recv_callback);

	return 0;
}



void parse_topic_file(char* logic_name, char* topic_name, CTD_status& ctd, DVL_status& dvl, PA500_status& pa500, Pinger_status& pinger, NGC_status& ngc)
{
	if (strcmp(logic_name, "Broker-address:") == 0) strcpy(broker_address, topic_name);
	if (strcmp(logic_name, "Broker-port:") == 0) sscanf(topic_name, "%d", &broker_port);
	if (strcmp(logic_name, "Keep-alive-time:") == 0) sscanf(topic_name, "%d", &keep_alive_time);

	if (strcmp(logic_name, "Environment:") == 0)		strcpy(environment, topic_name);	if (environment[0] == '/') environment[0] = '\0';
	if (strcmp(logic_name, "Robot:") == 0)				strcpy(robot, topic_name);			if (robot[0] == '/') robot[0] = '\0';

	

	if (strcmp(logic_name, "current-ver-vel-sensor:") == 0)				sprintf(sensor.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-ver-vel-measure:") == 0)			sprintf(measure_select.topic_name, "%s%s%s", environment, robot, topic_name);

	
	if (strcmp(logic_name, "CTD-depth:") == 0)							sprintf(ctd.depth.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "DVL-heave:") == 0)							sprintf(dvl.heave.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "PA500-altitude:") == 0)						sprintf(pa500.altitude.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "PINGER-z:") == 0)							sprintf(pinger.z.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-velocity-inertial-zDot-raw:") == 0)		sprintf(ngc.velocityInertial.zDot.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-zDot-filt:") == 0)	sprintf(ngc.velocityInertial.zDot.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-zDot-act:") == 0)		sprintf(ngc.velocityInertial.zDot.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	
	if (strcmp(logic_name, "ngc-velocity-body-w-raw:") == 0)			sprintf(ngc.velocityBody.w.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-w-filt:") == 0)			sprintf(ngc.velocityBody.w.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-w-act:") == 0)			sprintf(ngc.velocityBody.w.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	

	if (strcmp(logic_name, "ngc-pose-phi-act:") == 0)					sprintf(ngc.pose.phi.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-theta-act:") == 0)					sprintf(ngc.pose.theta.actual.topic_name, "%s%s%s", environment, robot, topic_name);
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


void set_var(DoubleVariable& sourceVar, DoubleVariable& destVar, int mode,bool publish_source=true)
{
	if (publish_source) publishMessage(sourceVar);

	if (measure_select.value == mode)
	{
		destVar.copyValueFrom(sourceVar);
		publishMessage(destVar);
	}
}



void body_to_inertial(NGC_status& ngc)
{
	double phi = ngc.pose.phi.actual.value;
	double theta = ngc.pose.phi.actual.value;
	double w = ngc.velocityBody.w.raw.value;
	double zDot;

	zDot =  w * (cos(phi) * cos(theta));

	ngc.velocityInertial.zDot.raw.copyValueFrom(ngc.velocityBody.w.raw);			ngc.velocityInertial.zDot.raw.value = zDot;
		

	NGCvariable* ngc_var = NULL;
	ngc_var = &(ngc.velocityInertial.zDot);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
}


void position_to_velocity(NGC_status& ngc,DoubleVariable& ver, bool depth=true)
{
	double z;
	bool flag = false;

	if (ver.valid == 1)
	{
		if (tp > 0 && tp < ver.timeStamp)
		{
			flag = true;
			double dt = ((double)(ver.timeStamp - tp)) / 1000.0;

			z = ver.value;
			if (!depth) z = -z;

			double zDot, w;
			double phi = ngc.pose.phi.actual.value;
			double theta = ngc.pose.theta.actual.value;

			zDot = (z - zp) / dt;
			w = zDot / (cos(phi) * cos(theta));


			ngc.velocityBody.w.raw.copyValueFrom(ver);				ngc.velocityBody.w.raw.value = w;
			ngc.velocityInertial.zDot.raw.copyValueFrom(ver);		ngc.velocityInertial.zDot.raw.value = zDot;


			NGCvariable* ngc_var = NULL;
			ngc_var = &(ngc.velocityBody.w);			set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
			ngc_var = &(ngc.velocityInertial.zDot);		set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		}

		tp = ver.timeStamp;
		zp = ver.value;

		if (!depth) zp = -zp;

	}
}


void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
	checkMessage(sensor,msg);
	checkMessage(measure_select, msg);

	CTD_status ctd;
	DVL_status dvl;
	PA500_status pa500;
	Pinger_status pinger;
	NGC_status ngc;

	ctd = ctd_access.get();
	dvl = dvl_access.get();
	pa500 = pa500_access.get();
	pinger = pinger_access.get();
	ngc = ngc_access.get();

	NGCvariable *ngc_var=NULL;

	checkMessage(sensor, msg);
	checkMessage(measure_select, msg);

	checkMessage(ngc.pose.phi.actual, msg);
	checkMessage(ngc.pose.theta.actual, msg);


	if (checkMessage(ctd.depth, msg) == true && sensor.value == SENSOR_CTD)
	{ position_to_velocity(ngc, ctd.depth); }
	
	if (checkMessage(dvl.heave, msg) == true && sensor.value == SENSOR_DVL)
	{ ngc_var = &(ngc.velocityBody.w);	ngc_var->raw.copyValueFrom(dvl.heave);	set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); body_to_inertial(ngc); }

	if (checkMessage(pa500.altitude, msg) == true && sensor.value == SENSOR_PA500)
	{ position_to_velocity(ngc, pa500.altitude,false); }


	if (checkMessage(pinger.z, msg) == true && sensor.value == SENSOR_USBL)
	{ position_to_velocity(ngc,pinger.z); }
	
	

	ngc_var = &(ngc.velocityInertial.zDot); if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER,false); }
	ngc_var = &(ngc.velocityBody.w); if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	
	ctd_access.set(ctd);
	dvl_access.set(dvl);
	pa500_access.set(pa500);
	pinger_access.set(pinger);
	ngc_access.set(ngc);
}






void execute()
{

}