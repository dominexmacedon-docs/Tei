#include "object.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *copy_string(const char *text) {
    if (text == NULL) {
        return NULL;
    }

    size_t length = strlen(text);

    char *copy = (char *)malloc(length + 1);

    if (copy == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(EXIT_FAILURE);
    }

    memcpy(copy, text, length + 1);

    return copy;
}

ObjectFunction *object_new_function(void) {
    ObjectFunction *function =
        (ObjectFunction *)malloc(sizeof(ObjectFunction));

    if (function == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(EXIT_FAILURE);
    }

    function->object.type = OBJECT_FUNCTION;
    function->object.next = NULL;

    function->name = NULL;
    function->arity = 0;

    chunk_init(&function->chunk);

    return function;
}

ObjectNative *object_new_native(
    const char *name,
    int arity,
    NativeFunction function
) {
    ObjectNative *native =
        (ObjectNative *)malloc(sizeof(ObjectNative));

    if (native == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(EXIT_FAILURE);
    }

    native->object.type = OBJECT_NATIVE;
    native->object.next = NULL;

    native->name = copy_string(name);
    native->arity = arity;
    native->function = function;

    return native;
}

void object_free(Object *object) {
    if (object == NULL) {
        return;
    }

    switch (object->type) {

        case OBJECT_FUNCTION: {
            ObjectFunction *function =
                (ObjectFunction *)object;

            if (function->name != NULL) {
                free(function->name);
            }

            chunk_free(&function->chunk);

            free(function);

            break;
        }

        case OBJECT_NATIVE: {
            ObjectNative *native =
                (ObjectNative *)object;

            if (native->name != NULL) {
                free(native->name);
            }

            free(native);

            break;
        }
    }
}
