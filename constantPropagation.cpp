#include <iostream>
#include "symbol_table.h"
#include <stdio.h>
#include <string.h>
using namespace std;

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


bool isLoop;

void startPropagation(NODE*);
void propagateDeclarations(NODE*);
void propagateFunction(NODE*);
void replaceIdent(NODE*);


void constantPropagation(NODE* p){
	isLoop = false;
	initialize_stack();
	startPropagation(p);
	exitScope();
}


void startPropagation(NODE* ptr){
	if(ptr == NULL || ptr->symbol == INTEGER||ptr->symbol == STRING || ptr->symbol == ELLIPSISS) return;
	if(isLoop){
		if(ptr->symbol == RETURNN) return;
		if(ptr->symbol == IDENT){
			binding* info			= getVaribleInfo((char*)ptr->value);
			info->isValueAvailable	= false;
			return;
		}
		else if(ptr->symbol == BLOCK){
			enterScope();
			ptr->bp = ptr->const_bp;
			while(ptr->bp != ptr->children){
				startPropagation(ptr->bp++);
			}
			exitScope();
			return;
		}
		else if(ptr->symbol == DECLARATION){
			ptr->bp = ptr->const_bp;
			propagateDeclarations(ptr);
			return;
		}
		else{
			ptr->bp = ptr->const_bp;
			while(ptr->bp != ptr->children){
				startPropagation(ptr->bp++);
			}
			return;
		}
	}
	if(ptr->symbol == IDENT) return;
	if(ptr->symbol == ASSIGN){
		ptr->bp 			= ptr->const_bp;
		NODE* lval 			= ptr->bp++;
		char* variableName 	= (char*)lval->value;
		NODE* rval 			= ptr->bp++;
		startPropagation(rval);
		if(rval->symbol == INTEGER){
			putVal(variableName, *((int*)rval->value), true);
		}
		else if(rval->symbol == IDENT){
			int* val = (int*)malloc(sizeof(int));
			binding* entry = getVaribleInfo((char*)rval->value);
			if(entry->isValueAvailable && (entry->type == Integer_type || entry->type == Character_type || entry->type == Bool_type)){
				if(entry->type == Integer_type){
					*val 		= entry->scope_size;
				}
				else if(entry->type == Character_type){
					char ans 	= entry->scope_size;
					*val 		= ans;
				}
				else if(entry->type == Bool_type){
					bool ans 	= entry->scope_size;
					*val 		= ans;
				}
				rval->symbol 		= INTEGER;
				rval->value 		= (void*)val;
				rval->numChildren 	= 0;
				rval->bp			= rval->const_bp;
				rval->children		= rval->const_bp;
				putVal(variableName, *val, true);
			}
			else
				putVal(variableName, -1, false);
		}
		else{
			putVal(variableName, -1, false);
		}
		return;
	}
	else if(ptr->symbol == MUL_ASSIGNN || ptr->symbol == DIV_ASSIGNN || ptr->symbol == MOD_ASSIGNN || ptr->symbol == ADD_ASSIGNN || ptr->symbol == SUB_ASSIGNN || ptr->symbol == LEFT_ASSIGNN || ptr->symbol == RIGHT_ASSIGNN || ptr->symbol == XOR_ASSIGNN || ptr->symbol == AND_ASSIGNN || ptr->symbol == OR_ASSIGNN){
		bool convert	= false;
		ptr->bp			= ptr->const_bp;
		NODE* lval		= ptr->bp++;
		NODE* rval		= ptr->bp++;
		startPropagation(rval);
		char* lvarName	= (char*)lval->value;
		binding* lentry 	= getVaribleInfo(lvarName);
		if(lentry->isValueAvailable && (lentry->type == Integer_type || lentry->type == Character_type || lentry->type == Bool_type)){
			if(lentry->scope_size == 0 || (lentry->scope_size == 1 && ptr->symbol == MUL_ASSIGNN) || ptr->symbol == OR_ASSIGNN || ptr->symbol == AND_ASSIGNN)
				convert = true;
		}
		if(rval->symbol == INTEGER){
			int v = *((int*)rval->value);
			if( v == 0 || (v == 1 && (ptr->symbol == MUL_ASSIGNN||ptr->symbol == DIV_ASSIGNN)) || ptr->symbol == OR_ASSIGNN || ptr->symbol == AND_ASSIGNN)
					convert = true;
			if(lentry->isValueAvailable && (lentry->type == Integer_type || lentry->type == Character_type || lentry->type == Bool_type))
					convert = true;			  	
		}
		if(rval->symbol == IDENT){
			char* rvarName	= (char*)rval->value;
			binding* rentry 	= getVaribleInfo(rvarName);
			if(rentry->isValueAvailable && (rentry->type == Integer_type || rentry->type == Character_type || rentry->type == Bool_type)){
				if(rentry->scope_size == 0 || (rentry->scope_size == 1 && (ptr->symbol == MUL_ASSIGNN||ptr->symbol == DIV_ASSIGNN)) || ptr->symbol == OR_ASSIGNN || ptr->symbol == AND_ASSIGNN)
					convert = true;
			}
			if((lentry->isValueAvailable && (lentry->type == Integer_type || lentry->type == Character_type || lentry->type == Bool_type))
			  &&(rentry->isValueAvailable && (rentry->type == Integer_type || rentry->type == Character_type || rentry->type == Bool_type)))
			  	convert = true;
		}
		if(convert){
			SYMBOL op;
			switch(ptr->symbol){
				case MUL_ASSIGNN:
					op = MULT;
					break;
				case DIV_ASSIGNN:
					op = DIVIDE;
					break;
				case MOD_ASSIGNN:
					op = REMAINDER;
					break;
				case ADD_ASSIGNN:
					op = PLUS;
					break;
				case SUB_ASSIGNN:
					op = SUB;
					break;
				case LEFT_ASSIGNN:
					op = LEFT_SHIFT;
					break;
				case RIGHT_ASSIGNN:
					op = RIGHT_SHIFT;
					break;
				case XOR_ASSIGNN:
					op = EXCLUSIVE_OR;
					break;
				case AND_ASSIGNN:
					op = AND;
					break;
				case OR_ASSIGNN:
					op = INCLUSIVE_OR;
					break;
			}
			NODE* copiedlVal 	= new NODE(IDENT,lvarName,0) ;
			NODE* rightTree 	= createBinaryNode(op, copiedlVal, rval);
			startPropagation(rightTree);
			NODE* newTree		= createBinaryNode(ASSIGN, lval, rightTree);
			*ptr				= *newTree;
		}
		putVal(lvarName, -1, false);
		return;
	}
	else if(ptr->symbol == PLUS || ptr->symbol == SUB || ptr->symbol == MULT || ptr->symbol == DIVIDE || ptr->symbol == REMAINDER || ptr->symbol == LEFT_SHIFT || ptr->symbol == RIGHT_SHIFT || ptr->symbol == EXCLUSIVE_OR || ptr->symbol == INCLUSIVE_OR || ptr->symbol == AND || ptr->symbol == LOGICAL_AND || ptr->symbol == LOGICAL_OR || ptr->symbol == LESS_THAN || ptr->symbol == GREATER_THAN || ptr->symbol == LESS_THAN_EQUAL_TO || ptr->symbol == GREATER_THAN_EQUAL_TO || ptr->symbol == EQUAL_TO || ptr->symbol == NOT_EQUAL_TO){
		ptr->bp = ptr->const_bp;
		NODE* firstNumber 	= ptr->bp++;
		NODE* secondNumber 	= ptr->bp++;
		startPropagation(firstNumber);
		startPropagation(secondNumber);
		if(firstNumber->symbol == IDENT)
			replaceIdent(firstNumber);
		if(secondNumber->symbol == IDENT){
			replaceIdent(secondNumber);
		return;
		}
	}
	else if(ptr->symbol == RETURNN && ptr->numChildren != 0){
		NODE* returnVal = ptr->const_bp;
		startPropagation(returnVal);
		if(returnVal->symbol == IDENT) replaceIdent(returnVal);
		return;
	}
	else if(ptr->symbol == IFTHEN){
		ptr->bp 	= ptr->const_bp;
		NODE* cond 	= ptr->const_bp; 
		while(ptr->bp != ptr->children) startPropagation(ptr->bp++);
		if(cond->symbol == IDENT)
			replaceIdent(cond);
		return;
	}
	else if(ptr->symbol == WHILEE){
		ptr->bp 	= ptr->const_bp;
		isLoop 		= true;
		NODE* cond	= ptr->bp++;
		NODE* loop	= ptr->bp++;
		startPropagation(cond);
		startPropagation(loop);
		isLoop 		= false;
		return;
	}
	else if(ptr->symbol == DECLARATION){
		ptr->bp = ptr->const_bp;
		propagateDeclarations(ptr);
		return;
	}
	else if(ptr->symbol == FUNC_DEF){
		ptr->bp = ptr->const_bp;
		propagateFunction(ptr);
		return;
	}
	else if(ptr->symbol == BLOCK){
		enterScope();
		ptr->bp = ptr->const_bp;
		while(ptr->bp != ptr->children){
			startPropagation(ptr->bp++);
		}
		exitScope();
		return;
	}
	else{
		ptr->bp = ptr->const_bp;
		while(ptr->bp != ptr->children){
			startPropagation(ptr->bp++);
		}
	}
	return;
}

void replaceIdent(NODE* firstNumber){
	int* val = (int*)malloc(sizeof(int));
	binding* entry = getVaribleInfo((char*)firstNumber->value);
	if(entry->isValueAvailable && (entry->type == Integer_type || entry->type == Character_type || entry->type == Bool_type)){
		isChanged = true;
		if(entry->type == Integer_type){
			*val 		= entry->scope_size;
		}
		else if(entry->type == Character_type){
			char ans 	= entry->scope_size;
			*val 		= ans;
		}
		else if(entry->type == Bool_type){
			bool ans 	= entry->scope_size;
			*val 		= ans;
		}
		firstNumber->symbol 		= INTEGER;
		firstNumber->value 			= (void*)val;
		firstNumber->numChildren 	= 0;
		firstNumber->children		= firstNumber->const_bp;
		firstNumber->bp				= firstNumber->const_bp;
	}
	return;
}

void propagateFunction(NODE* ptr){
	ptr->bp = ptr->const_bp;
	ptr->bp++;
	NODE* ident				= (ptr->bp++); ident->bp = ident->const_bp;
	NODE* function_body 	= (ptr->bp++); function_body->bp = function_body->const_bp;
	while (ident->symbol == POINTER) ident = ident->const_bp;								// To take care of pointers while declaraion
	NODE* name 				= (ident->bp++); name->bp = name->const_bp;
	NODE* parameters		= (ident->bp == ident->children)?(NULL):(ident->bp++);	//NULL means no parameters
	char* identifier_name 	= (char*)name->value;
	binding* entry = new binding(identifier_name, Function, -1);
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
				NODE* declaration_specifier 	= declaration->bp++;
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
				binding* en = new binding(identifier_name, t, -1);
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
		startPropagation(function_body->bp++);
	}
	exitScope();
	return;
}



void propagateDeclarations(NODE* ptr){
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
		NODE* declarator = declaration_list->bp; declarator->bp = declarator->const_bp;
		NODE* ident;
		if(declarator->symbol == INITIALIZE){ 			//check if the initializer part is correct or not, a = 10, 10 is correct or not
			ident  = declarator->bp++;
			while(ident->symbol == POINTER) ident = ident->const_bp;
			NODE* initializer = declarator->bp++; initializer->bp = initializer->const_bp;
			startPropagation(initializer);
			int ans; bool ans1; char ans2;
			if(initializer->symbol == INTEGER){
				isInitiazlized = true;
				switch(t){
					case Integer_type:
						ans = *((int*)initializer->value);
						variable_value = ans;
						break;
					case Bool_type:
						ans1 = *((int*)initializer->value);
						variable_value = ans1;
						break;
					case Character_type:
						ans2 = *((int*)initializer->value);
						variable_value = ans2;
						break;
				}
			}
			if(initializer->symbol == IDENT){
				binding* entry = getVaribleInfo((char*)initializer->value);
				if(entry->isValueAvailable && (entry->type == Integer_type || entry->type == Character_type || entry->type == Bool_type)){
					isInitiazlized = true;
					variable_value = entry->scope_size;
					}
				replaceIdent(initializer);	
			}
		}
		else if(declarator->symbol == FUNC_DECLARATOR){	//In the case of declaration of Function like "void printf();"
			t = Function;
			ident = declarator->bp++;
		}
		else     													//In case when there is simple declaration like int b,v,d; No initializing and no function declaraions
			ident = declarator;
		while (ident->symbol == POINTER){
			ident = ident->const_bp;			// To take care of pointers while declaraion
//			t = Pointer_type;
		}
		if(ident->symbol == FUNC_DECLARATOR){
			t = Function;
			ident = ident->const_bp;
		}
		char* identifier_name = (char*)ident->value;
		binding* entry = new binding(identifier_name, t, variable_value);
		entry->scope_size = variable_value;
		entry->isValueAvailable = isInitiazlized;
		if(symbol_table-bp == sizeAssigned)
			increaseStackSize();
		*(symbol_table++) = *entry;
		numEntries++;
		declaration_list->bp++;
	}
	return;
}

