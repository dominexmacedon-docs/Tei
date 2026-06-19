#include "vm.h"
#include "opcode.h"

#include <stdio.h>
#include <stdlib.h>

#define READ_BYTE() (*vm->ip++)
#define READ_SHORT() (vm->ip += 2, (vm->ip[-2] << 8) | vm->ip[-1])

static VMResult runtime_error(VM *vm, const char *message) {
    fprintf(stderr, "[VM Error] %s\n", message);
    return VM_RUNTIME_ERROR;
}

void vm_init(VM *vm) {
    vm->chunk = NULL;
    vm->ip = NULL;

    stack_init(&vm->stack);
    native_init(&vm->natives);

    native_register(&vm->natives, "show", -1, native_show);
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

Value vm_peek(VM *vm, int distance) {
    return stack_peek(&vm->stack, distance);
}

static void binary_num_op(VM *vm, char op) {
    Value b = vm_pop(vm);
    Value a = vm_pop(vm);

    if (a.type != VALUE_NUMBER || b.type != VALUE_NUMBER) {
        runtime_error(vm, "Operands must be numbers");
        vm_push(vm, value_null());
        return;
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
}

VMResult vm_run(VM *vm) {
    for (;;) {

        OpCode instruction = (OpCode)READ_BYTE();

        switch (instruction) {

            case OP_NOP:
                break;

            case OP_PUSH: {
                Value value = READ_CONSTANT();
                vm_push(vm, value);
                break;
            }

            case OP_POP:
                vm_pop(vm);
                break;

            case OP_ADD: {
                binary_num_op(vm, '+');
                break;
            }

            case OP_SUB:
                binary_num_op(vm, '-');
                break;

            case OP_MUL:
                binary_num_op(vm, '*');
                break;

            case OP_DIV:
                binary_num_op(vm, '/');
                break;

            case OP_MOD:
                binary_num_op(vm, '%');
                break;

            case OP_NEG: {
                Value v = vm_pop(vm);

                if (v.type != VALUE_NUMBER) {
                    return runtime_error(vm, "NEG expects number");
                }

                vm_push(vm, value_number(-v.as.number));
                break;
            }

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

            case OP_JUMP: {
                uint16_t offset = READ_SHORT();
                vm->ip += offset;
                break;
            }

            case OP_JUMP_IF_FALSE: {
                uint16_t offset = READ_SHORT();
                Value v = vm_pop(vm);

                if (!value_is_truthy(v)) {
                    vm->ip += offset;
                }
                break;
            }

            case OP_CALL: {
                return runtime_error(vm, "CALL not implemented yet");
            }

            case OP_RETURN:
                return VM_OK;

            case OP_SHOW: {
                Value v = vm_pop(vm);
                value_print(v);
                printf("\n");
                break;
            }

            case OP_HALT:
                return VM_OK;

            default:
                return runtime_error(vm, "Unknown opcode");
        }
    }
}

VMResult vm_interpret(VM *vm, Chunk *chunk) {
    vm->chunk = chunk;
    vm->ip = chunk->code;

    vm_reset_stack(vm);

    return vm_run(vm);
}
