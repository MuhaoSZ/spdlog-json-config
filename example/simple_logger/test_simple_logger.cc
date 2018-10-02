#include "simple_logger.h"



int main(int argc, char* argv[]){

    initialize_simple_logger("simple_config.json");

    LOGI("Hello {}", "world");
    LOGD("{1} {0}", "Hello", "World");
    LOGI("{1} {0}", "Hello", "World");
    LOGW("{:<30}", "left aligned 30");
    LOGW("{:>30}", "right aligned 30");
    LOGW("{:=^30}", "centered aligned 30");
    LOGC("Terrible Critical");


    LOGI("{:*<{}}", "dynamic width left align 60", 60);
    LOGI("dynamic precision(4) 3.1415926: {:.{}f}", 3.1415926, 4);

    LOGI("{:+f}; {:+f}", 3.14, -3.14); // show it always
    LOGI("{: f}; {: f}", 3.14, -3.14); // show a space for positive numbers
    LOGI("{:-f}; {:-f}", 3.14, -3.14); // show only the minus -- same as '{:f}; {:f}'
    LOGI("{:*=+10d}; {:*=+10d}", 314, -314); // show only the minus -- same as '{:f}; {:f}'

    LOGI("int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
    LOGI("int: {0:d};  hex: {0:#x};  oct: {0:#o};  bin: {0:#b}", 42);
    LOGI("{0:#04x}, {1:#04x}, {0:#08x}, {1:#08x}", 0, 0xa);

    return 0;
}


