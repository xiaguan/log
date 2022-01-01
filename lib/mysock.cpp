//
// Created by 75108 on 2021/12/28.
//
#include "mysock.h"

bool init = false;

Log::Logger::ptr initlogger(){
    static Log::Logger::ptr logger;
    if(init){
        return logger;
    }
    else{
        //创建两个appender用于输出
        Log::StdOutAppender::ptr stdout_appender(new Log::StdOutAppender);
        Log::FileOutAppender::ptr file_appender(new Log::FileOutAppender("lib.txt"));

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

int Socket(const int& family,const int& type,const int& protocol){
    int result = socket(family,type,protocol);
    if(result == -1){
        EasyError("socket error !");
    }
}

int Connect(const int& sockfd,const struct sockaddr * servaddr,socklen_t addrlen){
    int result = connect(sockfd,servaddr,addrlen);
    if(result == -1){
        EasyError("connect()  error !");
    }
    return result;
}

int Bind(const int & sockfd,const struct sockaddr * myaddr,socklen_t addrlen){
    int result = bind(sockfd,myaddr,addrlen);
    if(result==-1){
        EasyError("bind() error !");
    }
    return result;
}

int Listen(const int& sockfd, const int & backlog){
    int result = listen(sockfd,backlog);
    if(result == -1){
        EasyError("Listen() Error !");
    }
    return result;
}

int Accept(const int & sockfd,struct sockaddr * cliaddr,socklen_t *addrlen){
    int result = accept(sockfd,cliaddr,addrlen);
    if(result == -1){
        EasyError("accept() error");
    }
    return result;
}






