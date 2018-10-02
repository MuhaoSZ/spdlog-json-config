#ifndef __CONFIG_LOGGER_H__
#define __CONFIG_LOGGER_H__

#include <string>
#include "spdlog_json_config.h"

/**
 * @brief  define a logger and obtain the logger by its constant name or id.
 *
 * There is a constant logger name and a logger id after SpdlogJsonConfig creates a logger.
 * Obtain a logger via its id is directly get it from a array.
 *
 * The steps is almost the same as the "simple_logger" example.
 * Here declare the logger id variable "DEMO_LOGGER_ID" and initialize function
 * "initialize_demo_logger" in header file. The implementation is put in a .cc source file.
 *
 * 1. Define a constant logger name, and declare a logger id.
 * 2. Create a logger configuration file which contains the logger config for "DEMO".
 * 3. Declare initialize function "initialize_demo_logger". In the function, initialize
 *    SpdlogJsonConfig and "DEMO_LOGGER_ID".
 * 4. Obtain the logger "SIMPLE" by its name or its id to log.
 *    For convenience, define macros.
 */

static const char*  DEMO_LOGGER_NAME = "DEMO";
extern unsigned int DEMO_LOGGER_ID;

#define GET_LOGGER(id)     spdlog_json_config::SpdlogJsonConfig::GetInstance()->GetLogger(id)

// Get logger by name
#define DEMO_LOGD(fmt, ...) GET_LOGGER(DEMO_LOGGER_NAME)->debug(fmt, ##__VA_ARGS__)
#define DEMO_LOGI(fmt, ...) GET_LOGGER(DEMO_LOGGER_NAME)->info(fmt, ##__VA_ARGS__)
#define DEMO_LOGW(fmt, ...) GET_LOGGER(DEMO_LOGGER_NAME)->warn(fmt, ##__VA_ARGS__)
#define DEMO_LOGE(fmt, ...) GET_LOGGER(DEMO_LOGGER_NAME)->error(fmt, ##__VA_ARGS__)
#define DEMO_LOGC(fmt, ...) GET_LOGGER(DEMO_LOGGER_NAME)->critical(fmt, ##__VA_ARGS__)

// Get logger by id
#define DEMOID_LOGD(fmt, ...) GET_LOGGER(DEMO_LOGGER_ID)->debug(fmt, ##__VA_ARGS__)
#define DEMOID_LOGI(fmt, ...) GET_LOGGER(DEMO_LOGGER_ID)->info(fmt, ##__VA_ARGS__)
#define DEMOID_LOGW(fmt, ...) GET_LOGGER(DEMO_LOGGER_ID)->warn(fmt, ##__VA_ARGS__)
#define DEMOID_LOGE(fmt, ...) GET_LOGGER(DEMO_LOGGER_ID)->error(fmt, ##__VA_ARGS__)
#define DEMOID_LOGC(fmt, ...) GET_LOGGER(DEMO_LOGGER_ID)->critical(fmt, ##__VA_ARGS__)

extern bool initialize_demo_logger(const std::string& config_file);


#endif //__CONFIG_LOGGER_H__
