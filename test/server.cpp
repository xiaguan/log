#define _GNU_SOURCE 1

#include <oop_sock.h>
#include <sys/poll.h>

using namespace su;

auto logger = SU_LOG_ROOT;

const int USER_LIMIT = 5;
const int BUFFER_SIZE = 64;

void send_to_all(UserManager & manager,int fdGet,std::string msg){
    if(msg.size() > BUFFER_SIZE){
        SU_LOG_ERROR(logger) << " send_to_all :msg over size";
        return ;
    }
    msg.resize(BUFFER_SIZE);
    auto list = manager.getUserList();
    for(auto user_ptr : list){
        if(user_ptr->getSockfd() == fdGet) continue;
        user_ptr->send(msg.c_str(),BUFFER_SIZE);
    }
}

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
                    char errorBuf[10] ="full!";
                    new_user->send(errorBuf,10);
                    continue;
                }
                userMgr.addUser(new_user);
                setnonblocking(new_user->getSockfd());

                /*
                 * 每一个fds[cnt] 对应一个fd
                 */
                cnt++;
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
                SU_LOG_DEBUG(logger) <<"POLLIN ready " << fds[i].fd;
                auto user = userMgr.findUser(fds[i].fd);
                user->recv(buf,BUFFER_SIZE);
                int now_fd = user->getSockfd();
                std::string msg(buf);
                std::thread t(send_to_all,std::ref(userMgr),now_fd,msg);
                t.detach();
                SU_LOG_DEBUG(logger) <<"POLLIN END";
            }
        }   
    }
}