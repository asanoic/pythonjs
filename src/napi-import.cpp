#include "python.js.h"

#include "utils.h"
#include "configure.h"

Napi::Value pyjsImport(const Napi::CallbackInfo& info) {
    Napi::Env env = info.Env();

    int argLen = info.Length();
    if (argLen == 0 || argLen > 2) {
        Napi::TypeError::New(env, "Wrong number of arguments, should be 1 or 2 arguments.").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    if (!info[0].IsString()) {
        Napi::TypeError::New(env, "First argument should be string, which is python code.").ThrowAsJavaScriptException();
        return env.Undefined();
    }

    if (!info[1].IsUndefined()) {
        if (!info[1].IsArray()) {
            Napi::TypeError::New(env, "Second optional argument should be array of function info [{name, func}].").ThrowAsJavaScriptException();
            return env.Undefined();
        }
        addFunctionsToNodejsModule(info[1].As<Napi::Array>());
    }

    string moduleName = info[0].As<Napi::String>().Utf8Value();
    PyPtr pyModuleName(PyUnicode_DecodeFSDefault(moduleName.data()));
    PyObject* pyModule = PyImport_Import(pyModuleName.get());
    Napi::External<PyObject> handle = Napi::External<PyObject>::New(env, pyModule, bind(Py_DecRef, placeholders::_2));
    Napi::Object module = Napi::Object::New(env);
    module.Set("_handle", handle);
    module.Set("call", Napi::Function::New(env, pyjsCall, "call", pyModule));

    return module;
}
