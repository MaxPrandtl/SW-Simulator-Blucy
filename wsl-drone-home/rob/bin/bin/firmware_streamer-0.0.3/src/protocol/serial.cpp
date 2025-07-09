#include <protocol/serial.hpp>

Serial::Serial(
  const std::string &               sensor_name,
  const std::string  &              port_name  ,
  const unsigned int &              baud_rate  ,
  const std::shared_ptr<DataFormat> data_format,
  const std::string  &              log_level
) : ProtocolBase(sensor_name, "serial", port_name, data_format, log_level),
    io_(),
    serial_(io_) 
{
  this->port_name_ = port_name;
  this->baud_rate_ = baud_rate;
  /* */
}


Serial::~Serial(void)
{
  this->stop();
  this->close();
}


bool Serial::open(void)
{
  if (!this->serial_.is_open())
  {
    boost::system::error_code ec;
    this->serial_.open(this->port_name_, ec);
    if (ec)
    {
      this->logger_->error("Error opening: " + ec.message());
      return false;
    }
    this->logger_->info("connection open");
  }
  else
    this->logger_->info("connection already open");
  /* */
  return true;
}


bool Serial::setup(void)
{
  /* set option */
  this->serial_.set_option(boost::asio::serial_port::baud_rate(this->baud_rate_));
  this->serial_.set_option(boost::asio::serial_port_base::character_size(8));
  /* */
  return true;
}


void Serial::run(void)
{
  this->read();
  this->io_.run();
  /* */
}


bool Serial::read(void)
{
  /**/
  boost::asio::async_read
  (
    this->serial_, 
    boost::asio::buffer(*this->buffer_in_),
    boost::bind(&Serial::read_callback, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
  );
  /* */
  return true;
}


bool Serial::write(std::shared_ptr<std::vector<char>> msg)
{
  /**/
  this->logger_->debug("write: " + std::string(msg->data()));
  /* */
  return true;
}

bool Serial::stop(void)
{
  if (!this->io_.stopped())
  {
    this->io_.stop();
    this->logger_->info("connection stopped");
  }
  else
    this->logger_->info("connection already stopped");
  /* */
  return true;
}


bool Serial::close(void)
{
  if (this->serial_.is_open())
  {
    this->serial_.close();
    this->logger_->info("connection closed");
  }
  else
    this->logger_->info("connection already closed");
  /* */
  return true;
}


void Serial::read_callback(const boost::system::error_code& error, std::size_t bytes_received)
{
  if (! error)
  {
    this->logger_->debug("Bytes received: " + std::to_string(bytes_received));
    this->notifyObservers(this->buffer_in_);
    /* */
    this->read();
  }
  else
    this->logger_->error("Error during async_read: " + error.message());
}