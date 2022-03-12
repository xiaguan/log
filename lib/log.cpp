//
// Created by 75108 on 2022/1/24.
//


#include "log.h"

#include <memory>
#include <utility>


namespace su{
    namespace log{

        std::string ToString(Level level){
            switch(level) {
#define XX(name) \
    case Level::name: \
        return #name; \
        break;
                XX(DEBUG);
                XX(INFO);
                XX(WARN);
                XX(ERROR);
                XX(FATAL);
#undef XX
            }
            return "UNKNOWN";
        }

        // Event 的 默认初始化
        Event::Event(std::string  filename, unsigned int line, unsigned int thread_id, std::time_t time,Level level,
                     Logger::ptr logger):
                m_file_name(std::move(filename)),m_line(line),m_thread_id(thread_id),m_time(time),m_level(level),m_logger(std::move(logger))
        {}

        Formatter::Formatter(std::string pattern):m_pattern(std::move(pattern)) {init();}

        class MessageFormatItem : public Formatter::FormatItem {
        public:
            explicit MessageFormatItem(const std::string& str = "") {}
            void format(std::ostream& os, Logger::ptr logger,Event::ptr event) override {
                os << event->get_content();
            }
        };

        class LevelFormatItem: public Formatter::FormatItem{
        public:
            explicit LevelFormatItem(const std::string & str = "") {}
            void format(std::ostream& os, Logger::ptr logger,Event::ptr event) override {
                os << ToString(event->get_level());
            }
        };

        class NameFormatItem : public Formatter::FormatItem{
        public:
            explicit NameFormatItem(const std::string & str = ""){}
            void format(std::ostream & os,Logger::ptr logger,Event::ptr event) override{
                os << event->get_logger()->get_name();
            }
        };

        class ThreadIdFormatItem : public Formatter::FormatItem{
        public:
            explicit ThreadIdFormatItem(const std::string & str=""){}
            void format(std::ostream & os,Logger::ptr logger,Event::ptr event) override{
                os << event->get_thread_id();
            }
        };

        class DataTimeFormatItem : public Formatter::FormatItem{
        public:
            explicit DataTimeFormatItem(std::string  fmt = "%Y-%m-%d %H:%M:%S"):
                    m_fmt(std::move(fmt)){
                if(m_fmt.empty()){
                    m_fmt = "%Y-%m-%d %H:%M:%S";
                }
            }

            void format(std::ostream & os,Logger::ptr logger,Event::ptr event) override{
                auto tm = event->get_time();
                // 让标准库帮我们完成这些事情
                os << std::put_time(std::localtime(&tm),m_fmt.c_str());
            }

        private:
            std::string m_fmt;
        };

        class FilenameFormatItem:public Formatter::FormatItem{
        public:
            explicit FilenameFormatItem(const std::string & str = ""){}

            void format(std::ostream & os,Logger::ptr logger,Event::ptr event) override{
                os << event->get_filename();
            }
        };

        class LineFormatItem: public Formatter::FormatItem{
        public:
            explicit LineFormatItem(const std::string& str = ""){}


            void format(std::ostream & os,Logger::ptr logger,Event::ptr event) override{
                os << event->get_line();
            }
        };

        class NewLineFormatItem: public Formatter::FormatItem{
        public:
            explicit NewLineFormatItem(const std::string& str = ""){}
            void format(std::ostream & os,Logger::ptr logger,Event::ptr event) override{
                os << std::endl;
            }
        };

        class StringFormatItem: public Formatter::FormatItem{
        public:
            explicit StringFormatItem(std::string  str):m_string(std::move(str)){}
            void format(std::ostream & os,Logger::ptr logger,Event::ptr event) override{
                os << m_string;
            }
        private:
            std::string m_string;
        };

        class TabFormatItem:public Formatter::FormatItem{
        public:
            explicit TabFormatItem(const std::string & str = ""){}

            void format(std::ostream & os,Logger::ptr logger,Event::ptr event) override{
                os << "\t";
            }
        };

        // 整个项目，最大的代码量，解析pattern
        // 示例pattern: %d%T%p%T%m%n
        //%xxx %xxx{xxx} %%
        // 同时这里好像也是一种设计模式，叫什么状态机的实现，以后有时间要读一下设计莫
        void Formatter::init() {
            //str, format, type
            std::vector<std::tuple<std::string, std::string, int> > vec;
            std::string nstr;

            // 遍历整个pattern
            for (size_t i = 0; i < m_pattern.size(); ++i) {

                if (m_pattern[i] != '%') {
                    nstr.append(1, m_pattern[i]);
                    continue;
                }

                //这里满足的条件是m_pattern[i] == '%'
                // i+1就是，举个例子就是%d后面的d
                //std::cout <<"m_pate  "<<m_pattern[i] <<std::endl;

                if ((i + 1) < m_pattern.size()) {
                    if (m_pattern[i + 1] == '%') {
                        //这里可能是特殊情况的处理
                        //追加一个%
                        nstr.append(1, '%');
                        continue;
                    }
                }

                // 双指针，n指向i的下一个，frm_begin初始化为0，还有个status
                size_t n = i + 1;
                int fmt_status = 0;
                size_t fmt_begin = 0;

                // 这两个string我还不知道是干什么的
                std::string str;
                std::string fmt;

                //保证n不越界
                // 这个while是用来解析特殊格式的比如说:%d{xxxx}什么什么的
                // 读一些没有注释的代码就很折磨
                while (n < m_pattern.size())
                {
                    // 如果status为0，并且i+1不是字母，并且也不是{}
                    if (!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{'
                                        && m_pattern[n] != '}')) {
                        //裁剪 从i+1取n-i-1个,如果n没有变化的话，这里取到一个空字符串
                        str = m_pattern.substr(i + 1, n - i - 1);
                        break;
                    }
                    if (fmt_status == 0) {
                        if (m_pattern[n] == '{') {
                            // 如果是{左大括号，会截取{前的一个字符成为str
                            // 并赋予fmt_status和fmt_begin值
                            str = m_pattern.substr(i + 1, n - i - 1);
                            fmt_status = 1; //解析格式
                            fmt_begin = n;
                            ++n;
                            continue;
                        }
                    } else if (fmt_status == 1) {
                        if (m_pattern[n] == '}') {
                            fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                            //std::cout << "#" << fmt << std::endl;
                            fmt_status = 0;
                            ++n;
                            break;
                            //结束读取
                        }
                    }
                    ++n;
                    if (n == m_pattern.size()) {
                        if (str.empty()) {
                            str = m_pattern.substr(i + 1);
                        }
                    }
                }

                if (fmt_status == 0) {
                    if (!nstr.empty()) {
                        vec.emplace_back(nstr, std::string(), 0);
                        nstr.clear();
                    }
                    vec.emplace_back(str, fmt, 1);
                    i = n - 1;
                } else if (fmt_status == 1) {
                    std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
                    m_error = true;
                    vec.emplace_back("<<pattern_error>>", fmt, 0);
                }
            }


            if(!nstr.empty()) {
                vec.emplace_back(nstr, "", 0);
            }

            //生成一个静态key-value对照，通过string调用function
            static std::map<std::string, std::function<FormatItem::ptr(const std::string& str)>> s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return FormatItem::ptr(new C(fmt));}}

                    XX(m,MessageFormatItem),           //m:消息
                    XX(p,LevelFormatItem),
                    XX(c,NameFormatItem),
                    XX(n,NewLineFormatItem),
                    XX(d,DataTimeFormatItem),
                    XX(f,FilenameFormatItem),
                    XX(l,LineFormatItem),
                    XX(T,TabFormatItem),
                    XX(t,ThreadIdFormatItem),
#undef XX
            };

            for(auto& i : vec) {
                if(std::get<2>(i) == 0) {
                    m_items.push_back(FormatItem::ptr(new StringFormatItem(std::get<0>(i))));
                } else {
                    auto it = s_format_items.find(std::get<0>(i));
                    if(it == s_format_items.end()) {
                        m_items.push_back(FormatItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                        m_error = true;
                    } else {
                        m_items.push_back(it->second(std::get<1>(i)));
                    }
                }

                //std::cout << "(" << std::get<0>(i) << ") - (" << std::get<1>(i) << ") - (" << std::get<2>(i) << ")" << std::endl;
            }
            //std::cout << m_items.size() << std::endl;
        }

        std::string Formatter::format(std::shared_ptr<Logger> logger, Event::ptr event) {
            std::ostringstream ss;
            for(auto & item:m_items){
                item->format(ss,logger,event);
            }
            return ss.str();
        }

        std::ostream &Formatter::format(std::ostream &os, std::shared_ptr<Logger> logger, Event::ptr event) {
            for(auto & item:m_items){
                item->format(os,logger,event);
            }
            return os;
        }

        bool Formatter::isError() const {
            return m_error;
        }


        void Appender::set_format(Formatter::ptr new_fmt) {
            m_fmt = std::move(new_fmt);
        }

        void Appender::set_level(Level level) {
            m_level = level;
        }

        Logger::Logger(std::string name):m_name(std::move(name)) {
            m_fmt = std::make_shared<Formatter>("%d{%Y-%m-%d %H:%M:%S}%T%t%T%T%T[%p]%T[%c]%T%f:%l%T%m%n");
        }

        bool FileOutputAppender::reopen() {
            std::ifstream in(m_file_name);
            return in.is_open();
        }

        FileOutputAppender::FileOutputAppender(std::string name):m_file_name(std::move(name)) {
            m_filestream.open(m_file_name);
            if(!m_filestream.is_open()){
                if(!reopen()){
                    std::cout <<"file open error"<<std::endl;
                }
            }
        }

        void Logger::set_fmt(Formatter::ptr new_fmt){
            m_fmt = std::move(new_fmt);
        }

        void Logger::log(Event::ptr event) {
            std::lock_guard<std::mutex> m_guard(m_mut);
            for(auto & appender : m_appenders){
                appender->output_log(event);
            }
        }

        void Logger::out_put_all() {
            for(auto & event:m_events){
                log(event);
            }
        }

        void Logger::add_appender(Appender::ptr appender) {
            if(!appender->get_fmt()){
                appender->set_format(m_fmt);
            }
            m_appenders.emplace_back(appender);
        }

        void StdOutputAppender::output_log(Event::ptr event) {
            m_fmt->format(std::cout,event->get_logger(),event);
        }

        void FileOutputAppender::output_log(Event::ptr event) {
            m_fmt->format(m_filestream,event->get_logger(),event);
        }

        FileOutputAppender::~FileOutputAppender() {
            m_filestream.close();
        }

        EventWrap::EventWrap(Event::ptr event):m_event(std::move(event)){}

        EventWrap::~EventWrap() {
            m_event->getSS()<<std::flush;
            m_event->get_logger()->log(m_event);
        }

        std::stringstream & EventWrap::getSS() {
            return m_event->getSS();
        }


        LoggerManager::LoggerManager() {
            m_root = std::make_shared<Logger>();

            m_root->add_appender(Appender::ptr(new StdOutputAppender));

            Appender::ptr file_appender(new FileOutputAppender("root.txt"));
            file_appender->set_level(Level::DEBUG);

            m_root->add_appender(file_appender);

            m_root->set_fmt(std::make_shared<Formatter>("d{%Y-%m-%d %H:%M:%S}%T%t%T%T%T[%p]%T[%c]%T%f:%l%T%m%n"));
        }

        std::shared_ptr<Logger> LoggerManager::getLogger(const std::string &name) {
            auto it = m_loggers.find(name);
            return it == m_loggers.end()?m_root:it->second;
        }
    }
}