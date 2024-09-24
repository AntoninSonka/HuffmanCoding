#ifndef HUFFMAN_H
#define HUFFMAN_H


struct Node {
    char ch;
    int frequency;
    struct Node* right;
    struct Node* left;
};


// file.c

char* readFile(const char*, int*);

// huffman.c

struct Node* createNode(char);

struct Node** createNodeArr(char*, int, int*);

void heapify(struct Node** arr, int arrSize);

#endif //! HUFFMAN_H
