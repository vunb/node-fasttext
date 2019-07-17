#ifndef FASTTEXT_QUERY_H
#define FASTTEXT_QUERY_H

#include <napi.h>
#include "wrapper.h"
#include "nnWorker.h"
#include "node-util.h"

class FasttextQuery : public Napi::ObjectWrap<FasttextQuery>
{
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  FasttextQuery(const Napi::CallbackInfo &info);

  /**
   * Destructor
   */
  ~FasttextQuery()
  {
    delete wrapper_;
  }

private:
  static Napi::FunctionReference constructor;
  Napi::Value Nn(const Napi::CallbackInfo &info);

  Wrapper *wrapper_;
};

#endif
