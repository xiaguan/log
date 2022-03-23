//
// Created by 75108 on 2022/1/23.
//

#ifndef SU_SOCKET_H
#define SU_SOCKET_H

#include "mysock.h"
#include <memory>
#include <list>


namespace su{

    //TCPserver的封装
    class TCPserver;
    class User{
        public:
        friend TCPserver;
        typedef std::shared_ptr<User> ptr;
        bool send(char * buf,size_t len);
        bool recv(char * buf,size_t len);
        ~User();
        protected:
        struct sockaddr_in user_addr;
        int connfd;
    };

    class TCPserver{
    public:
        typedef std::shared_ptr<TCPserver> ptr;
        TCPserver(int port);
        void init();
        void addUser(User::ptr new_user);
        void delUser(User::ptr del_user);

        void accept(User::ptr user);

        int getListenfd() const {return listenfd;}
    private:
        struct sockaddr_in serv_addr;
        int listenfd;
        const int m_port;  //服务器启动的port
        char buf[MAX_SIZE];
        std::list<std::shared_ptr<User>> m_users;
    };


    //TCPClient的封装
    class TCPclient{
    public:
        TCPclient(std::string serv_adres,int port);
        void connect();
        int getSockfd() const ;
        bool send(char * buf,size_t len);
        bool recv(char * buf,size_t len);
    private:
        int m_sockfd;
        std::string m_serv_adres;
        int m_serv_port;
        struct sockaddr_in m_serv_addr;
    };
}


#endif //SU_SOCKET_H
