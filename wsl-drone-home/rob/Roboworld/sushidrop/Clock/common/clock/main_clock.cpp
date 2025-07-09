
#define __MAIN__

#include <stdio.h>
#include <string.h>
#include <stdlib.h>  
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include "mosquitto.h"
#include "../../generic/define.h"
#include "../../data/DataAccess.h"
#include "../../data/Time_status.h"
#include "Clock.h"
#include <fstream>


#define PROCESS_NAME "clock"


bool running = true;
struct 	mosquitto* mosq = NULL;

char broker_address[BUF_SIZE];
int broker_port;
int keep_alive_time;
char mqtt_client[BUF_SIZE];

char environment[BUF_SIZE];
char robot[BUF_SIZE];


DataAccess<Time_status> time_access;
RobotThread* clock_thread = NULL;


void execute();
void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg);
void parse_topic_file(char* logic_name, char* topic_name, Time_status& clock);

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

	

	while (!f.eof())
	{
		getline(f, line);

		if (line.length() != 0 && line[0] != '#')
		{
			sscanf(line.c_str(), "%s %s\n", s1, s2);
			parse_topic_file(s1, s2,clock);
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

	// Register topic subscriptions
	
	// End register topic subscriptions

	//mosquitto_message_callback_set(mosq, on_message_recv_callback);
	return 0;
}


void parse_topic_file(char* logic_name, char* topic_name, Time_status& clock)
{
	if (strcmp(logic_name, "Broker-address:") == 0)		strcpy(broker_address, topic_name);
	if (strcmp(logic_name, "Broker-port:") == 0)		sscanf(topic_name, "%d", &broker_port);
	if (strcmp(logic_name, "Keep-alive-time:") == 0)	sscanf(topic_name, "%d", &keep_alive_time);

	if (strcmp(logic_name, "Environment:") == 0)		strcpy(environment, topic_name);	if (environment[0] == '/') environment[0] = '\0';
	if (strcmp(logic_name, "Robot:") == 0)				strcpy(robot, topic_name);			if (robot[0] == '/') robot[0] = '\0';

	
	if (strcmp(logic_name, "timeStamp:") == 0)			sprintf(clock.timeStamp.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "dt:") == 0)					sprintf(clock.dt.topic_name, "%s%s%s", environment, robot, topic_name);
	if (strcmp(logic_name, "date-time:") == 0)			sprintf(clock.date_time.topic_name, "%s%s%s", environment, robot, topic_name);
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

	
	if (init_mqtt() != 0) return -1;
	

	clock_thread = new Clock(PROCESS_NAME,&time_access);
	clock_thread->create();


	while (running)
	{
		execute();
		mqtt_loop();
	}

	clock_thread->terminate();

	mosquitto_disconnect(mosq);
	mosquitto_destroy(mosq);
	mosquitto_lib_cleanup();

	return 0;
}


void on_message_recv_callback(struct mosquitto* mosq, void* obj, const struct mosquitto_message* msg)
{
	
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
	Time_status st;
	st = time_access.get();

	if (st.updated)
	{
		publishMessage(st.timeStamp);
		publishMessage(st.dt);
		publishMessage(st.date_time);

		//printf("%s\t|\t%s\t|\t%s\n", st.str.timeStamp, st.str.date_time, st.str.dt);

		//printf("%lld\n", st.timeStamp.value);
		
		st.updated = false;
		time_access.set(st);
	}
}