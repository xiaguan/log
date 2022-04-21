#include <config.h>
#include "yaml-cpp/yaml.h"

int main(){
    using namespace std;
    YAML::Node root = YAML::LoadFile("log.yml");
    su::ConfigVar<int>::ptr pre_port = su::Config::Lookup("system.port",0,"system port ");
    su::ConfigVar<float>::ptr next_port = su::Config::Lookup("system.port",(float)0,"system port");
    su::Config::LoadFromYAMLNode(root);
    su::ConfigVar<int>::ptr pre_port1 = su::Config::Lookup("system.port",0,"system port ");
    std::cout << pre_port->toString() << std::endl;
    return 0;   
}