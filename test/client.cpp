#include <oop_sock.h>
#include <mysock.h>
#include <sys/poll.h>
#include <thread>


using namespace su;

auto logger = SU_LOG_ROOT;

const int BUFFER_SIZE = 64;

void get_from_server(int connfd){
    pollfd fds[1];
    fds[0].fd = connfd;
    fds[0].events = POLLIN | POLLERR | POLLRDHUP;
    char buf[BUFFER_SIZE];

    while(true){
        poll(fds,1,-1);
        if(fds[0].revents & POLLIN){
            memset(buf,0,BUFFER_SIZE);
            int ret = read(connfd,buf,BUFFER_SIZE);
            if(ret <= 0){
                SU_LOG_ERROR(logger) <<"read error "<<errno;
            }else{
                std::cout << "Get from server :"<<buf<<std::endl;
            }
        }else if(fds[0].revents & POLLRDHUP){
            SU_LOG_ERROR(logger) <<"soemthing is error(POLLRDHUP)"<<errno;
        }else{
            SU_LOG_ERROR(logger) <<"soemthing is error(POLLERR) "<<errno;
        }
    }
}

int main(){
    init_logger(true);
    TCPclient m_client("127.0.0.1",4567);
    m_client.connect();
    char buf[BUFFER_SIZE];
    std::thread t(get_from_server,m_client.getSockfd());
    t.detach();
    while(true){
        memset(buf,0,BUFFER_SIZE);
        std::cin >> buf;
        if(m_client.send(buf,BUFFER_SIZE)){
            SU_LOG_DEBUG(logger) <<"send to sever "<<buf;
        }else{
            SU_LOG_ERROR(logger) <<"something is wrong "<<errno;
        }
    }
    return 0;
}