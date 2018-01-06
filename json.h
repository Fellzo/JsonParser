#ifndef JSON_JSON_H
#define JSON_JSON_H

struct Array {
    struct Node *values;
    int length;
};

struct Object {
    char **keys;
    struct Node *values;

    int numberOfKeys;
};

struct Node {
    union {
        char *value; // for null/true/false/number/JSON_STRING types
        struct Array array;
        struct Object object;
    };
    short dataType;

    char *(*toString)(struct Node * node); // return representation of node
};



char *normalizeJson(char *str);
struct Node *parseJson(char *str);
struct Array parseArray(char *str);
struct Object parseObject(char *str);
struct Node *getValueByKey(struct Object *obj, char *key);

#endif //JSON_JSON_H
