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

  /**
   * Destructor
   */
  ~NodeFasttext()
  {
    delete wrapper_;
  }

private:
  static Napi::FunctionReference constructor;

  Napi::Value LoadModel(const Napi::CallbackInfo &info);
  Napi::Value Predict(const Napi::CallbackInfo &info);
  Napi::Value Train(const Napi::CallbackInfo &info);
  Napi::Value Quantize(const Napi::CallbackInfo &info);

  Wrapper *wrapper_;
};

#endif
