#include <stdlib.h>

#include "stack.h"


void init(struct Stack *stack) {
    stack->__alloc = 256;
    stack->topPos = -1;
    stack->stack = (char *) malloc(stack->__alloc * sizeof(char));
}

void clear(struct Stack *stack) {
    stack->topPos = -1;
    stack->__alloc = 0;
    free(stack);
}

void push(struct Stack *stack, char x) {
    if (stack->topPos + 1 >= stack->__alloc) {
        stack->stack = (char *) realloc(stack->stack, (stack->__alloc * 2 + 1) * sizeof(char));
        stack->__alloc *= 2;
    }
    ++stack->topPos;
    stack->stack[stack->topPos] = x;
}

char pop(struct Stack *stack) {
    if (stack->topPos - 1 < stack->__alloc / 4) {
        realloc(stack->stack, stack->__alloc / 2 * sizeof(char));
    }
    --stack->topPos;
    return stack->stack[stack->topPos + 1];
}

bool isEmpty(struct Stack *stack) {
    return stack->topPos == -1;
}