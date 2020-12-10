int factorial(int n)
{
	if (n <= 0)
		return 1;
	else return n*factorial(n-1);
}

int fibonacci(int aaaa, int b, int n)
{
	if (n <= 0)
		return a;

	while (n > 0) {
		int t;
		t = b;
		b = a+b;
		a = t;
		n = n-1;
	}
	return b;
	int aaaa;
}

int main()
{
  return fibonacci(factorial(1), factorial(2), factorial(5));
}	


/*CODE_SECTIONS[|[FUNC_DEF[|[TYPE_INT]|[FUNC_DECLARATOR[|[IDENT(printf)]|[PARAMETERS[|[[DECLARATION[TYPE_INT],[IDENT(a)]]]|[[DECLARATION[TYPE_INT],[IDENT(k)]]]|[[DECLARATION[TYPE_INT],[IDENT(n)]]]]]]]|[BLOCK_LIST[|[[DECLARATION[TYPE_INT],[DECLARATION_LIST[|[IDENT(m)]|[IDENT(l)]|[IDENT(p)]|[IDENT(q)]]]]]|[IFTHEN[|[[ASSIGN[IDENT(a)],[INTEGER(10)]]]|[BLOCK_LIST[|[[DECLARATION[TYPE_INT],[DECLARATION_LIST[|[[INITIALIZE[IDENT(m)],[IDENT(l)]]]]]]]]]]]]]]]]retv = 0*/

