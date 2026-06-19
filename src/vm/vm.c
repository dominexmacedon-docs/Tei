#include "vm.h"
#include "opcode.h"

#include <stdio.h>
#include <stdlib.h>

#define READ_BYTE() (*vm->ip++)
#define READ_CONSTANT() (vm->chunk->constants[READ_BYTE()])

static VMResult runtime_error(VM *vm, const char *message) {
    fprintf(stderr, "[VM Error] %s\n", message);
    return VM_RUNTIME_ERROR;
}

/* =========================
   VM LIFECYCLE
========================= */

void vm_init(VM *vm) {
    vm->chunk = NULL;
    vm->ip = NULL;

    stack_init(&vm->stack);
    native_init(&vm->natives);
}

void vm_free(VM *vm) {
    native_free(&vm->natives);
    stack_free(&vm->stack);
}

void vm_reset_stack(VM *vm) {
    stack_clear(&vm->stack);
}

void vm_push(VM *vm, Value value) {
    stack_push(&vm->stack, value);
}

Value vm_pop(VM *vm) {
    return stack_pop(&vm->stack);
}

/* =========================
   BINARY OPS HELPERS
========================= */

static VMResult binary_numeric(VM *vm, char op) {
    Value b = vm_pop(vm);
    Value a = vm_pop(vm);

    if (a.type != VALUE_NUMBER || b.type != VALUE_NUMBER) {
        return runtime_error(vm, "Operands must be numbers");
    }

    double result = 0;

    switch (op) {
        case '+': result = a.as.number + b.as.number; break;
        case '-': result = a.as.number - b.as.number; break;
        case '*': result = a.as.number * b.as.number; break;
        case '/': result = a.as.number / b.as.number; break;
        case '%': result = (long)a.as.number % (long)b.as.number; break;
    }

    vm_push(vm, value_number(result));
    return VM_OK;
}

/* =========================
   EXECUTION LOOP
========================= */

VMResult vm_run(VM *vm) {
    for (;;) {

        OpCode instruction = (OpCode)READ_BYTE();

        switch (instruction) {

            /* -------------------------
               CONSTANT PUSH
            --------------------------*/
            case OP_PUSH: {
                Value constant = READ_CONSTANT();
                vm_push(vm, constant);
                break;
            }

            /* -------------------------
               STACK OPS
            --------------------------*/
            case OP_POP: {
                vm_pop(vm);
                break;
            }

            /* -------------------------
               ARITHMETIC
            --------------------------*/
            case OP_ADD:
                binary_numeric(vm, '+');
                break;

            case OP_SUB:
                binary_numeric(vm, '-');
                break;

            case OP_MUL:
                binary_numeric(vm, '*');
                break;

            case OP_DIV:
                binary_numeric(vm, '/');
                break;

            case OP_MOD:
                binary_numeric(vm, '%');
                break;

            case OP_NEG: {
                Value v = vm_pop(vm);

                if (v.type != VALUE_NUMBER) {
                    return runtime_error(vm, "NEG expects number");
                }

                vm_push(vm, value_number(-v.as.number));
                break;
            }

            /* -------------------------
               COMPARISONS
            --------------------------*/
            case OP_EQUAL: {
                Value b = vm_pop(vm);
                Value a = vm_pop(vm);
                vm_push(vm, value_bool(value_equals(a, b)));
                break;
            }

            case OP_NOT_EQUAL: {
                Value b = vm_pop(vm);
                Value a = vm_pop(vm);
                vm_push(vm, value_bool(!value_equals(a, b)));
                break;
            }

            case OP_GREATER: {
                Value b = vm_pop(vm);
                Value a = vm_pop(vm);
                vm_push(vm, value_bool(a.as.number > b.as.number));
                break;
            }

            case OP_GREATER_EQUAL: {
                Value b = vm_pop(vm);
                Value a = vm_pop(vm);
                vm_push(vm, value_bool(a.as.number >= b.as.number));
                break;
            }

            case OP_LESS: {
                Value b = vm_pop(vm);
                Value a = vm_pop(vm);
                vm_push(vm, value_bool(a.as.number < b.as.number));
                break;
            }

            case OP_LESS_EQUAL: {
                Value b = vm_pop(vm);
                Value a = vm_pop(vm);
                vm_push(vm, value_bool(a.as.number <= b.as.number));
                break;
            }

            case OP_NOT: {
                Value v = vm_pop(vm);
                vm_push(vm, value_bool(!value_is_truthy(v)));
                break;
            }

            /* -------------------------
               CONTROL FLOW
            --------------------------*/
            case OP_JUMP: {
                uint16_t offset = READ_BYTE();
                offset = (offset << 8) | READ_BYTE();
                vm->ip += offset;
                break;
            }

            case OP_JUMP_IF_FALSE: {
                uint16_t offset = READ_BYTE();
                offset = (offset << 8) | READ_BYTE();

                Value v = vm_pop(vm);

                if (!value_is_truthy(v)) {
                    vm->ip += offset;
                }
                break;
            }

            /* -------------------------
               FUNCTION (NOT USED YET)
            --------------------------*/
            case OP_CALL:
                return runtime_error(vm, "CALL not implemented yet");

            case OP_RETURN:
                return VM_OK;

            /* -------------------------
               OUTPUT
            --------------------------*/
            case OP_SHOW: {
                Value v = vm_pop(vm);
                value_print(v);
                printf("\n");
                break;
            }

            /* -------------------------
               STOP
            --------------------------*/
            case OP_HALT:
                return VM_OK;

            default:
                return runtime_error(vm, "Unknown opcode");
        }
    }
}

/* =========================
   ENTRY
========================= */

VMResult vm_interpret(VM *vm, Chunk *chunk) {
    vm->chunk = chunk;
    vm->ip = chunk->code;

    vm_reset_stack(vm);

    return vm_run(vm);
}
