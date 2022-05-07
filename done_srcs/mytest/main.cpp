#include <iostream>
#include <mutex>
#include <thread>
#include <future>
#include "Zipper.h"
#include <cstring>

#include <log.h>
#include <config.h>
using namespace std;

extern su::log::Logger::ptr logger;

void show_bytes_in_file(const std::string & filename){
    using namespace std;
    std::ifstream in(filename,ios::binary);
    in >> noskipws;
    char c;
    int cnt = 0;
    std::cout <<"test ";
    while(in >> c){
        if(cnt >100) return;
        print_in_two(c);std::cout <<" ";
        cnt++;
    }
}


void test_map(){
    std::map<char,std::string> map;
    map['a'] = "101011";
    map['b'] = "1111111111";
    map['c'] = "000000000";
    map['d'] = "1010101011";
    map['&'] = "10101011110";

    std::ofstream out("test.txt",ios::binary);
    detail::formatMap(map,out);
    out.close();

    std::ifstream in("test.txt",ios::binary);
    auto result = detail::getMap(in);
    for(auto &m : result){
        cout << m.first<<" "<<m.second<<endl;
    }
//        char c;
//    in >> noskipws;
//    while( in >> c){
//    print_in_two(c); cout << " ";
//    }
}

void prepare_txt(){
    std::ofstream out("1.txt");
    for(int i = 0;i<1000000;i++){
        out << 1;
    }
    out.close();
}

int main(int argc,char * argv[]) {
    Timer timer;
    timer.start();
    if(argc != 3){
        cout <<"Usage : patter(decode or encode) filname " <<endl;
        exit(1);
    }
    Zipper zipper;
    if(strcasecmp(argv[1],"encode") == 0){
        SU_LOG_INFO(logger) <<"encode : "<<argv[2];
        zipper.encode(argv[2]);
    }else if(strcasecmp(argv[1],"decode") == 0){
        zipper.decode(argv[2]);
    }else{
        cout << "Usage : patter(decode or encode) filename"<<endl;
    }
    timer.end();
}
