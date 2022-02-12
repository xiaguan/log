//
// Created by 75108 on 2021/12/28.
//
#include "mysock.h"

namespace Su{


    void sock_log_event(Su::LogEvent::ptr event,Su::LogLevel::level level){
        auto logger = Su::initlogger();
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
    int Connect(const int& sockfd,struct sockaddr_in & serv_addr,socklen_t addrlen){
        int result = connect(sockfd,(struct sockaddr*)&serv_addr,addrlen);
        if(result == -1){
            if(errno == ECONNREFUSED){
                EasyError("connect() error: hard error");
            }
            else if(errno == ETIMEDOUT ){
                EasyError("coonect() error : time out ");
            }
            else if(errno == EHOSTDOWN || errno == ENETUNREACH){
                EasyError("connect() error : soft error");
            }else{
                EasyError("connect() error ");
            }
        }else{
            // connect 是客户端向服务器端发起的，所以日志里应该写入服务器的信息
            std::string log_msg = "Connect() done ! Serv ip: ";
            //待写入
            char ipadress[INET_ADDRSTRLEN];
            inet_ntop(AF_INET,&serv_addr.sin_addr,ipadress,sizeof(ipadress)); 
            log_msg += std::string(ipadress);
            sock_log_info(log_msg);
        }
        return result;
    }

    int Bind(const int & sockfd,struct sockaddr_in & servaddr,socklen_t addrlen){
        int result = bind(sockfd,(struct sockaddr*)&servaddr,addrlen);
        if(result==-1){
            if(errno == EADDRINUSE){
                EasyError("bind() error: Address already in use ");
            }
            else EasyError("bind() error");
            exit(1);
        }else{
            std::string log_msg = "Bind done() port : ";
            //待写入
            log_msg += std::to_string(ntohs(servaddr.sin_port));
            sock_log_info(log_msg);
        }
        return result;
    }

    int Listen(const int& sockfd, const int & backlog){
        int result = listen(sockfd,backlog);
        if(result == -1){
            EasyError("Listen() Error !");
        }else{
            sock_log_info("listen() done "+std::to_string(backlog));
        }
        return result;
    }

    int Accept(const int & sockfd,struct sockaddr_in & client_addr,socklen_t *addrlen){
        int result = accept(sockfd,(struct sockaddr*)&client_addr,addrlen);
        if(result == -1){
            EasyError("accept() error");
        }else{
            std::string log_msg = "Accept() done with client ip: ";
            //待写入
            char ipadress[INET_ADDRSTRLEN];
            inet_ntop(AF_INET,&client_addr.sin_addr,ipadress,sizeof(ipadress)); 
            log_msg += std::string(ipadress);
            log_msg += " port :";
            log_msg += std::to_string(ntohs(client_addr.sin_port));
            sock_log_info(log_msg);
        }
        return result;
    }

    bool readn(int sockfd,void * vbuf,ssize_t readlen){
        //这里存在一个不注意就会出错的bug,循环调用read向buf的首地址写入
        int already_read_len = 0;
        char * buf = (char*)vbuf;
        while(readlen){
            ssize_t n = read(sockfd,buf+already_read_len,readlen);
            if(n<0){
                return false;
            }else if(n==0){
                //文件尾
                return true;
            }else{
                readlen -= n;
            }
            already_read_len += n;
        }
        return true;
    }

    bool writen(int sockfd,void * buf,ssize_t writelen){
        size_t nleft;
        ssize_t nwritten;
        const char * ptr;
        ptr = (char*)buf;
        nleft = writelen;
        while(nleft > 0){
            if ((nwritten = write(sockfd,ptr,nleft)) <=0){
                if(nwritten <0 && errno == EINTR){
                    nwritten = 0;
                }else{
                    return false;
                }
            }
            nleft -= nwritten;
            ptr += nwritten;
        }
        return true;
    }

    
    
}






