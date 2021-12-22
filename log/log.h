#pragma once
#include <iostream>
#include <inttypes.h>
#include <string>
#include <memory>
#include <list>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <time.h>
#include <map>
#include <functional>
#include <initializer_list>
#include <sstream>



namespace Log {
    // 日志级别
    class Logger;

    class LogLevel {
    public:
        enum level {
            DEBUG = 1,
            INFO = 2,
            WARN = 3,
            ERROR = 4,
            FATAL = 5,
        };
        static const char* ToString(LogLevel::level level);
    };


    class LogEvent {
    public:
        using ptr = std::shared_ptr<LogEvent>;
        const char* getFile() const { return filename; }
        int32_t getLine() const { return m_line; }
        uint32_t getELapse() const { return m_elapse; }
        uint32_t getThread() const { return m_threadid; }
        uint32_t getFiber() const { return m_fiberid; }
        std::string getContent() const { return m_ss.str(); }
        std::shared_ptr<Logger> getLogger() const { return m_logger; }
        uint32_t getTime() const { return m_time; }
        LogLevel::level getLevel() const { return m_level; }
        std::stringstream& getSS() { return m_ss; }

        LogEvent(std::shared_ptr<Logger> logger, LogLevel::level level
            , const char* file, int32_t line, uint32_t elapse
            , uint32_t thread_id, uint32_t fiber_id, uint64_t time
            , const std::string& thread_name);
    private:
        std::shared_ptr<Logger> m_logger;
        LogLevel::level m_level;  // 日志级别
        std::string msg_content;		//日志内容
        uint32_t m_line;				//行号
        uint32_t m_elapse = 0;			// 程序启动开始到现在的毫秒数
        uint32_t m_threadid = 0;		// 线程id
        uint32_t m_fiberid = 0;          //协程id
        uint32_t m_time;				// 时间戳
        const char* filename = nullptr;  // 文件名
        std::stringstream m_ss;          // 日志内容流
        std::string m_threadname = "";
    };

    /**
* @brief 日志事件包装器
*/
    class LogEventWrap {
    public:

        /**
         * @brief 构造函数
         * @param[in] e 日志事件
         */
        LogEventWrap(LogEvent::ptr e);

        /**
         * @brief 析构函数
         */
        ~LogEventWrap();

        /**
         * @brief 获取日志事件
         */
        LogEvent::ptr getEvent() const { return m_event; }

        /**
         * @brief 获取日志内容流
         */
        std::stringstream& getSS();
    private:
        /**
         * @brief 日志事件
         */
        LogEvent::ptr m_event;
    };


    //日志格式化
    class LogFormatter {
    public:
        using ptr = std::shared_ptr<LogFormatter>;
        LogFormatter(const std::string& pattern); //patter初始化

        std::string format(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event);
        std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event);
    public:
        class FormatterItem {
        public:
            typedef std::shared_ptr<FormatterItem> ptr;
            /**
             * @brief 析构函数
             */
            virtual ~FormatterItem() {}
            /**
             * @brief 格式化日志到流
             * @param[in, out] os 日志输出流
             * @param[in] logger 日志器
             * @param[in] level 日志等级
             * @param[in] event 日志事件
             */
            virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) = 0;
        };


        void init();

        std::vector<FormatterItem::ptr> m_items;
        std::string m_pattern;
        bool m_error = false;
    };

    //这是日志输出的基类
    class LogAppender {
    public:
        using ptr = std::shared_ptr<LogAppender>;
        virtual ~LogAppender() {}
        virtual void log(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) = 0;
        LogFormatter::ptr getformatter() { return m_formatter; }
        void setFormater(LogFormatter::ptr format) { m_formatter = format; }
        void setLevel(LogLevel::level level) { m_level = level; }

    protected:
        LogLevel::level m_level;
        LogFormatter::ptr m_formatter;
    };

    class Logger : public std::enable_shared_from_this<Logger> {
    public:
        using ptr = std::shared_ptr<Logger>;
        //构造函数
        Logger(const std::string& name = "root");

        void log(LogLevel::level level, LogEvent::ptr event);

        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);

        std::string getName() const { return m_name; };
    private:
        std::string m_name; // logger的名字
        LogLevel::level m_level;   // loger的level级别
        std::list<LogAppender::ptr> m_appenders; //输出器链表
    };


    class StdOutAppender :public LogAppender {
    public:
        using ptr = std::shared_ptr<StdOutAppender>;
        virtual void log(std::shared_ptr<Logger> logger, LogLevel::level level, Log::LogEvent::ptr event) override;
    };
    class FileOutAppender :public LogAppender {
    public:
        using ptr = std::shared_ptr<FileOutAppender>;
        virtual void log(std::shared_ptr<Logger> logger, LogLevel::level level, Log::LogEvent::ptr event) override;
        FileOutAppender(const std::string& filename);

        bool reopen();
    private:
        std::string m_filename;
        std::ofstream m_file;
    };
}