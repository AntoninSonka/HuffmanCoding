#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>
#include <stdint.h>

struct Node {
    char ch;
    int frequency;
    struct Node* right;
    struct Node* left;
};

struct Code {
    uint16_t code;
    int depth; // is also number of bits from rigt
    char ch;
    int frequency;
};

struct Tree {
    uint8_t identefire;
    char ch;
};

// file.c

char* readFile(const char*, int*);

// void writeCodeToFile(char*, int, struct Node*);

// huffman.c

struct Node** createNodeArr(char*, int, int*);

void heapify(struct Node**, int);

struct Node* buildHuffmanTree(struct Node**, int);

void freeTree(struct Node*);

void printTree(struct Node*, bool);

struct Code** assignCodes(struct Node*, int, int*);

struct Tree** assignTree(struct Node*, int);

uint16_t createFileHead(int, int);

#endif //! HUFFMAN_H
