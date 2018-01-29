#ifndef LOADMODEL_H
#define LOADMODEL_H

#include <nan.h>
#include "wrapper.h"

class LoadModel : public Nan::AsyncWorker {
	public:
		LoadModel(std::string filename , Wrapper *wrapper) : 
			Nan::AsyncWorker(new Nan::Callback()) , 
			filename(filename),
			result_(),
			wrapper_(wrapper) {};

		~LoadModel() {};

		void Execute ();
        void HandleOKCallback ();
        void HandleErrorCallback ();

    private: 
    	std::string filename;
    	std::map<std::string, std::string> result_;
    	Wrapper *wrapper_;
};

#endif
