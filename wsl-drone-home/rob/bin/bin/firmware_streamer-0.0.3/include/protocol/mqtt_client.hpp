#ifndef MQTT_CLIENT_HPP
#define MQTT_CLIENT_HPP

#include <dependencies.hpp>
#include <protocol/protocol_base.hpp>

class MqttClient : public ProtocolBase , public mosqpp::mosquittopp
{
  /* */
  public:
    MqttClient(
      const std::string    & sensor_name,
      const std::string    & server_address        ,
      const unsigned int   & server_port           ,
      const std::string    & client_name           ,
      const uint8_t        & qos                   ,
      const nlohmann::json & topics                ,
      const std::shared_ptr<DataFormat> data_format,
      const std::string    & log_level           
    );
    ~MqttClient(void);
    /* ProtocolBase */
    bool open  (void)                                   override;
    bool setup (void)                                   override;
    bool close (void)                                   override;
    bool stop  (void)                                   override;
    void run   (void)                                   override;
    bool read  (void)                                   override;
    bool write (std::shared_ptr<std::vector<char>> msg) override;
    /* mosquittopp */
    void on_connect   (int rc)                                         override;
    void on_publish   (int mid)                                        override;
    void on_subscribe (int mid, int qos_count, const int *granted_qos) override;
    void on_message   (const struct mosquitto_message * message)       override;
    void on_disconnect(int rc)                                         override;
  /* */
  private:
    bool is_connected(void);
    bool is_running  (void);
    /* */
    bool connected_ = false;
    bool running_   = false;
    /* */
    std::string              broker_address_;
    unsigned int             broker_port_   ;
    std::string              client_name_   ;
    uint8_t                  qos_           ;
    std::vector<std::string> topics_        ;
    std::string              timestamp_     ;
};

#endif //MQTT_CLIENT_HPP