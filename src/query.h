#ifndef QUERY_H
#define QUERY_H

#include <napi.h>
#include "wrapper.h"
#include "nnWorker.h"
#include "node-util.h"

class Query : public Napi::ObjectWrap<Query>
{
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  Query(const Napi::CallbackInfo &info);

  /**
   * Destructor
   */
  ~Query()
  {
    delete wrapper_;
  }

private:
  static Napi::FunctionReference constructor;
  Napi::Value Nn(const Napi::CallbackInfo &info);

  Wrapper *wrapper_;
};

#endif
