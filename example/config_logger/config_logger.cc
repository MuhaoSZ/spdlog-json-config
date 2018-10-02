#include <string>
#include "config_logger.h"


unsigned int DEMO_LOGGER_ID = 0;

bool initialize_demo_logger(const std::string& config_file){
    spdlog_json_config::SpdlogJsonConfig* instance = spdlog_json_config::SpdlogJsonConfig::GetInstance();
    if (!instance->Initialize(config_file)){
        printf("Fail to initialize logger from config file %s\n", config_file.c_str());
        return false;
    }
    else {
        DEMO_LOGI("LOGGER {} started.", DEMO_LOGGER_NAME);
    }

    // After initialize, invoke SpdLogger::GetLoggerId() to update the DEMO_LOGGER_ID
    if(!instance->GetLoggerId(DEMO_LOGGER_NAME, DEMO_LOGGER_ID)){
        printf("Fail to logger ID of %s\n", DEMO_LOGGER_NAME);
        return false;
    }
    else {
        DEMOID_LOGI("LOGGER {} get id. LOGGER ID = {}", DEMO_LOGGER_NAME, DEMO_LOGGER_ID);
    }

    return true;
}
