//
// Created by 75108 on 2021/12/28.
//

#ifndef LOG_MYSOCK_H
#define LOG_MYSOCK_H

#include "log.h"


//使用一个静态的logger进行日志的输出
void sock_log_event(Log::LogEvent::ptr ,Log::LogLevel::level level);

void EasyError(const std::string & errormsg);

int Socket(const int& family,const int& type,const int& protocol);

int Connect(int sockfd,const struct sockaddr * servaddr,socklen_t addrlen);

int Bind(const int & sockfd,const struct sockaddr * myaddr,socklen_t addrlen);

int Listen(const int& sockfd, const int & backlog);

int Accept(const int & sockfd,struct sockaddr * clidaddr,socklen_t *addrlen);

#endif //LOG_MYSOCK_H
