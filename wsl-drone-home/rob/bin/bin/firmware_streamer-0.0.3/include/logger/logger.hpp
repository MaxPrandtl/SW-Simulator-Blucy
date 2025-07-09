#ifndef LOGGER_HPP
#define LOGGER_HPP

#include <dependencies.hpp>


class Logger
{
  /* */
  public:
    Logger(const std::string & name, const std::string & log_level);
    ~Logger();
    /* */
    void trace   (const std::string & msg) {this->logger_->trace   (msg);};
    void debug   (const std::string & msg) {this->logger_->debug   (msg);};
    void info    (const std::string & msg) {this->logger_->info    (msg);};
    void warn    (const std::string & msg) {this->logger_->warn    (msg);};
    void error   (const std::string & msg) {this->logger_->error   (msg);};
    void critical(const std::string & msg) {this->logger_->critical(msg);};
    /* */
  protected:
    bool init_ptr();
    bool setup_ptr();
  /* */
  private:
    bool set_level();
    std::string name_                      ;
    std::string log_level_                 ;
    std::shared_ptr<spdlog::logger> logger_;
};

#endif //LOGGER_HPP