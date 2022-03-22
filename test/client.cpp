#include <oop_sock.h>
#include <mysock.h>
#include <sys/poll.h>


using namespace su;

auto logger = SU_LOG_ROOT;

int main(){
    init_logger(true);
    TCPclient m_client("127.0.0.1",4567);
    m_client.connect();

    pollfd fds[2];
    fds[0].fd = 0;
    fds[0].events = POLLIN;
    fds[0].revents = 0;

    fds[1].fd = m_client.getSockfd();
    fds[1].events = POLLIN | POLLRDHUP;
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

        if(fds[1].revents & POLLRDHUP){
            SU_LOG_DEBUG(logger) <<"server closed";
            break;
        }
        else if(fds[1].revents & POLLIN){
            memset(read_buf,0,64);
            m_client.recv(read_buf,63);
            std::cout <<"server : "<<read_buf<<std::endl;
        }

        if(fds[0].revents & POLLIN){
            ret = splice(0,NULL,pipefd[1],NULL,32768,SPLICE_F_MORE |SPLICE_F_MOVE);
            ret = splice(pipefd[0],NULL,m_client.getSockfd(),NULL,32768,SPLICE_F_MOVE | SPLICE_F_MOVE);
        }
    }
    return 0;
}