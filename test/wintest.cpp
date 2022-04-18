#include <log.h>
#include <config.h>
#include "yaml-cpp/yaml.h"
#include <string>
#include <algorithm>



int main(){
    std::string s="";
    if(s.find_first_not_of("abcd") != std::string::npos){
        cout <<"faile"<<endl;
    }
}