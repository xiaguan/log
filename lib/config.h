//
// Created by 75108 on 2022/3/23.
//

#pragma once

#include <memory>
#include <string>
#include <map>
#include <util/lexical_cast.h>
#include <log.h>


namespace su{
class ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVarBase> ptr;
    ConfigVarBase(std::string name,std::string description = ""):
    m_name(std::move(name)),m_description(std::move(description)){

    }

    virtual ~ConfigVarBase(){}

    virtual std::string toString() = 0;
    virtual bool fromString(std::string val) = 0;

protected:
    std::string m_name;
    std::string m_description;
};

template <typename T>
class ConfigVar:public ConfigVarBase{
public:
    typedef std::shared_ptr<ConfigVar> ptr;
    ConfigVar(std::string name,const T & new_val,std::string description =""):
            ConfigVarBase(name,description),m_val(new_val){

    }
    /*
     *  这里的try catch有很大的问题，先这样写，后面再改，
     *  因为lexical_cast 是我写的，并没有抛出异常，只是简单封装了简单的几个
     */

    std::string toString() override{
        try{
            return lexical_cast<std::string>(m_val);
        }catch (std::exception & e){
            SU_LOG_ERROR(SU_LOG_ROOT)<<"lexical_cast error"<<e.what()<<" "<<typeid(T).name();
        }
        return "";
    }

    bool fromString(std::string val) override{
           try{
               lexical_cast<T,std::string>(val);
               return true;
           }catch(std::exception& e){
               SU_LOG_ERROR(SU_LOG_ROOT)<<"lexical_cast error"<<e.what()<<" "<<typeid(T).name();
           }
           return false;
    }

private:
    T m_val;
};

class Config{
    typedef std::map<std::string,ConfigVarBase::ptr> ConfigMap;
};


}
