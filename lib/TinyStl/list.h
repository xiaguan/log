/*
 * sougou workflow list 仿写
 */
#include <log.h>

#ifndef _SU_STL_LIST
#define _SU_STL_LIST

/*
 * doubly linked list implementation
 * 有一些函数是很有用的，当操作整个list,而不是当个元素，
 * 当有时候我们呢已经知道下一个或者是给你一个元素
 */
struct list_head{
    struct list_head* next,*prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name)}

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

static inline void INIT_LIST_HEAD(struct list_head * list){
    list->next = list;
    list->prev = list;
}

static inline void __list_add(struct list_head* node,
                              struct list_head* next,
                              struct list_head* prev)
{
    next->prev = node;
    node->next = next;
    prev->next = node;
    next->prev = prev;
}

static inline void list_add(struct list_head* node,struct list_head* head)
{
    __list_add(node,head,head->next);
}

static inline void list_add_tail(struct list_head* node, struct list_head* head)
{
    __list_add(node,head->prev,head);
}




static inline void __list_del(struct list_head* prev,struct list_head* next)
{
    next->prev = prev;
    prev->next = next;
}

static inline void list_del(struct list_head* entry)
{
    __list_del(entry->prev,entry->next);
}

static inline void list_move(struct list_head * list,struct list_head* head)
{
    __list_del(list->prev,list->next);
    list_add(list,head);
}

static inline void list_move_tail(struct list_head* list,struct list_head* head)
{
    __list_del(list->prev,list->next);
    list_add_tail(list,head);
}

static inline int list_empty(const struct list_head* head)
{
    return head->next == head;
}

static inline void __list_splice(struct list_head *list,
                                 struct list_head *head)
{
    struct list_head *first = list->next;
    struct list_head *last = list->prev;
    struct list_head *at = head->next;

    first->prev = head;
    head->next = first;

    last->next = at;
    at->prev = last;
}

static inline void list_splice(struct list_head *list, struct list_head *head)
{
    if (!list_empty(list))
        __list_splice(list, head);
}

static inline void list_splice_init(struct list_head *list,
                                    struct list_head *head)
{
    if (!list_empty(list)) {
        __list_splice(list, head);
        INIT_LIST_HEAD(list);
    }
}

#define list_entry(ptr,type,member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define list_for_each(pos,head) \
     for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

#define list_for_each_prev(pos,head)                                   \
     for((pos) = (head) ->prev); (pos) != (head); (pos) = (pos)->prev

#define lists_for_each_safe(pos,n,head) \
    for((pos) = (head)->next,(n) = (pos)->next; (pos) != (head); \
                (pos) = (n),(n) = (pos)->next)
#define list_for_each_entry(pos,head,member) \
                for( (pos) = list_entry((head)->next,typeof (*(pos)),member); \
                    &(pos)->member != (head); \
                    (pos) = list_entry((pos)->member.next, typeof(*(pos)),member))


struct slist_node {
    struct slist_node *next;
};

struct slist_head {
    struct slist_node first, *last;
};

#define SLIST_HEAD_INIT(name)	{ { (struct slist_node *)0 }, &(name).first }

#define SLIST_HEAD(name) \
	struct slist_head name = SLIST_HEAD_INIT(name)

static inline void INIT_SLIST_HEAD(struct slist_head *list)
{
    list->first.next = (struct slist_node *) nullptr;
    list->last = &list->first;
}

static inline void slist_add_head(struct slist_node *node,
                                  struct slist_head *list)
{
    node->next = list->first.next;
    list->first.next = node;
    if (!node->next)
        list->last = node;
}

static inline void slist_add_tail(struct slist_node *node,
                                  struct slist_head *list)
{
    node->next = (struct slist_node *)0;
    list->last->next = node;
    list->last = node;
}

static inline void slist_add_after(struct slist_node *node,
                                   struct slist_node *prev,
                                   struct slist_head *list)
{
    node->next = prev->next;
    prev->next = node;
    if (!node->next)
        list->last = node;
}

static inline void slist_del_head(struct slist_head *list)
{
    list->first.next = list->first.next->next;
    if (!list->first.next)
        list->last = &list->first;
}

static inline void slist_del_after(struct slist_node *prev,
                                   struct slist_head *list)
{
    prev->next = prev->next->next;
    if (!prev->next)
        list->last = prev;
}

static inline int slist_empty(struct slist_head *list)
{
    return !list->first.next;
}

static inline void __slist_splice(struct slist_head *list,
                                  struct slist_node *at,
                                  struct slist_head *head)
{
    list->last->next = at->next;
    at->next = list->first.next;
    if (!list->last->next)
        head->last = list->last;
}

static inline void slist_splice(struct slist_head *list,
                                struct slist_node *at,
                                struct slist_head *head)
{
    if (!slist_empty(list))
        __slist_splice(list, at, head);
}

static inline void slist_splice_init(struct slist_head *list,
                                     struct slist_node *at,
                                     struct slist_head *head)
{
    if (!slist_empty(list)) {
        __slist_splice(list, at, head);
        INIT_SLIST_HEAD(list);
    }
}

#define slist_entry(ptr, type, member) \
	((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define slist_for_each(pos, head) \
	for ((pos) = (head)->first.next; pos; (pos) = (pos)->next)

#define slist_for_each_safe(pos, prev, head) \
	for ((prev) = &(head)->first, (pos) = (prev)->next; pos; \
		 (prev) = (prev)->next == (pos) ? (pos) : (prev), (pos) = (prev)->next)

#define slist_for_each_entry(pos, head, member) \
	for ((pos) = slist_entry((head)->first.next, typeof (*(pos)), member); \
		 &(pos)->member != (struct slist_node *)0; \
		 (pos) = slist_entry((pos)->member.next, typeof (*(pos)), member)

namespace su {
#define m_entry(ptr, type)\
    ((type *)((char *)(type *)((ptr)) - (unsigned long long)(&((type *)nullptr)->node)))

    template<typename T>
    struct __node {
        struct slist_node *node;
        T m_val;
    };

    template<typename T>
    class slist {
    public:
        slist() : head(new slist_head), m_sz(0) {
            INIT_SLIST_HEAD(head);
        }

        void add_after_head(const T &t) {
            auto *newNode = new (__node<T>);
            newNode->node = new slist_node;
            newNode->m_val = t;
            //SU_LOG_INFO(SU_LOG_ROOT()) << " address is "<<&(newNode->node)<<" "<<t;
            slist_add_head((struct slist_node *) (&newNode->node), head);
            //SU_LOG_INFO(SU_LOG_ROOT()) <<"head->next " << (head->first.next);
            m_sz++;
        }

        __node<T> *getPos(size_t pos) {
            int n = 0;
            struct slist_node *p;
            slist_for_each(p, head) {
                if (n == pos) {
                    //SU_LOG_INFO(SU_LOG_ROOT())<< m_entry(p,__node<int>);// <<" "<<m_entry(ps,__node<int>)->m_val ;
                    return m_entry(p, __node<int>);
                }
                n++;
            }
            SU_LOG_ERROR(SU_LOG_ROOT()) << "invalid pos " << pos << " list size = " << m_sz;
            return nullptr;
        }

        void insert(size_t pos, const T &new_val) {
            if (pos == 0) {
                SU_LOG_ERROR(SU_LOG_ROOT()) << " slist::insert() : pos invalid";
                return;
            }
            auto pre = getPos(pos - 1);
            //std::cout <<pre->m_val << "  "<<(pre->node==nullptr) << " " <<" "<<std::endl;
            if (pre == nullptr) { ;
                return;
            }
            auto *newNode = new (__node<T>);
            newNode->node = new slist_node;
            newNode->m_val = new_val;
            slist_add_after((struct slist_node *) (&newNode->node), (struct slist_node *) (&pre->node), head);
            m_sz++;
        }

        void del(size_t pos) {
            auto pre = getPos(pos - 1);
            slist_del_after(pre, head);
        }

        void test() {
            auto pos = new slist_node;
            slist_for_each(pos, head) {
                SU_LOG_INFO(SU_LOG_ROOT()) << m_entry(pos, __node<int>)->m_val;
            }
        }

        void dispaly() {
            for (size_t i = 0; i < m_sz; i++) {
                std::cout << getPos(i)->m_val << " ";
            }
            std::cout << std::endl;
        }

        size_t size() const { return m_sz; }

    private:
        slist_head *head;
        size_t m_sz;
    };


    template<typename T>
    struct normal_node {
        normal_node *next;
        T val;
    };

    template<typename T>
    class normal_list {
    public:
        typedef normal_node<T> node;

        normal_list() : head(new node) {
            head->next = nullptr;
        }

        void push_back(const T &v) {
            node *newNode = new node;
            newNode->val = v;
            newNode->next = head->next;
            head->next = newNode;
        }

    private:
        node *head;
    };
}
#endif
