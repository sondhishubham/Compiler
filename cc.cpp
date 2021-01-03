#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
//#include "ast.h"
#include "symbol_table.h"
#include "c.tab.hpp"
#include <fstream>
using namespace std;

extern "C" int yylex();
int yyparse();
extern "C" FILE *yyin;
extern NODE* abstract_syntax_tree;
extern void printTree(NODE*);
extern NODE* createUnaryNode(SYMBOL, NODE*);
extern NODE* createBinaryNode(SYMBOL, NODE*, NODE*);
extern void addChild(NODE*, NODE*);


	//symbol table declarations

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

bool isChanged;

string replaceCharacters(string);
string currBlock;
int branchNum;
int numRegister;
int numWhitespace;
int stringNum;
string constants = "";
void printGlobalDeclaration(NODE*, bool);
void printDeclaration(NODE*);
void printFuncDefinition(NODE*);
SYMBOL_TYPE cgen(NODE*, bool, string, SYMBOL_TYPE, int);
ofstream cc;
void printConstants(){
	cc << "\n\n\n";
	cc << constants;
	cc << "\n\n\n";
}

static void usage()
{
  printf("Usage: cc <prog.c>\n");
}


extern int check_semantics(NODE*);
extern void constantFolding(NODE*);
extern void constantPropagation(NODE*);
extern void copyPropagation(NODE*);
extern void removeDeadCode(NODE*);

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
	if(ret != 0){
  		printf("retv = %d\n", ret);
  		exit(0);
	}
  	initialize_stack();
   	int ans = check_semantics(abstract_syntax_tree);//Don't print tree before check_semantics!!!!
 	exitScope();
// 	free(bp);
 	if(ans != 0){
  		printf("retv = %d\n", ans);
  		exit(0);
	}
	printTree(abstract_syntax_tree);
	cout << "\n\n";
	isChanged = true;
	while(isChanged){
		isChanged = false;
		constantFolding(abstract_syntax_tree);
		constantPropagation(abstract_syntax_tree);
		copyPropagation(abstract_syntax_tree);
		removeDeadCode(abstract_syntax_tree);
	}
	printTree(abstract_syntax_tree);
	cout << "\n\n";
  	cc.open("cc.ll");
  	initialize_stack();
  	branchNum = 0; stringNum = 0; numWhitespace = 0;
  	cgen(abstract_syntax_tree, true, "", Function, 0);
  	printConstants();
  	exitScope();
  	exit(0);
}
															// Numregister is the next value and numregister-1 is the last used register.


SYMBOL_TYPE cgen(NODE* p, bool global, string ret_type, SYMBOL_TYPE t, int numPointer){	// If global = 1, it is an external declaration.
	if(p == NULL) return t;
	if(p->symbol == STRING){
		string numAssigned = to_string(stringNum++);
		string val = "\""+replaceCharacters(string((char*)p->value)) + "\\00\"";
		string size = to_string(val.length() - 4 - 2*numWhitespace);
		constants = constants + "@.str"+numAssigned+" = private unnamed_addr constant ["+size+" x i8] c"+val+", align 1\n";
		constants = constants + "@k"+numAssigned+" = global i8* getelementptr inbounds (["+size+" x i8], ["+size+" x i8]* @.str"+numAssigned+", i32 0, i32 0), align 8\n";
		cc << "\t%"<<numRegister++<<" = load i8*, i8** @k"<<numAssigned<<", align 8\n";
		if(numPointer == 0){
			if(t == Integer_type)
				cout << "Character pointer is used as an Integer\n";
			if(t == Character_type)
				cout << "Character pointer is used as a Character\n";
			if(t == Bool_type)
				cout << "Character pointer is used as a Boolean\n";
			exit(0);
		}
		return Character_type;
	}
	if(p->symbol == INTEGER){
		string align;
		if(numPointer > 0) align = ", align 8\n";
		else if(t == Integer_type) align = ", align 4\n";
		else align = ", align 1\n"; 
		int v = *((int*)p->value);
		int val;
		if(t == Bool_type){
			val = (v == 0)?(0):(1);
		}
		else{
			val = v;
		}
		cc <<"\t%"<<numRegister<<" = alloca "<<ret_type<<align;
		cc << "\tstore " <<ret_type <<" "<< val <<", "<<ret_type<<"* %" << numRegister++<<align;
	  	cc <<"\t%"<<numRegister++ <<" = load "<<ret_type<<", "<<ret_type<<"* %" << (numRegister-2)<<align;
		return t;
	}
	if(p->symbol == ASSIGN||p->symbol == MUL_ASSIGNN || p->symbol == DIV_ASSIGNN || p->symbol == MOD_ASSIGNN || p->symbol == ADD_ASSIGNN || p->symbol == SUB_ASSIGNN || p->symbol == LEFT_ASSIGNN || p->symbol == RIGHT_ASSIGNN || p->symbol == XOR_ASSIGNN || p->symbol == AND_ASSIGNN || p->symbol == OR_ASSIGNN){
		p->bp = p->const_bp;
		if(p->bp->symbol != IDENT){
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
		if(p->symbol == ASSIGN){
			cgen(rvalue, global,type,reg->type ,num_ptr);
		}
		else{
			int reg1, reg2;
			cgen(lvalue, global, "i32", Integer_type, 0);
			reg1 = (numRegister - 1);
			cgen(rvalue, global, "i32", Integer_type, 0);
			reg2 = (numRegister - 1);
			switch(p->symbol){
				case MUL_ASSIGNN:
					cc << "\t%"<<numRegister++<<" = mul nsw i32 %"<<reg1<<", %"<<reg2<<'\n';
					break;
				case DIV_ASSIGNN:
					cc << "\t%"<<numRegister++<<" = sdiv i32 %"<<reg1<<", %"<<reg2<<'\n';
					break;
				case MOD_ASSIGNN:
					cc << "\t%"<<numRegister++<<" = srem i32 %"<<reg1<<", %"<<reg2<<'\n';
					break;
				case ADD_ASSIGNN:
					cc << "\t%"<<numRegister++<<" = add nsw i32 %"<<reg1<<", %"<<reg2<<'\n';
					break;
				case SUB_ASSIGNN:
					cc << "\t%"<<numRegister++<<" = sub nsw i32 %"<<reg1<<", %"<<reg2<<'\n';
					break;
				case LEFT_ASSIGNN:
					cc << "\t%"<<numRegister++<<" = shl i32 %"<<reg1<<", %"<<reg2<<'\n';
					break;
				case RIGHT_ASSIGNN:
					cc << "\t%"<<numRegister++<<" = ashr i32 %"<<reg1<<", %"<<reg2<<'\n';
					break;
				case XOR_ASSIGNN:
					cc << "\t%"<<numRegister++<<" = xor i32 %"<<reg1<<", %"<<reg2<<'\n';
					break;
				case AND_ASSIGNN:
					cc << "\t%"<<numRegister++<<" = and i32 %"<<reg1<<", %"<<reg2<<'\n';
					break;
				case OR_ASSIGNN:
					cc << "\t%"<<numRegister++<<" = or i32 %"<<reg1<<", %"<<reg2<<'\n';
					break;
			}
			if(reg->type == Character_type)
				cc << "\t%"<<numRegister++<<" = trunc i32 %"<<(numRegister-2)<<" to i8\n";
			else if(reg->type == Bool_type){
				cc << "\t%"<<numRegister++<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
				cc << "\t%"<<numRegister++<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
			}
		}
		
		if(reg->scope_size == -1)
			cc << "\tstore "<<type<<" %"<<(numRegister-1)<<", "<<type<<"* @"<< reg->identifier <<align;
		else
			cc << "\tstore "<<type<<" %"<<(numRegister-1)<<", "<<type<<"* %"<< reg->scope_size <<align;
		return reg->type;
	}
	if(p->symbol == PLUS || p->symbol == SUB || p->symbol == MULT || p->symbol == DIVIDE || p->symbol == REMAINDER || p->symbol == LEFT_SHIFT || p->symbol == RIGHT_SHIFT || p->symbol == EXCLUSIVE_OR || p->symbol == INCLUSIVE_OR || p->symbol == AND){
		p->bp = p->const_bp;
		int reg1, reg2;
		SYMBOL_TYPE arg1 = cgen(p->bp++, global, "i32", Integer_type, 0);
		reg1 = numRegister-1;
		SYMBOL_TYPE arg2 = cgen(p->bp++, global, "i32", Integer_type, 0);
		reg2 = numRegister - 1;
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
			case EXCLUSIVE_OR:
				cc << "\t%"<<numRegister++<<" = xor i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case INCLUSIVE_OR:
				cc << "\t%"<<numRegister++<<" = or i32 %"<<reg1<<", %"<<reg2<<'\n';
				break;
			case AND:
				cc << "\t%"<<numRegister++<<" = and i32 %"<<reg1<<", %"<<reg2<<'\n';
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
	if(p->symbol == LESS_THAN || p->symbol == GREATER_THAN || p->symbol == LESS_THAN_EQUAL_TO || p->symbol == GREATER_THAN_EQUAL_TO || p->symbol == EQUAL_TO || p->symbol == NOT_EQUAL_TO){
		p->bp = p->const_bp;
		int reg1, reg2;
		SYMBOL_TYPE arg1 = cgen(p->bp++, global, "i32", Integer_type, 0);
		reg1 = numRegister-1;
		SYMBOL_TYPE arg2 = cgen(p->bp++, global, "i32", Integer_type, 0);
		reg2 = numRegister - 1;
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
	if(p->symbol == LOGICAL_AND || p->symbol == LOGICAL_OR){
		p->bp = p->const_bp;
		int reg1;
		int branchAssigned = branchNum++;
		cgen(p->bp++, global, "i32", Integer_type, 0);
  		cc << "\t%"<<numRegister++<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
  		string entry_block = currBlock;
  		cc << "\tbr i1 %"<<(numRegister-1)<<", label %true_case"<<branchAssigned<<", label %false_case"<<branchAssigned<<'\n';
  		if(p->symbol == LOGICAL_AND){
	  		cc << "\ntrue_case"<<branchAssigned<<":\n";
	  		currBlock = "true_case"+to_string(branchAssigned);
	  		cgen(p->bp++, global, "i32", Integer_type, 0);
  			cc << "\t%"<<numRegister++<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
	  		reg1 = numRegister - 1;
  			cc << "\tbr label %false_case"<<branchAssigned<<'\n';
	  		cc << "\nfalse_case"<<branchAssigned<<":\n";
	  		cc << "\t%"<<(numRegister++)<<" = phi i1 [ false, %"<<entry_block<<" ], [ %"<<(reg1)<<", %"<< currBlock<<" ]\n";
	  		currBlock = "false_case" + to_string(branchAssigned);
  		}
  		else if(p->symbol == LOGICAL_OR){
			cc << "\nfalse_case"<<branchAssigned<<":\n";
			currBlock = "false_case"+to_string(branchAssigned);
			cgen(p->bp++, global, "i32", Integer_type, 0);
  			cc << "\t%"<<numRegister++<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
  			reg1 = numRegister - 1;
  			cc << "\tbr label %true_case"<<branchAssigned<<'\n';
  			cc << "\ntrue_case"<<branchAssigned << ":\n";
	  		cc << "\t%"<<(numRegister++)<<" = phi i1 [ true, %"<<entry_block<<" ], [ %"<<(reg1)<<", %"<< currBlock<<" ]\n";
	  		currBlock = "true_case" + to_string(branchAssigned); 
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
		SYMBOL_TYPE expected = t;
		SYMBOL_TYPE recieved = reg->type;
		if(recieved == Character_type){
			if(expected == Integer_type)
				cc << "\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
			else if(expected == Bool_type){
				cc << "\t%"<<(numRegister++)<<" = icmp ne i8 %"<<(numRegister-2)<<", 0\n";
  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
			}
		}
		else if(recieved == Integer_type){
			if(expected == Character_type)
				cc << "\t%"<<(numRegister++)<<" = trunc i32 %"<<(numRegister-2)<<" to i8\n";
			else if(expected == Bool_type){
				cc << "\t%"<<(numRegister++)<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
  			}
		}
		else if(recieved == Bool_type){
			if(expected == Integer_type){
				cc << "\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
				cc << "\t%"<<(numRegister++)<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i32\n";
			}
			else if(expected == Character_type){
				cc << "\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
				cc << "\t%"<<(numRegister++)<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
			}
		}		
		return t;
	}
	if(p->symbol == IFTHEN){
		p->bp = p->const_bp;
		SYMBOL_TYPE reg =  cgen(p->bp++, global, "i8", Bool_type, 0);
		int branchAssigned = branchNum++;
		cc << "\t%"<<(numRegister++)<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n";
		cc << "\tbr i1 %"<<(numRegister-1)<<", label %true_case"<<branchAssigned<<", label %false_case"<<branchAssigned<<'\n';
		cc << "\ntrue_case"<<branchAssigned<<":\n";
		currBlock = "true_case" + to_string(branchAssigned);
		cgen(p->bp++, global, ret_type, t, numPointer);
		if(p->bp != p->children){
			cc << "\tbr label %move_ahead"<<branchAssigned<<'\n';
			cc << "\nfalse_case"<<branchAssigned<<":\n";
			currBlock = "false_case" + to_string(branchAssigned);
			cgen(p->bp++, global, ret_type, t, numPointer);
			cc << "\tbr label %move_ahead"<<branchAssigned<<'\n';
			cc << "\nmove_ahead"<<branchAssigned<<":\n";
			currBlock = "move_ahead"+to_string(branchAssigned);
			return t;
		}
		else{
			cc << "\tbr label %false_case"<<branchAssigned<<'\n';
			cc << "\nfalse_case"<<branchAssigned<<":\n";
			currBlock = "false_case" + to_string(branchAssigned);
		}
		return t;	
	}
	if(p->symbol == WHILEE){
		p->bp = p->const_bp;
		int branchAssigned = branchNum++;
		cc << "\tbr label %cond"<<branchAssigned<<'\n';
		cc << "\ncond"<<branchAssigned<<":\n";
		currBlock = "cond" + to_string(branchAssigned);
		SYMBOL_TYPE reg =  cgen(p->bp++, global, "i8", Bool_type, 0);
		cc << "\t%"<<(numRegister++)<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n";
		cc << "\tbr i1 %"<<(numRegister-1)<<", label %true_case"<<branchAssigned<<", label %false_case"<<branchAssigned<<'\n';
		cc << "\ntrue_case"<<branchAssigned<<":\n";
		currBlock = "true_case" + to_string(branchAssigned);
		cgen(p->bp++, global, ret_type, t, numPointer);
		cc << "\tbr label %cond"<<branchAssigned<<'\n';
		cc << "\nfalse_case"<<branchAssigned<<":\n";
		currBlock = "false_case" + to_string(branchAssigned);
		return t;
	}
	if(p->symbol == FUNC_CALL){
		p->bp = p->const_bp;
		NODE* ident = p->bp++;
		char* identifier = (char*)ident->value;
		binding* reg = getVaribleInfo(identifier);
		SYMBOL_TYPE* parameters = reg->par_types;
		SYMBOL_TYPE* pp 		= parameters;
		int* par_pointer		= reg->par_pointers;
		bool hasEllipse = false;
		for(int i(0);i<reg->numPar;i++){
			if(*pp == Ellipsis_type)
				hasEllipse = true;
			pp++;
		}
		SYMBOL_TYPE retType = reg->ret_type;
		int pointers = reg->numPointer;
		string pointer = "";
		while(pointers > 0){
			pointer = pointer + "*";
			pointers--;
		}
		string param = "";
		if(!hasEllipse){
			if(p->bp == p->children){
				if(reg->numPar > 0){
					cout << "Function "<<identifier<<" expects more arguments\n";
					exit(0);
				}
				param = "()\n";
			}
			else{
				param = "(";
				NODE* args = p->bp++;
				args->bp = args->const_bp;
				int numPoint(0);
				int totalArguments = args->children - args->bp;
				if(totalArguments != reg->numPar){
					cout << "Function "<<identifier<<" expects different number of arguments\n";
					exit(0);
				}
				for(int i(0);i<totalArguments;i++){
					if( i != 0) param = param + ", ";
					numPoint = *(par_pointer++);
					int n = numPoint;
					SYMBOL_TYPE expected = *(parameters++);	
					string type, ptr, type_ex;
					switch(expected){
						case Integer_type:
							type = "i32"; type_ex = "i32";
							break;
						case Character_type:
							type = "i8";type_ex = "i8";
							break;
						case Bool_type:
							type = "i1";type_ex = "i8";
							break;
					}
					while(numPoint>0){
						ptr = ptr+'*';
						numPoint--;
					}
					cgen(args->bp++, global, type_ex+pointer, expected, n);
					if(expected == Bool_type)
						cc<<"\t%"<<numRegister++<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n";
					param = param + type + ptr + " %"+ to_string(numRegister-1);
				}
				param = param + ")\n";
			}
			if(retType == Void_type){
				cc << "\tcall void @"<<identifier<<param;
			}
			else if(retType == Integer_type){
	  			cc <<"\t%"<<(numRegister++)<<" = call i32"+pointer+" @"<<identifier<<param;
			}
			else if(retType == Character_type){
	  			cc <<"\t%"<<(numRegister++)<<" = call i8"+pointer+" @"<<identifier<<param;
			}
			else if(retType == Bool_type){
	  			cc <<"\t%"<<(numRegister++)<<" = call i1"+pointer+" @"<<identifier<<param;
	  			cc <<"\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
			}
		}
		else{
			string func_args = "";
			if(p->bp == p->children){
				if(reg->numPar > 1){		// Because 1 is for ellipsis
					cout << "Function "<<identifier<<" expects more arguments\n";
					exit(0);
				}
				param = "()\n"; func_args = "(...) ";
			}
			else{
				func_args = " (";
				param = "(";
				NODE* args = p->bp++;
				args->bp = args->const_bp;
				int numPoint(0);
				int totalArguments = args->children - args->bp;
				if(totalArguments < (reg->numPar-1)){
					cout << "Function "<<identifier<<" expects more arguments\n";
					exit(0);
				}
				for(int i(0);i<totalArguments;i++){
					if(i < reg->numPar){
						numPoint = *(par_pointer++);
						int n = numPoint;
						SYMBOL_TYPE expected = *(parameters++);
						if( i != 0) {
							param = param + ", ";
							if(expected != Ellipsis_type) func_args = func_args + ", ";
						}	
						string type, ptr, type_ex;
						switch(expected){
							case Integer_type:
								type = "i32"; type_ex = "i32";
								break;
							case Character_type:
								type = "i8";type_ex = "i8";
								break;
							case Bool_type:
								type = "i1";type_ex = "i8";
								break;
							case Ellipsis_type:
								type = "i32";type_ex = "i32";
								break;
						}
						while(numPoint>0){
							ptr = ptr+'*';
							numPoint--;
						}
						if(expected != Ellipsis_type)
							func_args = func_args + type + ptr;
						else
							expected = Integer_type;
						cgen(args->bp++, global, type_ex+pointer, expected, n);
						if(expected == Bool_type)
							cc<<"\t%"<<numRegister++<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n"; // To convert the boolean to i1 to pass into the function
						param = param + type + ptr + " %"+ to_string(numRegister-1);
					}
					else {
						cgen(args->bp++, global, "i32", Integer_type, 0);
						param = param + ", i32" + " %"+ to_string(numRegister-1);
					}
				}
				param = param + ")\n";
				func_args = func_args + ", ... ) ";
			}
			if(retType == Void_type){
				cc << "\tcall void"+func_args+" @"<<identifier<<param;
			}
			else if(retType == Integer_type){
	  			cc <<"\t%"<<(numRegister++)<<" = call i32"+pointer+func_args+" @"<<identifier<<param;
			}
			else if(retType == Character_type){
	  			cc <<"\t%"<<(numRegister++)<<" = call i8"+pointer+func_args+" @"<<identifier<<param;
			}
			else if(retType == Bool_type){
	  			cc <<"\t%"<<(numRegister++)<<" = call i1"+pointer+func_args+" @"<<identifier<<param;
	  			cc <<"\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
			}
		}
		SYMBOL_TYPE expected = t;
		SYMBOL_TYPE recieved = retType;
		if(recieved == Character_type){
			if(expected == Integer_type)
				cc << "\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
			else if(expected == Bool_type){
				cc << "\t%"<<(numRegister++)<<" = icmp ne i8 %"<<(numRegister-2)<<", 0\n";
  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
			}
		}
		else if(recieved == Integer_type){
			if(expected == Character_type)
				cc << "\t%"<<(numRegister++)<<" = trunc i32 %"<<(numRegister-2)<<" to i8\n";
			else if(expected == Bool_type){
				cc << "\t%"<<(numRegister++)<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
  			}
		}
		else if(recieved == Bool_type){
			if(expected == Integer_type){
				cc << "\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
				cc << "\t%"<<(numRegister++)<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i32\n";
			}
			else if(expected == Character_type){
				cc << "\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
				cc << "\t%"<<(numRegister++)<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i8\n";
			}		
		}		
		return t;
	}
	if(p->symbol == BLOCK){
		p->bp = p->const_bp;
		enterScope();
		while(p->bp != p->children){
			cgen(p->bp, global, ret_type, t, numPointer);
			p->bp++;
		}
		exitScope();
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
		if(t == Bool_type)
			cgen(p->bp, false, "i8", t ,numPointer);
		else
			cgen(p->bp, false, ret_type, t ,numPointer);
		if(t == Bool_type){
			cc << "\t%"<<numRegister++<<" = trunc i8 %"<<(numRegister-2)<<" to i1\n";
		}
		cc << "\tret "<<ret_type<<" %" << (numRegister-1) << '\n';
		numRegister++;
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
	p->bp = p->const_bp;
	while(p->bp!=p->children){
		cgen(p->bp++,global,ret_type,t,numPointer);
	}
	return t;
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
					cc << "\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
					cc << "\t%"<<(numRegister++)<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
	  				cc << "\t%"<<(numRegister++)<<" = zext i1 %"<<(numRegister-2)<<" to i32\n";
				}
				else if(t == Character_type){
					cc << "\t%"<<(numRegister++)<<" = sext i8 %"<<(numRegister-2)<<" to i32\n";
					cc << "\t%"<<(numRegister++)<<" = icmp ne i32 %"<<(numRegister-2)<<", 0\n";
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
				cout << "Function declared in a function\n";
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
	int totalparameters(0);
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
			type = " i1"; ret_type = Bool_type;
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
		cout << "Redeclaration of "<< identifier << endl;
		exit(0);
	}
	binding* entry = new binding(identifier, Function, -1); // Value -1 means that the variable is global!
	entry->numPointer 	= num_ptr;
	entry->ret_type		= ret_type;
	binding* func 		= symbol_table;
	if(symbol_table-bp == sizeAssigned)
		increaseStackSize();
	*(symbol_table++) = *entry;
	numEntries++;
	SYMBOL_TYPE* par_list 		= NULL;
	int*		 pointer_par	= NULL;
	int*		 pointer_base	= NULL;
	SYMBOL_TYPE* base			= NULL;
	enterScope();
	string allocation = "";
	if(declarator->bp == declarator->children){
		parameters = "(){\n";
	}
	else{
		parameters = "(";
		NODE* par = declarator->bp++;
		par->bp = par->const_bp;
		int nChildren = par->children - par->bp;
		par_list = (SYMBOL_TYPE*)malloc(nChildren*sizeof(SYMBOL_TYPE));
		pointer_par = (int*)malloc(nChildren*sizeof(int));
		pointer_base = pointer_par;
		base = par_list;
		int num_reg_for_parameters = (par->children - par->bp);
		while(par->bp != par->children){
			totalparameters++;
			if(par->bp != par->const_bp) parameters = parameters + ", ";
				string par_type = "", par_pointer = "";
				SYMBOL_TYPE t, specific_t; int num_pointer(0);
				NODE* curr_par = par->bp; curr_par->bp = curr_par->const_bp;
				if(curr_par->symbol == DECLARATION){
					NODE* declaration_specifier = curr_par->bp++; declaration_specifier->bp = declaration_specifier->const_bp;
					NODE* declarator = curr_par->bp++; declarator->bp = declarator->const_bp;
					string specific_align="";
					while(declaration_specifier->symbol == CONSTT){
						declaration_specifier = declaration_specifier->const_bp;
					}
					switch(declaration_specifier->symbol){
						case TYPE_INT:
							par_type = "i32"; t = Integer_type; specific_align = ", align 4\n";
							break;
						case TYPE_CHAR:
							par_type = "i8"; t = Character_type; specific_align = ", align 1\n";
							break;
						case TYPE_BOOL:
							par_type = "i1"; t = Bool_type; specific_align = ", align 1\n";
							break;
					}
					*(par_list++) = t;
					specific_t = t;
					while(declarator->symbol == POINTER){
						declarator = declarator->bp;
						par_pointer = par_pointer + "*";
						specific_t = Pointer_type;
						num_pointer++;
						specific_align = ", align 8\n";
					}
					*(pointer_par++) = num_pointer;
					if(declarator->symbol == FUNC_DECLARATOR){
						cout << "Function declared with a function as one of its parameters\n";
						exit(0);
					}
					if(t == Bool_type){
						allocation = allocation + "\t%" + to_string(num_reg_for_parameters++) + " = alloca "+"i8"+par_pointer+specific_align;
						allocation = allocation + "\t%"+to_string(num_reg_for_parameters)+" = zext i1 %"+to_string(numRegister)+" to i8\n";
						allocation = allocation + "\tstore i8"+par_pointer+" %"+to_string(num_reg_for_parameters)+", i8"+par_pointer+"* %"+to_string(num_reg_for_parameters-1) + specific_align;
					}
					else{
						allocation = allocation + "\t%" + to_string(num_reg_for_parameters) + " = alloca "+par_type+par_pointer+specific_align;
						allocation = allocation + "\tstore "+par_type+par_pointer+" %"+to_string(numRegister)+", "+par_type+par_pointer+"* %"+to_string(num_reg_for_parameters) + specific_align;
					}
					int regVal = (t == Bool_type)?(num_reg_for_parameters-1):(num_reg_for_parameters);
					char* par_identifier = (char*)declarator->value;
					binding* entry = new binding(par_identifier, t, regVal);
					entry->numPointer 	= num_pointer;
					if(symbol_table-bp == sizeAssigned)
						increaseStackSize();
					*(symbol_table++) = *entry;
					numEntries++;
					string reg = to_string(numRegister);
					parameters = parameters + par_type + par_pointer + " %" + reg;
				}
				if(curr_par->symbol == ELLIPSISS){
					parameters = parameters + "...";
					*(par_list++) = Ellipsis_type;
					*(pointer_par++) = 0;
				}
			num_reg_for_parameters++;				
			numRegister++;
			par->bp++;
		}
		numRegister = num_reg_for_parameters;
		parameters = parameters + "){\n";
	}
	func->par_types 	= base;
	func->par_pointers 	= pointer_base;
	func->numPar		= totalparameters;
	bool isReturnAbsent = true;
	cc << "\ndefine"+ type + pointer + " @"<<identifier<<parameters;
	cc << "entry:\n";
	currBlock = "entry"; 
	cc << allocation;
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
	int totalparameters(0);
	bool isBool = false;
	bool isFunc = false;
	string isConstant = "global";
	p->bp = p->const_bp;
	NODE* declaration_specifiers  	= p->bp++;
	NODE* declaration_list			= p->bp++;
	if (declaration_specifiers->symbol == CONSTT){
		isConstant = "constant";										//Its const in case of global vairables if variable is const otherwise its global.
		declaration_specifiers = declaration_specifiers->const_bp;
	}
	SYMBOL_TYPE* par_list = NULL, *base = NULL;
	int* pointer_par	= NULL, *pointer_base = NULL;
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
			type = " i8"; align = ", align 1"; specific_align = align; t = Bool_type; isBool = true;
			break;
		case TYPE_VOID:
			type = ""; t = Void_type;
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
				if (val->symbol == INTEGER){
						if(isBool){
							int v = *((int*)val->value);
							initializer = (v==0)?("0"):("1");
						}
						else
							initializer = to_string(*((int*)val->value));
					}
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
				int nPar = par->children - par->bp;
				par_list = (SYMBOL_TYPE*)malloc(nPar*sizeof(SYMBOL_TYPE));
				pointer_par = (int*)malloc(nPar*(sizeof(int)));
				pointer_base= pointer_par;
				base 	 = par_list;
				bool hasSeenEllipsis = false;
				while(par->bp != par->children){
					if(hasSeenEllipsis){
						cout << "Parameters given to a function declaration after ellipsis\n";
						exit(0);
					}
					totalparameters++;
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
								par_type = "i32";*(par_list++) = Integer_type;
								break;
							case TYPE_CHAR:
								par_type = "i8"; *(par_list++) = Character_type;
								break;
							case TYPE_BOOL:
								par_type = "i1"; *(par_list++) = Bool_type;
								break;
						}
						int i(0);
						while(declarator->symbol == POINTER){
							declarator = declarator->bp;
							par_pointer = par_pointer + "*";
							i++;
						}
						*(pointer_par++) = i;
						if(declarator->symbol == FUNC_DECLARATOR){
							cout << "Function declared with a function as one of its parameters\n";
							exit(0);
						}
						parameters = parameters + par_type + par_pointer;
					}
					if(curr_par->symbol == ELLIPSISS){
						hasSeenEllipsis = true;
						parameters = parameters + "...";
						*(par_list++) = Ellipsis_type;
						*(pointer_par++) = 0;
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
				int nPar = par->children - par->bp;
				par_list = (SYMBOL_TYPE*)malloc(nPar*sizeof(SYMBOL_TYPE));
				pointer_par = (int*)malloc(nPar*(sizeof(int)));
				pointer_base= pointer_par;
				base 	 = par_list;
				bool hasSeenEllipsis = false;
				while(par->bp != par->children){
					if(hasSeenEllipsis){
						cout << "Parameter given to a function after ellipsis\n";
						exit(0);
					}
					totalparameters++;
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
								par_type = "i32";*(par_list++) = Integer_type;
								break;
							case TYPE_CHAR:
								par_type = "i8"; *(par_list++) = Character_type;
								break;
							case TYPE_BOOL:
								par_type = "i1"; *(par_list++) = Bool_type;
								break;
						}
						int i(0);
						while(declarator->symbol == POINTER){
							declarator = declarator->bp;
							par_pointer = par_pointer + "*";
							i++;
						}
						*(pointer_par++) = i;
						if(declarator->symbol == FUNC_DECLARATOR){
							cout << "Function declared with a function as one of its parameters\n";
							exit(0);
						}
						parameters = parameters + par_type + par_pointer;
					}
					if(curr_par->symbol == ELLIPSISS){
						hasSeenEllipsis = true;
						parameters = parameters + "...";
						*(par_list++) = Ellipsis_type;
						*(pointer_par++)=0;
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
					cout << "Redeclaration of "<< identifier << endl;
					exit(0);
				}
				binding* entry = new binding(identifier, Function, -1); // Value -1 means that the variable is global!
				entry->numPar		= totalparameters;
				entry->numPointer 	= num_pointer;
				entry->ret_type		= t;
				entry->par_types	= base;
				entry->par_pointers	= pointer_base;
				if(symbol_table-bp == sizeAssigned)
					increaseStackSize();
				*(symbol_table++) = *entry;
				numEntries++;
				if(t == Bool_type)
					cc << "\ndeclare i1" + pointer + " @"<<identifier<<parameters + "\n";
				else
					cc << "\ndeclare"+ type + pointer + " @"<<identifier<<parameters + "\n";
			}
			else{
				binding* entry = new binding(identifier, t, -1);
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



string replaceCharacters(string st){
	numWhitespace = 0;
    string rt = "";
    for(int i(0);i<st.length();i++){
    	if(i == 0 || i == st.length()-1) continue;
        if(st[i] == '\\' && st[i+1] == 't'){
            rt += "\\09"; i++;
            numWhitespace++;
            }
        else if(st[i] == '\\' && st[i+1]=='n'){
            rt += "\\0A"; i++;
            numWhitespace++;
            }
        else 
            rt += st[i];
    }
    return rt;
}


