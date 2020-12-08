int factorial(int n)
{
	if (n <= 0)
		return 1;
	else return n*factorial(n-1);
}


/*int main()*/
/*{*/
/*  return fibonacci(factorial(1), factorial(2), factorial(5));*/
/*}*/



/*CODE_SECTIONS[|[FUNC_DEF[|[TYPE_INT]|[FUNC_DECLARATOR[|[IDENT(factorial)]|[PARAMETERS[|[[DECLARATION[TYPE_INT],[IDENT(n)]]]]]]]|[BLOCK_LIST[|[IFTHEN[|[[LESS_THAN_EQUAL_TO[IDENT(n)],[INTEGER(0)]]]|[RETURN[|[INTEGER(1)]]]|[RETURN[|[[MULT[IDENT(n)],[FUNC_CALL[|[IDENT(factorial)]|[ARGUMENTS[|[[SUBTRACT[IDENT(n)],[INTEGER(1)]]]]]]]]]]]]]]]]]]*/

