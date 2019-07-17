
#include <napi.h>
#include "classifier.h"
#include "query.h"

Napi::Object Initialize(Napi::Env env, Napi::Object exports)
{
  FasttextClassifier::Init(env, exports);
  FasttextQuery::Init(env, exports);
  return exports;
}

NODE_API_MODULE(addon, Initialize)
