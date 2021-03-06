#include <string>

#include "Python.h"

#include "configure.h"
#include "python.js.h"

using namespace std;

Napi::Object pyjsInit(Napi::Env env, Napi::Object exports) {
    configureNodejsModule(exports);

    Napi::Object versions = Napi::Object::New(env);
    const napi_node_version* nodeVersion = Napi::VersionManagement::GetNodeVersion(env);
    versions.Set("node", Napi::String::New(env, to_string(nodeVersion->major) + "." + to_string(nodeVersion->minor)));
    versions.Set("node_napi", Napi::String::New(env, to_string(NAPI_VERSION)));
    versions.Set("python", Napi::String::New(env, PY_VERSION));
    versions.Set("python_capi", Napi::String::New(env, PYTHON_API_STRING));

    exports.Set("versions", versions);
    exports.Set("run", Napi::Function::New(env, pyjsRun));
    exports.Set("import", Napi::Function::New(env, pyjsImport));
    return exports;
}

NODE_API_MODULE(NAPI_PYTHON_JS, pyjsInit)
