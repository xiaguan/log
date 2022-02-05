//
// Created by 75108 on 2021/12/28.
//

#ifndef LOG_MYSOCK_H
#define LOG_MYSOCK_H

#include "log.h"
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>

const int MAX_SIZE = 1024;

namespace Su{

    void test();


//使用一个静态的logger进行日志的输出
    void sock_log_event(Su::LogEvent::ptr ,Su::LogLevel::level level);

    void EasyError(const std::string & errormsg);

    int Socket(const int& family,const int& type,const int& protocol);

    int Connect(const int& sockfd,struct sockaddr_in & servaddr,socklen_t addrlen);

    int Bind(const int & sockfd,struct sockaddr_in & servaddr,socklen_t addrlen);

    int Accept(const int & sockfd,struct sockaddr_in & servaddr,socklen_t *addrlen);

    int Listen(const int& sockfd, const int & backlog);

    bool readn(int sockfd,void * buf,ssize_t readlen);

    bool wrtien(int sockfd,void * buf,ssize_t writelen);
    
}

#endif //LOG_MYSOCK_H
