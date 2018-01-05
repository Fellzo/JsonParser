#ifndef JSON_CONFIG_H
#define JSON_CONFIG_H

#define MAX_KEY_LENGTH 256
#define MAX_KEYS_IN_OBJECT 256
#define MAX_ARRAY_LENGTH 800
#define MAX_STRING_LENGTH 5000
#define MAX_JSON_LENGTH 50000
#define MAX_LINE_LENGTH 6000
#define VALUE_DELIMITER ','

enum JSON_DATA_TYPES {
    integer,
    real,
    string,
    True,
    False,
    array,
    object,
    null
};


#endif //JSON_CONFIG_H