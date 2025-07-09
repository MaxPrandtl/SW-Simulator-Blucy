
#define __MAIN__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "mosquitto.h"
#include "../../../generic/define.h"
#include "../../../data/DataAccess.h"
#include "../../../data/NGC_status.h"
#include "../../../data/GPS_AHRS_status.h"
#include "../../../data/ECHOLOGGER_status.h"
#include "../../../data/UBLOX_status.h"
#include "../../../data/Time_status.h"
#include "../../../data/Actuator_swamp_status.h"
#include "../../../data/Thrust_mapping_status.h"

#include "SwampLogger.h"
#include <fstream>


#define PROCESS_NAME "SwampLogger"



bool running = true;
struct 	mosquitto* mosq = NULL;

char broker_address[BUF_SIZE];
int broker_port;
int keep_alive_time;
char mqtt_client[BUF_SIZE];

char environment[BUF_SIZE];
char robot[BUF_SIZE];


SwampLogger* logger = NULL;

DataAccess<Time_status> time_access;
DataAccess<NGC_status> ngc_access;
DataAccess<GPS_AHRS_status> gps_ahrs_access;
DataAccess<ECHOLOGGER_status> echo_access;
DataAccess<UBLOX_status> ublox_access;
DataAccess<Actuator_swamp_status> FL_motor_access;
DataAccess<Actuator_swamp_status> FR_motor_access;
DataAccess<Actuator_swamp_status> RL_motor_access;
DataAccess<Actuator_swamp_status> RR_motor_access;
DataAccess<Thrust_mapping_status> thrust_mapping_access;


void execute();
void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg);
void parse_topic_file(char* logic_name, char* topic_name, Time_status& clock, NGC_status& ngc, Actuator_swamp_status& fl_status, Actuator_swamp_status& fr_status, Actuator_swamp_status& rl_status, Actuator_swamp_status& rr_status, Thrust_mapping_status& thrust_status,GPS_AHRS_status& gps_ahrs_status, ECHOLOGGER_status& echo_status, UBLOX_status& ublox_status);

void ctrl_c_handler(int dummy)
{
	running = false;
}


int init_mqtt()
{
	// Read config file
	char s1[BUF_SIZE], s2[BUF_SIZE];

	char topics_file[BUF_SIZE];
	sprintf(topics_file, "%s%s", CONFIGURATION_FILES_DIRECTORY, CONFIGURATION_TOPICS_FILE);

	// read config file
	std::ifstream f(topics_file, std::ifstream::in);
	std::string line;
	
	if (f.fail()) return -1;

	
	
	Time_status clock;
	clock = time_access.get();

	NGC_status ngc;
	ngc = ngc_access.get();

	GPS_AHRS_status gps_ahrs;
	gps_ahrs = gps_ahrs_access.get();

	ECHOLOGGER_status echo;
	echo = echo_access.get();

	UBLOX_status ublox;
	ublox = ublox_access.get();

	Actuator_swamp_status fl_status;
	fl_status = FL_motor_access.get();

	Actuator_swamp_status fr_status;
	fr_status = FR_motor_access.get();

	Actuator_swamp_status rl_status;
	rl_status = RL_motor_access.get();

	Actuator_swamp_status rr_status;
	rr_status = RR_motor_access.get();

	Thrust_mapping_status thrust_mapping_status;
	thrust_mapping_status = thrust_mapping_access.get();

	while (!f.eof())
	{
		getline(f, line);

		if (line.length() != 0 && line[0] != '#')
		{
			sscanf(line.c_str(), "%s %s\n", s1, s2);
			parse_topic_file(s1, s2,clock, ngc, fl_status, fr_status, rl_status, rr_status, thrust_mapping_status,gps_ahrs,echo,ublox);
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


	time_access.set(clock);
	ngc_access.set(ngc);
	gps_ahrs_access.set(gps_ahrs);
	echo_access.set(echo);
	ublox_access.set(ublox);
	FL_motor_access.set(fl_status);
	FR_motor_access.set(fr_status);
	RL_motor_access.set(rl_status);
	RR_motor_access.set(rr_status);
	thrust_mapping_access.set(thrust_mapping_status);

	// Register topic subscriptions
	mosquitto_subscribe(mosq, NULL, clock.timeStamp.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, clock.dt.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, clock.date_time.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ngc.force.fu.reference.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.force.fv.reference.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.force.tr.reference.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ngc.force.fu.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.force.fv.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.force.tr.actual.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.u.reference.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.v.reference.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.r.reference.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.u.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.v.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.r.actual.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.xDot.reference.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.yDot.reference.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.speed.reference.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.course.reference.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.xDot.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.yDot.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.speed.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.velocityInertial.course.actual.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ngc.pose.x.reference.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.pose.y.reference.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.pose.psi.reference.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.pose.a.reference.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ngc.pose.x.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.pose.y.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.pose.psi.actual.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ngc.pose.a.actual.topic_name, 0);


	mosquitto_subscribe(mosq, NULL, fl_status.thr_power.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fl_status.thr_enable.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fl_status.thr_ref.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fl_status.thr_act.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, fr_status.thr_power.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fr_status.thr_enable.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fr_status.thr_ref.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fr_status.thr_act.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, rl_status.thr_power.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, rl_status.thr_enable.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, rl_status.thr_ref.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, rl_status.thr_act.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, rr_status.thr_power.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, rr_status.thr_enable.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, rr_status.thr_ref.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, rr_status.thr_act.topic_name, 0);



	mosquitto_subscribe(mosq, NULL, fl_status.azm_power.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fl_status.azm_enable.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fl_status.azm_ref.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fl_status.azm_act.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, fr_status.azm_power.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fr_status.azm_enable.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fr_status.azm_ref.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, fr_status.azm_act.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, rl_status.azm_power.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, rl_status.azm_enable.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, rl_status.azm_ref.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, rl_status.azm_act.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, rr_status.azm_power.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, rr_status.azm_enable.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, rr_status.azm_ref.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, rr_status.azm_act.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, thrust_mapping_status.command.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, thrust_mapping_status.status.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, gps_ahrs.latitude.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, gps_ahrs.longitude.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, echo.altitude.topic_name, 0);

	mosquitto_subscribe(mosq, NULL, ublox.latitude.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ublox.longitude.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ublox.x.topic_name, 0);
	mosquitto_subscribe(mosq, NULL, ublox.y.topic_name, 0);
	
	// End register topic subscriptions

	mosquitto_message_callback_set(mosq, on_message_recv_callback);
	return 0;
}


void parse_topic_file(char* logic_name, char* topic_name, Time_status& clock, NGC_status& ngc, Actuator_swamp_status &fl_status, Actuator_swamp_status& fr_status, Actuator_swamp_status& rl_status, Actuator_swamp_status& rr_status, Thrust_mapping_status& thrust_mapping_status, GPS_AHRS_status& gps_ahrs_status, ECHOLOGGER_status& echo_status, UBLOX_status& ublox_status)
{
	if (strcmp(logic_name, "Broker-address:") == 0)		strcpy(broker_address, topic_name);
	if (strcmp(logic_name, "Broker-port:") == 0)		sscanf(topic_name, "%d", &broker_port);
	if (strcmp(logic_name, "Keep-alive-time:") == 0)	sscanf(topic_name, "%d", &keep_alive_time);

	if (strcmp(logic_name, "Environment:") == 0)		strcpy(environment, topic_name);	if (environment[0] == '/') environment[0] = '\0';
	if (strcmp(logic_name, "Robot:") == 0)				strcpy(robot, topic_name);			if (robot[0] == '/') robot[0] = '\0';

	
	if (strcmp(logic_name, "timeStamp:") == 0)			sprintf(clock.timeStamp.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "dt:") == 0)					sprintf(clock.dt.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "date-time:") == 0)			sprintf(clock.date_time.topic_name, "%s%s%s", environment, robot, topic_name);


	if (strcmp(logic_name, "ngc-force-fu-ref:") == 0)	sprintf(ngc.force.fu.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fv-ref:") == 0)	sprintf(ngc.force.fv.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-tr-ref:") == 0)	sprintf(ngc.force.tr.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fu-act:") == 0)	sprintf(ngc.force.fu.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fv-act:") == 0)	sprintf(ngc.force.fv.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-tr-act:") == 0)	sprintf(ngc.force.tr.actual.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-velocity-body-r-ref:") == 0)	sprintf(ngc.velocityBody.r.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-u-ref:") == 0)	sprintf(ngc.velocityBody.u.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-v-ref:") == 0)	sprintf(ngc.velocityBody.v.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-r-act:") == 0)	sprintf(ngc.velocityBody.r.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-u-act:") == 0)	sprintf(ngc.velocityBody.u.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-v-act:") == 0)	sprintf(ngc.velocityBody.v.actual.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-velocity-inertial-xDot-ref:") == 0)		sprintf(ngc.velocityInertial.xDot.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-yDot-ref:") == 0)		sprintf(ngc.velocityInertial.yDot.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-speed-ref:") == 0)	sprintf(ngc.velocityInertial.speed.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-course-ref:") == 0)	sprintf(ngc.velocityInertial.course.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-xDot-act:") == 0)		sprintf(ngc.velocityInertial.xDot.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-yDot-act:") == 0)		sprintf(ngc.velocityInertial.yDot.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-speed-act:") == 0)	sprintf(ngc.velocityInertial.speed.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-course-act:") == 0)	sprintf(ngc.velocityInertial.course.actual.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-pose-x-ref:") == 0)		sprintf(ngc.pose.x.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-y-ref:") == 0)		sprintf(ngc.pose.y.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-psi-ref:") == 0)	sprintf(ngc.pose.psi.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-a-ref:") == 0)		sprintf(ngc.pose.a.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-x-act:") == 0)		sprintf(ngc.pose.x.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-y-act:") == 0)		sprintf(ngc.pose.y.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-psi-act:") == 0)	sprintf(ngc.pose.psi.actual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-a-act:") == 0)		sprintf(ngc.pose.a.actual.topic_name, "%s%s%s", environment, robot, topic_name);


	if (strcmp(logic_name, "FL-THR-power-command:") == 0)	sprintf(fl_status.thr_power.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FL-THR-enable-command:") == 0)	sprintf(fl_status.thr_enable.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FL-THR-force-reference:") == 0)	sprintf(fl_status.thr_ref.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FL-THR-force-actual:") == 0)	sprintf(fl_status.thr_act.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "FR-THR-power-command:") == 0)	sprintf(fr_status.thr_power.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FR-THR-enable-command:") == 0)	sprintf(fr_status.thr_enable.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FR-THR-force-reference:") == 0)	sprintf(fr_status.thr_ref.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FR-THR-force-actual:") == 0)	sprintf(fr_status.thr_act.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "RL-THR-power-command:") == 0)	sprintf(rl_status.thr_power.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "RL-THR-enable-command:") == 0)	sprintf(rl_status.thr_enable.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "RL-THR-force-reference:") == 0)	sprintf(rl_status.thr_ref.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "RL-THR-force-actual:") == 0)	sprintf(rl_status.thr_act.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "RR-THR-power-command:") == 0)	sprintf(rr_status.thr_power.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "RR-THR-enable-command:") == 0)	sprintf(rr_status.thr_enable.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "RR-THR-force-reference:") == 0)	sprintf(rr_status.thr_ref.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "RR-THR-force-actual:") == 0)	sprintf(rr_status.thr_act.topic_name, "%s%s%s", environment, robot, topic_name);


	if (strcmp(logic_name, "FL-AZM-power-command:") == 0)	sprintf(fl_status.azm_power.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FL-AZM-enable-command:") == 0)	sprintf(fl_status.azm_enable.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FL-AZM-angle-reference:") == 0)	sprintf(fl_status.azm_ref.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FL-AZM-angle-actual:") == 0)	sprintf(fl_status.azm_act.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "FR-AZM-power-command:") == 0)	sprintf(fr_status.azm_power.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FR-AZM-enable-command:") == 0)	sprintf(fr_status.azm_enable.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FR-AZM-angle-reference:") == 0)	sprintf(fr_status.azm_ref.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "FR-AZM-angle-actual:") == 0)	sprintf(fr_status.azm_act.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "RL-AZM-power-command:") == 0)	sprintf(rl_status.azm_power.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "RL-AZM-enable-command:") == 0)	sprintf(rl_status.azm_enable.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "RL-AZM-angle-reference:") == 0)	sprintf(rl_status.azm_ref.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "RL-AZM-angle-actual:") == 0)	sprintf(rl_status.azm_act.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "RR-AZM-power-command:") == 0)	sprintf(rr_status.azm_power.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "RR-AZM-enable-command:") == 0)	sprintf(rr_status.azm_enable.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "RR-AZM-angle-reference:") == 0)	sprintf(rr_status.azm_ref.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "RR-AZM-angle-actual:") == 0)	sprintf(rr_status.azm_act.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "thrust-mapping-command:") == 0)		sprintf(thrust_mapping_status.command.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "thrust-mapping-status:") == 0)		sprintf(thrust_mapping_status.status.topic_name, "%s%s%s", environment, robot, topic_name);


	if (strcmp(logic_name, "GPS-AHRS-latitude:") == 0)		sprintf(gps_ahrs_status.latitude.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "GPS-AHRS-longitude:") == 0)		sprintf(gps_ahrs_status.longitude.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ECHOLOGGER-altitude:") == 0)	sprintf(echo_status.altitude.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "UBLOX-latitude:") == 0)			sprintf(ublox_status.latitude.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "UBLOX-longitude:") == 0)		sprintf(ublox_status.longitude.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "UBLOX-x:") == 0)				sprintf(ublox_status.x.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "UBLOX-y:") == 0)				sprintf(ublox_status.y.topic_name, "%s%s%s", environment, robot, topic_name);

}


void mqtt_loop()
{
	int loop_ret;
	loop_ret=mosquitto_loop(mosq, MAX_MQTT_LOOP_WAIT, 1);
	if (loop_ret != 0) mosquitto_reconnect(mosq);
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
	

	signal(SIGINT, ctrl_c_handler);
	
	
	if (init_mqtt() != 0)
	{
		printf("ERROR init_mqtt()\n");
		return -1;
	}
	

	logger = new SwampLogger(PROCESS_NAME, &FL_motor_access, &FR_motor_access, &RL_motor_access, &RR_motor_access, &ngc_access, &thrust_mapping_access,&gps_ahrs_access,&echo_access,&ublox_access, &time_access );

	logger->create();

	

	while (running)
	{
		execute();
		mqtt_loop();
	}

	logger->terminate();

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


void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
	Time_status clock;
	clock = time_access.get();

	NGC_status ngc;
	ngc = ngc_access.get();

	GPS_AHRS_status gps_ahrs_status;
	gps_ahrs_status = gps_ahrs_access.get();

	ECHOLOGGER_status echo_status;
	echo_status = echo_access.get();

	UBLOX_status ublox_status;
	ublox_status = ublox_access.get();

	Actuator_swamp_status fl_status;
	fl_status = FL_motor_access.get();

	Actuator_swamp_status fr_status;
	fr_status = FR_motor_access.get();

	Actuator_swamp_status rl_status;
	rl_status = RL_motor_access.get();

	Actuator_swamp_status rr_status;
	rr_status = RR_motor_access.get();

	Thrust_mapping_status thrust_mapping_status;
	thrust_mapping_status = thrust_mapping_access.get();


	checkMessage(clock.timeStamp, msg);
	checkMessage(clock.dt, msg);
	checkMessage(clock.date_time, msg);

	checkMessage(ngc.force.fu.reference, msg);
	checkMessage(ngc.force.fv.reference, msg);
	checkMessage(ngc.force.tr.reference, msg);

	checkMessage(ngc.force.fu.actual, msg);
	checkMessage(ngc.force.fv.actual, msg);
	checkMessage(ngc.force.tr.actual, msg);

	checkMessage(ngc.velocityBody.u.reference, msg);
	checkMessage(ngc.velocityBody.v.reference, msg);
	checkMessage(ngc.velocityBody.r.reference, msg);

	checkMessage(ngc.velocityBody.u.actual, msg);
	checkMessage(ngc.velocityBody.v.actual, msg);
	checkMessage(ngc.velocityBody.r.actual, msg);

	checkMessage(ngc.velocityInertial.xDot.reference, msg);
	checkMessage(ngc.velocityInertial.yDot.reference, msg);
	checkMessage(ngc.velocityInertial.speed.reference, msg);
	checkMessage(ngc.velocityInertial.course.reference, msg);

	checkMessage(ngc.velocityInertial.xDot.actual, msg);
	checkMessage(ngc.velocityInertial.yDot.actual, msg);
	checkMessage(ngc.velocityInertial.speed.actual, msg);
	checkMessage(ngc.velocityInertial.course.actual, msg);

	checkMessage(ngc.pose.x.reference, msg);
	checkMessage(ngc.pose.y.reference, msg);
	checkMessage(ngc.pose.psi.reference, msg);
	checkMessage(ngc.pose.a.reference, msg);

	checkMessage(ngc.pose.x.actual, msg);
	checkMessage(ngc.pose.y.actual, msg);
	checkMessage(ngc.pose.psi.actual, msg);
	checkMessage(ngc.pose.a.actual, msg);


	checkMessage(fl_status.thr_power, msg);	checkMessage(fl_status.thr_enable, msg);	checkMessage(fl_status.thr_ref, msg);	checkMessage(fl_status.thr_act, msg);
	checkMessage(fr_status.thr_power, msg);	checkMessage(fr_status.thr_enable, msg);	checkMessage(fr_status.thr_ref, msg);	checkMessage(fr_status.thr_act, msg);
	checkMessage(rl_status.thr_power, msg);	checkMessage(rl_status.thr_enable, msg);	checkMessage(rl_status.thr_ref, msg);	checkMessage(rl_status.thr_act, msg);
	checkMessage(rr_status.thr_power, msg);	checkMessage(rr_status.thr_enable, msg);	checkMessage(rr_status.thr_ref, msg);	checkMessage(rr_status.thr_act, msg);

	checkMessage(fl_status.azm_power, msg);	checkMessage(fl_status.azm_enable, msg);	checkMessage(fl_status.azm_ref, msg);	checkMessage(fl_status.azm_act, msg);
	checkMessage(fr_status.azm_power, msg);	checkMessage(fr_status.azm_enable, msg);	checkMessage(fr_status.azm_ref, msg);	checkMessage(fr_status.azm_act, msg);
	checkMessage(rl_status.azm_power, msg);	checkMessage(rl_status.azm_enable, msg);	checkMessage(rl_status.azm_ref, msg);	checkMessage(rl_status.azm_act, msg);
	checkMessage(rr_status.azm_power, msg);	checkMessage(rr_status.azm_enable, msg);	checkMessage(rr_status.azm_ref, msg);	checkMessage(rr_status.azm_act, msg);

	
	checkMessage(thrust_mapping_status.command, msg);
	checkMessage(thrust_mapping_status.status, msg);

	checkMessage(gps_ahrs_status.latitude, msg);
	checkMessage(gps_ahrs_status.longitude, msg);

	checkMessage(echo_status.altitude, msg);

	checkMessage(ublox_status.latitude, msg);	checkMessage(ublox_status.longitude, msg);
	checkMessage(ublox_status.x, msg);			checkMessage(ublox_status.y, msg);

	time_access.set(clock);
	ngc_access.set(ngc);
	gps_ahrs_access.set(gps_ahrs_status);
	echo_access.set(echo_status);
	ublox_access.set(ublox_status);
	FL_motor_access.set(fl_status);
	FR_motor_access.set(fr_status);
	RL_motor_access.set(rl_status);
	RR_motor_access.set(rr_status);
	thrust_mapping_access.set(thrust_mapping_status);
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
	
}