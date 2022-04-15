#include <config.h>
#include "yaml-cpp/yaml.h"

int main(){
    YAML::Node root = YAML::LoadFile("log.yml");
    return 0;   
}