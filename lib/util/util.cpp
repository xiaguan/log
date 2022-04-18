//
// Created by 75108 on 2022/1/23.
//

#include "util/util.h"

namespace su{
    unsigned long long GetThreadId(){
        unsigned long long result = 0;
        std::ostringstream osr;
        osr << std::this_thread::get_id();
        result = std::stoull(osr.str());
        return result;
    }
}