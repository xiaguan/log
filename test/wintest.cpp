#include <TinyStl/list.h>
#include <algorithm>
using namespace su;
using namespace std;

su::slist<int> prepare_for_list(std::initializer_list<int> ls){
    su::slist<int> result;
    vector<int> vec(ls.begin(),ls.end());
    reverse(vec.begin(),vec.end());
    for(auto i : vec) result.add_after_head(i);
    return result;
}

void test2(){
    slist<int> m_list;
    vector<int> idxs={5};
    for(auto idx :idxs){
        m_list.dispaly();
        cout <<"insert idx "<<idx<<" :";
        m_list.insert(idx,500);
        m_list.dispaly();
       if(idx) m_list.del(idx);
    }
}

int main(){
    test2();
}