#include <log.h>
#include <config.h>

auto logger = SU_LOG_ROOT();
su::Config::ConfigVarMap su::Config::s_dates;
int main(){
    su::ConfigVar<float>::ptr p_float_test = su::Config::Lookup("system",1.87f,"port");
    cout << p_float_test->getValue()<<endl;
    cout << p_float_test->toString() << endl;
    SU_LOG_DEBUG(logger) <<"hello word";
}