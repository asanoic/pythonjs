#include "python.js.h"

#include "configure.h"
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
        addFunctionsToNodejsModule(info[1].As<Napi::Array>());
    }

    string pythonCode = info[0].As<Napi::String>().ToString();
    int exitCode = PyRun_SimpleStringFlags(pythonCode.data(), nullptr);
    return Napi::Number::New(env, exitCode);
}
