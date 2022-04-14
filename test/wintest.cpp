#include <iostream>
#include <util/lexical_cast.h>

class A{

};

int main(){
    std::string a = "123";
    a.resize(64);
    cout << a.size() <<endl;
}