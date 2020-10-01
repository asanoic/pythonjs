#include "utils.h"

unordered_map<string, Napi::Function>& nodeFunctions() {
    static unordered_map<string, Napi::Function> ret;
    return ret;
}


Napi::Value valuePythonToNode(PyObject* value, Napi::Env env) {
    if (strcmp(Py_TYPE(value)->tp_name, "str") == 0) {
        PyPtr unicode(PyUnicode_AsUTF8String(value));
        string retValue = PyBytes_AS_STRING(unicode.get());
        return Napi::String::New(env, retValue);

    } else if (strcmp(Py_TYPE(value)->tp_name, "int") == 0) {
        long retValue = PyLong_AsLong(value);
        return Napi::Number::New(env, retValue);

    } else if (strcmp(Py_TYPE(value)->tp_name, "float") == 0) {
        double retValue = PyFloat_AsDouble(value);
        return Napi::Number::New(env, retValue);
    }

    return env.Undefined();
}

PyObject* valueNodeToPython(Napi::Value value) {
    if (value.IsString()) {
        return PyUnicode_FromString(value.As<Napi::String>().Utf8Value().data());

    } else if (value.IsNumber()) {
        int retValue = value.As<Napi::Number>().DoubleValue();
        if ((int)retValue == retValue) {
            return PyLong_FromLongLong(value.As<Napi::Number>().Int64Value());
        } else {
            return PyFloat_FromDouble(value.As<Napi::Number>().DoubleValue());
        }
    }

    return Py_None;
}
