

int printf(char const *format, ...);


int sum(int a, int b){
	return a+b;
}

int main(int argc, char **argv){
	int p = 10, k, l , m;
	k = p + 3;
	if(p > 11){
		while(p > 0){
			k += k;
			p -= 1;
		}
	}
	else{
		while(p > 0){
			k+= p;
			p-=1;
		}
	}
	char* ll = "The answer is %d\n";
	printf(ll, p);
}


