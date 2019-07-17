#ifndef STR_STEP_H
#define STR_STEP_H

#include "str_node.h"
#include <iostream>

struct Step
{
    Node ** stone;
    Node * place;

public:
    Step(): stone(nullptr), place(nullptr) {}
    Step(Node ** s, Node * p): stone(s), place(p) {}
    Step(const Step&) = default;
    bool isValid() const {
        return stone != nullptr && place != nullptr;
    }
    void revertableStep()
    {
        place->occupied = (*stone)->occupied; // put down
        (*stone)->occupied = 0;              // and pick up
        std::swap(*stone, place);
    }

	bool operator == (const Step& S) const
	{
		return isValid() && S.isValid() && *stone == *S.stone && *place == *S.place;
	}

	bool operator != (const Step& S) const
	{
		return !isValid() || !S.isValid() || *stone != *S.stone || *place != *S.place;
	}

    void printData() const
    {
        printf("X%d->(%d, %d) ", (*stone)->occupied, place->row, place->col);
    }
};

#endif
