#include "node-argument.h"
#include "quantize.h"

void QuantizeWorker::Execute()
{
  try
  {
    result_ = wrapper_->quantize(args_);
  }
  catch (const std::string errorMessage)
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

void QuantizeWorker::OnError(const Napi::Error &e)
{
  Napi::HandleScope scope(Env());
  Napi::String error = Napi::String::New(Env(), e.Message());
  deferred_.Reject(error);

  // Call empty function
  Callback().Call({error});
}

void QuantizeWorker::OnOK()
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
