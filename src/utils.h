#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

#include "Python.h"
#include "napi.h"

using namespace std;

unordered_map<string, Napi::FunctionReference>& nodeFunctions();

Napi::Value valuePythonToNode(PyObject* value, Napi::Env env);
PyObject* valueNodeToPython(Napi::Value value);

struct PyDecRef {
    void operator()(PyObject* pyObj) const noexcept {
        Py_DecRef(pyObj);
    }
};

using PyPtr = unique_ptr<PyObject, PyDecRef>;

void addFinalizer(Napi::Value value, function<void(void)> func);

#endif // UTILS_H
