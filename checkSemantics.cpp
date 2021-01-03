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
extern binding* getVaribleInfo(char*);

int addFunction(NODE*);
int addDeclaration(NODE*);

int check_semantics(NODE* ptr){
	int answer;
	if(ptr == NULL) return 0;
	if(ptr->symbol == IDENT){
		answer =  doesExist((char*)ptr->value, Variable);//Do something about variable here
		return answer;
		}
	else if(ptr->symbol == INTEGER||ptr->symbol == STRING)
		return 0;
	else if(ptr->symbol == DECLARATION)
		return addDeclaration(ptr);
	else if(ptr->symbol == FUNC_DEF)
		return addFunction(ptr);
	else if(ptr->symbol == BLOCK){
		enterScope();
		while(ptr->bp != ptr->children){
			if(check_semantics(ptr->bp)==-1) return -1;
			ptr->bp++;
		}
		exitScope();
		return 0;
	}
	else{
		while(ptr->bp != ptr->children){
			if (check_semantics(ptr->bp) == -1) return -1;
			ptr->bp++;
		}
	}
	return 0;
}



int addFunction(NODE* ptr){
	ptr->bp++;
	NODE* ident				= (ptr->bp++);
	NODE* function_body 	= (ptr->bp++);
	while (ident->symbol == POINTER) ident = ident->bp;								// To take care of pointers while declaraion
	if(ident->symbol != FUNC_DECLARATOR){
		cout<<"Variable defined as a function"<<endl;
		return -1;
	}
	NODE* name 				= (ident->bp++);
	if(name->symbol == FUNC_DECLARATOR){
		cout<<"Function returning a function"<<endl;
		return -1;
	}
	NODE* parameters		= (ident->bp == ident->children)?(NULL):(ident->bp++);	//NULL means no parameters
	char* identifier_name 	= (char*)name->value;
	int existss = doesExist(identifier_name, Function);
	if(existss == 0){
		binding* e	= getVaribleInfo(identifier_name);
		if(e->type != Function){
			cout << identifier_name<<" was declared as a Variable\n";
			return -1;
		}
		if(e->isValueAvailable == true){
			cout << "Function "<<identifier_name<<" has already been defined\n";
			return -1;
		}
	}
	else{
		binding* entry 			= new binding(identifier_name, Function, 0);
		entry->isValueAvailable	= true;
		if(symbol_table-bp == sizeAssigned)
			increaseStackSize();
		*(symbol_table++) = *entry;
		numEntries++;
	}
	enterScope();
//	if(parameters->symbol == PARAMETERS) cout << "PARAMETER
	bool hasSeenEllipsis = false;
	if (parameters!=NULL){
		while(parameters->bp != parameters->children){
			if(hasSeenEllipsis){
				cout << "Parameter given to a Function definition after ellipsis\n";
				return -1;
			}
			if(parameters->bp->symbol != ELLIPSISS){
				NODE* declaration = parameters->bp;
				declaration->bp++;
				NODE* variable = declaration->bp++;
				while(variable->symbol == POINTER) variable = variable->bp;			//To take care of pointer
				if(variable->symbol == FUNC_DECLARATOR){
					cout << "Function given as an argument to a Function" << endl;
					return -1;
				}
				char* identifier_name = (char*)variable->value;
				if(isPreviouslyDeclared(identifier_name)) return -1;
				binding* en = new binding(identifier_name, Variable, 0);
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
		if(check_semantics(function_body->bp++) == -1) return -1;
	}
	exitScope();
	return 0;
}

int addDeclaration(NODE* ptr){
	NODE declaration_specifiers = *(ptr->bp++);
	NODE declaration_list = *(ptr->bp++);
	SYMBOL_TYPE t = Variable;
	int numDeclarations = declaration_list.children - declaration_list.bp;
	while(declaration_list.bp != declaration_list.children){
		NODE* ident;
		if((declaration_list.bp)->symbol == INITIALIZE){ 			//check if the initializer part is correct or not, a = 10, 10 is correct or not
			NODE* k = declaration_list.bp;
			ident  = k->bp++;
			while(ident->symbol == POINTER) ident = ident->bp;
			NODE* initializer = k->bp++;
			if (ident->symbol == FUNC_DECLARATOR){ 
				cout << "Function is declared as a variable"<<'\n';
				return -1;
			}
			if (check_semantics(initializer) == -1)	return -1;
		}
		else if((declaration_list.bp)->symbol == FUNC_DECLARATOR){	//In the case of declaration of Function like "void printf();"
			t = Function;
			NODE* k = declaration_list.bp;// k is the FUNC_DECLARATOR
			ident = k->bp++;
			if(ident->symbol == FUNC_DECLARATOR){
				cout << "Declaration of a function returning a function" <<endl;
				return -1;
			}
		}
		else     													//In case when there is simple declaration like int b,v,d; No initializing and no function declaraions
			ident = declaration_list.bp;
		while (ident->symbol == POINTER){
			ident = ident->bp;			// To take care of pointers while declaraion
		}
		if(ident->symbol == FUNC_DECLARATOR){
			t = Function;
			ident = ident->bp;
			if(ident->symbol == FUNC_DECLARATOR){
				cout << "Declaration of a function returning a function" <<endl;
				return -1;
			}
		}
		char* identifier_name = (char*)ident->value;
		if(t==Variable && isPreviouslyDeclared(identifier_name)){
			return -1;
		}
		if(t==Function){
			int exists 	= doesExist(identifier_name, Function);
			if(exists == 0){
				binding* e	= getVaribleInfo(identifier_name);
				if(e->type != Function){
					cout << identifier_name<<" was declared as a Variable\n";
					return -1;
				}
				declaration_list.bp++; continue;
			}
		}
		binding* entry = new binding(identifier_name, t, 0);
		if(symbol_table-bp == sizeAssigned)
			increaseStackSize();
		*(symbol_table++) = *entry;
		numEntries++;
		declaration_list.bp++;
	}
	return 0;
}


