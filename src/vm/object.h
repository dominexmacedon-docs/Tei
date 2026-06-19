#ifndef OBJECT_H
#define OBJECT_H

#include "chunk.h"

typedef enum {
    OBJECT_FUNCTION,
    OBJECT_NATIVE
} ObjectType;

typedef struct Object {
    ObjectType type;
    struct Object *next;
} Object;

typedef struct {
    Object object;

    char *name;

    int arity;

    Chunk chunk;
} ObjectFunction;

typedef Value (*NativeFunction)(int argc, Value *argv);

typedef struct {
    Object object;

    char *name;

    int arity;

    NativeFunction function;
} ObjectNative;

ObjectFunction *object_new_function(void);

ObjectNative *object_new_native(
    const char *name,
    int arity,
    NativeFunction function
);

void object_free(Object *object);

#endif
