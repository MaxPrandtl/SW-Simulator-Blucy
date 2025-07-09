#ifndef SERIAL_HPP
#define SERIAL_HPP

#include <dependencies.hpp>
#include <protocol/protocol_base.hpp>


class Serial : public ProtocolBase
{
  /* */
  public:
    Serial(
      const std::string  &              sensor_name,
      const std::string  &              port_name  ,
      const unsigned int &              baud_rate  ,
      const std::shared_ptr<DataFormat> data_format,
      const std::string  &              log_level
    );
    ~Serial(void);
    /* ProtocolBase */
    bool open  (void)                                   override;
    bool setup (void)                                   override;
    bool close (void)                                   override;
    bool stop  (void)                                   override;
    void run   (void)                                   override;
    bool read  (void)                                   override;
    bool write (std::shared_ptr<std::vector<char>> msg) override;
  /* */
  private:
    void read_callback(const boost::system::error_code & error, std::size_t bytes_received);
    boost::asio::io_service  io_        ;
    boost::asio::serial_port serial_    ;
    std::string              port_name_ ;
    int                      baud_rate_ ;
    std::string              format_    ;
};

#endif //SERIAL_HPP