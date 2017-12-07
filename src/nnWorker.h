
#ifndef NN_WORKER_H
#define NN_WORKER_H

#include <nan.h>
#include "wrapper.h"

class NnWorker : public Nan::AsyncWorker {
    public:
        NnWorker (Nan::Callback *callback, std::string query, int32_t k, Wrapper *wrapper)
            : Nan::AsyncWorker(callback),
                query_(query),
                k_(k),
                wrapper_(wrapper),
                result_() {};

        ~NnWorker () {};

        void Execute ();
        void HandleOKCallback ();
        void HandleErrorCallback ();

    private:
        std::string query_;
        int32_t k_;
        Wrapper *wrapper_;
        std::vector<PredictResult> result_;
};

#endif