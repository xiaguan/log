//
// Created by SuSun on 2022/4/28.
//
#include <iostream>
#include <config.h>
using namespace std;

void test(){
    ofstream out("result.yml");
    YAML::Node root,log;
    log["logs"] = root;
    YAML::Node logger1;
    logger1["name"] = "root";
    logger1["fmt"] = "%%T";
    YAML::Node fileappender,appenders,stdoutappender;
    fileappender["type"] = "FileOutAppeder";
    fileappender["level"] = "INFO";
    fileappender["filename"] = "root.txt";

    stdoutappender["type"] = "StdOutAppender";
    stdoutappender["level"] = "DEBUG";

    appenders.push_back(fileappender);
    appenders.push_back(stdoutappender);
    logger1["appenders"] = appenders;

    YAML::Node logger2;
    logger2["name"] = "system";
    logger2["fmt"] = "%d%T";
    root.push_back(logger1);
    root.push_back(logger2);
    out << log ;
}

void test2(){
    return;
}

int main(){
    test();
    test2();
    return 0;
}
