#include "train.h"
#include "node-argument.h"

void TrainWorker::Execute()
{
  try
  {
    result_ = wrapper_->train(args_);
  }
  catch (std::string errorMessage)
  {
    SetError(errorMessage.c_str());
  }
  catch (const char *str)
  {
    SetError(str);
  }
  catch (const std::exception &e)
  {
    SetError(e.what());
  }
}

void TrainWorker::OnError(const Napi::Error &e)
{
  Napi::HandleScope scope(Env());
  Napi::String error = Napi::String::New(Env(), e.Message());
  deferred_.Reject(error);

  // Call empty function
  Callback().Call({error});
}

void TrainWorker::OnOK()
{
  Napi::HandleScope scope(Env());
  Napi::Array result = Napi::Array::New(Env(), result_.size());
  Napi::Env env = Env();

  NodeArgument::NodeArgument nodeArg;
  Napi::Object result = nodeArg.mapToNapiObject(env, result_);
  defferred_.Resolve(result);

  // Call empty function
  if (!Callback().IsEmpty())
  {
    Callback().Call({Env().Null(), result});
  }
}
