
#ifndef NN_WORKER_H
#define NN_WORKER_H

#include <napi.h>
#include "wrapper.h"

class NnWorker : public Napi::AsyncWorker
{
public:
  NnWorker(
      std::string query,
      int32_t k,
      Wrapper *wrapper,
      Napi::Promise::Deferred deferred,
      Napi::Function &callback)
      : Napi::AsyncWorker(callback),
        deferred_(deferred),
        query_(query),
        k_(k),
        wrapper_(wrapper),
        result_(){};

  ~NnWorker(){};

  Napi::Promise::Deferred deferred_;

  void Execute();
  void OnOK();
  void OnError(const Napi::Error &e);

private:
  std::string query_;
  int32_t k_;
  Wrapper *wrapper_;
  std::vector<PredictResult> result_;
};

#endif
