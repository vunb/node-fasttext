#ifndef QUANTIZE_H
#define QUANTIZE_H

#include <nan.h>
#include "wrapper.h"

class Quantize : public Nan::AsyncWorker {
  public:
    Quantize(
        const std::vector<std::string> args,
        Wrapper *wrapper
    ): Nan::AsyncWorker(new Nan::Callback()), args_(args), wrapper_(wrapper), result_(){};

    ~Quantize(){};

    void Execute();
    void HandleOKCallback();
    void HandleErrorCallback();

  private:
    const std::vector<std::string> args_;
    Wrapper *wrapper_;
    std::map<std::string, std::string> result_;
};

#endif
