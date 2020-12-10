#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
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
int addFunction(NODE*);
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
  if(ret == 0){
  	initialize_stack();
   	int ans = check_semantics(abstract_syntax_tree);
 	exitScope();
 	free(bp);
  	printStack();
  	printf("retv = %d\n", ans);
  }
  exit(0);
}


int check_semantics(NODE* ptr){
	int answer;
	if(ptr->symbol == IDENT){
//		cout << (char*) ptr->value<<endl;
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
	binding* entry = new binding(identifier_name, Function, 0);
	if(symbol_table-bp == sizeAssigned)
		increaseStackSize();
	*(symbol_table++) = *entry;
	numEntries++;
	
	enterScope();
//	if(parameters->symbol == PARAMETERS) cout << "PARAMETER
	if (parameters!=NULL){
		while(parameters->bp != parameters->children){
			if(parameters->bp->symbol != ELLIPSISS){
				NODE* declaration = parameters->bp;
				declaration->bp++;
				NODE* variable = declaration->bp++;
				while(variable->symbol == POINTER) variable = variable->bp;			//To take care of pointer
				if(variable->symbol == FUNC_DECLARATOR){
					cout << "Function given as an argument to a fucntion" << endl;
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
				binding* en = new binding("Ellipsis", Variable, 0);
				if(symbol_table - bp == sizeAssigned)
					increaseStackSize();
				*(symbol_table++) = *en;
				numEntries++;
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
			NODE* initializer = k->bp++;
			if (ident->symbol == FUNC_DECLARATOR){ 
				cout << "Function is declared as a variable"<<'\n';
				return -1;
			}
			if (check_semantics(initializer) == -1)	return -1;
		}
		else if((declaration_list.bp)->symbol == FUNC_DECLARATOR){	//In the case of declaration of fucntion like "void printf();"
			t = Function;
			NODE* k = declaration_list.bp;
			ident = k->bp++;
			if(ident->symbol == FUNC_DECLARATOR){
				cout << "Declaration of a function returning a function" <<endl;
				return -1;
			}
		}
		else     													//In case when there is simple declaration like int b,v,d; No initializing and no function declaraions
			ident = declaration_list.bp;
		while (ident->symbol == POINTER) ident = ident->bp;			// To take care of pointers while declaraion
		char* identifier_name = (char*)ident->value;
		if(t==Variable && isPreviouslyDeclared(identifier_name)){
			return -1;
		}
		if(t==Function){
			int exists = doesExist(identifier_name, Function);
			if(exists == 0){
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
		free(bp);
		symbol_table = new_arr;
		bp = new_bp;
	}
	return;
}

int doesExist(char* k, SYMBOL_TYPE t){
	binding* curr_ptr = symbol_table - 1;
//	cout << endl;
//	cout << "The value of bp is "<< bp<<endl;
	while(curr_ptr != bp){
		if(curr_ptr->type == Block){
			curr_ptr--;
			continue;
		}
//		cout << "The value of curr_ptr is "<<curr_ptr<<endl;
		if(strcmp(k,curr_ptr->identifier)==0)
			return 0;
		curr_ptr--;
	}
	if(t == Variable) cout << k << " has not been declared yet" << '\n';
	return -1;
}

bool isPreviouslyDeclared(char* k){
//	cout << " I " <<endl;
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

switch(b.type){
	case Variable:
		cout << "Variable("<<b.identifier<<")|";
		break;
	case Function:
		cout << "Function("<<b.identifier<<")|";
		break;
	case Block:
		cout << "Block|";
		break; 	
	}
}

