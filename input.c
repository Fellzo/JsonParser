#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include "input.h"
#include "config.h"
#include "exception.h"


bool isAvailableIndex(char *str) {
    int len = (int) strlen(str);
    for (int i = 0; i < len; ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

struct NodesPath readMask() {
    struct NodesPath res;
    res.masks = (struct Mask *) malloc(MAX_KEYS_IN_INPUT * sizeof(struct Mask));
    res.length = 0;
    char *buff = (char *) malloc(MAX_KEYS_IN_INPUT * MAX_KEY_LENGTH * 2 * sizeof(char));
    scanf("%s", buff);
    int len = (int) strlen(buff);
    for (int i = 0; i < len; ++i) {
        if (buff[i] == '[') {
            bool inQuote = false;
            char *tmp = (char *) malloc(MAX_KEY_LENGTH * 2 * sizeof(char));
            int currPos = 0;
            for (int j = i + 1; j < len; ++j, ++i) {
                if (!inQuote && buff[j] == ']') {
                    break;
                }
                if (!inQuote && buff[j] == '[') {
                    abortWithError(&WrongKeyNameException);
                }
                if (buff[j] == '"' && buff[j - 1] != '\\') {
                    inQuote = !inQuote;
                }
                tmp[currPos] = buff[j];
                ++currPos;
            }
            tmp[currPos] = '\0';
            if (tmp[0] == '"') {
                res.masks[res.length].type = key;
                res.masks[res.length].key = (char *) malloc((strlen(tmp) + 1) * sizeof(char));
                strcpy(res.masks[res.length].key, tmp);
            } else {
                res.masks[res.length].type = arr_index;
                if (!isAvailableIndex(tmp)) {
                    abortWithError(&WrongArrayIndexException);
                }
                res.masks[res.length].index = atoi(tmp);
            }
            ++res.length;
            free(tmp);
        }
    }
    free(buff);
    return res;
}