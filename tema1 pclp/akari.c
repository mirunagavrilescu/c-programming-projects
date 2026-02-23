#include <stdio.h>

//caut casutele care au bec si pentru fiecare merg in toate dir(sus,jos,st,dr)
//pun pe casutele albe care au caracterul '-' caracterul 'x'
//pana la intalnirea unei casute cu '#' sau cifra sau pana la margine
void propagare_lumina(char grid[51][51], int n, int m)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (grid[i][j] == 'L') {
				int k = j + 1;
				while (k < m) {
					if (grid[i][k] == '#' ||
						(grid[i][k] >= '0' && grid[i][k] <= '4')) {
						break;
					}
					if (grid[i][k] == '-') {
						grid[i][k] = 'x';
					}
					k++;
				}
				k = j - 1;
				while (k >= 0) {
					if (grid[i][k] == '#' ||
						(grid[i][k] >= '0' && grid[i][k] <= '4')) {
						break;
					}
					if (grid[i][k] == '-') {
						grid[i][k] = 'x';
					}
					k--;
				}
				k = i + 1;
				while (k < n) {
					if (grid[k][j] == '#' ||
						(grid[k][j] >= '0' && grid[k][j] <= '4')) {
						break;
					}
					if (grid[k][j] == '-') {
						grid[k][j] = 'x';
					}
					k++;
				}
				k = i - 1;
				while (k >= 0) {
					if (grid[k][j] == '#' ||
						(grid[k][j] >= '0' && grid[k][j] <= '4')) {
						break;
					}
					if (grid[k][j] == '-') {
						grid[k][j] = 'x';
					}
					k--;
				}
			}
		}
	}
}

//pentru casutele care contin cifre merg cate o casuta in fiecare directie
//numar cate becuri sunt,daca nr de becuri este egal cu numarul din casuta
//pun pe toate casutele adiacente care au '-' caracterul 'x'
void celule_numerotate(char grid[51][51], int n, int m)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (grid[i][j] >= '0' && grid[i][j] <= '4') {
				int nr_becuri = 0;
				int r = grid[i][j] - '0';
				if (i - 1 >= 0 && grid[i - 1][j] == 'L') {
					nr_becuri++;
				}
				if (i + 1 < n && grid[i + 1][j] == 'L') {
					nr_becuri++;
				}
				if (j - 1 >= 0 && grid[i][j - 1] == 'L') {
					nr_becuri++;
				}
				if (j + 1 < m && grid[i][j + 1] == 'L') {
					nr_becuri++;
				}

				if (nr_becuri == r) {
					if (i - 1 >= 0 && grid[i - 1][j] == '-') {
						grid[i - 1][j] = 'x';
					}
					if (i + 1 < n && grid[i + 1][j] == '-') {
						grid[i + 1][j] = 'x';
					}
					if (j - 1 >= 0 && grid[i][j - 1] == '-') {
						grid[i][j - 1] = 'x';
					}
					if (j + 1 < m && grid[i][j + 1] == '-') {
						grid[i][j + 1] = 'x';
					}
				}
			}
		}
	}
}

//verific ca doua becuri sa nu se lumineze reciproc adica caut o casuta cu bec
//apoi verific sa nu se mai gaseasca pe aceeasi linie sau coloana un alt bec
//pana gasesc o casuta care contine '#', cifra sau pana la margine
//si in caz contrar ies din program cu valoarea 0(fals)
int becuri_luminate_reciproc(char grid[51][51], int n, int m)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (grid[i][j] == 'L') {
				int k = j + 1;
				while (k < m) {
					if (grid[i][k] == '#' ||
						(grid[i][k] >= '0' && grid[i][k] <= '4')) {
						break;
					}
					if (grid[i][k] == 'L') {
						return 0;
					}
					k++;
				}
				k = j - 1;
				while (k >= 0) {
					if (grid[i][k] == '#' ||
						(grid[i][k] >= '0' && grid[i][k] <= '4')) {
						break;
					}
					if (grid[i][k] == 'L') {
						return 0;
					}
					k--;
				}
				k = i + 1;
				while (k < n) {
					if (grid[k][j] == '#' ||
						(grid[k][j] >= '0' && grid[k][j] <= '4')) {
						break;
					}
					if (grid[k][j] == 'L') {
						return 0;
					}
					k++;
				}
				k = i - 1;
				while (k >= 0) {
					if (grid[k][j] == '#' ||
						(grid[k][j] >= '0' && grid[k][j] <= '4')) {
						break;
					}
					if (grid[k][j] == 'L') {
						return 0;
					}
					k--;
				}
			}
		}
	}
	return 1;
}

//verific daca pot pune bec in casutele adiacente casutelor numerotate
//adica daca casutele nu sunt deja luminate de alt bec
int poate_pune_bec(char grid[][51], int n, int m, int i, int j)
{
	int k;
	k = i - 1;
	while (k >= 0) {
		if (grid[k][j] == '#' ||
			(grid[k][j] >= '0' && grid[k][j] <= '4')) {
			break;
			}
		if (grid[k][j] == 'L') {
			return 0;
		}
		k--;
	}
	k = i + 1;
	while (k < n) {
		if (grid[k][j] == '#' ||
			(grid[k][j] >= '0' && grid[k][j] <= '4')) {
			break;
			}
		if (grid[k][j] == 'L') {
			return 0;
		}
		k++;
	}
	k = j - 1;
	while (k >= 0) {
		if (grid[i][k] == '#' ||
			(grid[i][k] >= '0' && grid[i][k] <= '4')) {
			break;
			}
		if (grid[i][k] == 'L') {
			return 0;
		}
		k--;
	}
	k = j + 1;
	while (k < m) {
		if (grid[i][k] == '#' ||
			(grid[i][k] >= '0' && grid[i][k] <= '4')) {
			break;
			}
		if (grid[i][k] == 'L') {
			return 0;
		}
		k++;
	}

	return 1;
}

//pentru fiecare casuta numerotata, numar cate becuri sunt in casutele adiacente
//si daca sunt mai multe decat numarul din casuta ies din program cu valoarea 0
//daca sunt mai putine verific daca pot sa pun bec si numar cate pot sa pun
//daca nu pot sa pun atatea becuri cate ar mai trb (celulele deja luminate)
//returnez tot 0
int nr_corect_becuri_adiacente(char grid[][51], int n, int m)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (grid[i][j] >= '0' && grid[i][j] <= '4') {
				int nr_becuri = 0;
				int nr_posibile = 0;
				int r = grid[i][j] - '0';
				if (i - 1 >= 0) {
					if (grid[i - 1][j] == 'L') {
						nr_becuri++;
					} else if (grid[i - 1][j] == '-' &&
							(poate_pune_bec(grid, n, m, i - 1, j))) {
						nr_posibile++;
					}
				}
				if (i + 1 < n) {
					if (grid[i + 1][j] == 'L') {
						nr_becuri++;
					} else if (grid[i + 1][j] == '-' &&
								poate_pune_bec(grid, n, m, i + 1, j)) {
						nr_posibile++;
					}
				}
				if (j - 1 >= 0) {
					if (grid[i][j - 1] == 'L') {
						nr_becuri++;
					} else if (grid[i][j - 1] == '-' &&
								poate_pune_bec(grid, n, m, i, j - 1)) {
						nr_posibile++;
					}
				}
				if (j + 1 < m) {
					if (grid[i][j + 1] == 'L') {
						nr_becuri++;
					} else if (grid[i][j + 1] == '-' &&
								poate_pune_bec(grid, n, m, i, j + 1)) {
						nr_posibile++;
					}
				}

				if (nr_becuri > r || nr_becuri + nr_posibile < r) {
					return 0;
				}
			}
		}
	}
	return 1;
}

void afisare_grid(char grid[][51], int n, int m)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			printf("%c", grid[i][j]);
			if (j + 1 < m) {
				printf(" ");
			}
		}
		printf("\n");
	}
}

int main(void)
{
	int optiune;
	scanf("%d", &optiune);
	int n, m;
	scanf("%d %d", &n, &m);
	char grid[51][51];
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			scanf(" %c", &grid[i][j]);
		}
	}
	propagare_lumina(grid, n, m);
	if (optiune == 1) {
		celule_numerotate(grid, n, m);
		afisare_grid(grid, n, m);
	} else if (optiune == 2) {
		if (becuri_luminate_reciproc(grid, n, m) &&
			nr_corect_becuri_adiacente(grid, n, m)) {
			printf("ichi\n");
		} else {
			printf("zero\n");
		}
	}
	return 0;
}
