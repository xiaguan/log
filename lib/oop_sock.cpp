//
// Created by 75108 on 2022/1/23.
//
#include "oop_sock.h"


namespace su{
    //USER
    bool User::send(const char * buf,size_t len){
        return writen(m_sockfd,buf,len);
    }

    bool User::recv(char * buf,size_t len){
        return read(m_sockfd,buf,len);
    }

    User::~User(){
        close(m_sockfd);
    }

    //UserMangger

    void UserManager::addUser(User::ptr new_user){
        users.push_back(new_user);
    }

    User::ptr UserManager::findUser(int sockfd){
        for(auto & user :users){
            if(user->getSockfd() == sockfd) return user;
        }
        return nullptr;
    }

    void UserManager::delUser(int sockfd){
        auto it = users.begin();
        while(it != users.end()){
            if((*it)->getSockfd() == sockfd){
                users.erase(it);
                return;
            }
        }
    }


    // TCPserver 的相关函数
    TCPserver::TCPserver(int port):m_port(port){
        init_logger(false);
        bzero(&serv_addr,sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr);
        serv_addr.sin_port = htons(m_port);
    }
    
    void TCPserver::init(){
        listenfd = Socket(AF_INET,SOCK_STREAM,0);
        listenfd = Bind(listenfd,serv_addr,sizeof(serv_addr));
        Listen(listenfd,5);
    }

    void TCPserver::accept(User::ptr user){
        socklen_t sz = sizeof(user->user_addr);
        user->m_sockfd = Accept(listenfd,user->user_addr,&sz);
    }

    

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

    int TCPclient::getSockfd()const {
        return m_sockfd;
    }

    bool TCPclient::send(char * buf,size_t len){
        return writen(m_sockfd,buf,len);
    }

    bool TCPclient::recv(char *buf,size_t len){
        return readn(m_sockfd,buf,len);
    }
}
