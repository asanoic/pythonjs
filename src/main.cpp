#include <string>
using namespace std;

#include "Python.h"
#include "napi.h"

#include "configure.h"
#include "python.js.h"

Napi::Object pyjsInit(Napi::Env env, Napi::Object exports) {
    configureNodejsModule();

    Napi::Object versions = Napi::Object::New(env);
    const napi_node_version* nodeVersion = Napi::VersionManagement::GetNodeVersion(env);
    versions.Set("node", Napi::String::New(env, to_string(nodeVersion->major) + "." + to_string(nodeVersion->minor)));
    versions.Set("node_napi", Napi::String::New(env, to_string(NAPI_VERSION)));
    versions.Set("python", Napi::String::New(env, PY_VERSION));
    versions.Set("python_capi", Napi::String::New(env, PYTHON_API_STRING));

    Py_Initialize();
    Napi::External<void> cleaner = Napi::External<void>::New(env, nullptr, bind(Py_FinalizeEx));

    exports.Set("_", cleaner);
    exports.Set("versions", versions);
    exports.Set("run", Napi::Function::New(env, pyjsRun));
    return exports;
}

NODE_API_MODULE(NAPI_PYTHON_JS, pyjsInit)
