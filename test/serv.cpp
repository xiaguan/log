//
// Created by 75108 on 2022/1/23.
//

#include <oop_sock.h>
#include <thread>
void get_and_resend(int connfd){
    char buf[20];
    Su::readn(connfd,buf,20);
    std::cout <<"Read done" <<std::endl;
    Su::writen(connfd,buf,20);
    close(connfd);
}
int main(){
    Su::TCPserver m_server(4567);
    m_server.init();
    while(true){
        m_server.accept();
        std::thread t(get_and_resend,m_server.get_connfd());
        t.detach();
    }
    return 0;
}