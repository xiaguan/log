//
// Created by 75108 on 2022/4/25.
//

#ifndef ZIPPER_ZIPPER_H
#define ZIPPER_ZIPPER_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <map>
#include <vector>
#include <memory>
#include <queue>
#include <string>
#include <bitset>
#include <chrono>
#include <log.h>




template <typename T>
void print_in_two(T t){
    for(int j = sizeof t * 8 -1;j>=0;j--){
        std::cout << (( t >> j) & 1) ;
    }
}



template <typename T>
std::string print_ss_two(T t){
    std::string result;
    for(int j = sizeof t * 8 -1;j>=0;j--){
        result.push_back(( t >> j) & 1 ? '1':'0') ;
    }
    return result;
}


class Zipper;

namespace detail{
    void formatMap(std::map<char,std::string> & map,std::ostream & out);
    std::map<std::string,char> getMap(std::istream & in);
    struct Node{
        typedef std::shared_ptr<Node> ptr;
        bool is_value{false};
        int val; // 权重
        char c ; // 一个字节的值
        Node::ptr left;
        Node::ptr right;
    };

    class HuffMan{
    public:
        friend class Zipper;
        HuffMan():root(nullptr){}
        unsigned long long construct(std::istream & in);
        void format(std::istream &in,std::ostream & out);
        void reformat(std::istream & in,std::ostream & out);
        void print();

        std::map<std::string,char> decode;
    private:
        void makeTree(const std::vector<Node::ptr> & nodes);
        void dfs(const Node::ptr& node ,const std::string&  prefix);
        std::map<char,std::string> encode;
        Node::ptr root;
    };
}


class Zipper {
public:
    void encode(const std::string & filename);
    void decode(const std::string & filename);
private:
    detail::HuffMan m_tree;
};


#endif //ZIPPER_ZIPPER_H
