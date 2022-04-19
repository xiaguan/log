#include <config.h>
#include "yaml-cpp/yaml.h"

int main(){
    using namespace std;
    YAML::Node root = YAML::LoadFile("log.yml");
    su::ConfigVar<std::vector<int> >::ptr n = su::Config::Lookup<std::vector<int>>("system.log.vec",vector<int>{1});
    su::Config::LoadFromFile(root);
    std::cout << n->toString() << std::endl;
    return 0;   
}