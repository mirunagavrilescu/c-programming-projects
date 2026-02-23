#include <stdio.h>
#include <stdlib.h>

int verificare_elicopter(int a[][1005], int l1,
						 int c1, int l2, int c2, int s)
{
	int patratele_afectate = 0;//nr de patratele afectate de umbra elicopterului
	int total_patratele = 0;//nr total patratele in triunghi

	if (l1 > l2) {
		int aux = l1; l1 = l2; l2 = aux;
		aux = c1; c1 = c2; c2 = aux;
	}
	if (s == -1) {//pentru patratele cu vf dedesubt
		if (c1 < c2) {//triunghi cu vf spre dreapta jos
			for (int i = l1; i <= l2; i++) {
				int latime = i - l1 + 1;//latimea triunghiului
				for (int d = 0; d < latime; d++) {
					int j = c1 + d;//coloana curenta
					if (a[i][j] == 0) {
						patratele_afectate++;
					}
					total_patratele++;
				}
			}
		} else {//pentru triunghi cu vf spre stanga jos
			for (int i = l1; i <= l2; i++) {
				int latime = i - l1 + 1;
				for (int d = 0; d < latime; d++) {
					int j = c1 - d;
					if (a[i][j] == 0) {
						patratele_afectate++;
					}
					total_patratele++;
				}
			}
		}
	} else {//pentru patratele cu vf deasupra
		if (c1 > c2) {//triunghi cu vf spre dreapta sus
			for (int i = l2; i >= l1; i--) {
				int latime = l2 - i + 1;
				for (int d = 0; d < latime; d++) {
					int j = c2 + d;
					if (a[i][j] == 0) {
						patratele_afectate++;
					}
					total_patratele++;
				}
			}
		} else {//triunghi cu vf spre stanga sus
			for (int i = l2; i >= l1; i--) {
				int latime = l2 - i + 1;
				for (int d = 0; d < latime; d++) {
					int j = c2 - d;
					if (a[i][j] == 0) {
						patratele_afectate++;
					}
					total_patratele++;
				}
			}
		}
	}
	if (patratele_afectate > total_patratele / 2) {
		return -1;
	} //elicopter aterizat greșit
	// mai mult de jumatate de patratele afectate
	if (patratele_afectate == 0) {
		return 1;
	} //elicopter care nu afecteaza niciun patratel
	return 0;//elicopterul face umbra
	//dar nu la mai mult de jumatate din patratelele
}

int main(void)
{
	int n, m;
	scanf("%d %d", &n, &m);
	int a[1005][1005];
	for (int i = 1; i <= n; i++) {
		for (int j = 1; j <= m; j++) {
			scanf("%d", &a[i][j]);
		}
	}
	int k;
	scanf("%d", &k);
	int elc_gresite[805];
	int x = 0; // elicoptere care nu fac umbra deloc
	int y = 0; // elicoptere care au aterizat gresit

	for (int p = 1; p <= k; p++) {
		int l1, c1, l2, c2, s;
		scanf("%d %d %d %d %d", &l1, &c1, &l2, &c2, &s);
		if (l1 == l2 || c1 == c2 || abs(l1 - l2) != abs(c1 - c2)) {
			printf("Elicopterul %d este pozitionat necorespunzator!\n", p);
		} //ipotenuza este pe orizontal sau pepe vertical
		//sau nu e paralela cu o pseudodiagonala
		else {
			int rez = verificare_elicopter(a, l1, c1, l2, c2, s);
			if (rez == -1) {
				elc_gresite[y++] = p;
			} else {
				if (rez == 1) {
					x++;
				}
			}
		}
	}

	printf("%d\n", x);
	printf("%d\n", y);
	for (int i = 0; i < y; i++) {
		printf("%d ", elc_gresite[i]);
	}
	printf("\n");

	return 0;
}
