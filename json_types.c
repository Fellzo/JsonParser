#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#include "JSON_types.h"
#include "config.h"
#include "stack.h"
#include "helpers.h"
#include "exception.h"

/**
 * JSON data types format checkers
 * Accept only normalized strings (without extra spaces and etc.)
 *
 * Format {} - optional part
 * [] - variable part
 * %digits% - 1 or more numeric symbols
 */

// Callbacks for checking values
typedef bool (CheckerFunction)(char *);
const int NUMBER_OF_CALLBACKS = 8;
CheckerFunction *checkingOrder[] = { &isInteger, &isReal, &isString, &isTrue, &isFalse, &isArray, &isObject, &isNull };


/**
 * Is JSON integer checking
 * format {-}%digits%
 * @param str
 * @return
 */
bool isInteger(char *str) {
    size_t len = strlen(str);
    size_t startFrom = 0;
    if (len < 1) {
        return false;
    }
    if (str[0] == '-') { // Leading minus checking
        startFrom = 1;
    }
    for (size_t i = startFrom; i < len; ++i) {
        if (!isdigit(str[i])) {
            return false;
        }
    }
    return true;
}

/**
 * Is JSON real number checking
 * format: {-}%digits%{.%digits%}{[eE]{[+-]}%digits%}
 * @param str
 * @return
 */
bool isReal(char *str) {
    size_t len = strlen(str);
    if (len < 1) {
        return false;
    }
    bool dotFound = false;
    bool expFound = false;
    for (size_t i = 0; i < len; ++i) {
        if (str[i] == '.') {
            dotFound = true;
        }
        if (tolower(str[i]) == 'e') {
            expFound = true;
        }
    }
    char *tmp = (char *) malloc(len * sizeof(char));
    strcpy(tmp, str);
    char *beforeExp = strtok(tmp, "eE");
    char *afterExp = strtok(NULL, "eE");
    char *beforeDot = strtok(beforeExp, ".");
    char *afterDot = strtok(NULL, ".");
    if (!isInteger(beforeDot)) {
        return false;
    }
    if (afterDot) {
        if (!isdigit(afterDot[0]) || !isInteger(afterDot)) { // check case like 123.+55
            return false;
        }
    } else {
        if (dotFound) {  // check case like 123.
            return false;
        }
    }
    if (afterExp) {
        if (afterExp[0] == '-' || afterExp[0] == '+') {
            ++afterExp;
        }
        if (!isInteger(afterExp)) {
            return false;
        }
    } else {
        if (expFound) {
            return false; // check case like 13e
        }
    }
    return true;
}

/**
 * Is JSON null checking
 * @param str
 * @return
 */
bool isNull(char *str) {
    return strcmp(str, "null") == 0;
}


/**
 * Is JSON string checking
 * @param str
 * @return
 */
bool isString(char *str) {
    size_t len = strlen(str);
    if (str[0] != '"' || str[len - 1] != '"') {
        return false;
    }
    for (size_t i = 1; i < len - 1; ++i) {
        if (str[i] == '"' && str[i - 1] != '\\') {
            return false;
        }
    }
    return true;
}

/**
 * Is JSON true checking
 * @param str
 * @return
 */
bool isTrue(char *str) {
    return strcmp(str, "true") == 0;
}


/**
 * Is JSON false checking
 * @param str
 * @return
 */
bool isFalse(char *str) {
    return strcmp(str, "false") == 0;
}

short getDataType(char *str) { // return type constant or -1
    for (size_t j = 0; j < NUMBER_OF_CALLBACKS; ++j) {
        if (checkingOrder[j](str)) {
            return (short) j;
        }
    }
    return -1;
}

/**
 * Is valid JSON array
 * @param str
 * @return
 */
bool isArray(char *str) {
    size_t len = strlen(str);
    if (len < 2 || str[0] != '[' || str[len - 1] != ']') {
        return false;
    }
    struct Vector values = splitValues(str, ',');
    for (size_t i = 0; i < values.length; ++i) {
        if (getDataType(values.values[i]) == -1) {
            return false;
        }
    }
    return true;
}


/**
 * Is valid JSON object
 * @param str
 * @return
 */
bool isObject(char *str) {
    size_t len = strlen(str);
    if (len < 2 || str[0] != '{' || str[len - 1] != '}') {
        return false;
    }
    struct Vector values = splitValues(str, ',');
    for (size_t i = 0; i < values.length; ++i) {
        struct Vector keyValue = splitValues(values.values[i], ':');
        if (keyValue.length != 2) {
            return false;
        }
        if (!isString(keyValue.values[0]) || getDataType(keyValue.values[1]) == -1) {
            return false;
        }
    }
    return true;
}