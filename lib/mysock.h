//
// Created by 75108 on 2021/12/28.
//

#ifndef LOG_MYSOCK_H
#define LOG_MYSOCK_H

#include "log.h"


//使用一个静态的logger进行日志的输出
void sock_log_event(Log::LogEvent::ptr ,Log::LogLevel::level level);

#endif //LOG_MYSOCK_H
