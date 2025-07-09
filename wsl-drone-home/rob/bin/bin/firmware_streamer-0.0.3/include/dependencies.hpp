#ifndef DEPENDENCIES_HPP
#define DEPENDENCIES_HPP

#pragma once

/* std */
#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <mutex>
#include <memory>
#include <thread>
#include <variant>
/* installed */
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <mosquittopp.h>
#include <spdlog/spdlog.h>
#include "spdlog/sinks/stdout_color_sinks.h"

/* local */
#include <nlohmann/json.hpp>
#include <logger/logger.hpp>

#endif // DEPENDENCIES_HPP