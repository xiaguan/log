#include <log.h>
#include <util.h>
#include <TinyStl/stack.h>
#include <stack>

const int N = 1e5;

int main(){
    su::log::Logger::ptr logger(new su::log::Logger("test"));

    su::log::StdOutputAppender::ptr std_appender(new su::log::StdOutputAppender());
    su::log::FileOutputAppender::ptr file_appender(new su::log::FileOutputAppender("test.txt"));

    logger->add_appender(file_appender);
    logger->add_appender(std_appender);

    su::Timer timer;

    std::cout << "-----------Test with 100000 push and pop repeat 100 times"<<std::endl;
    std::cout <<" The result of mine : ";
    timer.start();
    for(int j = 0;j<100;j++){
        su::stack<int> nums;
        for(int i = 0;i<N;i++) nums.push(i);
        for(int i = 0;i<N;i++) nums.pop();
    }
    timer.end();

    std::cout <<"The result of std : ";
    timer.start();
    for(int j = 0;j<100;j++){
        std::stack<int> std_stack;
        for(int i = 0;i<N;i++) std_stack.push(i);
        for(int i = 0;i<N;i++) std_stack.pop();
    }
    timer.end();

    std::cout <<"The result of threadsafe_stack : ";
    timer.start();
    for(int j = 0;j<100;j++){
        su::threadsafe_stack<int> nums;

        auto push_func = [&](int count){
            while(count--){
                nums.push(count);
            }
        };
        
        auto pop_func = [&](int count){
            while(count--){
                nums.pop();
            }
        };

        std::vector<std::thread> m_threads(4);  // harware_currency
        for(auto &t: m_threads){
            t = std::thread(push_func,N/4);
        }
        for(auto &t:m_threads) t.join();

        for(auto &t: m_threads){
            t = std::thread(pop_func,N/4);
        }
        for(auto &t:m_threads) t.join();

    }
    timer.end();
    std::cout << std::endl;



    std::cout << "-----------Test with 100 push and pop repeat 100000 times"<<std::endl;
    std::cout <<" The result of mine : ";
    timer.start();
    for(int j = 0;j<N;j++){
        su::stack<int> nums;
        for(int i = 0;i<100;i++) nums.push(i);
        for(int i = 0;i<100;i++) nums.pop();
    }
    timer.end();

    std::cout <<"-----------The result of std : ";
    timer.start();
    for(int j = 0;j<N;j++){
        std::stack<int> std_stack;
        for(int i = 0;i<100;i++) std_stack.push(i);
        for(int i = 0;i<100;i++) std_stack.pop();
    }
    timer.end();

    std::cout << std::endl;

    std::cout << "-----------Test with 300 push and pop repeat 100000 times"<<std::endl;
    std::cout <<" The result of mine : ";
    timer.start();
    for(int j = 0;j<N;j++){
        su::stack<int> nums;
        for(int i = 0;i<300;i++) nums.push(i);
        for(int i = 0;i<300;i++) nums.pop();
    }
    timer.end();

    std::cout <<"The result of std : ";
    timer.start();
    for(int j = 0;j<N;j++){
        std::stack<int> std_stack;
        for(int i = 0;i<300;i++) std_stack.push(i);
        for(int i = 0;i<300;i++) std_stack.pop();
    }
    timer.end();

    std::cout << std::endl;


    return 0;
}
