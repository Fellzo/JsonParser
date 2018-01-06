#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "helpers.h"
#include "exception.h"
#include "json.h"
#include "config.h"
#include "json_types.h"
#include "stack.h"


/**
 * Normalize string (delete extra spaces, new lines and etc.)
 * @param str Raw string
 * @throw WrongJsonFormat
 */
char *normalizeJson(char *str)
{
    str = __removeExtraSpaces(str);
    size_t jsonLength = strlen(str);
    if (str[0] != '{' && str[0] != '[') {
        abortWithError(&WrongJsonFormatException);
    } else {
        if((str[0] == '{' && str[jsonLength - 1] != '}') || (str[0] == '[' && str[jsonLength - 1] != ']')) {
            abortWithError(&WrongJsonFormatException);
        }
    }
    return str;
}

/**
 * Representation of numbers/true/false/null
 * @param node
 * @return
 */
char *primitiveRepresentation(struct Node *node) {
    return node->value;
}

struct Node *getValueByKey(struct Object *obj, char *key) {
    for (int i = 0; i < obj->numberOfKeys; ++i) {
        if (strcmp(obj->keys[i], key) == 0) {
            return &obj->values[i];
        }
    }
    return NULL;
}

/**
 * Representation of arrays
 * @param node
 * @return
 */
char *arrRepresentation(struct Node *node) {
    char *res = (char *) malloc((MAX_ARRAY_LENGTH * MAX_STRING_LENGTH + MAX_ARRAY_LENGTH * 3) * sizeof(char));
    res[0] = '[';
    for (int i = 0; i < node->array.length - 1; ++i) {
        strcat(res, node->array.values[i].toString(&node->array.values[i]));
        strcat(res, ", ");
    }
    if (node->array.length > 0) {
        strcat(res, node->array.values[node->array.length - 1].toString(&node->array.values[node->array.length - 1]));
    }
    strcat(res, "]");
    return res;
}

/**
 * Representation for key value pair
 * @param key
 * @param value
 * @return
 */
char *makePairRepresentation(char *key, char *value) {
    char *res = (char *) malloc((strlen(key) + strlen(value) + 4) * sizeof(char));
    strcat(res, key);
    strcat(res, ": ");
    strcat(res, value);
    return res;
}


/**
 * Representation of JSON object
 * @param node
 * @return
 */
char *objectRepresentation(struct Node *node) {
    char *res = (char *) malloc(MAX_ARRAY_LENGTH * MAX_STRING_LENGTH * sizeof(char) +
                                MAX_ARRAY_LENGTH * 2 * sizeof(char));
    int keysCnt = node->object.numberOfKeys;
    res[0] = '{';
    for (int i = 0; i < keysCnt - 1; ++i) {
        strcat(res, makePairRepresentation(node->object.keys[i],
                                          node->object.values[i].toString(&node->object.values[i])));
        strcat(res, ", ");
    }
    // Add last key to result
    if (keysCnt > 0) {
    strcat(res, makePairRepresentation(node->object.keys[keysCnt - 1],
                                       node->object.values[keysCnt - 1].toString(&node->object.values[keysCnt - 1])));
    }
    strcat(res, "}");
    return res;
}


struct Node makeNode(char *str) {
    struct Node res;
    res.dataType = getDataType(str);
    if (res.dataType == array) {
        res.array = parseArray(str);
        res.toString = arrRepresentation;
    } else {
        if (res.dataType == object) {
            res.object = parseObject(str);
            res.toString = objectRepresentation;
        } else {
            res.value = (char *) malloc(sizeof(char) * strlen(str));
            strcpy(res.value, str);
            res.toString = primitiveRepresentation;
        }
    }
    return res;
}


/**
 * parse JSON array from string
 * @param str
 * @return
 */
struct Array parseArray(char *str) {
    struct Array res;
    res.length = 0;
    struct Vector vector = splitValues(str, ',');
    if (vector.length >= MAX_ARRAY_LENGTH) {
        abortWithError(&ArrayLengthLimitException);
    }
    res.values = (struct Node *) malloc(sizeof(struct Node) * vector.length);
    for (size_t i = 0; i < vector.length; ++i) {
        res.values[res.length] = makeNode(vector.values[i]);
        ++res.length;

    }
    return res;
}

/**
 * Parse object from string
 * @param str
 * @return
 */
struct Object parseObject(char *str) {
    struct Object res;
    res.numberOfKeys = 0;
    struct Vector vector = splitValues(str, ','); // split object on key value strings
    if (vector.length > MAX_KEYS_IN_OBJECT) {
        abortWithError(&MaxKeysInObjectLimitException);
    }
    res.values = (struct Node *) malloc(sizeof(struct Node) * vector.length);
    res.keys = (char **) malloc(vector.length * sizeof(char *));

    for (size_t i = 0; i < vector.length; ++i) {
        struct Vector keyValue = splitValues(vector.values[i], ':');
        size_t keyLen = strlen(keyValue.values[0]);
        if (keyLen > MAX_KEY_LENGTH) {
            abortWithError(&KeyLengthLimitException);
        }
        res.keys[res.numberOfKeys] = (char *) malloc(sizeof(char) * (keyLen + 1));
        strcpy(res.keys[res.numberOfKeys], keyValue.values[0]);
        res.values[res.numberOfKeys] = makeNode(keyValue.values[1]);
        ++res.numberOfKeys;
    }
    return res;
}

/**
 * Main parse function
 * @param str
 * @return
 */
struct Node *parseJson(char *str) {
    struct Node *res = (struct Node *) malloc(sizeof(struct Node));
    bool isObj = isObject(str);
    bool isArr = isArray(str);
    if (!isObj && !isArr) {
        abortWithError(&WrongJsonFormatException);
    }
    if (isArr) {
        res->array = parseArray(str);
        res->dataType = array;
        res->toString = arrRepresentation;
    } else {
        res->object = parseObject(str);
        res->dataType = object;
        res->toString = objectRepresentation;
    }
    return res;
}