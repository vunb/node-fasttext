#include "classifier.h"
#include "loadModel.h"
#include "predictWorker.h"
#include "train.h"
#include "quantize.h"
#include <iostream>

Napi::FunctionReference FasttextClassifier::constructor;

Napi::Object FasttextClassifier::Init(Napi::Env env, Napi::Object exports)
{
  Napi::HandleScope scope(env);
  Napi::Function func = DefineClass(env, "FasttextClassifier",
                                    {InstanceMethod("loadModel", &FasttextClassifier::LoadModel),
                                     InstanceMethod("predict", &FasttextClassifier::Predict),
                                     InstanceMethod("train", &FasttextClassifier::Train),
                                     InstanceMethod("quantize", &FasttextClassifier::Quantize)});

  constructor = Napi::Persistent(func);
  constructor.SuppressDestruct();

  exports.Set("Classifier", func);
  return exports;
}

FasttextClassifier::FasttextClassifier(const Napi::CallbackInfo &info) : Napi::ObjectWrap<FasttextClassifier>(info)
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

Napi::Value FasttextClassifier::LoadModel(const Napi::CallbackInfo &info)
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

  return worker->deferred_.Promise();
}

Napi::Value FasttextClassifier::Predict(const Napi::CallbackInfo &info)
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

Napi::Value FasttextClassifier::Train(const Napi::CallbackInfo &info)
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

  if (command == "quantize")
  {
    QuantizeWorker *worker = new QuantizeWorker(args, this->wrapper_, deferred, callback);
    worker->Queue();
    return worker->deferred_.Promise();
  }
  else
  {
    TrainWorker *worker = new TrainWorker(args, this->wrapper_, deferred, callback);
    worker->Queue();
    return worker->deferred_.Promise();
  }
}

Napi::Value FasttextClassifier::Quantize(const Napi::CallbackInfo &info)
{
  Napi::Env env = info.Env();
  Napi::HandleScope scope(env);
  Napi::Function callback = Napi::Function::New(env, EmptyCallback);
  ;
  int32_t k = 1;

  if (info.Length() < 1 || !info[0].IsObject())
  {
    Napi::TypeError::New(env, "options must be an object").ThrowAsJavaScriptException();
  }

  if (info.Length() > 1 && info[1].IsFunction())
  {
    callback = info[1].As<Napi::Function>();
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
  args.push_back("quantize");

  for (int j = 0; j < count; j++)
  {
    args.push_back(argument[j]);
  }

  QuantizeWorker *worker = new QuantizeWorker(args, this->wrapper_, deferred, callback);
  worker->Queue();

  return worker->deferred_.Promise();
}
