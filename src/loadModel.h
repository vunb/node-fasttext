#ifndef LOADMODEL_H
#define LOADMODEL_H

#include <napi.h>
#include "wrapper.h"

class LoadModelWorker : public Napi::AsyncWorker
{
public:
  LoadModelWorker(std::string filename, Wrapper *wrapper, Napi::Function &callback, Napi::Promise::Deferred deferred)
      : Napi::AsyncWorker(callback),
        filename(filename),
        result_(),
        wrapper_(wrapper),
        defferred_(deferred){};

  ~LoadModelWorker(){};

  Napi::Promise::Deferred defferred_;
  void Execute();
  void OnOK();
  void OnError();

private:
  std::string filename;
  std::map<std::string, std::string> result_;
  Wrapper *wrapper_;
};

#endif
