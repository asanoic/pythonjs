#include "python-configure.h"

#include <memory>
#include <vector>

using namespace std;

#include "napi.h"

unordered_map<string, Napi::Function>& nodeFunctions() {
    static unordered_map<string, Napi::Function> ret;
    return ret;
}

struct PyDecRef {
    void operator()(PyObject* pyObj) const noexcept {
        Py_DecRef(pyObj);
    }
};

using PyPtr = unique_ptr<PyObject, PyDecRef>;

void configureNodejsModule() {
    PyImport_AppendInittab("nodejs", []() -> PyObject* {
        static PyMethodDef nodejsMethod[] = {
            {"call",
             [](PyObject* self, PyObject* args) -> PyObject* {
                 PyPtr iter(PyObject_GetIter(args));
                 string name;
                 napi_env env = nullptr;
                 vector<napi_value> nodeArgs;
                 for (PyPtr item(PyIter_Next(iter.get())); item; item.reset(PyIter_Next(iter.get()))) {
                     if (name.empty()) {
                         PyPtr unicode(PyUnicode_AsUTF8String(item.get()));
                         name = PyBytes_AS_STRING(unicode.get());
                         if (nodeFunctions().count(name) == 0) return Py_None;
                         env = nodeFunctions()[name].Env();
                     } else {
                         if (strcmp(Py_TYPE(item.get())->tp_name, "str") == 0) {
                             PyPtr unicode(PyUnicode_AsUTF8String(item.get()));
                             string value = PyBytes_AS_STRING(unicode.get());
                             nodeArgs.push_back(Napi::String::New(env, value));
                         } else if (strcmp(Py_TYPE(item.get())->tp_name, "int") == 0) {
                             long value = PyLong_AsLong(item.get());
                             nodeArgs.push_back(Napi::Number::New(env, value));
                         }
                     }
                 }
                 Napi::Value nodeResult = nodeFunctions()[name].Call(nodeArgs);
                 return PyLong_FromLong(nodeResult.As<Napi::Number>().Int32Value());
             },
             METH_VARARGS, "Dynamic call nodejs context function."},
            {NULL, NULL, 0, NULL}};

        static PyModuleDef nodejsModule = {
            PyModuleDef_HEAD_INIT, "nodejs", NULL, -1, nodejsMethod,
            NULL, NULL, NULL, NULL};

        return PyModule_Create(&nodejsModule);
    });
}
