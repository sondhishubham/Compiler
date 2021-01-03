

int printf(char const *format, ...);

int sum(int a, int b);

int main(){
	int p = 10, k, l , m;
	l = 1000;
	k = sum(1,2);
	if(p){
		int k = 10;
		k = l;
	}
	printf("The answer is %d\n", k);
}


int sum(int a, int b){
	return a+b;
}

