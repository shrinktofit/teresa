#ifndef SRC_NODE_ERRORS_H_
#define SRC_NODE_ERRORS_H_

#if defined(NODE_WANT_INTERNALS) && NODE_WANT_INTERNALS

#include "node.h"
#include "util-inl.h"
#include "env-inl.h"
#include "v8.h"

namespace node {

// Helpers to construct errors similar to the ones provided by
// lib/internal/errors.js.
// Example: with `V(ERR_INVALID_ARG_TYPE, TypeError)`, there will be
// `node::ERR_INVALID_ARG_TYPE(isolate, "message")` returning
// a `Local<Value>` containing the TypeError with proper code and message

#define ERRORS_WITH_CODE(V)                                                  \
  V(ERR_BUFFER_OUT_OF_BOUNDS, RangeError)                                    \
  V(ERR_BUFFER_TOO_LARGE, Error)                                             \
  V(ERR_INDEX_OUT_OF_RANGE, RangeError)                                      \
  V(ERR_INVALID_ARG_VALUE, TypeError)                                        \
  V(ERR_INVALID_ARG_TYPE, TypeError)                                         \
  V(ERR_MEMORY_ALLOCATION_FAILED, Error)                                     \
  V(ERR_MISSING_ARGS, TypeError)                                             \
  V(ERR_MISSING_MODULE, Error)                                               \
  V(ERR_STRING_TOO_LONG, Error)                                              \

#define V(code, type)                                                         \
  inline v8::Local<v8::Value> code(v8::Isolate* isolate,                      \
                                   const char* message)       {               \
    v8::Local<v8::String> js_code = OneByteString(isolate, #code);            \
    v8::Local<v8::String> js_msg = OneByteString(isolate, message);           \
    v8::Local<v8::Object> e =                                                 \
        v8::Exception::type(js_msg)->ToObject(                                \
            isolate->GetCurrentContext()).ToLocalChecked();                   \
    e->Set(isolate->GetCurrentContext(), OneByteString(isolate, "code"),      \
           js_code).FromJust();                                               \
    return e;                                                                 \
  }                                                                           \
  inline void THROW_ ## code(Environment* env, const char* message) {         \
    env->isolate()->ThrowException(code(env->isolate(), message));            \
  }
  ERRORS_WITH_CODE(V)
#undef V

// Errors with predefined static messages

#define PREDEFINED_ERROR_MESSAGES(V)                                         \
  V(ERR_INDEX_OUT_OF_RANGE, "Index out of range")                            \
  V(ERR_MEMORY_ALLOCATION_FAILED, "Failed to allocate memory")

#define V(code, message)                                                     \
  inline v8::Local<v8::Value> code(v8::Isolate* isolate) {                   \
    return code(isolate, message);                                           \
  }                                                                          \
  inline void THROW_ ## code(Environment* env) {                             \
    env->isolate()->ThrowException(code(env->isolate(), message));           \
  }
  PREDEFINED_ERROR_MESSAGES(V)
#undef V

// Errors with predefined non-static messages

inline v8::Local<v8::Value> ERR_BUFFER_TOO_LARGE(v8::Isolate *isolate) {
  char message[128];
  snprintf(message, sizeof(message),
      "Cannot create a Buffer larger than 0x%lx bytes",
      v8::TypedArray::kMaxLength);
  return ERR_BUFFER_TOO_LARGE(isolate, message);
}

inline v8::Local<v8::Value> ERR_STRING_TOO_LONG(v8::Isolate *isolate) {
  char message[128];
  snprintf(message, sizeof(message),
      "Cannot create a string longer than 0x%x characters",
      v8::String::kMaxLength);
  return ERR_STRING_TOO_LONG(isolate, message);
}

#define THROW_AND_RETURN_IF_NOT_BUFFER(env, val, prefix)                     \
  do {                                                                       \
    if (!Buffer::HasInstance(val))                                           \
      return node::THROW_ERR_INVALID_ARG_TYPE(env,                           \
                                              prefix " must be a buffer");   \
  } while (0)

#define THROW_AND_RETURN_IF_NOT_STRING(env, val, prefix)                     \
  do {                                                                       \
    if (!val->IsString())                                                    \
      return node::THROW_ERR_INVALID_ARG_TYPE(env,                           \
                                              prefix " must be a string");   \
  } while (0)

}  // namespace node

#endif  // defined(NODE_WANT_INTERNALS) && NODE_WANT_INTERNALS

#endif  // SRC_NODE_ERRORS_H_
