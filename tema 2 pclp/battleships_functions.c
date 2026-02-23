#include "battleships.h"
#include <math.h>
#include <string.h>
#include <stdlib.h>

void alloctabla(int N, int M, char ***addr_m)
{
	*addr_m = malloc(N * sizeof(char *));
	if (!*addr_m)
		return;

	for (int i = 0; i < N; i++) {
		(*addr_m)[i] = malloc(M * sizeof(char));
		if (!(*addr_m)[i]) {
			tablalibera(addr_m, N);
			return;
		}
		for (int j = 0; j < M; j++)
			(*addr_m)[i][j] = '0';
	}
}

void afisaretabla(char **tabla_proprie, int N, int M)
{
	for (int i = 0; i < N; i++) {
		for (int j = 0; j < M; j++)
			printf("%c ", tabla_proprie[i][j]);
		printf("\n");
	}
}

void tablalibera(char ***tabla_proprie, int N)
{
	if (!tabla_proprie || !*tabla_proprie) {
		return;
	}
	for (int i = 0; i < N; i++) {
		free((*tabla_proprie)[i]);
	}
	free(*tabla_proprie);
	*tabla_proprie = NULL;
}

int lungime_nava(char type)
{
	switch (type) {
	case 'S':
		return 5;
	case 'Y':
		return 4;
	case 'B':
		return 3;
	case 'L':
		return 2;
	case 'A':
		return 1;
	}
	return 0;
}

const char *nume_nava(char type)
{
	switch (type) {
	case 'S':
		return "Shinano";
	case 'Y':
		return "Yamato";
	case 'B':
		return "Belfast";
	case 'L':
		return "Laffey";
	case 'A':
		return "Albacore";
	default:
		return " ";
	}
}

int nava_valida(char **tabla_proprie, int N, int M, nava ship)
{
	int r = ship.x;
	int c = ship.y;
	for (int i = 0; i < ship.length; i++) {
		if (r < 0 || r >= N || c < 0 || c >= M)
			return 0;
		if (tabla_proprie[r][c] != '0')
			return 0;
		if (ship.orientare == 'H')
			c++;
		else
			r--;
	}
	return 1;
}

void plaseaza_nava(char **tabla_proprie, nava ship)
{
	int r = ship.x;
	int c = ship.y;
	char lungime_caracter = ship.length + '0';
	for (int i = 0; i < ship.length; i++) {
		tabla_proprie[r][c] = lungime_caracter;
		if (ship.orientare == 'H')
			c++;
		else
			r--;
	}
}

int coord_pe_nava(nava ship, int x, int y)
{
	if (ship.orientare == 'H')
		return (ship.x == x && y >= ship.y &&
				y <= ship.y + ship.length - 1);
	return (ship.y == y && x <= ship.x &&
			x >= ship.x - ship.length + 1);
}

int atacuri(jucator *adversar, int x, int y, int *nava_index)
{
	char celula_tinta = adversar->tabla_proprie[x][y];
	if (celula_tinta == '0')
		return 3;
	if (celula_tinta >= '1' && celula_tinta <= '5') {
		for (int i = 0; i < adversar->nr_nave; i++) {
			nava *ship = &adversar->nave[i];
			if (coord_pe_nava(*ship, x, y)) {
				*nava_index = i;
				if (ship->x == x && ship->y == y) {
					ship->vic = 1;
					return 2; // distrusa (lovitura in cap)
				}
				ship->hits++;
				if (ship->hits >= ship->length) {
					ship->vic = 1;
					return 2; //distrusa (total)
				}
				return 1; // lovit
			}
		}
	}
	return -1; // deja atacat
}

void coordonate_finale(nava ship, int *xf, int *yf)
{
	if (ship.orientare == 'H') {
		*xf = ship.x;
		*yf = ship.y + ship.length - 1;
	} else { // V
		*xf = ship.x - ship.length + 1;
		*yf = ship.y;
	}
}

void marcheaza_scufundata(jucator *adversar, nava ship, int N, int M)
{
	int r = ship.x;
	int c = ship.y;
	int L = ship.length;
	for (int i = 0; i < L; i++) {
		int curr_r = r;
		int curr_c = c;

		if (ship.orientare == 'H')
			curr_c = c + i;
		else
			curr_r = r - i;
		if (curr_r >= 0 && curr_r < N && curr_c >= 0 && curr_c < M)
			adversar->tabla_proprie[curr_r][curr_c] = '0';
	}
}

void init_jucatori(jucator *p1, jucator *p2, int N, int M, int k)
{
	alloctabla(N, M, &p1->tabla_proprie);
	alloctabla(N, M, &p2->tabla_proprie);
	alloctabla(N, M, &p1->tabla_atac);
	alloctabla(N, M, &p2->tabla_atac);

	p1->nave_distruse = 0;
	p2->nave_distruse = 0;
	p1->nr_nave = 0;
	p2->nr_nave = 0;
	p1->nave = malloc(k * sizeof(nava));
	p2->nave = malloc(k * sizeof(nava));
}

void etapa_plasare(jucator *p1, jucator *p2, int N, int M, int k)
{
	nava ship;
	int player_turn = 1;
	for (int i = 0; i < k;) {
		if (scanf(" %c %c %d %d", &ship.type, &ship.orientare,
				  &ship.x, &ship.y) != 4) {
			break;
		}
		ship.length = lungime_nava(ship.type);
		ship.x--;
		ship.y--;
		ship.hits = 0;
		ship.vic = 0;
		if (player_turn == 1) {
			if (nava_valida(p1->tabla_proprie, N, M, ship)) {
				plaseaza_nava(p1->tabla_proprie, ship);
				p1->nave[p1->nr_nave++] = ship;
				player_turn = 2;
				i++;
			} else {
				printf("Nava %s nu poate fi amplasata %s la coordonatele "
					   "(%d, %d).\n", nume_nava(ship.type),
						(ship.orientare == 'V' ? "vertical" : "orizontal"),
						ship.x + 1, ship.y + 1);
			}
		} else {
			if (nava_valida(p2->tabla_proprie, N, M, ship)) {
				plaseaza_nava(p2->tabla_proprie, ship);
				p2->nave[p2->nr_nave++] = ship;
				player_turn = 1;
				i++;
			} else {
				printf("Nava %s nu poate fi amplasata %s la coordonatele "
					   "(%d, %d).\n", nume_nava(ship.type),
						(ship.orientare == 'V' ? "vertical" : "orizontal"),
						ship.x + 1, ship.y + 1);
			}
		}
	}
	afisaretabla(p1->tabla_proprie, N, M);
	printf("\n");
	afisaretabla(p2->tabla_proprie, N, M);
}

void etapa_atac(jucator *p1, jucator *p2, int N, int M,
				info_partida *stat1, info_partida *stat2)
{
	jucator *atacator, *adversar;
	stat1->lovituri = 0;
	stat1->total_atacuri = 0;
	stat2->lovituri = 0;
	stat2->total_atacuri = 0;
	int turn = 1;
	while (p1->nave_distruse < p1->nr_nave &&
		   p2->nave_distruse < p2->nr_nave) {
		atacator = (turn == 1) ? p1 : p2;
		adversar = (turn == 1) ? p2 : p1;
		info_partida *curr_stat = (turn == 1) ? stat1 : stat2;
		int x, y;
		if (scanf(" %d %d", &x, &y) != 2) {
			break;
		}
		int valid_input = (x >= 1 && x <= N && y >= 1 && y <= M);
		if (valid_input) {
			curr_stat->total_atacuri++;
		}
		x--;
		y--;
		int nava_index = -1;
		if (!valid_input) {
			printf("Jucatorul %d a ratat o lovitura la coordonatele "
				  "(%d, %d).\n", turn, x + 1, y + 1);
			turn = 3 - turn;
			continue;
		}
		if (atacator->tabla_atac[x][y] == 'R' ||
			atacator->tabla_atac[x][y] == 'H' ||
			atacator->tabla_atac[x][y] == 'X') {
			printf("Coordonatele (%d, %d) au fost deja atacate de jucatorul "
				   "%d.\n", x + 1, y + 1, turn);
			turn = 3 - turn;
		} else {
			int rez = atacuri(adversar, x, y, &nava_index);
			if (rez == 3) { // ratare
				atacator->tabla_atac[x][y] = 'R';
				printf("Jucatorul %d a ratat o lovitura la coordonatele "
					   "(%d, %d).\n", turn, x + 1, y + 1);
				turn = 3 - turn;
			} else if (rez == 1) { // lovit
				curr_stat->lovituri++; // lovitura reusita
				atacator->tabla_atac[x][y] = 'H';
				adversar->tabla_proprie[x][y] = 6;
				printf("Jucatorul %d a lovit o nava %s la coordonatele "
					  "(%d, %d).\n", turn,
						nume_nava(adversar->nave[nava_index].type),
						x + 1, y + 1);
				turn = 3 - turn;
			} else if (rez == -1) {
				printf("Coordonatele (%d, %d) au fost deja atacate de "
					   "jucatorul %d.\n", turn, x + 1, y + 1);
				turn = 3 - turn;
			} else if (rez == 2) {
				curr_stat->lovituri++;
				atacator->tabla_atac[x][y] = 'X';
				adversar->nave_distruse++;
				marcheaza_scufundata(adversar,
									 adversar->nave[nava_index],
									N, M);
				int xf, yf;
				coordonate_finale(adversar->nave[nava_index], &xf, &yf);
				printf("Jucatorul %d a distrus o nava %s plasata intre "
					   "coordonatele (%d, %d) si (%d, %d).\n", turn,
						nume_nava(adversar->nave[nava_index].type),
						adversar->nave[nava_index].x + 1,
						adversar->nave[nava_index].y + 1,
						xf + 1, yf + 1);
				if (adversar->nave_distruse == adversar->nr_nave) {
					printf("Jucatorul %d a castigat.\n", turn);
					break;
				}
				turn = 3 - turn;
			}
		}
	}
}

void cleanup_joc(jucator *p1, jucator *p2, int N)
{
	tablalibera(&p1->tabla_proprie, N);
	tablalibera(&p2->tabla_proprie, N);
	tablalibera(&p1->tabla_atac, N);
	tablalibera(&p2->tabla_atac, N);
	free(p1->nave);
	free(p2->nave);
}

double medie_totala(info_partida *stats, int n)
{
	double numarator = 0.0;
	double numitor = 0.0;
	for (int i = 0; i < n; i++) {
		double acuratete_locala = 0;
		if (stats[i].total_atacuri > 0)
			acuratete_locala = (double)stats[i].lovituri /
						stats[i].total_atacuri;

		numarator += acuratete_locala * stats[i].nr_nave;
		numitor += stats[i].nr_nave;
	}
	if (numitor == 0)
		return 0.0;
	return (numarator / numitor) * 100.0;
}

//CMP MIN - sortez descrescator dupa castigul potential
int cmp_crestere_medie(const void *a, const void *b)
{
	info_partida *s1 = (info_partida *)a;
	info_partida *s2 = (info_partida *)b;
	long long gain1 = (long long)(s1->total_atacuri - s1->lovituri) *
				s1->nr_nave * s2->total_atacuri;
	long long gain2 = (long long)(s2->total_atacuri - s2->lovituri) *
				s2->nr_nave * s1->total_atacuri;
	if (gain1 > gain2)
		return -1;
	if (gain1 < gain2)
		return 1;
	return (s1->index_joc < s2->index_joc) ? -1 : 1;
}

// CMP MAX - sortez Crescator dupa impact
// Vreau sa eliminam jocurile care scad media cel mai putin.
int cmp_scadere_medie(const void *a, const void *b)
{
	info_partida *s1 = (info_partida *)a;
	info_partida *s2 = (info_partida *)b;
	long long loss1 = (long long)s1->lovituri * s1->nr_nave *
			  s2->total_atacuri;
	long long loss2 = (long long)s2->lovituri * s2->nr_nave *
			  s1->total_atacuri;
	if (loss1 < loss2)
		return -1;
	if (loss1 > loss2)
		return 1;
	return (s1->index_joc < s2->index_joc) ? -1 : 1;
}

void afiseaza_acuratete_joc(info_partida p, double target, int is_min)
{
	double acc = 0.0;
	if (p.total_atacuri > 0) {
		acc = (double)p.lovituri / p.total_atacuri * 100.0;
	}
	printf("%03d.%02d", (int)acc, (int)((acc - (int)acc) * 100));
	int necesar;
	int prag_lovituri = (int)ceil((double)p.total_atacuri * target / 100.0);
	if (is_min) {
		necesar = prag_lovituri - p.lovituri;
		if (necesar < 0)
			necesar = 0;
		if (necesar > (p.total_atacuri - p.lovituri))
			necesar = p.total_atacuri - p.lovituri;
	} else {
		necesar = p.lovituri - prag_lovituri;
		if (necesar < 0)
			necesar = 0;
	}
	printf(".%d\n", necesar);
}

void proceseaza_comenzi_bonus(info_partida *stats1, info_partida *stats2,
							  int numar_jocuri)
{
	char trash_key[100];
	char cmd[100];
	double epsilon = 0.000000001;
	scanf("%s", trash_key);
	while (scanf("%s", cmd) == 1) {
		if (cmd[0] == 'Q') {
			break;
		}
		int id_jucator = (cmd[0] == '0' || cmd[0] == 'O') ? 0 : 1;
		info_partida *target_stats = (id_jucator == 0) ? stats1 : stats2;
		char tip_comanda = cmd[1];
		char buffer_tinta[7];
		strncpy(buffer_tinta, cmd + 5, 6);
		buffer_tinta[6] = '\0';
		double tinta = atof(buffer_tinta);
		int is_min = (strstr(cmd, "MIN") != 0);
		if (tip_comanda == 'U') {
			char buffer_k[4];
			buffer_k[0] = cmd[2];
			buffer_k[1] = cmd[3];
			buffer_k[2] = cmd[4];
			buffer_k[3] = '\0';
			int game_idx = atoi(buffer_k) - 1;
			if (game_idx >= 0 && game_idx < numar_jocuri) {
				afiseaza_acuratete_joc(target_stats[game_idx],
									   tinta, is_min);
			} else {
				printf("Nu exista date pentru meciul cerut.\n");
			}
		} else {
			double current_avg = medie_totala(target_stats,
								  numar_jocuri);
			printf("%03d.%02d", (int)current_avg,
				   (int)((current_avg - (int)current_avg) * 100));

			info_partida *temp = malloc(numar_jocuri *
						   sizeof(info_partida));
			for (int i = 0; i < numar_jocuri; i++)
				temp[i] = target_stats[i];
			int modificari = 0;
			if (is_min) {
				qsort(temp, numar_jocuri, sizeof(info_partida),
					  cmp_crestere_medie);
				for (int i = 0; i < numar_jocuri; i++) {
					if (medie_totala(temp,
									 numar_jocuri) >=
						tinta - epsilon)
						break;
					temp[i].lovituri =
						temp[i].total_atacuri;
					modificari++;
				}
			} else {
				qsort(temp, numar_jocuri, sizeof(info_partida),
					  cmp_scadere_medie);
				for (int i = 0; i < numar_jocuri; i++) {
					int lovituri_vechi = temp[i].lovituri;
					temp[i].lovituri = 0;
					if (medie_totala(temp,
									 numar_jocuri) <
						tinta - epsilon) {
						temp[i].lovituri =
							lovituri_vechi;
					} else {
						modificari++;
					}
				}
			}
			printf(".%d\n", modificari);
			free(temp);
		}
	}
}
