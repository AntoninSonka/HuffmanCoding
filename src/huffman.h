#ifndef HUFFMAN_H
#define HUFFMAN_H

#include <stdbool.h>

struct Node {
    char ch;
    int frequency;
    struct Node* right;
    struct Node* left;
};


// file.c

char* readFile(const char*, int*);

// huffman.c

struct Node** createNodeArr(char*, int, int*);

void heapify(struct Node** arr, int arrSize);

struct Node* buildHuffmanTree(struct Node**, int);

void freeTree(struct Node*);

void printTree(struct Node*, bool);

#endif //! HUFFMAN_H
