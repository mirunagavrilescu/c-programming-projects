#include <stdio.h>

// calculez cel mai mare divizor comun
unsigned long long cmmdc(unsigned long long a, unsigned long long b)
{
	while (b != 0) {
		unsigned long long r = a % b;
		a = b;
		b = r;
	}
	return a;
}

// calculez cel mai mic multiplu comun
unsigned long long cmmmc(unsigned long long a, unsigned long long b)
{
	unsigned long long p = cmmdc(a, b);
	return (a / p) * b;
}

int main(void)
{
	unsigned long long window;
	scanf("%llu", &window);
	unsigned long long t[10001], x[10001];
	int k = 0;
	//citesc toate perechile care intra pana la ultima pereche (0,0)
	while (1) {
		scanf("%llu %llu", &t[k], &x[k]);
		if (t[k] == 0 && x[k] == 0)
			break;
		k++;//salvez cate intrari sunt
	}
	//iau toate perechile de valori doua cate doua
	//verific dacă diferența de timp dintre cele două este încă validă
	//calculez cmmdc si cmmmc
	for (int i = 0; i < k; i++) {
		for (int j = i + 1; j < k; j++) {
			if (t[j] - t[i] <= window) {
				printf("%llu %llu\n", cmmmc(x[i], x[j]), cmmdc(x[i], x[j]));
			}
		}
	}
	return 0;
}
