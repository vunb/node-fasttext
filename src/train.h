#ifndef TRAIN_WORKER_H
#define TRAIN_WORKER_H

#include <napi.h>
#include "wrapper.h"

class TrainWorker : public Napi::AsyncWorker
{
public:
  TrainWorker(const std::vector<std::string> args, Wrapper *wrapper, Napi::Promise::Deferred deferred, Napi::Function &callback)
      : Napi::AsyncWorker(callback),
        args_(args),
        wrapper_(wrapper),
        deferred_(deferred),
        result_(){};

  ~TrainWorker(){};

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
