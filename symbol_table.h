using namespace std;

enum Type{
	Variable,
	Function,
	Block 			// A new scope
};

typedef enum Type SYMBOL_TYPE;

class binding{
	public:
		char* identifier;
		SYMBOL_TYPE type;
		int scope_size;		//Also the number of pointers in a type. for ex, char** has type pointer and ret_type char and nArguments = 2;
		binding(char* id, SYMBOL_TYPE t, int size){
			identifier  = id;
			type 	    = t;
			scope_size	= size;
		}
};
