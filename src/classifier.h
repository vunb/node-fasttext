// classifier.h
#ifndef CLASSIFIER_H
#define CLASSIFIER_H

#include <node.h>
#include <node_object_wrap.h>
#include "wrapper.h"
#include "node-argument.h"
#include "classifierWorker.h"
#include "loadModel.h"

class Classifier : public Nan::ObjectWrap {
    public:
        static NAN_MODULE_INIT(Init) {
            v8::Local<v8::FunctionTemplate> tpl = Nan::New<v8::FunctionTemplate>(New);
            tpl->SetClassName(Nan::New("Classifier").ToLocalChecked());
            tpl->InstanceTemplate()->SetInternalFieldCount(1);

            Nan::SetPrototypeMethod(tpl, "predict", Predict);
            Nan::SetPrototypeMethod(tpl, "loadModel", loadModel);

            constructor().Reset(Nan::GetFunction(tpl).ToLocalChecked());
            Nan::Set(target, Nan::New("Classifier").ToLocalChecked(),
            Nan::GetFunction(tpl).ToLocalChecked());
        }

    private:
        explicit Classifier(std::string modelFilename) :
            wrapper_(new Wrapper(modelFilename))
            {}

        ~Classifier() {}

        static NAN_METHOD(New) {
            if (info.IsConstructCall()) {
                std::string command;
                if (!info[0]->IsString()) {
                     command = "model.bin";
                } else {
                    v8::String::Utf8Value commandArg(info[0]->ToString());
                    command = std::string(*commandArg);
                }


                Classifier *obj = new Classifier(command);
                obj->Wrap(info.This());
                info.GetReturnValue().Set(info.This());
            } else {
                const int argc = 1;
                v8::Local<v8::Value> argv[argc] = {info[0]};
                v8::Local<v8::Function> cons = Nan::New(constructor());
                info.GetReturnValue().Set(Nan::NewInstance(cons, argc, argv).ToLocalChecked());
            }
        }

        static NAN_METHOD(loadModel) {

        	if (!info[0]->IsString()) {
                Nan::ThrowError("model file path must be a string");
                return;
            } 

        	v8::String::Utf8Value modelArg(info[0]->ToString());
        	std::string filename = std::string(*modelArg);
        	Classifier* obj = Nan::ObjectWrap::Unwrap<Classifier>( info.Holder() );

            auto worker = new LoadModel(filename, obj->wrapper_);
            auto resolver = v8::Promise::Resolver::New( info.GetIsolate());
            worker->SaveToPersistent("key",resolver);
            info.GetReturnValue().Set(resolver->GetPromise());
            Nan::AsyncQueueWorker( worker );
        }

        static NAN_METHOD(Predict) {
            if (!info[0]->IsString()) {
                Nan::ThrowError("sentence must be a string");
                return;
            }

            if (!info[1]->IsUint32()) {
                Nan::ThrowError("k must be a number");
                return;
            }

            if (!info[2]->IsFunction()) {
                Nan::ThrowError("callback must be a function");
                return;
            }

            int32_t k = info[1]->Uint32Value();
            v8::String::Utf8Value sentenceArg(info[0]->ToString());
            std::string sentence = std::string(*sentenceArg);
            Nan::Callback *callback = new Nan::Callback(info[2].As<v8::Function>());

            Classifier* obj = Nan::ObjectWrap::Unwrap<Classifier>(info.Holder());

            Nan::AsyncQueueWorker(new ClassifierWorker(callback, sentence, k, obj->wrapper_));
        }

        static inline Nan::Persistent<v8::Function> & constructor() {
            static Nan::Persistent<v8::Function> my_constructor;
            return my_constructor;
        }

        Wrapper* wrapper_;
    };

#endif