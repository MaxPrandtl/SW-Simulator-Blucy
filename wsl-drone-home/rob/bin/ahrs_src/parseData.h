#pragma once

#include "mscl/mscl.h"
#include <iostream>
#include <string>
#include <chrono>
#include <thread>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <csignal>
#include <mosquitto.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <map>
#include <sstream>
#include <vector>

#include <memory>
#include <stdexcept>
#include <array>

#define MQTT_BROKER_IP "127.0.0.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_QOS 0
#define MQTT_TOPIC_PREFIX "us_imu"
#define MQTT_TIMESTAMP_TOPIC "clock/timestamp"

#define BROADCAST_IP "255.255.255.255"
#define BROADCAST_PORT "50001"

#define DEBUG false

/*

*/

struct mosquitto *mosq = NULL;

string ts = "0";
std::map<string, string> current = {};

int sock;                         /* Socket */
struct sockaddr_in broadcastAddr; /* Broadcast address */
string broadcastIP;               /* IP broadcast address */
unsigned short broadcastPort;     /* Server port */
char *sendString;                 /* String to broadcast */
int broadcastPermission;          /* Socket opt to set permission to broadcast */
unsigned int sendStringLen;       /* Length of string to broadcast */


// MQTT Publish
static void publish_to_mqtt(const string &topic, const string &payload, const int qos)
{
    mosquitto_publish(mosq, NULL, topic.c_str(), payload.size(), payload.c_str(), qos, false);
}

// MQTT Callbacks
static void on_connect(struct mosquitto *mosq, void *obj, int rc)
{
    cout << "mqtt connect " << obj << "::" << rc << endl;
}

void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg)
{
    string pl = (char *)msg->payload;
    ts = pl.substr(0, pl.find(" "));
    if(DEBUG)
        cout << "message: " << ts;
}

// Parse Data
//   Shows how to parse incoming data from an Inertial Device.
static void parseData(mscl::InertialNode &node)
{
    broadcastIP = "255.255.255.255";
    broadcastPort = atoi(BROADCAST_PORT);

    /* Create socket for sending/receiving datagrams */
    if ((sock = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) < 0)
        cout << "socket() failed";

    /* Set socket to allow broadcast */
    broadcastPermission = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void *)&broadcastPermission,
                   sizeof(broadcastPermission)) < 0)
        cout << "setsockopt() failed";

    /* Construct local address structure */
    memset(&broadcastAddr, 0, sizeof(broadcastAddr));               /* Zero out structure */
    broadcastAddr.sin_family = AF_INET;                             /* Internet address family */
    broadcastAddr.sin_addr.s_addr = inet_addr(broadcastIP.c_str()); /* Broadcast IP address */
    broadcastAddr.sin_port = htons(broadcastPort);                  /* Broadcast port */

    // Initialize Mosquitto library
    mosquitto_lib_init();
    
    // Create Mosquitto client instance
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq)
    {
        cerr << "Failed to create Mosquitto client instance" << endl;
        exit(EXIT_FAILURE);
    }

    // Set Callbacks for mosquitto
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);
    // Set mosquitto as threaded
    mosquitto_threaded_set(mosq, true);

    // Connect to mosquitto
    int rc = mosquitto_connect(mosq, MQTT_BROKER_IP, MQTT_BROKER_PORT, 0);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        cerr << "Failed to connect to MQTT broker: " << mosquitto_strerror(rc) << endl;
        exit(EXIT_FAILURE);
    }

    // Subscribe to timestamp topic
    rc = mosquitto_subscribe(mosq, NULL, "clock/timestamp", MQTT_QOS);
    if (rc != MOSQ_ERR_SUCCESS)
    {
        cerr << "Failed to subscribe to timestamp topic: " << mosquitto_strerror(rc) << endl;
        exit(EXIT_FAILURE);
    }

    // endless loop of reading in data
    while (true)
    {
        // get all the data packets from the node, with a timeout of 500 milliseconds
        mscl::MipDataPackets packets = node.getDataPackets(500);

        for (mscl::MipDataPacket packet : packets)
        {
            // get data from mosquitto (not using blocking loop (mosquitto_loop_start) as it
            // would block the system and halt all operations. As it is threaded, the calls
            // to "on_message" are in different threads and non blocking.)
            mosquitto_loop(mosq, 10, 10000);
            
            // get the data in the packet
            mscl::MipDataPoints data = packet.data();
            mscl::MipDataPoint dataPoint;

            // loop through all the data points in the packet
            for (unsigned int itr = 0; itr < data.size(); itr++)
            {
                dataPoint = data[itr];
                // prepare mqtt topic
                string topic = MQTT_TOPIC_PREFIX;
                topic = topic.append("/");
                if (dataPoint.channelName().rfind("est", 0) != std::string::npos)
                    topic = topic.append("Estimator/");
                if (dataPoint.channelName().rfind("svi", 0) != std::string::npos)
                    topic = topic.append("Svi/");
                topic = topic.append(dataPoint.channelName());
                
                // calculate QoS
                int qos = dataPoint.valid() ? 1 : 0;

                // convert datapoint to string
                current[dataPoint.channelName()] = dataPoint.as_string();

                // prepare blucy mqtt payload
                string payload = dataPoint.as_string() + " " + ts + " " + (qos == 1 ? "1" : "0");

                // publish to mqtt
                publish_to_mqtt(topic, payload, 0);
            }
        }
    }
}
