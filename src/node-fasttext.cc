#include "node-fasttext.h"
#include "loadModel.h"

Napi::FunctionReference NodeFasttext::constructor;

Napi::Object NodeFasttext::Init(Napi::Env env, Napi::Object exports) {
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "NodeFasttext", {
    InstanceMethod("loadModel", &NodeFasttext::LoadModel),
    InstanceMethod("plusOne", &NodeFasttext::PlusOne),
    InstanceMethod("value", &NodeFasttext::GetValue),
    InstanceMethod("multiply", &NodeFasttext::Multiply)
  });

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("NodeFasttext", func);
  return exports;
}

NodeFasttext::NodeFasttext(const Napi::CallbackInfo& info) : Napi::ObjectWrap<NodeFasttext>(info)  {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);

  int length = info.Length();

  if (length <= 0 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
  }

  Napi::Number value = info[0].As<Napi::Number>();
  this->value_ = value.DoubleValue();
  this->wrapper_ = new Wrapper(NULL);
}

Napi::Value NodeFasttext::GetValue(const Napi::CallbackInfo& info) {
  double num = this->value_;

  return Napi::Number::New(info.Env(), num);
}

Napi::Value NodeFasttext::PlusOne(const Napi::CallbackInfo& info) {
  this->value_ = this->value_ + 1;

  return NodeFasttext::GetValue(info);
}

Napi::Value NodeFasttext::Multiply(const Napi::CallbackInfo& info) {
  Napi::Number multiple;
  if (info.Length() <= 0 || !info[0].IsNumber()) {
    multiple = Napi::Number::New(info.Env(), 1);
  } else {
    multiple = info[0].As<Napi::Number>();
  }

  Napi::Object obj = constructor.New({ Napi::Number::New(info.Env(), this->value_ * multiple.DoubleValue()) });

  return obj;
}

Napi::Value NodeFasttext::LoadModel(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  Napi::Function callback;


  if (info.Length() < 1) {
    Napi::TypeError::New(env, "Path to model file is missing!").ThrowAsJavaScriptException();
  } else if (!info[0].IsString()) {
    Napi::TypeError::New(env, "Model file path must be a string!").ThrowAsJavaScriptException();
  }

  if (info.Length() > 1 && info[1].IsFunction())
  {
    callback = info[1].As<Napi::Function>();
  }

  Napi::String filename = info[0].As<Napi::String>();

  LoadModelWorker* worker = new LoadModelWorker(callback, filename, this->wrapper_);
  worker->Queue();

  return NodeFasttext::GetValue(info);
}
