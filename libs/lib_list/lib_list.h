#ifndef __lib_LIST_H__
#define __lib_LIST_H__

// 双向链表的实现

//链表指针
struct list_head {
    struct list_head *next, *prev;
};

//初始化一个名为name的双向链表头结构
#define LIST_HEAD_INIT(name) {&(name), &(name)}

//声明并初始化一个名为name的双向链表头结构
#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)
    
//对指针ptr所指向的双向链表进行初始化
#define INIT_LIST_HEAD(ptr) do { \
    (ptr)->next = (ptr); (ptr)->prev = (ptr); \
} while (0)

/*
    inline 函数是一种用于优化程序性能的方法。
    当函数被声明为 inline 时，编译器会尝试在调用该函数的地方直接插入函数的代码，而不是传统的函数调用方式。
    这样做可以减少函数调用的开销，因为它避免了传统函数调用时的堆栈操作和跳转指令，从而提高程序的执行效率
*/

//1. 链表增加(头/尾)
//2. 链表删除(头/尾)
//3. 返回数据(头/尾)
//4. 链表数量
//5. 链表是否为空
//6. 链表排序
//7. 链表遍历

#endif