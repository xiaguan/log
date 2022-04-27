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

void test3(){
    slist<int> m_list;
    //for(int i = 0;i<10;i++) m_list.add_after_head(i);
    vector<int> del_idx{5};
    for(auto idx : del_idx){
        cout << "before del : ";m_list.dispaly();
        cout <<  "after del pos "<<idx<<" : ";
        m_list.del(idx);
        m_list.dispaly();

        m_list.insert(idx,0);
        cout << endl;
    }

}

void test4(){
    slist<int> m_list= prepare_for_list({0,20,30,40,50,60,70,80,90,100});
    vector<int> values{25,85,110,8};
    m_list.dispaly();
    auto head = m_list.getHead();
    for(auto value : values){
        auto p = new struct slist_node;
        auto temp = p;
        slist_for_each(p,head){
            if(m_entry(p,__node<int>)->m_val > value){
                break;
            }
            temp = p;
        }
        auto newNode = new __node<int>;
        newNode->m_val = value;
        slist_add_after(&newNode->node,temp,head);
    }
    m_list.dispaly();
}


void test5(){
    slist<int> l1 = prepare_for_list({10,20,30,40,50,60,70,80,90,10});
    slist<int> l2,l3;
    cout <<"before : ";l1.dispaly();
    auto head = l1.getHead();
    struct slist_node * p;
    slist_for_each(p,head){
        int val = m_entry(p,__node<int>)->m_val;
        if(val % 2 ==0) l2.add_after_head(val);
        else l3.add_after_head(val);
    }
    cout << " oushu : ";l2.dispaly();
    cout << " jishu : ";l3.dispaly();
}

void test6(){
    slist<int> l1 = prepare_for_list({});
    slist<int> l2 = prepare_for_list({1, 2, 3, 4, 5, 6, 7, 8, 9, 10});
    slist<int> l3;
    l1.dispaly();
    l2.dispaly();
    auto h1 = l1.getHead();
    auto h2 = l2.getHead();
    auto p1 = h1->first.next;
    auto p2 =  h2->first.next;
    while(p1 && p2){
        int a = m_entry(p1,__node<int>)->m_val;
        int b = m_entry(p2,__node<int>)->m_val;
        if(a == b){
            auto newNode = new __node<int>;
            newNode->m_val = a;
            slist_add_tail(&newNode->node,l3.getHead());
            p1 = p1->next;
            p2 = p2->next;
        }
        else if(a < b){
            p1 = p1->next;
        }else{
            p2 = p2->next;
        }
    }
    l3.dispaly();
}

int main(){
    test6();
}