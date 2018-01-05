#ifndef JSON_HELPERS_H
#define JSON_HELPERS_H

#include <stdbool.h>

struct Vector {  // Vector of strings
    char **values;
    size_t length;
};

char *__removeExtraSpaces(char *str);
char *__trim(char *str);
struct Vector splitValues(char *str, char sep);
bool vectorsEquals(struct Vector a, struct Vector b);

#endif //JSON_HELPERS_H
