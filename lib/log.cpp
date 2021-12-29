#include "log.h"

namespace Log {



    const char* LogLevel::ToString(LogLevel::level level)
    {
        switch (level) {
#define XX(name)\
		case LogLevel::name: \
			return #name;\
			break;

            XX(DEBUG);
            XX(INFO);
            XX(WARN);
            XX(ERROR);
            XX(FATAL);
#undef XX
        default:
            return "UNKNOW";
        }
        return "UNKKNOW";
    }

    LogEventWrap::LogEventWrap(LogEvent::ptr e)
        :m_event(e) {
    }


    LogEventWrap::~LogEventWrap() {
        m_event->getLogger()->log(m_event->getLevel(), m_event);
    }

    std::stringstream& LogEventWrap::getSS() {
        return m_event->getSS();
    }

    class MessageFormatItem : public LogFormatter::FormatterItem {
    public:
        MessageFormatItem(const std::string& str = "") {}
        void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) override {
            os << event->getContent();
        }
    };
    class LevelFormatItem : public LogFormatter::FormatterItem {
    public:
        LevelFormatItem(const std::string& str = "") {}
        void format(std::ostream& os, std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) override {
            os << LogLevel::ToString(level);
        }
    };

    class ElapseFormatItem : public LogFormatter::FormatterItem {
    public:
        ElapseFormatItem(const std::string& str = "") {}
        void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, LogEvent::ptr event) override {
            os << event->getELapse();
        }
    };

    class NameFormatItem : public LogFormatter::FormatterItem {
    public:
        NameFormatItem(const std::string& str = "") {}
        void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, LogEvent::ptr event) override {
            os << event->getLogger()->getName();
        }
    };

    class ThreadIdFormatItem : public LogFormatter::FormatterItem {
    public:
        ThreadIdFormatItem(const std::string& str = "") {}
        void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, LogEvent::ptr event) override {
            os << event->getThread();
        }
    };

    class FiberIdFormatItem : public LogFormatter::FormatterItem {
    public:
        FiberIdFormatItem(const std::string& str = "") {}
        void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, LogEvent::ptr event) override {
            os << event->getFiber();
        }
    };

    class ThreadNameFormatItem : public LogFormatter::FormatterItem {
    public:
        ThreadNameFormatItem(const std::string& str = "") {}
        void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, LogEvent::ptr event) override {
            os << event->getThread();
        }
    };

    class DateTimeFormatItem : public LogFormatter::FormatterItem {
    public:
        DateTimeFormatItem(const std::string& format = "%Y-%m-%d %H:%M:%S")
            :m_format(format) {
            if (m_format.empty()) {
                m_format = "%Y-%m-%d %H:%M:%S";
            }
        }

        void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, LogEvent::ptr event) override {
            struct tm now_tm;
            const time_t time_m = event->getTime();
            localtime_s(&now_tm,&time_m);
            char buf[64];
            strftime(buf, sizeof(buf), m_format.c_str(), &now_tm);
            os << buf;
        }
    private:
        std::string m_format;
    };

    class FilenameFormatItem : public LogFormatter::FormatterItem {
    public:
        FilenameFormatItem(const std::string& str = "") {}
        void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, LogEvent::ptr event) override {
            os << event->getFile();
        }
    };

    class LineFormatItem : public LogFormatter::FormatterItem {
    public:
        LineFormatItem(const std::string& str = "") {}
        void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, LogEvent::ptr event) override {
            os << event->getLine();
        }
    };

    class NewLineFormatItem : public LogFormatter::FormatterItem {
    public:
        NewLineFormatItem(const std::string& str = "") {}
        void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, LogEvent::ptr event) override {
            os << std::endl;
        }
    };


    class StringFormatItem : public LogFormatter::FormatterItem {
    public:
        StringFormatItem(const std::string& str)
            :m_string(str) {}
        void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, LogEvent::ptr event) override {
            os << m_string;
        }
    private:
        std::string m_string;
    };

    LogEvent::LogEvent(std::shared_ptr<Logger> logger, LogLevel::level level
        , const char* file, int32_t line, uint32_t elapse
        , uint32_t thread_id, uint32_t fiber_id, uint64_t time
        , const std::string& thread_name)
        :filename(file)
        , m_line(line)
        , m_elapse(elapse)
        , m_threadid(thread_id)
        , m_fiberid(fiber_id)
        , m_time(time)
        , m_threadname(thread_name)
        , m_logger(logger)
        , m_level(level) {
    }



    class TabFormatItem : public LogFormatter::FormatterItem {
    public:
        TabFormatItem(const std::string& str = "") {}
        void format(std::ostream& os, Logger::ptr logger, LogLevel::level level, LogEvent::ptr event) override {
            os << "\t";
        }
    private:
        std::string m_string;
    };


    Logger::Logger(const std::string& name) :m_name(name)
    {

    }


    void Log::Logger::log(LogLevel::level level, LogEvent::ptr event)
    {
        if (level >= m_level) {
            auto self = shared_from_this();
            for (auto& appender : m_appenders) {
                appender->log(self, level, event);
            }
        }
    }

    void Logger::debug(LogEvent::ptr event)
    {
        log(LogLevel::DEBUG, event);
    }

    void Logger::info(LogEvent::ptr event)
    {
        log(LogLevel::level::INFO, event);
    }

    void Logger::warn(LogEvent::ptr event)
    {
        log(LogLevel::level::WARN, event);
    }

    void Logger::error(LogEvent::ptr event)
    {
        log(LogLevel::level::ERROR, event);
    }

    void Logger::fatal(LogEvent::ptr event)
    {
        log(LogLevel::level::FATAL, event);
    }

    void Logger::addAppender(LogAppender::ptr appender) {
        m_appenders.push_back(appender);
    }

    void Logger::delAppender(LogAppender::ptr appender) {
        for (auto it = m_appenders.begin(); it != m_appenders.end(); it++) {
            if (*it == appender) {
                m_appenders.erase(it);
                break;
            }
        }
    }





    void StdOutAppender::log(std::shared_ptr<Logger> logger, LogLevel::level level, Log::LogEvent::ptr event)
    {
        if (level >= m_level) {

            std::cout << m_formatter->format(logger, level, event);
        }
    }

    bool Log::FileOutAppender::reopen()
    {
        if (m_file.is_open()) {
            m_file.close();
        }
        m_file.open(m_filename);
        return !!m_file;
    }

    void FileOutAppender::log(std::shared_ptr<Logger> logger, LogLevel::level level, Log::LogEvent::ptr event)
    {
        m_file << m_formatter->format(logger, level, event);
    }

    FileOutAppender::FileOutAppender(const std::string& filename) :m_filename(filename)
    {
        m_file.open(filename,std::ofstream::app);
    }



    Log::LogFormatter::LogFormatter(const std::string& pattern) :m_pattern(pattern)
    {
        init();
    }

    std::string Log::LogFormatter::format(std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event)
    {
        std::stringstream ss;
        for (auto& i : m_items) {
            i->format(ss, logger, level, event);
        }
        return ss.str();
    }


    std::ostream& LogFormatter::format(std::ostream& ofs, std::shared_ptr<Logger> logger, LogLevel::level level, LogEvent::ptr event) {
        for (auto& i : m_items) {
            i->format(ofs, logger, level, event);
        }
        return ofs;
    }

    //%xxx %xxx(xxx)  %%

    void LogFormatter::init() {
        //str, format, type
        std::vector<std::tuple<std::string, std::string, int> > vec;
        std::string nstr;
        for (size_t i = 0; i < m_pattern.size(); ++i) {
            if (m_pattern[i] != '%') {
                nstr.append(1, m_pattern[i]);
                continue;
            }

            if ((i + 1) < m_pattern.size()) {
                if (m_pattern[i + 1] == '%') {
                    nstr.append(1, '%');
                    continue;
                }
            }

            size_t n = i + 1;
            int fmt_status = 0;
            size_t fmt_begin = 0;

            std::string str;
            std::string fmt;
            while (n < m_pattern.size()) {
                if (!fmt_status && (!isalpha(m_pattern[n]) && m_pattern[n] != '{'
                    && m_pattern[n] != '}')) {
                    str = m_pattern.substr(i + 1, n - i - 1);
                    break;
                }
                if (fmt_status == 0) {
                    if (m_pattern[n] == '{') {
                        str = m_pattern.substr(i + 1, n - i - 1);
                        //std::cout << "*" << str << std::endl;
                        fmt_status = 1; //������ʽ
                        fmt_begin = n;
                        ++n;
                        continue;
                    }
                }
                else if (fmt_status == 1) {
                    if (m_pattern[n] == '}') {
                        fmt = m_pattern.substr(fmt_begin + 1, n - fmt_begin - 1);
                        //std::cout << "#" << fmt << std::endl;
                        fmt_status = 0;
                        ++n;
                        break;
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
                    vec.push_back(std::make_tuple(nstr, std::string(), 0));
                    nstr.clear();
                }
                vec.push_back(std::make_tuple(str, fmt, 1));
                i = n - 1;
            }
            else if (fmt_status == 1) {
                std::cout << "pattern parse error: " << m_pattern << " - " << m_pattern.substr(i) << std::endl;
                m_error = true;
                vec.push_back(std::make_tuple("<<pattern_error>>", fmt, 0));
            }
        }

        if (!nstr.empty()) {
            vec.push_back(std::make_tuple(nstr, "", 0));
        }
        static std::map<std::string, std::function<FormatterItem::ptr(const std::string& str)> > s_format_items = {
#define XX(str, C) \
        {#str, [](const std::string& fmt) { return FormatterItem::ptr(new C(fmt));}}

                XX(m, MessageFormatItem),           //m:��Ϣ
                XX(p, LevelFormatItem),             //p:��־����
                XX(r, ElapseFormatItem),            //r:�ۼƺ�����
                XX(c, NameFormatItem),              //c:��־����
                XX(t, ThreadIdFormatItem),          //t:�߳�id
                XX(n, NewLineFormatItem),           //n:����
                XX(d, DateTimeFormatItem),          //d:ʱ��
                XX(f, FilenameFormatItem),          //f:�ļ���
                XX(l, LineFormatItem),              //l:�к�
                XX(T, TabFormatItem),               //T:Tab
                XX(F, FiberIdFormatItem),           //F:Э��id
                XX(N, ThreadNameFormatItem),        //N:�߳�����
#undef XX
        };

        for (auto& i : vec) {
            if (std::get<2>(i) == 0) {
                m_items.push_back(FormatterItem::ptr(new StringFormatItem(std::get<0>(i))));
            }
            else {
                auto it = s_format_items.find(std::get<0>(i));
                if (it == s_format_items.end()) {
                    m_items.push_back(FormatterItem::ptr(new StringFormatItem("<<error_format %" + std::get<0>(i) + ">>")));
                    m_error = true;
                }
                else {
                    m_items.push_back(it->second(std::get<1>(i)));
                }
            }

            //std::cout << "(" << std::get<0>(i) << ") - (" << std::get<1>(i) << ") - (" << std::get<2>(i) << ")" << std::endl;
        }
        //std::cout << m_items.size() << std::endl;
    }

}