#ifndef FUNCTIONSLIST_H
#define FUNCTIONSLIST_H

#include "functions.h"

typedef const char *(*handler_fn)(void);

const char *root(void);
const char *square(void);
const char *cube(void);
const char *helloworld(void);
const char *pingpong(void);

static handler_fn p[NUM_FUNCTIONS] = {
    [0] = root,
    [1] = square,
    [2] = cube,
    [3] = helloworld,
    [4] = pingpong
};

#endif
