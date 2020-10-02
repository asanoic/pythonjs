#include <string>
using namespace std;

#include "python.js.h"
#include "utils.h"

Napi::Value pyjsRun(const Napi::CallbackInfo& info) {
    int argLen = info.Length();
    Napi::Env env = info.Env();
    if (argLen == 0 || argLen > 2) {
        Napi::TypeError::New(env, "Wrong number of arguments, should be 1 or 2 arguments.").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "First argument should be string, which is python code.").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    nodeFunctions().clear();

    if (!info[1].IsUndefined()) {
        if (!info[1].IsArray()) {
            Napi::TypeError::New(env, "Second optional argument should be array of function info [{name, func}].").ThrowAsJavaScriptException();
            return env.Undefined();
        }
        Napi::Array funcArr = info[1].As<Napi::Array>();
        int funcLen = funcArr.Length();
        for (int i = 0, n = funcArr.Length(); i < n; ++i) {
            Napi::Object funcInfo = funcArr.Get(i).As<Napi::Object>();
            string name = funcInfo.Get("name").As<Napi::String>().ToString();
            Napi::Function func = funcInfo.Get("func").As<Napi::Function>();
            nodeFunctions()[name] = func;
        }
    }

    string pythonCode = info[0].As<Napi::String>().ToString();
    int exitCode = PyRun_SimpleStringFlags(pythonCode.data(), nullptr);
    return Napi::Number::New(env, exitCode);
}
