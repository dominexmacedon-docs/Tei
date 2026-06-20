#include "vm.h"
#include "opcode.h"
#include "object.h"
#include "frame.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define READ_BYTE() (*vm->ip++)
#define READ_SHORT() ((vm->ip += 2), (uint16_t)((vm->ip[-2] << 8) | vm->ip[-1]))
#define READ_CONSTANT() (vm->chunk->constants[READ_BYTE()])

static VMResult runtime_error(VM *vm, const char *message) {
    fprintf(stderr, "[VM Error] %s\n", message);
    return VM_RUNTIME_ERROR;
}

void vm_init(VM *vm) {
    vm->chunk = NULL;
    vm->ip = NULL;

    stack_init(&vm->stack);
    native_init(&vm->natives);
    frame_init(&vm->frames);
}

void vm_free(VM *vm) {
    native_free(&vm->natives);
    stack_free(&vm->stack);
    frame_free(&vm->frames);
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
    return vm->stack.values[vm->stack.top - 1 - distance];
}

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

static VMResult call_function(VM *vm, ObjectFunction *function, int arg_count) {
    if (arg_count != function->arity) {
        return runtime_error(vm, "Incorrect argument count");
    }

    if (frame_push(&vm->frames, &function->chunk, function->chunk.code, vm->stack.top - arg_count - 1) == 0) {
        return runtime_error(vm, "Too many call frames");
    }

    vm->chunk = &function->chunk;
    vm->ip = function->chunk.code;

    return VM_OK;
}

VMResult vm_run(VM *vm) {
    for (;;) {

        OpCode instruction = (OpCode)READ_BYTE();

        switch (instruction) {

            case OP_PUSH: {
                Value constant = READ_CONSTANT();
                vm_push(vm, constant);
                break;
            }

            case OP_POP:
                vm_pop(vm);
                break;

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
                int arg_count = READ_BYTE();
                Value callee = vm_peek(vm, arg_count);

                if (callee.type != VALUE_STRING) {
                    Object *obj = (Object *)callee.as.string;

                    if (obj->type == OBJECT_FUNCTION) {
                        ObjectFunction *function = (ObjectFunction *)obj;
                        VMResult result = call_function(vm, function, arg_count);
                        if (result != VM_OK) return result;
                        break;
                    }

                    if (obj->type == OBJECT_NATIVE) {
                        ObjectNative *native = (ObjectNative *)obj;

                        if (native->arity != arg_count) {
                            return runtime_error(vm, "Native argument mismatch");
                        }

                        Value args[32];

                        for (int i = arg_count - 1; i >= 0; i--) {
                            args[i] = vm_pop(vm);
                        }

                        vm_pop(vm);

                        Value result = native->function(arg_count, args);
                        vm_push(vm, result);
                        break;
                    }

                    return runtime_error(vm, "Invalid call target");
                }

                return runtime_error(vm, "Call target not a function");
            }

            case OP_RETURN: {
                if (vm->frames.count == 0) {
                    return VM_OK;
                }

                Value result = vm_pop(vm);

                frame_pop(&vm->frames);

                if (vm->frames.count == 0) {
                    return VM_OK;
                }

                vm_push(vm, result);

                CallFrame *frame = frame_current(&vm->frames);
                vm->chunk = frame->chunk;
                vm->ip = frame->ip;

                break;
            }

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

    frame_push(&vm->frames, chunk, chunk->code, 0);

    vm_reset_stack(vm);
    return vm_run(vm);
}
