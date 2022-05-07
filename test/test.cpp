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

su::slist<int> prepare_for_list(std::initializer_list<int> ls){
    su::slist<int> result;
    for(auto i : ls){
        result.add_after_head(i);
    }
    return result;
}

int main(){
    su::slist<int> l1,l2;
    for(int i = 1;i<=10;i++) l2.add_after_head(i);
    l2.add_after_head(18);
    l2.add_after_head(20);
    l2.add_after_head(30);

    l1 = prepare_for_list({1, 3, 6, 10, 15, 16, 17, 18, 19, 20});

    l1.dispaly();
    l2.dispaly();


}