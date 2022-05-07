//
// Created by 75108 on 2022/4/25.
//

#include "Zipper.h"
#include <thread>
#include <cstring>
#include <thread>
#include <mutex>
#include <future>
#include <algorithm>

long long dp[800];

auto logger = SU_LOG_ROOT();

void process_buf(char * buf,long long sz){
    for(long long  i = 0;i <sz ;i++){
        dp[buf[i]+300]++;
    }
}
char * prepare_buf(long long BUFFER_SIZE){
    return new char[BUFFER_SIZE];
}



namespace detail{



    unsigned long long HuffMan::construct(std::istream &in) {
        unsigned long long result = 0;
        using namespace std;
        const long long BUFFER_SIZE = 20*1024*1024;  // 1mb
        memset(dp,0,sizeof(dp));
        char* buf  = new char[BUFFER_SIZE];
        long long true_size= 0 ;
        in >> noskipws;
        std::vector<thread> threads;
        SU_LOG_INFO(logger) <<"async I/O with threads";
        do{
            future<char * > new_buf = async(prepare_buf,BUFFER_SIZE);
            true_size = in.readsome(buf,BUFFER_SIZE);
            result += true_size;
            threads.emplace_back(process_buf,buf,true_size);
            buf = new_buf.get();
        }while(true_size == BUFFER_SIZE);
        delete [] buf;
        for(auto & t :threads) t.join();

        std::map<char,int> vals;
        for(int i = 0;i<800;i++) if(dp[i] != 0) vals[(char)(i-300)] = dp[i];

        std::vector<Node::ptr> nodes;
        for(auto & val : vals){
            Node::ptr newNode(new Node());
            newNode->val = val.second;
            newNode->c = val.first;
            newNode->is_value = true;
            newNode->left = newNode->right = nullptr;
            nodes.push_back(newNode);
        }
        makeTree(nodes);
        std::cout <<"decode table sz "<<decode.size() <<std::endl;
        return result;
    }

    void HuffMan::dfs(const Node::ptr& node,const std::string&  prefix){
        if(node == nullptr) return;
        if(node->is_value){
            encode[node->c] = prefix;
            //std::cout <<node->c <<" "<<encode[node->c]<<std::endl;
            decode[prefix] = node->c;
        }
        dfs(node->left,prefix+"0");
        dfs(node->right,prefix+"1");
    }
    void HuffMan::makeTree(const std::vector<Node::ptr> &nodes) {
        SU_LOG_INFO(logger) <<"start to makeTree "<<nodes.size();
        using namespace std;
        priority_queue<pair<int,Node::ptr>,vector<pair<int,Node::ptr>>,greater<>> q;

        for(const auto& node : nodes){
            q.push({node->val,node});
        }

        if(q.size() == 1){
            encode[nodes[0]->c] ="0";
            return;
        }

        while(q.size() > 1){
            auto t1 = q.top();q.pop();
            auto t2 = q.top();q.pop();

            Node::ptr newNode(new Node());
            newNode->val = t1.second->val + t2.second->val;
            newNode->left = t1.second;
            newNode->right = t2.second;

            q.push({newNode->val,newNode});
        }
        root = q.top().second;
        SU_LOG_DEBUG(logger) << "start dfs "<<endl;
        dfs(root,"");
    }


    void formatMap(std::map<char,std::string> & map,std::ostream & out){
        std::cout << "construct size : "<<map.size()<<std::endl;
        unsigned long map_sz = map.size();
        out.write((char*)&map_sz,4);
        char map_buf[2000];

        memset(map_buf,0,map.size()+map.size() * 4);
        int idx = -1;
        int cnt = 0;
        for(auto it = map.begin();it != map.end();it++){
            map_buf[++idx] = (it->second.size());
            int sz = it->second.size();
            const std::string & s = it->second;
            char & c = map_buf[idx+1];
            for(int i = 7;i>=0 && sz;i--,sz--){
                if(s[7-i]=='1')c = ((1 << i) | c);
            }
            char & b = map_buf[idx+2];
            for(int j = 7;j>=0 && sz;j--,sz--){
                if(s[8+(7-j)]=='1')b = ((1 << j) | b);
            }
            idx+=2;
            map_buf[++idx] = it->first;
            SU_LOG_INFO(logger) <<it->first<<" "<<it->second<<" "<<print_ss_two(map_buf[idx-3])<<" "<<print_ss_two(map_buf[idx-2])
            <<print_ss_two(map_buf[idx-1])<<" "<<print_ss_two(map_buf[idx])<<" "<<print_ss_two(it->first);
            /*
            print_in_two(map_buf[idx-2]);print_in_two(map_buf[idx-1]);std::cout <<" ";
            std::cout <<"construct : "<<it->first<<" - "<<it->second<<std::endl;
            */
            /*
            for(int j = 0;j<4;j++) {
                print_in_two(map_buf[idx-3+j]);std::cout <<" ";
            }8?
            /*
            if(cnt < 3){
                for(int j = 0;j<4;j++){
                    print_in_two(map_buf[idx-3+j]);std::cout<<" ";
                }
                std::cout << std::endl;
                cnt++;
            }*/
        }
        std::cout <<"write "<<idx+1<<" bytes"<<std::endl;
       out.write(map_buf,idx+1);
       std::cout << std::endl;
    }

    std::map<std::string,char> getMap(std::istream & in){
        SU_LOG_INFO(logger) <<"decoder start getMap ";
        in >> std::noskipws;
        unsigned long sz = 0;
        SU_LOG_INFO(logger)<< in.readsome((char*)&sz,4);
        SU_LOG_INFO(logger) <<"decoder map size is "<<sz;
        char buf[3000];
        SU_LOG_INFO(logger)<<"should get "<<sz*4<<" but get "<<in.readsome(buf,sz*4);
        int cnt = 0;
        std::map<std::string,char> result;
        for(int i = 0;i<sz*4;i += 4){
            //std::cout <<" "<<i<<" "<<sz<<std::endl;
            /*
            for(int j = 0;j<4;j++) {
                print_in_two(buf[i+j]);
                std::cout <<" "<<j<<" ";
            }*/
            int map_sz = buf[i];
            char key = buf[i+3];
            std::string value;
            char c = buf[i+1];
            //print_in_two(c);print_in_two(buf[i+2]);std::cout <<" ";
            int j = 7;
            for(;j>=0 && map_sz;j--,map_sz--) value.push_back((c>> j) & 1 ? '1' :'0');
            c = buf[i+2];
	        j = 7;
            for(;j>=0 && map_sz;j--,map_sz--) value.push_back((c>> j) & 1 ? '1' :'0');
            SU_LOG_INFO(logger) << key<<"-"<<value<<" "<<print_ss_two(buf[i])<<" "<<print_ss_two(buf[i+1])<<print_ss_two(buf[i+2])<<" "<<print_ss_two(buf[i+3]);
            //result[value] = key;
            auto it = result.find(value);
            if(it != result.end()){
                SU_LOG_ERROR(logger) <<"Find the key is already exists "<<value<<" "<<it->first<<" find : "<<key;
            }else{
                result.insert({value,key});
            }
           //std::cout <<"decode : "<<key<<"-"<<value<<std::endl;
        }
        SU_LOG_INFO(logger) <<" get true map size is "<<result.size();
        return result;
    }

    std::mutex out_mut;
    void process_out_buf(char * buf,std::map<char,std::string>& map,std::ofstream & out,unsigned long sz){
        /*
         * 读入了一定量的数据，然后查找map，输出到out
         * 输出的时候要规定顺序
         */
        std::lock_guard<std::mutex> guard(out_mut);
        const long long BUFFER_SIZE = 20*1024*1024;  // 10mb
        char * out_buf = new char[BUFFER_SIZE];
        memset(out_buf,0,sz);
        unsigned long idx = 0 ;
        char buf_idx = 7;
        for(unsigned long i = 0;i<sz;i++){
                auto s = map[buf[i]];
                for(int j = 0;j<s.size();j++){
                    if(buf_idx == -1){
                        idx++;
                        buf_idx = 7;
                    }
                    if(s[j]== '1'){
                        buf[idx] = ((1 << buf_idx) | buf[idx]);
                    }
                    buf_idx--;
                }
        }
        delete [] out_buf;
    }

    void HuffMan::format(std::istream &in, std::ostream &out) {
        SU_LOG_INFO(logger) <<"encode start format ";
        SU_LOG_INFO(logger) <<" decode size is "<<decode.size();
        long long cnt = 0;
        using namespace std;
        const long long BUFFER_SIZE = 10*1024*1024;  // 1mb
        future<char * > new_buf = async(prepare_buf,BUFFER_SIZE);
        formatMap(encode,out);
        char* buf  = new_buf.get();
        char * write_buf = new char[BUFFER_SIZE];
        memset(write_buf,0,BUFFER_SIZE);
        unsigned long true_size= 0 ;
        unsigned long write_idx = 0;
        in >> noskipws;
        do{
            Timer timer("while loop");timer.start();
            Timer read_timer("read_timer");read_timer.start();
            new_buf = async(prepare_buf,BUFFER_SIZE);
            true_size = in.readsome(buf,BUFFER_SIZE);
            read_timer.end();
            SU_LOG_INFO(SU_LOG_ROOT()) << "format is processing .....";
            char buf_idx = 7;
            for(unsigned long i = 0;i<true_size;i++){
                auto s = encode[buf[i]];
                for(char j : s){
                    if(buf_idx == -1){
                        write_idx++;
                        if(write_idx == BUFFER_SIZE){
                                out.write(write_buf,BUFFER_SIZE);
                                cnt += BUFFER_SIZE;
                                memset(write_buf,0,BUFFER_SIZE);
                                write_idx = 0;
                            }
                        buf_idx = 7;
                    }
                    if(j== '1'){
                        write_buf[write_idx] = (1 << buf_idx) | write_buf[write_idx];
                    }
                    buf_idx--;
                }
            }
            delete [] buf;
            Timer get_timer("get timer");get_timer.start();
            buf = new_buf.get();
            get_timer.end();
            timer.end();
        }while(true_size);
        out.write(write_buf,write_idx+1);
        cnt += write_idx;      
        SU_LOG_INFO(logger) <<"encode format exit";
    }
}

void Zipper::encode(const std::string &filename) {
    unsigned long long file_size = 0;
    std::ifstream m_fs(filename,std::ios::binary);
    if(!m_fs.is_open()){
        SU_LOG_ERROR(logger) <<"encode open failed ";
        return ;
    }
    SU_LOG_INFO(logger) <<"encode start construct ";
    file_size = m_tree.construct(m_fs);
    SU_LOG_INFO(logger) <<"encode ended construct" ;
    m_fs.close();m_fs.close();
    m_fs.open(filename,std::ios::binary);
    std::ofstream out("_zip_"+filename);
    SU_LOG_INFO(logger) <<"encoder file size is"<<file_size;
    out.write((char*)&file_size,8);
    m_tree.format(m_fs,out);
}

void Zipper::decode(const std::string &filename) {
    SU_LOG_INFO(logger) <<"decode start ";
    std::ifstream in(filename,std::ios::binary);
    unsigned long long file_sz;
    in >> std::noskipws;
    in.readsome((char*)&file_sz,8);
    SU_LOG_INFO(logger) <<" decode file true size should be "<<file_sz;
    m_tree.decode = detail::getMap(in);
    std::vector<std::pair<std::string,char>> pairs;
    for(auto &m : m_tree.decode){
        pairs.push_back({m.first,m.second});
    }
    std::sort(pairs.begin(),pairs.end(),[](const std::pair<std::string,char> & l,const std::pair<std::string,char> &r){
        return l.first.size()<=r.first.size();
    });
    std::ofstream out("result"+std::string(filename.begin()+5,filename.end()),std::ios::binary);
    if(!out.is_open()){
        throw std::logic_error("decode file open error");
    }
    std::string s;
    const int BUFFER_SIZE = 1*1024*1024;
    char * buf  = new char[BUFFER_SIZE];
    unsigned long true_size = 0;
    do{
        std::future<char*> new_buf = std::async(prepare_buf,BUFFER_SIZE);
        true_size = in.readsome(buf,BUFFER_SIZE);
        std::cout <<"true size is "<<true_size<<" "<<in.bad()<<std::endl;
        for(int i = 0;i<true_size;i++){
            for(int j = 7;j>=0;j--){
                s.push_back((buf[i] >> j) & 1 ? '1':'0');
                auto it = m_tree.decode.find(s);
                if(it != m_tree.decode.end()){
                    out << it->second;
                    s.clear();
                    file_sz--;
                    if(file_sz == 0){
                        out.close();
                        return;
                    }
                }
            }
        }
        delete [] buf;
        buf = new_buf.get();
    }while(true_size);
    delete [] buf;
    std::cout << "left sz is "<<file_sz <<" s "<<s.size()<<std::endl;
    out << std::flush;
    out.close();
}
