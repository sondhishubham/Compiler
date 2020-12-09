#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include "ast.h"
#include "symbol_table.h"
#include "c.tab.hpp"
using namespace std;

extern "C" int yylex();
int yyparse();
extern "C" FILE *yyin;
extern NODE* abstract_syntax_tree;
extern void printTree(NODE*);
int check_semantics(NODE*);
int addDeclaration(NODE*);
binding* get_binding(char*);
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

static void usage()
{
  printf("Usage: cc <prog.c>\n");
}

int
main(int argc, char **argv)
{
  if (argc != 2) {
    usage();
    exit(1);
  }
  char const *filename = argv[1];
  yyin = fopen(filename, "r");
  assert(yyin);
  int ret = yyparse();
  initialize_stack();
  check_semantics(abstract_syntax_tree);
  printStack();
  printf("retv = %d\n", ret);
  exit(0);
}


int check_semantics(NODE* ptr){
	if(ptr->symbol == DECLARATION){
		addDeclaration(ptr);
	}
	else{
		while(ptr->bp != ptr->children){
			check_semantics(ptr->bp++);
		}
	}
	return 0;
}

int addDeclaration(NODE* ptr){
	NODE declaration_specifiers = *(ptr->bp++);
	NODE declaration_list = *(ptr->bp++);
	SYMBOL_TYPE t;
	int numDeclarations = declaration_list.children - declaration_list.bp;
	if(declaration_specifiers.symbol == CONSTT) declaration_specifiers = *(declaration_specifiers.bp);
	switch(declaration_specifiers.symbol){
		case TYPE_VOID:
			t = Void;
			break;
    	case TYPE_INT:
    		t = Integer;
    		break;
    	case TYPE_SHORT:
    		t = Short;
    		break;
    	case TYPE_LONG:
    		t = Long;
    		break;
    	case TYPE_DOUBLE:
    		t = Double;
    		break;
    	case TYPE_BOOL:
    		t = Bool;
    		break;
    	case TYPE_CHAR:
    		t = Charater;
    		break;
    	case TYPE_FLOAT:
    		t = Float;
    		break;    		
	}
	while(declaration_list.bp != declaration_list.children){
		if((declaration_list.bp)->symbol == INITIALIZE){
			cout << "Nothing"<<endl;
		}
		else{//In case when there is simple declaration like int b,v,d; No initializing and no function declaraions
			NODE* ident = declaration_list.bp;
			char* identifier_name = (char*)ident->value;
			if(isPreviouslyDeclared(identifier_name)){
				return -1;
			}
			binding* entry = new binding(identifier_name, t, NULL, 0, t);
			if(symbol_table-bp == sizeAssigned)
				increaseStackSize();
			*(symbol_table++) = *entry;
		}
		declaration_list.bp++;
	}
	return 0;
}

void initialize_stack(){
	symbol_table = (binding*) malloc(sizeAssigned*sizeof(binding));
	bp = symbol_table;
	binding* entry = new binding(NULL, Block, NULL, 0, Block);
	*(symbol_table++) = *entry;
	numEntries = 1;
	return;
}

void enterScope(){
	if(symbol_table-bp == sizeAssigned)
		increaseStackSize();
	binding* entry = new binding(NULL, Block, NULL, numEntries, Block); // The integer numArguments contain the number of entries between current address and last scope.
	*(symbol_table++) = *entry;
	numEntries = 1;
	return;
}

void exitScope(){
	symbol_table -= numEntries;
	numEntries = symbol_table->nArguments;
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
		free(bp);
		symbol_table = new_arr;
		bp = new_bp;
	}
	return;
}

binding* get_binding(char* k){
	binding* curr_ptr = symbol_table - 1;
	while(curr_ptr != bp){
		if(k == curr_ptr->identifier)
			return curr_ptr;
		curr_ptr--;
	}
	cout << k << " has not been declared yet";
	return NULL;
}

bool isPreviouslyDeclared(char* k){
//	cout << " I " <<endl;
	binding* curr_ptr = symbol_table - 1;
	while(curr_ptr->type != Block){
		if(k == curr_ptr->identifier){
			cout << k<<endl;
			cout << curr_ptr->identifier <<endl;
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

switch(b.type){
	case Void:
		cout << "Void("<<*b.identifier<<")|";
		break;
	case Short:
		cout << "Short("<<*b.identifier<<")|";
		break;
	case Long:
		cout << "Long("<<*b.identifier<<")|";
		break;
	case Float:
		cout << "Float("<<*b.identifier<<")|";
		break;
	case Integer:
		cout << "Integer("<<*b.identifier<<")|";	
		break;
	case Bool:
		cout << "Bool("<<*b.identifier<<")|";
		break;
	case Double:
		cout << "Double("<<*b.identifier<<")|";
		break;
	case Charater:
		cout << "Charater("<<*b.identifier<<")|";
		break;
	case Function:
		cout << "Function("<<*b.identifier<<")|";
		break;
	case Pointer:
		cout << "Pointer("<<*b.identifier<<")|";
		break;
	case Block:
		cout << "Block|";
		break; 	
	}
}

