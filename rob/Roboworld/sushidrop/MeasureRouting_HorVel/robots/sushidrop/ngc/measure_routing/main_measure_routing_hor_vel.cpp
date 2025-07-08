
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
#include "../../../../data/DVL_status.h"
#include "../../../../data/NGC_status.h"
#include "../../../../data/DataAccess.h"


#define PROCESS_NAME "MEASURE_ROUTING_HOR_VEL"

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
DataAccess<DVL_status> dvl_access;
DataAccess<NGC_status> ngc_access;

double xp=0.0, yp=0.0;
uint64 tp = 0;


void parse_topic_file(char* logic_name, char* topic_name, Pinger_status& pinger, GPS_AHRS_status& gps_ahrs, DVL_status& dvl,  NGC_status& ngc);
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


	Pinger_status pinger;
	GPS_AHRS_status gps_ahrs;
	DVL_status dvl;
	NGC_status ngc;

	pinger = pinger_access.get();
	gps_ahrs = gps_ahrs_access.get();
	dvl = dvl_access.get();
	ngc = ngc_access.get();

	while (!f.eof())
	{
		getline(f, line);

		if (line.length() != 0 && line[0] != '#')
		{
			sscanf(line.c_str(), "%s %s\n", s1, s2);
			parse_topic_file(s1, s2, pinger, gps_ahrs,dvl,ngc);
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
	
	mosquitto_subscribe(mosq, NULL, gps_ahrs.northVel.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, gps_ahrs.eastVel.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, gps_ahrs.speed.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, gps_ahrs.course.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, dvl.surge.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, dvl.sway.topic_name, 0);
	

	mosquitto_subscribe(mosq, NULL, ngc.pose.psi.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.u.filtered.topic_name, 0);	
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.v.filtered.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.uSeaCurrent.filtered.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.vSeaCurrent.filtered.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.xDot.filtered.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.yDot.filtered.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.speed.filtered.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.course.filtered.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.xDotSeaCurrent.filtered.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.yDotSeaCurrent.filtered.topic_name, 0);
	
	// End register topic subscriptions


	pinger_access.set(pinger);
	gps_ahrs_access.set(gps_ahrs);
	dvl_access.set(dvl);
	ngc_access.set(ngc);

	mosquitto_message_callback_set(mosq, on_message_recv_callback);

	return 0;
}



void parse_topic_file(char* logic_name, char* topic_name, Pinger_status& pinger, GPS_AHRS_status& gps_ahrs, DVL_status& dvl,  NGC_status& ngc)
{
	if (strcmp(logic_name, "Broker-address:") == 0) strcpy(broker_address, topic_name);
	if (strcmp(logic_name, "Broker-port:") == 0) sscanf(topic_name, "%d", &broker_port);
	if (strcmp(logic_name, "Keep-alive-time:") == 0) sscanf(topic_name, "%d", &keep_alive_time);

	if (strcmp(logic_name, "Environment:") == 0)		strcpy(environment, topic_name);	if (environment[0] == '/') environment[0] = '\0';
	if (strcmp(logic_name, "Robot:") == 0)				strcpy(robot, topic_name);			if (robot[0] == '/') robot[0] = '\0';

	

	if (strcmp(logic_name, "current-hor-vel-sensor:") == 0)				sprintf(sensor.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-hor-vel-measure:") == 0)			sprintf(measure_select.topic_name, "%s%s%s", environment, robot, topic_name);

	

	if (strcmp(logic_name, "PINGER-x:") == 0)							sprintf(pinger.x.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "PINGER-y:") == 0)							sprintf(pinger.y.topic_name, "%s%s%s", environment, robot, topic_name);
	
	if (strcmp(logic_name, "GPS-AHRS-northVel:") == 0)					sprintf(gps_ahrs.northVel.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "GPS-AHRS-eastVel:") == 0)					sprintf(gps_ahrs.eastVel.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "GPS-AHRS-speed:") == 0)						sprintf(gps_ahrs.speed.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "GPS-AHRS-course:") == 0)					sprintf(gps_ahrs.course.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "DVL-surge:") == 0)							sprintf(dvl.surge.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "DVL-sway:") == 0)							sprintf(dvl.sway.topic_name, "%s%s%s", environment, robot, topic_name);


	if (strcmp(logic_name, "ngc-velocity-inertial-xDot-raw:") == 0)		sprintf(ngc.velocityInertial.xDot.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-yDot-raw:") == 0)		sprintf(ngc.velocityInertial.yDot.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-xDot-filt:") == 0)	sprintf(ngc.velocityInertial.xDot.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-yDot-filt:") == 0)	sprintf(ngc.velocityInertial.yDot.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-xDot-act:") == 0)		sprintf(ngc.velocityInertial.xDot.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-yDot-act:") == 0)		sprintf(ngc.velocityInertial.yDot.actual.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-velocity-inertial-speed-raw:") == 0)	sprintf(ngc.velocityInertial.speed.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-course-raw:") == 0)	sprintf(ngc.velocityInertial.course.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-speed-filt:") == 0)	sprintf(ngc.velocityInertial.speed.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-course-filt:") == 0)	sprintf(ngc.velocityInertial.course.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-speed-act:") == 0)	sprintf(ngc.velocityInertial.speed.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-course-act:") == 0)	sprintf(ngc.velocityInertial.course.actual.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-velocity-body-u-raw:") == 0)			sprintf(ngc.velocityBody.u.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-v-raw:") == 0)			sprintf(ngc.velocityBody.v.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-u-filt:") == 0)			sprintf(ngc.velocityBody.u.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-v-filt:") == 0)			sprintf(ngc.velocityBody.v.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-u-act:") == 0)			sprintf(ngc.velocityBody.u.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-v-act:") == 0)			sprintf(ngc.velocityBody.v.actual.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-velocity-body-u-SeaCurrent-raw:") == 0)			sprintf(ngc.velocityBody.uSeaCurrent.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-v-SeaCurrent-raw:") == 0)			sprintf(ngc.velocityBody.vSeaCurrent.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-u-SeaCurrent-filt:") == 0)		sprintf(ngc.velocityBody.uSeaCurrent.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-v-SeaCurrent-filt:") == 0)		sprintf(ngc.velocityBody.vSeaCurrent.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-u-SeaCurrent-act:") == 0)			sprintf(ngc.velocityBody.uSeaCurrent.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-v-SeaCurrent-act:") == 0)			sprintf(ngc.velocityBody.vSeaCurrent.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	
	
	if (strcmp(logic_name, "ngc-velocity-inertial-xDot-SeaCurrent-act:") == 0)	sprintf(ngc.velocityInertial.xDotSeaCurrent.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-yDot-SeaCurrent-act:") == 0)	sprintf(ngc.velocityInertial.yDotSeaCurrent.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-xDot-SeaCurrent-raw:") == 0)	sprintf(ngc.velocityInertial.xDotSeaCurrent.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-yDot-SeaCurrent-raw:") == 0)	sprintf(ngc.velocityInertial.yDotSeaCurrent.raw.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-xDot-SeaCurrent-filt:") == 0)	sprintf(ngc.velocityInertial.xDotSeaCurrent.filtered.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-yDot-SeaCurrent-filt:") == 0)	sprintf(ngc.velocityInertial.yDotSeaCurrent.filtered.topic_name, "%s%s%s", environment, robot, topic_name);

	

	if (strcmp(logic_name, "ngc-pose-psi-act:") == 0)					sprintf(ngc.pose.psi.actual.topic_name, "%s%s%s", environment, robot, topic_name);
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


void inertial_to_body(NGC_status &ngc)
{
	if (ngc.velocityInertial.xDot.raw.timeStamp == ngc.velocityInertial.yDot.raw.timeStamp)
	{
		
		double psi = ngc.pose.psi.actual.value;
		double xDot = ngc.velocityInertial.xDot.raw.value;
		double yDot = ngc.velocityInertial.yDot.raw.value;
		double u, v;

		u =  xDot * cos(psi) + yDot * sin(psi);
		v = -xDot * sin(psi) + yDot * cos(psi);

		ngc.velocityBody.uSeaCurrent.raw.copyValueFrom(ngc.velocityBody.u.raw);						ngc.velocityBody.uSeaCurrent.raw.value = 0.0;
		ngc.velocityBody.vSeaCurrent.raw.copyValueFrom(ngc.velocityBody.v.raw);						ngc.velocityBody.vSeaCurrent.raw.value = 0.0;
		ngc.velocityBody.u.raw.copyValueFrom(ngc.velocityInertial.xDot.raw);						ngc.velocityBody.u.raw.value = u;
		ngc.velocityBody.v.raw.copyValueFrom(ngc.velocityInertial.yDot.raw);						ngc.velocityBody.v.raw.value = v;
		ngc.velocityInertial.xDotSeaCurrent.raw.copyValueFrom(ngc.velocityInertial.xDot.raw);		ngc.velocityInertial.xDotSeaCurrent.raw.value = 0.0;
		ngc.velocityInertial.yDotSeaCurrent.raw.copyValueFrom(ngc.velocityInertial.yDot.raw);		ngc.velocityInertial.yDotSeaCurrent.raw.value = 0.0;

		NGCvariable* ngc_var = NULL;
		ngc_var = &(ngc.velocityBody.uSeaCurrent);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		ngc_var = &(ngc.velocityBody.vSeaCurrent);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		ngc_var = &(ngc.velocityBody.u);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		ngc_var = &(ngc.velocityBody.v);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		ngc_var = &(ngc.velocityInertial.xDotSeaCurrent);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		ngc_var = &(ngc.velocityInertial.yDotSeaCurrent);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
	}
}


void body_to_inertial(NGC_status& ngc)
{
	if (ngc.velocityBody.u.raw.timeStamp == ngc.velocityBody.v.raw.timeStamp  && ngc.velocityBody.u.raw.valid==1 && ngc.velocityBody.v.raw.valid == 1)
	{
		double psi = ngc.pose.psi.actual.value;
		double u = ngc.velocityBody.u.raw.value;
		double v = ngc.velocityBody.v.raw.value;
		double xDot, yDot,course, speed;

		xDot = u * cos(psi) - v * sin(psi);
		yDot = u * sin(psi) + v * cos(psi);

		speed = sqrt(xDot * xDot + yDot*yDot);
		course = atan2(yDot, xDot);

		ngc.velocityBody.uSeaCurrent.raw.copyValueFrom(ngc.velocityBody.u.raw);					ngc.velocityBody.uSeaCurrent.raw.value = 0.0;
		ngc.velocityBody.vSeaCurrent.raw.copyValueFrom(ngc.velocityBody.v.raw);					ngc.velocityBody.vSeaCurrent.raw.value = 0.0;
		ngc.velocityInertial.xDot.raw.copyValueFrom(ngc.velocityBody.u.raw);					ngc.velocityInertial.xDot.raw.value = xDot;
		ngc.velocityInertial.yDot.raw.copyValueFrom(ngc.velocityBody.v.raw);					ngc.velocityInertial.yDot.raw.value = yDot;
		ngc.velocityInertial.xDotSeaCurrent.raw.copyValueFrom(ngc.velocityInertial.xDot.raw);	ngc.velocityInertial.xDotSeaCurrent.raw.value = 0.0;
		ngc.velocityInertial.yDotSeaCurrent.raw.copyValueFrom(ngc.velocityInertial.yDot.raw);	ngc.velocityInertial.yDotSeaCurrent.raw.value = 0.0;
		ngc.velocityInertial.speed.raw.copyValueFrom(ngc.velocityBody.u.raw);					ngc.velocityInertial.speed.raw.value = speed;
		ngc.velocityInertial.course.raw.copyValueFrom(ngc.velocityBody.v.raw);					ngc.velocityInertial.course.raw.value = course;

		NGCvariable* ngc_var = NULL;
		ngc_var = &(ngc.velocityBody.uSeaCurrent);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		ngc_var = &(ngc.velocityBody.vSeaCurrent);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		ngc_var = &(ngc.velocityInertial.xDot);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		ngc_var = &(ngc.velocityInertial.yDot);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		ngc_var = &(ngc.velocityInertial.speed);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		ngc_var = &(ngc.velocityInertial.course);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		ngc_var = &(ngc.velocityInertial.xDotSeaCurrent);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		ngc_var = &(ngc.velocityInertial.yDotSeaCurrent);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
	}
}


void pinger_to_velocity(NGC_status& ngc,Pinger_status& pinger)
{
	if (pinger.x.timeStamp == pinger.y.timeStamp  && pinger.x.valid==1 && pinger.y.valid==1)
	{
		if (tp > 0  &&  tp < pinger.x.timeStamp)
		{
			double x = pinger.x.value;
			double y = pinger.y.value;
			double dt = ((double)(pinger.x.timeStamp - tp)) / 1000.0;

			double xDot, yDot, course, speed, u ,v;
			double psi = ngc.pose.psi.actual.value;
		
			xDot = (x - xp) / dt;
			yDot = (y - yp) / dt;
			speed = sqrt(xDot * xDot + yDot * yDot);
			course = atan2(yDot, xDot);
			u = xDot * cos(psi) + yDot * sin(psi);
			v = -xDot * sin(psi) + yDot * cos(psi);

			ngc.velocityBody.uSeaCurrent.raw.copyValueFrom(ngc.velocityBody.u.raw);			ngc.velocityBody.uSeaCurrent.raw.value = 0.0;
			ngc.velocityBody.vSeaCurrent.raw.copyValueFrom(ngc.velocityBody.v.raw);			ngc.velocityBody.vSeaCurrent.raw.value = 0.0;
			ngc.velocityBody.u.raw.copyValueFrom(ngc.pose.psi.actual);						ngc.velocityBody.u.raw.value = u;
			ngc.velocityBody.v.raw.copyValueFrom(ngc.pose.psi.actual);						ngc.velocityBody.v.raw.value = v;
			ngc.velocityInertial.xDot.raw.copyValueFrom(ngc.pose.psi.actual);				ngc.velocityInertial.xDot.raw.value = xDot;
			ngc.velocityInertial.yDot.raw.copyValueFrom(ngc.pose.psi.actual);				ngc.velocityInertial.yDot.raw.value = yDot;
			ngc.velocityInertial.xDotSeaCurrent.raw.copyValueFrom(ngc.pose.psi.actual);		ngc.velocityInertial.xDotSeaCurrent.raw.value = 0.0;
			ngc.velocityInertial.yDotSeaCurrent.raw.copyValueFrom(ngc.pose.psi.actual);		ngc.velocityInertial.yDotSeaCurrent.raw.value = 0.0;
			ngc.velocityInertial.speed.raw.copyValueFrom(ngc.pose.psi.actual);				ngc.velocityInertial.speed.raw.value = speed;
			ngc.velocityInertial.course.raw.copyValueFrom(ngc.pose.psi.actual);				ngc.velocityInertial.course.raw.value = course;


			NGCvariable* ngc_var = NULL;
			ngc_var = &(ngc.velocityBody.uSeaCurrent);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
			ngc_var = &(ngc.velocityBody.vSeaCurrent);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
			ngc_var = &(ngc.velocityBody.u);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
			ngc_var = &(ngc.velocityBody.v);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
			ngc_var = &(ngc.velocityInertial.xDot);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
			ngc_var = &(ngc.velocityInertial.yDot);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
			ngc_var = &(ngc.velocityInertial.speed);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
			ngc_var = &(ngc.velocityInertial.course);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
			ngc_var = &(ngc.velocityInertial.xDotSeaCurrent);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
			ngc_var = &(ngc.velocityInertial.yDotSeaCurrent);  set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW);
		}

		tp = pinger.x.timeStamp;
		xp = pinger.x.value;
		yp = pinger.y.value;
	}
}


void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
	checkMessage(sensor,msg);
	checkMessage(measure_select, msg);

	Pinger_status pinger;
	pinger = pinger_access.get();

	GPS_AHRS_status gps_ahrs;
	gps_ahrs = gps_ahrs_access.get();

	DVL_status dvl;
	dvl = dvl_access.get();

	NGC_status ngc;
	ngc = ngc_access.get();

	NGCvariable *ngc_var=NULL;

	checkMessage(sensor, msg);
	checkMessage(measure_select, msg);

	checkMessage(ngc.pose.psi.actual, msg);

	if (checkMessage(gps_ahrs.northVel, msg) == true && sensor.value == SENSOR_GPS_AHRS)
	{ ngc_var = &(ngc.velocityInertial.xDot);	ngc_var->raw.copyValueFrom(gps_ahrs.northVel);	set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); inertial_to_body(ngc); }

	if (checkMessage(gps_ahrs.eastVel, msg) == true && sensor.value == SENSOR_GPS_AHRS)
	{ ngc_var = &(ngc.velocityInertial.yDot);	ngc_var->raw.copyValueFrom(gps_ahrs.eastVel);	set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); inertial_to_body(ngc); }

	if (checkMessage(gps_ahrs.speed, msg) == true && sensor.value == SENSOR_GPS_AHRS)
	{ ngc_var = &(ngc.velocityInertial.speed);	ngc_var->raw.copyValueFrom(gps_ahrs.speed);		set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); inertial_to_body(ngc); }

	if (checkMessage(gps_ahrs.course, msg) == true && sensor.value == SENSOR_GPS_AHRS)
	{
		ngc_var = &(ngc.velocityInertial.course);	ngc_var->raw.copyValueFrom(gps_ahrs.course);  ngc_var->raw.value = modpi(ngc_var->raw.value*M_PI/180.0);	set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); inertial_to_body(ngc);
	}



	if (checkMessage(dvl.surge, msg) == true && sensor.value == SENSOR_DVL)
	{ ngc_var = &(ngc.velocityBody.u);	ngc_var->raw.copyValueFrom(dvl.surge);	set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); body_to_inertial(ngc); }

	if (checkMessage(dvl.sway, msg) == true && sensor.value == SENSOR_DVL)
	{ ngc_var = &(ngc.velocityBody.v);	ngc_var->raw.copyValueFrom(dvl.sway);	set_var(ngc_var->raw, ngc_var->actual, MEASURE_RAW); body_to_inertial(ngc);	}



	if (checkMessage(pinger.x, msg) == true && sensor.value == SENSOR_USBL)
	{ pinger_to_velocity(ngc,pinger); }

	if (checkMessage(pinger.y, msg) == true && sensor.value == SENSOR_USBL)
	{ pinger_to_velocity(ngc, pinger);	}

	

	ngc_var = &(ngc.velocityInertial.xDot); if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER,false); }
	ngc_var = &(ngc.velocityInertial.yDot); if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	ngc_var = &(ngc.velocityInertial.speed); if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	ngc_var = &(ngc.velocityInertial.course); if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	ngc_var = &(ngc.velocityInertial.xDotSeaCurrent); if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	ngc_var = &(ngc.velocityInertial.yDotSeaCurrent); if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	ngc_var = &(ngc.velocityBody.u); if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	ngc_var = &(ngc.velocityBody.v); if (checkMessage(ngc_var->filtered, msg) == true) { set_var(ngc_var->filtered, ngc_var->actual, MEASURE_FILTER, false); }
	
	pinger_access.set(pinger);
	gps_ahrs_access.set(gps_ahrs);
	dvl_access.set(dvl);
	ngc_access.set(ngc);
}






void execute()
{

}