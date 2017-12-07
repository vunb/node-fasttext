
#include <nan.h>
#include "classifier.h"   // NOLINT(build/include)
#include "query.h"   // NOLINT(build/include)

NAN_MODULE_INIT(Init) {
  Classifier::Init(target);
  Query::Init(target);
}

NODE_MODULE(myaddon, Init)