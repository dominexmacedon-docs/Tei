#ifndef COMPILER_H
#define COMPILER_H

#include "../parser/parser.h"
#include "../vm/chunk.h"
#include "../vm/value.h"
#include "../vm/opcode.h"

typedef struct {
    Chunk *chunk;
} Compiler;
void compiler_init(Compiler *compiler, Chunk *chunk);
void compiler_free(Compiler *compiler);
int compiler_compile(Compiler *compiler, ASTNode *node);
int compiler_compile_statement(Compiler *compiler, ASTNode *node);
int compiler_compile_block(Compiler *compiler, ASTNode *node);
int compiler_compile_expression(Compiler *compiler, ASTNode *node);
void emit_byte(Compiler *compiler, unsigned char byte, int line);
void emit_opcode(Compiler *compiler, OpCode opcode, int line);

void emit_bytes(
    Compiler *compiler,
    unsigned char byte1,
    unsigned char byte2,
    int line
);

int emit_constant(
    Compiler *compiler,
    Value value,
    int line
);

int emit_jump(
    Compiler *compiler,
    OpCode opcode,
    int line
);

void patch_jump(
    Compiler *compiler,
    int offset
);

void emit_loop(
    Compiler *compiler,
    int loop_start,
    int line
);

void emit_return(
    Compiler *compiler,
    int line
);

#endif
