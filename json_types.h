#ifndef JSON_JSON_TYPES_H
#define JSON_JSON_TYPES_H

#include <stdbool.h>

bool isInteger(char *str);
bool isReal(char *str);
bool isString(char *str);
bool isTrue(char *str);
bool isFalse(char *str);
bool isNull(char *str);
bool isArray(char *str);
bool isObject(char *str);
short getDataType(char *str);

#endif //JSON_JSON_TYPES_H
