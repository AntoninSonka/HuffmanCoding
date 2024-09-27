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


// file.c

char* readFile(const char*, int*);

void writeCodeToFile(char*, int, struct Node*);

// huffman.c

struct Node** createNodeArr(char*, int, int*);

void heapify(struct Node** arr, int arrSize);

struct Node* buildHuffmanTree(struct Node**, int);

void freeTree(struct Node*);

void generateCodes(struct Node*);

void printTree(struct Node*, bool);

#endif //! HUFFMAN_H
