#include <logger/logger.hpp>

Logger::Logger(const std::string & name, const std::string & log_level)
{
  /* */
  this->name_      = name     ;
  this->log_level_ = log_level;
  /* */
  this->init_ptr();
  /* */
  this->set_level();
}

Logger::~Logger() {}


bool Logger::init_ptr(void)
{
  /* init ptr */
  if (!this->logger_)
    this->logger_ = spdlog::stdout_color_mt(this->name_);
  /* */
  
  return true;
}


bool Logger::setup_ptr(void)
{
  /* set logger */
  this->logger_->set_pattern("[%Y-%m-%d %H:%M:%S] [%l] [%n] %v");
  this->set_level();
  /* */  
  return true;
}


bool Logger::set_level(void)
{
  /* */
  if (this->log_level_ == "trace")
    this->logger_->set_level(spdlog::level::trace);
  else if (this->log_level_ == "debug")
    this->logger_->set_level(spdlog::level::debug);
  else if (this->log_level_ == "info")
    this->logger_->set_level(spdlog::level::info);
  else if (this->log_level_ == "warn")
    this->logger_->set_level(spdlog::level::warn);
  else if (this->log_level_ == "err")
    this->logger_->set_level(spdlog::level::err);
  else if (this->log_level_ == "critical")
    this->logger_->set_level(spdlog::level::critical);
  /* */
  return true;
}