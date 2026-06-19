#ifndef CHUNK_H
#define CHUNK_H

#include <stddef.h>

#include "opcode.h"
#include "value.h"

typedef struct {
    unsigned char *code;
    int *lines;

    size_t count;
    size_t capacity;

    Value *constants;
    size_t constant_count;
    size_t constant_capacity;
} Chunk;

void chunk_init(Chunk *chunk);

void chunk_free(Chunk *chunk);

void chunk_write(Chunk *chunk, unsigned char byte, int line);

int chunk_add_constant(Chunk *chunk, Value value);

#endif
