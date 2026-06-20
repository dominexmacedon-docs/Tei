#include <stdio.h>

#include "../src/vm/chunk.h"
#include "../src/vm/opcode.h"
#include "../src/vm/value.h"
#include "../src/vm/vm.h"

static void emit_op(Chunk *chunk, OpCode op) {
    chunk_write(chunk, (unsigned char)op, 1);
}

static void emit_constant(Chunk *chunk, Value value) {
    int index = chunk_add_constant(chunk, value);

    emit_op(chunk, OP_PUSH);
    chunk_write(chunk, (unsigned char)index, 1);
}

static void emit_jump_if_false(Chunk *chunk, uint16_t offset) {
    emit_op(chunk, OP_JUMP);
    chunk->code[chunk->count - 1] = OP_JUMP_IF_FALSE;

    chunk_write(chunk, (offset >> 8) & 0xff, 1);
    chunk_write(chunk, offset & 0xff, 1);
}

static void emit_jump(Chunk *chunk, uint16_t offset) {
    emit_op(chunk, OP_JUMP);

    chunk_write(chunk, (offset >> 8) & 0xff, 1);
    chunk_write(chunk, offset & 0xff, 1);
}

int main(void) {

    Chunk chunk;
    chunk_init(&chunk);

    printf("========== BUILDING BYTECODE ==========\n");

    /*
        show 10 + 20;
    */

    emit_constant(&chunk, value_number(10));
    emit_constant(&chunk, value_number(20));
    emit_op(&chunk, OP_ADD);
    emit_op(&chunk, OP_SHOW);

    /*
        if (1 < 2)
            show 111;
    */

    emit_constant(&chunk, value_number(1));
    emit_constant(&chunk, value_number(2));
    emit_op(&chunk, OP_LESS);

    /*
        Skip THEN block if false.

        THEN block =
            PUSH 111
            SHOW

        Size = 4 bytes
    */

    emit_jump_if_false(&chunk, 4);

    emit_constant(&chunk, value_number(111));
    emit_op(&chunk, OP_SHOW);

    /*
        if (2 < 1)
            show 999;
        else
            show 222;
    */

    emit_constant(&chunk, value_number(2));
    emit_constant(&chunk, value_number(1));
    emit_op(&chunk, OP_LESS);

    /*
        THEN size = 4 bytes
        JUMP size = 3 bytes
        total skip = 7
    */

    emit_jump_if_false(&chunk, 7);

    emit_constant(&chunk, value_number(999));
    emit_op(&chunk, OP_SHOW);

    /*
        Skip ELSE
    */

    emit_jump(&chunk, 4);

    emit_constant(&chunk, value_number(222));
    emit_op(&chunk, OP_SHOW);

    /*
        show !(0);
    */

    emit_constant(&chunk, value_number(0));
    emit_op(&chunk, OP_NOT);
    emit_op(&chunk, OP_SHOW);

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
