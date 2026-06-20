#include "value.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *copy_string(const char *text) {
    size_t length = strlen(text);

    char *copy = (char *)malloc(length + 1);

    if (!copy) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    memcpy(copy, text, length + 1);
    return copy;
}

Value value_null(void) {
    Value v;
    v.type = VALUE_NULL;
    return v;
}

Value value_bool(int b) {
    Value v;
    v.type = VALUE_BOOL;
    v.as.boolean = b ? 1 : 0;
    return v;
}

Value value_number(double n) {
    Value v;
    v.type = VALUE_NUMBER;
    v.as.number = n;
    return v;
}

Value value_string(const char *s) {
    Value v;
    v.type = VALUE_STRING;
    v.as.string = copy_string(s);
    return v;
}

Value value_object(Object *object) {
    Value v;
    v.type = VALUE_OBJECT;
    v.as.object = object;
    return v;
}

void value_free(Value *v) {
    if (!v) {
        return;
    }

    if (v->type == VALUE_STRING) {
        free(v->as.string);
        v->as.string = NULL;
    }

    if (v->type == VALUE_OBJECT) {
        v->as.object = NULL;
    }

    v->type = VALUE_NULL;
}

Value value_copy(Value v) {
    switch (v.type) {
        case VALUE_NULL:
            return value_null();

        case VALUE_BOOL:
            return value_bool(v.as.boolean);

        case VALUE_NUMBER:
            return value_number(v.as.number);

        case VALUE_STRING:
            return value_string(v.as.string);

        case VALUE_OBJECT:
            return value_object(v.as.object);

        default:
            return value_null();
    }
}

int value_is_truthy(Value v) {
    switch (v.type) {
        case VALUE_NULL:
            return 0;

        case VALUE_BOOL:
            return v.as.boolean;

        case VALUE_NUMBER:
            return v.as.number != 0;

        case VALUE_STRING:
            return v.as.string != NULL && v.as.string[0] != '\0';

        case VALUE_OBJECT:
            return v.as.object != NULL;

        default:
            return 0;
    }
}

int value_equals(Value a, Value b) {
    if (a.type != b.type) {
        return 0;
    }

    switch (a.type) {
        case VALUE_NULL:
            return 1;

        case VALUE_BOOL:
            return a.as.boolean == b.as.boolean;

        case VALUE_NUMBER:
            return a.as.number == b.as.number;

        case VALUE_STRING:
            return strcmp(a.as.string, b.as.string) == 0;

        case VALUE_OBJECT:
            return a.as.object == b.as.object;

        default:
            return 0;
    }
}

void value_print(Value v) {
    switch (v.type) {
        case VALUE_NULL:
            printf("null");
            break;

        case VALUE_BOOL:
            printf(v.as.boolean ? "true" : "false");
            break;

        case VALUE_NUMBER:
            printf("%g", v.as.number);
            break;

        case VALUE_STRING:
            printf("%s", v.as.string);
            break;

        case VALUE_OBJECT:
            printf("<object:%p>", (void *)v.as.object);
            break;
    }
}
