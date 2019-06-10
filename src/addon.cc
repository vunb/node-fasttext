
#include <node.h>
#include "classifier.h"   // NOLINT(build/include)
#include "query.h"        // NOLINT(build/include)

NAN_MODULE_INIT(Init) {
  Classifier::Init(target);
  Query::Init(target);
}

void Initialize(v8::Local<v8::Object> exports) {
  Classifier::Init(exports);
  Query::Init(exports);
}

NODE_MODULE(fastTextAddon, Initialize)
