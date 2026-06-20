#ifndef FRAME_H
#define FRAME_H

#include <stdint.h>

#include "chunk.h"
#include "value.h"

#define FRAMES_MAX 64

typedef struct {
    Chunk *chunk;
    uint8_t *ip;
    uint8_t *return_ip;
    int stack_offset;
} CallFrame;

typedef struct {
    CallFrame frames[FRAMES_MAX];
    int count;
} CallStack;

void frame_init(CallStack *cs);

void frame_free(CallStack *cs);

CallFrame *frame_current(CallStack *cs);

int frame_push(
    CallStack *cs,
    Chunk *chunk,
    uint8_t *ip,
    uint8_t *return_ip,
    int stack_offset
);

void frame_pop(CallStack *cs);

#endif
