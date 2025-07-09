/**
    Scritto da: Marco Montanari
    Data: 12/07/2023 12:00
    Versione: 0.1
    
    Main Folder: ixblue_stdbin_decoder/examples/udp_listener.cpp
    Destination: /home/rob/bin
    
    Comando di compilazione:  g++ -o /home/rob/bin/ib_ins udp_listener.cpp -I/usr/share/c++-mscl/source/ -I/usr/share/c++-mscl/Boost/include/ -L/usr/share/c++-mscl -lmosquitto -lmscl -lstdc++ -std=c++11 -fpermissive -lpthread -lixblue_stdbin_decoder


    Descrizione:
    Script che si collega alla eth UDP Port 8119 per leggere dati in formato ixblue_stdbin
    Dati Codificati direttamente dal FOG: 
    -attitudeHeading
    -attitudeHeadingDeviation
    -headingRollPitchRate
    -rotationRateVesselFrame
    -accelerationVesselFrame
    -positionDeviation
    -speedVesselFrame
    -position
    -sensorStatus
    -heaveSurgeSwaySpeed

    Dati Codificati Input:
    -utc
    -gnss1
    -gnss2
    -depth
    -dvlGroundSpeed1
    -dvlWaterSpeed1
    -soundVelocity
    -dvlGroundSpeed2
    -dvlWaterSpeed2
    

*/

#include <array>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <limits>
#include <string>
#include <mosquitto.h>

#include <boost/asio.hpp>

#include <ixblue_stdbin_decoder/stdbin_decoder.h>

#define PROCESS_PRIORITY 80
#define LOOP_PERIOD_MS 100
#define COMM_TIMEOUT_SEC 5
#define SIMULATION_FLAG 0
#define DEBUG_LOGGING_FLAG 0
#define MQTT_BROKER_IP "127.0.0.1"
#define MQTT_BROKER_PORT 1883
#define MQTT_QOS 0
#define MQTT_TOPIC_PREFIX "ib_ins"
#define MQTT_TIMESTAMP_TOPIC "clock/timestamp"
#define SERIAL_DEVICE_NAME "/dev/ttyS5"
#define UDP_PORT 8119

using namespace std;

// Global variables
volatile bool running = true;
int fd;
struct mosquitto *mosq = NULL;
using boost::asio::ip::udp;

string ts = "0";

// Publish data to MQTT broker
void publish_to_mqtt(const string& topic, const string& payload)
{
    mosquitto_publish(mosq, NULL, topic.c_str(), payload.size(), payload.c_str(), MQTT_QOS, false);
}


void on_message(struct mosquitto *mosq, void *obj, const struct mosquitto_message *msg){
	string pl = (char *)msg->payload;
    ts = pl.substr(0, pl.find(" "));
}

int main(int argc, char** argv)
{
 // Initialize Mosquitto library
    mosquitto_lib_init();
    // Create Mosquitto client instance
    mosq = mosquitto_new(NULL, true, NULL);
    if (!mosq) {
    cerr << "Failed to create Mosquitto client instance" << endl;
    exit(EXIT_FAILURE);
    }
    mosquitto_message_callback_set(mosq, on_message);
    mosquitto_threaded_set(mosq, true);

    // Connect to MQTT broker
    //mosquitto_connect_callback_set(mosq, on_connect);
    //mosquitto_message_callback_set(mosq, on_message);
    //mosquitto_log_callback_set(mosq, on_log);
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


    const int port = UDP_PORT;
    if(port <= 0 || port > std::numeric_limits<uint16_t>::max())
    {
        std::cerr << "Port (=" << port << ") must be between 1 and "
                  << std::numeric_limits<uint16_t>::max() << '\n';
        return 1;
    }

    boost::asio::io_service io;
    udp::socket socket{io, udp::endpoint(udp::v4(), port)};

    std::array<uint8_t, 9000> recvBuffer;
    udp::endpoint senderEndpoint;
    ixblue_stdbin_decoder::StdBinDecoder decoder;
    auto lastPrint = std::chrono::steady_clock::now();

    std::cout << "Listening on UDP port " << port << "...\n";

    try
    {
        while(true)
        {
            mosquitto_loop(mosq, 10, 10000);
            const auto bytesRead =
                socket.receive_from(boost::asio::buffer(recvBuffer), senderEndpoint);
            decoder.addNewData(recvBuffer.data(), bytesRead);
            while(decoder.parseNextFrame())
            {
                if((std::chrono::steady_clock::now() - lastPrint) >
                   std::chrono::milliseconds{50})
                {
                    const auto nav = decoder.getLastNavData();
                    string topic = "ib_ins/";
                    lastPrint = std::chrono::steady_clock::now();

                    
                    if(nav.attitudeHeading.is_initialized())
                    {
                        publish_to_mqtt(topic+"heading", std::to_string(nav.attitudeHeading->heading_deg)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"roll", std::to_string(nav.attitudeHeading->roll_deg)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"pitch", std::to_string(nav.attitudeHeading->pitch_deg)+ " " + ts + " 1");
                    }
                    if(nav.attitudeHeadingDeviation.is_initialized())
                    {
                        publish_to_mqtt(topic+"headingStd", std::to_string(nav.attitudeHeadingDeviation->heading_stdDev_deg)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"rollStd", std::to_string(nav.attitudeHeadingDeviation->roll_stdDev_deg)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"pitchStd", std::to_string(nav.attitudeHeadingDeviation->pitch_stdDev_deg)+ " " + ts + " 1");
                    }
                    if(nav.headingRollPitchRate.is_initialized())
                    {
                        publish_to_mqtt(topic+"navigation/headingRate", std::to_string(nav.headingRollPitchRate->heading_rate)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/rollRate", std::to_string(nav.headingRollPitchRate->roll_rate)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/pitchRate", std::to_string(nav.headingRollPitchRate->pitch_rate)+ " " + ts + " 1");
                    }
                    if(nav.rotationRateVesselFrame.is_initialized())
                    {
                        publish_to_mqtt(topic+"navigation/xv1_degsec", std::to_string(nav.rotationRateVesselFrame->xv1_degsec)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/xv2_degsec", std::to_string(nav.rotationRateVesselFrame->xv2_degsec)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/xv3_degsec", std::to_string(nav.rotationRateVesselFrame->xv3_degsec)+ " " + ts + " 1");
                    }
                    
                    if(nav.accelerationVesselFrame.is_initialized())
                    {
                        publish_to_mqtt(topic+"navigation/xv1_msec2", std::to_string(nav.accelerationVesselFrame->xv1_msec2)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/xv2_msec2", std::to_string(nav.accelerationVesselFrame->xv2_msec2)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/xv3_msec2", std::to_string(nav.accelerationVesselFrame->xv3_msec2)+ " " + ts + " 1");
                    }
                    
                    if(nav.positionDeviation.is_initialized())
                    {
                        publish_to_mqtt(topic+"navigation/north_stddev_m", std::to_string(nav.positionDeviation->north_stddev_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/east_stddev_m", std::to_string(nav.positionDeviation->east_stddev_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/north_east_corr", std::to_string(nav.positionDeviation->north_east_corr)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/altitude_stddev_m", std::to_string(nav.positionDeviation->altitude_stddev_m)+ " " + ts + " 1");
                    }
                    if(nav.speedVesselFrame.is_initialized())
                    {
                        publish_to_mqtt(topic+"navigation/xv1_msec", std::to_string(nav.speedVesselFrame->xv1_msec)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/xv2_msec", std::to_string(nav.speedVesselFrame->xv2_msec)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/xv3_msec", std::to_string(nav.speedVesselFrame->xv3_msec)+ " " + ts + " 1");
                    }

                    if(nav.position.is_initialized())
                    {
			            publish_to_mqtt(topic + "navigation/latitude", std::to_string(nav.position->latitude_deg) + " " + ts + " 1");
                        publish_to_mqtt(topic + "navigation/longitude", std::to_string(nav.position->longitude_deg) + " " + ts + " 1");
                        publish_to_mqtt(topic + "navigation/altitude", std::to_string(nav.position->altitude_m) + " " + ts + " 1");
                    }
                    if(nav.sensorStatus.is_initialized())
                    {
			            publish_to_mqtt(topic + "health/status1", std::to_string(nav.sensorStatus->status1) + " " + ts + " 1");
                        publish_to_mqtt(topic + "health/status2", std::to_string(nav.sensorStatus->status2) + " " + ts + " 1");
                    }

                    if(nav.heaveSurgeSwaySpeed.is_initialized())
                    {
                        publish_to_mqtt(topic+"navigation/realtime_heave_speed", std::to_string(nav.heaveSurgeSwaySpeed->realtime_heave_speed)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/surge_speed", std::to_string(nav.heaveSurgeSwaySpeed->surge_speed)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"navigation/sway_speed", std::to_string(nav.heaveSurgeSwaySpeed->sway_speed)+ " " + ts + " 1");
                    }
//DATI INPUT
                    if(nav.utc.is_initialized())
                    {
                        publish_to_mqtt(topic+"input/utc/validityTime_100us", std::to_string(nav.utc->validityTime_100us)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/utc/source", std::to_string(nav.utc->source)+ " " + ts + " 1");
                    }

                    if(nav.gnss1.is_initialized())
                    {
                        publish_to_mqtt(topic+"input/gnss1/validityTime_100us", std::to_string(nav.gnss1->validityTime_100us)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss1/gnss_id", std::to_string(nav.gnss1->gnss_id)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss1/gnss_quality", std::to_string(nav.gnss1->gnss_quality)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss1/latitude_deg", std::to_string(nav.gnss1->latitude_deg)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss1/longitude_deg", std::to_string(nav.gnss1->longitude_deg)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss1/altitude_m", std::to_string(nav.gnss1->altitude_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss1/latitude_stddev_m", std::to_string(nav.gnss1->latitude_stddev_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss1/longitude_stddev_m", std::to_string(nav.gnss1->longitude_stddev_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss1/altitude_stddev_m", std::to_string(nav.gnss1->altitude_stddev_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss1/lat_lon_stddev_m2", std::to_string(nav.gnss1->lat_lon_stddev_m2)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss1/geoidal_separation_m", std::to_string(nav.gnss1->geoidal_separation_m)+ " " + ts + " 1");
                    }
                    
                    if(nav.gnss2.is_initialized())
                    {
                        publish_to_mqtt(topic+"input/gnss2/validityTime_100us", std::to_string(nav.gnss2->validityTime_100us)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss2/gnss_id", std::to_string(nav.gnss2->gnss_id)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss2/gnss_quality", std::to_string(nav.gnss2->gnss_quality)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss2/latitude_deg", std::to_string(nav.gnss2->latitude_deg)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss2/longitude_deg", std::to_string(nav.gnss2->longitude_deg)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss2/altitude_m", std::to_string(nav.gnss2->altitude_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss2/latitude_stddev_m", std::to_string(nav.gnss2->latitude_stddev_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss2/longitude_stddev_m", std::to_string(nav.gnss2->longitude_stddev_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss2/altitude_stddev_m", std::to_string(nav.gnss2->altitude_stddev_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss2/lat_lon_stddev_m2", std::to_string(nav.gnss2->lat_lon_stddev_m2)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/gnss2/geoidal_separation_m", std::to_string(nav.gnss2->geoidal_separation_m)+ " " + ts + " 1");
                    }
                    
                    //if(nav.gnssManual.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/gnssManual", std::to_string(nav.gnssManual)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.emlog1.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/emlog1", std::to_string(nav.emlog1)+ " " + ts + " 1");
                   //}
                    
                    //if(nav.emlog2.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/emlog2", std::to_string(nav.emlog2)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.usbl1.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/usbl1", std::to_string(nav.usbl1)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.usbl2.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/usbl2", std::to_string(nav.usbl2)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.usbl3.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/usbl3", std::to_string(nav.usbl3)+ " " + ts + " 1");
 //}
                    if(nav.depth.is_initialized())
                    {
                        publish_to_mqtt(topic+"input/depth/validityTime_100us", std::to_string(nav.depth->validityTime_100us)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/depth/depth_m", std::to_string(nav.depth->depth_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/depth/depth_stddev_m", std::to_string(nav.depth->depth_stddev_m)+ " " + ts + " 1");
                        }
                    if(nav.dvlGroundSpeed1.is_initialized())
                    { 
                        publish_to_mqtt(topic+"input/dvlGroundSpeed1/validityTime_100us", std::to_string(nav.dvlGroundSpeed1->validityTime_100us)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed1/dvl_id", std::to_string(nav.dvlGroundSpeed1->dvl_id)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed1/xv1_groundspeed_ms", std::to_string(nav.dvlGroundSpeed1->xv1_groundspeed_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed1/xv2_groundspeed_ms", std::to_string(nav.dvlGroundSpeed1->xv2_groundspeed_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed1/xv3_groundspeed_ms", std::to_string(nav.dvlGroundSpeed1->xv3_groundspeed_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed1/dvl_speedofsound_ms", std::to_string(nav.dvlGroundSpeed1->dvl_speedofsound_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed1/dvl_altitude_m", std::to_string(nav.dvlGroundSpeed1->dvl_altitude_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed1/xv1_stddev_ms", std::to_string(nav.dvlGroundSpeed1->xv1_stddev_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed1/xv2_stddev_ms", std::to_string(nav.dvlGroundSpeed1->xv2_stddev_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed1/xv3_stddev_ms", std::to_string(nav.dvlGroundSpeed1->xv3_stddev_ms)+ " " + ts + " 1");
                     }
                    if(nav.dvlWaterSpeed1.is_initialized())
                    {
                        publish_to_mqtt(topic+"input/dvlWaterSpeed1/validityTime_100us", std::to_string(nav.dvlWaterSpeed1->validityTime_100us)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed1/dvl_id", std::to_string(nav.dvlWaterSpeed1->dvl_id)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed1/xv1_waterspeed_ms", std::to_string(nav.dvlWaterSpeed1->xv1_waterspeed_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed1/xv2_waterspeed_ms", std::to_string(nav.dvlWaterSpeed1->xv2_waterspeed_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed1/xv3_waterspeed_ms", std::to_string(nav.dvlWaterSpeed1->xv3_waterspeed_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed1/dvl_speedofsound_ms", std::to_string(nav.dvlWaterSpeed1->dvl_speedofsound_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed1/xv1_stddev_ms", std::to_string(nav.dvlWaterSpeed1->xv1_stddev_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed1/xv2_stddev_ms", std::to_string(nav.dvlWaterSpeed1->xv2_stddev_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed1/xv3_stddev_ms", std::to_string(nav.dvlWaterSpeed1->xv3_stddev_ms)+ " " + ts + " 1");
                         }
                    if(nav.soundVelocity.is_initialized())
                    {
                        publish_to_mqtt(topic+"input/soundVelocity/validityTime_100us", std::to_string(nav.soundVelocity->validityTime_100us)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/soundVelocity/ext_speedofsound_ms", std::to_string(nav.soundVelocity->ext_speedofsound_ms)+ " " + ts + " 1");
 }
                   //if(nav.dmi.is_initialized())
                   //{
                        //publish_to_mqtt(topic+"input/dmi", std::to_string(nav.dmi)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.lbl1.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/lbl1", std::to_string(nav.lbl1)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.lbl2.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/lbl2", std::to_string(nav.lbl2)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.lbl3.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/lbl3", std::to_string(nav.lbl3)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.lbl4.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/lbl4", std::to_string(nav.lbl4)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.eventMarkerA.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/eventMarkerA", std::to_string(nav.eventMarkerA)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.eventMarkerB.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/eventMarkerB", std::to_string(nav.eventMarkerB)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.eventMarkerC.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/eventMarkerC", std::to_string(nav.eventMarkerC)+ " " + ts + " 1");
 //}
                    if(nav.dvlGroundSpeed2.is_initialized())
                    {
                        publish_to_mqtt(topic+"input/dvlGroundSpeed2/validityTime_100us", std::to_string(nav.dvlGroundSpeed2->validityTime_100us)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed2/dvl_id", std::to_string(nav.dvlGroundSpeed2->dvl_id)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed2/xv1_groundspeed_ms", std::to_string(nav.dvlGroundSpeed2->xv1_groundspeed_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed2/xv2_groundspeed_ms", std::to_string(nav.dvlGroundSpeed2->xv2_groundspeed_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed2/xv3_groundspeed_ms", std::to_string(nav.dvlGroundSpeed2->xv3_groundspeed_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed2/dvl_speedofsound_ms", std::to_string(nav.dvlGroundSpeed2->dvl_speedofsound_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed2/dvl_altitude_m", std::to_string(nav.dvlGroundSpeed2->dvl_altitude_m)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed2/xv1_stddev_ms", std::to_string(nav.dvlGroundSpeed2->xv1_stddev_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed2/xv2_stddev_ms", std::to_string(nav.dvlGroundSpeed2->xv2_stddev_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlGroundSpeed2/xv3_stddev_ms", std::to_string(nav.dvlGroundSpeed2->xv3_stddev_ms)+ " " + ts + " 1");
                     }
                    if(nav.dvlWaterSpeed2.is_initialized())
                    {
                        publish_to_mqtt(topic+"input/dvlWaterSpeed2/validityTime_100us", std::to_string(nav.dvlWaterSpeed2->validityTime_100us)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed2/dvl_id", std::to_string(nav.dvlWaterSpeed2->dvl_id)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed2/xv1_waterspeed_ms", std::to_string(nav.dvlWaterSpeed2->xv1_waterspeed_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed2/xv2_waterspeed_ms", std::to_string(nav.dvlWaterSpeed2->xv2_waterspeed_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed2/xv3_waterspeed_ms", std::to_string(nav.dvlWaterSpeed2->xv3_waterspeed_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed2/dvl_speedofsound_ms", std::to_string(nav.dvlWaterSpeed2->dvl_speedofsound_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed2/xv1_stddev_ms", std::to_string(nav.dvlWaterSpeed2->xv1_stddev_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed2/xv2_stddev_ms", std::to_string(nav.dvlWaterSpeed2->xv2_stddev_ms)+ " " + ts + " 1");
                        publish_to_mqtt(topic+"input/dvlWaterSpeed2/xv3_stddev_ms", std::to_string(nav.dvlWaterSpeed2->xv3_stddev_ms)+ " " + ts + " 1");
                         }
                    //if(nav.turretAngles.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/turretAngles", std::to_string(nav.turretAngles)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.vtg1.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/vtg1", std::to_string(nav.vtg1)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.vtg2.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/vtg2", std::to_string(nav.vtg2)+ " " + ts + " 1");
                    //}
                    
                    //if(nav.logBook.is_initialized())
                    //{
                        //publish_to_mqtt(topic+"input/logBook", std::to_string(nav.logBook)+ " " + ts + " 1");
                    //}
                    

                }
            }
        }
    }
    catch(const std::runtime_error& e)
    {
        std::cerr << "Parser error: " << e.what() << '\n';
        return 1;
    }
}

