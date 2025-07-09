#include <protocol/mqtt_client.hpp>

MqttClient::MqttClient(
  const std::string &               sensor_name   ,
  const std::string    &            broker_address,
  const unsigned int   &            broker_port   ,
  const std::string    &            client_name   ,
  const uint8_t        &            qos           ,
  const nlohmann::json &            topics        ,
  const std::shared_ptr<DataFormat> data_format   ,
  const std::string    &            log_level
) : ProtocolBase(sensor_name, "mqtt", client_name, data_format, log_level),
    mosqpp::mosquittopp(client_name.data())
{
  this->broker_address_ = broker_address;
  this->broker_port_    = broker_port;
  this->client_name_    = client_name;
  this->qos_            = qos;
  /* */
  for (const auto& topic : topics)
    this->topics_.push_back(topic);
  /* */
  this->connected_ = false;
  this->timestamp_ = "";
  /* */
  this->init_ptr();
}


MqttClient::~MqttClient(void)
{
  this->stop();
  this->close();
}


bool MqttClient::open(void)
{
  mosqpp::lib_init();
  /* */
  this->connect(this->broker_address_.data(), this->broker_port_, 60);
  /* */
  return true;
}


bool MqttClient::setup(void)
{
  /* */
  this->subscribe(nullptr, "clock/timestamp");
  /* */
  return true;
}


void MqttClient::run(void)
{
  /* */
  this->loop_start();
  this->running_ = true;
}


bool MqttClient::stop(void)
{
  if (this->is_running())
  {
    this->loop_stop(true);
    this->running_ = false;
    this->logger_->info("connection stopped");
  }
  else
    this->logger_->info("connection already stopped");
  /* */
  return true;
}


bool MqttClient::close(void)
{
  if (this->is_connected())
  {
    this->disconnect();
    mosqpp::lib_cleanup();
    this->logger_->info("connection closed");
  }
  else
    this->logger_->info("already closed");
  return true;
}


bool MqttClient::read(void)
{

  /* */
  return true;
}

bool MqttClient::write(std::shared_ptr<std::vector<char>> msg)
{
  if (this->is_connected())
  {
    for (const auto topic : this->topics_)
    {
      /* topic raw */
      if (topic == "raw")
      {
        this->buffer_out_ = msg;
        std::string payload = std::string(this->buffer_out_->begin(), this->buffer_out_->end()) + ", " + this->timestamp_ + ", " + std::to_string(this->qos_);
        this->publish(NULL, (this->client_name_ + "/" + topic).data(), payload.size(), payload.data(), 0, false);
      }
      /* topic parsed*/
      else
      {
        if (this->timestamp_ != "")
        {
          this->datas_->parse_datas(msg);
          for (const auto data : *this->datas_->get_datas())
          {
            std::string payload = std::to_string(data.value) + ", " + this->timestamp_ + ", " + std::to_string(this->qos_);
            this->publish(NULL, (this->client_name_ + "/" + data.label).data(), payload.size(), payload.data(), 0, false);
          }
        }
      }
    }
  }
  else
    this->logger_->info("not connected to the broker, please check connection");

  /* */
  return true;
}


void MqttClient::on_connect(int rc)
{
  /* */
  switch (rc)
  {
    case 0:
      this->connected_ = true;
      this->logger_->info("connection open");
      this->logger_->debug("Connected to MQTT broker [" + this->broker_address_+ ":" + std::to_string(this->broker_port_) + "]");
      break;
    case 1:
      this->logger_->warn("unacceptable protocol version");
      break;
    case 2:
      this->logger_->warn("identifier rejected");
      break;
    case 3:
      this->logger_->warn("broker unavailable");
      break;
    default:
      this->logger_->warn("cannot open connection");
      break;
  }
}


void MqttClient::on_disconnect(int rc)
{
  if (rc == 0)
  {
    this->logger_->info("Disconnected from MQTT broker");
    this->connected_ = false;
  }
}


void MqttClient::on_publish(int mid)
{
  this->logger_->debug("Message published with message ID: " + std::to_string(mid));
}

void MqttClient::on_subscribe(int mid, int qos_count, const int *granted_qos)
{
  this->logger_->debug("Subscription successsful. MID: " + std::to_string(mid));
}


void MqttClient::on_message(const struct mosquitto_message * message)
{
  std::string payload(static_cast<char*>(message->payload), message->payloadlen); 
  this->timestamp_ = payload;
  this->logger_->debug("Message received: " + this->timestamp_);
}


bool MqttClient::is_connected(void)
{
  /* */
  return this->connected_;
}


bool MqttClient::is_running(void)
{
  /* */
  return this->running_;
}

