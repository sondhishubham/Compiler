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
	MUL_ASSIGNN,
	DIV_ASSIGNN,
	MOD_ASSIGNN,
	ADD_ASSIGNN,
	SUB_ASSIGNN,
	LEFT_ASSIGNN,
	RIGHT_ASSIGNN,
	AND_ASSIGNN,
	XOR_ASSIGNN,
	OR_ASSIGNN,
    DECLARATION,		// int m, DECLARATION is a binary op with first val = int and second val = m
    FUNC_DECLARATOR,
    CODE_SECTIONS,
    
    CONTINUEE,
    BREAKK,
    RETURNN,
    POINTER,
    CONSTT,
    ELLIPSISS,
    
    IFTHEN,
    WHILEE,
    DOWHILE,
    GOTOO,
    
    TYPE_VOID,
    TYPE_INT,
    TYPE_SHORT,
    TYPE_LONG,
    TYPE_DOUBLE,
    TYPE_BOOL,
    TYPE_CHAR,
    TYPE_FLOAT,
    
    INITIALIZE,			// int m = 3, (m,3) is the binary op initialize
    DECLARATION_LIST,  // int m,j,k,n (m,j,k,n is the list)
    BLOCK,
    PARAMETERS,
    FUNC_DEF,
    ARGUMENTS,
    FUNC_CALL
};

typedef enum Symbol SYMBOL;


class NODE{
    public:
        NODE(enum Symbol sym, void* val, int nChildren){
            symbol = sym;
            value = val;
            children = (NODE*)malloc(sizeof(NODE)*nChildren);
            bp = children;
            const_bp = children;
            numChildren = nChildren;
        }
        enum Symbol symbol;
        void* value;
        int numChildren;
        NODE* children;
        NODE* bp;
        NODE* const_bp;
};


