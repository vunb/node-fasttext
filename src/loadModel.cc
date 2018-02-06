#include "loadModel.h"
#include "node-argument.h"


void LoadModel::Execute () {
	try {
        result_ = wrapper_->loadModel( filename );
    } catch (std::string errorMessage) {
        SetErrorMessage(errorMessage.c_str());
    }
}

void LoadModel::HandleErrorCallback () {
    Nan::HandleScope scope;
    auto res = GetFromPersistent("key").As<v8::Promise::Resolver>();
    res->Reject( Nan::GetCurrentContext() , Nan::Error(ErrorMessage()));
    v8::Isolate::GetCurrent()->RunMicrotasks();
}

void LoadModel::HandleOKCallback () {
	
    Nan::HandleScope scope;
    NodeArgument::NodeArgument nodeArg;
    v8::Local<v8::Object> result = nodeArg.mapToObject( result_ );

    auto res = GetFromPersistent("key").As<v8::Promise::Resolver>();
    res->Resolve( Nan::GetCurrentContext() , result);
    v8::Isolate::GetCurrent()->RunMicrotasks();
}