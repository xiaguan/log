//
// Created by 75108 on 2022/1/23.
//

#ifndef SU_SOCKET_H
#define SU_SOCKET_H

#include "mysock.h"


namespace su{

    //TCPserver的封装
    class TCPserver{
    public:
        TCPserver(int port);
        void init();  //启动
        void accept();  // 接受客户端
        sockaddr_in get_client();  //返回客户结构体用于操作
        int get_connfd();
        int get_listenfd();
    private:
        int m_listenfd;   //listenfd
        int m_connfd;    //connfd
        struct sockaddr_in m_serv_addr,m_client_addr;
        const int m_port;  //服务器启动的port
    };


    //TCPClient的封装
    class TCPclient{
    public:
        TCPclient(std::string serv_adres,int port);
        void connect();
        int get_sockfd();
    private:
        int m_sockfd;
        std::string m_serv_adres;
        int m_serv_port;
        struct sockaddr_in m_serv_addr;
    };
}


#endif //SU_SOCKET_H
