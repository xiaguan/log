#include <iostream>
#include <util/lexical_cast.h>

class A{

};

int main(){
    std::string a = "123";
    A r = lexical_cast<A,std::string>(a);
}