#ifndef TRAIN_H
#define TRAIN_H

#include <nan.h>
#include "wrapper.h"

class Train : public Nan::AsyncWorker {
	public:
		Train(const std::vector<std::string> args, Wrapper *wrapper) : 
			Nan::AsyncWorker(new Nan::Callback()) , 
			args_(args),
			wrapper_(wrapper),
			result_() {};

		~Train() {};

		void Execute ();
        void HandleOKCallback ();
        void HandleErrorCallback ();

    private: 
    	const std::vector<std::string> args_;
    	Wrapper *wrapper_;
    	std::map<std::string, std::string> result_;
};

#endif