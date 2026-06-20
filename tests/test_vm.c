#include <stdio.h>
#include "../src/vm/vm.h"
#include "../src/vm/chunk.h"
#include "../src/vm/opcode.h"
#include "../src/vm/value.h"

static void emit_op(Chunk *chunk, OpCode op, int line) {
    chunk_write(chunk, (unsigned char)op, line);
}

static void emit_const(Chunk *chunk, Value v, int line) {
    int idx = chunk_add_constant(chunk, v);
    chunk_write(chunk, (unsigned char)idx, line);
}

static void run_vm(Chunk *chunk, const char *label) {
    printf("Running %s...\n", label);

    VM vm;
    vm_init(&vm);

    vm_interpret(&vm, chunk);

    vm_free(&vm);
    chunk_free(chunk);

    printf("%s done.\n\n", label);
}

/* ---------------- ARITHMETIC ---------------- */

static void test_arithmetic() {
    Chunk chunk;
    chunk_init(&chunk);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(10), 1);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(20), 1);

    emit_op(&chunk, OP_ADD, 1);
    emit_op(&chunk, OP_SHOW, 1);

    run_vm(&chunk, "arithmetic test");
}

/* ---------------- SUB / MUL / DIV / MOD ---------------- */

static void test_math_ops() {
    Chunk chunk;
    chunk_init(&chunk);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(50), 1);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(10), 1);

    emit_op(&chunk, OP_SUB, 1);
    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(2), 1);
    emit_op(&chunk, OP_MUL, 1);
    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(3), 1);
    emit_op(&chunk, OP_DIV, 1);
    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(2), 1);
    emit_op(&chunk, OP_MOD, 1);

    emit_op(&chunk, OP_SHOW, 1);

    run_vm(&chunk, "math ops test");
}

/* ---------------- COMPARISON ---------------- */

static void test_comparison() {
    Chunk chunk;
    chunk_init(&chunk);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(5), 1);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(10), 1);

    emit_op(&chunk, OP_LESS, 1);
    emit_op(&chunk, OP_SHOW, 1);

    run_vm(&chunk, "comparison test");
}

/* ---------------- EQUALITY ---------------- */

static void test_equality() {
    Chunk chunk;
    chunk_init(&chunk);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(42), 1);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(42), 1);

    emit_op(&chunk, OP_EQUAL, 1);
    emit_op(&chunk, OP_SHOW, 1);

    run_vm(&chunk, "equality test");
}

/* ---------------- BOOLEAN ---------------- */

static void test_boolean() {
    Chunk chunk;
    chunk_init(&chunk);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_bool(1), 1);

    emit_op(&chunk, OP_NOT, 1);
    emit_op(&chunk, OP_SHOW, 1);

    run_vm(&chunk, "boolean test");
}

/* ---------------- JUMP ---------------- */

static void test_jump() {
    Chunk chunk;
    chunk_init(&chunk);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_bool(0), 1);

    emit_op(&chunk, OP_JUMP_IF_FALSE, 1);
    chunk_write(&chunk, 0, 1);
    chunk_write(&chunk, 4, 1);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(999), 1);

    emit_op(&chunk, OP_SHOW, 1);

    run_vm(&chunk, "jump test");
}

/* ---------------- STACK POP ---------------- */

static void test_pop() {
    Chunk chunk;
    chunk_init(&chunk);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(100), 1);

    emit_op(&chunk, OP_POP, 1);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(1), 1);

    emit_op(&chunk, OP_SHOW, 1);

    run_vm(&chunk, "pop test");
}

/* ---------------- NEGATION ---------------- */

static void test_neg() {
    Chunk chunk;
    chunk_init(&chunk);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(7), 1);

    emit_op(&chunk, OP_NEG, 1);
    emit_op(&chunk, OP_SHOW, 1);

    run_vm(&chunk, "neg test");
}

/* ---------------- HALT ---------------- */

static void test_halt() {
    Chunk chunk;
    chunk_init(&chunk);

    emit_op(&chunk, OP_PUSH, 1);
    emit_const(&chunk, value_number(123), 1);

    emit_op(&chunk, OP_HALT, 1);

    run_vm(&chunk, "halt test");
}

int main() {
    test_arithmetic();
    test_math_ops();
    test_comparison();
    test_equality();
    test_boolean();
    test_jump();
    test_pop();
    test_neg();
    test_halt();

    printf("ALL OPCODE TESTS COMPLETED\n");
    return 0;
}
