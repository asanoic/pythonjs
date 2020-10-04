#ifndef CONFIGURE_H
#define CONFIGURE_H

#include "napi.h"

void configureNodejsModule(Napi::Value jsModule);

void addFunctionsToNodejsModule(Napi::Array funcArr);

#endif // CONFIGURE_H
