#ifndef JSON_EXCEPTION_H
#define JSON_EXCEPTION_H

#include <stdbool.h>

#define WRITE_IN_LOG_FILE true
#define ERROR_LOG_FILE_NAME "errors.log"

struct Error {
    const int code;
    const char *message;
};

struct Error WrongJsonFormatException;
struct Error ArrayLengthLimitException;
struct Error KeyLengthLimitException;
struct Error MemoryError;
struct Error MaxKeysInObjectLimitException;
struct Error JsonObjectTooLongException;
struct Error WrongArrayIndexException;
struct Error WrongKeyNameException;
struct Error KeyNotFoundException;
struct Error FileNotFoundException;

void abortWithError(struct Error *error);

#endif //JSON_EXCEPTION_H
