#include "config_logger.h"



int main(int argc, char* argv[]){

    initialize_demo_logger("logger_config.json");

    DEMO_LOGI("Hello {}", "world");
    DEMO_LOGD("{1} {0}", "Hello", "World");
    DEMO_LOGI("{1} {0}", "Hello", "World");
    DEMO_LOGW("This is the default logger got by name");
    DEMO_LOGW("{:<30}", "left aligned 30");
    DEMO_LOGW("{:>30}", "right aligned 30");
    DEMO_LOGW("{:=^30}", "centered aligned 30");
    DEMO_LOGC("Terrible Critical");


    DEMOID_LOGI("This is the default logger got by id");
    DEMOID_LOGI("{:*<{}}", "dynamic width left align 60", 60);
    DEMOID_LOGI("dynamic precision(4) 3.1415926: {:.{}f}", 3.1415926, 4);

    DEMOID_LOGI("{:+f}; {:+f}", 3.14, -3.14); // show it always
    DEMOID_LOGI("{: f}; {: f}", 3.14, -3.14); // show a space for positive numbers
    DEMOID_LOGI("{:-f}; {:-f}", 3.14, -3.14); // show only the minus -- same as '{:f}; {:f}'
    DEMOID_LOGI("{:*=+10d}; {:*=+10d}", 314, -314); // show only the minus -- same as '{:f}; {:f}'

    DEMOID_LOGI("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    DEMOID_LOGI("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42);
    DEMOID_LOGI("{0:#04x}, {1:#04x}, {0:#08x}, {1:#08x}", 0, 0xa);

    return 0;
}


