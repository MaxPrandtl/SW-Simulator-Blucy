#include <dependencies.hpp>
#include <connections/connections.hpp>

#define PERIOD_MS    1000
#define TOT_TIME_MS 20000

const std::chrono::milliseconds period(PERIOD_MS);
const std::chrono::milliseconds tot_time(TOT_TIME_MS);

std::chrono::steady_clock::time_point startExecutionTime;
std::chrono::steady_clock::time_point lastExecutionTime;


int main(int argc, char* argv[])
{
  /* check conf.json args */
  if (argc != 2)
  {
    std::cerr << "usage: ./run_connection ../config/connection/config.json" << std::endl;
    return 1;
  }
  /* */
  std::string config_filename = argv[1];
  /* load conf.json */
  std::ifstream file (config_filename);
  nlohmann::json config;
  file >> config;
  /* init ptr */
  std::shared_ptr<Connection> connection = std::make_shared<Connection>(config);
  std::shared_ptr<Logger> logger = std::make_shared<Logger>(std::string(config["name"]) + " : run_connection", "info");
  /* */
  connection->open();
  /* */
  connection->setup();
  /* */
  connection->start();
  /* */
  startExecutionTime = std::chrono::steady_clock::now();
  /* */
  while (true)
  {
    lastExecutionTime = std::chrono::steady_clock::now() + period;
    logger->info("running.. ");
    std::this_thread::sleep_until(lastExecutionTime);
  }
  /* */
  connection->stop();
  /* */
  connection->join();
  /* */
  return 0;
}