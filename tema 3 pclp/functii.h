#ifndef FUNCTII_H
#define FUNCTII_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>
#define PI 3.14159265358979323846

typedef struct {
	char *axiom;
	char **rules;
	int loaded;
	char *filename;
} lsystem;

typedef struct {
	unsigned char r;//culorile
	unsigned char g;
	unsigned char b;
} pixel;

typedef struct {
	int w;//lățime
	int h;//înălțime
	pixel **data;//matricea de pixeli
	pixel *block;//sir de pixeli
} imag;

typedef struct {
	double x;//poziția curentă X
	double y;//poziția curentă Y
	double dir;//orientare
	double pas;//lungimea pasului
	double turn;//unghiul de rotație
} turtle;

typedef struct {
	double x;
	double y;
	double dir;
} state;//pastrez pozitia si directia testoasei

typedef struct {
	state *v;//pastrez istoricul pozitiilor
	int top;//vf. stivei
	int size;//capacitatea stivei
} stack;

typedef struct {
	lsystem sys;//copia L-system
	int turtle_ran;//1 daca a mai fost comanda 0 daca nu
	turtle t;
	int iter;//numărul de derivări
	unsigned char r, g, b;
	int type_ran;//1 daca a mai fost comanda TYPE 0 daca nu
	char *text;
	int tx, ty;//coordonatele textului
	int font_loaded;//1 daca a mai fost comanda font 0 daca nu
	char *font_file;//numele fisierului font
	char *cached_deriv;//salvez rezultatul derivarii
	pixel *img_state;//copia imaginii
	int state_w, state_h;//dimensiunile la momentul salvarii
	int load_ran;//1 dacă a mai fost comanda LOAD 0 daca nu
	char *load_file;//numele fișierului încărcat
} action;

typedef struct _glyph {
	int code;//codul ASCII
	int w, h;//latime, inaltime (din BBX)
	int x_off, y_off;//offset fata de cursor (din BBX)
	int dx, dy;//cat avansam cursorul dupa scriere (din DWIDTH)
	unsigned char **mat;//matricea de pixeli
} glyph;

typedef struct {
	char *name;//numele fontului
	struct _glyph *chars[256];
	int loaded;
	char *file;
} font;

typedef struct {
	imag img;
	lsystem sys;
	font fontp;
	action *history;
	int history_size;
	int current;
	char last_bg_file[256];//numele fisierului pe care desenez
	int running;
} game;

void str_newline(char *str);
int load_lsystem(const char *filename, lsystem *sys);
char *derive(const lsystem *sys, int n);
void cleanup(lsystem *sys);
lsystem copy_lsystem(const lsystem *source);
imag copy_img(const imag *source);
void draw_pixel(imag *img, int x, int y, unsigned char r,
				unsigned char g, unsigned char b);
void draw_line(imag *img, int x0, int y0, int x1, int y1,
			   unsigned char r, unsigned char g, unsigned char b);
void free_image(imag *img);
void skip_comments(FILE *fp);
int alloc_mem(imag *img);
int read_p3(FILE *f, imag *img);
imag load_image(const char *filename, int *succes, int silent);
void save_image(const imag *img, const char *filename);
void interpret_turtle(imag *img, const char *deriv, turtle t, unsigned char r,
					  unsigned char g, unsigned char b, int silent);
void free_font(font *f);
void hex_to_bin(const char *hex, unsigned char *row, int width);
void free_glyph(glyph *g);
void p_bbx(char *buf, glyph *g);
void init_font(font *f, const char *filename);
int load_font(const char *filename, font *f, int silent);
void draw_glyph(imag *img, glyph *g, int x, int y, unsigned char r,
				unsigned char col_g, unsigned char b);
void draw_text(imag *img, font *f, const char *txt, int x, int y,
			   unsigned char r, unsigned char g, unsigned char b);
void replay_history(imag *img, action *history, int current,
					const char *bg_filename, font *fontp);
pixel *create_img_state(imag *img);
int get_bit(const imag *img, long bit_idx);
void bitcheck(const imag *img);
void init_game(game *gs);
void close_game(game *gs);
void do_load(game *gs, const char *command);
void do_font(game *gs, const char *command);
void do_type(game *gs, const char *command);
void do_lsystem(game *gs, const char *command);
void do_undo(game *gs);
void do_redo(game *gs);
void do_derive(game *gs, const char *command);
void do_turtle(game *gs, const char *command);
void do_save(game *g, const char *command);
void free_action(action *a);

#endif
