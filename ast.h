#include <stdio.h>
#include <stdlib.h>
#include <iostream>
using namespace std;

enum Symbol{

	
    STRING,
    IDENT,
    INTEGER,
    
    PLUS,
    SUB,
    DIVIDE,
    REMAINDER,
    LEFT_SHIFT,
    RIGHT_SHIFT,
    LESS_THAN_EQUAL_TO,
    GREATER_THAN_EQUAL_TO,
    LESS_THAN,
    GREATER_THAN,
    EQUAL_TO,
    NOT_EQUAL_TO,
    AND,
    EXCLUSIVE_OR,
    INCLUSIVE_OR,
    LOGICAL_AND,
    LOGICAL_OR,
    MULT,
    ASSIGN,
    DECLARATION,
    FUNC_DECLARATOR,
    
    TYPE_VOID,
    TYPE_INT,
    TYPE_SHORT,
    TYPE_LONG,
    TYPE_DOUBLE,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_FLOAT,
    
    BLOCK,
    PARAMETERS,
    FUNC_DEF
};

typedef enum Symbol SYMBOL;


class NODE{
    public:
        NODE(enum Symbol sym, void* val, int nChildren){
            symbol = sym;
            value = val;
            children = (NODE*)malloc(sizeof(NODE)*nChildren);
            bp = children;
            numChildren = nChildren;
        }
        enum Symbol symbol;
        void* value;
        int numChildren;
        NODE* children;
        NODE* bp;
};


