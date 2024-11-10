#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>
#include <stdint.h>

typedef struct Node {
    char ch;
    int frequency;
    struct Node* right;
    struct Node* left;
}Node;

typedef struct {
    uint16_t code;
    int depth; // is also number of bits from rigt
    char ch;
    int frequency;
}Code;

typedef struct {
    uint8_t identefire;
    char ch;
}Tree;

// file.c

char* readFile(const char*, int*);

// huffman.c

Node** createNodeArr(char*, int, int*);

void heapify(Node**, int);

Node* buildHuffmanTree(Node**, int);

void freeTree(Node*);

void printTree(Node*, bool);

Code** assignCodes(Node*, int, int*);

Tree** assignTree(Node*, int);

uint16_t createFileHead(int, int);

uint8_t* treeToBin(Tree**, int, int*, int*);

uint8_t* textToBin(char*, int, Code**, int, int*, int*);

#endif //! HUFFMAN_H
