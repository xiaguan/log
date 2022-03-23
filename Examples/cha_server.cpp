#include <oop_sock.h>
#include <mysock.h>
#include <sys/poll.h>

const int USER_LIMIT = 5;
const int BUFFER_SIZE = 64;
const int FD_LIMIT = 65535;

struct chat_client:public su::User{
    int getfd() const {return connfd;}
    char readbuf[BUFFER_SIZE];
    char * writebuf;
};
typedef std::shared_ptr<chat_client> user_ptr;

auto logger = SU_LOG_ROOT;

int main(){
    su::TCPserver m_server(4567);
    m_server.init();

    user_ptr users[FD_LIMIT];
    pollfd fds[USER_LIMIT+1];
    int user_count = 0;
    for(int i = 0;i<=USER_LIMIT;i++){
        fds[i].fd = -1;
        fds[i].events = 0;
    }

    fds[0].fd = m_server.getListenfd();
    fds[0].events = POLLIN | POLLERR;
    fds[0].revents = 0;

    int c;
    int ret = 0;
    SU_LOG_DEBUG(logger) <<"poll fd and user done !";
    while(true){
        ret = poll(fds,user_count+1,-1);
        if(ret < 0 ){
            SU_LOG_ERROR(logger) <<"poll failure  errno : "<<errno;
            break; 
        }

        for(int i = 0;i<user_count+1;i++){
            if(fds[i].fd == m_server.getListenfd() && fds[i].revents & POLLIN){
                user_ptr new_user(new chat_client);
                m_server.accept(new_user);
                if(user_count >= USER_LIMIT){
                    close(new_user->getfd());
                    continue;
                }

                user_count++;
                int connfd = new_user->getfd();
                users[connfd] = new_user;
                su::setnonblocking(connfd);

                fds[user_count].fd = connfd;
                fds[user_count].events = POLLIN | POLLRDHUP | POLLERR;
                fds[user_count].revents = 0;
                SU_LOG_DEBUG(logger) <<" A new user joined ";
            }
            else if(fds[i].revents & POLLERR){
                SU_LOG_ERROR(logger) <<"get an error ";
                continue;
            }else if(fds[i].revents & POLLRDHUP){
                users[fds[i].fd] = users[fds[user_count].fd];
                close(fds[i].fd);
                fds[i] = fds[user_count];
                i--;
                user_count--;
                SU_LOG_DEBUG(logger) <<"a user left";
            }else if(fds[i].revents & POLLIN){
                int connfd = fds[i].fd;
                memset(users[connfd]->readbuf,'\0',BUFFER_SIZE);
                ret = users[connfd]->recv(users[connfd]->readbuf,BUFFER_SIZE);
                SU_LOG_DEBUG(logger) <<" recv data from fd : "<<connfd;
                if(!ret){
                    if(errno != EAGAIN){
                        SU_LOG_ERROR(logger) << "a client g le ";
                        close(connfd);
                        users[fds[i].fd] = users[fds[user_count].fd];
                        fds[i] = fds[user_count];
                        i--;
                        user_count--;
                        continue;
                    }
                    SU_LOG_ERROR(logger) <<"recv errno : "<<errno;
                }

                for(int j = 1;j<=user_count;j++){
                    if(fds[i].fd = connfd) continue;

                    fds[i].events |= ~POLLIN;
                    fds[j].events |= POLLOUT;
                    users[fds[j].fd]->writebuf = users[connfd]->readbuf;
                }
            }else if(fds[i].revents & POLLOUT){
                int connfd = fds[i].fd;
                if(!users[connfd]->writebuf) continue;
                ret = users[connfd]->send(users[connfd]->readbuf,MAX_SIZE);
                users[connfd]->writebuf = NULL;
                fds[i].events |= ~POLLOUT;
                fds[i].events |= POLLIN;
            }
        }
    }
    return 0;
} 