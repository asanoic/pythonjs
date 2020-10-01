#ifndef PYTHONCONFIGURE_H
#define PYTHONCONFIGURE_H

#include "Python.h"

#include <unordered_map>
#include "napi.h"

using namespace std;

unordered_map<string, Napi::Function>& nodeFunctions();

void configureNodejsModule();

#endif // PYTHONCONFIGURE_H
