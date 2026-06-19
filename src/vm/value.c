#include "value.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char *copy_string(const char *text) {
    size_t length = strlen(text);

    char *copy = (char *)malloc(length + 1);

    if (copy == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(EXIT_FAILURE);
    }

    memcpy(copy, text, length + 1);

    return copy;
}

Value value_null(void) {
    Value value;

    value.type = VALUE_NULL;

    return value;
}

Value value_bool(int boolean) {
    Value value;

    value.type = VALUE_BOOL;
    value.as.boolean = boolean ? 1 : 0;

    return value;
}

Value value_number(double number) {
    Value value;

    value.type = VALUE_NUMBER;
    value.as.number = number;

    return value;
}

Value value_string(const char *string) {
    Value value;

    value.type = VALUE_STRING;
    value.as.string = copy_string(string);

    return value;
}

void value_free(Value *value) {
    if (value == NULL) {
        return;
    }

    if (value->type == VALUE_STRING) {
        free(value->as.string);
        value->as.string = NULL;
    }

    value->type = VALUE_NULL;
}

Value value_copy(Value value) {
    switch (value.type) {

        case VALUE_NULL:
            return value_null();

        case VALUE_BOOL:
            return value_bool(value.as.boolean);

        case VALUE_NUMBER:
            return value_number(value.as.number);

        case VALUE_STRING:
            return value_string(value.as.string);

        default:
            return value_null();
    }
}

int value_is_truthy(Value value) {
    switch (value.type) {

        case VALUE_NULL:
            return 0;

        case VALUE_BOOL:
            return value.as.boolean;

        case VALUE_NUMBER:
            return value.as.number != 0;

        case VALUE_STRING:
            return value.as.string != NULL &&
                   value.as.string[0] != '\0';

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

        default:
            return 0;
    }
}

void value_print(Value value) {
    switch (value.type) {

        case VALUE_NULL:
            printf("null");
            break;

        case VALUE_BOOL:
            printf("%s", value.as.boolean ? "true" : "false");
            break;

        case VALUE_NUMBER:
            printf("%g", value.as.number);
            break;

        case VALUE_STRING:
            printf("%s", value.as.string);
            break;
    }
}
