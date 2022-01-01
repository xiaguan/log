#include <iostream>
#include <thread>
#include "lib/log.h"

bool init = false;


Log::Logger::ptr initlogger(){
    Log::Logger::ptr logger;
    if(init){
        return logger;
    }
    else{
        logger = std::make_shared<Log::Logger>("serv");
        //创建两个appender用于输出
        Log::LogAppender::ptr stdout_appender(new Log::StdOutAppender);
        Log::LogAppender::ptr file_appender(new Log::FileOutAppender("lib.txt"));

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
        logger->setLevel(Log::LogLevel::DEBUG);

        init = true;
        return logger;
    }
}

void sock_log_event(Log::LogEvent::ptr event,Log::LogLevel::level level){
    auto logger = initlogger();
    logger->log(level,event);
    if(level==Log::LogLevel::FATAL){
        exit(1);
    }
}

void EasyError(const std::string & errormsg){
    Log::LogEvent::ptr event = std::make_shared<Log::LogEvent>(Log::LogEvent(nullptr,Log::LogLevel::FATAL,"NULL",0));
    event->getSS()<<errormsg;
    sock_log_event(event,Log::LogLevel::FATAL);
}

int main(int argc, char** argv) {
    EasyError("hhh");
    return 0;
}
