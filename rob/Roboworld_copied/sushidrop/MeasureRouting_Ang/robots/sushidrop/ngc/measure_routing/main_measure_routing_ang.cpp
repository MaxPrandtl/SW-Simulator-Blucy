
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
#include "../../../../generic/RobotMath.h"
#include "../../../../data/FOG_status.h"
#include "../../../../data/GPS_AHRS_status.h"
#include "../../../../data/NGC_status.h"
#include "../../../../data/DataAccess.h"


#define PROCESS_NAME "MEASURE_ROUTING_ANG"

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
DataAccess<FOG_status> fog_access;
DataAccess<GPS_AHRS_status> gps_ahrs_access;
DataAccess<NGC_status> ngc_access;


void parse_topic_file(char* logic_name, char* topic_name, FOG_status& fog, GPS_AHRS_status& gps_ahrs,NGC_status& ngc);
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


	FOG_status fog;
	GPS_AHRS_status gps_ahrs;
	NGC_status ngc;

	fog = fog_access.get();
	gps_ahrs = gps_ahrs_access.get();
	ngc = ngc_access.get();

	while (!f.eof())
	{
		getline(f, line);

		if (line.length() != 0 && line[0] != '#')
		{
			sscanf(line.c_str(), "%s %s\n", s1, s2);
			parse_topic_file(s1, s2, fog, gps_ahrs,ngc);
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
	
	mosquitto_subscribe(mosq, NULL, fog.roll.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fog.pitch.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fog.heading.topic_name, 0);
	
	
	mosquitto_subscribe(mosq, NULL, gps_ahrs.roll.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, gps_ahrs.pitch.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, gps_ahrs.heading.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, gps_ahrs.xGyro.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, gps_ahrs.yGyro.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, gps_ahrs.zGyro.topic_name, 0);
	
	mosquitto_subscribe(mosq, NULL, ngc.pose.phi.filtered.topic_name, 0);	
	mosquitto_subscribe(mosq, NULL, ngc.pose.theta.filtered.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.pose.psi.filtered.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.p.filtered.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.q.filtered.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.r.filtered.topic_name, 0);
	
	// End register topic subscriptions


	fog_access.set(fog);
	gps_ahrs_access.set(gps_ahrs);
	ngc_access.set(ngc);

	mosquitto_message_callback_set(mosq, on_message_recv_callback);

	return 0;
}



void parse_topic_file(char* logic_name, char* topic_name, FOG_status& fog, GPS_AHRS_status& gps_ahrs,NGC_status& ngc)
{
	if (strcmp(logic_name, "Broker-address:") == 0) strcpy(broker_address, topic_name);
	if (strcmp(logic_name, "Broker-port:") == 0) sscanf(topic_name, "%d", &broker_port);
	if (strcmp(logic_name, "Keep-alive-time:") == 0) sscanf(topic_name, "%d", &keep_alive_time);

	if (strcmp(logic_name, "Environment:") == 0)		strcpy(environment, topic_name);	if (environment[0] == '/') environment[0] = '\0';
	if (strcmp(logic_name, "Robot:") == 0)				strcpy(robot, topic_name);			if (robot[0] == '/') robot[0] = '\0';



	if (strcmp(logic_name, "current-ang-sensor:") == 0)			sprintf(sensor.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-ang-measure:") == 0)		sprintf(measure_select.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-pose-phi-raw:") == 0)			sprintf(ngc.pose.phi.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-theta-raw:") == 0)			sprintf(ngc.pose.theta.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-psi-raw:") == 0)			sprintf(ngc.pose.psi.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-p-raw:") == 0)	sprintf(ngc.velocityBody.p.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-q-raw:") == 0)	sprintf(ngc.velocityBody.q.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-r-raw:") == 0)	sprintf(ngc.velocityBody.r.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-phi-filt:") == 0)			sprintf(ngc.pose.phi.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-theta-filt:") == 0)		sprintf(ngc.pose.theta.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-psi-filt:") == 0)			sprintf(ngc.pose.psi.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-p-filt:") == 0)	sprintf(ngc.velocityBody.p.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-q-filt:") == 0)	sprintf(ngc.velocityBody.q.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-r-filt:") == 0)	sprintf(ngc.velocityBody.r.filtered.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "FOG-roll:") == 0)					sprintf(fog.roll.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FOG-pitch:") == 0)					sprintf(fog.pitch.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FOG-heading:") == 0)				sprintf(fog.heading.topic_name, "%s%s%s", environment, robot, topic_name);
	/*
	if (strcmp(logic_name, "FOG-roll-rate:") == 0)				sprintf(fog.rollRate.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FOG-pitch-rate:") == 0)				sprintf(fog.pitchRate.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FOG-heading-rate:") == 0)			sprintf(fog.headingRate.topic_name, "%s%s%s", environment, robot, topic_name);
	*/

	if (strcmp(logic_name, "GPS-AHRS-roll:") == 0)				sprintf(gps_ahrs.roll.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "GPS-AHRS-pitch:") == 0)				sprintf(gps_ahrs.pitch.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "GPS-AHRS-heading:") == 0)			sprintf(gps_ahrs.heading.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "GPS-AHRS-roll-rate:") == 0)			sprintf(gps_ahrs.xGyro.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "GPS-AHRS-pitch-rate:") == 0)		sprintf(gps_ahrs.yGyro.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "GPS-AHRS-heading-rate:") == 0)		sprintf(gps_ahrs.zGyro.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-pose-phi-act:") == 0)			sprintf(ngc.pose.phi.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-theta-act:") == 0)			sprintf(ngc.pose.theta.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-psi-act:") == 0)			sprintf(ngc.pose.psi.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-p-act:") == 0)	sprintf(ngc.velocityBody.p.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-q-act:") == 0)	sprintf(ngc.velocityBody.q.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-r-act:") == 0)	sprintf(ngc.velocityBody.r.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	
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

	sensor.value = SENSOR_FOG;
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


void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
	checkMessage(sensor,msg);
	checkMessage(measure_select, msg);

	FOG_status fog;
	fog = fog_access.get();

	GPS_AHRS_status gps_ahrs;
	gps_ahrs = gps_ahrs_access.get();

	NGC_status ngc;
	ngc = ngc_access.get();

	NGCvariable* ngc_var = NULL;

	checkMessage(sensor, msg);
	checkMessage(measure_select, msg);


	if (checkMessage(fog.roll, msg) == true && sensor.value == SENSOR_FOG)
	{
		ngc_var = &(ngc.pose.phi);  ngc_var->raw.copyValueFrom(fog.roll);	ngc_var->raw.value = modpi(ngc_var->raw.value*M_PI/180.0);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
	}

	if (checkMessage(gps_ahrs.roll, msg) == true && sensor.value == SENSOR_GPS_AHRS)
	{ ngc_var = &(ngc.pose.phi);  ngc_var->raw.copyValueFrom(gps_ahrs.roll); ngc_var->raw.value = modpi(ngc_var->raw.value * M_PI / 180.0);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);}


	if (checkMessage(fog.pitch, msg) == true && sensor.value == SENSOR_FOG)
	{ ngc_var = &(ngc.pose.theta);  ngc_var->raw.copyValueFrom(fog.pitch);  ngc_var->raw.value = modpi(ngc_var->raw.value * M_PI / 180.0);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); }

	if (checkMessage(gps_ahrs.pitch, msg) == true && sensor.value == SENSOR_GPS_AHRS)
	{ ngc_var = &(ngc.pose.theta);  ngc_var->raw.copyValueFrom(gps_ahrs.pitch); ngc_var->raw.value = modpi(ngc_var->raw.value * M_PI / 180.0);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); }


	if (checkMessage(fog.heading, msg) == true && sensor.value == SENSOR_FOG)
	{ ngc_var = &(ngc.pose.psi);  ngc_var->raw.copyValueFrom(fog.heading);  ngc_var->raw.value = modpi(ngc_var->raw.value * M_PI / 180.0);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); }


	if (checkMessage(gps_ahrs.heading, msg) == true && sensor.value == SENSOR_GPS_AHRS)
	{ ngc_var = &(ngc.pose.psi);  ngc_var->raw.copyValueFrom(gps_ahrs.heading);	ngc_var->raw.value = modpi(ngc_var->raw.value * M_PI / 180.0);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); }


	
	if (checkMessage(gps_ahrs.xGyro, msg) == true)
	{ ngc_var = &(ngc.velocityBody.p);  ngc_var->raw.copyValueFrom(gps_ahrs.xGyro); ngc_var->raw.value = ngc_var->raw.value ;  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); }


	if (checkMessage(gps_ahrs.yGyro, msg) == true)
	{ ngc_var = &(ngc.velocityBody.q);  ngc_var->raw.copyValueFrom(gps_ahrs.yGyro);	ngc_var->raw.value = ngc_var->raw.value ; set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); }


	if (checkMessage(gps_ahrs.zGyro, msg) == true)
	{ ngc_var = &(ngc.velocityBody.r);  ngc_var->raw.copyValueFrom(gps_ahrs.zGyro); ngc_var->raw.value = ngc_var->raw.value ; set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); }


	ngc_var = &(ngc.pose.phi);			if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	ngc_var = &(ngc.pose.theta);		if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	ngc_var = &(ngc.pose.psi);			if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	ngc_var = &(ngc.velocityBody.p);	if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	ngc_var = &(ngc.velocityBody.q);	if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	ngc_var = &(ngc.velocityBody.r);	if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }

	fog_access.set(fog);
	gps_ahrs_access.set(gps_ahrs);
	ngc_access.set(ngc);
}






void execute()
{

}