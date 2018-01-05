#ifndef JSON_EXCEPTION_H
#define JSON_EXCEPTION_H

#include <stdbool.h>

#define WRITE_IN_LOG_FILE true
#define ERROR_LOG_FILE_NAME "errors.log"

struct Exception {
    const int code;
    const char *message;
};

struct Exception WrongJsonFormatException;
struct Exception ArrayLengthLimitException;
struct Exception KeyLengthLimitException;
struct Exception MemoryError;
struct Exception MaxKeysInObjectLimitException;
struct Exception JsonObjectTooLongException;
struct Exception WrongArrayIndexException;
struct Exception WrongKeyNameException;
struct Exception KeyNotFoundException;

void throw(struct Exception *exception);

#endif //JSON_EXCEPTION_H
