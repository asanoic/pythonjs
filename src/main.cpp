#include <string>
using namespace std;

#include "Python.h"
#include "napi.h"

Napi::Object napiInit(Napi::Env env, Napi::Object exports) {
    PyImport_AppendInittab("emb", []() -> PyObject* {
        static PyMethodDef EmbMethods[] = {
            {"numargs",
             [](PyObject* self, PyObject* args) -> PyObject* {
                 int v  = 0, m = 0;
                 int len = PyObject_Size(args);
//                 PyObject* itr = PyObject_GetIter(args);
//                 for (PyObject* item = PyIter_Next(itr); item; item = PyIter_Next(itr)) {
//                     int ttt = PyObject_Size(item);
//                 }
                 if (!PyArg_ParseTuple(args, "ii", &v, &m))
                     return NULL;
                 return PyLong_FromLong(v * m);
             },
             METH_VARARGS,
             "Return the number of arguments received by the process."},
            {NULL, NULL, 0, NULL}};

        static PyModuleDef EmbModule = {
            PyModuleDef_HEAD_INIT, "emb", NULL, -1, EmbMethods,
            NULL, NULL, NULL, NULL};

        return PyModule_Create(&EmbModule);
    });
    Py_Initialize();
    Napi::Object versions = Napi::Object::New(env);
    const napi_node_version* nodeVersion = Napi::VersionManagement::GetNodeVersion(env);
    versions.Set("node", Napi::String::New(env, to_string(nodeVersion->major) + "." + to_string(nodeVersion->minor)));
    versions.Set("node_napi", Napi::String::New(env, to_string(NAPI_VERSION)));
    versions.Set("python", Napi::String::New(env, PY_VERSION));
    versions.Set("python_capi", Napi::String::New(env, PYTHON_API_STRING));
    exports.Set("versions", versions);
    PyRun_SimpleStringFlags(
        "from time import time,ctime\n"
        "from emb import numargs\n"
        "print('Today is', ctime(time()))\n"
        "print('Number of arguments', numargs(42, 100))",
        nullptr);

    Py_FinalizeEx();
    return exports;
}

NODE_API_MODULE(NAPI_PYTHON_JS, napiInit)
