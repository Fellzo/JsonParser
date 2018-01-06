#include <stdio.h>
#include <stdlib.h>

#include "exception.h"

struct Error WrongJsonFormatException = { .message = "Wrong JSON format", .code = 1};
struct Error ArrayLengthLimitException = { .message = "Array is too long", .code = 2};
struct Error KeyLengthLimitException = { .message = "Object key is too long", .code = 3};
struct Error MemoryError = { .message = "Can't allocate memory for object", .code = 4};
struct Error MaxKeysInObjectLimitException = { .message = "Too many keys in object", .code = 5};
struct Error JsonObjectTooLongException = { .message = "Json is too long", .code = 6};
struct Error WrongKeyNameException = { .message = "Wrong key name", .code = 7};
struct Error WrongArrayIndexException = { .message = "Wrong array index", .code = 8};
struct Error KeyNotFoundException = { .message = "Key not found", .code = 9};
struct Error FileNotFoundException = { .message = "Json file not found", .code = 10};

void abortWithError(struct Error *error)
{
    printf("%s\n", error->message);
    if (WRITE_IN_LOG_FILE) {
        FILE *errorFile = fopen(ERROR_LOG_FILE_NAME, "a");
        if (errorFile) {
            fputs(error->message, errorFile);
            fputs("\n", errorFile);
            fclose(errorFile);
        } else {
            printf("%s\n", "Can't open/create errors log file.");
        }
    }
    exit(error->code);
}