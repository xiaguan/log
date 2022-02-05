//
// Created by 75108 on 2022/1/23.
//

#include <oop_sock.h>
int main(){
    Su::TCPserver m_server(4567);
    m_server.init();
    m_server.accept();
    auto client_addr = m_server.get_client();
    return 0;
}