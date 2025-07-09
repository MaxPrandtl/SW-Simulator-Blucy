#include <protocol/udp_client.hpp>

UdpClient::UdpClient(
  const std::string  &              sensor_name   ,
  const std::string  &              local_address ,
  const unsigned int &              local_port    ,
  const std::string  &              server_address,
  const unsigned int &              server_port   ,
  const std::string  &              name          ,
  const std::shared_ptr<DataFormat> data_format   ,
  const std::string  &              log_level
) : ProtocolBase(sensor_name, "udp", name, data_format, log_level),
    io_(),
    socket_(io_)
{
  /* */
  this->local_endpoint_  = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(local_address ), local_port ),
  this->server_endpoint_ = boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string(server_address), server_port),
  /* */  
  this->local_address_  = local_address;
  this->local_port_     = local_port;
  this->server_address_ = server_address;
  this->server_port_    = server_port;
  this->name_           = name;
  /* */
}


UdpClient::~UdpClient(void)
{
  this->stop();
  this->close();
}


bool UdpClient::open(void)
{
  if (!this->socket_.is_open())
  {
    this->socket_.open(boost::asio::ip::udp::v4());
    this->socket_.bind(this->local_endpoint_);
    this->logger_->info("connection open");
    this->logger_->debug("bound to: " + this->socket_.local_endpoint().address().to_string() + ":" + std::to_string(this->socket_.local_endpoint().port()));
  }
  else
    this->logger_->info("already open");
  /* */
  return true;
}


bool UdpClient::setup(void)
{
  /* */
  return true;
}


void UdpClient::run(void)
{
  this->read();
  this->io_.run();
  /* */
}


bool UdpClient::stop(void)
{
  if (!this->io_.stopped())
  {
    this->io_.stop();
    this->logger_->info("stopped");
  }
  else
    this->logger_->info("already stopped");
  /* */
  return true;
}


bool UdpClient::close(void)
{
  if (this->socket_.is_open())
  {
    this->socket_.close();
    this->logger_->info("closed");
  }
  else
    this->logger_->info("already closed");
  return true;
}


bool UdpClient::read(void)
{
  if (this->socket_.is_open())
  {
    this->socket_.async_receive_from
    (
      boost::asio::buffer(*this->buffer_in_),
      this->local_endpoint_,
      boost::bind(&UdpClient::read_callback, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
    );
  }
  /* */
  return true;
}

bool UdpClient::write(std::shared_ptr<std::vector<char>> msg)
{
  if (this->socket_.is_open())
  {
    this->buffer_out_ = msg;
    this->socket_.async_send_to
    (
      boost::asio::buffer(*this->buffer_out_),
      this->server_endpoint_,
      boost::bind(&UdpClient::write_callback, this, boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred)
    );
  }  
  /* */
  return true;
}


void UdpClient::read_callback(const boost::system::error_code& error, std::size_t bytes_received)
{
  if (! error)
  {
    this->logger_->debug("Bytes received: " + std::to_string(bytes_received) + " from [" + this->local_endpoint_.address().to_string() + ":" + std::to_string(this->local_endpoint_.port()) + "]");
    this->logger_->debug("payload: " + std::string(this->buffer_in_->begin(), this->buffer_in_->begin() + bytes_received));
    /* */
    this->read();
  }
  else
    this->logger_->debug("Error during async_read: " + error.message());
}


void UdpClient::write_callback(const boost::system::error_code & error, std::size_t bytes_sent)
{
  /* */
  if (! error)
    this->logger_->debug("Bytes sent: " + std::to_string(bytes_sent) + " to [" + this->server_endpoint_.address().to_string() + ":" + std::to_string(this->server_endpoint_.port()) + "]");
  else
    this->logger_->debug("Error during async_write: " + error.message());
}

