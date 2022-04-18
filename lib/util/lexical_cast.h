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

using namespace std;

namespace detail{

     inline const char * s_true = "true";
     inline const char* s_false = "false";

    template <typename To, typename From>
    class Converter{

    };

    template <typename From>
    class Converter<long,From>{
    public:
        static long convert(const From & from){
            return std::stol(from);
        }
    };

    template <typename From>
    class Converter<int,From>{
    public:
        static long convert(const From & from){
            return std::stoi(from);
        }
    };

    template <typename From>
    struct Converter<long long ,From>{
        static long long convert(const From & from){
            return std::stoll(from);
        }
    };

    template <typename From>
    class Converter<unsigned long long,From>{
    public:
        static long convert(const From & from){
            return std::stoull(from);
        }
    };

    template <typename From>
    struct Converter<float,From>{
    public:
        static float convert(const From & from){
            return std::stof(from);
        }
    };

    template <typename From>
    struct Converter<double,From>{
    public:
        static float convert(const From & from){
            return std::stod(from);
        }
    };

    template <typename From>
    struct Converter<string,From>{
    public:
        static std::string convert(const From & from){
            return std::to_string(from);
        }
    };

    

}

template <typename To, typename From>
    typename std::enable_if<!std::is_same<To, From>::value, To>::type lexical_cast(const From& from)
    {
        return detail::Converter<To, From>::convert(from);
    }

    template <typename To, typename From>
    typename std::enable_if<std::is_same<To, From>::value, To>::type lexical_cast(const From& from)
    {
        return from;
    }

#endif //LIB_TEST_LEXICAL_CAST_H
