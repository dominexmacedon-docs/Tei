#include "chunk.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void *grow_array(void *pointer, size_t old_size, size_t new_size) {
    (void)old_size;

    void *result = realloc(pointer, new_size);

    if (result == NULL) {
        fprintf(stderr, "Out of memory.\n");
        exit(EXIT_FAILURE);
    }

    return result;
}

static size_t grow_capacity(size_t capacity) {
    if (capacity < 8) {
        return 8;
    }

    return capacity * 2;
}

void chunk_init(Chunk *chunk) {
    chunk->code = NULL;
    chunk->lines = NULL;

    chunk->count = 0;
    chunk->capacity = 0;

    chunk->constants = NULL;
    chunk->constant_count = 0;
    chunk->constant_capacity = 0;
}

void chunk_free(Chunk *chunk) {
    size_t i;

    for (i = 0; i < chunk->constant_count; i++) {
        value_free(&chunk->constants[i]);
    }

    free(chunk->code);
    free(chunk->lines);
    free(chunk->constants);

    chunk_init(chunk);
}

void chunk_write(Chunk *chunk, unsigned char byte, int line) {
    if (chunk->capacity < chunk->count + 1) {

        size_t old_capacity = chunk->capacity;

        chunk->capacity = grow_capacity(old_capacity);

        chunk->code = grow_array(
            chunk->code,
            old_capacity * sizeof(unsigned char),
            chunk->capacity * sizeof(unsigned char)
        );

        chunk->lines = grow_array(
            chunk->lines,
            old_capacity * sizeof(int),
            chunk->capacity * sizeof(int)
        );
    }

    chunk->code[chunk->count] = byte;
    chunk->lines[chunk->count] = line;

    chunk->count++;
}

int chunk_add_constant(Chunk *chunk, Value value) {
    if (chunk->constant_capacity < chunk->constant_count + 1) {

        size_t old_capacity = chunk->constant_capacity;

        chunk->constant_capacity = grow_capacity(old_capacity);

        chunk->constants = grow_array(
            chunk->constants,
            old_capacity * sizeof(Value),
            chunk->constant_capacity * sizeof(Value)
        );
    }

    chunk->constants[chunk->constant_count] = value_copy(value);

    chunk->constant_count++;

    return (int)(chunk->constant_count - 1);
}
