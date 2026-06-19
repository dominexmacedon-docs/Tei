#ifndef NATIVE_H
#define NATIVE_H

#include <stddef.h>

#include "object.h"

#define NATIVE_MAX 256

typedef struct {
    ObjectNative *functions[NATIVE_MAX];
    size_t count;
} NativeRegistry;

void native_init(NativeRegistry *registry);

void native_free(NativeRegistry *registry);

int native_register(
    NativeRegistry *registry,
    const char *name,
    int arity,
    NativeFunction function
);

ObjectNative *native_find(
    NativeRegistry *registry,
    const char *name
);

Value native_show(int argc, Value *argv);

#endif
