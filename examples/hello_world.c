int it(_Bool m, _Bool t){
	return m + t;
}

_Bool bt(int k, char c, _Bool mm){
	int kk = k + c + mm;
	return kk;
}

char ct(int k, char c, _Bool mm){
	int kk = k + c + mm;
	return kk;
}
int main()
{
	int kk 	= it(1,1);
	_Bool t1 = 1;
	_Bool ll = bt(10, 'a', t1);
	int las	= ct(10, 'a', 0);
}	
