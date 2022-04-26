//
// Created by 75108 on 2022/3/23.
//

#ifndef Su_Lib_CONFIG
#define Su_Lib_CONFIG


#include <memory>
#include <string>
#include <map>
#include <yaml-cpp/yaml.h>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <functional>

#include "log.h"
#include "util/lexical_cast.h"
#include "util/util.h"

namespace su {
    /*
     *  std::string m_name 字段名称
     *  std::string m_description 字段描述
     *  设置为基类的原因是，字段里所存放的值的类型是多样化的，交给子类处理
     */

    class ConfigVarBase {
    public:
        typedef std::shared_ptr<ConfigVarBase> ptr;

        explicit ConfigVarBase(std::string name, std::string description = " ") :
                m_name(std::move(name)), m_description(std::move(description)) {

        }

        virtual ~ConfigVarBase() = default;

        /*
         * 基类提供的方法：
         * get获取字段，描述
         * toString and fromString both use the lexical_cast
         */

        const std::string &getName() const { return m_name; }

        const std::string &getDescription() const { return m_description; }

        virtual std::string toString() = 0;

        virtual bool fromString(const std::string &val) = 0;

        virtual std::string getTypeName() const = 0;
    protected:
        std::string m_name;
        std::string m_description;
    };

    template<typename From, typename To>
    class LexicalCast {
    public:
        To operator()(const From &from) {
            return lexical_cast<From, To>()(from);
        }
    };


// string -----> vector<T>
    template<class T>
    class LexicalCast<std::string, std::vector<T> > {
    public:
        std::vector<T> operator()(const std::string &s) {
            YAML::Node node = YAML::Load(s);
            typename std::vector<T> vec;
            std::stringstream ss;
            for (size_t i = 0; i < node.size(); i++) {
                ss.str("");
                ss << node[i];
                vec.push_back(lexical_cast<std::string, T>()(ss.str()));
            }
            return vec;
        }
    };

// vector<T> ----> std::string
    template<typename T>
    class LexicalCast<std::vector<T>, std::string> {
    public:
        std::string operator()(const std::vector<T> &vec) {
            YAML::Node node(YAML::NodeType::Sequence);
            for (auto &i: vec) {
                node.push_back(YAML::Load(lexical_cast<T, std::string>()(i)));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

// string ----> list<T>

    template<typename T>
    class LexicalCast<std::string, std::list<T> > {
    public:
        std::list<T> operator()(const std::string &str) {
            YAML::Node node(YAML::Load(str));
            typename std::list<T> vec;
            std::stringstream ss;
            for (int i = 0; i < node.size(); i++) {
                ss.clear();
                ss << node[i];
                vec.push_back(lexical_cast<std::string, T>()(ss.str()));
            }
            return vec;
        }
    };


// list<T> -----> string
    template<typename T>
    class LexicalCast<std::list<T>, std::string> {
    public:
        std::string operator()(const std::list<T> &list) {
            YAML::Node node(YAML::NodeType::Sequence);
            for (auto &i: list) {
                node.push_back(lexical_cast<T, std::string>()(i));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

// string ----> set<T>
    template<typename T>
    class LexicalCast<std::string, std::set<T> > {
    public:
        std::set<T> operator()(const std::string &str) {
            YAML::Node node(YAML::Load(str));
            typename std::set<T> vec;
            std::stringstream ss;
            for (int i = 0; i < node.size(); i++) {
                ss.clear();
                ss << node[i];
                vec.insert(lexical_cast<std::string, T>()(ss.str()));
            }
            return vec;
        }
    };


// set<T> ----->string
    template<typename T>
    class LexicalCast<std::set<T>, std::string> {
    public:
        std::string operator()(const std::set<T> &s) {
            YAML::Node node(YAML::NodeType::Sequence);
            for (auto &i: s) {
                node.push_back(lexical_cast<T, std::string>()(i));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

/*
 * ???
 */
// string   ---> unordered_set<T>
    template<typename T>
    class LexicalCast<std::string, std::unordered_set<T>> {
    public:
        std::unordered_set<T> operator()(const std::string &s) {
            YAML::Node node(YAML::Load(s));
            typename std::unordered_set<T> vec;
            std::stringstream ss;
            for (size_t i = 0; i < node.size(); ++i) {
                ss.clear();
                ss << node[i];
                vec.insert(lexical_cast<std::string, T>()(ss.str()));
            }
            return vec;
        }
    };

    // unordered_set<T>  ----> string
    template<typename T>
    class LexicalCast<std::unordered_set<T>, std::string> {
    public:
    public:
        std::string operator()(const std::unordered_set<T> &vec) {
            YAML::Node node(YAML::NodeType::Sequence);
            for (auto &i: vec) {
                node.push_back(lexical_cast<T, std::string>()(i));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };


    // std::string ----->std::map<std::string,T>
    template<typename T>
    class LexicalCast<std::string, std::map<std::string, T> > {
    public:
        std::map<std::string, T> operator()(const std::string &s) {
            YAML::Node node(YAML::Load(s));
            typename std::map<std::string, T> vec;
            std::stringstream ss;
            for (auto it = node.begin();
                 it != node.end(); ++it) {
                ss.clear();
                ss << it->second;
                vec.insert(std::pair{it->first.Scalar(), lexical_cast<std::string, T>()(ss.str())});
            }
            return vec;
        }
    };

    // std::map<std::string,T>  ----> std::string
    template<typename T>
    class LexicalCast<std::map<std::string, T>, std::string> {
    public:
        std::string operator()(const std::map<std::string, T> &vec) {
            YAML::Node node(YAML::NodeType::Map);
            for (auto &i: vec) {
                node[i.first] = YAML::Load(lexical_cast<T, std::string>()(i.second));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };

    // std::string ----->std::unordered_map<std::string,T>
    template<typename T>
    class LexicalCast<std::string, std::unordered_map<std::string, T> > {
    public:
        std::unordered_map<std::string, T> operator()(const std::string &s) {
            YAML::Node node(YAML::Load(s));
            typename std::unordered_map<std::string, T> vec;
            std::stringstream ss;
            for (auto it = node.begin();
                 it != node.end(); ++it) {
                ss.clear();
                ss << it->second;
                vec.insert(std::pair{it->first.Scalar(), lexical_cast<std::string, T>()(ss.str())});
            }
            return vec;
        }
    };

    // std::unordered_map<std::string,T>  ----> std::string
    template<typename T>
    class LexicalCast<std::unordered_map<std::string, T>, std::string> {
    public:
        std::string operator()(const std::unordered_map<std::string, T> &vec) {
            YAML::Node node(YAML::NodeType::Map);
            for (auto &i: vec) {
                node[i.first] = YAML::Load(lexical_cast<T, std::string>()(i.second));
            }
            std::stringstream ss;
            ss << node;
            return ss.str();
        }
    };


    template<class T, class FromStr = LexicalCast<std::string, T>, class ToStr = LexicalCast<T, std::string>>
    class ConfigVar : public ConfigVarBase {
    public:
        typedef std::shared_ptr<ConfigVar> ptr;
        typedef std::function<void (const T& old_value, const T& new_vluae)> on_change_cb;

        ConfigVar(const std::string &name, const T &val,
                  const std::string &description = "") :
                ConfigVarBase(name, description), m_val(val) {
        }
·
        std::string toString() override {
            try {
                return ToStr()(m_val);
            } catch (std::exception &e) {
                SU_LOG_ERROR(SU_LOG_ROOT()) << "ConfigVar::toString exception"
                << e.what() << " convert: "
                << TypeToName<T>()<< " to string ";
            }
            return "";
        }

        bool fromString(const std::string &val) override {
            try {
                m_val = FromStr()(val);
                return true;
            } catch (std::exception &e) {
                SU_LOG_ERROR(SU_LOG_ROOT()) << "ConfigVar::fromString exception " << e.what() <<
                " convert : " << val<< " to " << TypeToName<T>();

            }
            return false;
        }

        T getValue() const { return m_val; }

        void setValue(const T &val) {
            if(val == m_val){
                return ;
            }
            for(auto& cb : m_cbs){
                cb->second(m_val,val);
            }
            m_val = val;
        }

        uint64_t addListener(on_change_cb cb){
            static uint64_t s_fun_id = 0;
            ++s_fun_id;
            m_cbs[s_fun_id] = cb;
            return s_fun_id;
        }

        void delListener(uint64_t key){
            m_cbs.erase(key);
        }

        on_change_cb getListener(uint64_t key){
            auto it = m_cbs.find(key);
            return it == m_cbs.end() ? nullptr : it->second;
        }

        void clearListener() {
            m_cbs.clear();
        }

        std::string getTypeName() const override{ return TypeToName<T>();}

    private:
        T m_val;
        std::map<uint64_t, on_change_cb> m_cbs;
    };

    class Config {
    public:
        typedef std::map<std::string, ConfigVarBase::ptr> ConfigVarMap;

        template<class T>
        static typename ConfigVar<T>::ptr
        Lookup(const std::string &name, const T &default_value, const std::string &description = " ") {
            auto it = GetDates().find(name);
            if( it != GetDates().end()){
                auto tmp = std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
                if(tmp){
                    SU_LOG_INFO(SU_LOG_ROOT()) <<"Look up name = "<<name<<" exists";
                    return tmp;
                }else{
                    SU_LOG_DEBUG(SU_LOG_ROOT()) <<"Look up name = "<<name <<" exists ,find type =  "
                    << TypeToName<T>() <<" ,real_type = "<<it->second->getTypeName();
                    return nullptr;
                }
            }
            if (name.find_first_not_of("abcdefghijklmnopqrstuvwxyz._0123456789") != std::string::npos) {
                SU_LOG_ERROR(SU_LOG_ROOT()) << "Lookup name invalid " << name;
                /*
                 * invalid类的继承关系：exception->logic_error->invalid_argument
                 * 这里指无效参数，也就是name里不包含有效的字符
                 * std::string::npos == size_type的最大值，用来表示一个不存在的位置
                 */
                throw std::invalid_argument(name);
            }
            typename ConfigVar<T>::ptr v(new ConfigVar<T>(name, default_value, description));
            GetDates()[name] = v;
            return v;
        }

        template<class T>
        static typename ConfigVar<T>::ptr Lookup(const std::string &name) {
            auto it = GetDates().find(name);
            if (it == GetDates().end()) {
                return nullptr;
            }
            return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
        }

        static void LoadFromYAMLNode(const YAML::Node &root);

        static ConfigVarBase::ptr LookupBase(const std::string &name);

    private:
        static ConfigVarMap &GetDates() {
            static ConfigVarMap s_date;
            return s_date;
        }
    };
}

#endif
