#include "native.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void native_init(NativeRegistry *registry) {
    registry->count = 0;
}

void native_free(NativeRegistry *registry) {
    size_t i;

    for (i = 0; i < registry->count; i++) {
        object_free((Object *)registry->functions[i]);
    }

    registry->count = 0;
}

int native_register(
    NativeRegistry *registry,
    const char *name,
    int arity,
    NativeFunction function
) {
    if (registry->count >= NATIVE_MAX) {
        return 0;
    }

    registry->functions[registry->count++] =
        object_new_native(name, arity, function);

    return 1;
}

ObjectNative *native_find(
    NativeRegistry *registry,
    const char *name
) {
    size_t i;

    for (i = 0; i < registry->count; i++) {
        if (strcmp(registry->functions[i]->name, name) == 0) {
            return registry->functions[i];
        }
    }

    return NULL;
}

Value native_show(int argc, Value *argv) {
    int i;

    for (i = 0; i < argc; i++) {
        value_print(argv[i]);

        if (i + 1 < argc) {
            printf(" ");
        }
    }

    printf("\n");

    return value_null();
}
