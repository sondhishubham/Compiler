

int printf(char const *format, ...);


int sum(int a, int b){
	return a+b;
}

int main(){
	int p = 10, k, l , m;
	l = 1000;
	int mm = 3-10;
	k = p;
	while(p > 0){
		k = k + mm;
		p -= 1;
	}
	
	char* ll = "The answer is %d\n";
	printf(ll, k);
}


