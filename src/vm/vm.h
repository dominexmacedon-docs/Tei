#ifndef VM_H
#define VM_H

#include <stdint.h>
#include "chunk.h"
#include "stack.h"
#include "native.h"
#include "frame.h"

typedef enum {
    VM_OK,
    VM_RUNTIME_ERROR
} VMResult;

typedef struct {
    Chunk *chunk;
    uint8_t *ip;

    Stack stack;
    NativeRegistry natives;
    CallStack frames;
} VM;

void vm_init(VM *vm);
void vm_free(VM *vm);

void vm_reset_stack(VM *vm);

void vm_push(VM *vm, Value value);
Value vm_pop(VM *vm);
Value vm_peek(VM *vm, int distance);

VMResult vm_run(VM *vm);
VMResult vm_interpret(VM *vm, Chunk *chunk);

#endif
