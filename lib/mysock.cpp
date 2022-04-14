//
// Created by 75108 on 2021/12/28.
//
#include "mysock.h"

#include <utility>
#include <fcntl.h>
#include <sys/poll.h>
#include <sys/epoll.h>

namespace su{

    // sock模块的日志器
    std::shared_ptr<log::Logger> logger = nullptr;

    void init_logger(bool is_client){
        if(is_client){
            logger = new_logger("client");
        }else{
            logger = new_logger("server");
        }
    }

    std::string getAddress(const sockaddr_in & addr){
        char ip_adress[INET_ADDRSTRLEN];
        inet_ntop(AF_INET,&addr.sin_addr,ip_adress,sizeof(ip_adress));
        return std::string(ip_adress);
    }

    unsigned int getPort(const sockaddr_in & addr){
        unsigned  int result = ntohs(addr.sin_port);
        return result;
    }

//封装过的socket函数，成功时写入日志，失败返回-1，并终止程序；
    int Socket(const int& family,const int& type,const int& protocol){
        int result = socket(family,type,protocol);
        if(result == -1){
            SU_LOG_ERROR(logger)<<"socket() error " << errno ;
        }else{
            SU_LOG_DEBUG(logger) <<"socket() done with sock_fd : "+std::to_string(result);
        }
        return result;
    }

//connect 失败
    int Connect(const int& sockfd,struct sockaddr_in & serv_addr,socklen_t addrlen){
        int result = connect(sockfd,(struct sockaddr*)&serv_addr,addrlen);
        if(result == -1){
            if(errno == ECONNREFUSED){
                SU_LOG_ERROR(logger) <<"connect() error : hard error";
            }
            else if(errno == ETIMEDOUT ){
                SU_LOG_ERROR(logger) << "connect() error : time out ";
            }
            else if(errno == EHOSTDOWN || errno == ENETUNREACH){
                SU_LOG_ERROR(logger)<<"connect() error : soft error";
            }else{
                SU_LOG_ERROR(logger) <<"connect() error ! "<<errno ;
            }
        }else{
            // connect 是客户端向服务器端发起的，所以日志里应该写入服务器的信息
            SU_LOG_DEBUG(logger) <<"Connect() done ! Serv ip: " << getAddress(serv_addr) <<" port : "<< getPort(serv_addr);

        }
        return result;
    }

    int Bind(const int & sockfd,struct sockaddr_in & servaddr,socklen_t addrlen){
        int result = bind(sockfd,(struct sockaddr*)&servaddr,addrlen);
        if(result==-1){
            if(errno == EADDRINUSE){
                SU_LOG_ERROR(logger) <<"bind() error: Address already in use ";
            }
            else {
                SU_LOG_ERROR(logger) <<"bind() error ! " << errno;
            }
            exit(1);
        }else{
            SU_LOG_DEBUG(logger) << "Bind done() port : " << getPort(servaddr);
        }
        return sockfd;
    }

    int Listen(const int& sockfd, const int & backlog){
        int result = listen(sockfd,backlog);
        if(result == -1){
            SU_LOG_ERROR(logger) <<"Listen() ERROR "<<errno;
        }else{
            SU_LOG_DEBUG(logger) << "listen() done " << backlog;
        }
        return result;
    }

    int Accept(const int & sockfd,struct sockaddr_in & client_addr,socklen_t *addrlen){
        int result = accept(sockfd,(struct sockaddr*)&client_addr,addrlen);
        if(result == -1){
            SU_LOG_ERROR(logger) <<"accept() error ! "<<errno ;
        }else{
            SU_LOG_DEBUG(logger) <<"Accept() done with client ip: " <<getAddress(client_addr) <<" port : " <<getPort(client_addr);
        }
        return result;
    }

    bool readn(int sockfd,void * vbuf,ssize_t readlen){
        SU_LOG_DEBUG(logger) <<"readn() start ";
        //这里存在一个不注意就会出错的bug,循环调用read向buf的首地址写入
        int already_read_len = 0;
        char * buf = (char*)vbuf;
        while(readlen){
            ssize_t n = read(sockfd,buf+already_read_len,readlen);
            if(n<0){
                SU_LOG_DEBUG(logger) <<"readn() error  " <<errno;
                return false;
            }else if(n==0){
                //文件尾
                SU_LOG_DEBUG(logger) <<"readn() with sz 0done !";
                return false;
            }else{
                readlen -= n;
            }
            SU_LOG_DEBUG(logger) <<"read "<<n;
            already_read_len += n;
        }
        SU_LOG_DEBUG(logger) <<"readn() done";
        return true;
    }

    bool writen(int sockfd,const char * buf,ssize_t writelen){
        size_t nleft;
        ssize_t nwritten;
        const char * ptr = buf;
        nleft = writelen;
        while(nleft > 0){
            if ((nwritten = write(sockfd,ptr,nleft)) <=0){
                if(nwritten <0 && errno == EINTR){
                    nwritten = 0;
                }else{
                    SU_LOG_ERROR(logger) <<" error "<<errno ;
                    return false;
                }
            }
            nleft -= nwritten;
            ptr += nwritten;
        }
        return true;
    }

    std::shared_ptr<log::Logger> new_logger(std::string loggername){
        std::shared_ptr<log::Logger> logger1 = std::make_shared<log::Logger>(loggername);

        auto std_out = std::make_shared<su::log::StdOutputAppender>();
        auto file_out = std::make_shared<su::log::FileOutputAppender>(logger1->get_name()+".txt");

        std_out->set_level(su::log::Level::DEBUG);
        file_out->set_level(su::log::Level::ERROR);

        auto fmt = std::make_shared<su::log::Formatter>("%d{%Y-%m-%d %H:%M:%S}%T%t%T[%p]%T[%c]%T%f:%l%T%m%n");
        std_out->set_format(fmt);
        file_out->set_format(fmt);

        logger1->add_appender(std_out);
        logger1->add_appender(file_out);

        return logger1;
    }

    int setnonblocking(int fd){
        int old_option = fcntl(fd,F_GETFL);
        int new_option = old_option | O_NONBLOCK;
        fcntl(fd,F_SETFL,new_option);
        return old_option;
    }

}






