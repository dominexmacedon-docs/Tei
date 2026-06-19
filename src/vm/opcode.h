#ifndef OPCODE_H
#define OPCODE_H

typedef enum {

    OP_NOP,

    OP_PUSH,

    OP_POP,

    OP_LOAD,

    OP_STORE,

    OP_ADD,
    OP_SUB,
    OP_MUL,
    OP_DIV,
    OP_MOD,

    OP_NEG,

    OP_EQUAL,
    OP_NOT_EQUAL,
    OP_GREATER,
    OP_GREATER_EQUAL,
    OP_LESS,
    OP_LESS_EQUAL,

    OP_NOT,

    OP_JUMP,
    OP_JUMP_IF_FALSE,

    OP_CALL,
    OP_RETURN,

    OP_SHOW,

    OP_HALT

} OpCode;

#endif
