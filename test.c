#include <assert.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "helpers.h"
#include "json_types.h"
#include "test.h"
#include "stack.h"

void isIntegerTests() {
    assert(isInteger("1000") == true); // Simple test
    assert(isInteger("-1000") == true); // Leading minus
    assert(isInteger("+1000") == false); // JSON number can't has leading plus
    assert(isInteger("132.312") == false); // Real number
    assert(isInteger("1234eee") == false); // Text after numbers
    assert(isInteger("") == false); // Fail on empty string
    assert(isInteger("randomstring") == false); // Fail on text
}

void isRealTests() {
    assert(isReal("1000") == true); // Integer can be real too
    assert(isReal("10.1") == true); // Simple test
    assert(isReal("10e5") == true); // Exponential test
    assert(isReal("10E5") == true); // Register exponential test
    assert(isReal("10e-5") == true); // Sign after E
    assert(isReal("10e+5") == true); // Sign after E 2
    assert(isReal("10.312e5") == true); // Dot and E
    assert(isReal("10.312e-144") == true); // Dot and sign E
    assert(isReal("-10.55e+144") == true); // Complex test
    assert(isReal("10e55.1") == false); // Only integer exponent
    assert(isReal("10.") == false); // Missing part after dot
    assert(isReal("512e") == false); // Missing part after exp
    assert(isReal("randomstring") == false); // Number missing
    assert(isReal("") == false); // Empty string
    assert(isReal("123ere") == false); // Text after numbers
}

void isNullTests() {
    assert(isNull("null") == true);
    assert(isNull("123") == false); // Number is not null
    assert(isNull("somestring") == false); // String is not null
    assert(isNull("NULL") == false); // Register sensitive
}

void isTrueTests() {
    assert(isTrue("true") == true);
    assert(isTrue("True") == false); // Register sensitive
    assert(isTrue("125") == false); // Number
    assert(isTrue("\"randstring\"") == false); // Random json format string
    assert(isTrue("") == false); // Empty string
}

void isFalseTests() {
    assert(isFalse("false") == true);
    assert(isFalse("False") == false); // Register sensitive
    assert(isFalse("125") == false); // Number
    assert(isFalse("\"randstring\"") == false); // Random json format string
    assert(isFalse("") == false); // Empty string
}

void isStringTests() {
    assert(isString("\"random string\"") == true); // Simple test
    assert(isString("\"With quotes \\\" \"") == true); // With escaped quotes inside string
    assert(isString("\"\"") == true); // Empty quotes
    assert(isString("") == false); // Empty string
    assert(isString("\"\"\"") == false); // Extra unescaped quotes
}

void isArrayTests() {
    assert(isArray("[]") == true); // Empty array
    assert(isArray("[1,2,3]") == true); // Simple test
    assert(isArray("[1,5,{},1e5,[5,3,[null,{}]]]") == true); // Hard test
    assert(isArray("[") == false); //
    assert(isArray("][") == false);
    assert(isArray("\"[]\"") == false);
    assert(isArray("{}") == false);
}

void isObjectTests() {
    assert(isObject("{}") == true);
    assert(isObject("{\"type\":\"home\",\"number\":\"212 555-1234\"}") == true);
    assert(isObject("{2, 3, 5}") == false);
    assert(isObject("{") == false);
    assert(isObject("{]") == false);
}

void jsonTypesTests() {
    isIntegerTests();
    isRealTests();
    isNullTests();
    isStringTests();
    isFalseTests();
    isTrueTests();
    isArrayTests();
    isObjectTests();
}

void vectorsTest1() {
    struct Vector a;
    struct Vector b;
    a.values = (char **) malloc(sizeof(char **) * 4);
    b.values = (char **) malloc(sizeof(char **) * 4);
    a.length = 3;
    b.length = 3;
    a.values[0] = "testS1";
    a.values[1] = "testS2";
    a.values[2] = "testS3";
    b.values[0] = "testS1";
    b.values[1] = "testS2";
    b.values[2] = "testS3";
    assert(vectorsEquals(a, b) == true);
}

void vectorsTest2() {
    struct Vector a;
    struct Vector b;
    a.values = (char **) malloc(sizeof(char **) * 4);
    b.values = (char **) malloc(sizeof(char **) * 4);
    a.length = 3;
    b.length = 4;
    a.values[0] = "testS1";
    a.values[1] = "testS2";
    a.values[2] = "testS3";
    b.values[0] = "testS1";
    b.values[1] = "testS2";
    b.values[2] = "testS3";
    b.values[2] = "testS4";
    assert(vectorsEquals(a, b) == false);
}

void vectorsTest3() {
    struct Vector a;
    struct Vector b;
    a.values = (char **) malloc(sizeof(char **) * 4);
    b.values = (char **) malloc(sizeof(char **) * 4);
    a.length = 3;
    b.length = 3;
    a.values[0] = "testS1";
    a.values[1] = "tes1tS2";
    a.values[2] = "testS3";
    b.values[0] = "t1estS1";
    b.values[1] = "testS2";
    b.values[2] = "testS3";
    assert(vectorsEquals(a, b) == false);
}



void helpersTests() {
    vectorsTest1();
    vectorsTest2();
    vectorsTest3();
}

void stackTests() {
    struct Stack stack;
    init(&stack);
    assert(isEmpty(&stack) == true);
    push(&stack, '{');
    push(&stack, '(');
    push(&stack, '*');
    assert(pop(&stack) == '*');
    assert(pop(&stack) == '(');
    push(&stack, '&');
    assert(pop(&stack) == '&');
    assert(pop(&stack) == '{');
    assert(isEmpty(&stack) == true);
    clear(&stack);
}

void startTests() {
    jsonTypesTests();
    stackTests();
    helpersTests();
    printf("All tests passed!\n");
}