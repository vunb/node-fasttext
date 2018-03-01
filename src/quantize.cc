#include "node-argument.h"
#include "quantize.h"


void Quantize::Execute () {
	try {
        result_ = wrapper_->quantize( args_ );
    } catch (std::string errorMessage) {
        SetErrorMessage(errorMessage.c_str());
    } catch (const char * str) {
        std::cout << "Exception: " << str << std::endl;
        SetErrorMessage(str);
    } catch(const std::exception& e) {
	    // Handle exception
        std::cout << "Exception: " << e.what() << std::endl;
        SetErrorMessage(e.what());
    }
}

void Quantize::HandleErrorCallback() {
    Nan::HandleScope scope;
    const auto res = GetFromPersistent("key").As<v8::Promise::Resolver>();
    (void)res->Reject(Nan::GetCurrentContext(), Nan::Error(ErrorMessage()));
    v8::Isolate::GetCurrent()->RunMicrotasks();
}

void Quantize::HandleOKCallback() {
    Nan::HandleScope scope;

    NodeArgument::NodeArgument nodeArg;
    const v8::Local<v8::Object> result = nodeArg.mapToObject(result_);
    const auto res = GetFromPersistent("key").As<v8::Promise::Resolver>();
    (void)res->Resolve(result);
    v8::Isolate::GetCurrent()->RunMicrotasks();
}
