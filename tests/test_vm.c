#include <stdio.h>
#include "../src/vm/vm.h"
#include "../src/vm/opcode.h"
#include "../src/vm/chunk.h"
#include "../src/vm/value.h"

static void write_op(Chunk *chunk, OpCode op, int line) {
    chunk_write(chunk, (unsigned char)op, line);
}

static void write_constant(Chunk *chunk, Value value, int line) {
    int index = chunk_add_constant(chunk, value);
    chunk_write(chunk, (unsigned char)index, line);
}

static void test_arithmetic() {
    printf("Running arithmetic test...\n");

    Chunk chunk;
    chunk_init(&chunk);

    write_op(&chunk, OP_PUSH, 1);
    write_constant(&chunk, value_number(10), 1);

    write_op(&chunk, OP_PUSH, 1);
    write_constant(&chunk, value_number(20), 1);

    write_op(&chunk, OP_ADD, 1);
    write_op(&chunk, OP_SHOW, 1);
    write_op(&chunk, OP_HALT, 1);

    VM vm;
    vm_init(&vm);

    vm_interpret(&vm, &chunk);

    vm_free(&vm);
    chunk_free(&chunk);

    printf("Arithmetic test done.\n\n");
}

static void test_comparison() {
    printf("Running comparison test...\n");

    Chunk chunk;
    chunk_init(&chunk);

    write_op(&chunk, OP_PUSH, 1);
    write_constant(&chunk, value_number(5), 1);

    write_op(&chunk, OP_PUSH, 1);
    write_constant(&chunk, value_number(10), 1);

    write_op(&chunk, OP_LESS, 1);
    write_op(&chunk, OP_SHOW, 1);
    write_op(&chunk, OP_HALT, 1);

    VM vm;
    vm_init(&vm);

    vm_interpret(&vm, &chunk);

    vm_free(&vm);
    chunk_free(&chunk);

    printf("Comparison test done.\n\n");
}

static void test_jump() {
    printf("Running jump test...\n");

    Chunk chunk;
    chunk_init(&chunk);

    write_op(&chunk, OP_PUSH, 1);
    write_constant(&chunk, value_number(1), 1);

    write_op(&chunk, OP_JUMP_IF_FALSE, 1);
    chunk_write(&chunk, 2, 1);
    chunk_write(&chunk, 0, 1);

    write_op(&chunk, OP_PUSH, 1);
    write_constant(&chunk, value_number(999), 1);

    write_op(&chunk, OP_SHOW, 1);
    write_op(&chunk, OP_HALT, 1);

    VM vm;
    vm_init(&vm);

    vm_interpret(&vm, &chunk);

    vm_free(&vm);
    chunk_free(&chunk);

    printf("Jump test done.\n\n");
}

static void test_boolean() {
    printf("Running boolean test...\n");

    Chunk chunk;
    chunk_init(&chunk);

    write_op(&chunk, OP_PUSH, 1);
    write_constant(&chunk, value_bool(1), 1);

    write_op(&chunk, OP_NOT, 1);
    write_op(&chunk, OP_SHOW, 1);
    write_op(&chunk, OP_HALT, 1);

    VM vm;
    vm_init(&vm);

    vm_interpret(&vm, &chunk);

    vm_free(&vm);
    chunk_free(&chunk);

    printf("Boolean test done.\n\n");
}

int main() {
    test_arithmetic();
    test_comparison();
    test_jump();
    test_boolean();

    printf("All VM tests completed.\n");
    return 0;
}
