//
// Created by 75108 on 2022/1/23.
//

#include <oop_sock.h>
#include <thread>
#include <vector>

using namespace std;
int cnt = 0;

void test(){
    cnt++;
    Su::TCPclient my_client("127.0.0.1",4567);
    my_client.connect();
    char buf[20] = "hello world";
    Su::writen(my_client.get_sockfd(),buf,20);
    Su::readn(my_client.get_sockfd(),buf,20);
    std::cout <<"Get from server " << buf <<std::endl;
    close(my_client.get_sockfd());
}

int main(){
    std::vector<std::thread> ts;
    for(int i = 0;i<10;i++){
        ts.push_back(std::thread(test));
    }
    for(auto & t : ts){
        t.join();
    }
    return 0;
}

