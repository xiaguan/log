//
// Created by 75108 on 2022/1/23.
//

#include <iostream>
#include <mysock.h>
#include <log.h>
#include <oop_sock.h>
#include <func.h>
using namespace std;

void hello_world(){
    cout<<"Hello world"<<endl;
}

int main(){
    std::function<void()> hello(hello_world);
    auto t = Su::join_thread(hello,"hello");
    t.join();
    return 0;
    }

