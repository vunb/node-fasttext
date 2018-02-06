/**
 * This is header file to convert node object arugment to standar argv C or C++ argument
 *
 * Author: Yusuf Syaifudin
 * Date: December 6, 2016 10:57 AM
 *
 */

#ifndef NODEARGUMENT_NODEARGUMENT_H
#define NODEARGUMENT_NODEARGUMENT_H

#include <v8.h>
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <map>
#include <algorithm>    // for std::find
#include <iterator>     // for std::begin, std::end

namespace NodeArgument
{
    struct CArgument
    {
      size_t argc;
      char** argv;
    };

    class NodeArgument
    {

    public:
        char* concat(const char *s1, const char *s2);
        int AddStringArgument(char*** strings, size_t* count, const char* newStr);
        void PrintArguments(char** strings, size_t count);
        bool isOnlyDouble(const char* str);
        CArgument ObjectToCArgument(v8::Local<v8::Object> obj);
        std::vector<std::string> ObjectToArrayString(v8::Local<v8::Object> obj);
        v8::Local<v8::Object>  mapToObject(std::map<std::string, std::string> obj);
    };
}

#endif
