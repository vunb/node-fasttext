// classifier.h
#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <node.h>
#include <node_object_wrap.h>
#include "wrapper.h"
#include "node-argument.h"
#include "classifierWorker.h"
#include "loadModel.h"
#include "train.h"
#include "quantize.h"

class Classifier : public Nan::ObjectWrap
{
public:
  static NAN_MODULE_INIT(Init)
  {
    v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
    tpl->SetClassName(Nan::New("Classifier").ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);

    Nan::SetPrototypeMethod(tpl, "train", train);
    Nan::SetPrototypeMethod(tpl, "predict", predict);
    Nan::SetPrototypeMethod(tpl, "loadModel", loadModel);
    Nan::SetPrototypeMethod(tpl, "quantize", quantize);

    constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
    Nan::Set(target, Nan::New("Classifier").ToLocalChecked(),
             Nan::GetFunction(tpl).ToLocalChecked());
  }

private:
  explicit Classifier(std::string modelFilename) : wrapper_(new Wrapper(modelFilename))
  {
  }

  ~Classifier() {
    delete wrapper_;
  }

  static NAN_METHOD(New)
  {
    if (info.IsConstructCall())
    {
      std::string command;
      if (!info[0]->IsString())
      {
        command = "model.bin";
      }
      else
      {
        v8::String::Utf8Value commandArg(info[0]->ToString());
        command = std::string(*commandArg);
      }

      Classifier *obj = new Classifier(command);
      obj->Wrap(info.This());
      info.GetReturnValue().Set(info.This());
    }
    else
    {
      const int argc = 1;
      v8::Local<v8::Value> argv[argc] = {info[0]};
      v8::Local<v8::Function> cons = Nan::New(constructor());
      info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
    }
  }

  static NAN_METHOD(loadModel)
  {

    if (!info[0]->IsString())
    {
      Nan::ThrowError("model file path must be a string");
      return;
    }

    v8::String::Utf8Value modelArg(info[0]->ToString());
    std::string filename = std::string(*modelArg);
    Classifier *obj = Nan::ObjectWrap::Unwrap<Classifier>(info.Holder());

    const auto worker = new LoadModel(filename, obj->wrapper_);
    const auto resolver = v8::Promise::Resolver::New(info.GetIsolate());
    worker->SaveToPersistent("key", resolver);
    info.GetReturnValue().Set(resolver->GetPromise());
    Nan::AsyncQueueWorker(worker);
  }

  static NAN_METHOD(predict)
  {
    if (!info[0]->IsString())
    {
      Nan::ThrowError("sentence must be a string");
      return;
    }

    int32_t k = 1;
    if (info[1]->IsUint32())
    {
      k = info[1]->Uint32Value();
    }

    v8::String::Utf8Value sentenceArg(info[0]->ToString());
    std::string sentence = std::string(*sentenceArg);

    Classifier *obj = Nan::ObjectWrap::Unwrap<Classifier>(info.Holder());

    auto worker = new ClassifierWorker(sentence, k, obj->wrapper_);
    auto resolver = v8::Promise::Resolver::New(info.GetIsolate());
    worker->SaveToPersistent("key", resolver);
    info.GetReturnValue().Set(resolver->GetPromise());
    Nan::AsyncQueueWorker(worker);
  }

  static NAN_METHOD(train)
  {

    if (!info[0]->IsString())
    {
      Nan::ThrowError("type argument must be a string.");
      return;
    }

    if (!info[1]->IsObject())
    {
      Nan::ThrowError("options argument must be an object.");
      return;
    }

    v8::String::Utf8Value commandArg(info[0]->ToString());
    v8::String::Utf8Value commandConf(info[1]->ToString());
    std::string command = std::string(*commandArg);
    std::string conf = std::string(*commandConf);

    if (command == "cbow" || command == "quantize" || command == "skipgram" || command == "supervised")
    {

      v8::Local<v8::Object> confObj = v8::Local<v8::Object>::Cast(info[1]);

      NodeArgument::NodeArgument nodeArg;
      NodeArgument::CArgument c_argument;

      try
      {
        c_argument = nodeArg.ObjectToCArgument(confObj);
      }
      catch (std::string errorMessage)
      {
        Nan::ThrowError(errorMessage.c_str());
        return;
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

      // std::cout << "Args <<<<< Params" << std::endl;
      // for (std::string& a : args) {
      //     std::cout << a << " " << std::endl;
      // }
      // std::cout << std::endl;

      Classifier *obj = Nan::ObjectWrap::Unwrap<Classifier>(info.Holder());

      if (command == "quantize")
      {
        auto worker = new Quantize(args, obj->wrapper_);
        auto resolver = v8::Promise::Resolver::New(info.GetIsolate());
        worker->SaveToPersistent("key", resolver);
        info.GetReturnValue().Set(resolver->GetPromise());
        Nan::AsyncQueueWorker(worker);
      }
      else
      {
        auto worker = new Train(args, obj->wrapper_);
        auto resolver = v8::Promise::Resolver::New(info.GetIsolate());
        worker->SaveToPersistent("key", resolver);
        info.GetReturnValue().Set(resolver->GetPromise());
        Nan::AsyncQueueWorker(worker);
      }
    }
    else
    {
      Nan::ThrowError("Permitted types are ['cbow', 'quantize', 'skipgram', 'supervised'].");
      return;
    }
  }

  static inline Nan::Persistent<v8::Function> &constructor()
  {
    static Nan::Persistent<v8::Function> my_constructor;
    return my_constructor;
  }

  Wrapper *wrapper_;

  static NAN_METHOD(quantize)
  {

    if (!info[0]->IsString())
    {
      Nan::ThrowError("type argument must be a string.");
      return;
    }

    if (!info[1]->IsObject())
    {
      Nan::ThrowError("options argument must be an object.");
      return;
    }

    const v8::String::Utf8Value commandArg(info[0]->ToString());
    const v8::String::Utf8Value commandConf(info[1]->ToString());
    const std::string command = std::string(*commandArg);
    const std::string conf = std::string(*commandConf);

    if (command == "quantize")
    {
      const v8::Local<v8::Object> confObj = v8::Local<v8::Object>::Cast(info[1]);

      NodeArgument::NodeArgument nodeArg;
      NodeArgument::CArgument c_argument;
      try
      {
        c_argument = nodeArg.ObjectToCArgument(confObj);
      }
      catch (std::string errorMessage)
      {
        Nan::ThrowError(errorMessage.c_str());
        return;
      }

      const int count = c_argument.argc;
      char **argument = c_argument.argv;

      std::vector<std::string> args;
      args.push_back("-command");
      args.push_back(command.c_str());

      for (int j = 0; j < count; j++)
      {
        args.push_back(argument[j]);
      }

      const Classifier *obj = Nan::ObjectWrap::Unwrap<Classifier>(info.Holder());

      const auto worker = new Quantize(args, obj->wrapper_);
      const auto resolver = v8::Promise::Resolver::New(info.GetIsolate());
      worker->SaveToPersistent("key", resolver);
      info.GetReturnValue().Set(resolver->GetPromise());
      Nan::AsyncQueueWorker(worker);
    }
    else
    {
      Nan::ThrowError("Permitted types are ['quantize'].");
      return;
    }
  }
};

#endif
