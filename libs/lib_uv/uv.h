#ifndef __UV_H__
#define __UV_H__

//
struct _uv_queue {
  struct _uv_queue* next;
  struct _uv_queue* prev;
};

struct uv_loop_s {
    void *data; //用户自定义数据

    unsigned int active_handles;    //长请求
    struct uv__queue handle_queue;  //链表
    union{
        void* unused;  
        unsigned int count;
    }active_reqs;   //短请求
}

#endif
