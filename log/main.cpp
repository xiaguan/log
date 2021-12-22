#include <iostream>
#include "log.h"
#include <thread>

int main(int argc, char** argv) {
    Log::Logger::ptr logger(new Log::Logger);
    Log::StdOutAppender::ptr stdout_appender(new Log::StdOutAppender);
    

    Log::FileOutAppender::ptr file_appender(new Log::FileOutAppender("log.txt"));
    Log::LogFormatter::ptr fmt(new Log::LogFormatter("%d%T%p%T%m%t%n"));
    file_appender->setFormater(fmt);
    file_appender->setLevel(Log::LogLevel::DEBUG);
    stdout_appender->setFormater(fmt);
    stdout_appender->setLevel(Log::LogLevel::DEBUG);


    logger->addAppender(stdout_appender);
    logger->addAppender(file_appender);

    //Log::LogEvent::ptr event(new Log::LogEvent(__FILE__, __LINE__, 0, Log::GetThreadId(), Log::GetFiberId(), time(0)));
    //event->getSS() << "hello Log log";
    //logger->log(Log::LogLevel::DEBUG, event);

    std::cout << "hello sylar log" << std::endl;

    for (int i = 0; i < 10; i++) {
        Log::LogEventWrap(Log::LogEvent::ptr(new Log::LogEvent(logger, Log::LogLevel::INFO, "mian.cpp", 21, 66, 0, 0, time(0), "main"))).getSS() << "hello world";
    }

    return 0;

}
