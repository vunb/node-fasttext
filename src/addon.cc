
#include <napi.h>
#include "node-fasttext.h"
// #include "classifier.h"   // NOLINT(build/include)
// #include "query.h"        // NOLINT(build/include)

// void Initialize(v8::Local<v8::Object> exports) {
//   Classifier::Init(exports);
//   Query::Init(exports);
// }

Napi::Object Initialize(Napi::Env env, Napi::Object exports) {
  return NodeFasttext::Init(env, exports);

}

NODE_API_MODULE(addon, Initialize)
