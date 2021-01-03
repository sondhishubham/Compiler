#include <iostream>
#include "symbol_table.h"
#include <stdio.h>
#include <string.h>
using namespace std;


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
extern void remove(NODE*);
extern void markWholeDeclarations(NODE*);
extern void removeWholeDeclarations(NODE*);

void removeDeadCode(NODE*);
void markDeadCode(NODE*);
void markDeclarations(NODE*);
void markFunctions(NODE*);



void removeDeadCode(NODE*p){
	initialize_stack();
	markDeadCode(p);
	remove(p);
	markWholeDeclarations(p);
	removeWholeDeclarations(p);
	exitScope();
//	free(bp);
}


void markDeadCode(NODE* ptr){
	if(ptr == NULL || ptr->symbol == INTEGER||ptr->symbol == STRING || ptr->symbol == ELLIPSISS)
		return;
	else if(ptr->symbol == DECLARATION){
		markDeclarations(ptr);
		return;
		}
	else if(ptr->symbol == FUNC_DEF){
		ptr->bp 			= ptr->const_bp;
		ptr->bp++;
		NODE* ident 		= ptr->bp++; ident->bp = ident->const_bp;
		while (ident->symbol == POINTER) ident = ident->const_bp;
		NODE* name 			= (ident->bp++); name->bp = name->const_bp;
		char* identifier	= (char*)name->value;
		string str 			= "main";
		const char *cstr = str.c_str();
		if(strcmp(identifier,cstr)==0){ 
			ptr->isNotNeeded = false;
		}
		markFunctions(ptr);
		return;
	}
	else if(ptr->symbol == FUNC_CALL){
		ptr->bp 		= ptr->const_bp;
		NODE* ident 	= ptr->bp++;
		binding* en		= getVaribleInfo((char*)ident->value);
		NODE* p			= en->astPointer;
		p->isNotNeeded	= false;
		if(ptr->bp!=ptr->children){
			markDeadCode(ptr->bp++);
		}
	}
	else if(ptr->symbol == IDENT){
		binding* en		= getVaribleInfo((char*)ptr->value);
		NODE* p			= en->astPointer;
		p->isNotNeeded	= false;
		return;
	}
	else if(ptr->symbol == BLOCK){
		if(ptr->numChildren == 0) return;
		enterScope();
		ptr->bp = ptr->const_bp;
		while(ptr->bp != ptr->children){
			markDeadCode(ptr->bp++);
		}
		exitScope();
		return;
	}
	else{
		if(ptr->numChildren > 0){
			ptr->bp	= ptr->const_bp;
			while(ptr->bp != ptr->children){
				markDeadCode(ptr->bp++);
			}
		}
	}
	return;
}


void markFunctions(NODE* ptr){
	ptr->bp = ptr->const_bp;
	ptr->bp++;
	NODE* ident				= (ptr->bp++); ident->bp = ident->const_bp;
	NODE* function_body 	= (ptr->bp++); function_body->bp = function_body->const_bp;
	while (ident->symbol == POINTER) ident = ident->const_bp;								// To take care of pointers while declaraion
	NODE* name 				= (ident->bp++); name->bp = name->const_bp;
	NODE* parameters		= (ident->bp == ident->children)?(NULL):(ident->bp++);	//NULL means no parameters
	char* identifier_name 	= (char*)name->value;
	int existss				= doesExist(identifier_name, Function);
	if(existss == 0){
		binding* e					= getVaribleInfo(identifier_name);
		NODE* prev_node				= e->astPointer;
		prev_node->inSymbolTable	= true;
		prev_node->isNotNeeded		= true;
		ptr->inSymbolTable			= true;
//		e->astPointer				= ptr;
	}
	else{
		binding* entry 		= new binding(identifier_name, Function, -1);
		ptr->inSymbolTable	= true;
		entry->astPointer	= ptr;
		if(symbol_table-bp == sizeAssigned)
			increaseStackSize();
		*(symbol_table++) = *entry;
		numEntries++;
	}
	enterScope();
	bool hasSeenEllipsis = false;
	if (parameters!=NULL){
		parameters->bp = parameters->const_bp;
		while(parameters->bp != parameters->children){
			if(parameters->bp->symbol != ELLIPSISS){
				NODE* declaration = parameters->bp; declaration->bp = declaration->const_bp;
				declaration->bp++;
				NODE* variable = declaration->bp++;
				while(variable->symbol == POINTER) variable = variable->const_bp;
				char* identifier_name 		= (char*)variable->value;
				binding* en = new binding(identifier_name, Variable, -1);
				en->astPointer				= parameters->bp;
				en->astPointer->isNotNeeded = false;
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
		markDeadCode(function_body->bp++);
	}
	exitScope();
	if(existss == 0){
		binding* e					= getVaribleInfo(identifier_name);
		NODE* prev_node				= e->astPointer;
		*prev_node					= *ptr;
		ptr->inSymbolTable			= true;
		ptr->isNotNeeded			= true;
		prev_node->inSymbolTable	= true;
		prev_node->isNotNeeded		= false;
		e->astPointer				= ptr;
	}
	return;
}


void markDeclarations(NODE* ptr){
	ptr->bp = ptr->const_bp;
	NODE* markerNode;
	NODE* declaration_specifier = ptr->bp++; declaration_specifier->bp = declaration_specifier->const_bp;
	NODE* declaration_list 	= ptr->bp++; declaration_list->bp = declaration_list->const_bp;
	int numDeclarations 	= declaration_list->children - declaration_list->bp;
	SYMBOL_TYPE t			= Variable; 
	while(declaration_list->bp != declaration_list->children){
		bool isInitiazlized = false;
		NODE* declarator = declaration_list->bp; declarator->bp = declarator->const_bp;
		NODE* ident;
		markerNode = (numDeclarations == 1)?(ptr):(declaration_list->bp);
		if(declarator->symbol == INITIALIZE){ 			//check if the initializer part is correct or not, a = 10, 10 is correct or not
			ident  = declarator->bp++;
			while(ident->symbol == POINTER) ident = ident->const_bp;
			NODE* initializer = declarator->bp++;
			markDeadCode(initializer);
		}
		else if(declarator->symbol == FUNC_DECLARATOR){	//In the case of declaration of Function like "void printf();"
			t 			= Function;
			ident 		= declarator->bp++;
			char* name	= (char*)ident->value;
			int exists	= doesExist(name, Function);
			if(exists == 0){
				markerNode->inSymbolTable	= true;
				markerNode->isNotNeeded		= true;
				return;
			}
		}
		else{     													//In case when there is simple declaration like int b,v,d; No initializing and no function declaraions
			ident = declarator;
		}
		while (ident->symbol == POINTER){
			ident = ident->bp;			// To take care of pointers while declaraion
		}
		if(ident->symbol == FUNC_DECLARATOR){
			t = Function;
			ident = ident->const_bp;
			char* name	= (char*)ident->value;
			int exists	= doesExist(name, Function);
			if(exists == 0){
				markerNode->inSymbolTable	= true;
				markerNode->isNotNeeded		= true;
				return;
			}
		}
		char* identifier_name 		= (char*)ident->value;
		binding* entry = new binding(identifier_name, t, 0);
		entry->astPointer			= markerNode;
		markerNode->inSymbolTable	= true;
		entry->isValueAvailable 	= isInitiazlized;
		if(symbol_table-bp == sizeAssigned)
			increaseStackSize();
		*(symbol_table++) = *entry;
		numEntries++;
		declaration_list->bp++;
	}
	return;
}
