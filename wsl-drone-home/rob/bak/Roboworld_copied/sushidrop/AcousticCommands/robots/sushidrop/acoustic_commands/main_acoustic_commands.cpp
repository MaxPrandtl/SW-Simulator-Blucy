
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
#include "../../../generic/define.h"
#include "../../../generic/Variable.h"

#include "../../../data/Time_status.h"
#include "../../../data/USBL_status.h"
#include "../../../data/IO_europe_status.h"
#include "../../../data/NGC_status.h"

#include "../../../data/Task_execution.h"

#include "../../../generic/commands_def.h"




#define PROCESS_NAME "ACOUSTIC_COMMANDS"

#define COMMAND_QOS 0


bool running = true;
struct 	mosquitto* mosq = NULL;


char broker_address[BUF_SIZE];
int broker_port;
int keep_alive_time;
char mqtt_client[BUF_SIZE];

char environment[BUF_SIZE];
char robot[BUF_SIZE];



USBL_status pinger;
Time_status clk;
IO_europe_status io;
NGC_status ngc;

IntVariable ang_sensor,ang_measure_select;
IntVariable hor_vel_sensor, hor_vel_measure_select;
IntVariable hor_pos_sensor, hor_pos_measure_select;
IntVariable ver_vel_sensor, ver_vel_measure_select;
IntVariable ver_pos_sensor, ver_pos_measure_select;
IntVariable alt_sensor, alt_measure_select;

Task_execution ang_filter, hor_vel_filter, hor_pos_filter, ver_vel_filter, ver_pos_filter, alt_filter;


void parse_topic_file(char* logic_name, char* topic_name);
void execute();
void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg);
void publishMessage(Variable& dv, int qos = 0);


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


	

	while (!f.eof())
	{
		getline(f, line);

		if (line.length() != 0 && line[0] != '#')
		{
			sscanf(line.c_str(), "%s %s\n", s1, s2);
			parse_topic_file(s1, s2);
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
	
	mosquitto_subscribe(mosq, NULL, pinger.msgRecv.topic_name, 0);
	
	mosquitto_subscribe(mosq, NULL, clk.timeStamp.topic_name, 0);
	// End register topic subscriptions

	
	

	mosquitto_message_callback_set(mosq, on_message_recv_callback);
	
	return 0;
}



void parse_topic_file(char* logic_name, char* topic_name)
{
	if (strcmp(logic_name, "Broker-address:") == 0) strcpy(broker_address, topic_name);
	if (strcmp(logic_name, "Broker-port:") == 0) sscanf(topic_name, "%d", &broker_port);
	if (strcmp(logic_name, "Keep-alive-time:") == 0) sscanf(topic_name, "%d", &keep_alive_time);

	if (strcmp(logic_name, "Environment:") == 0)		strcpy(environment, topic_name);	if (environment[0] == '/') environment[0] = '\0';
	if (strcmp(logic_name, "Robot:") == 0)				strcpy(robot, topic_name);			if (robot[0] == '/') robot[0] = '\0';

	
	if (strcmp(logic_name, "PINGER-message-recv:") == 0)		sprintf(pinger.msgRecv.topic_name, "%s%s%s", environment, robot, topic_name);


	if (strcmp(logic_name, "timeStamp:") == 0)					sprintf(clk.timeStamp.topic_name, "%s%s%s", environment, robot, topic_name);


	if (strcmp(logic_name, "VFL-motor-power-command:") == 0)    sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_VFL_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "VFR-motor-power-command:") == 0)    sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_VFR_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "VRL-motor-power-command:") == 0)    sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_VRL_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "VRR-motor-power-command:") == 0)    sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_VRR_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "HFL-motor-power-command:") == 0)    sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_HFL_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "HFR-motor-power-command:") == 0)    sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_HFR_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "HRL-motor-power-command:") == 0)    sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_HRL_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "HRR-motor-power-command:") == 0)    sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_HRR_POWER].topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "VFL-motor-enable-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_VFL_ENABLE].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "VFR-motor-enable-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_VFR_ENABLE].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "VRL-motor-enable-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_VRL_ENABLE].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "VRR-motor-enable-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_VRR_ENABLE].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "HFL-motor-enable-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_HFL_ENABLE].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "HFR-motor-enable-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_HFR_ENABLE].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "HRL-motor-enable-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_HRL_ENABLE].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "HRR-motor-enable-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_MOTOR_HRR_ENABLE].topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "DVL-power-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_DVL_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "PA500-power-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_PA500_1_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ECHOLOGGER-power-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_PA500_2_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "PINGER-power-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_EVOLOGICS_MODEM_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "Light-FU-power-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_LIGHTS_UP_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "Light-FB-power-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_LIGHTS_MIDDLE_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "Light-CB-power-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_LIGHTS_DOWN_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "POE-power-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_POE_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "CPU2-power-command:") == 0)   sprintf(io.digitalCommand[EUROPE_DIO_CPU_2_POWER].topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "motors-auto-start-stop-command:") == 0)    sprintf(io.autoStartStop_command.topic_name, "%s%s%s", environment, robot, topic_name);



	if (strcmp(logic_name, "ngc-force-fu-man:") == 0)			sprintf(ngc.force.fu.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fv-man:") == 0)			sprintf(ngc.force.fv.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fw-man:") == 0)			sprintf(ngc.force.fw.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-tr-man:") == 0)			sprintf(ngc.force.tr.manual.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-velocity-body-u-man:") == 0)	sprintf(ngc.velocityBody.u.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-v-man:") == 0)	sprintf(ngc.velocityBody.v.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-w-man:") == 0)	sprintf(ngc.velocityBody.w.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-r-man:") == 0)	sprintf(ngc.velocityBody.r.manual.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-velocity-inertial-speed-man:") == 0)	sprintf(ngc.velocityInertial.speed.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-inertial-course") == 0)		sprintf(ngc.velocityInertial.course.manual.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-pose-x-man:") == 0)		sprintf(ngc.pose.x.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-y-man:") == 0)		sprintf(ngc.pose.y.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-z-man:") == 0)		sprintf(ngc.pose.z.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-a-man:") == 0)		sprintf(ngc.pose.a.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-psi-man:") == 0)	sprintf(ngc.pose.psi.manual.topic_name, "%s%s%s", environment, robot, topic_name);



	if (strcmp(logic_name, "current-ang-sensor:") == 0)			sprintf(ang_sensor.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-ang-measure:") == 0)		sprintf(ang_measure_select.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-hor-vel-sensor:") == 0)		sprintf(hor_vel_sensor.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-hor-vel-measure:") == 0)	sprintf(hor_vel_measure_select.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-hor-pos-sensor:") == 0)		sprintf(hor_pos_sensor.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-hor-pos-measure:") == 0)	sprintf(hor_pos_measure_select.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-ver-vel-sensor:") == 0)		sprintf(ver_vel_sensor.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-ver-vel-measure:") == 0)	sprintf(ver_vel_measure_select.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-ver-pos-sensor:") == 0)		sprintf(ver_pos_sensor.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-ver-pos-measure:") == 0)	sprintf(ver_pos_measure_select.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-alt-sensor:") == 0)			sprintf(alt_sensor.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "current-alt-measure:") == 0)		sprintf(alt_measure_select.topic_name, "%s%s%s", environment, robot, topic_name);


	if (strcmp(logic_name, "filter-ang-execution-command:") == 0)		sprintf(ang_filter.command.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "filter-hor-vel-execution-command:") == 0)	sprintf(hor_vel_filter.command.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "filter-hor-pos-execution-command:") == 0)	sprintf(hor_pos_filter.command.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "filter-ver-vel-execution-command:") == 0)	sprintf(ver_vel_filter.command.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "filter-ver-pos-execution-command:") == 0)	sprintf(ver_pos_filter.command.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "filter-alt-execution-command:") == 0)		sprintf(alt_filter.command.topic_name, "%s%s%s", environment, robot, topic_name);
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


	
	
	int loop_ret;
	while (running)
	{
		loop_ret = mqtt_loop();
		if (loop_ret == 0) execute();
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



void publishMessage(Variable& dv,int qos)
{
	char message[BUF_SIZE];
	if (dv.updated == 1)
	{
		dv.toString(message);
		mosquitto_publish(mosq, NULL, dv.topic_name, strlen(message), message, qos, false);
		dv.updated = 0;
	}
}




void parse_command(std::vector<std::string>& cmd_msg, const char* cmd_string)
{
	int i = 0;
	char s[BUF_SIZE];
	std::string str;

	int k = 0;
	do
	{
		if (cmd_string[i] == ' ' || cmd_string[i] == NULL)
		{
			if (k > 0)
			{
				s[k] = NULL;
				str = s;
				cmd_msg.push_back(str);
				k = 0;
			}
		}
		else
		{
			s[k] = cmd_string[i];
			k++;
		}
	} while (cmd_string[i++] != NULL);

}



void exec_io_cmd(std::vector<std::string>& cmd_msg)
{
	IO_cmd io_cmd;
	int index, int_value;
	double double_value;

	sscanf(cmd_msg[1].c_str(), "%d", &io_cmd);


	switch (io_cmd)
	{
	case SET_DIGITAL: sscanf(cmd_msg[2].c_str(), "%d", &index);
					  sscanf(cmd_msg[3].c_str(), "%d", &int_value);
					  io.digitalCommand[index].value = int_value;
					  io.digitalCommand[index].valid = 1;
				      io.digitalCommand[index].updated = 1;
					  io.digitalCommand[index].timeStamp = clk.timeStamp.value;

					  publishMessage(io.digitalCommand[index]);

					  break;


	case SET_ANALOG: sscanf(cmd_msg[2].c_str(), "%d", &index);
					 sscanf(cmd_msg[3].c_str(), "%lf", &double_value);
					 io.analogCommand[index].value = double_value;
					 io.analogCommand[index].valid = 1;
					 io.analogCommand[index].updated = 1;
					 io.analogCommand[index].timeStamp = clk.timeStamp.value;

					 publishMessage(io.analogCommand[index]);

					 break;


	case AUTO_START: io.autoStartStop_command.value = 1;
					 io.autoStartStop_command.valid = 1;
					 io.autoStartStop_command.updated = 1;
					 io.autoStartStop_command.timeStamp = clk.timeStamp.value;

					 publishMessage(io.autoStartStop_command);

					 break;


	case AUTO_STOP:  io.autoStartStop_command.value = 2;
					 io.autoStartStop_command.valid = 1;
					 io.autoStartStop_command.updated = 1;
					 io.autoStartStop_command.timeStamp = clk.timeStamp.value;

					 publishMessage(io.autoStartStop_command);

					 break;
	}

	
}




void exec_ngc_cmd(std::vector<std::string>& cmd_msg)
{
	int cmd_type;
	double value;

	sscanf(cmd_msg[1].c_str(), "%d", &(cmd_type));
	sscanf(cmd_msg[2].c_str(), "%lf", &(value));



	switch (cmd_type)
	{
	case SET_FU_REF: ngc.force.fu.manual.value = value;   ngc.force.fu.manual.valid = 1;   ngc.force.fu.manual.updated = 1;	ngc.force.fu.manual.timeStamp = clk.timeStamp.value;
					 publishMessage(ngc.force.fu.manual);
					 break;

	case SET_FV_REF: ngc.force.fv.manual.value = value;   ngc.force.fv.manual.valid = 1;   ngc.force.fv.manual.updated = 1;	ngc.force.fv.manual.timeStamp = clk.timeStamp.value;
				 	 publishMessage(ngc.force.fv.manual);
					 break;

	case SET_FW_REF: ngc.force.fw.manual.value = value;   ngc.force.fw.manual.valid = 1;   ngc.force.fw.manual.updated = 1;	ngc.force.fw.manual.timeStamp = clk.timeStamp.value;
					 publishMessage(ngc.force.fw.manual);
					 break;

	case SET_TR_REF: ngc.force.tr.manual.value = value;   ngc.force.tr.manual.valid = 1;   ngc.force.tr.manual.updated = 1;	ngc.force.tr.manual.timeStamp = clk.timeStamp.value;
					 publishMessage(ngc.force.tr.manual);
					 break;



	case SET_U_REF: ngc.velocityBody.u.manual.value = value;   ngc.velocityBody.u.manual.valid = 1;   ngc.velocityBody.u.manual.updated = 1;	ngc.velocityBody.u.manual.timeStamp = clk.timeStamp.value;
					publishMessage(ngc.velocityBody.u.manual);
					break;

	case SET_V_REF: ngc.velocityBody.v.manual.value = value;   ngc.velocityBody.v.manual.valid = 1;   ngc.velocityBody.v.manual.updated = 1;	ngc.velocityBody.v.manual.timeStamp = clk.timeStamp.value;
					publishMessage(ngc.velocityBody.v.manual);
					break;

	case SET_W_REF: ngc.velocityBody.w.manual.value = value;   ngc.velocityBody.w.manual.valid = 1;   ngc.velocityBody.w.manual.updated = 1;	ngc.velocityBody.w.manual.timeStamp = clk.timeStamp.value;
					publishMessage(ngc.velocityBody.w.manual);
					break;

	case SET_R_REF: ngc.velocityBody.r.manual.value = value;   ngc.velocityBody.r.manual.valid = 1;   ngc.velocityBody.r.manual.updated = 1;	ngc.velocityBody.r.manual.timeStamp = clk.timeStamp.value;
					publishMessage(ngc.velocityBody.r.manual);
					break;



	case SET_SPEED_REF: ngc.velocityInertial.speed.manual.value = value;   ngc.velocityInertial.speed.manual.valid = 1;   ngc.velocityInertial.speed.manual.updated = 1;	ngc.velocityInertial.speed.manual.timeStamp = clk.timeStamp.value;
						publishMessage(ngc.velocityInertial.speed.manual);
						break;

	case SET_COURSE_REF: ngc.velocityInertial.course.manual.value = value;   ngc.velocityInertial.course.manual.valid = 1;   ngc.velocityInertial.course.manual.updated = 1;	ngc.velocityInertial.course.manual.timeStamp = clk.timeStamp.value;
						 publishMessage(ngc.velocityInertial.course.manual);
						 break;



	case SET_X_REF: ngc.pose.x.manual.value = value;   ngc.pose.x.manual.valid = 1;   ngc.pose.x.manual.updated = 1;	ngc.pose.x.manual.timeStamp = clk.timeStamp.value;
					publishMessage(ngc.pose.x.manual);
					break;

	case SET_Y_REF: ngc.pose.y.manual.value = value;   ngc.pose.y.manual.valid = 1;   ngc.pose.y.manual.updated = 1;	ngc.pose.y.manual.timeStamp = clk.timeStamp.value;
					publishMessage(ngc.pose.y.manual);
					break;

	case SET_Z_REF: ngc.pose.z.manual.value = value;   ngc.pose.z.manual.valid = 1;   ngc.pose.z.manual.updated = 1;	ngc.pose.z.manual.timeStamp = clk.timeStamp.value;
					publishMessage(ngc.pose.z.manual);
					break;

	case SET_ALT_REF: ngc.pose.a.manual.value = value;   ngc.pose.a.manual.valid = 1;   ngc.pose.a.manual.updated = 1;	ngc.pose.a.manual.timeStamp = clk.timeStamp.value;
					  publishMessage(ngc.pose.a.manual);
					  break;

	case SET_PSI_REF: ngc.pose.psi.manual.value = value;   ngc.pose.psi.manual.valid = 1;   ngc.pose.psi.manual.updated = 1;	ngc.pose.psi.manual.timeStamp = clk.timeStamp.value;
					  publishMessage(ngc.pose.psi.manual);
					  break;


	}

}



void exec_task_cmd(std::vector<std::string>& cmd_msg)
{
	int cmd_type, value, task_code;
	double param;
	sscanf(cmd_msg[1].c_str(), "%d", &(cmd_type));
	sscanf(cmd_msg[2].c_str(), "%d", &(value));
	sscanf(cmd_msg[3].c_str(), "%d", &(task_code));
	sscanf(cmd_msg[4].c_str(), "%lf", &(param));



	switch (cmd_type)
	{
	case SET_ANG_RAW_FILTER:		ang_measure_select.value = value;	ang_measure_select.valid = 1;	ang_measure_select.updated = 1;	 ang_measure_select.timeStamp = clk.timeStamp.value;
									publishMessage(ang_measure_select);
									break;

	case SET_HOR_VEL_RAW_FILTER:	hor_vel_measure_select.value = value;	hor_vel_measure_select.valid = 1;	hor_vel_measure_select.updated = 1;	 hor_vel_measure_select.timeStamp = clk.timeStamp.value;
									publishMessage(hor_vel_measure_select);
									break;


	case SET_HOR_POS_RAW_FILTER:	hor_pos_measure_select.value = value;	hor_pos_measure_select.valid = 1;	hor_pos_measure_select.updated = 1;	 hor_pos_measure_select.timeStamp = clk.timeStamp.value;
									publishMessage(hor_pos_measure_select);
									break;


	case SET_VER_VEL_RAW_FILTER:	ver_vel_measure_select.value = value;	ver_vel_measure_select.valid = 1;	ver_vel_measure_select.updated = 1;	 ver_vel_measure_select.timeStamp = clk.timeStamp.value;
									publishMessage(ver_vel_measure_select);
									break;


	case SET_VER_POS_RAW_FILTER:	ver_pos_measure_select.value = value;	ver_pos_measure_select.valid = 1;	ver_pos_measure_select.updated = 1;	 ver_pos_measure_select.timeStamp = clk.timeStamp.value;
									publishMessage(ver_pos_measure_select);
									break;


	case SET_ALT_RAW_FILTER:		alt_measure_select.value = value;	alt_measure_select.valid = 1;	alt_measure_select.updated = 1;	 alt_measure_select.timeStamp = clk.timeStamp.value;
									publishMessage(alt_measure_select);
									break;



	case INIT_ANG_FILTER:			ang_filter.command.value=value;		ang_filter.command.valid = 1;	ang_filter.command.updated = 1;	 ang_filter.command.timeStamp = clk.timeStamp.value;
									publishMessage(ang_filter.command);
									break;

	case INIT_HOR_VEL_FILTER:		hor_vel_filter.command.value = value;	hor_vel_filter.command.valid = 1;	hor_vel_filter.command.updated = 1;	 hor_vel_filter.command.timeStamp = clk.timeStamp.value;
									publishMessage(hor_vel_filter.command);
									break;

	case INIT_HOR_POS_FILTER:		hor_pos_filter.command.value = value;	hor_pos_filter.command.valid = 1;	hor_pos_filter.command.updated = 1;	 hor_pos_filter.command.timeStamp = clk.timeStamp.value;
									publishMessage(hor_pos_filter.command);
									break;

	case INIT_VER_VEL_FILTER:		ver_vel_filter.command.value = value;	ver_vel_filter.command.valid = 1;	ver_vel_filter.command.updated = 1;	 ver_vel_filter.command.timeStamp = clk.timeStamp.value;
									publishMessage(ver_vel_filter.command);
									break;

	case INIT_VER_POS_FILTER:		ver_pos_filter.command.value = value;	ver_pos_filter.command.valid = 1;	ver_pos_filter.command.updated = 1;	 ver_pos_filter.command.timeStamp = clk.timeStamp.value;
									publishMessage(ver_pos_filter.command);
									break;

	case INIT_ALT_FILTER:			alt_filter.command.value = value;		alt_filter.command.valid = 1;	alt_filter.command.updated = 1;	 alt_filter.command.timeStamp = clk.timeStamp.value;
									publishMessage(alt_filter.command);
									break;





	case SET_ANG_SENSOR:			ang_sensor.value=value;		ang_sensor.valid = 1;	ang_sensor.updated = 1;	 ang_sensor.timeStamp = clk.timeStamp.value;
									publishMessage(ang_sensor);
									break;

	case SET_HOR_VEL_SENSOR:		hor_vel_sensor.value = value;	hor_vel_sensor.valid = 1;	hor_vel_sensor.updated = 1;	 hor_vel_sensor.timeStamp = clk.timeStamp.value;
									publishMessage(hor_vel_sensor);
									break;

	case SET_HOR_POS_SENSOR:		hor_pos_sensor.value = value;	hor_pos_sensor.valid = 1;	hor_pos_sensor.updated = 1;	 hor_pos_sensor.timeStamp = clk.timeStamp.value;
									publishMessage(hor_pos_sensor);
									break;

	case SET_VER_VEL_SENSOR:		ver_vel_sensor.value = value;	ver_vel_sensor.valid = 1;	ver_vel_sensor.updated = 1;	 ver_vel_sensor.timeStamp = clk.timeStamp.value;
									publishMessage(ver_vel_sensor);
									break;

	case SET_VER_POS_SENSOR:		ver_pos_sensor.value = value;	ver_pos_sensor.valid = 1;	ver_pos_sensor.updated = 1;	 ver_pos_sensor.timeStamp = clk.timeStamp.value;
									publishMessage(ver_pos_sensor);
									break;

	case SET_ALT_SENSOR:			alt_sensor.value = value;		alt_sensor.valid = 1;	alt_sensor.updated = 1;	 alt_sensor.timeStamp = clk.timeStamp.value;
									publishMessage(alt_sensor);
									break;
	}
}



void exec_command(std::vector<std::string> &cmd_msg)
{
	int system_cmd;
	sscanf(cmd_msg[0].c_str(), "%d", &system_cmd);

	switch (system_cmd)
	{
		case IO_SYSTEM:		exec_io_cmd(cmd_msg); break;
		case NGC_SYSTEM:	exec_ngc_cmd(cmd_msg); break;
		case TASK_SYSTEM:	exec_task_cmd(cmd_msg); break;
	};
}




void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
	std::vector<std::string> cmd_msg;

	checkMessage(clk.timeStamp, msg);

	if (checkMessage(pinger.msgRecv, msg)) { parse_command(cmd_msg, pinger.msgRecv.value.c_str()); exec_command(cmd_msg); }
}




void execute()
{

	/*
	USBL_status usbl;
	usbl = usbl_access.get();

	publishMessage_ground(usbl.x);
	publishMessage_ground(usbl.y);
	publishMessage_ground(usbl.z);
	publishMessage_ground(usbl.msgRecv);
	publishMessage_ground(usbl.device_status);


	publishMessage(usbl.fromUSBL,2);  if (usbl.comm_status.value == SENDING) usbl.comm_status.value = SENT;

	//printf("***publish  %s \n", usbl.toPinger.value);

	usbl_access.set(usbl);

	*/
}