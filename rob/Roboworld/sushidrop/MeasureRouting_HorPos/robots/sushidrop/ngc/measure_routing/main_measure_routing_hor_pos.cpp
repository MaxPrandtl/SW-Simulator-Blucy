
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
#include "../../../../data/Pinger_status.h"
#include "../../../../data/GPS_AHRS_status.h"
#include "../../../../data/NGC_status.h"
#include "../../../../data/DataAccess.h"


#define PROCESS_NAME "MEASURE_ROUTING_HOR_POS"

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
DataAccess<Pinger_status> pinger_access;
DataAccess<GPS_AHRS_status> gps_ahrs_access;
DataAccess<NGC_status> ngc_access;


void parse_topic_file(char* logic_name, char* topic_name, Pinger_status& pinger, GPS_AHRS_status& gps_ahrs,NGC_status& ngc);
void execute();
void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg);
void publishMessage(Variable& dv);

void ctrl_c_handler(int dummy)
{
	running = false;
}



int init_pos()
{
	FILE* f;
	char value[100], value2[100], name_dir_file[256];
	double val;

	double utmzone;
	char utmzone_char;


	GPS_AHRS_status gps_ahrs;
	gps_ahrs = gps_ahrs_access.get();

	sprintf(name_dir_file, "%s%s", CONFIGURATION_FILES_DIRECTORY, CONFIGURATION_GPS_AHRS_FILE);

	if ((f = fopen(name_dir_file, "r")) != NULL)
	{
		fscanf(f, "%s\n", value);

		while (!feof(f))
		{
			fscanf(f, "%s %s %lf\n", value, value2, &val);

			if (strcmp(value, "origin_latitude") == 0) gps_ahrs.lat0.value = val;
			if (strcmp(value, "origin_longitude") == 0) gps_ahrs.lon0.value = val;
		}

		fclose(f);

		compute_xy_from_lat_lon(gps_ahrs.lat0.value, gps_ahrs.lon0.value, gps_ahrs.x0.value, gps_ahrs.y0.value, utmzone, utmzone_char);

		gps_ahrs_access.set(gps_ahrs);

		return 0;
	}

	else
	{
		printf("GPS_AHRS::init - File error: %s\n", name_dir_file);
		return -1;
	}



	return 0;
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


	Pinger_status pinger;
	GPS_AHRS_status gps_ahrs;
	NGC_status ngc;

	pinger = pinger_access.get();
	gps_ahrs = gps_ahrs_access.get();
	ngc = ngc_access.get();

	while (!f.eof())
	{
		getline(f, line);

		if (line.length() != 0 && line[0] != '#')
		{
			sscanf(line.c_str(), "%s %s\n", s1, s2);
			parse_topic_file(s1, s2, pinger, gps_ahrs,ngc);
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
	
	
	mosquitto_subscribe(mosq, NULL, pinger.x.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, pinger.y.topic_name, 0);		

	mosquitto_subscribe(mosq, NULL, gps_ahrs.latitude.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, gps_ahrs.longitude.topic_name, 0); 

	mosquitto_subscribe(mosq, NULL, ngc.pose.x.filtered.topic_name, 0);	
	mosquitto_subscribe(mosq, NULL, ngc.pose.y.filtered.topic_name, 0);
	
	
	
	// End register topic subscriptions


	pinger_access.set(pinger);
	gps_ahrs_access.set(gps_ahrs);
	ngc_access.set(ngc);

	mosquitto_message_callback_set(mosq, on_message_recv_callback);

	return 0;
}



void parse_topic_file(char* logic_name, char* topic_name, Pinger_status& pinger, GPS_AHRS_status& gps_ahrs,NGC_status& ngc)
{
	if (strcmp(logic_name, "Broker-address:") == 0) strcpy(broker_address, topic_name);
	if (strcmp(logic_name, "Broker-port:") == 0) sscanf(topic_name, "%d", &broker_port);
	if (strcmp(logic_name, "Keep-alive-time:") == 0) sscanf(topic_name, "%d", &keep_alive_time);

	if (strcmp(logic_name, "Environment:") == 0)		strcpy(environment, topic_name);	if (environment[0] == '/') environment[0] = '\0';
	if (strcmp(logic_name, "Robot:") == 0)				strcpy(robot, topic_name);			if (robot[0] == '/') robot[0] = '\0';

	

	if (strcmp(logic_name, "current-hor-pos-sensor:") == 0)		sprintf(sensor.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-hor-pos-measure:") == 0)	sprintf(measure_select.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-pose-x-raw:") == 0)			sprintf(ngc.pose.x.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-y-raw:") == 0)			sprintf(ngc.pose.y.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-x-filt:") == 0)		sprintf(ngc.pose.x.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-y-filt:") == 0)		sprintf(ngc.pose.y.filtered.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "PINGER-x:") == 0)				sprintf(pinger.x.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "PINGER-y:") == 0)				sprintf(pinger.y.topic_name, "%s%s%s", environment, robot, topic_name);
	
	if (strcmp(logic_name, "GPS-AHRS-latitude:") == 0)		sprintf(gps_ahrs.latitude.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "GPS-AHRS-longitude:") == 0)		sprintf(gps_ahrs.longitude.topic_name, "%s%s%s", environment, robot, topic_name);
	
	if (strcmp(logic_name, "ngc-pose-x-act:") == 0)			sprintf(ngc.pose.x.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-y-act:") == 0)			sprintf(ngc.pose.y.actual.topic_name, "%s%s%s", environment, robot, topic_name);
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

	init_pos();

	sensor.value = SENSOR_GPS_AHRS;
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


void computeXY(GPS_AHRS_status& gps_ahrs, NGC_status& ngc)
{
	double utmzone;
	char utmzone_char;

	

	compute_xy_from_lat_lon(gps_ahrs.latitude.value, gps_ahrs.longitude.value, gps_ahrs.x.value, gps_ahrs.y.value, utmzone, utmzone_char);

	ngc.pose.x.raw.value = gps_ahrs.x.value - gps_ahrs.x0.value;
	ngc.pose.y.raw.value = gps_ahrs.y.value - gps_ahrs.y0.value;
}


void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
	checkMessage(sensor,msg);
	checkMessage(measure_select, msg);

	

	Pinger_status pinger;
	pinger = pinger_access.get();

	GPS_AHRS_status gps_ahrs;
	gps_ahrs = gps_ahrs_access.get();

	NGC_status ngc;
	ngc = ngc_access.get();

	NGCvariable* ngc_var=NULL;

	checkMessage(sensor, msg);
	checkMessage(measure_select, msg);

	if (checkMessage(pinger.x, msg) == true && sensor.value == SENSOR_USBL)
	{ ngc_var = &(ngc.pose.x);  ngc_var->raw.copyValueFrom(pinger.x);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); }

	if (checkMessage(gps_ahrs.latitude, msg) == true && sensor.value == SENSOR_GPS_AHRS)
	{
		ngc_var = &(ngc.pose.x);  ngc_var->raw.copyValueFrom(gps_ahrs.latitude); computeXY(gps_ahrs,ngc); set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
	}

	if (checkMessage(pinger.y, msg) == true && sensor.value == SENSOR_USBL)
	{ ngc_var = &(ngc.pose.y);  ngc_var->raw.copyValueFrom(pinger.y);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); }

	if (checkMessage(gps_ahrs.longitude, msg) == true && sensor.value == SENSOR_GPS_AHRS)
	{
		ngc_var = &(ngc.pose.y);  ngc_var->raw.copyValueFrom(gps_ahrs.longitude); computeXY(gps_ahrs,ngc);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
	}


	ngc_var = &(ngc.pose.x);  if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER,false); }
	ngc_var = &(ngc.pose.y);  if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	
	pinger_access.set(pinger);
	gps_ahrs_access.set(gps_ahrs);
	ngc_access.set(ngc);
}






void execute()
{

}