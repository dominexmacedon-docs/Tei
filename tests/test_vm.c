#include <stdio.h>

#include "../src/vm/vm.h"
#include "../src/vm/chunk.h"
#include "../src/vm/opcode.h"
#include "../src/vm/value.h"

static void write_op(Chunk *chunk, OpCode op, int line) {
    chunk_write(chunk, (unsigned char)op, line);
}

static void write_constant(Chunk *chunk, Value value, int line) {
    int index = chunk_add_constant(chunk, value);

    // OP_PUSH <constant_index>
    chunk_write(chunk, OP_PUSH, line);
    chunk_write(chunk, (unsigned char)index, line);
}

int main(void) {
    Chunk chunk;
    chunk_init(&chunk);

    // Build bytecode:
    // 10
    write_constant(&chunk, value_number(10), 1);

    // 20
    write_constant(&chunk, value_number(20), 2);

    // ADD (10 + 20)
    write_op(&chunk, OP_ADD, 3);

    // 2
    write_constant(&chunk, value_number(2), 4);

    // MUL ((10 + 20) * 2)
    write_op(&chunk, OP_MUL, 5);

    // SHOW result
    write_op(&chunk, OP_SHOW, 6);

    // STOP VM
    write_op(&chunk, OP_HALT, 7);

    VM vm;
    vm_init(&vm);

    printf("========== VM TEST ==========\n");

    VMResult result = vm_interpret(&vm, &chunk);

    printf("\n=============================\n");
    printf("VM Result: %s\n", result == VM_OK ? "OK" : "ERROR");

    vm_free(&vm);
    chunk_free(&chunk);

    return 0;
}
