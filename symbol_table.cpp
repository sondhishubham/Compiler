#include <iostream>
#include "symbol_table.h"
#include <stdio.h>
#include <string.h>

using namespace std;

int doesExist(char*, SYMBOL_TYPE);
bool isPreviouslyDeclared(char*);
void enterScope();
void exitScope();

int sizeAssigned = 16;
binding* symbol_table;
binding* bp;			// entries start from (bp+1), First entry is block;
int numEntries;
void initialize_stack();
void increaseStackSize();
void printStack();
void printEntry(binding);
void putVal(char*, int, bool);
binding* getVaribleInfo(char*);



void initialize_stack(){
	symbol_table = (binding*) malloc(sizeAssigned*sizeof(binding));
	bp = symbol_table;
	binding* entry = new binding(NULL, Block, 0);
	*(symbol_table++) = *entry;
	numEntries = 1;
	return;
}

void enterScope(){
	if(symbol_table-bp == sizeAssigned)
		increaseStackSize();
	binding* entry = new binding(NULL, Block, numEntries); // The integer numArguments contain the number of entries between current address and last scope.
	*(symbol_table++) = *entry;
	numEntries = 1;
	return;
}

void exitScope(){
	symbol_table -= numEntries;
	numEntries = symbol_table->scope_size;
}



void increaseStackSize(){
	int size = symbol_table - bp;
	if(sizeAssigned == size){
		sizeAssigned = 2*(sizeAssigned+1);
		binding* new_arr = (binding*)malloc(sizeAssigned*sizeof(binding));
		binding* new_bp = new_arr;
		binding* itr = bp;
		while(itr!=symbol_table){
			*(new_arr++) = *(itr++);
		}
		symbol_table = new_arr;
		bp = new_bp;
	}
	return;
}

binding* getVaribleInfo(char* k){
	binding* curr_ptr = symbol_table - 1;
	while(curr_ptr != bp){
		if(curr_ptr->type == Block){
			curr_ptr--;
			continue;
		}
		if(strcmp(k,curr_ptr->identifier)==0)
			return curr_ptr;
		curr_ptr--;
	}
	return NULL;
}

void putVal(char* k, int t, bool isAvailable){
	binding* curr_ptr = symbol_table - 1;
	while(curr_ptr != bp){
		if(curr_ptr->type == Block){
			curr_ptr--;
			continue;
		}
		if(strcmp(k,curr_ptr->identifier)==0){
			curr_ptr->scope_size = t;
			curr_ptr->isValueAvailable = isAvailable;
			return;
		}
		curr_ptr--;
	}
	return;
}

int doesExist(char* k, SYMBOL_TYPE t){
	binding* curr_ptr = symbol_table - 1;
	while(curr_ptr != bp){
		if(curr_ptr->type == Block){
			curr_ptr--;
			continue;
		}
		if(strcmp(k,curr_ptr->identifier)==0)
			return 0;
		curr_ptr--;
	}
	if(t == Variable) cout << k << " has not been declared yet" << '\n';
	return -1;
}

bool isPreviouslyDeclared(char* k){
	binding* curr_ptr = symbol_table - 1;
	while(curr_ptr->type != Block){
		if(strcmp(k,curr_ptr->identifier)==0){
			cout << k << " was already declared"<<'\n';
			return true;
			}
		curr_ptr--;
	}
	return false;
}





void printStack(){
	while(bp != symbol_table) printEntry(*(bp++));
}

void printEntry(binding b){
//cout << "I am here\n";
switch(b.type){
	case Integer_type:
		cout << "[Integer("<<b.identifier<<")|" << "Register("<<b.scope_size <<")]";
		break;
	case Character_type:
		cout << "[Character("<<b.identifier<<")|" << "Register("<<b.scope_size <<")]";
		break;
	case Bool_type:
		cout << "[Bool("<<b.identifier<<")|" << "Register("<<b.scope_size <<")]";
		break;
	case Pointer_type:
		cout << "[Pointer("<<b.identifier<<", " <<b.numPointer << ")|" << "Register("<<b.scope_size <<")]";
		break;
	case Variable:
		cout << "[Variable("<<b.identifier<<")|" << "Register("<<b.scope_size <<")]";
		break;
	case Function:
		cout << "Function("<<b.identifier<<")|";
		break;
	case Block:
		cout << "Block|";
		break; 	
	}
}

