#define CATCH_CONFIG_MAIN
#include "catch.hpp"


#include "spdlog_json_config.h"

static const char* PARSER_LOGGER_NAME = "PARSER";
static uint32_t    PARSER_LOGGER_ID;


#ifndef PARSER_MACRO
#define PARSER_MACRO

#define GET_LOGGER(id) spdlog_json_config::SpdlogJsonConfig::GetInstance()->GetLogger(id)
#define PARSERLOGD(fmt, ...) GET_LOGGER(PARSER_LOGGER_NAME)->debug(fmt, ##__VA_ARGS__)
#define PARSERLOGI(fmt, ...) GET_LOGGER(PARSER_LOGGER_NAME)->info(fmt, ##__VA_ARGS__)
#define PARSERLOGW(fmt, ...) GET_LOGGER(PARSER_LOGGER_NAME)->warn(fmt, ##__VA_ARGS__)
#define PARSERLOGE(fmt, ...) GET_LOGGER(PARSER_LOGGER_NAME)->error(fmt, ##__VA_ARGS__)
#define PARSERLOGC(fmt, ...) GET_LOGGER(PARSER_LOGGER_NAME)->critical(fmt, ##__VA_ARGS__)

#define PARSERID_LOGD(fmt, ...) GET_LOGGER(PARSER_LOGGER_ID)->debug(fmt, ##__VA_ARGS__)
#define PARSERID_LOGI(fmt, ...) GET_LOGGER(PARSER_LOGGER_ID)->info(fmt, ##__VA_ARGS__)
#define PARSERID_LOGW(fmt, ...) GET_LOGGER(PARSER_LOGGER_ID)->warn(fmt, ##__VA_ARGS__)
#define PARSERID_LOGE(fmt, ...) GET_LOGGER(PARSER_LOGGER_ID)->error(fmt, ##__VA_ARGS__)
#define PARSERID_LOGC(fmt, ...) GET_LOGGER(PARSER_LOGGER_ID)->critical(fmt, ##__VA_ARGS__)

#endif


TEST_CASE("Test MACRO", "[MACRO_PARSER]"){

    spdlog::shutdown();
    REQUIRE(spdlog_json_config::SpdlogJsonConfig::GetInstance()->Initialize("./parser_logger_config.json") == true);

    REQUIRE(spdlog_json_config::SpdlogJsonConfig::GetInstance()->GetLoggerId(PARSER_LOGGER_NAME, PARSER_LOGGER_ID) == true);
    
    // Get logger by name
    PARSERLOGI("Hello {}", "world");
    PARSERLOGD("{1} {0}", "Hello", "World");
    PARSERLOGI("{1} {0}", "Hello", "World");
    PARSERLOGW("{:<30}", "left aligned 30");
    PARSERLOGW("{:>30}", "right aligned 30");
    PARSERLOGW("{:=^30}", "centered aligned 30");
    PARSERLOGC("Terrible Critical");


    PARSERLOGI("{:*<{}}", "dynamic width left align 60", 60);
    PARSERLOGI("dynamic precision(4) 3.1415926: {:.{}f}", 3.1415926, 4);

    PARSERLOGI("{:+f}; {:+f}", 3.14, -3.14); // show it always
    PARSERLOGI("{: f}; {: f}", 3.14, -3.14); // show a space for positive numbers
    PARSERLOGI("{:-f}; {:-f}", 3.14, -3.14); // show only the minus -- same as '{:f}; {:f}'
    PARSERLOGI("{:*=+10d}; {:*=+10d}", 314, -314); // show only the minus -- same as '{:f}; {:f}'

    PARSERLOGI("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    PARSERLOGI("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42);
    PARSERLOGI("{0:#04x}, {1:#04x}, {0:#08x}, {1:#08x}", 0, 0xa);

    // Get logger by id
    PARSERID_LOGI("Hello {}", "world");
    PARSERID_LOGD("{1} {0}", "Hello", "World");
    PARSERID_LOGI("{1} {0}", "Hello", "World");
    PARSERID_LOGW("{:<30}", "left aligned 30");
    PARSERID_LOGW("{:>30}", "right aligned 30");
    PARSERID_LOGW("{:=^30}", "centered aligned 30");
    PARSERID_LOGC("Terrible Critical");


    PARSERID_LOGI("{:*<{}}", "dynamic width left align 60", 60);
    PARSERID_LOGI("dynamic precision(4) 3.1415926: {:.{}f}", 3.1415926, 4);

    PARSERID_LOGI("{:+f}; {:+f}", 3.14, -3.14); // show it always
    PARSERID_LOGI("{: f}; {: f}", 3.14, -3.14); // show a space for positive numbers
    PARSERID_LOGI("{:-f}; {:-f}", 3.14, -3.14); // show only the minus -- same as '{:f}; {:f}'
    PARSERID_LOGI("{:*=+10d}; {:*=+10d}", 314, -314); // show only the minus -- same as '{:f}; {:f}'

    PARSERID_LOGI("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    PARSERID_LOGI("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42);
    PARSERID_LOGI("{0:#04x}, {1:#04x}, {0:#08x}, {1:#08x}", 0, 0xa);


}



