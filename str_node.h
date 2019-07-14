#ifndef STRUCT_NODE_H
#define STRUCT_NODE_H

#include "global.h"

struct Node
{
    Node* next[8] = {};
    int occupied = WALL;
    int row, col;
};

#endif
