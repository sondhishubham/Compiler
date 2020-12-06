%{
#include <cstdio>
#include <iostream>
#include "ast.h"

using namespace std;

NODE* createFunction(NODE*,NODE*,NODE*);
NODE* createUnaryNode(SYMBOL, NODE*);
NODE* createBinaryNode(SYMBOL, NODE*, NODE*);
void addChild(NODE*, NODE*);
void printTree(NODE*);
// stuff from flex that bison needs to know about:
extern "C" int yylex();
int yyparse();
extern "C" FILE *yyin;

void yyerror(const char *s);

%}
%union{
    int number;
    NODE* node_ptr;
    char* word;
}
%token  <number> I_CONSTANT
%token  <word> STRING_LITERAL IDENTIFIER
%token	F_CONSTANT FUNC_NAME SIZEOF
%token	PTR_OP INC_OP DEC_OP LEFT_OP RIGHT_OP LE_OP GE_OP EQ_OP NE_OP
%token	AND_OP OR_OP MUL_ASSIGN DIV_ASSIGN MOD_ASSIGN ADD_ASSIGN
%token	SUB_ASSIGN LEFT_ASSIGN RIGHT_ASSIGN AND_ASSIGN
%token	XOR_ASSIGN OR_ASSIGN
%token	TYPEDEF_NAME ENUMERATION_CONSTANT

%token	TYPEDEF EXTERN STATIC AUTO REGISTER INLINE
%token	CONST RESTRICT VOLATILE
%token	BOOL CHAR SHORT INT LONG SIGNED UNSIGNED FLOAT DOUBLE VOID
%token	COMPLEX IMAGINARY 
%token	STRUCT UNION ENUM ELLIPSIS

%token	CASE DEFAULT IF ELSE SWITCH WHILE DO FOR GOTO CONTINUE BREAK RETURN

%token	ALIGNAS ALIGNOF ATOMIC GENERIC NORETURN STATIC_ASSERT THREAD_LOCAL

%type <node_ptr> primary_expression constant postfix_expression unary_expression multiplicative_expression additive_expression shift_expression cast_expression relational_expression equality_expression and_expression exclusive_or_expression inclusive_or_expression logical_and_expression logical_or_expression conditional_expression assignment_expression string expression expression_statement block_item block_item_list statement type_specifier declaration_specifiers compound_statement function_definition declarator direct_declarator parameter_type_list parameter_list parameter_declaration external_declaration translation_unit declaration init_declarator init_declarator_list

%start  start_unit
%%

primary_expression
	: IDENTIFIER		{char* val = $1;$$= new NODE(IDENT,(void*)val,0);}
	| constant			{$$ = $1;}
	| string            {$$ = $1;}
	| '(' expression ')'{$$ = $2;}
//	| generic_selection
	;

constant
	: I_CONSTANT		{int* val = (int*)malloc(sizeof(int));*val = $1;$$= new NODE(INTEGER,(void*)val,0);}
//	| F_CONSTANT
//	| ENUMERATION_CONSTANT	/* after it has been defined as such */
	;

//enumeration_constant		/* before it has been defined as such */
//	: IDENTIFIER		{char* val = $1;$$= new NODE(IDENT,(void*)val,0);}
//	;

string
	: STRING_LITERAL    {char* val = $1;$$= new NODE(STRING,(void*)val,0);}
//	| FUNC_NAME
	;

//generic_selection
//	: GENERIC '(' assignment_expression ',' generic_assoc_list ')'
//	;

//generic_assoc_list
//	: generic_association
//	| generic_assoc_list ',' generic_association
//	;

//generic_association
//	: type_name ':' assignment_expression
//	| DEFAULT ':' assignment_expression
//	;

postfix_expression
	: primary_expression											{$$ = $1;}
//	| postfix_expression '[' expression ']'
//	| postfix_expression '(' ')'
//	| postfix_expression '(' argument_expression_list ')'
//	| postfix_expression '.' IDENTIFIER
//	| postfix_expression PTR_OP IDENTIFIER
//	| postfix_expression INC_OP
//	| postfix_expression DEC_OP
//	| '(' type_name ')' '{' initializer_list '}'
//	| '(' type_name ')' '{' initializer_list ',' '}'
	;

//argument_expression_list
//	: assignment_expression
//	| argument_expression_list ',' assignment_expression
//	;

unary_expression
	: postfix_expression											{$$ = $1;}
//	| INC_OP unary_expression
//	| DEC_OP unary_expression
//	| unary_operator cast_expression
//	| SIZEOF unary_expression
//	| SIZEOF '(' type_name ')'
//	| ALIGNOF '(' type_name ')'
	;

//unary_operator
//	: '&'
//	| '*'
//	| '+'
//	| '-'
//	| '~'
//	| '!'
//	;

cast_expression
	: unary_expression												{$$ = $1;}
//	| '(' type_name ')' cast_expression
	;

multiplicative_expression
	: cast_expression												{$$ = $1;}
	| multiplicative_expression '*' cast_expression					{$$ = createBinaryNode(MULT, $1, $3);}
	| multiplicative_expression '/' cast_expression					{$$ = createBinaryNode(DIVIDE, $1, $3);}
	| multiplicative_expression '%' cast_expression					{$$ = createBinaryNode(REMAINDER, $1, $3);}
	;

additive_expression
	: multiplicative_expression										{$$ = $1;}
	| additive_expression '+' multiplicative_expression   			{$$ = createBinaryNode(PLUS, $1, $3);}
	| additive_expression '-' multiplicative_expression				{$$ = createBinaryNode(SUB, $1, $3);}
	;

shift_expression	
	: additive_expression											{$$ = $1;}
	| shift_expression LEFT_OP additive_expression					{$$ = createBinaryNode(LEFT_SHIFT, $1, $3);}
	| shift_expression RIGHT_OP additive_expression					{$$ = createBinaryNode(RIGHT_SHIFT, $1, $3);}
	;

relational_expression
	: shift_expression                                              {$$ = $1;}
	| relational_expression '<' shift_expression					{$$ = createBinaryNode(LESS_THAN, $1, $3);}		
	| relational_expression '>' shift_expression					{$$ = createBinaryNode(GREATER_THAN, $1, $3);}				
	| relational_expression LE_OP shift_expression					{$$ = createBinaryNode(LESS_THAN_EQUAL_TO, $1, $3);}
	| relational_expression GE_OP shift_expression					{$$ = createBinaryNode(GREATER_THAN_EQUAL_TO, $1, $3);}
	;

equality_expression
	: relational_expression                                         {$$ = $1;}
	| equality_expression EQ_OP relational_expression				{$$ = createBinaryNode(EQUAL_TO, $1, $3);}
	| equality_expression NE_OP relational_expression				{$$ = createBinaryNode(NOT_EQUAL_TO, $1, $3);}
	;

and_expression
	: equality_expression                                           {$$ = $1;}
	| and_expression '&' equality_expression						{$$ = createBinaryNode(AND, $1, $3);}
	;

exclusive_or_expression
	: and_expression                                                {$$ = $1;}
	| exclusive_or_expression '^' and_expression					{$$ = createBinaryNode(EXCLUSIVE_OR, $1, $3);}
	;

inclusive_or_expression
	: exclusive_or_expression                                       {$$ = $1;}
	| inclusive_or_expression '|' exclusive_or_expression			{$$ = createBinaryNode(INCLUSIVE_OR, $1, $3);}
	;

logical_and_expression
	: inclusive_or_expression                                       {$$ = $1;}
	| logical_and_expression AND_OP inclusive_or_expression			{$$ = createBinaryNode(LOGICAL_AND, $1, $3);}
	;

logical_or_expression
	: logical_and_expression                                        {$$ = $1;}
	| logical_or_expression OR_OP logical_and_expression			{$$ = createBinaryNode(LOGICAL_OR, $1, $3);}
	;

conditional_expression
	: logical_or_expression                                                     {$$ = $1;}
//	| logical_or_expression '?' expression ':' conditional_expression
	;

assignment_expression
	: conditional_expression                                                    {$$ = $1;}
	| unary_expression '=' assignment_expression                				{$$ = createBinaryNode(ASSIGN, $1, $3);}
	| unary_expression MUL_ASSIGN assignment_expression							{$$ = createBinaryNode(MUL_ASSIGNN, $1, $3);}
	| unary_expression DIV_ASSIGN assignment_expression							{$$ = createBinaryNode(DIV_ASSIGNN, $1, $3);}
	| unary_expression MOD_ASSIGN assignment_expression							{$$ = createBinaryNode(MOD_ASSIGNN, $1, $3);}
	| unary_expression ADD_ASSIGN assignment_expression							{$$ = createBinaryNode(ADD_ASSIGNN, $1, $3);}
	| unary_expression SUB_ASSIGN assignment_expression							{$$ = createBinaryNode(SUB_ASSIGNN, $1, $3);}
	| unary_expression LEFT_ASSIGN assignment_expression						{$$ = createBinaryNode(LEFT_ASSIGNN, $1, $3);}
	| unary_expression RIGHT_ASSIGN assignment_expression						{$$ = createBinaryNode(RIGHT_ASSIGNN, $1, $3);}		
	| unary_expression AND_ASSIGN assignment_expression							{$$ = createBinaryNode(AND_ASSIGNN, $1, $3);}	
	| unary_expression XOR_ASSIGN assignment_expression							{$$ = createBinaryNode(XOR_ASSIGNN, $1, $3);}	
	| unary_expression OR_ASSIGN assignment_expression							{$$ = createBinaryNode(OR_ASSIGNN, $1, $3);}	
	;

expression
	: assignment_expression														{$$ = $1;}
//	| expression ',' assignment_expression
	;

//constant_expression
//	: conditional_expression	/* with constraints */
//	;

declaration
//	: declaration_specifiers ';'								
	: declaration_specifiers init_declarator_list ';'		{$$ = createBinaryNode(DECLARATION,$1,$2);}  //DECLARATION_LIST in int m,k,l; is [m,k,l]	
//	| static_assert_declaration		
	;

declaration_specifiers
//	: storage_class_specifier declaration_specifiers
//	| storage_class_specifier
//	| type_specifier declaration_specifiers
	: type_specifier										{$$ = $1;}
//	| type_qualifier declaration_specifiers
//	| type_qualifier
//	| function_specifier declaration_specifiers
//	| function_specifier
//	| alignment_specifier declaration_specifiers
//	| alignment_specifier
	;

init_declarator_list
	: init_declarator										{$$=$1;}
	| init_declarator_list ',' init_declarator				{$$ = $1; addChild($$, $3);}
	;

init_declarator
//	: declarator '=' initializer
	: declarator											{$$ = createUnaryNode(DECLARATION_LIST, $1);}
	;

//storage_class_specifier
//	: TYPEDEF	/* identifiers must be flagged as TYPEDEF_NAME */
//	| EXTERN
//	| STATIC
//	| THREAD_LOCAL
//	| AUTO
//	| REGISTER
//	;

type_specifier
	: VOID													{$$ = new NODE(TYPE_VOID,NULL,0);}
	| CHAR													{$$ = new NODE(TYPE_CHAR,NULL,0);}	
	| SHORT													{$$ = new NODE(TYPE_SHORT,NULL,0);}
	| INT													{$$ = new NODE(TYPE_INT,NULL,0);}
	| LONG													{$$ = new NODE(TYPE_LONG,NULL,0);}
	| FLOAT													{$$ = new NODE(TYPE_FLOAT,NULL,0);}
	| DOUBLE												{$$ = new NODE(TYPE_DOUBLE,NULL,0);}
//	| SIGNED												{$$ = new NODE(TYPE_VOID,NULL,0);}
//	| UNSIGNED												{$$ = new NODE(TYPE_VOID,NULL,0);}
	| BOOL													{$$ = new NODE(TYPE_BOOL,NULL,0);}
//	| COMPLEX												
//	| IMAGINARY	  	/* non-mandated extension */
//	| atomic_type_specifier
//	| struct_or_union_specifier
//	| enum_specifier
//	| TYPEDEF_NAME		/* after it has been defined as such */
	;

//struct_or_union_specifier
//	: struct_or_union '{' struct_declaration_list '}'
//	| struct_or_union IDENTIFIER '{' struct_declaration_list '}'
//	| struct_or_union IDENTIFIER
//	;

//struct_or_union
//	: STRUCT
//	| UNION
//	;

//struct_declaration_list
//	: struct_declaration
//	| struct_declaration_list struct_declaration
//	;

//struct_declaration
//	: specifier_qualifier_list ';'	/* for anonymous struct/union */
//	| specifier_qualifier_list struct_declarator_list ';'
//	| static_assert_declaration
//	;

//specifier_qualifier_list
//	: type_specifier specifier_qualifier_list
//	| type_specifier
//	| type_qualifier specifier_qualifier_list
//	| type_qualifier
//	;

//struct_declarator_list
//	: struct_declarator
//	| struct_declarator_list ',' struct_declarator
//	;

//struct_declarator
//	: ':' constant_expression
//	| declarator ':' constant_expression
//	| declarator
//	;

//enum_specifier
//	: ENUM '{' enumerator_list '}'
//	| ENUM '{' enumerator_list ',' '}'
//	| ENUM IDENTIFIER '{' enumerator_list '}'
//	| ENUM IDENTIFIER '{' enumerator_list ',' '}'
//	| ENUM IDENTIFIER
//	;

//enumerator_list
//	: enumerator
//	| enumerator_list ',' enumerator
//	;

//enumerator	/* identifiers must be flagged as ENUMERATION_CONSTANT */
//	: enumeration_constant '=' constant_expression
//	| enumeration_constant
//	;

//atomic_type_specifier
//	: ATOMIC '(' type_name ')'
//	;

//type_qualifier
//	: CONST
//	| RESTRICT
//	| VOLATILE
//	| ATOMIC
//	;

//function_specifier
//	: INLINE
//	| NORETURN
//	;

//alignment_specifier
//	: ALIGNAS '(' type_name ')'
//	| ALIGNAS '(' constant_expression ')'
//	;

declarator
//	: pointer direct_declarator			
	: direct_declarator					{$$ = $1;}
	;

direct_declarator
	: IDENTIFIER															{char* val = $1;$$= new NODE(IDENT,(void*)val,0);}													
	| '(' declarator ')'													{$$ = $2;}
//	| direct_declarator '[' ']'			
//	| direct_declarator '[' '*' ']'
//	| direct_declarator '[' STATIC type_qualifier_list assignment_expression ']'
//	| direct_declarator '[' STATIC assignment_expression ']'
//	| direct_declarator '[' type_qualifier_list '*' ']'
//	| direct_declarator '[' type_qualifier_list STATIC assignment_expression ']'
//	| direct_declarator '[' type_qualifier_list assignment_expression ']'
//	| direct_declarator '[' type_qualifier_list ']'
//	| direct_declarator '[' assignment_expression ']'
	| direct_declarator '(' parameter_type_list ')'							{$$ = createBinaryNode(FUNC_DECLARATOR, $1, $3);}		
	| direct_declarator '(' ')'												{$$ = createUnaryNode(FUNC_DECLARATOR,$1);}
//	| direct_declarator '(' identifier_list ')'
	;

//pointer
//	: '*' type_qualifier_list pointer
//	| '*' type_qualifier_list
//	| '*' pointer
//	| '*'
//	;

//type_qualifier_list
//	: type_qualifier
//	| type_qualifier_list type_qualifier
//	;


parameter_type_list
//	: parameter_list ',' ELLIPSIS
	: parameter_list										{$$ = $1;}
	;

parameter_list
	: parameter_declaration									{$$ = $1;}
	| parameter_list ',' parameter_declaration				{$$=$1;addChild($$,$3);}
	;

parameter_declaration
	: declaration_specifiers declarator						{NODE* m = createBinaryNode(DECLARATION, $1, $2); $$ = createUnaryNode(PARAMETERS,m);}
//	| declaration_specifiers abstract_declarator
	| declaration_specifiers								{$$ = createUnaryNode(PARAMETERS, $1);}
	;

//identifier_list
//	: IDENTIFIER
//	| identifier_list ',' IDENTIFIER
//	;

//type_name
//	: specifier_qualifier_list abstract_declarator
//	| specifier_qualifier_list
//	;

//abstract_declarator
//	: pointer direct_abstract_declarator
//	| pointer
//	| direct_abstract_declarator
//	;

//direct_abstract_declarator
//	: '(' abstract_declarator ')'
//	| '[' ']'
//	| '[' '*' ']'
//	| '[' STATIC type_qualifier_list assignment_expression ']'
//	| '[' STATIC assignment_expression ']'
//	| '[' type_qualifier_list STATIC assignment_expression ']'
//	| '[' type_qualifier_list assignment_expression ']'
//	| '[' type_qualifier_list ']'
//	| '[' assignment_expression ']'
//	| direct_abstract_declarator '[' ']'
//	| direct_abstract_declarator '[' '*' ']'
//	| direct_abstract_declarator '[' STATIC type_qualifier_list assignment_expression ']'
//	| direct_abstract_declarator '[' STATIC assignment_expression ']'
//	| direct_abstract_declarator '[' type_qualifier_list assignment_expression ']'
//	| direct_abstract_declarator '[' type_qualifier_list STATIC assignment_expression ']'
//	| direct_abstract_declarator '[' type_qualifier_list ']'
//	| direct_abstract_declarator '[' assignment_expression ']'
//	| '(' ')'
//	| '(' parameter_type_list ')'
//	| direct_abstract_declarator '(' ')'
//	| direct_abstract_declarator '(' parameter_type_list ')'
//	;

//initializer
//	: '{' initializer_list '}'
//	| '{' initializer_list ',' '}'
//	| assignment_expression
//	;

//initializer_list
//	: designation initializer
//	| initializer
//	| initializer_list ',' designation initializer
//	| initializer_list ',' initializer
//	;

//designation
//	: designator_list '='
//	;

//designator_list
//	: designator
//	| designator_list designator
//	;

//designator
//	: '[' constant_expression ']'
//	| '.' IDENTIFIER
//	;

//static_assert_declaration
//	: STATIC_ASSERT '(' constant_expression ',' STRING_LITERAL ')' ';'
//	;

statement
//	: labeled_statement
//	| compound_statement
	: expression_statement									{$$ = $1;}
//	| selection_statement
//	| iteration_statement
//	| jump_statement
	;

//labeled_statement
//	: IDENTIFIER ':' statement
//	| CASE constant_expression ':' statement
//	| DEFAULT ':' statement
//	;

compound_statement
	: '{' '}'													{$$ = new NODE(BLOCK, NULL, 0);}
	| '{'  block_item_list '}'  								{$$ = $2;}
	;

block_item_list
	: block_item												{$$ = $1;}
	| block_item_list block_item								{$$ = $1; addChild($$,$2);} //addChild($$, $2);}
	;

block_item
	: declaration												{$$ = createUnaryNode(BLOCK,$1);}
	| statement													{$$ = createUnaryNode(BLOCK,$1);}
	;

expression_statement
//	: ';'														{$$ = NULL;}
	: expression ';'											{$$ = $1;}
	;

//selection_statement
//	: IF '(' expression ')' statement ELSE statement
//	| IF '(' expression ')' statement
//	| SWITCH '(' expression ')' statement
//	;

//iteration_statement
//	: WHILE '(' expression ')' statement
//	| DO statement WHILE '(' expression ')' ';'
//	| FOR '(' expression_statement expression_statement ')' statement
//	| FOR '(' expression_statement expression_statement expression ')' statement
//	| FOR '(' declaration expression_statement ')' statement
//	| FOR '(' declaration expression_statement expression ')' statement
//	;

//jump_statement
//	: GOTO IDENTIFIER ';'
//	| CONTINUE ';'
//	| BREAK ';'
//	| RETURN ';'
//	| RETURN expression ';'
//	;

start_unit
	:translation_unit															{printTree($1);cout<<'\n';}

translation_unit
	: external_declaration                                          			{$$ = $1;}
	| translation_unit external_declaration										{$$ = $1;addChild($$,$2);}
	;

external_declaration
	: function_definition														{$$ = createUnaryNode(CODE_SECTIONS,$1);}
//	| declaration
	;

function_definition
//	: declaration_specifiers declarator declaration_list compound_statement		//{$$ = createFunction($1,$2,$3,$4);}			
	: declaration_specifiers declarator compound_statement						{$$ = createFunction($1,$2,$3);}						
	;

//declaration_list
//	: declaration										
//	| declaration_list declaration							
//	;

%%
#include <stdio.h>


NODE* createFunction(NODE* specifier, NODE* func_dec, NODE* function_body){
	NODE* m = new NODE(FUNC_DEF, NULL, 4);
	*(m->children++) = *specifier;
	*(m->children++) = *func_dec;
	*(m->children++) = *function_body;
	return m;
}

NODE* createUnaryNode(SYMBOL sym, NODE* child){
	NODE* m = new NODE(sym, NULL, 1);
	*(m->children++) = *child;
	return m;
}

NODE* createBinaryNode(SYMBOL sym, NODE* left, NODE* right){
	NODE* m = new NODE(sym, NULL, 2);
	*(m->children++) = *left;
	*(m->children++) = *right;
	return m;
}

void addChild(NODE* parent, NODE* child){
	int totalChildren = (parent->children - parent->bp);
	if(totalChildren == parent->numChildren){
		NODE* new_arr = (NODE*)malloc(2*(totalChildren+1)*sizeof(NODE)); // +1 to avoid infinite loop in totalChildren=0 case.
		parent->numChildren = 2*(totalChildren+1);
		NODE* new_bp = new_arr;
		NODE* itr = parent->bp;
		while(itr!=parent->children){
			*(new_arr++) = *(itr++);
		}
		free(parent->bp);
		parent->children = new_arr;
		parent->bp = new_bp;
		*(parent->children++) = *(child->bp);
	}
	else if(totalChildren < parent->numChildren){
		*(parent->children++) = *(child->bp);
	}
	else{
		cout<<"The number of children assigned are greater than the number of children allowed!!!\n";
	}
	return;
}

void printTree(NODE* p){
    switch(p->symbol){   
    	case INTEGER:
    		cout << "INTEGER("<<*((int*)p->value)<<")";	
    		break;
    	case IDENT:
    		cout << "IDENT("<<(char*)p->value<<")";
    		break;
    	case STRING:
    		cout << "STRING("<<(char*)p->value<<")";
    		break;    		    
    	case TYPE_VOID:
    		cout << "TYPE_VOID";
    		break;
    	case TYPE_INT:
    		cout << "TYPE_INT";
    		break;
    	case TYPE_SHORT:
    		cout << "TYPE_SHORT";
    		break;
    	case TYPE_LONG:
    		cout << "TYPE_LONG";
    		break;
    	case TYPE_DOUBLE:
    		cout << "TYPE_DOUBLE";
    		break;
    	case TYPE_BOOL:
    		cout << "TYPE_BOOL";
    		break;
    	case TYPE_CHAR:
    		cout << "TYPE_CHAR";
    		break;
    	case TYPE_FLOAT:
    		cout << "TYPE_FLOAT";
    		break;
    
    		
    	//Write all the binary rules here
    	case PLUS:
    		cout << "[PLUS[";
    	case ASSIGN:
    		if(p->symbol == ASSIGN)cout << "[ASSIGN[";
    	case MUL_ASSIGNN:
    		if(p->symbol == MUL_ASSIGNN)cout << "[MUL_ASSIGN[";
    	case DIV_ASSIGNN:
    		if(p->symbol == DIV_ASSIGNN)cout << "[DIV_ASSIGN[";
    	case MOD_ASSIGNN:
    		if(p->symbol == MOD_ASSIGNN)cout << "[MOD_ASSIGN[";
    	case ADD_ASSIGNN:
    		if(p->symbol == ADD_ASSIGNN)cout << "[ADD_ASSIGN[";
    	case SUB_ASSIGNN:
    		if(p->symbol == SUB_ASSIGNN)cout << "[SUB_ASSIGN[";
    	case LEFT_ASSIGNN:
    		if(p->symbol == LEFT_ASSIGNN)cout << "[LEFT_ASSIGN[";
    	case RIGHT_ASSIGNN:
    		if(p->symbol == RIGHT_ASSIGNN)cout << "[RIGHT_ASSIGN[";
    	case AND_ASSIGNN:
    		if(p->symbol == AND_ASSIGNN)cout << "[AND_ASSIGN[";
    	case XOR_ASSIGNN:
    		if(p->symbol == XOR_ASSIGNN)cout << "[XOR_ASSIGN[";
    	case OR_ASSIGNN:
    		if(p->symbol == OR_ASSIGNN)cout << "[OR_ASSIGN[";
    	case SUB:
    		if(p->symbol == SUB)cout << "[SUBTRACT[";
    	case DIVIDE:
    		if(p->symbol == DIVIDE)cout << "[DIVIDE[";
    	case REMAINDER:
    		if(p->symbol == REMAINDER)cout << "[REMAINDER[";
    	case LEFT_SHIFT:
    		if(p->symbol == LEFT_SHIFT)cout << "[LEFT_SHIFT[";
    	case RIGHT_SHIFT:
    		if(p->symbol == RIGHT_SHIFT)cout << "[RIGHT_SHIFT[";
    	case LESS_THAN_EQUAL_TO:
    		if(p->symbol == LESS_THAN_EQUAL_TO)cout << "[LESS_THAN_EQUAL_TO[";
    	case GREATER_THAN_EQUAL_TO:
    		if(p->symbol == GREATER_THAN_EQUAL_TO)cout << "[GREATER_THAN_EQUAL_TO[";
	    case LESS_THAN:
    		if(p->symbol == LESS_THAN)cout << "[LESS_THAN[";
    	case GREATER_THAN:
    		if(p->symbol == GREATER_THAN)cout << "[GREATER_THAN[";
		case EQUAL_TO:
    		if(p->symbol == EQUAL_TO)cout << "[EQUAL_TO[";
	    case NOT_EQUAL_TO:
    		if(p->symbol == NOT_EQUAL_TO)cout << "[NOT_EQUAL_TO[";
    	case AND:
    		if(p->symbol == AND)cout << "[AND[";
    	case EXCLUSIVE_OR:
    		if(p->symbol == EXCLUSIVE_OR)cout << "[EXCLUSIVE_OR[";
    	case INCLUSIVE_OR:
    		if(p->symbol == INCLUSIVE_OR)cout << "[INCLUSIVE_OR[";
    	case LOGICAL_AND:
    		if(p->symbol == LOGICAL_AND)cout << "[LOGICAL_AND[";
    	case LOGICAL_OR:
    		if(p->symbol == LOGICAL_OR)cout << "[LOGICAL_OR[";
    	case MULT:
    		if(p->symbol == MULT)cout << "[MULT[";
    	case DECLARATION:
    		if(p->symbol == DECLARATION)cout << "[DECLARATION[";
    		printTree(p->bp++);
    		cout << "],[";
    		printTree(p->bp++);
    		cout << "]]";
    		break;
    		
    	case BLOCK:
    		cout<< "BLOCK_LIST[";
    	case DECLARATION_LIST:
    		if(p->symbol == DECLARATION_LIST)cout<< "DECLARATION_LIST[";
    	case FUNC_DEF:
    		if(p->symbol == FUNC_DEF)cout<< "FUNC_DEF[";
    	case FUNC_DECLARATOR:
    		if(p->symbol == FUNC_DECLARATOR)cout<< "FUNC_DECLARATOR[";
    	case PARAMETERS:
    		if(p->symbol == PARAMETERS)cout<< "PARAMETERS[";
    	case CODE_SECTIONS:
    		if(p->symbol == CODE_SECTIONS)cout<< "CODE_SECTIONS[";
    		while(p->bp != p->children){
    			cout<<"|[";
    			printTree(p->bp++);
    			cout<<"]";
    		}
    		cout<<"]";
    		break;
    	DEFAULT:
    		cout<<"No prining rule for "<<p->symbol<<endl;
    }
    return;
}


void yyerror(const char *s)
{
	fflush(stdout);
	fprintf(stderr, "*** %s\n", s);
}


