#include "frame.h"

#include <stdio.h>
#include <stdlib.h>

static void runtime_error(const char *msg) {
    fprintf(stderr, "[Frame Error] %s\n", msg);
    exit(EXIT_FAILURE);
}

void frame_init(CallStack *cs) {
    cs->count = 0;
}

void frame_free(CallStack *cs) {
    cs->count = 0;
}

CallFrame *frame_current(CallStack *cs) {
    if (cs->count == 0) {
        runtime_error("No active call frame");
    }

    return &cs->frames[cs->count - 1];
}

int frame_push(
    CallStack *cs,
    Chunk *chunk,
    uint8_t *ip,
    uint8_t *return_ip,
    int stack_offset
) {
    if (cs->count >= FRAMES_MAX) {
        runtime_error("Stack overflow: too many call frames");
    }

    CallFrame *frame = &cs->frames[cs->count++];

    frame->chunk = chunk;
    frame->ip = ip;
    frame->return_ip = return_ip;
    frame->stack_offset = stack_offset;

    return 1;
}

void frame_pop(CallStack *cs) {
    if (cs->count == 0) {
        runtime_error("Frame underflow");
    }

    cs->count--;
}
