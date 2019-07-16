#include "predictWorker.h"

void PredictWorker::Execute()
{
  try
  {
    wrapper_->loadModel();
    result_ = wrapper_->predict(sentence_, k_);
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

void PredictWorker::OnError(const Napi::Error &e)
{
  Napi::HandleScope scope(Env());
  Napi::String error = Napi::String::New(Env(), e.Message());
  deferred_.Reject(error);

  // Call empty function
  Callback().Call({error});
}

void PredictWorker::OnOK()
{
  Napi::Env env = Env();
  Napi::HandleScope scope(env);
  Napi::Array result = Napi::Array::New(env, result_.size());

  for (unsigned int i = 0; i < result_.size(); i++)
  {
    Napi::Object obj = Napi::Object::New(env);

    obj.Set(Napi::String::New(env, "label"), Napi::String::New(env, result_[i].label));
    obj.Set(Napi::String::New(env, "value"), Napi::Number::New(env, result_[i].value));

    result.Set(i, obj);
  }

  deferred_.Resolve(result);

  // Call empty function
  if (!Callback().IsEmpty())
  {
    Callback().Call({env.Null(), result});
  }
}
