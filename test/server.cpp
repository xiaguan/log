#include <oop_sock.h>
#include <log.h>
#include <thread>

auto logger = SU_LOG_ROOT;

void process_echo_user(su::User::ptr user){
    char buf[MAX_SIZE];
    while(user->recv(buf,MAX_SIZE)){
        user->send(buf,MAX_SIZE);
    }
}

int main(){
    su::init_logger(false);
    su::TCPserver::ptr m_server(new su::TCPserver(4567));
    m_server->init();
    while(true){
        su::User::ptr new_user(new su::User());
        m_server->accept(new_user);
        std::thread t(process_echo_user,new_user);
        t.detach();
    }
}