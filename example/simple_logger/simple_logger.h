#ifndef __SIMPLE_LOGGER_H__
#define __SIMPLE_LOGGER_H__

#include <string>
#include "spdlog_json_config.h"

/**
 * @brief  define a logger and obtain the logger by its constant name
 *
 * This is an example of general way to use the SpdlogJsonConfig.
 * 1. Define a constant logger name, for example, "SIMPLE".
 * 2. Create a logger configuration file which contains the logger config for "SIMPLE"
 * 3. Invoke SpdlogJsonConfig::Initialize() to initialize SpdlogJsonConfig with the configuration file
 * 4. Obtain the logger "SIMPLE" by its name to log.
 *    For convenience, define macros in this example.
 */


const static char* SIMPLE_LOGGER_NAME = "SIMPLE";


#define GET_LOGGER(id) spdlog_json_config::SpdlogJsonConfig::GetInstance()->GetLogger(id)

#define LOGD(fmt, ...) GET_LOGGER(SIMPLE_LOGGER_NAME)->debug(fmt, ##__VA_ARGS__)
#define LOGI(fmt, ...) GET_LOGGER(SIMPLE_LOGGER_NAME)->info(fmt, ##__VA_ARGS__)
#define LOGW(fmt, ...) GET_LOGGER(SIMPLE_LOGGER_NAME)->warn(fmt, ##__VA_ARGS__)
#define LOGE(fmt, ...) GET_LOGGER(SIMPLE_LOGGER_NAME)->error(fmt, ##__VA_ARGS__)
#define LOGC(fmt, ...) GET_LOGGER(SIMPLE_LOGGER_NAME)->critical(fmt, ##__VA_ARGS__)

bool initialize_simple_logger(const std::string& config_file){
    return spdlog_json_config::SpdlogJsonConfig::GetInstance()->Initialize(config_file);
}


#endif //__SIMPLE_LOGGER_H__
