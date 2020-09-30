#include <string>
using namespace std;

#include "Python.h"
#include "napi.h"

Napi::Object napiInit(Napi::Env env, Napi::Object exports) {
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
        "print('Today is', ctime(time()))\n",
        nullptr);

    Py_FinalizeEx();
    return exports;
}

NODE_API_MODULE(NAPI_PYTHON_JS, napiInit)
