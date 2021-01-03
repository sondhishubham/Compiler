#include <iostream>
#include "symbol_table.h"
#include <stdio.h>
#include <string.h>
using namespace std;

extern bool isChanged;

void constantFolding(NODE* p){
	if(p==NULL || p->symbol == INTEGER || p->symbol == STRING || p->symbol == IDENT || p->symbol == ELLIPSISS || p->numChildren == 0) return;
	p->bp = p->const_bp;
	if(p->symbol == PLUS || p->symbol == SUB || p->symbol == MULT || p->symbol == DIVIDE || p->symbol == REMAINDER || p->symbol == LEFT_SHIFT || p->symbol == RIGHT_SHIFT || p->symbol == EXCLUSIVE_OR || p->symbol == INCLUSIVE_OR || p->symbol == AND || p->symbol == LOGICAL_AND || p->symbol == LOGICAL_OR || p->symbol == LESS_THAN || p->symbol == GREATER_THAN || p->symbol == LESS_THAN_EQUAL_TO || p->symbol == GREATER_THAN_EQUAL_TO || p->symbol == EQUAL_TO || p->symbol == NOT_EQUAL_TO){
		NODE* firstNumber	= p->bp++;
		NODE* secondNumber	= p->bp++;
		constantFolding(firstNumber);
		constantFolding(secondNumber);
		if(firstNumber->symbol==INTEGER && secondNumber->symbol == INTEGER){
			isChanged = true;
			int* val = (int*)malloc(sizeof(int));
			switch(p->symbol){
				case PLUS:
					*val = *((int*)firstNumber->value) + *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case SUB:
					*val = *((int*)firstNumber->value) - *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case MULT:
					*val = *((int*)firstNumber->value) * *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case DIVIDE:
					*val = *((int*)firstNumber->value) / *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case REMAINDER:
					*val = *((int*)firstNumber->value) % *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case LEFT_SHIFT:
					*val = *((int*)firstNumber->value) << *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case RIGHT_SHIFT:
					*val = *((int*)firstNumber->value) >> *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case EXCLUSIVE_OR:
					*val = *((int*)firstNumber->value) ^ *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case INCLUSIVE_OR:
				case LOGICAL_OR:
					*val = *((int*)firstNumber->value) || *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case AND:
				case LOGICAL_AND:
					*val = *((int*)firstNumber->value) && *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case LESS_THAN:
					*val = *((int*)firstNumber->value) < *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case GREATER_THAN:
					*val = *((int*)firstNumber->value) > *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case LESS_THAN_EQUAL_TO:
					*val = *((int*)firstNumber->value) <= *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case GREATER_THAN_EQUAL_TO:
					*val = *((int*)firstNumber->value) >= *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case EQUAL_TO:
					*val = *((int*)firstNumber->value) == *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
				case NOT_EQUAL_TO:
					*val = *((int*)firstNumber->value) != *((int*)secondNumber->value);
					p->symbol = INTEGER;
					break;
			}
			p->numChildren = 0;
			p->children = p->const_bp;
			p->bp = p->const_bp;
			p->value = (void*)val;
		}
		else if(firstNumber->symbol == INTEGER && *((int*)firstNumber->value)==0){	
			int* val = (int*)malloc(sizeof(int));
			if(p->symbol == PLUS || p->symbol == LOGICAL_OR || p->symbol == INCLUSIVE_OR || p->symbol == EXCLUSIVE_OR){
				isChanged 	= true;
				*p 			= *secondNumber;
			}
			else if(p->symbol == MULT || p->symbol == LEFT_SHIFT || p->symbol == RIGHT_SHIFT || p->symbol == REMAINDER || p->symbol == DIVIDE || p->symbol == AND || p->symbol == LOGICAL_AND){
				isChanged 	= true;
				*val = 0;
				p->numChildren = 0;
				p->children = p->const_bp;
				p->bp 		= p->const_bp;
				p->symbol 	= INTEGER;
				p->value 	= (void*)val;
			}
		}
		else if(secondNumber->symbol == INTEGER && *((int*)secondNumber->value)==0){
			int* val = (int*)malloc(sizeof(int));
			if(p->symbol == PLUS || p->symbol == SUB || p->symbol == LOGICAL_OR || p->symbol == INCLUSIVE_OR || p->symbol == EXCLUSIVE_OR || p->symbol == LEFT_SHIFT || p->symbol == RIGHT_SHIFT){
				isChanged 	= true;
				*p = *firstNumber;
			}
			else if(p->symbol == MULT || p->symbol == AND || p->symbol == LOGICAL_AND){
				*val = 0;
				isChanged 	= true;
				p->numChildren = 0;
				p->children = p->const_bp;
				p->bp 		= p->const_bp;
				p->symbol 	= INTEGER;
				p->value 	= (void*)val;
			}
			else if(p->symbol == DIVIDE || p->symbol == REMAINDER){
				cout << "Division by 0\n";
				exit(0);
			}
		}
		else if(firstNumber->symbol == INTEGER && *((int*)firstNumber->value)!=0){	
			int* val = (int*)malloc(sizeof(int));
			if(p->symbol == AND || p->symbol == LOGICAL_AND || (*((int*)firstNumber->value)==1 && (p->symbol == MULT))){
				*p = *secondNumber;
				isChanged 	= true;
			}
			else if(p->symbol == LOGICAL_OR || p->symbol == INCLUSIVE_OR){
				*val = 1;
				isChanged 	= true;
				p->numChildren = 0;
				p->children = p->const_bp;
				p->bp 		= p->const_bp;
				p->symbol 	= INTEGER;
				p->value 	= (void*)val;
			}
		}
		else if(secondNumber->symbol == INTEGER && *((int*)secondNumber->value)!=0){
			int* val = (int*)malloc(sizeof(int));
			if(p->symbol == AND || p->symbol == LOGICAL_AND || (*((int*)secondNumber->value)==1 && (p->symbol == MULT || p->symbol == DIVIDE))){
				*p = *firstNumber;
				isChanged 	= true;
			}
			else if(p->symbol == LOGICAL_OR || p->symbol == INCLUSIVE_OR){
				*val = 1;
				isChanged 	= true;
				p->numChildren = 0;
				p->children = p->const_bp;
				p->bp 		= p->const_bp;
				p->symbol 	= INTEGER;
				p->value 	= (void*)val;
			}
		}
		return;
	}
	if(p->symbol == IFTHEN){
		NODE* cond = p->bp++;
		NODE* trueBlock = p->bp++;
		NODE* falseBlock = (p->bp == p->children)?(NULL):(p->bp++);
		constantFolding(cond);
		constantFolding(trueBlock);
		constantFolding(falseBlock);
		if(cond->symbol == INTEGER){
			isChanged	= true;
			if (*((int*)cond->value) != 0)
				*p = *trueBlock;
			else{
				if(falseBlock == NULL){
					*p = *(new NODE(BLOCK, NULL, 0));
					}
				else
					*p = *falseBlock;
			}
		}
		return;
	}
	if(p->symbol == WHILEE){
		return;
	}
	while(p->bp != p->children){
		constantFolding(p->bp++);
	}
	return; 
}

