#ifndef NODEFASTTEXT_H
#define NODEFASTTEXT_H

#include <napi.h>
#include "node-util.h"
#include "node-argument.h"
#include "wrapper.h"

class NodeFasttext : public Napi::ObjectWrap<NodeFasttext>
{
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  NodeFasttext(const Napi::CallbackInfo &info);

private:
  static Napi::FunctionReference constructor;

  Napi::Value GetValue(const Napi::CallbackInfo &info);
  Napi::Value PlusOne(const Napi::CallbackInfo &info);
  Napi::Value Multiply(const Napi::CallbackInfo &info);
  Napi::Value LoadModel(const Napi::CallbackInfo &info);
  Napi::Value Predict(const Napi::CallbackInfo &info);
  Napi::Value Train(const Napi::CallbackInfo &info);

  double value_;
  Wrapper *wrapper_;
};

#endif
