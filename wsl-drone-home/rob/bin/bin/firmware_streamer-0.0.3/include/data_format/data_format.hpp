#include <dependencies.hpp>
/* */
#ifndef DATA_FORMAT_HPP
#define DATA_FORMAT_HPP
/* */
template <typename T> union type_to_bytes
{
  T       value;
  uint8_t bytes[sizeof(T)];
};
/* */
template <typename T> float convert_byte_to_value(std::vector<char> & buffer, const unsigned int & bytes, T data)
{
  for (int i = 0; i < bytes; i++)
  {
    uint8_t byte = buffer.front();
    data.bytes[i] = byte;
    /* delete first*/
    buffer.erase(buffer.begin());
  }
  float value_converted = (float)data.value;
  /* */
  return value_converted;
};


struct DataStruct
{
  unsigned int bytes;
  std::string  label;
  std::string  type ;
  float        value;
};


class DataFormat
{
  /* */
  public:
    DataFormat(const std::string & sensor_name, const std::string & name, const std::string & path, const std::string & log_level);
    ~DataFormat(void);
    /* */
    int get_bytes(void);
    std::shared_ptr<std::vector<DataStruct>> get_datas();
    bool parse_datas(std::shared_ptr<std::vector<char>> buffer);
  /* */
  protected:
    /* */
    bool init_ptr(void);
    bool setup(void);
    /* */
    std::shared_ptr<Logger>                  logger_;
    std::shared_ptr<std::vector<DataStruct>> datas_ ;
  /* */
  private:
    /* */
    bool parse_data(std::vector<char> & buffer, DataStruct & data_struct);
    /* */
    std::string  sensor_name_;
    std::string  name_       ;
    std::string  path_       ;
    std::string  log_level_  ;
    unsigned int bytes_      ;
    /* */
};

#endif // DATA_FORMAT_HPP