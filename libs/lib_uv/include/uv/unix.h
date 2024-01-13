#ifndef __UNIX_H__
#define __UNIX_H__

#define UV_LOOP_PRIVATE_FIELDS                                                \
  unsigned long flags;                                                        \
  int backend_fd;                                                             \
  struct uv__queue pending_queue;                                             \
  struct uv__queue watcher_queue;                                             \
  uv__io_t** watchers;                                                        \
  unsigned int nwatchers;                                                     \
  unsigned int nfds;                                                          \
  struct uv__queue wq;                                                        \
  uv_mutex_t wq_mutex;                                                        \
  uv_async_t wq_async;                                                        \
  uv_rwlock_t cloexec_lock;                                                   \
  uv_handle_t* closing_handles;                                               \
  struct uv__queue process_handles;                                           \
  struct uv__queue prepare_handles;                                           \
  struct uv__queue check_handles;                                             \
  struct uv__queue idle_handles;                                              \
  struct uv__queue async_handles;                                             \
  void (*async_unused)(void);  /* TODO(bnoordhuis) Remove in libuv v2. */     \
  uv__io_t async_io_watcher;                                                  \
  int async_wfd;                                                              \
  struct {                                                                    \
    void* min;                                                                \
    unsigned int nelts;                                                       \
  } timer_heap;                                                               \
  uint64_t timer_counter;                                                     \
  uint64_t time;                                                              \
  int signal_pipefd[2];                                                       \
  uv__io_t signal_io_watcher;                                                 \
  uv_signal_t child_watcher;                                                  \
  int emfile_fd;                                                              \
  UV_PLATFORM_LOOP_FIELDS                                                     \

  #endif