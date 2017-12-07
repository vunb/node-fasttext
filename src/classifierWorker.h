
#ifndef CLASSIFIER_WORKER_H
#define CLASSIFIER_WORKER_H

#include <nan.h>
#include "wrapper.h"

class ClassifierWorker : public Nan::AsyncWorker {
    public:
        ClassifierWorker (Nan::Callback *callback, std::string sentence, int32_t k, Wrapper *wrapper)
            : Nan::AsyncWorker(callback),
                sentence_(sentence),
                wrapper_(wrapper),
                result_(),
                k_(k) {};

        ~ClassifierWorker () {};

        void Execute ();
        void HandleOKCallback ();
        void HandleErrorCallback ();

    private:
        std::string sentence_;
        Wrapper *wrapper_;
        std::vector<PredictResult> result_;
        int32_t k_;
};

#endif