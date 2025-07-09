#ifndef PROTOCOL_BASE_HPP
#define PROTOCOL_BASE_HPP

#include <dependencies.hpp>
#include <data_format/data_format.hpp>


class ProtocolBase
{
  /* */
  public:
    ProtocolBase(
      const std::string &               sensor_name,
      const std::string &               protocol   ,
      const std::string &               name       ,
      const std::shared_ptr<DataFormat> datas      ,
      const std::string &               log_level
    );
    virtual ~ProtocolBase() = default;
    virtual bool open  (void) = 0;
    virtual bool setup (void) = 0;
    virtual bool close (void) = 0;
    virtual void run   (void) = 0;
    virtual bool stop  (void) = 0;
    virtual bool read  (void) = 0;
    virtual bool write (std::shared_ptr<std::vector<char>> msg) = 0;
    /* */
    std::string get_protocol(void) { return this->protocol_;};
    std::string get_name    (void) { return this->name_    ;};
    /* handle thread */
    void start(void);
    void join (void);
    /* handle Observer */
    void attachObservers (std::shared_ptr<ProtocolBase> observer);
    void notifyObservers (std::shared_ptr<std::vector<char>> msg);
  /* */
  protected:
    /* */
    bool init_ptr (void);
    bool setup_ptr(void);
    /* */
    std::shared_ptr<Logger>     logger_;
    std::shared_ptr<DataFormat> datas_ ;
    /* */
    std::shared_ptr<std::vector<char>> buffer_in_ ;
    std::shared_ptr<std::vector<char>> buffer_out_;
  /* */
  private:
    std::string sensor_name_;
    std::string protocol_   ;
    std::string name_       ;
    std::string log_level_  ;
    std::thread thread_     ;  /* thread */
    std::vector<std::shared_ptr<ProtocolBase>> observers_; /* is not a callback anymore */
};

#endif //PROTOCOL_BASE_HPP