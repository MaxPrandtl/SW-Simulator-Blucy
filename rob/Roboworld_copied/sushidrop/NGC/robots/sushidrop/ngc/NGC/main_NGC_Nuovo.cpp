
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
#include "../../../../data/NGC_status.h"
#include "../../../../data/Time_status.h"
#include "../../../../data/Task_execution.h"



#define PROCESS_NAME "NGC"

#define COMMAND_QOS 0


bool running = true;
struct 	mosquitto* mosq = NULL;

char broker_address[BUF_SIZE];
int broker_port;
int keep_alive_time;
char mqtt_client[BUF_SIZE];

char environment[BUF_SIZE];
char robot[BUF_SIZE];


NGC_status ngc;
Time_status clk;
Task_execution control_angvel;
Task_execution control_angpos;
Task_execution control_horvel;
Task_execution control_horpos;
Task_execution control_vervel;
Task_execution control_verpos;
Task_execution control_alt;
Task_execution pf_ua;

void publishMessage(Variable& dv);
void parse_topic_file(char* logic_name, char* topic_name);
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
	mosquitto_subscribe(mosq, NULL, clk.timeStamp.topic_name, 0);


	// subscript to simulation topics
	mosquitto_subscribe(mosq, NULL, ngc.force.fu.manual.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.force.fv.manual.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.force.fw.manual.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.force.tr.manual.topic_name, COMMAND_QOS);

	mosquitto_subscribe(mosq, NULL, ngc.force.fu.automatic.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.force.fv.automatic.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.force.fw.automatic.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.force.tr.automatic.topic_name, COMMAND_QOS);

	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.u.manual.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.v.manual.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.w.manual.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.r.manual.topic_name, COMMAND_QOS);

	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.u.automatic.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.v.automatic.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.w.automatic.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.velocityBody.r.automatic.topic_name, COMMAND_QOS);

	mosquitto_subscribe(mosq, NULL, ngc.pose.x.manual.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.pose.y.manual.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.pose.z.manual.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.pose.a.manual.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, ngc.pose.psi.manual.topic_name, COMMAND_QOS);

	mosquitto_subscribe(mosq, NULL, ngc.pose.psi.automatic.topic_name, COMMAND_QOS);


	mosquitto_subscribe(mosq, NULL, control_angvel.status.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, control_angpos.status.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, control_horvel.status.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, control_horpos.status.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, control_vervel.status.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, control_verpos.status.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, control_alt.status.topic_name, COMMAND_QOS);
	mosquitto_subscribe(mosq, NULL, pf_ua.status.topic_name, COMMAND_QOS);
	
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
	
	
	if (strcmp(logic_name, "timeStamp:") == 0)					sprintf(clk.timeStamp.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-force-fu-man:") == 0)			sprintf(ngc.force.fu.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fv-man:") == 0)			sprintf(ngc.force.fv.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fw-man:") == 0)			sprintf(ngc.force.fw.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-tr-man:") == 0)			sprintf(ngc.force.tr.manual.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-force-fu-auto:") == 0)			sprintf(ngc.force.fu.automatic.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fv-auto:") == 0)			sprintf(ngc.force.fv.automatic.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fw-auto:") == 0)			sprintf(ngc.force.fw.automatic.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-tr-auto:") == 0)			sprintf(ngc.force.tr.automatic.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-force-fu-ref:") == 0)			sprintf(ngc.force.fu.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fv-ref:") == 0)			sprintf(ngc.force.fv.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-fw-ref:") == 0)			sprintf(ngc.force.fw.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-force-tr-ref:") == 0)			sprintf(ngc.force.tr.reference.topic_name, "%s%s%s", environment, robot, topic_name);



	if (strcmp(logic_name, "ngc-velocity-body-u-man:") == 0)	sprintf(ngc.velocityBody.u.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-v-man:") == 0)	sprintf(ngc.velocityBody.v.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-w-man:") == 0)	sprintf(ngc.velocityBody.w.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-r-man:") == 0)	sprintf(ngc.velocityBody.r.manual.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-velocity-body-u-auto:") == 0)	sprintf(ngc.velocityBody.u.automatic.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-v-auto:") == 0)	sprintf(ngc.velocityBody.v.automatic.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-w-auto:") == 0)	sprintf(ngc.velocityBody.w.automatic.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-r-auto:") == 0)	sprintf(ngc.velocityBody.r.automatic.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-velocity-body-u-ref:") == 0)	sprintf(ngc.velocityBody.u.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-v-ref:") == 0)	sprintf(ngc.velocityBody.v.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-w-ref:") == 0)	sprintf(ngc.velocityBody.w.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-velocity-body-r-ref:") == 0)	sprintf(ngc.velocityBody.r.reference.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-pose-x-man:") == 0)				sprintf(ngc.pose.x.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-y-man:") == 0)				sprintf(ngc.pose.y.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-z-man:") == 0)				sprintf(ngc.pose.z.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-a-man:") == 0)				sprintf(ngc.pose.a.manual.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-psi-man:") == 0)			sprintf(ngc.pose.psi.manual.topic_name, "%s%s%s", environment, robot, topic_name);

	if (strcmp(logic_name, "ngc-pose-psi-auto:") == 0)			sprintf(ngc.pose.psi.automatic.topic_name, "%s%s%s", environment, robot, topic_name);
	

	if (strcmp(logic_name, "ngc-pose-x-ref:") == 0)				sprintf(ngc.pose.x.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-y-ref:") == 0)				sprintf(ngc.pose.y.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-z-ref:") == 0)				sprintf(ngc.pose.z.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-a-ref:") == 0)				sprintf(ngc.pose.a.reference.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "ngc-pose-psi-ref:") == 0)			sprintf(ngc.pose.psi.reference.topic_name, "%s%s%s", environment, robot, topic_name);


	if (strcmp(logic_name, "control-ang-vel-execution-status:") == 0)	sprintf(control_angvel.status.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-ang-pos-execution-status:") == 0)	sprintf(control_angpos.status.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-hor-vel-execution-status:") == 0)	sprintf(control_horvel.status.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-hor-pos-execution-status:") == 0)	sprintf(control_horpos.status.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-ver-vel-execution-status:") == 0)	sprintf(control_vervel.status.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-ver-pos-execution-status:") == 0)	sprintf(control_verpos.status.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "control-alt-execution-status:") == 0)		sprintf(control_alt.status.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "path-following-UA-execution-status:") == 0)	sprintf(pf_ua.status.topic_name, "%s%s%s", environment, robot, topic_name);
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


void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
	checkMessage(clk.timeStamp,msg);

	checkMessage(ngc.force.fu.manual, msg);
	checkMessage(ngc.force.fv.manual, msg);
	checkMessage(ngc.force.fw.manual, msg);
	checkMessage(ngc.force.tr.manual, msg);

	checkMessage(ngc.force.fu.automatic, msg);
	checkMessage(ngc.force.fv.automatic, msg);
	checkMessage(ngc.force.fw.automatic, msg);
	checkMessage(ngc.force.tr.automatic, msg);

	checkMessage(ngc.velocityBody.u.manual, msg);
	checkMessage(ngc.velocityBody.v.manual, msg);
	checkMessage(ngc.velocityBody.w.manual, msg);
	checkMessage(ngc.velocityBody.r.manual, msg);

	checkMessage(ngc.velocityBody.u.automatic, msg);
	checkMessage(ngc.velocityBody.v.automatic, msg);
	checkMessage(ngc.velocityBody.w.automatic, msg);
	checkMessage(ngc.velocityBody.r.automatic, msg);

	checkMessage(ngc.pose.psi.manual, msg);
	checkMessage(ngc.pose.x.manual, msg);
	checkMessage(ngc.pose.y.manual, msg);
	checkMessage(ngc.pose.z.manual, msg);
	checkMessage(ngc.pose.a.manual, msg);

	checkMessage(ngc.pose.psi.automatic, msg);
	

	checkMessage(control_angvel.status,msg);
	checkMessage(control_angpos.status, msg);
	checkMessage(control_horvel.status, msg);
	checkMessage(control_horpos.status, msg);
	checkMessage(control_vervel.status, msg);
	checkMessage(control_verpos.status, msg);
	checkMessage(control_alt.status, msg);
	checkMessage(pf_ua.status, msg);
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
	// ANG
	if (control_angpos.status.value == TASK_RUNNING)
	{
		if (pf_ua.status.value == TASK_RUNNING)
		{
			ngc.pose.psi.reference.copyValueFrom(ngc.pose.psi.automatic);
			ngc.pose.psi.automatic.updated = 0;
		}
		else
		{
			ngc.pose.psi.reference.copyValueFrom(ngc.pose.psi.manual);
			ngc.pose.psi.manual.updated = 0;
		}
	}


	if (control_angvel.status.value == TASK_RUNNING)
	{
		if (control_angpos.status.value == TASK_RUNNING)
		{
			ngc.velocityBody.r.reference.copyValueFrom(ngc.velocityBody.r.automatic);
			ngc.velocityBody.r.automatic.updated = 0;
		}
		else
		{
			ngc.velocityBody.r.reference.copyValueFrom(ngc.velocityBody.r.manual);
			ngc.velocityBody.r.manual.updated = 0;
		}
	}
	

	if (control_angpos.status.value == TASK_RUNNING || control_angvel.status.value == TASK_RUNNING)
	{
		ngc.force.tr.reference.copyValueFrom(ngc.force.tr.automatic);
		ngc.force.tr.automatic.updated = 0;
	}
	else
	{
		ngc.force.tr.reference.copyValueFrom(ngc.force.tr.manual);
		ngc.force.tr.manual.updated = 0;
	}


	// HOR
	if (control_horpos.status.value == TASK_RUNNING)
	{
		ngc.pose.x.reference.copyValueFrom(ngc.pose.x.manual);
		ngc.pose.x.manual.updated = 0;

		ngc.pose.y.reference.copyValueFrom(ngc.pose.y.manual);
		ngc.pose.y.manual.updated = 0;
	}


	if (control_horvel.status.value == TASK_RUNNING)
	{
		
		if (control_horpos.status.value == TASK_RUNNING)
		{
			ngc.velocityBody.u.reference.copyValueFrom(ngc.velocityBody.u.automatic);
			ngc.velocityBody.u.automatic.updated = 0;

			ngc.velocityBody.v.reference.copyValueFrom(ngc.velocityBody.v.automatic);
			ngc.velocityBody.v.automatic.updated = 0;
		}
		else
		{
			ngc.velocityBody.u.reference.copyValueFrom(ngc.velocityBody.u.manual);
			ngc.velocityBody.u.manual.updated = 0;
			
			ngc.velocityBody.v.reference.copyValueFrom(ngc.velocityBody.v.manual);
			ngc.velocityBody.v.manual.updated = 0;
		}
	}


	if (control_horpos.status.value == TASK_RUNNING || control_horvel.status.value == TASK_RUNNING)
	{
		ngc.force.fu.reference.copyValueFrom(ngc.force.fu.automatic);
		ngc.force.fu.automatic.updated = 0;

		ngc.force.fv.reference.copyValueFrom(ngc.force.fv.automatic);
		ngc.force.fv.automatic.updated = 0;
	}
	else
	{
		ngc.force.fu.reference.copyValueFrom(ngc.force.fu.manual);
		ngc.force.fu.manual.updated = 0;

		ngc.force.fv.reference.copyValueFrom(ngc.force.fv.manual);
		ngc.force.fv.manual.updated = 0;
	}




	// VER
	if (control_verpos.status.value == TASK_RUNNING)
	{
		ngc.pose.z.reference.copyValueFrom(ngc.pose.z.manual);
		ngc.pose.z.manual.updated = 0;
	}

	if (control_alt.status.value == TASK_RUNNING)
	{
		ngc.pose.a.reference.copyValueFrom(ngc.pose.a.manual);
		ngc.pose.a.manual.updated = 0;
	}


	if (control_vervel.status.value == TASK_RUNNING)
	{

		if (control_verpos.status.value == TASK_RUNNING || control_alt.status.value == TASK_RUNNING)
		{
			ngc.velocityBody.w.reference.copyValueFrom(ngc.velocityBody.w.automatic);
			ngc.velocityBody.w.automatic.updated = 0;
		}
		else
		{
			ngc.velocityBody.w.reference.copyValueFrom(ngc.velocityBody.w.manual);
			ngc.velocityBody.w.manual.updated = 0;
		}
	}


	if (control_verpos.status.value == TASK_RUNNING || control_alt.status.value == TASK_RUNNING || control_vervel.status.value == TASK_RUNNING)
	{
		ngc.force.fw.reference.copyValueFrom(ngc.force.fw.automatic);
		ngc.force.fw.automatic.updated = 0;
	}
	else
	{
		ngc.force.fw.reference.copyValueFrom(ngc.force.fw.manual);
		ngc.force.fw.manual.updated = 0;
	}



	
	
	publishMessage(ngc.force.fu.reference);
	publishMessage(ngc.force.fv.reference);
	publishMessage(ngc.force.fw.reference);
	publishMessage(ngc.force.tr.reference);

	publishMessage(ngc.velocityBody.u.reference);
	publishMessage(ngc.velocityBody.v.reference);
	publishMessage(ngc.velocityBody.w.reference);
	publishMessage(ngc.velocityBody.r.reference);

	publishMessage(ngc.pose.x.reference);
	publishMessage(ngc.pose.y.reference);
	publishMessage(ngc.pose.z.reference);
	publishMessage(ngc.pose.a.reference);
	publishMessage(ngc.pose.psi.reference);
}