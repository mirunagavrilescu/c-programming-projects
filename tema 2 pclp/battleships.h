#ifndef BATTLESHIPS_H
#define BATTLESHIPS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Structura pentru o nava
typedef struct {
	char type;
	int length;
	int x, y;
	char orientare;
	int hits;
	int vic;
} nava;

// Structura pentru statistici
typedef struct {
	int lovituri;
	int total_atacuri;
	int nr_nave;//Folosit ca pondere
	int index_joc;// Pentru a pastra ordinea initiala dupa sortare
} info_partida;

typedef struct {
	char **tabla_proprie;
	char **tabla_atac;
	nava *nave;
	int p1, p2;
	int nr_nave;
	int nave_distruse;
} jucator;

// Functii pentru table
void alloctabla(int N, int M, char ***addr_m);
void afisaretabla(char **tabla_proprie, int N, int M);
void tablalibera(char ***tabla_proprie, int N);

// Functii pt nave
int lungime_nava(char type);
const char *nume_nava(char type);
int nava_valida(char **tabla_proprie, int N, int M, nava ship);
void plaseaza_nava(char **tabla_proprie, nava ship);
int coord_pe_nava(nava ship, int x, int y);
int atacuri(jucator *atacat, int x, int y, int *nava_index);
void coordonate_finale(nava ship, int *xf, int *yf);
void marcheaza_scufundata(jucator *adversar, nava ship, int N, int M);

// organizare joc
void init_jucatori(jucator *p1, jucator *p2, int N, int M, int k);
void etapa_plasare(jucator *p1, jucator *p2, int N, int M, int k);
void etapa_atac(jucator *p1, jucator *p2, int N, int M,
				info_partida *stat1, info_partida *stat2);
void cleanup_joc(jucator *p1, jucator *p2, int N);

//task 2
void proceseaza_comenzi_bonus(info_partida *stats1, info_partida *stats2,
							  int numar_jocuri);

#endif
