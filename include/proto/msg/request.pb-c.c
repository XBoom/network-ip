/* Generated by the protocol buffer compiler.  DO NOT EDIT! */
/* Generated from: request.proto */

/* Do not generate deprecated warnings for self */
#ifndef PROTOBUF_C__NO_DEPRECATED
#define PROTOBUF_C__NO_DEPRECATED
#endif

#include "request.pb-c.h"
void   fifo_req_head__init
                     (FifoReqHead         *message)
{
  static const FifoReqHead init_value = FIFO_REQ_HEAD__INIT;
  *message = init_value;
}
size_t fifo_req_head__get_packed_size
                     (const FifoReqHead *message)
{
  assert(message->base.descriptor == &fifo_req_head__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t fifo_req_head__pack
                     (const FifoReqHead *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &fifo_req_head__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t fifo_req_head__pack_to_buffer
                     (const FifoReqHead *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &fifo_req_head__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
FifoReqHead *
       fifo_req_head__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (FifoReqHead *)
     protobuf_c_message_unpack (&fifo_req_head__descriptor,
                                allocator, len, data);
}
void   fifo_req_head__free_unpacked
                     (FifoReqHead *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &fifo_req_head__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   fifo_resp_head__init
                     (FifoRespHead         *message)
{
  static const FifoRespHead init_value = FIFO_RESP_HEAD__INIT;
  *message = init_value;
}
size_t fifo_resp_head__get_packed_size
                     (const FifoRespHead *message)
{
  assert(message->base.descriptor == &fifo_resp_head__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t fifo_resp_head__pack
                     (const FifoRespHead *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &fifo_resp_head__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t fifo_resp_head__pack_to_buffer
                     (const FifoRespHead *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &fifo_resp_head__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
FifoRespHead *
       fifo_resp_head__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (FifoRespHead *)
     protobuf_c_message_unpack (&fifo_resp_head__descriptor,
                                allocator, len, data);
}
void   fifo_resp_head__free_unpacked
                     (FifoRespHead *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &fifo_resp_head__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   fifo_request__init
                     (FifoRequest         *message)
{
  static const FifoRequest init_value = FIFO_REQUEST__INIT;
  *message = init_value;
}
size_t fifo_request__get_packed_size
                     (const FifoRequest *message)
{
  assert(message->base.descriptor == &fifo_request__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t fifo_request__pack
                     (const FifoRequest *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &fifo_request__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t fifo_request__pack_to_buffer
                     (const FifoRequest *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &fifo_request__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
FifoRequest *
       fifo_request__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (FifoRequest *)
     protobuf_c_message_unpack (&fifo_request__descriptor,
                                allocator, len, data);
}
void   fifo_request__free_unpacked
                     (FifoRequest *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &fifo_request__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
void   fifo_response__init
                     (FifoResponse         *message)
{
  static const FifoResponse init_value = FIFO_RESPONSE__INIT;
  *message = init_value;
}
size_t fifo_response__get_packed_size
                     (const FifoResponse *message)
{
  assert(message->base.descriptor == &fifo_response__descriptor);
  return protobuf_c_message_get_packed_size ((const ProtobufCMessage*)(message));
}
size_t fifo_response__pack
                     (const FifoResponse *message,
                      uint8_t       *out)
{
  assert(message->base.descriptor == &fifo_response__descriptor);
  return protobuf_c_message_pack ((const ProtobufCMessage*)message, out);
}
size_t fifo_response__pack_to_buffer
                     (const FifoResponse *message,
                      ProtobufCBuffer *buffer)
{
  assert(message->base.descriptor == &fifo_response__descriptor);
  return protobuf_c_message_pack_to_buffer ((const ProtobufCMessage*)message, buffer);
}
FifoResponse *
       fifo_response__unpack
                     (ProtobufCAllocator  *allocator,
                      size_t               len,
                      const uint8_t       *data)
{
  return (FifoResponse *)
     protobuf_c_message_unpack (&fifo_response__descriptor,
                                allocator, len, data);
}
void   fifo_response__free_unpacked
                     (FifoResponse *message,
                      ProtobufCAllocator *allocator)
{
  if(!message)
    return;
  assert(message->base.descriptor == &fifo_response__descriptor);
  protobuf_c_message_free_unpacked ((ProtobufCMessage*)message, allocator);
}
static const ProtobufCFieldDescriptor fifo_req_head__field_descriptors[3] =
{
  {
    "id",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(FifoReqHead, id),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "fifo",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(FifoReqHead, fifo),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "body_len",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(FifoReqHead, body_len),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned fifo_req_head__field_indices_by_name[] = {
  2,   /* field[2] = body_len */
  1,   /* field[1] = fifo */
  0,   /* field[0] = id */
};
static const ProtobufCIntRange fifo_req_head__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor fifo_req_head__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "fifo_req_head",
  "FifoReqHead",
  "FifoReqHead",
  "",
  sizeof(FifoReqHead),
  3,
  fifo_req_head__field_descriptors,
  fifo_req_head__field_indices_by_name,
  1,  fifo_req_head__number_ranges,
  (ProtobufCMessageInit) fifo_req_head__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor fifo_resp_head__field_descriptors[3] =
{
  {
    "id",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(FifoRespHead, id),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "code",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(FifoRespHead, code),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "len",
    3,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_INT32,
    0,   /* quantifier_offset */
    offsetof(FifoRespHead, len),
    NULL,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned fifo_resp_head__field_indices_by_name[] = {
  1,   /* field[1] = code */
  0,   /* field[0] = id */
  2,   /* field[2] = len */
};
static const ProtobufCIntRange fifo_resp_head__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 3 }
};
const ProtobufCMessageDescriptor fifo_resp_head__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "fifo_resp_head",
  "FifoRespHead",
  "FifoRespHead",
  "",
  sizeof(FifoRespHead),
  3,
  fifo_resp_head__field_descriptors,
  fifo_resp_head__field_indices_by_name,
  1,  fifo_resp_head__number_ranges,
  (ProtobufCMessageInit) fifo_resp_head__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor fifo_request__field_descriptors[2] =
{
  {
    "head",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(FifoRequest, head),
    &fifo_req_head__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "body",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(FifoRequest, body),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned fifo_request__field_indices_by_name[] = {
  1,   /* field[1] = body */
  0,   /* field[0] = head */
};
static const ProtobufCIntRange fifo_request__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor fifo_request__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "fifo_request",
  "FifoRequest",
  "FifoRequest",
  "",
  sizeof(FifoRequest),
  2,
  fifo_request__field_descriptors,
  fifo_request__field_indices_by_name,
  1,  fifo_request__number_ranges,
  (ProtobufCMessageInit) fifo_request__init,
  NULL,NULL,NULL    /* reserved[123] */
};
static const ProtobufCFieldDescriptor fifo_response__field_descriptors[2] =
{
  {
    "head",
    1,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_MESSAGE,
    0,   /* quantifier_offset */
    offsetof(FifoResponse, head),
    &fifo_resp_head__descriptor,
    NULL,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
  {
    "body",
    2,
    PROTOBUF_C_LABEL_NONE,
    PROTOBUF_C_TYPE_STRING,
    0,   /* quantifier_offset */
    offsetof(FifoResponse, body),
    NULL,
    &protobuf_c_empty_string,
    0,             /* flags */
    0,NULL,NULL    /* reserved1,reserved2, etc */
  },
};
static const unsigned fifo_response__field_indices_by_name[] = {
  1,   /* field[1] = body */
  0,   /* field[0] = head */
};
static const ProtobufCIntRange fifo_response__number_ranges[1 + 1] =
{
  { 1, 0 },
  { 0, 2 }
};
const ProtobufCMessageDescriptor fifo_response__descriptor =
{
  PROTOBUF_C__MESSAGE_DESCRIPTOR_MAGIC,
  "fifo_response",
  "FifoResponse",
  "FifoResponse",
  "",
  sizeof(FifoResponse),
  2,
  fifo_response__field_descriptors,
  fifo_response__field_indices_by_name,
  1,  fifo_response__number_ranges,
  (ProtobufCMessageInit) fifo_response__init,
  NULL,NULL,NULL    /* reserved[123] */
};
