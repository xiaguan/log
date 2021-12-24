#include <iostream>
#include "log/log.h"
#include <thread>

int main(int argc, char** argv) {
    //创建一个logger
    Log::Logger::ptr logger(new Log::Logger);

    //创建两个appender用于输出
    Log::StdOutAppender::ptr stdout_appender(new Log::StdOutAppender);
    Log::FileOutAppender::ptr file_appender(new Log::FileOutAppender("log.txt"));

    //设置格式
    Log::LogFormatter::ptr fmt(new Log::LogFormatter("%d%T%p%T%m%T%t%T%f%T%l%n"));

    //appender的format控制
    file_appender->setFormater(fmt);
    file_appender->setLevel(Log::LogLevel::DEBUG);
    stdout_appender->setFormater(fmt);
    stdout_appender->setLevel(Log::LogLevel::DEBUG);

    //将两个appender放入logger的m_appender链表中
    logger->addAppender(stdout_appender);
    logger->addAppender(file_appender);

    //Log::LogEvent::ptr event(new Log::LogEvent(__FILE__, __LINE__, 0, Log::GetThreadId(), Log::GetFiberId(), time(0)));
    //event->getSS() << "hello Log log";
    //logger->log(Log::LogLevel::DEBUG, event);

    Log::writelog(logger,"hello wrold");


    std::cout <<"is right ?"<<std::endl;
    return 0;

}
