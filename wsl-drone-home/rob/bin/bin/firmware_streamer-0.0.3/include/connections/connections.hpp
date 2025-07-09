#ifndef CONNECTION_HPP
#define CONNECTION_HPP

#include <dependencies.hpp>
#include <data_format/data_format.hpp>
#include <protocol/protocol_base.hpp>
#include <protocol/mqtt_client.hpp>
#include <protocol/serial.hpp>
#include <protocol/udp_client.hpp>

class Connection
{
  enum DIRECTION
  {
    INCOMING = 0,
    OUTGOING = 1
  };
  /* */
  public:
    Connection(const nlohmann::json & connection_config);
    ~Connection();
    /*  */
    bool open();
    bool setup();
    bool start();
    bool stop();
    bool join();
    bool close();
  /* */
  protected:
    /* */
    bool init_ptr        (void);
    bool init_connections(void);
    /* */
    std::shared_ptr<Logger>     logger_     ;
    std::shared_ptr<DataFormat> data_format_;
  /* */
  private:
    /* */
    bool init_connection(Connection::DIRECTION direction, const nlohmann::json & config_connections);
    bool init_observers(void);
    /* */
    std::string    name_              ;
    bool           enabled_           ;
    std::string    log_level_         ;
    nlohmann::json config_data_format_;
    nlohmann::json config_incomings_  ;
    nlohmann::json config_outgoings_  ;
    std::vector<std::shared_ptr<ProtocolBase>> incomings_;
    std::vector<std::shared_ptr<ProtocolBase>> outgoings_;
    /* */
};

#endif //CONNECTION_HPP