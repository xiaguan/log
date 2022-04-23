#include <log.h>
#include <TinyStl/list.h>
#include <list>

using namespace std;

slist_head * head;

struct B{
    int val;
};

struct A{
    int val;
    B b;
};

int main(){

    su::__node<int>* n = new su::__node<int>;
    n->node = new slist_node;
    n->m_val = 100;

    cout << &(n->node) <<endl;
    cout << &(n) << endl;
    cout << (unsigned long long )(&((su::__node<int>*)nullptr)->m_val) <<endl;
    cout << sizeof (unsigned long long) << " "<<sizeof(&((su::__node<int>*)nullptr)->m_val) << endl;
    cout << ((su::__node<int>*)((char *)(su::__node<int>*)(&n->node) - (unsigned long long)(&((su::__node<int>*)nullptr)->node))) << endl;

    su::Timer timer;
    std::list<int> list;
    timer.start();
    for(int j = 0;j<10;j++)
        for(int i = 0;i<1e6;i++) list.push_back(i);
    timer.end();

    su::slist<int> m_list;
    timer.start();
    for(int j = 0;j<10;j++)
        for(int i = 0;i<1e6;i++) m_list.add_after_head(i);
    timer.end();

    su::normal_list<int> n_list;
    timer.start();
    for(int j = 0;j<10;j++)
        for(int i = 0;i<1e6;i++) n_list.push_back(i);
    timer.end();
}