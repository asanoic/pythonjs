#ifndef PYTHON_JS_H
#define PYTHON_JS_H

#include "napi.h"

Napi::Value pyjsRun(const Napi::CallbackInfo& info);

#endif // PYTHON_JS_H
