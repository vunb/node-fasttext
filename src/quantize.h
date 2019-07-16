#ifndef QUANTIZE_H
#define QUANTIZE_H

#include <napi.h>
#include "wrapper.h"

class QuantizeWorker : public Napi::AsyncWorker
{
public:
  QuantizeWorker(
      const std::vector<std::string> args,
      Wrapper *wrapper,
      Napi::Promise::Deferred deferred,
      Napi::Function &callback)
      : Napi::AsyncWorker(callback),
        deferred_(deferred),
        args_(args),
        wrapper_(wrapper),
        result_(){};

  ~QuantizeWorker(){};

  Napi::Promise::Deferred deferred_;

  void Execute();
  void OnOK();
  void OnError(const Napi::Error &e);

private:
  const std::vector<std::string> args_;
  Wrapper *wrapper_;
  std::map<std::string, std::string> result_;
};

#endif
