#include <data_format/data_format.hpp>


DataFormat::DataFormat(const std::string & sensor_name, const std::string & name, const std::string & path, const std::string & log_level)
{
  this->sensor_name_ = sensor_name;
  this->name_        = name       ;
  this->path_        = path       ;
  this->log_level_   = log_level  ;
  /* init ptr */
  this->init_ptr();
  /* */
  this->setup();
}

DataFormat::~DataFormat(void){};


bool DataFormat::init_ptr(void)
{
  if (!this->logger_)
    this->logger_ = std::make_shared<Logger>((this->sensor_name_ + " : data_format : " + this->name_), this->log_level_);
  if (!this->datas_)
    this->datas_  = std::make_shared<std::vector<DataStruct>>();
  return true;
}

bool DataFormat::setup(void)
{
  /* load file */
  std::string path_name = this->path_ + "/" + this->name_  + ".json";
  std::ifstream  file(path_name);
  nlohmann::json format;
  if (!file.is_open())
  {
    this->logger_->error("Could not open the file!");
    return false;
  }
  file >> format;
  this->logger_->info("loaded.");
  /* */
  this->bytes_ = 0;
  /* generate data */
  for (const auto & data : format["datas"])
  {
    /* create data */
    DataStruct data_struct;
    /* read data */
    int bytes         = data["bytes"];
    std::string type  = data["type" ];
    std::string label = data["label"];
    /* create data */
    data_struct.bytes = bytes;
    data_struct.label = label;
    data_struct.type  = type ;
    /* add data */
    this->datas_->push_back(data_struct);
    /* compute size */
    this->bytes_ += data_struct.bytes;
  }
  /* */
  this->logger_->info("data length: " + std::to_string(this->bytes_));
  /* */
  return true;
}


int DataFormat::get_bytes(void)
{
  /* */
  return this->bytes_;
}


std::shared_ptr<std::vector<DataStruct>> DataFormat::get_datas()
{
  /* */
  return this->datas_;
}


bool DataFormat::parse_datas(std::shared_ptr<std::vector<char>> buffer)
{
  std::vector<char> buffer_copy = *buffer;
  for (auto & data_struct : *this->datas_)
    this->parse_data(buffer_copy, data_struct);
  /* */
  return true;
}


bool DataFormat::parse_data(std::vector<char> & buffer, DataStruct & data_struct)
{
  /* get data_type  */
  if (data_struct.type == "int")
  {
    if (data_struct.bytes == 1)
    {
      type_to_bytes<uint8_t> data;
      data_struct.value = convert_byte_to_value(buffer, data_struct.bytes, data);
    }
    else if (data_struct.bytes == 2)
    {
      type_to_bytes<uint16_t> data;
      data_struct.value = convert_byte_to_value(buffer, data_struct.bytes, data);
    }
    else if (data_struct.bytes == 4)
    {
      type_to_bytes<uint32_t> data;
      data_struct.value = convert_byte_to_value(buffer, data_struct.bytes, data);
    }
  }
  else if (data_struct.type == "float")
  {
    type_to_bytes<float> data;
    data_struct.value = convert_byte_to_value(buffer, data_struct.bytes, data);
  }
  /* */
  return true;
}