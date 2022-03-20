#include <mysock.h>

using namespace std;

int main(){
    su::init_logger(true);
    int listenfd,connfd;
    struct sockaddr_in serv_addr;
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family  = AF_INET;
    inet_pton(AF_INET,"127.0.0.1",&serv_addr.sin_addr);
    serv_addr.sin_port = htons(45678);
    listenfd = su::Socket(AF_INET,SOCK_STREAM,0);

    listenfd = su::Bind(listenfd,serv_addr,sizeof(serv_addr));
    cout << listenfd << endl;
    listenfd = su::Listen(listenfd,5);
}