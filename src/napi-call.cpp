#include "python.js.h"

#include "utils.h"

Napi::Value pyjsCall(const Napi::CallbackInfo& info) {
    int argLen = info.Length();
    Napi::Env env = info.Env();
    if (argLen == 0) {
        Napi::TypeError::New(env, "Wrong number of arguments, first argument should be python function name.").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "First argument should be string, which is python code.").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    PyObject* pyModule = reinterpret_cast<PyObject*>(info.Data());
    string funcName = info[0].As<Napi::String>().Utf8Value();
    PyPtr pyFunc(PyObject_GetAttrString(pyModule, funcName.data()));
    if (!pyFunc || !PyCallable_Check(pyFunc.get())) {
        Napi::TypeError::New(env, funcName + " is not a python function").ThrowAsJavaScriptException();
        return env.Undefined();
    }
    PyPtr pyArgs(PyTuple_New(argLen - 1));
    for (int i = 1; i < argLen; ++i) {
        PyTuple_SetItem(pyArgs.get(), i - 1, valueNodeToPython(info[i]));
    }

    PyPtr pyRet(PyObject_CallObject(pyFunc.get(), pyArgs.get()));

    return valuePythonToNode(pyRet.get(), env);

}
