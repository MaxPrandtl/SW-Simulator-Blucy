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

#define PROCESS_PRIORITY 80
#define LOOP_PERIOD_MS 10
#define COMM_TIMEOUT_SEC 5
#define SIMULATION_FLAG 0
#define DEBUG_LOGGING_FLAG 0
#define MQTT_BROKER_IP "127.0.0.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_QOS 0
#define MQTT_TOPIC_PREFIX "miniCT_02"
#define MQTT_TIMESTAMP_TOPIC "clock/timestamp"
#define SERIAL_DEVICE_NAME "/dev/ttyS7"
#define SERIAL_BAUD_RATE 19200

using namespace std;

// Global variables
volatile bool running = true;
int fd;
struct mosquitto *mosq = NULL;

int serial_port;

string ts = "0";

// Signal handler for interrupt signal (CTRL-C)
void interrupt_handler(int signum)
{
    running = false;
}

// Set process priority
void set_process_priority(int priority)
{
    struct sched_param sched_params;
    memset(&sched_params, 0, sizeof(sched_params));
    sched_params.sched_priority = priority;
    if (sched_setscheduler(0, SCHED_FIFO, &sched_params) == -1) {
        cerr << "Failed to set process priority" << endl;
        exit(EXIT_FAILURE);
    }
}

// Open serial device
int open_serial_device(const char* device_name, int baud_rate) {
    struct termios options;
    memset(&options, 0, sizeof(options));
    fd = open(device_name, O_RDWR | O_NOCTTY);
    if (fd < 0) {
        cerr << "Failed to open serial device" << endl;
        exit(EXIT_FAILURE);
    }
    if (tcgetattr(fd, &options) != 0) {
        cerr << "Failed to get serial port attributes: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }

    cfsetospeed(&options, baud_rate);
    cfsetispeed(&options, baud_rate);
    //options.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
    //options.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
    //options.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
    //options.c_cflag |= CS8; // 8 bits per byte (most common)
    //options.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
    //options.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)
    //options.c_lflag &= ~ICANON;
    //options.c_lflag &= ~ECHO; // Disable echo
    //options.c_lflag &= ~ECHOE; // Disable erasure
    //options.c_lflag &= ~ECHONL; // Disable new-line echo
    //options.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
    //options.c_iflag = IGNPAR;
    //options.c_iflag &= ~(IXON | IXOFF | IXANY); // disable software flow control
    //options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // raw input
    //options.c_oflag = 0;
    //options.c_oflag &= ~OPOST; // raw output
    //options.c_cc[VMIN] = 0; // read non-blocking
    //options.c_cc[VTIME] = COMM_TIMEOUT_SEC;
    tcflush(fd, TCIFLUSH);
    if (tcsetattr(fd, TCSANOW, &options) != 0) {
        cerr << "Failed to set serial port attributes: " << strerror(errno) << endl;
        exit(EXIT_FAILURE);
    }
    return fd;
}

int serial_port_close(int fd){
    tcflush(fd, TCIOFLUSH); 
    return close(fd);
}
// Read data from serial device
bool read_serial_data(int fd, string& data)
{
    //cout << "reading" << endl;
    char buffer[256];
    int count = read(fd, buffer, sizeof(buffer));
    if (count > 0) {
        //buffer[count] = "\0";
        data += buffer;
        //cout << count << " data: " << buffer << endl;
        return true;
    } else {
        return false;
    }
}

// Publish data to MQTT broker
void publish_to_mqtt(const string& topic, const string& payload)
{
    mosquitto_publish(mosq, NULL, topic.c_str(), payload.size(), payload.c_str(), MQTT_QOS, false);
}

// MQTT Callbacks
void on_connect(struct mosquitto *mosq, void *obj, int rc){
cout << "mqtt connect " << obj << "::" << rc <<endl;
}
void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg){
	string pl = (char *)msg->payload;
    ts = pl.substr(0, pl.find(" "));
}
void on_log(struct mosquitto *mosq, void *obj, int level, const char *str){

}

bool parse_data(string s, string &temp, string &cond){
    string t_string = "";
    string c_string = "";
    if (s.find("\t") != std::string::npos){
        temp = s.substr(0, s.find("\t"));
        cond = s.substr(s.find("\t")+1, s.find("\t"));
    } else {
        temp = s;
    }
    return true;
}

void timestamp_loop(){
    while(running){
        usleep(LOOP_PERIOD_MS);
        int rc = mosquitto_loop(mosq, 0, 1);
        if (rc != MOSQ_ERR_SUCCESS && rc != MOSQ_ERR_CONN_LOST) {
            cerr << "Error in Mosquitto loop: " << mosquitto_strerror(rc) << endl;
            break;
        }
    }
}

// Main function
int main(int argc, char* argv[])
{
    // Set process priority
    set_process_priority(PROCESS_PRIORITY);

    // Register interrupt signal handler
    signal(SIGINT, interrupt_handler);

    // Open serial device
    serial_port = open_serial_device(SERIAL_DEVICE_NAME, SERIAL_BAUD_RATE);

    // Initialize Mosquitto library
    mosquitto_lib_init();
    // Create Mosquitto client instance
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
    cerr << "Failed to create Mosquitto client instance" << endl;
    exit(EXIT_FAILURE);
    }

    // Connect to MQTT broker
    mosquitto_connect_callback_set(mosq, on_connect);
    mosquitto_message_callback_set(mosq, on_message);
    mosquitto_log_callback_set(mosq, on_log);
    int rc = mosquitto_connect(mosq, MQTT_BROKER_IP, MQTT_BROKER_PORT, 0);
    if (rc != MOSQ_ERR_SUCCESS) {
    cerr << "Failed to connect to MQTT broker: " << mosquitto_strerror(rc) << endl;
    exit(EXIT_FAILURE);
    }

    // Subscribe to timestamp topic
    rc = mosquitto_subscribe(mosq, NULL, "clock/timestamp", MQTT_QOS);
    if (rc != MOSQ_ERR_SUCCESS) {
    cerr << "Failed to subscribe to timestamp topic: " << mosquitto_strerror(rc) << endl;
    exit(EXIT_FAILURE);
    }

    // Set up polling for serial device
    //struct pollfd pollfds[1];
    //pollfds[0].fd = fd;
    //pollfds[0].events = POLLIN;


    thread time_loop(timestamp_loop);
    // Start loop

    bool message_complete = false;
    while (running) {
        // Wait for loop period
        usleep(LOOP_PERIOD_MS);

        // Check for interrupt signal
        //if (interrupt_received) {
        //    cout << "Interrupt signal received, exiting" << endl;
        //    break;
        //}
        // Check for incoming MQTT messages
   


        string data;
        while (!message_complete){
            read_serial_data(serial_port, data);
            usleep(LOOP_PERIOD_MS);
            if (data.find("\n") != std::string::npos){
                message_complete = true;
            }
        }


    // Parse temperature and conductivity values from data
        string temperature, conductivity;
        if(parse_data(data, temperature, conductivity)){
            data = "";
        }

        // Publish temperature to MQTT broker
        string temp_topic = string(MQTT_TOPIC_PREFIX) + "/temperature";
        string temp_payload = temperature + " " + ts + " 1";
        cout << "temp: " << temp_payload << endl;
        mosquitto_publish(mosq, NULL, temp_topic.c_str(), temp_payload.size(), temp_payload.c_str(), 1, false);

        // Publish conductivity to MQTT broker
        string cond_topic = string(MQTT_TOPIC_PREFIX) + "/conductivity";
        string cond_payload = conductivity + " " + ts + " 1";
        cout << "cond: " <<  cond_payload << endl;
        mosquitto_publish(mosq, NULL, cond_topic.c_str(), cond_payload.size(), cond_payload.c_str(), 1, false);

        message_complete = false;
    }
    time_loop.join();

    // Cleanup Mosquitto client
    mosquitto_destroy(mosq);
    mosquitto_lib_cleanup();

    // Close serial port
    serial_port_close(serial_port);


    return 0;
}
