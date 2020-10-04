#include "utils.h"

unordered_map<string, Napi::FunctionReference>& nodeFunctions() {
    static unordered_map<string, Napi::FunctionReference> ret;
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

    } else if (strcmp(Py_TYPE(value)->tp_name, "bool") == 0) {
        return Napi::Boolean::New(env, value == Py_True);

    } else if (strcmp(Py_TYPE(value)->tp_name, "tuple") == 0 || strcmp(Py_TYPE(value)->tp_name, "list") == 0) {
        Napi::Array retValue = Napi::Array::New(env);
        PyPtr iter(PyObject_GetIter(value));
        for (PyPtr item(PyIter_Next(iter.get())); item; item.reset(PyIter_Next(iter.get()))) {
            retValue.Set(retValue.Length(), valuePythonToNode(item.get(), env));
        }
        return retValue;

    } else if (strcmp(Py_TYPE(value)->tp_name, "dict") == 0) {
        Napi::Object retValue = Napi::Object::New(env);
        PyPtr list(PyDict_Items(value));
        for (int i = 0, n = PyList_Size(list.get()); i < n; ++i) {
            PyObject* tuple = PyList_GetItem(list.get(), i);
            Napi::Value key = valuePythonToNode(PyTuple_GetItem(tuple, 0), env);
            Napi::Value value = valuePythonToNode(PyTuple_GetItem(tuple, 1), env);
            retValue.Set(key, value);
        }
        return retValue;
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
    } else if (value.IsBoolean()) {
        return PyBool_FromLong(value.As<Napi::Boolean>().Value() ? 1 : 0);

    } else if (value.IsArray()) {
        Napi::Array nodeArr = value.As<Napi::Array>();
        int n = nodeArr.Length();
        PyObject* retValue = PyList_New(n);
        for (int i = 0; i < n; ++i) {
            PyList_SetItem(retValue, i, valueNodeToPython(nodeArr.Get(i)));
        }
        return retValue;

    } else if (value.IsObject()) {
        Napi::Object nodeObj = value.As<Napi::Object>();
        Napi::Array keys = nodeObj.GetPropertyNames();
        PyObject* retValue = PyDict_New();
        for (int i = 0, n = keys.Length(); i < n; ++i) {
            Napi::Value key = keys.Get(i);
            Napi::Value value = nodeObj.Get(key);
            PyDict_SetItem(retValue, valueNodeToPython(key), valueNodeToPython(value));
        }
        return retValue;
    }

    return (Py_IncRef(Py_None), Py_None);
}

void addFinalizer(Napi::Value value, function<void(void)> func) {
    function<void(void)>* f = new function<void(void)>(func);
    static auto callback = [](napi_env e, void* d, void* h) noexcept -> void {
        function<void(void)>* f = reinterpret_cast<function<void(void)>*>(h);
        (*f)();
        delete f;
    };
    napi_add_finalizer(value.Env(), value, nullptr, callback, f, nullptr);
}
