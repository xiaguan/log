//
// Created by 75108 on 2022/3/17.
//

#ifndef LIB_TEST_LEXICAL_CAST_H
#define LIB_TEST_LEXICAL_CAST_H

#include <type_traits>
#include <string>
#include <cstdlib>
#include <algorithm>
#include <stdexcept>
#include <cctype>
#include <cstring>
#include <sstream>

namespace detail{
    template <typename From,typename To>
    class Converter{

    };

    template<>
    class Converter<std::string,std::string>{
    public:
        static std::string convert(const std::string & s){
            return s;
        }
    };

    // 把类型T转换为string,就调用标准库的就好了，没什么特别的
    template <typename From>
    class Converter<From,std::string>{
    public:
         static std::string convert(const From & from){
            return std::to_string(from);
        }
    };

    // 这边就要手打了，就是从string到各种东西，我们也选择使用标准库的std::stoxxxx
    // 有个问题待解决就是没有unsigned int
    template<> class Converter<std::string,int>{
    public:
        static int convert(const std::string & from){
            return stoi(from);
        }
    };

    template<> class Converter<std::string,unsigned int>{
    public:
        static unsigned int convert(const std::string & from){
            std::stringstream ss(from);
            unsigned int result;
            ss >> result;
            return result;
        }
    };

    template<> class Converter<std::string,long>{
    public:
        static long convert(const std::string & from){
            return std::stol(from);
        }
    };

    template<> class Converter<std::string,long long >{
    public:
        static long long convert(const std::string & from){
            return std::stoll(from);
        }
    };

    template<> class Converter<std::string,unsigned long>{
    public:
        static unsigned long convert(const std::string & from){
            return std::stoul(from);
        }
    };

    template<> class Converter<std::string,unsigned long long>{
    public:
        static unsigned long long convert(const std::string & from){
            return std::stoull(from);
        }
    };

    template<> class Converter<std::string,double>{
    public:
        static double convert(const std::string & from){
            return std::stod(from);
        }
    };

    template<> class Converter<std::string,long double>{
    public:
        static long double convert(const std::string & from){
            return std::stold(from);
        }
    };

    template<> class Converter<std::string,float>{
    public:
        static float convert(const std::string & from){
            return std::stof(from);
        }
    };

}

/*
 * 这里是我们的lexical_cast的最基本形式，就是
 */
// 这里的意思是from和to类型不同，需要转换

template <typename From, typename To>
class lexical_cast{
    public:
     To operator()(const From & from){
        return detail::Converter<From,To>::convert(from);
    }
};

#endif //LIB_TEST_LEXICAL_CAST_H
