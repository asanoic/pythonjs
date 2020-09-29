#include <string>
using namespace std;

#include "napi.h"

Napi::Object napiInit(Napi::Env env, Napi::Object exports) {
    return exports;
}

NODE_API_MODULE(NAPI_PYTHONJS, napiInit)
