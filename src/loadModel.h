#ifndef LOADMODEL_H
#define LOADMODEL_H

#include <napi.h>
#include "wrapper.h"

class LoadModelWorker : public Napi::AsyncWorker
{
public:
  LoadModelWorker(std::string filename, Wrapper *wrapper, Napi::Promise::Deferred deferred, Napi::Function &callback)
      : Napi::AsyncWorker(callback),
        filename(filename),
        result_(),
        wrapper_(wrapper),
        deferred_(deferred){};

  ~LoadModelWorker(){};

  Napi::Promise::Deferred deferred_;
  void Execute();
  void OnOK();
  void OnError();

private:
  std::string filename;
  std::map<std::string, std::string> result_;
  Wrapper *wrapper_;
};

#endif
