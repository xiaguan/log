//
// Created by 75108 on 2022/3/23.
//

#ifndef Su_Lib_CONFIG
#define Su_Lib_CONFIG


#include <memory>
#include <string>
#include <map>
#include <util/lexical_cast.h>
#include <log.h>

namespace su{
    /*
     *  std::string m_name 字段名称
     *  std::string m_description 字段描述
     *  设置为基类的原因是，字段里所存放的值的类型是多样化的，交给子类处理
     */
class ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(std::string name,std::string description = " "):
    m_name(std::move(name)),m_description(std::move(description)){}

    virtual ~ConfigVarBase()= default;

    /*
     * 基类提供的方法：
     * get获取字段，描述
     * 和string的交互
     */

    const std::string & getName() const { return m_name;}
    const std::string & getDescription() const {return m_description;}

    virtual std::string toString() = 0;
    virtual bool fromString(const std::string& val) = 0;
protected:
    std::string m_name;
    std::string m_description;
};

template <class T>
class ConfigVar: public ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVar> ptr;

    ConfigVar(const std::string & name,const T& val,const std::string & description = ""):
            ConfigVarBase(name,description),m_val(val){
    }

    std::string toString() override{
        try{
            return std::to_string(m_val);
        }catch(std::exception &e){
            SU_LOG_ERROR(SU_LOG_ROOT()) << "ConfigVar::toString exception"<<e.what()<<" convert: "<<typeid(m_val).name()
            <<" to string ";
        }
        return "";
    }

    bool fromString(const std::string & val) override{
        try{
            m_val = lexical_cast<T>(val);
            return true;
        }catch(std::exception & e){
            SU_LOG_ERROR(SU_LOG_ROOT()) <<"ConfigVar::fromString exception"<<e.what()<<" convert : "<<val<<" to "<< typeid(m_val).name();
            
        }
        return false;
    }

    T getValue() const { return m_val;}
    void setValue(const T& val){m_val = val;}
private:
    T m_val;
};

class Config{
public:
    typedef std::map<std::string,ConfigVarBase::ptr> ConfigVarMap;

    template <class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string&name,const T& default_value
            ,const std::string & description = " "){
              auto tmp = Lookup<T>(name);
              if(tmp){
                  SU_LOG_INFO(SU_LOG_ROOT()) << "Lookup name = " << name
                  << "exists";
                  return tmp;
              }
              if(name.find_first_not_of("abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ._012345678") != std::string::npos){
                  SU_LOG_ERROR(SU_LOG_ROOT()) <<"Lookup name invalid "<< name;
                  /*
                   * invalid类的继承关系：exception->logic_error->invalid_argument
                   * 这里指无效参数，也就是name里不包含有效的字符
                   * std::string::npos == size_type的最大值，用来表示一个不存在的位置
                   */
                  throw std::invalid_argument(name);
              }
              typename ConfigVar<T>::ptr v(new ConfigVar<T>(name,default_value,description));
              s_dates[name] = v;
              return v;
    }

    template <class T>
    static typename ConfigVar<T>::ptr Lookup(const std::string& name){
        auto it = s_dates.find(name);
        if(it == s_dates.end()){
            return nullptr;
        }
        return std::dynamic_pointer_cast<ConfigVar<T> >(it->second);
    }

private:
    static ConfigVarMap s_dates;
};
}

#endif
