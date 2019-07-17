#include "loadModel.h"
#include "node-argument.h"

void LoadModelWorker::Execute()
{
  try
  {
    result_ = wrapper_->loadModel(filename);
  }
  catch (std::string errorMessage)
  {
    SetError(errorMessage.c_str());
  }
}

void LoadModelWorker::OnOK()
{
  NodeArgument::NodeArgument nodeArg;
  Napi::Object result = nodeArg.mapToNapiObject(Env(), result_);

  deferred_.Resolve(result);

  // Call empty function
  if (!Callback().IsEmpty())
  {
    Callback().Call({Env().Null(), result});
  }
}

void LoadModelWorker::OnError()
{
  Napi::HandleScope scope(Env());
  deferred_.Reject(Napi::String::New(Env(), "Can't load model file!"));

  // Call empty function
  Callback().Call({});
}
