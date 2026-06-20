#include <stdio.h>

#include "../src/vm/chunk.h"
#include "../src/vm/opcode.h"
#include "../src/vm/value.h"
#include "../src/vm/vm.h"

static void emit_op(Chunk *chunk, OpCode op) {
    chunk_write(chunk, (unsigned char)op, 0);
}

static void emit_constant(Chunk *chunk, Value value) {
    int index = chunk_add_constant(chunk, value);

    emit_op(chunk, OP_PUSH);
    chunk_write(chunk, (unsigned char)index, 0);
}

int main(void) {
    Chunk chunk;
    chunk_init(&chunk);

    printf("========== BUILDING BYTECODE ==========\n");

    /* 10 + 20 = 30 */
    emit_constant(&chunk, value_number(10));
    emit_constant(&chunk, value_number(20));
    emit_op(&chunk, OP_ADD);
    emit_op(&chunk, OP_SHOW);

    /* 100 - 58 = 42 */
    emit_constant(&chunk, value_number(100));
    emit_constant(&chunk, value_number(58));
    emit_op(&chunk, OP_SUB);
    emit_op(&chunk, OP_SHOW);

    /* 6 * 7 = 42 */
    emit_constant(&chunk, value_number(6));
    emit_constant(&chunk, value_number(7));
    emit_op(&chunk, OP_MUL);
    emit_op(&chunk, OP_SHOW);

    /* 100 / 5 = 20 */
    emit_constant(&chunk, value_number(100));
    emit_constant(&chunk, value_number(5));
    emit_op(&chunk, OP_DIV);
    emit_op(&chunk, OP_SHOW);

    /* 22 % 5 = 2 */
    emit_constant(&chunk, value_number(22));
    emit_constant(&chunk, value_number(5));
    emit_op(&chunk, OP_MOD);
    emit_op(&chunk, OP_SHOW);

    /* -8 */
    emit_constant(&chunk, value_number(8));
    emit_op(&chunk, OP_NEG);
    emit_op(&chunk, OP_SHOW);

    /* 10 == 10 */
    emit_constant(&chunk, value_number(10));
    emit_constant(&chunk, value_number(10));
    emit_op(&chunk, OP_EQUAL);
    emit_op(&chunk, OP_SHOW);

    /* 10 != 20 */
    emit_constant(&chunk, value_number(10));
    emit_constant(&chunk, value_number(20));
    emit_op(&chunk, OP_NOT_EQUAL);
    emit_op(&chunk, OP_SHOW);

    /* 20 > 10 */
    emit_constant(&chunk, value_number(20));
    emit_constant(&chunk, value_number(10));
    emit_op(&chunk, OP_GREATER);
    emit_op(&chunk, OP_SHOW);

    /* 5 < 10 */
    emit_constant(&chunk, value_number(5));
    emit_constant(&chunk, value_number(10));
    emit_op(&chunk, OP_LESS);
    emit_op(&chunk, OP_SHOW);

    /* !false */
    emit_constant(&chunk, value_bool(0));
    emit_op(&chunk, OP_NOT);
    emit_op(&chunk, OP_SHOW);

    /* push then pop */
    emit_constant(&chunk, value_number(999));
    emit_op(&chunk, OP_POP);

    emit_op(&chunk, OP_HALT);

    VM vm;
    vm_init(&vm);

    printf("========== RUNNING VM ==========\n");

    VMResult result = vm_interpret(&vm, &chunk);

    printf("================================\n");
    printf("VM Result: %s\n", result == VM_OK ? "OK" : "ERROR");

    vm_free(&vm);
    chunk_free(&chunk);

    return 0;
}
