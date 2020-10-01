#ifndef UTILS_H
#define UTILS_H

#include <memory>
#include <vector>
#include <string>
#include <unordered_map>

#include "napi.h"
#include "Python.h"

using namespace std;


unordered_map<string, Napi::Function>& nodeFunctions();

Napi::Value valuePythonToNode(PyObject* value, Napi::Env env);
PyObject* valueNodeToPython(Napi::Value value);

struct PyDecRef {
    void operator()(PyObject* pyObj) const noexcept {
        Py_DecRef(pyObj);
    }
};

using PyPtr = unique_ptr<PyObject, PyDecRef>;


#endif // UTILS_H
