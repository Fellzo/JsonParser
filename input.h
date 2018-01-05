#ifndef JSON_INPUT_H
#define JSON_INPUT_H

#include "config.h"

enum {
    arr_index,
    key
};

struct Mask {
    union {
        int index;
        char *key;
    };
    short type;
};

struct NodesPath {
    struct Mask *masks;
    int length;
};

struct NodesPath readMask();

#endif //JSON_INPUT_H