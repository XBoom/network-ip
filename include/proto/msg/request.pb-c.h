/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: request.proto */

#ifndef PROTOBUF_C_request_2eproto__INCLUDED
#define PROTOBUF_C_request_2eproto__INCLUDED

#include <protobuf-c/protobuf-c.h>

PROTOBUF_C__BEGIN_DECLS

#if PROTOBUF_C_VERSION_NUMBER < 1003000
# error This file was generated by a newer version of protoc-c which is incompatible with your libprotobuf-c headers. Please update your headers.
#elif 1004001 < PROTOBUF_C_MIN_COMPILER_VERSION
# error This file was generated by an older version of protoc-c which is incompatible with your libprotobuf-c headers. Please regenerate this file with a newer version of protoc-c.
#endif


typedef struct FifoReqHead FifoReqHead;
typedef struct FifoRespHead FifoRespHead;
typedef struct FifoRequest FifoRequest;
typedef struct FifoResponse FifoResponse;


/* --- enums --- */


/* --- messages --- */

/*
 *消息头
 */
struct  FifoReqHead
{
  ProtobufCMessage base;
  /*
   *消息请求 id
   */
  int32_t id;
  /*
   *响应的管道名称
   */
  char *fifo;
  /*
   *body 长度
   */
  int32_t body_len;
};
#define FIFO_REQ_HEAD__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&fifo_req_head__descriptor) \
    , 0, (char *)protobuf_c_empty_string, 0 }


/*
 *消息头
 */
struct  FifoRespHead
{
  ProtobufCMessage base;
  /*
   *响应 ID，与请求一样表示响应的哪个请求
   */
  int32_t id;
  /*
   *表示响应错误码
   */
  int32_t code;
  /*
   *数据长度
   */
  int32_t len;
};
#define FIFO_RESP_HEAD__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&fifo_resp_head__descriptor) \
    , 0, 0, 0 }


struct  FifoRequest
{
  ProtobufCMessage base;
  FifoReqHead *head;
  char *body;
};
#define FIFO_REQUEST__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&fifo_request__descriptor) \
    , NULL, (char *)protobuf_c_empty_string }


struct  FifoResponse
{
  ProtobufCMessage base;
  FifoRespHead *head;
  char *body;
};
#define FIFO_RESPONSE__INIT \
 { PROTOBUF_C_MESSAGE_INIT (&fifo_response__descriptor) \
    , NULL, (char *)protobuf_c_empty_string }


/* FifoReqHead methods */
void   fifo_req_head__init
                     (FifoReqHead         *message);
size_t fifo_req_head__get_packed_size
                     (const FifoReqHead   *message);
size_t fifo_req_head__pack
                     (const FifoReqHead   *message,
                      uint8_t             *out);
size_t fifo_req_head__pack_to_buffer
                     (const FifoReqHead   *message,
                      ProtobufCBuffer     *buffer);
FifoReqHead *
       fifo_req_head__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   fifo_req_head__free_unpacked
                     (FifoReqHead *message,
                      ProtobufCAllocator *allocator);
/* FifoRespHead methods */
void   fifo_resp_head__init
                     (FifoRespHead         *message);
size_t fifo_resp_head__get_packed_size
                     (const FifoRespHead   *message);
size_t fifo_resp_head__pack
                     (const FifoRespHead   *message,
                      uint8_t             *out);
size_t fifo_resp_head__pack_to_buffer
                     (const FifoRespHead   *message,
                      ProtobufCBuffer     *buffer);
FifoRespHead *
       fifo_resp_head__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   fifo_resp_head__free_unpacked
                     (FifoRespHead *message,
                      ProtobufCAllocator *allocator);
/* FifoRequest methods */
void   fifo_request__init
                     (FifoRequest         *message);
size_t fifo_request__get_packed_size
                     (const FifoRequest   *message);
size_t fifo_request__pack
                     (const FifoRequest   *message,
                      uint8_t             *out);
size_t fifo_request__pack_to_buffer
                     (const FifoRequest   *message,
                      ProtobufCBuffer     *buffer);
FifoRequest *
       fifo_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   fifo_request__free_unpacked
                     (FifoRequest *message,
                      ProtobufCAllocator *allocator);
/* FifoResponse methods */
void   fifo_response__init
                     (FifoResponse         *message);
size_t fifo_response__get_packed_size
                     (const FifoResponse   *message);
size_t fifo_response__pack
                     (const FifoResponse   *message,
                      uint8_t             *out);
size_t fifo_response__pack_to_buffer
                     (const FifoResponse   *message,
                      ProtobufCBuffer     *buffer);
FifoResponse *
       fifo_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data);
void   fifo_response__free_unpacked
                     (FifoResponse *message,
                      ProtobufCAllocator *allocator);
/* --- per-message closures --- */

typedef void (*FifoReqHead_Closure)
                 (const FifoReqHead *message,
                  void *closure_data);
typedef void (*FifoRespHead_Closure)
                 (const FifoRespHead *message,
                  void *closure_data);
typedef void (*FifoRequest_Closure)
                 (const FifoRequest *message,
                  void *closure_data);
typedef void (*FifoResponse_Closure)
                 (const FifoResponse *message,
                  void *closure_data);

/* --- services --- */


/* --- descriptors --- */

extern const ProtobufCMessageDescriptor fifo_req_head__descriptor;
extern const ProtobufCMessageDescriptor fifo_resp_head__descriptor;
extern const ProtobufCMessageDescriptor fifo_request__descriptor;
extern const ProtobufCMessageDescriptor fifo_response__descriptor;

PROTOBUF_C__END_DECLS


#endif  /* PROTOBUF_C_request_2eproto__INCLUDED */
