#include "compiler.h"

#include <stdio.h>
#include <stdlib.h>

void emit_byte(Compiler *compiler, unsigned char byte, int line) {
    chunk_write(compiler->chunk, byte, line);
}

void emit_opcode(Compiler *compiler, OpCode opcode, int line) {
    emit_byte(compiler, (unsigned char)opcode, line);
}

void emit_bytes(
    Compiler *compiler,
    unsigned char byte1,
    unsigned char byte2,
    int line
) {
    emit_byte(compiler, byte1, line);
    emit_byte(compiler, byte2, line);
}

int emit_constant(
    Compiler *compiler,
    Value value,
    int line
) {
    int index = chunk_add_constant(compiler->chunk, value);

    if (index > 255) {
        fprintf(stderr, "Too many constants in chunk.\n");
        exit(EXIT_FAILURE);
    }

    emit_opcode(compiler, OP_PUSH, line);
    emit_byte(compiler, (unsigned char)index, line);

    return index;
}

int emit_jump(
    Compiler *compiler,
    OpCode opcode,
    int line
) {
    emit_opcode(compiler, opcode, line);

    emit_byte(compiler, 0xff, line);
    emit_byte(compiler, 0xff, line);

    return (int)(compiler->chunk->count - 2);
}

void patch_jump(
    Compiler *compiler,
    int offset
) {
    int jump = (int)compiler->chunk->count - offset - 2;

    if (jump > 65535) {
        fprintf(stderr, "Jump offset too large.\n");
        exit(EXIT_FAILURE);
    }

    compiler->chunk->code[offset] = (jump >> 8) & 0xff;
    compiler->chunk->code[offset + 1] = jump & 0xff;
}

void emit_loop(
    Compiler *compiler,
    int loop_start,
    int line
) {
    emit_opcode(compiler, OP_JUMP, line);

    int offset = (int)compiler->chunk->count - loop_start + 2;

    if (offset > 65535) {
        fprintf(stderr, "Loop body too large.\n");
        exit(EXIT_FAILURE);
    }

    emit_byte(compiler, (offset >> 8) & 0xff, line);
    emit_byte(compiler, offset & 0xff, line);
}

void emit_return(
    Compiler *compiler,
    int line
) {
    emit_opcode(compiler, OP_RETURN, line);
}
