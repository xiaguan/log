#include <log.h>
#include <config.h>

auto logger = SU_LOG_ROOT();

int main(){
    su::ConfigVar<int>::ptr test = su::Config::Lookup("port",8080,"port");
    SU_LOG_DEBUG(logger) << test->getValue();
    SU_LOG_DEBUG(logger) <<"hello world";
}