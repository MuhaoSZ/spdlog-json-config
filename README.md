## Overview
Header only spdlog config lib for initializing spdlog from a json file. 

## Features
* Initialize spdlog from a json configuration file.
  Sink, pattern, logger and thread pool are all configurable.

* Current supported sinks:  
  - stdout_sink_st
  - stdout_sink_mt
  - stderr_sink_st
  - stderr_sink_mt
  - stdout_color_sink_st
  - stdout_color_sink_mt
  - stderr_color_sink_st
  - stderr_color_sink_mt
  - syslog_sink_st
  - syslog_sink_mt
  - basic_file_sink_st
  - basic_file_sink_mt
  - daily_file_sink_st
  - daily_file_sink_mt
  - rotating_file_sink_st
  - rotating_file_sink_mt

* A default logger whose sink type is "stdout_color_sink_st" is automatically created for use.

## Requirements
* g++ compiler that supports C++11
* GNU Make  

The codes are tested against g++4.8.5, GNU Make 3.82 on CentOS 7

## Dependencies
* [spdlog v1.x](https://github.com/gabime/spdlog)
* [rapidjson](https://github.com/Tencent/rapidjson)

## How to use


## Configuration Example
    {
        "SINKS": {
            "stdout_color_sink": {
                "type": "stdout_color_sink_mt",
                "level": "debug"
            },
    
            "stderr_color_sink": {
                "type": "stderr_color_sink_mt",
                "level": "debug"
            },
    
            "syslog_sink": {
                "type": "syslog_sink_mt",
                "indent": "  ",
                "level": "debug"
            },
    
            "basic_file_sink": {
                "type": "basic_file_sink_mt",
                "file_name": "./logs/logfile.log",
                "truncate": true,
                "level": "debug"
            },
    
            "daily_file_sink": {
                "type": "daily_file_sink_mt",
                "base_file_name": "./logs/daily.log",
                "rotation_hour": 0,
                "rotation_minitue":0,
                "truncate": false,
                "level": "debug"
            },
    
            "rotate_file_sink": {
                "type": "rotating_file_sink_mt",
                "base_file_name": "./logs/rotate.log",
                "max_size": 10485760,
                "max_files": 10,
                "level": "debug"
            }
    
        },
    
        "PATTERNS": {
            "general_pattern": "[%C-%m-%d %H:%M:%S.%e][%n]%^[%L]%$ %v"
        },
    
        "LOGGERS": {
            "CONSOLE": {
                "sinks": ["stdout_color_sink"],
                "pattern": "general_pattern",
                "level": "debug",
                "sync_type": "sync"
            },
    
            "PARSER": {
                "sinks": ["stdout_color_sink", "syslog_sink", "basic_file_sink", "daily_file_sink", "rotate_file_sink"],
                "pattern": "general_pattern",
                "level": "debug",
                "sync_type": "async"
            }
        },
        
        "THREAD_POOL": {
            "thread_count": 2,
            "queue_size": 8192
        }
        
    }


## Codes Example
Three examples are put in folder "example".  
* default_logger: use default log directly, not need to create a config file.
* simple_logger: initialize spdlog according to config file, obtain logger by logger name.
* config_logger: initialize spdlog according to config file, obtain logger by logger name or logger id.


