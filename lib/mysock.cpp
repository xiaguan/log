//
// Created by 75108 on 2021/12/28.
//
#include "mysock.h"

bool init = false;

Su::Logger::ptr initlogger(){
    static Su::Logger::ptr logger;
    if(init){
        return logger;
    }
    else{
        //创建两个appender用于输出
        Su::StdOutAppender::ptr stdout_appender(new Su::StdOutAppender);
        Su::FileOutAppender::ptr file_appender(new Su::FileOutAppender("lib.txt"));

        //设置格式
        Su::LogFormatter::ptr fmt(new Su::LogFormatter("%d%T%p%T%m%T%t%T%f%T%l%n"));

        //appender的format控制
        file_appender->setFormater(fmt);
        file_appender->setLevel(Su::LogLevel::DEBUG);
        stdout_appender->setFormater(fmt);
        stdout_appender->setLevel(Su::LogLevel::DEBUG);

        //将两个appender放入logger的m_appender链表中
        logger->addAppender(stdout_appender);
        logger->addAppender(file_appender);
        logger->setLevel(Su::LogLevel::DEBUG);

        init = true;
        return logger;
    }
}

void sock_log_event(Su::LogEvent::ptr event,Su::LogLevel::level level){
    auto logger = initlogger();
    logger->log(level,event);
    if(level==Su::LogLevel::FATAL){
        exit(1);
    }
}

void sock_log_debug(const std::string & msg){
    Su::LogEvent::ptr event = std::make_shared<Su::LogEvent>(Su::LogEvent(nullptr,Su::LogLevel::FATAL,"NULL",0));
    event->getSS()<<msg;
    sock_log_event(event,Su::LogLevel::DEBUG);
}

void sock_log_info(const std::string & msg){
    Su::LogEvent::ptr event = std::make_shared<Su::LogEvent>(Su::LogEvent(nullptr,Su::LogLevel::FATAL,"NULL",0));
    event->getSS()<<msg;
    sock_log_event(event,Su::LogLevel::INFO);
}

void EasyError(const std::string & errormsg){
    Su::LogEvent::ptr event = std::make_shared<Su::LogEvent>(Su::LogEvent(nullptr,Su::LogLevel::FATAL,"NULL",0));
    event->getSS()<<errormsg;
    sock_log_event(event,Su::LogLevel::ERROR);
}

//封装过的socket函数，成功时写入日志，失败返回-1，并终止程序；
int Socket(const int& family,const int& type,const int& protocol){
    int result = socket(family,type,protocol);
    if(result == -1){
        EasyError("socket error !");
    }else{
        sock_log_info("socket done "+std::to_string(result));
    }
    return result;
}

//connect 失败
int Connect(const int& sockfd,const struct sockaddr * servaddr,socklen_t addrlen){
    int result = connect(sockfd,servaddr,addrlen);
    if(result == -1){
        EasyError("connect()  error !");
    }else{
        // connect 是客户端向服务器端发起的，所以日志里应该写入服务器的信息
        std::string log_msg = "Connect() done ! Serv ip";
        //待写入
        sock_log_info(log_msg);
    }
    return result;
}

int Bind(const int & sockfd,const struct sockaddr * myaddr,socklen_t addrlen){
    int result = bind(sockfd,myaddr,addrlen);
    if(result==-1){
        EasyError("bind() error !");
    }else{
        std::string log_msg = "Bind done() port : ";
        //待写入
        sock_log_info(log_msg);
    }
    return result;
}

int Listen(const int& sockfd, const int & backlog){
    int result = listen(sockfd,backlog);
    if(result == -1){
        EasyError("Listen() Error !");
    }else{
        sock_log_info("listen() done ");
    }
    return result;
}

int Accept(const int & sockfd,struct sockaddr * cliaddr,socklen_t *addrlen){
    int result = accept(sockfd,cliaddr,addrlen);
    if(result == -1){
        EasyError("accept() error");
    }else{
        std::string log_msg = "Accept() done with ";
        //待写入
        sock_log_info(log_msg);
    }
    return result;
}






