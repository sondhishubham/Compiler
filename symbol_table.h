using namespace std;

enum Type{
	Variable,
	Function,
	Block,			// A new scope
	
		//Adding these 5 later to account for the type of a variable in cgen 
	Integer_type,
	Character_type,
	Pointer_type,
	Bool_type,
	Void_type,
	Ellipsis_type
};

typedef enum Type SYMBOL_TYPE;

class binding{
	public:
		int numPointer;		// Added to take care of the number of pointers in a variable. eg. int*** l, numPointer = 3
		char* identifier;
		SYMBOL_TYPE type;
		SYMBOL_TYPE ret_type;	//The return type of functions;
		SYMBOL_TYPE* par_types;	//The type of parameters
		int* par_pointers;		//The number of pointers in parameters;
		int numPar;
		int scope_size;		//Also the register assigned to a variable;
		binding(char* id, SYMBOL_TYPE t, int size){
			numPointer  = 0;
			identifier  = id;
			type 	    = t;
			ret_type	= t;
			par_pointers= 0;
			scope_size	= size;
			numPar		= 0;
		}
};
