//
// Created by 75108 on 2022/4/14.
//

#include <config.h>

namespace su{
    ConfigVarBase::ptr Config::LookupBase(const std::string & name){
        auto it = GetDates().find(name);
        return it == GetDates().end() ? nullptr : it->second;
    }

    static void ListAllMember(const std::string & prefix,
    const YAML::Node& node,std::list<std::pair<std::string,const YAML::Node> > & output){
        if(prefix.find_first_not_of("abcdefghijklmnopqrstuvwxyz._012345678")
        != std::string::npos){
            SU_LOG_ERROR(SU_LOG_ROOT()) <<"LoadAllNodes(): invalid error "<<prefix<<" node : "<<node;
            return ;
        }
        output.push_back(std::make_pair(prefix,node));
        if(node.IsMap()){
            for(auto it = node.begin();
                it != node.end();++it){
                   ListAllMember(prefix.empty() ? it->first.Scalar() 
                   : prefix + "." + it->first.Scalar(),it->second,output); 
                }
        }
    }

     void Config::LoadFromYAMLNode(const YAML::Node & root){
        std::list<std::pair<std::string,const YAML::Node> > all_nodes;
        ListAllMember("",root,all_nodes);
        for(auto i : all_nodes){
            std::string key = i.first;
            if(key.empty()) continue;
            ConfigVarBase::ptr var = LookupBase(key);
            if(var){
                if(i.second.IsScalar()){
                    var->fromString(i.second.Scalar());
                } else {
                    std::stringstream ss;
                    ss << i.second;
                    var->fromString(ss.str());
                }
            }
        }
    }
}