#include <iostream>
#include "symbol_table.h"
#include <stdio.h>
#include <string.h>
using namespace std;

extern bool isChanged;
extern void printTree(NODE*);
extern bool isChanged;
extern int doesExist(char*, SYMBOL_TYPE);
extern bool isPreviouslyDeclared(char*);
extern void enterScope();
extern void exitScope();
extern int sizeAssigned;
extern binding* symbol_table;
extern binding* bp;			// entries start from (bp+1), First entry is block;
extern int numEntries;
extern void initialize_stack();
extern void increaseStackSize();
extern void printStack();
extern void printEntry(binding);
extern void putVal(char*, int, bool);
extern binding* getVaribleInfo(char*);
extern NODE* createUnaryNode(SYMBOL, NODE*);
extern NODE* createBinaryNode(SYMBOL, NODE*, NODE*);

bool isloop;

void startCopyPropagation(NODE*);
void propagateCopyDeclarations(NODE*);
void propagateCopyFunction(NODE*);
void makeUnavailable(char*);
void copyPropagation(NODE* p){
	isloop = false;
	initialize_stack();
	startCopyPropagation(p);
	exitScope();
}


void startCopyPropagation(NODE* ptr){
//	printTree(ptr); cout << "\n\n";
	if(ptr == NULL || ptr->symbol == INTEGER||ptr->symbol == STRING || ptr->symbol == ELLIPSISS) return;
	if(isloop){
		if(ptr->symbol == RETURNN) return;
		if(ptr->symbol == IDENT){
			binding* info			= getVaribleInfo((char*)ptr->value);
			makeUnavailable((char*)ptr->value);
			info->isValueAvailable	= false;
			return;
		}
		else if(ptr->symbol == BLOCK){
			enterScope();
			ptr->bp = ptr->const_bp;
			while(ptr->bp != ptr->children){
				startCopyPropagation(ptr->bp++);
			}
			exitScope();
			return;
		}
		else{
			ptr->bp = ptr->const_bp;
			while(ptr->bp != ptr->children){
				startCopyPropagation(ptr->bp++);
			}
			return;
		}
	}
	if(ptr->symbol == IDENT){
		ptr->bp 			= ptr->const_bp;
		char* variableName 	= (char*)ptr->value;
		binding* info 		= getVaribleInfo(variableName);
		if(info->isValueAvailable){
			binding* copyVal	= info->copyIndent;
			if(copyVal >= symbol_table){
				return;
			}
			ptr->value		= (void*)copyVal->identifier;
			isChanged		= true;
		}
		return;
	}
	if(ptr->symbol == ASSIGN){
		ptr->bp 				= ptr->const_bp;
		NODE* lval 				= ptr->bp++;
		char* lvariableName 	= (char*)lval->value;
		NODE* rval 				= ptr->bp++;
		makeUnavailable(lvariableName);
		startCopyPropagation(rval);
		if(rval->symbol == IDENT){
			char* rvariableName	= (char*)rval->value;
			if(strcmp(lvariableName,rvariableName)==0){
				*ptr			= *(new NODE(BLOCK,NULL,0));
				return;
			}
			binding* rinfo			= getVaribleInfo(rvariableName);
			binding* linfo	 		= getVaribleInfo(lvariableName);
			linfo->isValueAvailable	= true;
			linfo->copyIndent		= rinfo;
			return;
		}
		else{
			binding* linfo 			= getVaribleInfo(lvariableName);
			linfo->isValueAvailable	= false;
		}
		return;
	}
	else if(ptr->symbol == MUL_ASSIGNN || ptr->symbol == DIV_ASSIGNN || ptr->symbol == MOD_ASSIGNN || ptr->symbol == ADD_ASSIGNN || ptr->symbol == SUB_ASSIGNN || ptr->symbol == LEFT_ASSIGNN || ptr->symbol == RIGHT_ASSIGNN || ptr->symbol == XOR_ASSIGNN || ptr->symbol == AND_ASSIGNN || ptr->symbol == OR_ASSIGNN){
		ptr->bp 				= ptr->const_bp;
		NODE* lval 				= ptr->bp++;
		char* lvariableName 	= (char*)lval->value;
		NODE* rval 				= ptr->bp++;
		startCopyPropagation(rval);
		binding* linfo			= getVaribleInfo(lvariableName);
		linfo->isValueAvailable	= false;
		makeUnavailable(lvariableName);
		return;
	}
	else if(ptr->symbol == RETURNN && ptr->numChildren != 0){
		NODE* returnVal = ptr->const_bp;
		startCopyPropagation(returnVal);
		return;
	}
	else if(ptr->symbol == DECLARATION){
		ptr->bp = ptr->const_bp;
		propagateCopyDeclarations(ptr);
		return;
	}
	else if(ptr->symbol == FUNC_DEF){
		ptr->bp = ptr->const_bp;
		propagateCopyFunction(ptr);
		return;
	}
	else if(ptr->symbol == WHILEE){
		ptr->bp 	= ptr->const_bp;
		isloop 		= true;
		startCopyPropagation(ptr->bp++);
		startCopyPropagation(ptr->bp++);
		isloop		= false;
		return;
	}
	else if(ptr->symbol == BLOCK){
		enterScope();
		ptr->bp = ptr->const_bp;
		while(ptr->bp != ptr->children){
			startCopyPropagation(ptr->bp++);
		}
		exitScope();
		return;
	}
	else{
		ptr->bp = ptr->const_bp;
		while(ptr->bp != ptr->children){
			startCopyPropagation(ptr->bp++);
		}
	}	
	return;
}

void makeUnavailable(char* k){
	binding* curr_ptr = symbol_table - 1;
	while(curr_ptr != bp){
		if(curr_ptr->type == Block){
			curr_ptr--;
			continue;
		}
		if(strcmp(curr_ptr->identifier,k) == 0) return;
		if(curr_ptr->isValueAvailable){
			binding* equalIdent	= curr_ptr->copyIndent;
			if(strcmp(equalIdent->identifier,k)==0){
				curr_ptr->isValueAvailable = false;
			}
		}
		curr_ptr--;
	}
	return;
}

void propagateCopyFunction(NODE* ptr){
	ptr->bp = ptr->const_bp;
	ptr->bp++;
	NODE* ident				= (ptr->bp++); ident->bp = ident->const_bp;
	NODE* function_body 	= (ptr->bp++); function_body->bp = function_body->const_bp;
	while (ident->symbol == POINTER) ident = ident->const_bp;								// To take care of pointers while declaraion
	NODE* name 				= (ident->bp++); name->bp = name->const_bp;
	NODE* parameters		= (ident->bp == ident->children)?(NULL):(ident->bp++);	//NULL means no parameters
	char* identifier_name 	= (char*)name->value;
	binding* entry = new binding(identifier_name, Function, -1);
	entry->isValueAvailable = false;
	if(symbol_table-bp == sizeAssigned)
		increaseStackSize();
	*(symbol_table++) = *entry;
	numEntries++;
	
	enterScope();
//	if(parameters->symbol == PARAMETERS) cout << "PARAMETER
	bool hasSeenEllipsis = false;
	if (parameters!=NULL){
		parameters->bp = parameters->const_bp;
		while(parameters->bp != parameters->children){
			if(parameters->bp->symbol != ELLIPSISS){
				NODE* declaration = parameters->bp; declaration->bp = declaration->const_bp;
				NODE* declaration_specifier 	= declaration->bp++; declaration_specifier->bp = declaration_specifier->const_bp;
				while (declaration_specifier->symbol	== CONSTT) declaration_specifier = declaration_specifier->const_bp;
				SYMBOL_TYPE t;	
				switch (declaration_specifier->symbol){
					case TYPE_INT:
						t = Integer_type;
						break;
					case TYPE_BOOL:
						t = Bool_type;
						break;
					case TYPE_CHAR:
						t = Character_type;
						break;
				}
				NODE* variable = declaration->bp++;
				while(variable->symbol == POINTER) variable = variable->const_bp;
				char* identifier_name = (char*)variable->value;
				binding* en = new binding(identifier_name, t, 0);
				if(symbol_table - bp == sizeAssigned)
					increaseStackSize();
				*(symbol_table++) = *en;
				numEntries++;
			}
			else if(parameters->bp->symbol == ELLIPSISS){
				hasSeenEllipsis = true;
			}
			parameters->bp++;
		}
	}
	while(function_body->bp != function_body->children){
		startCopyPropagation(function_body->bp++);
	}
	exitScope();
	return;
}



void propagateCopyDeclarations(NODE* ptr){
	ptr->bp = ptr->const_bp;
	NODE* declaration_specifier = ptr->bp++; declaration_specifier->bp = declaration_specifier->const_bp;
	NODE* declaration_list = ptr->bp++; declaration_list->bp = declaration_list->const_bp;
	SYMBOL_TYPE t = Variable;
	int numDeclarations = declaration_list->children - declaration_list->bp;
	int variable_value = -1;
	if(declaration_specifier->symbol == CONSTT) declaration_specifier = declaration_specifier->const_bp;
	switch (declaration_specifier->symbol){
		case TYPE_INT:
			t = Integer_type;
			break;
		case TYPE_BOOL:
			t = Bool_type;
			break;
		case TYPE_CHAR:
			t = Character_type;
			break;
	}
	
	while(declaration_list->bp != declaration_list->children){
		bool isInitiazlized = false;
		binding* rIdentifier;
		NODE* declarator = declaration_list->bp; declarator->bp = declarator->const_bp;
		NODE* ident;
		if(declarator->symbol == INITIALIZE){ 			//check if the initializer part is correct or not, a = 10, 10 is correct or not
			ident  = declarator->bp++;
			while(ident->symbol == POINTER) ident = ident->const_bp;
			NODE* initializer = declarator->bp++; initializer->bp = initializer->const_bp;
			startCopyPropagation(initializer);
			
			if(initializer->symbol == IDENT){
				isInitiazlized = true;
				rIdentifier = getVaribleInfo((char*)initializer->value);
			}
		}
		else if(declarator->symbol == FUNC_DECLARATOR){	 // Don't need function declarations for copy propagation.
			t = Function;
			ident = declarator->bp++;
			isInitiazlized = false;
		}
		else     													//In case when there is simple declaration like int b,v,d; No initializing and no function declaraions
			ident = declarator;
		while (ident->symbol == POINTER){
			ident = ident->const_bp;			// To take care of pointers while declaraion
		}
		if(ident->symbol == FUNC_DECLARATOR){
			t = Function;
			isInitiazlized = false;
			ident = declarator->bp++;
		}
		char* identifier_name = (char*)ident->value;
		binding* entry 		= new binding(identifier_name, t, 0);
		entry->copyIndent	= (isInitiazlized)?(rIdentifier):(NULL);
		entry->isValueAvailable = isInitiazlized;
		if(symbol_table-bp == sizeAssigned)
			increaseStackSize();
		*(symbol_table++) = *entry;
		numEntries++;
		declaration_list->bp++;
	}
	return;
}	
