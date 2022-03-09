//
// Created by 75108 on 2022/1/24.
//

#ifndef Su_Lib_Log
#define Su_Lib_Log

#include <iostream>
#include <string>
#include <chrono>
#include <memory>
#include <utility>
#include <vector>
#include <fstream>
#include <sstream>
#include <map>
#include <functional>
#include <tuple>
#include <chrono>
#include <iomanip>
#include <thread>
#include <mutex>

static time_t get_time_now(){
    auto time_now = std::chrono::system_clock::now();
    auto tm_t = std::chrono::system_clock::to_time_t(time_now);
    return tm_t;
}


#define SU_LOG_LEVEL(logger, level) \
         su::log::EventWrap(std::make_shared<su::log::Event>(__FILE__,__LINE__,0,get_time_now(),level,logger)).getSS()

#define SU_LOG_DEBUG(logger) \
SU_LOG_LEVEL(logger,su::log::Level::DEBUG)

#define SU_LOG_ERROR(logger) \
SU_LOG_LEVEL(logger,su::log::Level::ERROR)

#define SU_LOG_WARN(logger) \
SU_LOG_LEVEL(logger,su::log::Level::WARN)

#define SU_LOG_FATAL(logger) \
SU_LOG_LEVEL(logger,su::log::Level::FATAL)

#define SU_LOG_INFO(logger) \
SU_LOG_LEVEL(logger,su::log::Level::INFO)

#define SU_LOG_ROOT su::log::Single_logger::getInstance() //NOLINT



namespace su{
    namespace log {
        class Logger;

        //日志级别设定，采用enum class
        enum class Level {
            DEBUG = 0,
            ERROR = 1,
            INFO = 2,
            FATAL = 3,
            WARN = 4
        };

        std::string ToString(Level level);

        //事件类设计，
        class Event {
        public:
            typedef std::shared_ptr<Event> ptr;

            Event(std::string filename, unsigned int line, unsigned int thread_id, std::time_t time, Level level,
                  std::shared_ptr<Logger> logger);

            // 一堆的get
            std::stringstream &getSS() { return m_log_content; }

            std::string get_content() const { return m_log_content.str(); }

            std::string get_filename() const { return m_file_name; }

            unsigned int get_line() const { return m_line; }

            unsigned int get_thread_id() const { return m_thread_id; }

            Level get_level() const { return m_level; }

            std::shared_ptr<Logger> get_logger() const { return m_logger; }

            std::time_t get_time() const { return m_time; }

        private:
            Level m_level;
            std::stringstream m_log_content;   //日志消息内容
            std::string m_file_name;     // 日志中要输出的文件名
            unsigned int m_line;    //行号
            unsigned int m_thread_id;  //线程号
            std::time_t m_time;   //发生的时间
            std::shared_ptr<Logger> m_logger;

            //后期还会加入一些event的数据
        };


        class Formatter {
            /**
            * @brief 构造函数
            * @param[in] pattern 格式模板
            * @details
            *  %m 消息
            *  %p 日志级别
            *  %r 累计毫秒数  没有写
            *  %c 日志名称
            *  %t 线程id
            *  %n 换行
            *  %d 时间
            *  %f 文件名
            *  %l 行号
            *  %T 制表符
            *  %F 协程id  没实现
            *  %N 线程名称  没实现
            *
            *  默认格式 "%d{%Y-%m-%d %H:%M:%S}%T%t%T%T%T[%p]%T[%c]%T%f:%l%T%m%n"
            */
        public:
            typedef std::shared_ptr<Formatter> ptr;

            // patter初始化
            explicit Formatter(std::string pattern);

            //两种format形式
            std::string format(std::shared_ptr<Logger> logger, Event::ptr event);

            std::ostream &format(std::ostream &os, std::shared_ptr<Logger> logger, Event::ptr event);

            //日志内容格式化
            class FormatItem {
            public:
                typedef std::shared_ptr<FormatItem> ptr;

                //这也是一个纯虚基类，给定信息，进行格式处理
                virtual ~FormatItem() = default;

                virtual void format(std::ostream &os, std::shared_ptr<Logger> logger, Event::ptr event) = 0;
            };

            void init();

            bool isError() const;

            std::string getPatter() const { return m_pattern; };
        private:
            std::string m_pattern;   //format格式控制
            bool m_error = false;
            //日志解析后的格式
            std::vector<FormatItem::ptr> m_items;
        };

        class Appender {
        public:
            typedef std::shared_ptr<Appender> ptr;

            // 纯虚输出函数,这里我并没有打算向参数里加入另外一个级别
            // 输出器可以设定它自己要输出的日志级别，但传入时，没必要再进行多余日志级别的传入
            // 抽象类的析构必须是virtual的
            virtual void output_log(Event::ptr event) = 0;

            //设置输出格式
            void set_format(Formatter::ptr new_fmt);

            //获取级别
            Level get_level() const { return m_level; }

            //设置级别
            void set_level(Level level = Level::DEBUG);

            Formatter::ptr get_fmt() const {return m_fmt;}

            virtual ~Appender() = default;

        protected:
            Level m_level;  //达到级别才进行输出
            Formatter::ptr m_fmt{nullptr};
        };

        //设计第一个输出器，标准输出
        class StdOutputAppender : public Appender {
            //它并不需要去设置新的东西，只需要把内容答应到标准输出就Ok,
            // 当要把纯虚函数override掉，就这样
        public:
            typedef std::shared_ptr<StdOutputAppender> ptr;

            void output_log(Event::ptr event) override;

        private:

        };

        // 从文件进行输出
        class FileOutputAppender : public Appender {
        public:
            typedef std::shared_ptr<FileOutputAppender> ptr;

            explicit FileOutputAppender(std::string name);

            void output_log(Event::ptr event) override;

            bool reopen();

        private:
            const std::string m_file_name;
            //文件流
            std::ofstream m_filestream;
        };

        class Logger {
        public:
            typedef std::shared_ptr<Logger> ptr;

            explicit Logger(std::string name = "root");

            //添加日志事件
            void add_event(Event::ptr event);

            // 添加日志输出器
            void add_appender(Appender::ptr appender);

            //日志输出
            void out_put_all();

            void log(Event::ptr event);

            //设置格式
            void set_fmt(Formatter::ptr new_fmt);

            std::string get_name() { return m_name; }

        private:
            std::vector<Event::ptr> m_events;  //只用存指针，不存它的全部内容
            std::vector<Appender::ptr> m_appenders;  //也是只存指针
            std::string m_name;   //日志器的名字
            Formatter::ptr m_fmt;  //日志器统一的输出格式
            std::mutex m_mut;
        };


        // 我们需要一个RAII类来进行每一个event的生命周期管理
        class EventWrap {
        public:
            EventWrap(Event::ptr event);

            ~EventWrap();

            std::stringstream &getSS();

        private:
            Event::ptr m_event;
        };


        template<class T, class X = void, int N = 0>
        class SingletonPtr {
        public:
            /**
             * @brief 返回单例智能指针
             */
            static std::shared_ptr<T> GetInstance() {
                static std::shared_ptr<T> v(new T);
                return v;
                //return GetInstancePtr<T, X, N>();
            }
        };

        class Single_logger{
        public:
            static Logger::ptr getInstance();
        private:
            Single_logger();
            static std::shared_ptr<Single_logger> m_single;
        };


    }
}

#endif //Su_Lib_Log
