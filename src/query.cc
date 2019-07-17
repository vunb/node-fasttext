#include "query.h"

Napi::FunctionReference FasttextQuery::constructor;

Napi::Object FasttextQuery::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);
  Napi::Function func = DefineClass(env, "FasttextQuery",
                                    {InstanceMethod("nn", &FasttextQuery::Nn)});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Query", func);
  return exports;
}

FasttextQuery::FasttextQuery(const Napi::CallbackInfo &info) : Napi::ObjectWrap<FasttextQuery>(info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  if (info.Length() == 0 || !info[0].IsString())
  {
    Napi::TypeError::New(env, "Path to model file is missing!").ThrowAsJavaScriptException();
  }

  std::string modelFileName = info[0].As<Napi::String>().Utf8Value();
  this->wrapper_ = new Wrapper(modelFileName);
}

Napi::Value FasttextQuery::Nn(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  Napi::Function callback = Napi::Function::New(env, EmptyCallback);
  int32_t k = 10;

  if (info.Length() == 0 || !info[0].IsString())
  {
    Napi::TypeError::New(env, "query must be a string").ThrowAsJavaScriptException();
  }

  if (info.Length() > 1)
  {
    if (info[1].IsNumber())
    {
      k = info[1].As<Napi::Number>().Int32Value();
    }
    else if (info[1].IsFunction())
    {
      callback = info[1].As<Napi::Function>();
    }
  }

  if (info.Length() > 2 && info[2].IsFunction())
  {
    callback = info[1].As<Napi::Function>();
  }

  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(info.Env());
  Napi::String query = info[0].As<Napi::String>();

  NnWorker *worker = new NnWorker(query, k, this->wrapper_, deferred, callback);
  worker->Queue();

  return worker->deferred_.Promise();
}
