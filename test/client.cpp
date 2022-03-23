#include <oop_sock.h>
#include <mysock.h>
#include <sys/poll.h>


using namespace su;

auto logger = SU_LOG_ROOT;

const int BUFFER_SIZE = 64;

int main(){
    init_logger(true);
    TCPclient m_client("127.0.0.1",4567);
    m_client.connect();

    pollfd fds[2];
    fds[0].fd = 0;
    fds[0].events = POLLOUT;
    fds[0].revents = 0;

    fds[1].fd = m_client.getSockfd();
    fds[1].events = POLLIN | POLLRDHUP | POLLERR;
    fds[1].revents = 0;

    char read_buf[64];

    int pipefd[2];
    int ret = pipe(pipefd);
    if(ret == -1 ) return 0;

    while(true){
        ret = poll(fds,2,-1);

        if(ret < 0){
            SU_LOG_FATAL(logger) <<"poil failure";
            break;
        }

        if(fds[1].revents & POLLRDHUP || fds[1].revents & POLLERR){
            SU_LOG_DEBUG(logger) <<"server closed";
            break;
        }                
        if(fds[1].revents & POLLIN){
            SU_LOG_DEBUG(logger) <<"debug";
            memset(read_buf,0,64);
            m_client.recv(read_buf,BUFFER_SIZE);
            std::cout <<"server : "<<read_buf<<std::flush;
        }

        if(fds[0].revents & POLLOUT){
            memset(read_buf,0,BUFFER_SIZE);
            SU_LOG_DEBUG(logger) <<"POLLOUT START";
            std::cin>>read_buf;

            m_client.send(read_buf,BUFFER_SIZE);
            SU_LOG_DEBUG(logger) <<"POLLOUT END";
        }
    }
    return 0;
}