//
// Created by 75108 on 2022/3/12.
//

#ifndef LIB_TEST_SINGLETON_H
#define LIB_TEST_SINGLETON_H

#include <memory>

namespace su{
    template <typename T,class X = void,int N = 0>
    class Singleton{
    public:
        static T * GetInstance(){
            static T v;
            return &v;
        }
    };

    template <typename T,class X = void,int N = 0>
    class SingletonPtr{
    public:
        static std::shared_ptr<T> GetInstance(){
            static std::shared_ptr<T> v(new T);
            return v;
        }
    };
}

#endif
