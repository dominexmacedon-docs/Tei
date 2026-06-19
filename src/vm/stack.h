#ifndef STACK_H
#define STACK_H

#include <stddef.h>

#include "value.h"

#define STACK_MAX 1024

typedef struct {
    Value values[STACK_MAX];
    size_t top;
} Stack;

void stack_init(Stack *stack);

void stack_free(Stack *stack);

int stack_is_empty(const Stack *stack);

int stack_is_full(const Stack *stack);

void stack_push(Stack *stack, Value value);

Value stack_pop(Stack *stack);

Value stack_peek(const Stack *stack);

void stack_clear(Stack *stack);

#endif
