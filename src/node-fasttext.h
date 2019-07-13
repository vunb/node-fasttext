#ifndef NODEFASTTEXT_H
#define NODEFASTTEXT_H

#include <napi.h>
#include "wrapper.h"

class NodeFasttext : public Napi::ObjectWrap<NodeFasttext> {
 public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  NodeFasttext(const Napi::CallbackInfo& info);

 private:
  static Napi::FunctionReference constructor;

  Napi::Value GetValue(const Napi::CallbackInfo& info);
  Napi::Value PlusOne(const Napi::CallbackInfo& info);
  Napi::Value Multiply(const Napi::CallbackInfo& info);
  Napi::Value LoadModel(const Napi::CallbackInfo& info);

  double value_;
  Wrapper *wrapper_;

};

#endif
