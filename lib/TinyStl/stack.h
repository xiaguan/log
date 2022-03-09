//
// Created by 75108 on 2022/3/9.
//

#ifndef LOG_STACK_H
#define LOG_STACK_H

//
// Created by 75108 on 2022/3/2.
//

#ifndef UNTITLED_STACK_H
#define UNTITLED_STACK_H

#include <memory>
#include <iostream>
#include <exception>
#include <mutex>

namespace su{

    template <typename elementType>
    class stack {
    public:
        /*
         * 返回栈的大小的函数
         */
        size_t size() const {return first_free-elements;}

        /*
         * 返回栈是否为空
         */
        bool empty() const{return (first_free==elements);}

        /*
         * 新元素入栈
         */
        void push(elementType new_element){

            check_is_full();
            m_allocator.template construct(first_free,new_element);first_free++;
        }

        /*
         * 弹出栈顶元素不返回该元素
         */
        void pop(){
            if(empty()) return;
            m_allocator.template destroy(--first_free);
        }

        /*
         * 返回栈顶元素的引用
         */
        elementType& top(){
            if(empty()) {
                std::cerr <<"stack:  use top() in an empty stack "<<std::endl;
            }
            return *(first_free-1);
        }


        // 构造函数
        // 默认构造函数
        explicit stack(size_t new_sz = 100){
            elements = m_allocator.allocate(new_sz);
            first_free = elements;
            cap = elements+new_sz;
        }

        ~stack(){
            if(elements){
                for(auto p = first_free; p != elements;)
                    m_allocator.template destroy(--p);
                m_allocator.deallocate(elements,cap-elements);
            }
        }

        stack& operator=(const stack& other){
            auto sz = size();
            other.elements = other.m_allocator.allocate(sz);
            first_free = elements+sz;
            other.cap = elements+capacity();

            memcpy(other.elements,elements,sz);
        }

    private:
        using size_t = unsigned long long int;
        std::allocator<elementType> m_allocator;

        elementType *elements;  //指向数组首元素的指针
        elementType *first_free;  // 指向数第一个空闲元素的指针
        elementType *cap;        //指向分配的数组的最后一个元素的指针

        void check_is_full(){
            if(size() == capacity()){
                resize();
            }
        }

        void free(){
            if(elements){
                for(auto p = first_free; p != elements;)
                    m_allocator.template destroy(--p);
                m_allocator.deallocate(elements,cap-elements);
            }
        }

        // 重新分配空间大小
        void resize(){

            auto new_sz = size()?2*size():1;
            auto new_data = m_allocator.allocate(new_sz);

            auto dest = new_data;
            auto elem = elements;

            for(size_t i = 0; i!= size();i++){
                m_allocator.template construct(dest++,std::move(*elem++));
            }

            //释放原先的数据
            free();
            elements = new_data;
            first_free = dest;
            cap = elements+new_sz;
        }

        // 申请的空间的大小
        size_t capacity() const { return cap-elements;}
    };


    /*
     * 线程安全的栈
     */

    template <typename T>
    class threadsafe_stack{
    private:
        stack<T> data;
        mutable std::mutex m;
    public:
        threadsafe_stack(){}
        threadsafe_stack(const threadsafe_stack& other){
            std::lock_guard guard(m);
            data = other.data;
        }
        threadsafe_stack& operator=(const threadsafe_stack& other) = delete;

        void push(T new_value){
            std::lock_guard guard(m);
            data.push(new_value);
        }

        std::shared_ptr<T> pop(){
            std::lock_guard guard(m);
            if(data.empty()) return nullptr;
            std::shared_ptr<T> const res(std::make_shared<T>(data.top()));
            data.pop();
            return res;
        }

        void pop(T & value){
            std::lock_guard guard(m);
            if(data.empty()) return ;
            value = data.top();
            data.pop();
        }

        bool empty() const{
            std::lock_guard guard(m);
            return data.empty();
        }
    };

}


#endif //UNTITLED_STACK_H


#endif //LOG_STACK_H
