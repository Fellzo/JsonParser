#include <stdio.h>
#include <stdlib.h>
#include <check.h>


#include "json.h"
#include "test.h"
#include "config.h"
#include "input.h"
#include "exception.h"


int main(int argc, char **argv) {
    char *json = (char *) malloc(sizeof(char) * MAX_JSON_LENGTH);
    char *fileName;
    if (argc > 1) {
        if (strcmp(argv[1], "start_tests") == 0) {
            startTests();
            return 0;
        }
        fileName = argv[1];
    } else {
        fileName = "test.json";
    }
    FILE *jF = fopen(fileName, "r");
    char *buff = (char *) malloc(sizeof(char) * MAX_LINE_LENGTH);
    while (!feof(jF)) {
        fgets(buff, MAX_LINE_LENGTH, jF);
        strcat(json, buff);
    }
    json = normalizeJson(json);
    struct Node *root = parseJson(json);
    struct Node *currNode = root;
    printf("Enter path to Node:\n");
    struct NodesPath inp = readMask();
    for (int i = 0; i < inp.length; ++i) {
        if (inp.masks[i].type == key) {
            if (currNode->dataType != object) {
                throw(&WrongKeyNameException);
            }
            currNode = getValueByKey(&currNode->object, inp.masks[i].key);
            if (currNode == NULL) {
                throw(&KeyNotFoundException);
            }
        } else {
            if (currNode->dataType != array || currNode->array.length <= inp.masks[i].index) {
                throw(&WrongArrayIndexException);
            }
            currNode = &currNode->array.values[inp.masks[i].index];
        }
    }
    printf("%s\n", currNode->toString(currNode));
    return 0;
}
