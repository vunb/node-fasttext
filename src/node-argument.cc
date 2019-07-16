/**
 * Convert node object arugment to standar argv C or C++ argument
 *
 * Author: Yusuf Syaifudin
 * Date: December 6, 2016 10:57 AM
 *
 */
#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <vector>
#include <algorithm> // for std::find
#include <iterator>  // for std::begin, std::end

#include "node-argument.h"

namespace NodeArgument
{

bool NodeArgument::isOnlyDouble(const char *str)
{
  char *endptr = 0;
  strtod(str, &endptr);
  if (*endptr != '\0' || endptr == str)
  {
    return false;
  }
  return true;
}

/**
   * Concenate string
   */
char *NodeArgument::concat(const char *s1, const char *s2)
{
  char *result = (char *)malloc(strlen(s1) + strlen(s2) + 1); //+1 for the zero-terminator
  //in real code you would check for errors in malloc here
  strcpy(result, s1);
  strcat(result, s2);
  return result;
}

/**
   * Add string to parameter argument
   */
int NodeArgument::AddStringArgument(char ***strings, size_t *count, const char *newStr)
{
  char *copy;
  char **p;

  if (strings == NULL || newStr == NULL ||
      (copy = (char *)malloc(strlen(newStr) + 1)) == NULL)
  {

    return 0;
  }

  strcpy(copy, newStr);

  if ((p = (char **)realloc(*strings, (*count + 1) * sizeof(char *))) == NULL)
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
void NodeArgument::PrintArguments(char **strings, size_t count)
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

CArgument NodeArgument::NapiObjectToCArgument(Napi::Env env, Napi::Object obj)
{
  Napi::HandleScope scope(env);
  Napi::Array props = obj.GetPropertyNames();

  char **arguments = NULL;
  size_t count = 0;

  uint32_t indexLen = 0;
  if (!props.IsEmpty())
  {
    indexLen = props.Length();

    // for validation
    std::string permitted_command[] = {
        "input", "test", "output", "lr", "lrUpdateRate",
        "dim", "ws", "epoch", "minCount", "minCountLabel", "neg",
        "wordNgrams", "loss", "bucket", "minn", "maxn",
        "thread", "t", "label", "verbose", "pretrainedVectors",
        "cutoff", "dsub", "qnorm", "qout", "retrain"};

    for (uint32_t i = 0; i < indexLen; ++i)
    {
      Napi::String key = props.Get(i).As<Napi::String>();

      std::string keyValue = key.Utf8Value();
      char *theKey = (char *)keyValue.c_str();

      bool exists = std::find(std::begin(permitted_command),
                              std::end(permitted_command), keyValue) != std::end(permitted_command);

      if (!exists)
      {
        throw "Unknown argument: " + keyValue;
      }

      Napi::Value value = obj.Get(keyValue);
      NodeArgument::AddStringArgument(&arguments, &count, NodeArgument::concat("-", theKey));

      if (!value.IsBoolean())
      {
        std::string valueValue = value.ToString().Utf8Value();
        // std::cout << "OKKKKK!!!" << keyValue << ": " << valueValue << std::endl;
        char *theValue = (char *)valueValue.c_str();
        NodeArgument::AddStringArgument(&arguments, &count, theValue);
      }
    }
  }

  CArgument response = {count, arguments};
  return response;
}

Napi::Object NodeArgument::mapToNapiObject(Napi::Env env, std::map<std::string, std::string> obj)
{
  Napi::Object result = Napi::Object::New(env);

  for (auto const &iterator : obj)
  {
    Napi::Value value;

    if (isOnlyDouble(iterator.second.c_str()))
    {
      value = Napi::Number::New(env, atof(iterator.second.c_str()));
    }
    else
    {
      value = Napi::String::New(env, iterator.second.c_str());
    }
    result.Set(Napi::String::New(env, iterator.first.c_str()), value);
  }
  return result;
}

} // namespace NodeArgument
