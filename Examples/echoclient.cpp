#include <oop_sock.h>
#include <log.h>

using namespace std;
int main(){
    su::TCPclient m_client("127.0.0.1",4567);
    m_client.connect();

    char buf[MAX_SIZE];

    while(cin >> buf){
        m_client.send(buf,MAX_SIZE);
        m_client.recv(buf,MAX_SIZE);
        cout <<"Get from the server "<< buf <<endl;
    }
    return 0;
} 