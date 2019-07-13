#ifndef LOADMODEL_H
#define LOADMODEL_H

#include <napi.h>
#include "wrapper.h"

class LoadModelWorker : public Napi::AsyncWorker {
	public:
		LoadModelWorker(Napi::Function& callback, std::string filename , Wrapper *wrapper) :
			Napi::AsyncWorker(callback) ,
			filename(filename),
			result_(),
			wrapper_(wrapper) {};

		~LoadModelWorker() {};

		void Execute ();
    void OnOK ();

    private:
    	std::string filename;
    	std::map<std::string, std::string> result_;
    	Wrapper *wrapper_;
};

#endif
