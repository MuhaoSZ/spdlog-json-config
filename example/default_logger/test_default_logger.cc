#include "spdlog_json_config.h"

/**
 * @brief  Use default logger
 *
 * The SpdlogJsonConfig define a default logger which can be directly.
 * the default logger can be obtained by using the "DEFAULT_LOGGER_NAME" or "DEFAULT_LOGGER_ID".
 * The example here defines the macros for the default logger.
 *
 * The benefit to use default logger is that we don't need a configuration file.
 */


#ifndef DEFINE_SPDLOG_DEFAULT_LOGGER
#define DEFINE_SPDLOG_DEFAULT_LOGGER


#define GET_LOGGER(id)     spdlog_json_config::SpdlogJsonConfig::GetInstance()->GetLogger(id)

#define DEFAULT_NAME_LOGD(fmt, ...) GET_LOGGER(spdlog_json_config::DEFAULT_LOGGER_NAME)->debug(fmt, ##__VA_ARGS__)
#define DEFAULT_NAME_LOGI(fmt, ...) GET_LOGGER(spdlog_json_config::DEFAULT_LOGGER_NAME)->info(fmt, ##__VA_ARGS__)
#define DEFAULT_NAME_LOGW(fmt, ...) GET_LOGGER(spdlog_json_config::DEFAULT_LOGGER_NAME)->warn(fmt, ##__VA_ARGS__)
#define DEFAULT_NAME_LOGE(fmt, ...) GET_LOGGER(spdlog_json_config::DEFAULT_LOGGER_NAME)->error(fmt, ##__VA_ARGS__)
#define DEFAULT_NAME_LOGC(fmt, ...) GET_LOGGER(spdlog_json_config::DEFAULT_LOGGER_NAME)->critical(fmt, ##__VA_ARGS__)

#define DEFAULT_ID_LOGD(fmt, ...) GET_LOGGER(spdlog_json_config::DEFAULT_LOGGER_ID)->debug(fmt, ##__VA_ARGS__)
#define DEFAULT_ID_LOGI(fmt, ...) GET_LOGGER(spdlog_json_config::DEFAULT_LOGGER_ID)->info(fmt, ##__VA_ARGS__)
#define DEFAULT_ID_LOGW(fmt, ...) GET_LOGGER(spdlog_json_config::DEFAULT_LOGGER_ID)->warn(fmt, ##__VA_ARGS__)
#define DEFAULT_ID_LOGE(fmt, ...) GET_LOGGER(spdlog_json_config::DEFAULT_LOGGER_ID)->error(fmt, ##__VA_ARGS__)
#define DEFAULT_ID_LOGC(fmt, ...) GET_LOGGER(spdlog_json_config::DEFAULT_LOGGER_ID)->critical(fmt, ##__VA_ARGS__)

#endif


int main(int argc, char* argv[]){

    DEFAULT_NAME_LOGI("Hello {}", "world");
    DEFAULT_NAME_LOGD("{1} {0}", "Hello", "World");
    DEFAULT_NAME_LOGI("{1} {0}", "Hello", "World");
    DEFAULT_NAME_LOGW("This is the default logger got by name");
    DEFAULT_NAME_LOGW("{:<30}", "left aligned 30");
    DEFAULT_NAME_LOGW("{:>30}", "right aligned 30");
    DEFAULT_NAME_LOGW("{:=^30}", "centered aligned 30");
    DEFAULT_NAME_LOGC("Terrible Critical");


    DEFAULT_ID_LOGI("This is the default logger got by id");
    DEFAULT_ID_LOGI("{:*<{}}", "dynamic width left align 60", 60);
    DEFAULT_ID_LOGI("dynamic precision(4) 3.1415926: {:.{}f}", 3.1415926, 4);

    DEFAULT_ID_LOGI("{:+f}; {:+f}", 3.14, -3.14); // show it always
    DEFAULT_ID_LOGI("{: f}; {: f}", 3.14, -3.14); // show a space for positive numbers
    DEFAULT_ID_LOGI("{:-f}; {:-f}", 3.14, -3.14); // show only the minus -- same as '{:f}; {:f}'
    DEFAULT_ID_LOGI("{:*=+10d}; {:*=+10d}", 314, -314); // show only the minus -- same as '{:f}; {:f}'

    DEFAULT_ID_LOGI("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    DEFAULT_ID_LOGI("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42);
    DEFAULT_ID_LOGI("{0:#04x}, {1:#04x}, {0:#08x}, {1:#08x}", 0, 0xa);

    return 0;
}


