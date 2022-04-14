//
// Created by 75108 on 2022/1/23.
//

#ifndef SU_SOCKET_H
#define SU_SOCKET_H

#include "mysock.h"
#include <memory>
#include <list>


namespace su{

    //TCP_server的封装
    class TCPserver;
    class User{
        public:
        friend TCPserver;
        typedef std::shared_ptr<User> ptr;
        bool send(const char * buf,size_t len);
        bool recv(char * buf,size_t len);

        int getSockfd() const {return m_sockfd;}
        ~User();
        protected:
        // 没有必要保存sockaddr_in，系统提供的api是很全的。
        // struct sockaddr_in user_addr;
        int m_sockfd;
    };

    class UserManager{
    public:
        typedef std::shared_ptr<UserManager> ptr;

        // 类函数的一些声明
        UserManager()= default;
        UserManager(const UserManager & a) = delete;

        /*
         * Mgr类的一些接口:增加，删除，查找
         */

         void addUser(User::ptr new_user);

        // 查找
        User::ptr findUser(int sockfd);

        void delUser(int socfd);
        void delUser(User::ptr user);

        std::list<User::ptr>& getUserList() {return users;}
    private:
        std::list<User::ptr> users;
    };

    class TCPserver{
    public:
        typedef std::shared_ptr<TCPserver> ptr;
        TCPserver(int port);
        void init();

        void accept(User::ptr user);

        int getListenfd() const {return listenfd;}
    private:
        struct sockaddr_in serv_addr;
        int listenfd;
        const int m_port;  //服务器启动的port
        char buf[MAX_SIZE];
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
