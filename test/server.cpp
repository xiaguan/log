#define _GNU_SOURCE 1

#include <oop_sock.h>
#include <sys/poll.h>

using namespace su;

const int USER_LIMIT = 5;
const int BUFFER_SIZE = 64;

auto logger = SU_LOG_ROOT;

int main(){
    UserManager userMgr;
    TCPserver m_server(4567);
    m_server.init();

    pollfd fds[USER_LIMIT];

    for(int i = 0;i<USER_LIMIT;i++){
        fds[i].fd = -1;
        fds[i].events = 0;
    }

    fds[0].fd = m_server.getListenfd();;
    fds[0].events = POLLIN | POLLERR;
    fds[0].revents = 0;

    int serv_fd = m_server.getListenfd();
    
    int cnt = 0;
    int ret = 0;
    char buf[BUFFER_SIZE];
    while(true){
        for(int i = 0;i<USER_LIMIT;i++){
            ret = poll(fds,cnt+1,-1);
            if(ret < 0 ){
                SU_LOG_ERROR(logger) <<"POLL ERROR";
            }
            if(fds[i].fd == serv_fd && fds[i].revents &POLLIN){
                User::ptr new_user(new User);
                m_server.accept(new_user);
                if(cnt == USER_LIMIT){
                    char buf[10] ="full!";
                    new_user->send(buf,10);
                    continue;
                }
                userMgr.addUser(new_user);

                cnt++;
                std::cout <<"debug";
                setnonblocking(new_user->getSockfd());
                fds[cnt].fd = new_user->getSockfd();
                fds[cnt].events = POLLIN | POLLRDHUP | POLLERR;
                fds[cnt].revents = 0;

                SU_LOG_DEBUG(logger) <<" a new user joined : " <<cnt <<" "<<new_user->getSockfd();
            }
            else if(fds[i].revents & POLLERR){
                // ?
                SU_LOG_ERROR(logger) <<" get an error " <<fds[i].fd;
                continue;
            }else if(fds[i].revents & POLLRDHUP){
                SU_LOG_ERROR(logger) <<"A user error "<<fds[i].fd;
                userMgr.delUser(fds[i].fd);
                fds[i] = fds[cnt];
                cnt--;
            }else if(fds[i].revents & POLLIN){
                SU_LOG_DEBUG(logger) <<"POLLIN ready" << fds[i].fd;
                auto user = userMgr.findUser(fds[i].fd);
                user->recv(buf,BUFFER_SIZE);
                int now_fd = user->getSockfd();

                for(int j = 1;j<=cnt;j++){
                    std::cout << now_fd << fds[j].fd <<std::endl;
                    if(fds[j].fd == now_fd) continue;
                    auto send_to = userMgr.findUser(fds[j].fd);
                    if(send_to->send(buf,BUFFER_SIZE)){
                        SU_LOG_DEBUG(logger) <<"send to "<<send_to->getSockfd();
                    }else{
                        SU_LOG_ERROR(logger) <<"send error!";
                    }
                }
                SU_LOG_DEBUG(logger) <<"POLLIN END";

            }
        }   
    }
}