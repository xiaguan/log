//
// Created by 75108 on 2022/1/23.
//

#ifndef LOG_FUN_H
#define LOG_FUN_H
#include "log.h"
#include <functional>
#include <chrono>
#include <thread>

namespace Su{

    //多线程加入函数
    template <typename T>
    std::thread join_thread(std::function<T> new_func,const
    std::string & msg){
        std::thread new_thread(new_func);
        Su::sylar_log_write(msg,Su::LogLevel::INFO);
        return std::move(new_thread);
    }

    //函数运行时间
    template <typename T>
    std::chrono::duration<double,std::ratio<1,1000>> func_run_time(std::function<T> func, int n){
        auto start = std::chrono::high_resolution_clock::now();
        while(n--){
            func();
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto result = std::chrono::duration<double,std::ratio<1,1000>>(end-start);
        return result;
    }
}

#endif //LOG_FUN_H
