//
// Created by 75108 on 2021/12/28.
//
#include "mysock.h"

namespace Su{

    su::log::Logger::ptr logger = SU_LOG_ROOT;

//封装过的socket函数，成功时写入日志，失败返回-1，并终止程序；
    int Socket(const int& family,const int& type,const int& protocol){
        int result = socket(family,type,protocol);
        if(result == -1){
            SU_LOG_ERROR(logger)<<"socket() error";
        }else{
            SU_LOG_DEBUG(logger) <<"socket done "+std::to_string(result);
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
                SU_LOG_ERROR(logger) << "coonect() error : time out ";
            }
            else if(errno == EHOSTDOWN || errno == ENETUNREACH){
                SU_LOG_ERROR(logger)<<"connect() error : soft error";
            }else{
                SU_LOG_ERROR(logger) <<"connect() error !";
            }
        }else{
            // connect 是客户端向服务器端发起的，所以日志里应该写入服务器的信息
            SU_LOG_DEBUG(logger) <<"Connect() done ! Serv ip:";

            char ipadress[INET_ADDRSTRLEN];
            inet_ntop(AF_INET,&serv_addr.sin_addr,ipadress,sizeof(ipadress));

            SU_LOG_DEBUG(logger) << std::string(ipadress);

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
                SU_LOG_ERROR(logger) <<"bind() error !";
            }
            exit(1);
        }else{
            SU_LOG_DEBUG(logger) << "Bind done() port : ";
            SU_LOG_DEBUG(logger) << std::to_string(ntohs(servaddr.sin_port));
        }
        return result;
    }

    int Listen(const int& sockfd, const int & backlog){
        int result = listen(sockfd,backlog);
        if(result == -1){
            SU_LOG_ERROR(logger) <<"Listen() ERROR";
        }else{
            SU_LOG_DEBUG(logger) << "listen() done ";
            SU_LOG_DEBUG(logger) << std::to_string(backlog);
        }
        return result;
    }

    int Accept(const int & sockfd,struct sockaddr_in & client_addr,socklen_t *addrlen){
        int result = accept(sockfd,(struct sockaddr*)&client_addr,addrlen);
        if(result == -1){
            SU_LOG_ERROR(logger) <<"accept() error !";
        }else{
            SU_LOG_DEBUG(logger) <<"Accept() done with client ip: ";
            //待写入
            char ipadress[INET_ADDRSTRLEN];
            inet_ntop(AF_INET,&client_addr.sin_addr,ipadress,sizeof(ipadress));
            SU_LOG_DEBUG(logger) << ipadress ;
            SU_LOG_DEBUG(logger) << "port : ";
            SU_LOG_DEBUG(logger) << std::to_string(ntohs(client_addr.sin_port)) ;
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






