#ifndef VALUE_H
#define VALUE_H

#include <stddef.h>

typedef struct Object Object;

typedef enum {
    VALUE_NULL,
    VALUE_BOOL,
    VALUE_NUMBER,
    VALUE_STRING,
    VALUE_OBJECT
} ValueType;

typedef struct {
    ValueType type;

    union {
        int boolean;
        double number;
        char *string;
        Object *object;
    } as;
} Value;

Value value_null(void);
Value value_bool(int value);
Value value_number(double value);
Value value_string(const char *value);
Value value_object(Object *object);

void value_free(Value *value);
Value value_copy(Value value);

int value_is_truthy(Value value);
int value_equals(Value a, Value b);

void value_print(Value value);

#endif
