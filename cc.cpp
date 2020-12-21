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
binding* getVaribleInfo(char*);
void printGlobalDeclaration(NODE*, bool);
void printDeclaration(NODE*);
void printFuncDefinition(NODE*);
SYMBOL_TYPE cgen(NODE*, bool, string, SYMBOL_TYPE, int);
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
  	cgen(abstract_syntax_tree, true, "", Function, 0);
  	printStack();
  	exit(0);
  }
  printf("retv = %d\n", ret);
  exit(0);
}

															// Numregister is the next value and numregister-1 is the last used register.


SYMBOL_TYPE cgen(NODE* p, bool global, string ret_type, SYMBOL_TYPE t, int numPointer){	// If global = 1, it is an external declaration.
	if(p->symbol == INTEGER){
		string align;
		if(numPointer > 0) align = ", align 8\n";
		else if(t == Integer_type) align = ", align 4\n";
		else align = ", align 1\n"; 
		int val = *((int*)p->value);
		cc <<"\t%"<<numRegister<<" = alloca "<<ret_type<<align;
		cc << "\tstore " <<ret_type <<" "<< val <<", "<<ret_type<<"* %" << numRegister++<<align;
	  	cc <<"\t%"<<numRegister++ <<" = load "<<ret_type<<", "<<ret_type<<"* %" << (numRegister-2)<<align;
		return t;
	}
	if(p->symbol == ASSIGN){
		p->bp = p->const_bp;
		if(p->bp->symbol == INTEGER){
			cout << "rvalue given on the left side of =\n";
			exit(0);
		}
		NODE* lvalue = p->bp++;
		NODE* rvalue = p->bp++;
		char * identifier = (char*)lvalue->value;
		binding* reg = getVaribleInfo(identifier);
		int num_pointer = reg->numPointer; int num_ptr = num_pointer;
		string type = "", align = "";
		if(reg->type == Integer_type){
			type = "i32"; align = ", align 4\n";
		}
		else if(reg->type == Character_type){
			type = "i8"; align = ", align 1\n";
		}
		else if(reg->type == Bool_type){
			type = "i8"; align = ", align 1\n";
		}	
		while(num_pointer > 0){
			type = type + "*";
			num_pointer--;
			align = ", align 8\n";
		}
		SYMBOL_TYPE k = cgen(rvalue, global,type,reg->type,num_ptr);
		if(k == Character_type){
			if(reg->type == Integer_type)
				cc << "\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
			else if(reg->type == Bool_type){
				cc << "\t%"<<(numRegister++)<<" = icmp ne i8 %"<<(numRegister-2)<<", 0\n";
  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
			}
		}
		else if(k == Integer_type){
			if(reg->type == Character_type)
				cc << "\t%"<<(numRegister++)<<" = trunc i32 %"<<(numRegister-2)<<" to i8\n";
			else if(reg->type == Bool_type){
				cc << "\t%"<<(numRegister++)<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
  			}
		}
		else if(k == Bool_type){
			if(reg->type == Integer_type){
				cc << "\t%"<<(numRegister++)<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n";
  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i32\n";
			}
			else if(reg->type == Character_type || reg->type == Bool_type){
				cc << "\t%"<<(numRegister++)<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n";
  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
			}
		}
		
		if(reg->scope_size == -1)
			cc << "\tstore "<<type<<" %"<<(numRegister-1)<<", "<<type<<"* @"<< reg->identifier <<align;
		else
			cc << "\tstore "<<type<<" %"<<(numRegister-1)<<", "<<type<<"* %"<< reg->scope_size <<align;
		return reg->type;
	}
	if(p->symbol == PLUS || p->symbol == SUB || p->symbol == MULT || p->symbol == DIVIDE || p->symbol == REMAINDER || p->symbol == LEFT_SHIFT || p->symbol == RIGHT_SHIFT){
		p->bp = p->const_bp;
		int reg1, reg2;
		SYMBOL_TYPE arg1 = cgen(p->bp++, global, ret_type, t, numPointer);
		reg1 = numRegister-1;
		if(arg1 == Character_type){
			cc <<"\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
			reg1 = (numRegister-1);
		}
		else if(arg1 == Bool_type){
			cc <<"\t%"<<(numRegister++)<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n";
			cc <<"\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i32\n";
			reg1 = (numRegister-1);
		}
		SYMBOL_TYPE arg2 = cgen(p->bp++, global, ret_type, t, numPointer);
		reg2 = numRegister - 1;
		if(arg2 == Character_type){
			cc <<"\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
			reg2 = (numRegister-1);
		}
		else if(arg2 == Bool_type){
			cc <<"\t%"<<(numRegister++)<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n";
			cc <<"\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i32\n";
			reg2 = (numRegister-1);
		}
		switch(p->symbol){
			case PLUS:
				cc << "\t%"<<numRegister++<<" = add nsw i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case SUB:
				cc << "\t%"<<numRegister++<<" = sub nsw i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case MULT:
				cc << "\t%"<<numRegister++<<" = mul nsw i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case DIVIDE:
				cc << "\t%"<<numRegister++<<" = sdiv i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case REMAINDER:
				cc << "\t%"<<numRegister++<<" = srem i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case LEFT_SHIFT:
				cc << "\t%"<<numRegister++<<" = shl i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case RIGHT_SHIFT:
				cc << "\t%"<<numRegister++<<" = ashr i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
		}
		if(t == Character_type)
			cc << "\t%"<<numRegister++<<" = trunc i32 %"<<(numRegister-2)<<" to i8\n";
		else if(t == Bool_type){
			cc << "\t%"<<numRegister++<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
			cc << "\t%"<<numRegister++<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
		}		
		return t;
	}
	if(p->symbol == LESS_THAN || p->symbol == GREATER_THAN || p->symbol == LESS_THAN_EQUAL_TO || p->symbol == GREATER_THAN_EQUAL_TO || p->symbol == EQUAL_TO || p->symbol == NOT_EQUAL_TO || p->symbol == EXCLUSIVE_OR || p->symbol == INCLUSIVE_OR || p->symbol == AND){
		p->bp = p->const_bp;
		int reg1, reg2;
		SYMBOL_TYPE arg1 = cgen(p->bp++, global, ret_type, t, numPointer);
		reg1 = numRegister-1;
		if(arg1 == Character_type){
			cc <<"\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
			reg1 = (numRegister-1);
		}
		else if(arg1 == Bool_type){
			cc <<"\t%"<<(numRegister++)<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n";
			cc <<"\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i32\n";
			reg1 = (numRegister-1);
		}
		SYMBOL_TYPE arg2 = cgen(p->bp++, global, ret_type, t, numPointer);
		reg2 = numRegister - 1;
		if(arg2 == Character_type){
			cc <<"\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
			reg2 = (numRegister-1);
		}
		else if(arg2 == Bool_type){
			cc <<"\t%"<<(numRegister++)<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n";
			cc <<"\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i32\n";
			reg2 = (numRegister-1);
		}
		switch(p->symbol){
			case LESS_THAN:
				cc << "\t%"<<numRegister++<<" = icmp slt i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case GREATER_THAN:
				cc << "\t%"<<numRegister++<<" = icmp sgt i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case LESS_THAN_EQUAL_TO:
				cc << "\t%"<<numRegister++<<" = icmp sle i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case GREATER_THAN_EQUAL_TO:
				cc << "\t%"<<numRegister++<<" = icmp sge i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case EQUAL_TO:
				cc << "\t%"<<numRegister++<<" = icmp eq i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case NOT_EQUAL_TO:
				cc << "\t%"<<numRegister++<<" = icmp ne i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case EXCLUSIVE_OR:
				cc << "\t%"<<numRegister++<<" = xor i32 %"<<reg1<<", %"<<reg2<<'\n';
				cc << "\t%"<<numRegister++<<" = icmp ne i32 %"<<numRegister-2<<", 0"<<'\n';
				break;
			case INCLUSIVE_OR:
				cc << "\t%"<<numRegister++<<" = or i32 %"<<reg1<<", %"<<reg2<<'\n';
				cc << "\t%"<<numRegister++<<" = icmp ne i32 %"<<numRegister-2<<", 0"<<'\n';
				break;
			case AND:
				cc << "\t%"<<numRegister++<<" = and i32 %"<<reg1<<", %"<<reg2<<'\n';
				cc << "\t%"<<numRegister++<<" = icmp ne i32 %"<<numRegister-2<<", 0"<<'\n';
				break;
		}
		if(t == Character_type){
			cc << "\t%"<<numRegister++<<" = zext i1 %"<<(numRegister-2)<<" to i32\n";
			cc << "\t%"<<numRegister++<<" = trunc i32 %"<<(numRegister-2)<<" to i8\n";
		}
		else if(t == Bool_type){
			cc << "\t%"<<numRegister++<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
		}
		else if(t == Integer_type){
			cc << "\t%"<<numRegister++<<" = zext i1 %"<<(numRegister-2)<<" to i32\n";
		}
		return t;
	}
	if(p->symbol == IDENT){
		char* identifier = (char*)p->value;
		binding* reg = getVaribleInfo(identifier);
		int num_pointer = reg->numPointer;
		string type = "", align = "";
		if(reg->type == Integer_type){
			type = "i32"; align = ", align 4\n";
		}
		else if(reg->type == Character_type){
			type = "i8"; align = ", align 1\n";
		}
		else if(reg->type == Bool_type){
			type = "i8"; align = ", align 1\n";
		}	
		while(num_pointer > 0){
			type = type + "*";
			num_pointer--;
			align = ", align 8\n";
		}
		if(reg->scope_size == -1)
			cc <<"\t%"<<numRegister++<<" = "<<"load "<<type<<", "<<type<<"* @"<<reg->identifier<<align;
		else
			cc <<"\t%"<<numRegister++<<" = "<<"load "<<type<<", "<<type<<"* %"<<reg->scope_size<<align;
		return reg->type;
	}
	if(p->symbol == IFTHEN){
		return t;
	}
	if(p->symbol == RETURNN){
		if(p->numChildren==0){
			if(numPointer>0){
				cc << "\tret " << ret_type <<" null\n";
			}
			else if(t == Integer_type){
				cc << "\tret "<< ret_type << " 0\n";
			}
			else if(t == Character_type){
				cc << "\tret "<< ret_type << " 0\n";
			}
			else if(t == Bool_type){
				cc << "\tret "<< ret_type << " false\n";
			}
			else if(t == Void_type){
				cc << "\tret void\n";
			}
			return t;
		}
		p->bp = p->const_bp;
		cgen(p->bp, false, ret_type, t,numPointer);
		cc << "\tret "<<ret_type<<" %" << (numRegister-1) << '\n';
		return t;
	}
	if(p->symbol == DECLARATION){
		if (global) printGlobalDeclaration(p,true); else printDeclaration(p);
		return t;
	}
	else if(p->symbol == FUNC_DEF){
		printFuncDefinition(p);
		return t;
	}
	else{
		p->bp = p->const_bp;
		while(p->bp!=p->children){
			cgen(p->bp++,global,ret_type,t,numPointer);
		}
		return t;
	}
}


void printDeclaration(NODE* p){
	p->bp = p->const_bp;
	NODE* declaration_specifier = p->bp++; declaration_specifier->bp 	= declaration_specifier->const_bp;
	NODE* declaration_list 		= p->bp++; declaration_list->bp 		= declaration_list->const_bp;
	if (declaration_specifier->symbol == CONSTT) declaration_specifier = declaration_specifier->const_bp;
	SYMBOL_TYPE t; int num_pointer(0); SYMBOL_TYPE specific_t;
	string type = "", pointer = "", align = "", specific_align = "";
	switch(declaration_specifier->symbol){
		case TYPE_INT:
			type = "i32"; align = ", align 4"; specific_align = align; t = Integer_type;
			break;
		case TYPE_CHAR:
			type = "i8"; align = ", align 1"; specific_align = align; t = Character_type;
			break;
		case TYPE_BOOL:
			type = "i8"; align = ", align 1"; specific_align = align; t = Bool_type;
			break;
	}
	specific_t = t;
	while(declaration_list->bp != declaration_list-> children){
		NODE* ident;
		NODE* k = declaration_list->bp; k->bp = k->const_bp;
		if(k->symbol == INITIALIZE){
			ident = k->bp++; ident->bp = ident->const_bp;
			while(ident->symbol == POINTER){
				pointer = pointer+"*";
				specific_t = Pointer_type;
				num_pointer++;
				specific_align = ", align 8";
				ident = ident->const_bp;
			}
			ident->bp = ident->const_bp;
			if(ident->symbol == FUNC_DECLARATOR){
				cout << "Function declared in a function\n";
				exit(0);
			}
			int curr_register = numRegister;
			binding* entry = new binding((char*)ident->value, t, curr_register);
			entry->numPointer = num_pointer;
			if(symbol_table-bp == sizeAssigned)
				increaseStackSize();
			*(symbol_table++) = *entry;
			numEntries++;
			NODE* val = k->bp++;
			cc << "\t%"<<numRegister++<<"= alloca "<<type+pointer+specific_align<<"\n"; // numRegister is increased here for the next instruction.
			SYMBOL_TYPE k = cgen(val, false, type+pointer,t,num_pointer);
			if(k == Character_type){
				if(t == Integer_type)
					cc << "\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
				else if(t == Bool_type){
					cc << "\t%"<<(numRegister++)<<" = icmp ne i8 %"<<(numRegister-2)<<", 0\n";
	  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
				}
			}
			else if(k == Integer_type){
				if(t == Character_type)
					cc << "\t%"<<(numRegister++)<<" = trunc i32 %"<<(numRegister-2)<<" to i8\n";
				else if(t == Bool_type){
					cc << "\t%"<<(numRegister++)<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
	  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
	  			}
			}
			else if(k == Bool_type){
				if(t == Integer_type){
					cc << "\t%"<<(numRegister++)<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n";
	  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i32\n";
				}
				else if(t == Character_type || t == Bool_type){
					cc << "\t%"<<(numRegister++)<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n";
	  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
				}
			}
			cc << "\tstore " <<type+pointer<<" %"<<numRegister-1<<", "+type <<'*'+pointer<<" %" <<curr_register<<align << '\n';
		}
		else{
			ident = k; ident->bp = ident->const_bp;
			while(ident->symbol == POINTER){
				specific_t = Pointer_type;
				num_pointer++;
				pointer = pointer+"*";
				specific_align = ", align 8";
				ident = ident->const_bp;
			}
			if(ident->symbol == FUNC_DECLARATOR){
				cout << "Fucntion declared in a function\n";
				exit(0);
			}
			binding* entry = new binding((char*)ident->value, t, numRegister);
			entry->numPointer = num_pointer; 
			if(symbol_table-bp == sizeAssigned)
				increaseStackSize();
			*(symbol_table++) = *entry;
			numEntries++;
			cc << "\t%"<<numRegister++<<"= alloca "<<type+pointer+specific_align<<"\n";
		}
	declaration_list->bp++;
	pointer = "";
	num_pointer = 0;
	specific_align = align;
	specific_t = t;
	}
	return;
}


void printFuncDefinition(NODE* p){
	numRegister = 0;
	p->bp = p->const_bp;
	NODE* declaration_specifier		= p->bp++; declaration_specifier->bp = declaration_specifier->const_bp;
	NODE* declarator 				= p->bp++; declarator->bp = declarator->const_bp;
	NODE* function_body 			= p->bp++; function_body->bp = function_body->const_bp;
	if (declaration_specifier->symbol == CONSTT) declaration_specifier = declaration_specifier->const_bp;
	string type = "", pointer = "", parameters = ""; SYMBOL_TYPE ret_type; int num_ptr(0);
	switch(declaration_specifier->symbol){
		case TYPE_INT:
			type = " i32"; ret_type = Integer_type;
			break;
		case TYPE_CHAR:
			type = " i8"; ret_type = Character_type;
			break;
		case TYPE_BOOL:
			type = " i8"; ret_type = Bool_type;
			break;
		case TYPE_VOID:
			type = ""; ret_type = Void_type;
	}
	while(declarator->symbol == POINTER){
		declarator = declarator->const_bp;
		pointer = pointer + "*";
		num_ptr++;
	}
	declarator->bp = declarator->const_bp;
	NODE* ident = declarator->bp++; ident->bp = ident->const_bp;
	char* identifier = (char*) ident->value;
	if(doesExist(identifier,Function)==0){
		cout << "Redaclaration of "<< identifier << endl;
		exit(0);
	}
	binding* entry = new binding(identifier, Function, -1); // Value -1 means that the variable is global!
	entry->numPointer = num_ptr;
	if(symbol_table-bp == sizeAssigned)
		increaseStackSize();
	*(symbol_table++) = *entry;
	numEntries++;
	enterScope();
	if(declarator->bp == declarator->children){
		parameters = "(){\n";
	}
	else{
		parameters = "(";
		NODE* par = declarator->bp++;
		par->bp = par->const_bp;
		while(par->bp != par->children){
			if(par->bp != par->const_bp) parameters = parameters + ", ";
				string par_type = "", par_pointer = "";
				SYMBOL_TYPE t, specific_t; int num_pointer(0);
				NODE* curr_par = par->bp; curr_par->bp = curr_par->const_bp;
				if(curr_par->symbol == DECLARATION){
					NODE* declaration_specifier = curr_par->bp++; declaration_specifier->bp = declaration_specifier->const_bp;
					NODE* declarator = curr_par->bp++; declarator->bp = declarator->const_bp;
					while(declaration_specifier->symbol == CONSTT){
						declaration_specifier = declaration_specifier->const_bp;
					}
					switch(declaration_specifier->symbol){
						case TYPE_INT:
							par_type = "i32"; t = Integer_type;
							break;
						case TYPE_CHAR:
							par_type = "i8"; t = Character_type;
							break;
						case TYPE_BOOL:
							par_type = "i8"; t = Bool_type;
							break;
					}
					specific_t = t;
					while(declarator->symbol == POINTER){
						declarator = declarator->bp;
						par_pointer = par_pointer + "*";
						specific_t = Pointer_type;
						num_pointer++;
					}
					if(declarator->symbol == FUNC_DECLARATOR){
						cout << "Fucntion declared with a function as one of its parameters\n";
						exit(0);
					}
					char* par_identifier = (char*)declarator->value;
					binding* entry = new binding(par_identifier, t, numRegister);
					entry->numPointer = num_pointer;
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
	bool isReturnAbsent = true;
	cc << "\ndefine"+ type + pointer + " @"<<identifier<<parameters;
	while (function_body->bp != function_body->children){
		if(function_body->bp->symbol == RETURNN) isReturnAbsent = false;
		cgen(function_body->bp, false,type+pointer,ret_type,num_ptr);
		function_body->bp++;
	}
	if(isReturnAbsent){
		NODE* last_node = new NODE(RETURNN, NULL, 0);
		cgen(last_node, false, type+pointer, ret_type,num_ptr);
	}
	cc << "}\n\n";
	exitScope();
	return;
}	




void printGlobalDeclaration(NODE* p, bool global){
	bool isInitiazlizer = false;
	bool isFunc = false;
	string isConstant = "global";
	p->bp = p->const_bp;
	NODE* declaration_specifiers  	= p->bp++;
	NODE* declaration_list			= p->bp++;
	if (declaration_specifiers->symbol == CONSTT){
		isConstant = "constant";										//Its const in case of global vairables if variable is const otherwise its global.
		declaration_specifiers = declaration_specifiers->const_bp;
	}
	char* identifier;
	SYMBOL_TYPE t, specific_t; int num_pointer(0);
	string type = "", align = "", specific_align = "", pointer = "", initializer = "", parameters = "";
	switch(declaration_specifiers->symbol){
		case TYPE_INT:
			type = " i32"; align = ", align 4"; specific_align = align; t = Integer_type;
			break;
		case TYPE_CHAR:
			type = " i8"; align = ", align 1"; specific_align = align; t = Character_type;
			break;
		case TYPE_BOOL:
			type = " i8"; align = ", align 1"; specific_align = align; t = Bool_type;
			break;
	}
	specific_t = t;
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
			specific_t = Pointer_type;
			num_pointer++;
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
				binding* entry = new binding(identifier, specific_t, -1);
				entry->numPointer = num_pointer;
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
		num_pointer = 0;
		specific_t = t;
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

