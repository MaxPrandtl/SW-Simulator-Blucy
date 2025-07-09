#include <connections/connections.hpp>

Connection::Connection(const nlohmann::json & connection_config) 
{
  /* */
  this->name_               = connection_config["name"       ];
  this->enabled_            = connection_config["enabled"    ];
  this->log_level_          = connection_config["log_level"  ];
  this->config_data_format_ = connection_config["data_format"];
  this->config_incomings_   = connection_config["incoming"   ];
  this->config_outgoings_   = connection_config["outgoing"   ];
  /* */
  this->init_ptr();
  /* */
  this->init_connections();
}


Connection::~Connection(void)
{
  this->stop();
  this->close();
}


bool Connection::init_ptr(void)
{
  if (!this->logger_)
    this->logger_ = std::make_shared<Logger>((this->name_ + " : connection"), this->log_level_);
  if (!this->data_format_)
    this->data_format_ = std::make_shared<DataFormat>(this->name_, this->config_data_format_["name"], this->config_data_format_["path"], this->config_data_format_["log_level"]);
  /* */
  return true;
}


bool Connection::init_connection(Connection::DIRECTION direction, const nlohmann::json & config_connections)
{
  /* setup connection */
  for (const auto& config : config_connections)
  {
    /* check if enabled */
    if (config["enabled"])
    {
      std::shared_ptr<ProtocolBase> connection;
      /* create object connection */
      if (config["protocol"] == "serial")
        connection = std::make_shared<Serial>(this->name_, config["port"], config["rate"], this->data_format_ , config["log_level"]);
      else if (config["protocol"] == "mqtt")
        connection = std::make_shared<MqttClient>(this->name_, config["broker_address"], config["broker_port"], config["name"], config["qos"], config["topics"], this->data_format_ , config["log_level"]);
      else if (config["protocol"] == "udp")
        connection = std::make_shared<UdpClient>(this->name_, config["local_address"], config["local_port"], config["server_address"], config["server_port"], config["name"], this->data_format_ , config["log_level"]);
      else
      {
        std::cout << "no protocol " << config["protocol"] << " available" << std::endl;
        break; /* because I dont have to add this connection in the incomings or outgoings */
      }
    /* check direction */
    (direction == Connection::INCOMING) ? this->incomings_.push_back(connection) : this->outgoings_.push_back(connection); 
    }
  }
  /* */
  return true;
}


bool Connection::init_observers(void)
{
  /* attach callback 
   * for now only the serial incoming is receiving datas
   * to send to the outgoings. To edit for further use.  
   */
  for (const auto & outgoing : this->outgoings_)
    this->incomings_[0]->attachObservers(outgoing);
}



bool Connection::init_connections(void)
{
  /* setup connection */
  this->init_connection(Connection::INCOMING, this->config_incomings_);
  this->init_connection(Connection::OUTGOING, this->config_outgoings_);
  this->init_observers();
  /* */
  return true;
}


bool Connection::open(void)
{
  /* open incomings */
  for (const auto& incoming : this->incomings_)
    incoming->open();
  /* open outgoings */
  for (const auto& outgoing : this->outgoings_)
    outgoing->open();
  /* */
  return true;
}


bool Connection::setup(void)
{
  /* setup incomings */
  for (const auto& incoming : this->incomings_)
    incoming->setup();
  /* setup outgoings */
  for (const auto& outgoing : this->outgoings_)
    outgoing->setup();
  /* */
  return true;
}


bool Connection::start(void)
{
  /* start incoming */
  for (const auto& incoming : this->incomings_)
    incoming->start();
  /* start outgoing */
  for (const auto& outgoing : this->outgoings_)
    outgoing->start();
  /* */
  return true;
}


bool Connection::stop(void)
{
  /* stop incoming */
  for (const auto& incoming : this->incomings_)
    incoming->stop();
  /* stop outgoing */
  for (const auto& outgoing : this->outgoings_)
    outgoing->stop();
  /* */
  return true;
}


bool Connection::join(void)
{
  /* join incoming */
  for (const auto& incoming : this->incomings_)
    incoming->join();
  /* join outgoing */
  for (const auto& outgoing : this->outgoings_)
    outgoing->join();
  /* */
  return true;
}


bool Connection::close(void)
{
  /* close incoming */
  for (const auto& incoming : this->incomings_)
    incoming->close();
  /* close outgoing */
  for (const auto& outgoing : this->outgoings_)
    outgoing->close();
  /* */
  return true;
}
