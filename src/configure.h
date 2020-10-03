#ifndef CONFIGURE_H
#define CONFIGURE_H

#include "napi.h"

void configureNodejsModule();

void addFunctionsToNodejsModule(Napi::Array funcArr);

void clearNodeFunctions();

#endif // CONFIGURE_H
