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

namespace su{

    void test();

    int Socket(const int& family,const int& type,const int& protocol);

    int Connect(const int& sockfd,struct sockaddr_in & servaddr,socklen_t addrlen);

    int Bind(const int & sockfd,struct sockaddr_in & servaddr,socklen_t addrlen);

    int Accept(const int & sockfd,struct sockaddr_in & servaddr,socklen_t *addrlen);

    int Listen(const int& sockfd, const int & backlog);

    bool readn(int sockfd,void * buf,ssize_t readlen);

    bool writen(int sockfd,void * buf,ssize_t writelen);

    void init_logger(bool is_client);

    class sock_logger{
    public:
        static std::shared_ptr<log::Logger> getInstance(std::string name);
    private:
        explicit sock_logger(std::string name);
        static std::shared_ptr<log::Logger> m_logger;
    };
    std::shared_ptr<log::Logger> sock_logger::m_logger = nullptr;

    
}

#endif //LOG_MYSOCK_H
