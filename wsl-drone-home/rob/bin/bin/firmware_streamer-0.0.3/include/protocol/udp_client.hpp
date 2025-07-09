#ifndef UDP_CLIENT_HPP
#define UDP_CLIENT_HPP

#include <dependencies.hpp>
#include <protocol/protocol_base.hpp>


class UdpClient : public ProtocolBase
{
  /* */
  public:
    UdpClient(
      const std::string  &              sensor_name   ,
      const std::string  &              local_address ,
      const unsigned int &              local_port    ,
      const std::string  &              server_address,
      const unsigned int &              server_port   ,
      const std::string  &              name          ,
      const std::shared_ptr<DataFormat> data_format   ,
      const std::string  &              log_level
    );
    ~UdpClient(void);
    /* ProtocolBase */
    bool setup (void)                                   override;
    bool open  (void)                                   override;
    bool close (void)                                   override;
    bool stop  (void)                                   override;
    void run   (void)                                   override;
    bool read  (void)                                   override;
    bool write (std::shared_ptr<std::vector<char>> msg) override;
  /* */
  private:
    /* */
    void read_callback (const boost::system::error_code & error, std::size_t bytes_received);
    void write_callback(const boost::system::error_code & error, std::size_t bytes_sent    );
    /* */
    boost::asio::io_service        io_             ;
    boost::asio::ip::udp::socket   socket_         ;
    boost::asio::ip::udp::endpoint local_endpoint_ ;
    boost::asio::ip::udp::endpoint server_endpoint_;
    /* */
    std::string  local_address_ ;
    unsigned int local_port_    ;
    std::string  server_address_;
    unsigned int server_port_   ;
    std::string  name_          ;
};

#endif //UDP_CLIENT_HPP