#ifndef FASTTEXT_CLASSIFIER_H
#define FASTTEXT_CLASSIFIER_H

#include <napi.h>
#include "node-util.h"
#include "node-argument.h"
#include "wrapper.h"

class FasttextClassifier : public Napi::ObjectWrap<FasttextClassifier>
{
public:
  static Napi::Object Init(Napi::Env env, Napi::Object exports);
  FasttextClassifier(const Napi::CallbackInfo &info);

  /**
   * Destructor
   */
  ~FasttextClassifier()
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
