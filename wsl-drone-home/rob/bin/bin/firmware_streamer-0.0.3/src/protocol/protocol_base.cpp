#include <protocol/protocol_base.hpp>

ProtocolBase::ProtocolBase(const std::string & sensor_name, const std::string & protocol, const std::string & name, const std::shared_ptr<DataFormat> datas, const std::string & log_level)
{
  /* */
  this->sensor_name_ = sensor_name;
  this->protocol_    = protocol   ;
  this->name_        = name       ;
  this->datas_       = datas      ;
  this->log_level_   = log_level  ;
  /* init ptr */
  this->init_ptr();
  /* */
  this->setup_ptr();
  /* */
  this->logger_->info("connection created");
}


bool ProtocolBase::init_ptr(void)
{
  /* init ptr */
  if (!this->logger_)
    this->logger_ = std::make_shared<Logger>((this->sensor_name_ + " : " + this->protocol_ + " : " + this->name_), this->log_level_);
  /* */
  this->buffer_in_  = std::make_shared<std::vector<char>>();
  this->buffer_out_ = std::make_shared<std::vector<char>>();
  return true;
}


bool ProtocolBase::setup_ptr(void)
{
  /* setup buffers:
   * for now they are not a ptr. Eventually they will become.
   */
  this->buffer_in_ ->resize(this->datas_->get_bytes());
  this->buffer_out_->resize(this->datas_->get_bytes());
  /* */  
  return true;
}


void ProtocolBase::start(void)
{
  this->thread_ = std::thread(&ProtocolBase::run, this);
}


void ProtocolBase::join(void)
{
  if(this->thread_.joinable())
    this->thread_.join();
}


void ProtocolBase::attachObservers(std::shared_ptr<ProtocolBase> observer)
{ 
  this->observers_.push_back(observer);
}


void ProtocolBase::notifyObservers(std::shared_ptr<std::vector<char>> msg)
{
  for (auto& observer : this->observers_)
    observer->write(msg);
}