
#ifndef LOG_LOG_LOG
#define LOG_LOG_LOG

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


//��ʽ����
/*
 * %m ��Ϣ����
 *
 *
 */



namespace Su {
    // ��־����
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

        static const char *ToString(LogLevel::level level);
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
        const LogLevel::level getLevel() const { return m_level; }
        std::stringstream& getSS() { return m_ss; }

        //������
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::level level
            , const char* file, int32_t line, uint32_t elapse
            , uint32_t thread_id, uint32_t fiber_id, uint64_t time
            , const std::string& thread_name);

        //ʡ��һЩû��Ҫ��Ϣ
        LogEvent(std::shared_ptr<Logger> logger, LogLevel::level level, const char *file, int32_t line):
                LogEvent(logger,level,file,line,0,0,0,time(0),"")
        {

        }
    private:
        std::shared_ptr<Logger> m_logger;
        LogLevel::level m_level;  // ��־����
        std::string msg_content;		//��־����
        uint32_t m_line;				//�к�
        uint32_t m_elapse = 0;			// ����������ʼ�����ڵĺ�����
        uint32_t m_threadid = 0;		// �߳�id
        uint32_t m_fiberid = 0;          //Э��id
        uint32_t m_time;				// ʱ���
        const char* filename = nullptr;  // �ļ���
        std::stringstream m_ss;          // ��־������
        std::string m_threadname = "";
    };

    /**
* @brief ��־�¼���װ��
*/
    class LogEventWrap {
    public:

        /**
         * @brief ���캯��
         * @param[in] e ��־�¼�
         */
        LogEventWrap(LogEvent::ptr e);

        /**
         * @brief ��������
         */
        ~LogEventWrap();

        /**
         * @brief ��ȡ��־�¼�
         */
        LogEvent::ptr getEvent() const { return m_event; }

        /**
         * @brief ��ȡ��־������
         */
        std::stringstream& getSS();
    private:
        /**
         * @brief ��־�¼�
         */
        LogEvent::ptr m_event;
    };


    //��־��ʽ��
    class LogFormatter {
    public:
        using ptr = std::shared_ptr<LogFormatter>;
        LogFormatter(const std::string& pattern); //patter��ʼ��

        std::string format(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event);
        std::ostream& format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event);
    public:
        class FormatterItem {
        public:
            typedef std::shared_ptr<FormatterItem> ptr;
            /**
             * @brief ��������
             */
            virtual ~FormatterItem() {}
            /**
             * @brief ��ʽ����־����
             * @param[in, out] os ��־�����
             * @param[in] logger ��־��
             * @param[in] level ��־�ȼ�
             * @param[in] event ��־�¼�
             */
            virtual void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) = 0;
        };


        void init();

        std::vector<FormatterItem::ptr> m_items;
        std::string m_pattern;
        bool m_error = false;
    };

    //������־����Ļ���
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
        //���캯��
        Logger(const std::string& name = "root");

        void log(LogLevel::level level, LogEvent::ptr event);

        void debug(LogEvent::ptr event);
        void info(LogEvent::ptr event);
        void warn(LogEvent::ptr event);
        void error(LogEvent::ptr event);
        void fatal(LogEvent::ptr event);

        void setLevel(LogLevel::level level){
            m_level = level;
        }

        void addAppender(LogAppender::ptr appender);
        void delAppender(LogAppender::ptr appender);

        std::string getName() const { return m_name; };
    private:
        std::string m_name; // logger������
        LogLevel::level m_level;   // loger��level����
        std::list<LogAppender::ptr> m_appenders; //���������
    };


    class StdOutAppender :public LogAppender {
    public:
        using ptr = std::shared_ptr<StdOutAppender>;
        virtual void log(std::shared_ptr<Logger> logger, LogLevel::level level, Su::LogEvent::ptr event) override;
    };
    class FileOutAppender :public LogAppender {
    public:
        using ptr = std::shared_ptr<FileOutAppender>;
        virtual void log(std::shared_ptr<Logger> logger, LogLevel::level level, Su::LogEvent::ptr event) override;
        FileOutAppender(const std::string& filename);

        bool reopen();
    private:
        std::string m_filename;
        std::ofstream m_file;
    };

    Su::Logger::ptr initlogger();
     void sylar_log_write(const std::string & msg,Su::LogLevel::level level);

}

#endif