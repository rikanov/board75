#ifndef STRUCT_NODE_H
#define STRUCT_NODE_H

#include "global.h"

struct Node
{
    Node* next[8] = {};
    int occupied = WALL;
    int row = 0, col = 0;
	bool operator == (const Node& N) const {
		return row == N.row && col == N.col;
	}
	bool operator != (const Node& N) const {
		return row != N.row || col != N.col;
	}
};

#endif
