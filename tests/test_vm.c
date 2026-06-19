#include <stdio.h>

#include "../src/vm/chunk.h"
#include "../src/vm/opcode.h"
#include "../src/vm/value.h"
#include "../src/vm/vm.h"

static void emit_constant(Chunk *chunk, Value value) {
    int index = chunk_add_constant(chunk, value);

    chunk_write(chunk, OP_PUSH, 1);
    chunk_write(chunk, (unsigned char)index, 1);
}

static void emit_opcode(Chunk *chunk, OpCode op) {
    chunk_write(chunk, op, 1);
}

int main(void) {
    Chunk chunk;
    chunk_init(&chunk);

    printf("========== BUILDING BYTECODE ==========\n");

    /* show(10 + 20) */
    emit_constant(&chunk, value_number(10));
    emit_constant(&chunk, value_number(20));
    emit_opcode(&chunk, OP_ADD);
    emit_opcode(&chunk, OP_SHOW);

    /* show(50 - 8) */
    emit_constant(&chunk, value_number(50));
    emit_constant(&chunk, value_number(8));
    emit_opcode(&chunk, OP_SUB);
    emit_opcode(&chunk, OP_SHOW);

    /* show(6 * 7) */
    emit_constant(&chunk, value_number(6));
    emit_constant(&chunk, value_number(7));
    emit_opcode(&chunk, OP_MUL);
    emit_opcode(&chunk, OP_SHOW);

    /* show(100 / 5) */
    emit_constant(&chunk, value_number(100));
    emit_constant(&chunk, value_number(5));
    emit_opcode(&chunk, OP_DIV);
    emit_opcode(&chunk, OP_SHOW);

    /* show(17 % 5) */
    emit_constant(&chunk, value_number(17));
    emit_constant(&chunk, value_number(5));
    emit_opcode(&chunk, OP_MOD);
    emit_opcode(&chunk, OP_SHOW);

    /* show(-8) */
    emit_constant(&chunk, value_number(8));
    emit_opcode(&chunk, OP_NEG);
    emit_opcode(&chunk, OP_SHOW);

    /* show(5 == 5) */
    emit_constant(&chunk, value_number(5));
    emit_constant(&chunk, value_number(5));
    emit_opcode(&chunk, OP_EQUAL);
    emit_opcode(&chunk, OP_SHOW);

    /* show(5 != 6) */
    emit_constant(&chunk, value_number(5));
    emit_constant(&chunk, value_number(6));
    emit_opcode(&chunk, OP_NOT_EQUAL);
    emit_opcode(&chunk, OP_SHOW);

    /* show(9 > 3) */
    emit_constant(&chunk, value_number(9));
    emit_constant(&chunk, value_number(3));
    emit_opcode(&chunk, OP_GREATER);
    emit_opcode(&chunk, OP_SHOW);

    /* show(2 < 7) */
    emit_constant(&chunk, value_number(2));
    emit_constant(&chunk, value_number(7));
    emit_opcode(&chunk, OP_LESS);
    emit_opcode(&chunk, OP_SHOW);

    /* show(!false) */
    emit_constant(&chunk, value_bool(0));
    emit_opcode(&chunk, OP_NOT);
    emit_opcode(&chunk, OP_SHOW);

    emit_opcode(&chunk, OP_HALT);

    printf("========== RUNNING VM ==========\n");

    VM vm;
    vm_init(&vm);

    VMResult result = vm_interpret(&vm, &chunk);

    printf("================================\n");
    printf("VM Result: %s\n",
           result == VM_OK ? "OK" : "RUNTIME ERROR");

    vm_free(&vm);
    chunk_free(&chunk);

    return 0;
}
