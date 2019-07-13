// #include "node-argument.h"
// #include "train.h"


// void Train::Execute () {
// 	try {
//         result_ = wrapper_->train( args_ );
//     } catch (std::string errorMessage) {
//         SetErrorMessage(errorMessage.c_str());
//     } catch (const char * str) {
//         std::cout << "Exception: " << str << std::endl;
//         SetErrorMessage(str);
//     } catch(const std::exception& e) {
// 	    // Handle exception
//         std::cout << "Exception: " << e.what() << std::endl;
//         SetErrorMessage(e.what());
//     }
// }


// void Train::HandleErrorCallback () {
//     Nan::HandleScope scope;
//     auto res =  GetFromPersistent("key").As<v8::Promise::Resolver>();
//     res->Reject( Nan::GetCurrentContext() , Nan::Error(ErrorMessage()));
//     v8::Isolate::GetCurrent()->RunMicrotasks();
// }

// void Train::HandleOKCallback () {
//     Nan::HandleScope scope;

//     NodeArgument::NodeArgument nodeArg;
//     v8::Local<v8::Object> result = nodeArg.mapToObject( result_ );
//     auto res = GetFromPersistent("key").As<v8::Promise::Resolver>();
//     res->Resolve(result);
//     v8::Isolate::GetCurrent()->RunMicrotasks();
// }
