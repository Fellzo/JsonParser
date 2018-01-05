#ifndef JSON_STACK_H
#define JSON_STACK_H

#include <stdbool.h>

/**
 * Character stack for parser
 */

struct Stack {
    int topPos;
    int __alloc;
    char *stack;
};

void init(struct Stack *stack);
void push(struct Stack *stack, char x);
char pop(struct Stack *stack);
bool isEmpty(struct Stack *stack);

#endif //JSON_STACK_H
