#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>

#include "helpers.h"
#include "config.h"
#include "stack.h"
#include "json.h"
#include "exception.h"

/**
 * Remove leading and trailing symbols
 * @param str Raw string
 * @return String without raw and trailing symbols
 */
char *__trim(char *str) {
    int left = 0, right = (int) strlen(str);
    char *res = (char *) malloc(sizeof(char) * right);
    while (isspace(str[left]) && left < right) {
        ++left;
    }
    while (isspace(str[right - 1]) && right > left) {
        --right;
    }
    strncpy(res, str + left, right - left);
    return res;
}

/**
 * Remove all space symbols outside quotes
 * @param str Raw string
 * @return String without extra spaces
 */
char *__removeExtraSpaces(char *str) {
    int current = 1;
    int strLength = (int) strlen(str);
    char *res = (char *) malloc(sizeof(char) * strLength);
    res[0] = str[0];
    bool inQuotes = false;
    for (int i = 1; i < strLength; ++i) {
        if (str[i] == '"' && str[i - 1] != '\\') {
            inQuotes = !inQuotes;
        }
        if (!isspace(str[i]) || inQuotes) {
            res[current] = str[i];
            ++current;
        }
    }
    res[current] = '\0';
    return res;
}

bool isBracketsMatch(char open, char close) {
    return (open == '{' && close == '}') || (open == '[' && close == ']');
}

/**
 * Split Json array or object by sep
 * @param str
 * @return
 */
struct Vector splitValues(char *str, char sep) {
    struct Vector res;
    res.values = (char **) malloc(MAX_ARRAY_LENGTH * sizeof(char *));
    res.length = 0;
    int startPos = 0;
    int endPos = (int) strlen(str);
    if (str[0] == '[' || str[0] == '{') {  // Don't check first pair of brackets
        startPos = 1;
        --endPos;
    }
    char *currentValue = (char *) malloc(MAX_JSON_LENGTH * sizeof(char));
    int currentPos = 0;
    bool inQuote = false;
    struct Stack stack;
    init(&stack);
    for (int i = startPos; i < endPos; ++i) {
        if (str[i] == '"') {
            if (i == 0 || str[i - 1] != '\\') {
                inQuote = !inQuote;
            }
        }
        if ((str[i] == sep && !inQuote && isEmpty(&stack)) || i == endPos - 1) {
            if (i == endPos - 1) {
                currentValue[currentPos] = str[i];
                ++currentPos;
            }
            currentValue[currentPos] = '\0';

            res.values[res.length] = (char *) malloc((currentPos + 1) * sizeof(char));
            strcpy(res.values[res.length], currentValue);
            currentPos = 0;
            ++res.length;
        } else {
            currentValue[currentPos] = str[i];
            ++currentPos;
            if (currentPos >= MAX_JSON_LENGTH) {
                throw(&JsonObjectTooLongException);
            }
        }
        if (str[i] == '{' || str[i] == '[') {
            push(&stack, str[i]);
        }
        if (str[i] == '}' || str[i] == ']') {
            if (isEmpty(&stack)) {
                throw(&WrongJsonFormatException);
            }
            char c = pop(&stack);
            if (!isBracketsMatch(c, str[i])) {
                throw(&WrongJsonFormatException);
            }
        }
    }
    free(currentValue);
    return res;
}

/**
 * check is vector a equals vector b
 * @param a
 * @param b
 * @return
 */
bool vectorsEquals(struct Vector a, struct Vector b) {
    if (a.length != b.length) {
        return false;
    }
    for (size_t i = 0; i < a.length; ++i) {
        if (strcmp(a.values[i], b.values[i]) != 0) {
            return false;
        }
    }
    return true;
}