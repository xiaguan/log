#include <iostream>
#include <thread>
#include "lib/log.h"
#include "lib/mysock.h"


int main(int argc, char** argv) {
    Log::LogEvent::ptr event = std::make_shared<Log::LogEvent>(Log::LogEvent(nullptr,Log::LogLevel::DEBUG,"main.cpp",26));
    event->getSS()<<"sock test";
    sock_log_event(event,Log::LogLevel::DEBUG);
    return 0;
}
