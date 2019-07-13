#include "loadModel.h"
#include "node-argument.h"


void LoadModelWorker::Execute () {
	try {
    result_ = wrapper_->loadModel( filename );
  } catch (std::string errorMessage) {
    SetError(errorMessage.c_str());
  }
}

void LoadModelWorker::OnOK () {
  NodeArgument::NodeArgument nodeArg;
  v8::Local<v8::Object> result = nodeArg.mapToObject( result_ );

  // TODO: Return map result
  Callback().Call({Env().Undefined(), Napi::Number::New(Env(), 1000)});
}
