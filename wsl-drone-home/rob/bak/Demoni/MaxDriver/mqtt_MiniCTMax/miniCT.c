//script prova mqtt_miniCT to mosquitto
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <termios.h>
#include <mosquitto.h>
#include <time.h>

// Setting parameters
#define PROCESS_PRIORITY 80
#define LOOP_PERIOD_MS 100
#define COMM_TIMEOUT_S 5
#define SIMULATION_FLAG 0
#define DEBUG_LOGGING_FLAG 0
#define MQTT_BROKER_ADDRESS "127.0.0.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_QOS 2
#define MQTT_TOPIC_PREFIX "miniCT"
#define MQTT_TIMESTAMP_TOPIC "clock/timestamp"
#define SERIAL_DEVICE_NAME "/dev/ttyS7"
#define SERIAL_BAUD_RATE 19200

//Global variables


int main(void) {
    int serial_fd;
    struct termios tty;
    char buf[256];
    int len;
    int rc;
    struct mosquitto *mqtt;
    time_t current_time;
    char timestamp[20];

    // Open serial port
    serial_fd = open(SERIAL_DEVICE, O_RDWR | O_NOCTTY | O_NDELAY);
    if (serial_fd == -1) {
        perror("open");
        exit(1);
    }

    // Set serial port settings
    memset(&tty, 0, sizeof(tty));
    cfsetospeed(&tty, BAUD_RATE);
    cfsetispeed(&tty, BAUD_RATE);
    tty.c_cflag |= (CLOCAL | CREAD);
    tty.c_cflag &= ~CSIZE;
    tty.c_cflag |= CS8;
    tty.c_cflag &= ~PARENB;
    tty.c_cflag &= ~CSTOPB;
    tcsetattr(serial_fd, TCSANOW, &tty);

    // Connect to MQTT server
    mosquitto_lib_init();
    mqtt = mosquitto_new(NULL, true, NULL);
    if (!mqtt) {
        perror("mosquitto_new");
        exit(1);
    }
    rc = mosquitto_connect(mqtt, MQTT_HOST, MQTT_PORT, 60);
    if (rc != MOSQ_ERR_SUCCESS) {
        fprintf(stderr, "mosquitto_connect failed: %s\n", mosquitto_strerror(rc));
        exit(1);
    }

    // Continuously read data from serial and publish to MQTT
    while (1) {
        len = read(serial_fd, buf, sizeof(buf));
        if (len > 0) {
            buf[len] = '\0';
            current_time = time(NULL);
            strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localtime(&current_time));
            int i;
            for (i = 0; i < len; i += MAX_PAYLOAD_SIZE) {
                int payload_size = len - i < MAX_PAYLOAD_SIZE ? len - i : MAX_PAYLOAD_SIZE;
                rc = mosquitto_publish(mqtt, NULL, MQTT_TOPIC, payload_size, &buf[i], 1, false);
                if (rc != MOSQ_ERR_SUCCESS) {
                    fprintf(stderr, "mosquitto_publish failed: %s\n", mosquitto_strerror(rc));
                }
                usleep(1000); // Sleep for 1ms
            }
            rc = mosquitto_publish(mqtt, NULL, "miniCT_Max/timestamp", strlen(timestamp), timestamp, 1, false);
            if (rc != MOSQ_ERR_SUCCESS) {
                fprintf(stderr, "mosquitto_publish failed: %s\n", mosquitto_strerror(rc));
            }
        }
        usleep(10000); // Sleep for 10ms
    }

    // Clean up
    mosquitto_disconnect(mqtt);
    mosquitto_destroy(mqtt);
    mosquitto_lib_cleanup();
    close(serial_fd);

    return 0;
}