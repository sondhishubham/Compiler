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
		int scope_size;		//Also the register assigned to a variable;
		binding(char* id, SYMBOL_TYPE t, int size){
			identifier  = id;
			type 	    = t;
			scope_size	= size;
		}
};
