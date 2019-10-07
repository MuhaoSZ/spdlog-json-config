#ifndef __SPDLOG_JSON_CONFIG_H__
#define __SPDLOG_JSON_CONFIG_H__


#include <string>
#include <memory>
#include <unordered_map>
#include <set>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <libgen.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <assert.h>

#include "rapidjson/document.h"
#include "rapidjson/error/en.h"
#include "spdlog/spdlog.h"
#include "spdlog/logger.h"
#include "spdlog/async_logger.h"
#include "spdlog/details/registry.h"
#include "spdlog/details/thread_pool.h"
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#ifdef _WIN32
#include "spdlog/sinks/wincolor_sink.h"
#else
#include "spdlog/sinks/ansicolor_sink.h"
#include "spdlog/sinks/syslog_sink.h"
#endif

#include "spdlog/sinks/basic_file_sink.h"
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"


namespace spdlog_json_config {

// The default logger name and id.
const static char*    DEFAULT_LOGGER_NAME = "DEFAULT";
const static uint32_t DEFAULT_LOGGER_ID = 0;

/**
 * @brief class SpdlogJsonConfig reads configuration from json file and initialize spd logger
 *
 * Usage:
 *   1. Get an instance and initialize
 *
 *          SpdlogJsonConfig* instance = SpdlogJsonConfig::GetInstance();
 *          instance->Initialize("config.json");
 *
 *   2. Get logger via logger name to logging
 *
 *          SpdlogJsonConfig::GetInstance()->GetLogger("foo")->debug("log something");
 *
 *      OR get logger id via logger name, and get logger via logger id.
 *
 *          uint32_t foo_logger_id;
 *          SpdlogJsonConfig::GetInstance()->GetLoggerId("foo", foo_logger_id);
 *          SpdlogJsonConfig::GetInstance()->GetLogger(foo_logger_id)->debug("log something");
 *
 */
class SpdlogJsonConfig {
public:
    const constexpr static char* __CLASS__ = "SpdlogJsonConfig";
    const static uint32_t MAX_LOGGER_NUM = 32;  ///< Max number logger supported in logger manager

    const constexpr static char* CONFIG_KEYWORD_SINKS      = "SINKS";
    const constexpr static char* CONFIG_KEYWORD_PATTERNS   = "PATTERNS";
    const constexpr static char* CONFIG_KEYWORD_LOGGERS    = "LOGGERS";
    const constexpr static char* CONFIG_KEYWORD_THREADPOOL = "THREAD_POOL";

    const constexpr static char* SINK_TYPE_STDOUT_SINK_ST           = "stdout_sink_st";
    const constexpr static char* SINK_TYPE_STDOUT_SINK_MT           = "stdout_sink_mt";
    const constexpr static char* SINK_TYPE_STDERR_SINK_ST           = "stderr_sink_st";
    const constexpr static char* SINK_TYPE_STDERR_SINK_MT           = "stderr_sink_mt";
    const constexpr static char* SINK_TYPE_STDOUT_COLOR_SINK_ST     = "stdout_color_sink_st";
    const constexpr static char* SINK_TYPE_STDOUT_COLOR_SINK_MT     = "stdout_color_sink_mt";
    const constexpr static char* SINK_TYPE_STDERR_COLOR_SINK_ST     = "stderr_color_sink_st";
    const constexpr static char* SINK_TYPE_STDERR_COLOR_SINK_MT     = "stderr_color_sink_mt";
    #ifndef _WIN32
    const constexpr static char* SINK_TYPE_SYSLOG_SINK_ST           = "syslog_sink_st";
    const constexpr static char* SINK_TYPE_SYSLOG_SINK_MT           = "syslog_sink_mt";
	#endif
    const constexpr static char* SINK_TYPE_BASIC_FILE_SINK_ST       = "basic_file_sink_st";
    const constexpr static char* SINK_TYPE_BASIC_FILE_SINK_MT       = "basic_file_sink_mt";
    const constexpr static char* SINK_TYPE_DAILY_FILE_SINK_ST       = "daily_file_sink_st";
    const constexpr static char* SINK_TYPE_DAILY_FILE_SINK_MT       = "daily_file_sink_mt";
    const constexpr static char* SINK_TYPE_ROTATING_FILE_SINK_ST    = "rotating_file_sink_st";
    const constexpr static char* SINK_TYPE_ROTATING_FILE_SINK_MT    = "rotating_file_sink_mt";


    SpdlogJsonConfig(const spdlog::logger&) = delete;
    SpdlogJsonConfig& operator=(const spdlog::logger&) = delete;
    virtual ~SpdlogJsonConfig() {}

    /// @brief  Get the singleton instance
    static SpdlogJsonConfig* GetInstance(){
        static SpdlogJsonConfig instance;  // Only initialize one time
        return &instance;
    }

    /// @brief  Create logger according  to configuration
    bool Initialize(const std::string& config_file) {
        // Read configuration file
        std::string file_content;
        if(!ReadFile(config_file, file_content)) {
            return false;
        }

        // parse configuration file with rapidjson
        // and create spdlog::logger according to configuration
        if(!ConfigLogger(file_content)){
            return false;
        }

        return true;
    }

    /// @brief Get shared_ptr to spdlog::logger by name
    ///
    /// @param logger_name  logger name
    /// @return             shard_ptr to spdlog::logger
    std::shared_ptr<spdlog::logger> GetLogger(const std::string& logger_name){
        return spdlog::get(logger_name);
    }

    /// @brief Get shared_ptr to spdlog::logger by logger id
    ///
    /// @param logger_id    logger id
    /// @return             shard_ptr to spdlog::logger
    std::shared_ptr<spdlog::logger> GetLogger(uint32_t logger_id){
        return logger_table_[logger_id];
    }

    /// @brief  Get logger id by logger name
    ///
    /// @param  [in] logger_name     the logger name
    /// @param  [out] logger_id      the logger id corresponding to the logger name
    /// @return true if success, otherwise false
    bool GetLoggerId(const std::string& logger_name, uint32_t& logger_id) {
        std::unordered_map<std::string, uint32_t>::iterator it;
        it = name_to_id_.find(logger_name);
        if(it == name_to_id_.end()){
            printf("%s::%s: Logger %s not found\n", __CLASS__, __FUNCTION__, logger_name.c_str());
            return false;
        }

        logger_id = it->second;
        return true;
    }

private:

    /// @brief  Default constructor. Create the default logger.
    SpdlogJsonConfig(){
        for(uint32_t i = 0; i < MAX_LOGGER_NUM; i++){
            logger_table_[i] = nullptr;
        }
        logger_count_ = 0;

        supported_sink_type_.insert(SINK_TYPE_STDOUT_SINK_ST);
        supported_sink_type_.insert(SINK_TYPE_STDOUT_SINK_MT);
        supported_sink_type_.insert(SINK_TYPE_STDERR_SINK_ST);
        supported_sink_type_.insert(SINK_TYPE_STDERR_SINK_MT);
        supported_sink_type_.insert(SINK_TYPE_STDOUT_COLOR_SINK_ST);
        supported_sink_type_.insert(SINK_TYPE_STDOUT_COLOR_SINK_MT);
        supported_sink_type_.insert(SINK_TYPE_STDERR_COLOR_SINK_ST);
        supported_sink_type_.insert(SINK_TYPE_STDERR_COLOR_SINK_MT);
        #ifndef _WIN32
        supported_sink_type_.insert(SINK_TYPE_SYSLOG_SINK_ST);
        supported_sink_type_.insert(SINK_TYPE_SYSLOG_SINK_MT);
		#endif
        supported_sink_type_.insert(SINK_TYPE_BASIC_FILE_SINK_ST);
        supported_sink_type_.insert(SINK_TYPE_BASIC_FILE_SINK_MT);
        supported_sink_type_.insert(SINK_TYPE_DAILY_FILE_SINK_ST);
        supported_sink_type_.insert(SINK_TYPE_DAILY_FILE_SINK_MT);
        supported_sink_type_.insert(SINK_TYPE_ROTATING_FILE_SINK_ST);
        supported_sink_type_.insert(SINK_TYPE_ROTATING_FILE_SINK_MT);

        DEFAULT_PATTERN = std::string("[%C-%m-%d %H:%M:%S.%e][%n]%^[%L]%$ %v");
        DEFAULT_SINK    = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
        DEFAULT_LOGGER  = std::make_shared<spdlog::logger>(DEFAULT_LOGGER_NAME, DEFAULT_SINK);
        DEFAULT_LOGGER->set_pattern(DEFAULT_PATTERN);
        DEFAULT_LOGGER->set_level(spdlog::level::info);

        spdlog::register_logger(DEFAULT_LOGGER);
        logger_table_[logger_count_] = GetLogger(DEFAULT_LOGGER->name());
        SetLoggerId(DEFAULT_LOGGER->name(), logger_count_);
        assert(DEFAULT_LOGGER_ID == logger_count_);   // The DEFAULT_LOGGER_ID must be equal to 0.
        logger_count_++;
    }

    /// @brief  Associate the logger name to logger id
    ///
    /// @param  [in] logger_name    the logger name
    /// @param  [in] logger_id      the logger id corresponding to the logger name
    /// @return true if success, otherwise false
    bool SetLoggerId(const std::string& logger_name, uint32_t logger_id) {
        std::unordered_map<std::string, uint32_t>::iterator it;
        it = name_to_id_.find(logger_name);
        if(it != name_to_id_.end()){
            printf("%s::%s: Logger %s already existed\n", __CLASS__, __FUNCTION__, logger_name.c_str());
            return false;
        }

        name_to_id_[logger_name] = logger_id;
        return true;
    }

    bool ReadFile(const std::string& file_path, std::string& content){
        long  filesize = 0;
        char* buffer = nullptr;
        size_t result;

        FILE* f = fopen(file_path.c_str(), "rb");

        if (f == NULL) {
            printf("%s::%s: Fail to open configuration file: %s\n",
                   __CLASS__, __FUNCTION__, file_path.c_str());
            return false;
        }

        fseek(f, 0, SEEK_END);
        filesize = ftell(f);
        rewind(f);

        buffer = (char*)malloc(sizeof(char) * filesize);
        if (buffer == NULL) {
            printf("%s::%s: Fail to allocate memory for buffer. file size = %lld\n",
                   __CLASS__, __FUNCTION__, filesize);
            return false;
        }

        result = fread(buffer, 1, filesize, f);
        if ((long)result != filesize) {
            printf("%s::%s: Fail to read all content from file: %s\n",
                   __CLASS__, __FUNCTION__, file_path.c_str());
            return false;
        }

        content.clear();
        content.append(buffer, filesize);

        fclose(f);
        free(buffer);

        return true;
    }

    bool ConfigLogger(const std::string& content) {
        rapidjson::Document doc;
        rapidjson::Document::AllocatorType& allocator = doc.GetAllocator();

        doc.Parse<rapidjson::kParseCommentsFlag>(content.c_str());
        if (doc.HasParseError()) {
            rapidjson::ParseErrorCode code = doc.GetParseError();
            printf("%s::%s: rapidjson parse error: %s (%lu)\n",
                   __CLASS__, __FUNCTION__, rapidjson::GetParseError_En(code), doc.GetErrorOffset());
            printf("%s::%s: rapidjson parse error, json pos: %s\n",
                   __CLASS__, __FUNCTION__, content.c_str() + doc.GetErrorOffset());
            return false;
        }

        rapidjson::Value::ConstMemberIterator it;

        //
        // Get thread pool configuration
        //
        uint32_t thread_count = 1;
        uint32_t queue_size = 8192;
        it = doc.FindMember(CONFIG_KEYWORD_THREADPOOL);
        if(it != doc.MemberEnd()){
            const rapidjson::Value& threadpool_config = it->value;
            if ((it = threadpool_config.FindMember("thread_count")) != threadpool_config.MemberEnd()){
                thread_count = it->value.GetUint();
            }

            if ((it = threadpool_config.FindMember("queue_size")) != threadpool_config.MemberEnd()){
                queue_size = it->value.GetUint();
            }
        }

        // create thread pool
        std::shared_ptr<spdlog::details::thread_pool> tp =
                std::make_shared<spdlog::details::thread_pool>(queue_size, thread_count);
        spdlog::details::registry::instance().set_tp(tp);


        //
        // Get sinks configuration
        //

        // store sink configuration according to name
        std::unordered_map<std::string, rapidjson::Value> sink_config;
        it = doc.FindMember(CONFIG_KEYWORD_SINKS);
        if(it != doc.MemberEnd()){
            const rapidjson::Value& sinks = it->value;
            for(it = sinks.MemberBegin(); it != sinks.MemberEnd(); it++){
                sink_config[std::string(it->name.GetString())] = rapidjson::Value(it->value, allocator);
            }
        }

        //
        // Get patterns configuration
        //
        std::unordered_map<std::string, std::string> pattern_config;
        it = doc.FindMember(CONFIG_KEYWORD_PATTERNS);
        if(it != doc.MemberEnd()){
            const rapidjson::Value& patterns = it->value;
            for(it = patterns.MemberBegin(); it != patterns.MemberEnd(); it++){
                pattern_config[it->name.GetString()] = std::string(it->value.GetString());
            }
        }

        //
        // Get loggers configuration and create logger
        //
        it = doc.FindMember(CONFIG_KEYWORD_LOGGERS);
        if(it != doc.MemberEnd()){
            try{
                // list to store sinks that will be used in a logger
                std::vector<std::shared_ptr<spdlog::sinks::sink>> sink_list;
                const rapidjson::Value& loggers_config = it->value;
                for(it = loggers_config.MemberBegin(); it != loggers_config.MemberEnd(); it++){
                    sink_list.clear();
                    std::string logger_name(it->name.GetString());    // get logger name
                    const rapidjson::Value& logger_param = it->value; // configuration for the logger

                    //
                    // get sinks for the logger
                    //
                    rapidjson::Value::ConstMemberIterator param_it;
                    param_it = logger_param.FindMember("sinks");
                    if(param_it != logger_param.MemberEnd()){
                        if(!param_it->value.IsArray()){
                            printf("%s::%s: sinks value in config file is not an array\n", __CLASS__, __FUNCTION__);
                            return false;
                        }
                        else {
                            // get the sink of this logger.
                            // If the sink required not created yet, create it as per configuration
                            rapidjson::Value::ConstValueIterator value_it;
                            for(value_it = param_it->value.Begin(); value_it != param_it->value.End(); value_it++){
                                std::string sink_name(value_it->GetString());
                                std::unordered_map<std::string, std::shared_ptr<spdlog::sinks::sink>>::iterator got;
                                got = sink_map_.find(sink_name);
                                if(got == sink_map_.end()){
                                    // Get sink configuration and create.
                                    std::unordered_map<std::string, rapidjson::Value>::iterator sink_config_it;
                                    sink_config_it = sink_config.find(sink_name);
                                    if(sink_config_it == sink_config.end()){
                                        printf("%s::%s: sink '%s' not define in config file\n",
                                               __CLASS__, __FUNCTION__, sink_name.c_str());
                                        return false;
                                    }

                                    if(!GenerateSink(sink_name, sink_config_it->second, pattern_config)){
                                        printf("%s::%s: Generate sink '%s' failure\n",
                                               __CLASS__, __FUNCTION__, sink_name.c_str());
                                        return false;
                                    }
                                }

                                sink_list.push_back(sink_map_[sink_name]);
                            }
                        }
                    }
                    else {
                        printf("%s::%s: No sinks defined for logger '%s'. Use default sink",
                               __CLASS__, __FUNCTION__, logger_name.c_str());
                        sink_list.push_back(DEFAULT_SINK);
                    }

                    //
                    // get pattern for logger
                    //
                    std::string logger_pattern;
                    param_it = logger_param.FindMember("pattern");
                    if(param_it != logger_param.MemberEnd()){
                        std::unordered_map<std::string, std::string>::iterator it =
                            pattern_config.find(std::string(param_it->value.GetString()));
                        if(it == pattern_config.end()){
                            printf("%s::%s: No pattern '%s' defined for logger '%s'.\n",
                                   __CLASS__, __FUNCTION__, param_it->value.GetString(), logger_name.c_str());
                            return false;
                        }
                        else{
                            logger_pattern = it->second;
                        }
                    }
                    else {
                        printf("%s::%s: No pattern defined for logger '%s'. Use default pattern\n",
                               __CLASS__, __FUNCTION__, logger_name.c_str());
                        logger_pattern = DEFAULT_PATTERN;
                    }

                    //
                    // get level for logger
                    //
                    std::string logger_level;
                    param_it = logger_param.FindMember("level");
                    if(param_it != logger_param.MemberEnd()){
                        logger_level = std::string(param_it->value.GetString());
                    }
                    else {
                        printf("%s::%s: No level defined for logger '%s'. Use default level 'info'\n",
                               __CLASS__, __FUNCTION__, logger_name.c_str());
                        logger_level = std::string("info");
                    }

                    //
                    // get sync type for logger
                    //
                    std::string sync_type("sync");
                    param_it = logger_param.FindMember("sync_type");
                    if(param_it != logger_param.MemberEnd()){
                        sync_type = std::string(param_it->value.GetString());
                    }

                    // Create logger according to sync_type
                    std::shared_ptr<spdlog::logger> logger;
                    if (sync_type == "sync"){
                        logger = CreateSync(logger_name, sink_list);
                    }
                    else if(sync_type == "async"){
                        logger = CreateAsync(logger_name, sink_list);
                    }
                    else if(sync_type == "async_nb"){
                        logger = CreateAsyncNB(logger_name, sink_list);
                    }
                    else {
                        fprintf(stderr, "Unknown sync_type: %s\n", sync_type.c_str());
                        return false;
                    }

                    //spdlog::set_async_mode(4096);
                    //auto logger = std::make_shared<spdlog::logger>(logger_name, begin(sink_list), end(sink_list));

                    logger->set_level(spdlog::level::from_str(logger_level));
                    logger->set_pattern(logger_pattern);
                    spdlog::register_logger(logger);

                    logger_table_[logger_count_] = GetLogger(logger_name);
                    if(!SetLoggerId(logger_name, logger_count_)){
                        return false;
                    }
                    logger_count_++;

                    logger->info("Logger started");
                }

            }
            catch(const spdlog::spdlog_ex& ex){
                printf("Logger initialization failure: %s \n", ex.what());
                return false;
            }
        }

        return true;
    }

    bool GenerateSink(const std::string& sink_name,
    		          const rapidjson::Value& value,
					  const std::unordered_map<std::string, std::string>& pattern_config){
        rapidjson::Value::ConstMemberIterator it;
        it = value.FindMember("type");
        if(it == value.MemberEnd()){
            printf("%s::%s: Not find type in sink '%s'\n", __CLASS__, __FUNCTION__, sink_name.c_str());
            return false;
        }

        std::string sink_type(it->value.GetString());
        if(supported_sink_type_.find(sink_type) == supported_sink_type_.end()){
            printf("%s::%s: sink type '%s' not supported\n", __CLASS__, __FUNCTION__, sink_type.c_str());
            return false;
        }

        if(sink_type == SINK_TYPE_STDOUT_SINK_ST){
            sink_map_[sink_name] = std::make_shared<spdlog::sinks::stdout_sink_st>();
        }
        else if(sink_type == SINK_TYPE_STDOUT_SINK_MT){
            sink_map_[sink_name] = std::make_shared<spdlog::sinks::stdout_sink_mt>();
        }
        else if (sink_type == SINK_TYPE_STDERR_SINK_ST){
            sink_map_[sink_name] = std::make_shared<spdlog::sinks::stderr_sink_st>();
        }
        else if(sink_type == SINK_TYPE_STDERR_SINK_MT){
            sink_map_[sink_name] = std::make_shared<spdlog::sinks::stderr_sink_mt>();
        }
        else if (sink_type == SINK_TYPE_STDOUT_COLOR_SINK_ST) {
            sink_map_[sink_name] = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
        }
        else if(sink_type == SINK_TYPE_STDOUT_COLOR_SINK_MT){
            sink_map_[sink_name] = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        }
        else if(sink_type == SINK_TYPE_STDERR_COLOR_SINK_ST){
            sink_map_[sink_name] = std::make_shared<spdlog::sinks::stderr_color_sink_st>();
        }
        else if(sink_type == SINK_TYPE_STDERR_COLOR_SINK_MT){
            sink_map_[sink_name] = std::make_shared<spdlog::sinks::stderr_color_sink_mt>();
        }
		#ifndef _WIN32
        else if(sink_type == SINK_TYPE_SYSLOG_SINK_ST ||
                sink_type == SINK_TYPE_SYSLOG_SINK_MT){

            std::string indent("");
            it = value.FindMember("indent");
            if(it != value.MemberEnd()){
                indent = std::string(it->value.GetString());
            }

            if(sink_type == SINK_TYPE_SYSLOG_SINK_ST){
                sink_map_[sink_name] = std::make_shared<spdlog::sinks::syslog_sink_st>(indent);
            }
            else {
                // SINK_TYPE_SYSLOG_SINK_MT
                sink_map_[sink_name] = std::make_shared<spdlog::sinks::syslog_sink_mt>(indent);
            }
        }
		#endif
        else if(sink_type == SINK_TYPE_BASIC_FILE_SINK_ST ||
                sink_type == SINK_TYPE_BASIC_FILE_SINK_MT){
            std::string file_name;
            bool        truncate;

            // mandatory configuration for daily file sink
            it = value.FindMember("file_name");
            if(it == value.MemberEnd()) file_name = std::string("./log_file.log");
            else file_name = std::string(it->value.GetString());

            it = value.FindMember("truncate");
            if(it == value.MemberEnd()) truncate = false;
            else truncate = it->value.GetBool();

            char buffer[file_name.size() + 1];
            strcpy(buffer, file_name.c_str());
            char* dir_name = dirname(buffer);
            if(strcmp(dir_name, ".") != 0){
                if(!CreateDirectory(dir_name)){
                    printf("%s::%s: Create directory '%s' failure\n", __CLASS__, __FUNCTION__, dir_name);
                    return false;
                }
            }

            if(sink_type == SINK_TYPE_BASIC_FILE_SINK_ST){
                sink_map_[sink_name] = std::make_shared<spdlog::sinks::basic_file_sink_st>(
                                           file_name, truncate);
            }
            else {
                // sink_type == SINK_TYPE_BASIC_FILE_SINK_MT
                sink_map_[sink_name] = std::make_shared<spdlog::sinks::basic_file_sink_mt>(
                                           file_name, truncate);
            }

        }
        else if (sink_type == SINK_TYPE_DAILY_FILE_SINK_ST ||
                 sink_type == SINK_TYPE_DAILY_FILE_SINK_MT) {
            std::string base_file_name;
            int32_t     rotation_hour;
            int32_t     rotation_minute;
            bool        truncate;

            // mandatory configuration for daily file sink
            it = value.FindMember("base_file_name");
            if(it == value.MemberEnd()) base_file_name = std::string("./daily.log");
            else base_file_name = std::string(it->value.GetString());

            it = value.FindMember("rotation_hour");
            if(it == value.MemberEnd()) rotation_hour = 0;
            else rotation_hour = it->value.GetInt();

            it = value.FindMember("rotation_minute");
            if(it == value.MemberEnd()) rotation_minute = 0;
            else rotation_minute = it->value.GetInt();

            it = value.FindMember("truncate");
            if(it == value.MemberEnd()) truncate = false;
            else truncate = it->value.GetBool();

            char buffer[base_file_name.size() + 1];
            strcpy(buffer, base_file_name.c_str());
            char* dir_name = dirname(buffer);
            if(strcmp(dir_name, ".") != 0){
                if(!CreateDirectory(dir_name)){
                    printf("%s::%s: Create directory '%s' failure\n", __CLASS__, __FUNCTION__, dir_name);
                    return false;
                }
            }

            if(sink_type == SINK_TYPE_DAILY_FILE_SINK_ST){
                sink_map_[sink_name] = std::make_shared<spdlog::sinks::daily_file_sink_st>(
                                           base_file_name, rotation_hour, rotation_minute, truncate);
            }
            else {
                // sink_type == SINK_TYPE_DAILY_FILE_SINK_MT
                sink_map_[sink_name] = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
                                           base_file_name, rotation_hour, rotation_minute, truncate);
            }
        }
        else if (sink_type == SINK_TYPE_ROTATING_FILE_SINK_ST ||
                 sink_type == SINK_TYPE_ROTATING_FILE_SINK_MT) {
            std::string base_file_name;
            size_t      max_files;
            size_t      max_size;

            // mandatory configuration for daily file sink
            it = value.FindMember("base_file_name");
            if(it == value.MemberEnd()) base_file_name = std::string("./rotate.log");
            else base_file_name = std::string(it->value.GetString());

            it = value.FindMember("max_files");
            if(it == value.MemberEnd()) max_files = 10;
            else max_files = it->value.GetUint64();

            it = value.FindMember("max_size");
            if(it == value.MemberEnd()) max_size = 1024 * 1024 * 10; // 10MB
            else max_size = it->value.GetUint64();

            char buffer[base_file_name.size() + 1];
            strcpy(buffer, base_file_name.c_str());
            char* dir_name = dirname(buffer);
            if(strcmp(dir_name, ".") != 0){
                if(!CreateDirectory(dir_name)){
                    printf("%s::%s: Create directory '%s' failure\n", __CLASS__, __FUNCTION__, dir_name);
                    return false;
                }
            }

            if(sink_type == SINK_TYPE_ROTATING_FILE_SINK_ST){
                sink_map_[sink_name] = std::make_shared<spdlog::sinks::rotating_file_sink_st>(
                                           base_file_name, max_size, max_files);
            }
            else {
                // SINK_TYPE_ROTATING_FILE_SINK_MT
                sink_map_[sink_name] = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                                           base_file_name, max_size, max_files);
            }
        }

        // set level if any
        it = value.FindMember("level");
        if(it != value.MemberEnd()){
            std::string level = std::string(it->value.GetString());
            sink_map_[sink_name]->set_level(spdlog::level::from_str(level));
        }

        // set pattern if any
        it = value.FindMember("pattern");
        if(it != value.MemberEnd()){
            std::unordered_map<std::string, std::string>::const_iterator pattern_it =
                pattern_config.find(std::string(it->value.GetString()));
            if(pattern_it != pattern_config.end()){
                sink_map_[sink_name]->set_pattern(pattern_it->second);
            }
        }

        return true;
    }

    bool CreateDirectory(const char* dir_path){
        struct stat st = {0};
        if(stat(dir_path, &st) == -1){
			#ifdef _WIN32
			  if (mkdir(dir_path) == -1) return false;
			#else
            if (mkdir(dir_path, 0755) == -1) return false;
			#endif
        }
        return true;
    }

    std::shared_ptr<spdlog::logger>
    CreateSync(const std::string& logger_name,
               std::vector<std::shared_ptr<spdlog::sinks::sink>>& sink_list){
        auto new_logger = std::make_shared<spdlog::logger>(std::move(logger_name),
                                                               begin(sink_list), end(sink_list));
            //spdlog::details::registry::instance().register_and_init(new_logger);
            return new_logger;
    }

    std::shared_ptr<spdlog::async_logger>
    CreateAsync(const std::string& logger_name,
    		    std::vector<std::shared_ptr<spdlog::sinks::sink>>& sink_list){
        auto &registry_inst = spdlog::details::registry::instance();

        // create global thread pool if not already exists..
        std::lock_guard<std::recursive_mutex> tp_lock(registry_inst.tp_mutex());
        auto tp = registry_inst.get_tp();
        if (tp == nullptr)
        {
            fprintf(stderr, "SpdlogJsonConfig::CreateAsync: Thread Pool not created.\n");
            return nullptr;
        }

        auto new_logger = std::make_shared<spdlog::async_logger>(logger_name,
                                                                 begin(sink_list), end(sink_list),
                                                                 std::move(tp),
                                                                 spdlog::async_overflow_policy::block);
        //registry_inst.register_and_init(new_logger);
        return new_logger;
    }

    std::shared_ptr<spdlog::async_logger>
    CreateAsyncNB(const std::string& logger_name,
                  std::vector<std::shared_ptr<spdlog::sinks::sink>>& sink_list){
        auto &registry_inst = spdlog::details::registry::instance();

        // create global thread pool if not already exists..
        std::lock_guard<std::recursive_mutex> tp_lock(registry_inst.tp_mutex());
        auto tp = registry_inst.get_tp();
        if (tp == nullptr)
        {
            fprintf(stderr, "SpdlogJsonConfig::CreateAsyncNB: Thread Pool not created.\n");
            return nullptr;
        }

        auto new_logger = std::make_shared<spdlog::async_logger>(logger_name,
                                                                 begin(sink_list), end(sink_list),
                                                                 std::move(tp),
                                                                 spdlog::async_overflow_policy::overrun_oldest);
        //registry_inst.register_and_init(new_logger);
        return new_logger;
    }


    std::string DEFAULT_PATTERN;
    std::shared_ptr<spdlog::sinks::sink> DEFAULT_SINK;
    std::shared_ptr<spdlog::logger> DEFAULT_LOGGER;

    /// set to store all currently support sink type
    std::set<std::string> supported_sink_type_;

    /// map to map logger name to logger id
    std::unordered_map<std::string, uint32_t> name_to_id_;

    /// table to store all shared_ptr to created spdlog::logger. For efficient access.
    std::shared_ptr<spdlog::logger> logger_table_[MAX_LOGGER_NUM];

    /// total number of spdlog::logger created
    uint32_t logger_count_;

    /// map to map logger name to shared_ptr to created spdlog::logger
    std::unordered_map<std::string, std::shared_ptr<spdlog::logger>> logger_map_;

    /// map to map sink name to shared_ptr to created sinks
    std::unordered_map<std::string, std::shared_ptr<spdlog::sinks::sink>> sink_map_;
};

} // namespace spdlog_json_config

#endif // __SPDLOG_JSON_CONFIG_H__
