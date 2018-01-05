#include <stdio.h>
#include <stdlib.h>

#include "exception.h"

struct Exception WrongJsonFormatException = { .message = "Wrong JSON format", .code = 1};
struct Exception ArrayLengthLimitException = { .message = "Array is too long", .code = 2};
struct Exception KeyLengthLimitException = { .message = "Object key is too long", .code = 3};
struct Exception MemoryError = { .message = "Can't allocate memory for object", .code = 4};
struct Exception MaxKeysInObjectLimitException = { .message = "Too many keys in object", .code = 5};
struct Exception JsonObjectTooLongException = { .message = "Json is too long", .code = 6};

void throw(struct Exception *exception)
{
    printf("%s\n", exception->message);
    if (WRITE_IN_LOG_FILE) {
        FILE *errorFile = fopen(ERROR_LOG_FILE_NAME, "a");
        if (errorFile) {
            fputs(exception->message, errorFile);
            fputs("\n", errorFile);
            fclose(errorFile);
        } else {
            printf("%s\n", "Can't open/create errors log file.");
        }
    }
    exit(exception->code);
}