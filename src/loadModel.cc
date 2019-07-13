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
  // Napi::Object result = NodeArgument::NodeArgument::mapToNapiObject(defferred_.Env(), result_);
  Napi::Env env = defferred_.Env();
  Napi::Object result = Napi::Object::New(env);

  for (auto const &iterator : result_)
  {
    Napi::Value value;

    if (nodeArg.isOnlyDouble(iterator.second.c_str()))
    {
      value = Napi::Number::New(env, atof(iterator.second.c_str()));
    }
    else
    {
      value = Napi::String::New(env, iterator.second.c_str());
    }
    result.Set(Napi::String::New(env, iterator.first.c_str()), value);
  }

  defferred_.Resolve(result);

  // Call empty function
  Callback().Call({});
}

void LoadModelWorker::OnError()
{
  Napi::HandleScope scope(Env());
  defferred_.Reject(Napi::String::New(Env(), "Something wrong"));

  // Call empty function
  Callback().Call({});
}
