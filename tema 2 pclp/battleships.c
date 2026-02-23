#include <stdio.h>
#include "battleships.h"
#include <stdlib.h>

int main(void)
{
	int J;
	if (scanf("%d", &J) != 1) {
		return 0;
	}
	//alocare statistitici
	info_partida *stats_p1;
	stats_p1 = malloc(J * sizeof(info_partida));
	info_partida *stats_p2;
	stats_p2 = malloc(J * sizeof(info_partida));
	if (!stats_p1 || !stats_p2) {
		free(stats_p1);
		free(stats_p2);
		return 0;
	}
	for (int j = 0; j < J; j++) {
		int N, M;
		if (scanf("%d %d", &N, &M) != 2) {
			break;
		}
		int k = 2 * ((N * M) / 70 + (N * M) / 55 +
				(N * M) / 40 + (N * M) / 30 + (N * M) / 20);
		jucator p1, p2;
		//stats curente
		stats_p1[j].index_joc = j;
		stats_p1[j].nr_nave = k / 2;
		stats_p2[j].index_joc = j;
		stats_p2[j].nr_nave = k / 2;
		//initializare si alocare
		init_jucatori(&p1, &p2, N, M, k);
		//plasez navele
		etapa_plasare(&p1, &p2, N, M, k);
		//atacuri
		etapa_atac(&p1, &p2, N, M, &stats_p1[j], &stats_p2[j]);
		//eliberez memoriaa
		cleanup_joc(&p1, &p2, N);
	}
	//task 2
	proceseaza_comenzi_bonus(stats_p1, stats_p2, J);
	free(stats_p1);
	free(stats_p2);
	return 0;
}
