#include "configure.h"

#include "utils.h"

void configureNodejsModule(Napi::Value jsModule) {
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
                         nodeArgs.push_back(valuePythonToNode(item.get(), env));
                     }
                 }
                 Napi::Value nodeResult = nodeFunctions()[name].Call(nodeArgs);
                 return valueNodeToPython(nodeResult);
             },
             METH_VARARGS, "Dynamic call nodejs context function."},
            {nullptr, nullptr, 0, nullptr}};

        static PyModuleDef nodejsModule = {
            PyModuleDef_HEAD_INIT, "nodejs", nullptr, -1, nodejsMethod,
            nullptr, nullptr, nullptr, nullptr};

        return PyModule_Create(&nodejsModule);
    });

    Py_Initialize();
    addFinalizer(jsModule, bind(Py_FinalizeEx));
    addFinalizer(jsModule, clearNodeFunctions);

    // add current folder to search folder
    PyRun_SimpleStringFlags("import sys\nsys.path.append('.')", nullptr);

}

void addFunctionsToNodejsModule(Napi::Array funcArr) {
    int funcLen = funcArr.Length();
    for (int i = 0, n = funcArr.Length(); i < n; ++i) {
        Napi::Object funcInfo = funcArr.Get(i).As<Napi::Object>();
        string name = funcInfo.Get("name").As<Napi::String>().ToString();
        Napi::Function func = funcInfo.Get("func").As<Napi::Function>();
        nodeFunctions()[name] = Napi::Persistent(func);
        nodeFunctions()[name].SuppressDestruct();
    }
}

void clearNodeFunctions() {
    nodeFunctions().clear();
}
