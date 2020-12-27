
int printf(char const *format, ...);

/*int it(_Bool m, _Bool t){*/
/*	return m + t;*/
/*}*/

/*_Bool bt(int k, char c, _Bool mm){*/
/*	int kk = k + c + mm;*/
/*	return kk;*/
/*}*/

/*char ct(int k, char c, _Bool mm){*/
/*	int kk = k + c + mm;*/
/*	return kk;*/
/*}*/
/*int main()*/
/*{*/
/*	int kk 	= it(1,1);*/
/*	_Bool t1 = 1;*/
/*	char ll = bt(10, 'a', t1) + 'a';*/
/*	_Bool las	= ct(10, 'a', 0);*/
/*	printf("The int is %d \t The char is %c \t The bool is %d\n",kk,ll,las);*/
/*}	*/


char p = 'a';

int main(){
	int a = 15;
	int b = 10;
	int l = p||a||b||p;
	printf("%d\n", l);
}
