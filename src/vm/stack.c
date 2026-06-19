#include "stack.h"

#include <stdio.h>
#include <stdlib.h>

void stack_init(Stack *stack) {
    stack->top = 0;
}

void stack_free(Stack *stack) {
    while (!stack_is_empty(stack)) {
        Value value = stack_pop(stack);
        value_free(&value);
    }
}

int stack_is_empty(const Stack *stack) {
    return stack->top == 0;
}

int stack_is_full(const Stack *stack) {
    return stack->top >= STACK_MAX;
}

void stack_push(Stack *stack, Value value) {
    if (stack_is_full(stack)) {
        fprintf(stderr, "VM stack overflow.\n");
        exit(EXIT_FAILURE);
    }

    stack->values[stack->top++] = value_copy(value);
}

Value stack_pop(Stack *stack) {
    if (stack_is_empty(stack)) {
        fprintf(stderr, "VM stack underflow.\n");
        exit(EXIT_FAILURE);
    }

    return stack->values[--stack->top];
}

Value stack_peek(const Stack *stack) {
    if (stack_is_empty(stack)) {
        fprintf(stderr, "VM stack is empty.\n");
        exit(EXIT_FAILURE);
    }

    return stack->values[stack->top - 1];
}

void stack_clear(Stack *stack) {
    while (!stack_is_empty(stack)) {
        Value value = stack_pop(stack);
        value_free(&value);
    }
}
