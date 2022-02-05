//
// Created by 75108 on 2022/1/23.
//
#include "socket.h"

namespace Su{

    // TCPserver 的相关函数
    TCPserver::TCPserver(int port):m_port(port){}
    void TCPserver::init(){
        //sockfd 获取
        m_sockfd = Su::Socket(AF_INET,SOCK_STREAM,0);

        //填充服务器结构体;
        bzero(&m_serv_addr,sizeof(m_serv_addr));
        m_serv_addr.sin_family = AF_INET;
        inet_pton(AF_INET,"127.0.0.1",&m_serv_addr.sin_addr);
        m_serv_addr.sin_port = htons(m_port);
        
        Su::Bind(m_sockfd,m_serv_addr,sizeof(m_serv_addr));
        Su::Listen(m_sockfd,5);
        //成功启动
    }

    void TCPserver::accept(){
        socklen_t sz = sizeof(m_client_addr);
        Su::Accept(m_sockfd,m_client_addr,&sz);
    }

    struct sockaddr_in Su::TCPserver::get_client() {return m_client_addr;};


    //TCPclient 相关函数
    TCPclient::TCPclient(std::string serv_adres,int port):
    m_serv_adres(std::move(serv_adres)),m_serv_port(std::move(port)){
        m_sockfd = Socket(AF_INET,SOCK_STREAM,0);
        bzero(&m_serv_addr,sizeof(m_serv_addr));
        m_serv_addr.sin_family = AF_INET;
        inet_pton(AF_INET,m_serv_adres.c_str(),&m_serv_addr.sin_addr);
        m_serv_addr.sin_port = htons(m_serv_port);
    }

    void TCPclient::connect() {
        socklen_t sz = sizeof(m_serv_addr);
        Su::Connect(m_sockfd,m_serv_addr,sz);
    }
}
