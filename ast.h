#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

enum Symbol{
    PLUS,
    MULT,
    INTEGER
};

typedef enum Symbol SYMBOL;


class NODE{
    public:
        NODE(enum Symbol sym, void* val, int nChildren){
            symbol = sym;
            value = val;
            children = (NODE*)malloc(sizeof(NODE)*nChildren);
            bp = children;
        }
        enum Symbol symbol;
        void* value;
        int numChildren;
        NODE* children;
        NODE* bp;
};


