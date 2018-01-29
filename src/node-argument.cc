/**
 * Convert node object arugment to standar argv C or C++ argument
 *
 * Author: Yusuf Syaifudin
 * Date: December 6, 2016 10:57 AM
 *
 */
#include <node.h>
#include <v8.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <algorithm>    // for std::find
#include <iterator>     // for std::begin, std::end

#include "node-argument.h"

namespace NodeArgument
{

  /**
   * Concenate string
   */
  char* NodeArgument::concat(const char *s1, const char *s2)
  {
      char *result = (char*)malloc(strlen(s1)+strlen(s2)+1);//+1 for the zero-terminator
      //in real code you would check for errors in malloc here
      strcpy(result, s1);
      strcat(result, s2);
      return result;
  }

  /**
   * Add string to parameter argument
   */
  int NodeArgument::AddStringArgument(char*** strings, size_t* count, const char* newStr)
  {
    char* copy;
    char** p;
   
    if (strings == NULL || newStr == NULL || 
      (copy = (char*)malloc(strlen(newStr) + 1)) == NULL) 
    {

      return 0;
    }
   
    strcpy(copy, newStr);
   
    if ((p = (char**) realloc(*strings, (*count + 1) * sizeof(char*))) == NULL)
    {
      free(copy);
      return 0;
    }
   
    *strings = p;
   
    (*strings)[(*count)++] = copy;
   
    return 1;
  }
   
  /**
   * Print all argument value
   */
  void NodeArgument::PrintArguments(char** strings, size_t count)
  {
    printf("BEGIN\n");
    if (strings != NULL)
    {
      while (count--)
      {
        printf("  %s\n", *strings++);
      }
    }
    printf("END\n");
  }

  /**
   * Convert Node V8 Object to char** argument (argv) in C/C++
   */
  CArgument NodeArgument::ObjectToCArgument(v8::Local<v8::Object> obj) 
  {
    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);

    v8::Local<v8::Context> context = v8::Context::New(isolate);
    v8::MaybeLocal<v8::Array> maybe_props = obj->GetOwnPropertyNames(context);

    char** arguments = NULL;
    size_t count = 0;

    uint32_t indexLen = 0;
    if (!maybe_props.IsEmpty()) 
    {
      v8::Local<v8::Array> props = maybe_props.ToLocalChecked();
      indexLen = props->Length();

      // for validation
      std::string permitted_command[] = {
        "input", "test", "output", "lr", "lrUpdateRate", 
        "dim", "ws", "epoch", "minCount", "minCountLabel", "neg",
        "wordNgrams", "loss", "bucket", "minn", "maxn",
        "thread", "t", "label", "verbose", "pretrainedVectors"
      };
      
      for (uint32_t i = 0; i < indexLen; ++i) {
        v8::Local<v8::Value> key = props->Get(i);
        const v8::String::Utf8Value utf8_key(key);

        std::string keyValue = std::string(*utf8_key);
        char *theKey = (char *)keyValue.c_str();

        bool exists = std::find(std::begin(permitted_command), 
          std::end(permitted_command), keyValue) != std::end(permitted_command);

        if(!exists)
        {
          throw "Unknown argument: " + keyValue;
        }

        v8::Local<v8::Value> value = obj->Get(v8::String::NewFromUtf8(isolate, theKey));
        v8::String::Utf8Value utf8_value(value->ToString());

        std::string valueValue = std::string(*utf8_value);
        char *theValue = (char *)valueValue.c_str();

        NodeArgument::AddStringArgument(&arguments, &count, NodeArgument::concat("-", theKey));
        NodeArgument::AddStringArgument(&arguments, &count, theValue);

      }
    }

    CArgument response = { count, arguments };
    return response;
  } 


  /**
   * Local Object to fixed array
   */
  std::vector<std::string> NodeArgument::ObjectToArrayString(v8::Local<v8::Object> obj)
  {

    v8::Isolate* isolate = v8::Isolate::GetCurrent();
    v8::HandleScope scope(isolate);

    v8::Local<v8::Context> context = v8::Context::New(isolate);
    v8::MaybeLocal<v8::Array> maybe_props = obj->GetOwnPropertyNames(context);
    v8::Local<v8::Array> props = maybe_props.ToLocalChecked();

    uint32_t indexLen = props->Length();
    std::vector<std::string> v; // initialize

    for (uint32_t i = 0; i < indexLen; ++i) {
      v8::Local<v8::Value> key = props->Get(i);
      const v8::String::Utf8Value utf8_key(key);

      std::string keyValue = std::string(*utf8_key);
      char *theKey = (char *)keyValue.c_str();

      v8::Local<v8::Value> value = obj->Get(v8::String::NewFromUtf8(isolate, theKey));
      v8::String::Utf8Value utf8_value(value->ToString());

      std::string valueValue = std::string(*utf8_value);
        // char *theValue = (char *)valueValue.c_str();
      v.push_back(valueValue);
    }

    return v;
}

}