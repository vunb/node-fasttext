#include "node-fasttext.h"
#include "loadModel.h"
#include "predictWorker.h"
#include "train.h"
#include <iostream>

Napi::FunctionReference NodeFasttext::constructor;

Napi::Object NodeFasttext::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);

  Napi::Function func = DefineClass(env, "Classifier",
                                    {InstanceMethod("loadModel", &NodeFasttext::LoadModel),
                                     InstanceMethod("predict", &NodeFasttext::Predict),
                                     InstanceMethod("train", &NodeFasttext::Train),
                                     InstanceMethod("plusOne", &NodeFasttext::PlusOne),
                                     InstanceMethod("value", &NodeFasttext::GetValue),
                                     InstanceMethod("multiply", &NodeFasttext::Multiply)});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Classifier", func);
  return exports;
}

NodeFasttext::NodeFasttext(const Napi::CallbackInfo &info) : Napi::ObjectWrap<NodeFasttext>(info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  std::string modelFileName = "";

  if (info.Length() > 0 && info[0].IsString())
  {
    modelFileName = info[0].As<Napi::String>().Utf8Value();
  }

  this->wrapper_ = new Wrapper(modelFileName);
}

Napi::Value NodeFasttext::GetValue(const Napi::CallbackInfo &info)
{
  double num = this->value_;

  return Napi::Number::New(info.Env(), num);
}

Napi::Value NodeFasttext::PlusOne(const Napi::CallbackInfo &info)
{
  this->value_ = this->value_ + 1;

  return NodeFasttext::GetValue(info);
}

Napi::Value NodeFasttext::Multiply(const Napi::CallbackInfo &info)
{
  Napi::Number multiple;
  if (info.Length() <= 0 || !info[0].IsNumber())
  {
    multiple = Napi::Number::New(info.Env(), 1);
  }
  else
  {
    multiple = info[0].As<Napi::Number>();
  }

  Napi::Object obj = constructor.New({Napi::Number::New(info.Env(), this->value_ * multiple.DoubleValue())});

  return obj;
}

Napi::Value NodeFasttext::LoadModel(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  Napi::Function callback;

  if (info.Length() < 1)
  {
    Napi::TypeError::New(env, "Path to model file is missing!").ThrowAsJavaScriptException();
  }
  else if (!info[0].IsString())
  {
    Napi::TypeError::New(env, "Model file path must be a string!").ThrowAsJavaScriptException();
  }

  if (info.Length() > 1 && info[1].IsFunction())
  {
    callback = info[1].As<Napi::Function>();
  }
  else
  {
    callback = Napi::Function::New(env, EmptyCallback);
  }

  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(info.Env());
  Napi::String filename = info[0].As<Napi::String>();

  std::cout << "Preparing load model from: " << filename.Utf8Value() << std::endl;

  LoadModelWorker *worker = new LoadModelWorker(filename, this->wrapper_, deferred, callback);
  worker->Queue();

  return worker->defferred_.Promise();
}

Napi::Value NodeFasttext::Predict(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  Napi::Function callback = Napi::Function::New(env, EmptyCallback);
  ;
  int32_t k = 1;

  if (info.Length() < 1 || !info[0].IsString())
  {
    Napi::TypeError::New(env, "sentence must be a string").ThrowAsJavaScriptException();
  }

  if (info.Length() > 1 && info[1].IsNumber())
  {
    k = info[1].As<Napi::Number>().Int32Value();
  }

  if (info.Length() > 2 && info[2].IsFunction())
  {
    callback = info[2].As<Napi::Function>();
  }

  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(info.Env());
  Napi::String sentence = info[0].As<Napi::String>();

  PredictWorker *worker = new PredictWorker(sentence, k, this->wrapper_, deferred, callback);
  worker->Queue();

  return worker->deferred_.Promise();
}

Napi::Value NodeFasttext::Train(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  Napi::Function callback = Napi::Function::New(env, EmptyCallback);
  ;
  int32_t k = 1;

  if (info.Length() < 2)
  {
    Napi::TypeError::New(env, "requires at least 2 parameters").ThrowAsJavaScriptException();
  }
  else if (!info[0].IsString())
  {
    Napi::TypeError::New(env, "command must be a string").ThrowAsJavaScriptException();
  }
  else if (!info[1].IsObject())
  {
    Napi::TypeError::New(env, "options must be an object").ThrowAsJavaScriptException();
  }

  if (info.Length() > 2 && info[2].IsFunction())
  {
    callback = info[2].As<Napi::Function>();
  }

  std::string command = info[0].As<Napi::String>().Utf8Value();

  if (!(command == "cbow" || command == "quantize" || command == "skipgram" || command == "supervised"))
  {
    Napi::TypeError::New(env, "Permitted command types are ['cbow', 'quantize', 'skipgram', 'supervised").ThrowAsJavaScriptException();
  }

  Napi::Promise::Deferred deferred = Napi::Promise::Deferred::New(info.Env());
  NodeArgument::NodeArgument nodeArg;
  NodeArgument::CArgument c_argument;

  try
  {
    Napi::Object confObj = info[1].As<Napi::Object>();
    c_argument = nodeArg.NapiObjectToCArgument(env, confObj);
  }
  catch (std::string errorMessage)
  {
    Napi::TypeError::New(env, errorMessage.c_str()).ThrowAsJavaScriptException();
  }

  int count = c_argument.argc;
  char **argument = c_argument.argv;

  std::vector<std::string> args;
  args.push_back("-command");
  args.push_back(command.c_str());

  for (int j = 0; j < count; j++)
  {
    args.push_back(argument[j]);
  }

  //TODO: check command type quantize, supervised
  TrainWorker *worker = new TrainWorker(args, this->wrapper_, deferred, callback);
  worker->Queue();

  return worker->deferred_.Promise();
}
