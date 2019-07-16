
#ifndef PREDICT_WORKER_H
#define PREDICT_WORKER_H

#include <napi.h>
#include "wrapper.h"

class PredictWorker : public Napi::AsyncWorker
{
public:
  PredictWorker(
      std::string sentence,
      int32_t k,
      Wrapper *wrapper,
      Napi::Promise::Deferred deferred,
      Napi::Function &callback)
      : Napi::AsyncWorker(callback),
        deferred_(deferred),
        sentence_(sentence),
        wrapper_(wrapper),
        result_(),
        k_(k){};

  ~PredictWorker(){};

  Napi::Promise::Deferred deferred_;

  void Execute();
  void OnOK();
  void OnError(const Napi::Error &e);

private:
  std::string sentence_;
  Wrapper *wrapper_;
  std::vector<PredictResult> result_;
  int32_t k_;
};

#endif
