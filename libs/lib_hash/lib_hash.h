#ifndef __LIB_HASH__
#define __LIB_HASH__

//用于自定义hash表释放函数
typedef void (*hash_data_free_func_t)(void *data);

//用于自定义hash表生成key函数(用于较复杂的key)
typedef unsigned int (*hash_key_func_t)(const void *key, int klen);

//hash table 节点
typedef struct ht_item_st
{
    int klen;       //key
    unsigned int __hval;    //???
    void *val;      //value
    struct ht_item *next;   //链表下一个节点
    char key[0];
}ht_item_st;

//hash table 表
typedef struct ht_hash_table
{
    int nslot;   //槽的目录
    int count;  //当前数量
    //int max_count;  //最大数量
    //int min_count;  //最小数量
    //int walk_count;   //遍历计数，查看是否正在被遍历
    ht_item_st** item; //用来存放链表
    hash_data_free_func_t hdel; //hash表元素释放函数
    hash_key_func_t hkey;       //hash表生成key函数
}ht_hash_table;

/**
 * @brief 构建hash表
 * @param
 *  hash_data_free_func_t hdel hash表元素释放函数
 *  hash_key_func_t hkey hash表生成key函数
 *  unsigned int nslot hash表槽的数量
*/
ht_hash_table *wns_hash_create(hash_data_free_func_t hdel, hash_key_func_t hkey, unsigned int nslot)
{

}

#endif
