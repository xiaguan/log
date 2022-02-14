//
// Created by 75108 on 2022/1/23.
//
#include "oop_sock.h"


namespace su{

    // TCPserver 的相关函数
    TCPserver::TCPserver(int port):m_port(port){}
    void TCPserver::init(){
        init_logger(false);
        //sockfd 获取
        m_listenfd = Socket(AF_INET,SOCK_STREAM,0);

        //填充服务器结构体;
        bzero(&m_serv_addr,sizeof(m_serv_addr));
        m_serv_addr.sin_family = AF_INET;
        inet_pton(AF_INET,"127.0.0.1",&m_serv_addr.sin_addr);
        m_serv_addr.sin_port = htons(m_port);
        
        Bind(m_listenfd,m_serv_addr,sizeof(m_serv_addr));
        Listen(m_listenfd,5);
        //成功启动
    }

    void TCPserver::accept(){
        socklen_t sz = sizeof(m_client_addr);
        m_connfd = Accept(m_listenfd,m_client_addr,&sz);
    }

    struct sockaddr_in TCPserver::get_client() {return m_client_addr;};
    int TCPserver::get_connfd() {return m_connfd;}
    int TCPserver::get_listenfd(){return m_listenfd;}


    //TCPclient 相关函数
    TCPclient::TCPclient(std::string serv_adres,int port):
    m_serv_adres(std::move(serv_adres)),m_serv_port(std::move(port)){
        init_logger(true);
        m_sockfd = Socket(AF_INET,SOCK_STREAM,0);
        bzero(&m_serv_addr,sizeof(m_serv_addr));
        m_serv_addr.sin_family = AF_INET;
        inet_pton(AF_INET,m_serv_adres.c_str(),&m_serv_addr.sin_addr);
        m_serv_addr.sin_port = htons(m_serv_port);
    }

    void TCPclient::connect() {
        socklen_t sz = sizeof(m_serv_addr);
        Connect(m_sockfd,m_serv_addr,sz);
    }

    int TCPclient::get_sockfd(){
        return m_sockfd;
    }
}
