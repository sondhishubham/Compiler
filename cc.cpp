#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include "ast.h"
#include "symbol_table.h"
#include "c.tab.hpp"
#include <fstream>
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


int numRegister;
void printGlobalDeclaration(NODE*, bool);
void printDeclaration(NODE*);
void printFuncDefinition(NODE*);
void cgen(NODE*, bool);
ofstream cc;


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
//  printTree(abstract_syntax_tree); cout << '\n';
  if(ret == 0){
  	initialize_stack();
   	int ans = check_semantics(abstract_syntax_tree);
 	exitScope();
 	free(bp);
  	printf("retv = %d\n", ans);
  	cc.open("cc.ll");
  	initialize_stack();
  	cgen(abstract_syntax_tree, true);
  	printStack();
  	exit(0);
  }
  printf("retv = %d\n", ret);
  exit(0);
}

															// Numregister is the next value and numregister-1 is the last used register.


void cgen(NODE* p, bool global){	// If global = 1, it is an external declaration.
	if(p->symbol == INTEGER){
		int val = *((int*)p->value);
		cc <<"%"<<numRegister<<" = alloca i32, align 4\n";
		cc << "store i32 "<< val <<", i32* %" << numRegister++<<", align 4\n";
	  	cc <<"%"<<numRegister++ <<" = load i32, i32* %" << (numRegister-2)<<", align 4\n";
		return;
	}
	if(p->symbol == RETURNN){
		p->bp = p->const_bp;
		if(p->bp == p->children){
			cc << "ret i32 0";
			return;
		}
		cgen(p->bp, false);
		cc << "ret i32 %" << (numRegister-1) << '\n';
		return;
	}
	if(p->symbol == DECLARATION){
		if (global) printGlobalDeclaration(p,true); else printDeclaration(p);
		return;
	}
	else if(p->symbol == FUNC_DEF){
		printFuncDefinition(p);
		return;
	}
	else{
		p->bp = p->const_bp;
		while(p->bp!=p->children){
			cgen(p->bp++,global);
		}
		return;
	}
}


void printDeclaration(NODE* p){
	p->bp = p->const_bp;
	NODE* declaration_specifier = p->bp++; declaration_specifier->bp 	= declaration_specifier->const_bp;
	NODE* declaration_list 		= p->bp++; declaration_list->bp 		= declaration_list->const_bp;
	if (declaration_specifier->symbol == CONSTT) declaration_specifier = declaration_specifier->const_bp;
	string type = "", pointer = "", align = "", specific_align = "";
	switch(declaration_specifier->symbol){
		case TYPE_INT:
			type = "i32"; align = ", align 4"; specific_align = align;
			break;
		case TYPE_CHAR:
			type = "i8"; align = ", align 1"; specific_align = align;
			break;
		case TYPE_BOOL:
			type = "i8"; align = ", align 1"; specific_align = align;
			break;
	}
	while(declaration_list->bp != declaration_list-> children){
		NODE* ident;
		NODE* k = declaration_list->bp; k->bp = k->const_bp;
		if(k->symbol == INITIALIZE){
			ident = k->bp++; ident->bp = ident->const_bp;
			while(ident->symbol == POINTER){
				pointer = pointer+"*";
				specific_align = ", align 8";
				ident = ident->const_bp;
			}
			ident->bp = ident->const_bp;
			if(ident->symbol == FUNC_DECLARATOR){
				cout << "Function declared in a function\n";
				exit(0);
			}
			int curr_register = numRegister;
			binding* entry = new binding((char*)ident->value, Variable, curr_register);
			if(symbol_table-bp == sizeAssigned)
				increaseStackSize();
			*(symbol_table++) = *entry;
			numEntries++;
			NODE* val = k->bp++;
			cc << "%"<<numRegister++<<"= alloca "<<type+pointer+specific_align<<"\n"; // numRegister is increased here for the next instruction.
			cgen(val, false);
			cc << "store i32"+pointer+" %"<<numRegister-1<<", "+type <<'*'+pointer<<" %" <<curr_register<<align << '\n';
		}
		else{
			ident = k; ident->bp = ident->const_bp;
			while(ident->symbol == POINTER){
				pointer = pointer+"*";
				specific_align = ", align 8";
				ident = ident->const_bp;
			}
			if(ident->symbol == FUNC_DECLARATOR){
				cout << "Fucntion declared in a function\n";
				exit(0);
			}
			binding* entry = new binding((char*)ident->value, Variable, numRegister); 
			if(symbol_table-bp == sizeAssigned)
				increaseStackSize();
			*(symbol_table++) = *entry;
			numEntries++;
			cc << "%"<<numRegister++<<"= alloca "<<type+pointer+specific_align<<"\n";
		}
	declaration_list->bp++;
	pointer = "";
	specific_align = align;
	}
}


void printFuncDefinition(NODE* p){
	numRegister = 0;
	p->bp = p->const_bp;
	NODE* declaration_specifier		= p->bp++; declaration_specifier->bp = declaration_specifier->const_bp;
	NODE* declarator 				= p->bp++; declarator->bp = declarator->const_bp;
	NODE* function_body 			= p->bp++; function_body->bp = function_body->const_bp;
	if (declaration_specifier->symbol == CONSTT) declaration_specifier = declaration_specifier->const_bp;
	string type = "", pointer = "", parameters = "";
	switch(declaration_specifier->symbol){
		case TYPE_INT:
			type = " i32";
			break;
		case TYPE_CHAR:
			type = " i8";
			break;
		case TYPE_BOOL:
			type = " i8";
			break;
	}
	while(declarator->symbol == POINTER){
		declarator = declarator->const_bp;
		pointer = pointer + "*";
	}
	declarator->bp = declarator->const_bp;
	NODE* ident = declarator->bp++; ident->bp = ident->const_bp;
	char* identifier = (char*) ident->value;
	if(doesExist(identifier,Function)==0){
		cout << "Redaclaration of "<< identifier << endl;
		exit(0);
	}
	binding* entry = new binding(identifier, Function, -1); // Value -1 means that the variable is global!
	if(symbol_table-bp == sizeAssigned)
		increaseStackSize();
	*(symbol_table++) = *entry;
	numEntries++;
	enterScope();
	if(declarator->bp == declarator->children){
		parameters = "()";
	}
	else{
		parameters = "(";
		NODE* par = declarator->bp++;
		par->bp = par->const_bp;
		while(par->bp != par->children){
			if(par->bp != par->const_bp) parameters = parameters + ", ";
				string par_type = "", par_pointer = "";
				NODE* curr_par = par->bp; curr_par->bp = curr_par->const_bp;
				if(curr_par->symbol == DECLARATION){
					NODE* declaration_specifier = curr_par->bp++; declaration_specifier->bp = declaration_specifier->const_bp;
					NODE* declarator = curr_par->bp++; declarator->bp = declarator->const_bp;
					while(declaration_specifier->symbol == CONSTT){
						declaration_specifier = declaration_specifier->const_bp;
					}
					switch(declaration_specifier->symbol){
						case TYPE_INT:
							par_type = "i32";
							break;
						case TYPE_CHAR:
							par_type = "i8";
							break;
						case TYPE_BOOL:
							par_type = "i8";
							break;
					}
					while(declarator->symbol == POINTER){
						declarator = declarator->bp;
						par_pointer = par_pointer + "*";
					}
					if(declarator->symbol == FUNC_DECLARATOR){
						cout << "Fucntion declared with a function as one of its parameters\n";
						exit(0);
					}
					char* par_identifier = (char*)declarator->value;
					binding* entry = new binding(par_identifier, Variable, numRegister); // Value -1 means that the variable is global!
					if(symbol_table-bp == sizeAssigned)
						increaseStackSize();
					*(symbol_table++) = *entry;
					numEntries++;
					string reg = to_string(numRegister);
					parameters = parameters + par_type + par_pointer + " %" + reg;
				}
			numRegister++;
			par->bp++;
		}
		parameters = parameters + "){\n";
	}
	numRegister++;
	cc << "\ndefine"+ type + pointer + " @"<<identifier<<parameters;
	while (function_body->bp != function_body->children){
		cgen(function_body->bp, false);
		function_body->bp++;
	}
	cc << "}\n\n";
}	




void printGlobalDeclaration(NODE* p, bool global){
	bool isInitiazlizer = false;
	bool isFunc = false;
	string isConstant = "global";
	p->bp = p->const_bp;
	NODE* declaration_specifiers  	= p->bp++;
	NODE* declaration_list			= p->bp++;
	if (declaration_specifiers->symbol == CONSTT){
		isConstant = "constant";										//Its const in case of gloval vairables if variable is const otherwise its global.
		declaration_specifiers = declaration_specifiers->const_bp;
	}
	char* identifier;
	string type = "", align = "", specific_align = "", pointer = "", initializer = "", parameters = "";
	switch(declaration_specifiers->symbol){
		case TYPE_INT:
			type = " i32"; align = ", align 4"; specific_align = align;
			break;
		case TYPE_CHAR:
			type = " i8"; align = ", align 1"; specific_align = align;
			break;
		case TYPE_BOOL:
			type = " i8"; align = ", align 1"; specific_align = align;
			break;
	}
	declaration_list->bp = declaration_list->const_bp;
	while(declaration_list->bp != declaration_list->children){
		NODE* ident;
		NODE* k = declaration_list->bp;
		k->bp = k->const_bp;
		if(k->symbol == INITIALIZE){
			isInitiazlizer = true;
			ident = k->bp++; ident->bp = ident->const_bp;
			NODE* val = k->bp++; val->bp = val->const_bp; 
			if(global){
				if (val->symbol == INTEGER)
					initializer = to_string(*((int*)val->value));
				else{
					cout << "Only constants can be given as initializer in external declarations"<<endl;
					exit(0);
				}
			}
		}
		else if(k->symbol == FUNC_DECLARATOR){
			isFunc = true;
			ident = k->bp++; ident->bp = ident->const_bp;
			if(k->bp == k->children){
				parameters = "()";
			}
			else{
				parameters = "(";
				NODE* par = k->bp++;
				par->bp = par->const_bp;
				while(par->bp != par->children){
					if(par->bp != par->const_bp) parameters = parameters + ", ";
					string par_type = "", par_pointer = "";
					NODE* curr_par = par->bp; curr_par->bp = curr_par->const_bp;
					if(curr_par->symbol == DECLARATION){
						NODE* declaration_specifier = curr_par->bp++; declaration_specifier->bp = declaration_specifier->const_bp;
						NODE* declarator = curr_par->bp++; declarator->bp = declarator->const_bp;
						while(declaration_specifier->symbol == CONSTT){
							declaration_specifier = declaration_specifier->const_bp;
						}
						switch(declaration_specifiers->symbol){
							case TYPE_INT:
								par_type = "i32";
								break;
							case TYPE_CHAR:
								par_type = "i8";
								break;
							case TYPE_BOOL:
								par_type = "i8";
								break;
						}
						while(declarator->symbol == POINTER){
							declarator = declarator->bp;
							par_pointer = par_pointer + "*";
						}
						if(declarator->symbol == FUNC_DECLARATOR){
							cout << "Fucntion declared with a function as one of its parameters\n";
							exit(0);
						}
						parameters = parameters + par_type + par_pointer;
					}
					par->bp++;
				}
				parameters = parameters + ")";
			}
		}
		else{
			ident = declaration_list->bp;
			initializer = "0";
		}
		while(ident->symbol==POINTER){		//In case of pointer
			specific_align = ", align 8";
			pointer = pointer+'*';
			ident=ident->const_bp;
			if(!isInitiazlizer) initializer = "null";
		}
		if(ident->symbol == FUNC_DECLARATOR){
			NODE* parent = ident; parent->bp = parent->const_bp;
			isFunc = true;
			ident = parent->bp++; ident->bp = ident->const_bp;
			if(parent->bp == parent->children){
				parameters = "()";
			}
			else{
				parameters = "(";
				NODE* par = parent->bp++;
				par->bp = par->const_bp;
				while(par->bp != par->children){
					if(par->bp != par->const_bp) parameters = parameters + ", ";
					string par_type = "", par_pointer = "";
					NODE* curr_par = par->bp; curr_par->bp = curr_par->const_bp;
					if(curr_par->symbol == DECLARATION){
						NODE* declaration_specifier = curr_par->bp++; declaration_specifier->bp = declaration_specifier->const_bp;
						NODE* declarator = curr_par->bp++; declarator->bp = declarator->const_bp;
						while(declaration_specifier->symbol == CONSTT){
							declaration_specifier = declaration_specifier->const_bp;
						}
						switch(declaration_specifiers->symbol){
							case TYPE_INT:
								par_type = "i32";
								break;
							case TYPE_CHAR:
								par_type = "i8";
								break;
							case TYPE_BOOL:
								par_type = "i8";
								break;
						}
						while(declarator->symbol == POINTER){
							declarator = declarator->bp;
							par_pointer = par_pointer + "*";
						}
						if(declarator->symbol == FUNC_DECLARATOR){
							cout << "Fucntion declared with a function as one of its parameters\n";
							exit(0);
						}
						parameters = parameters + par_type + par_pointer;
					}
					par->bp++;
				}
				parameters = parameters + ")";
			}
		}
		identifier = (char*)ident->value;
		if(global){
			if(isFunc){
				if(doesExist(identifier,Function)==0){
					cout << "Redaclaration of "<< identifier << endl;
					exit(0);
				}
				binding* entry = new binding(identifier, Function, -1); // Value -1 means that the variable is global!
				if(symbol_table-bp == sizeAssigned)
					increaseStackSize();
				*(symbol_table++) = *entry;
				numEntries++;
				cc << "\ndeclare"+ type + pointer + " @"<<identifier<<parameters + "\n";
			}
			else{
				binding* entry = new binding(identifier, Variable, -1);
				if(symbol_table-bp == sizeAssigned)
					increaseStackSize();
				*(symbol_table++) = *entry;
				numEntries++;
				cc << "@"<<identifier<<" = "+isConstant+type+pointer+" "+initializer+specific_align+"\n";
			}
		}
		declaration_list->bp++;
		pointer = "";
		initializer = "";
		parameters = "";
		specific_align = align;
		initializer = "";
		isInitiazlizer = false;
		isFunc = false;
	}
	return;
}




































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
			while(ident->symbol == POINTER) ident = ident->bp;
			NODE* initializer = k->bp++;
			if (ident->symbol == FUNC_DECLARATOR){ 
				cout << "Function is declared as a variable"<<'\n';
				return -1;
			}
			if (check_semantics(initializer) == -1)	return -1;
		}
		else if((declaration_list.bp)->symbol == FUNC_DECLARATOR){	//In the case of declaration of fucntion like "void printf();"
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

