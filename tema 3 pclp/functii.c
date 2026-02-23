#include "functii.h"

void free_action(action *a)
{
	//curata memoria
	cleanup(&a->sys);
	if (a->cached_deriv) {
		free(a->cached_deriv);
		a->cached_deriv = NULL;
	}
	if (a->text) {
		free(a->text);
		a->text = NULL;
	}
	if (a->font_file) {
		free(a->font_file);
		a->font_file = NULL;
	}
	if (a->img_state) {
		free(a->img_state);
		a->img_state = NULL;
	}
	if (a->load_file) {
		free(a->load_file);
		a->load_file = NULL;
	}
}

void str_newline(char *str)
{
	//elimina enter-ul de la finalul sirului citit
	if (!str) {
		return;
	}
	size_t len = strlen(str);
	while (len > 0 && (str[len - 1] == '\n' || str[len - 1] == '\r')) {
		str[len - 1] = '\0';
		len--;
	}
}

int load_lsystem(const char *filename, lsystem *sys)
{
	//Citeste axioma si regulile L-System-ului dintr-un fisier text
	FILE *f = fopen(filename, "r");
	if (!f) {
		return 0;
	}
	char buffer[1024];
	if (!fgets(buffer, sizeof(buffer), f)) {
		fclose(f);
		return 0;
	}
	size_t len = strlen(buffer);
	while (len > 0 && (buffer[len - 1] == '\n' || buffer[len - 1] == '\r')) {
		buffer[len - 1] = '\0';
		len--;
	}
	char *axiom = malloc(strlen(buffer) + 1);
	strcpy(axiom, buffer);
	int nrules;
	if (fscanf(f, "%d\n", &nrules) != 1 || nrules < 0) {
		free(axiom);
		fclose(f);
		return 0;
	}
	char **rules = calloc(256, sizeof(char *));
	for (int i = 0; i < nrules; i++) {
		char symbol;
		char succesor[1024];
		if (fscanf(f, "%c %s\n", &symbol, succesor) != 2) {
			free(axiom);
			fclose(f);
			return 0;
		}
		rules[(unsigned char)symbol] = malloc(strlen(succesor) + 1);
		strcpy(rules[(unsigned char)symbol], succesor);
	}
	fclose(f);
	cleanup(sys);
	sys->axiom = axiom;
	sys->rules = rules;
	sys->loaded = 1;
	sys->filename = malloc(strlen(filename) + 1);
	strcpy(sys->filename, filename);
	return 1;
}

char *derive(const lsystem *sys, int n)
{
	//Genereaza sirul final aplicand regulile de N ori
	if (!sys->loaded) {
		return NULL;
	}
	char *current = malloc(strlen(sys->axiom) + 1);
	strcpy(current, sys->axiom);
	for (int step = 0; step < n; step++) {
		int new_len = 0;
		for (int i = 0; current[i] != '\0'; i++) {
			unsigned char c = current[i];
			if (sys->rules[c]) {
				new_len += strlen(sys->rules[c]);
			} else {
				new_len += 1;
			}
		}
		char *next = malloc(new_len + 1);
		int pos = 0;
		for (int i = 0; current[i] != '\0'; i++) {
			unsigned char c = current[i];
			if (sys->rules[c]) {
				char *rep = sys->rules[c];
				for (int j = 0; rep[j] != '\0'; j++) {
					next[pos++] = rep[j];
				}
			} else {
				next[pos++] = c;
			}
		}
		next[pos] = '\0';
		free(current);
		current = next;
	}
	return current;
}

void cleanup(lsystem *sys)
{
	//Elibereaza memoria alocata pentru regulile L-System-ului
	if (!sys->loaded) {
		return;
	}
	free(sys->axiom);
	for (int i = 0; i < 256; i++) {
		if (sys->rules[i]) {
			free(sys->rules[i]);
		}
	}
	free(sys->rules);
	sys->axiom = NULL;
	sys->rules = NULL;
	sys->loaded = 0;
	free(sys->filename);
	sys->filename = NULL;
}

lsystem copy_lsystem(const lsystem *source)
{
	//Face o copie a L-System-ului curent
	lsystem dest;
	dest.loaded = source->loaded;

	if (!source->loaded) {
		dest.axiom = NULL;
		dest.rules = NULL;
		dest.filename = NULL;
		dest.loaded = 0;
		return dest;
	}
	dest.axiom = malloc(strlen(source->axiom) + 1);
	strcpy(dest.axiom, source->axiom);
	if (source->filename) {
		dest.filename = malloc(strlen(source->filename) + 1);
		strcpy(dest.filename, source->filename);
	} else {
		dest.filename = NULL;
	}
	dest.rules = calloc(256, sizeof(char *));
	for (int i = 0; i < 256; i++) {
		if (source->rules[i]) {
			dest.rules[i] = malloc(strlen(source->rules[i]) + 1);
			strcpy(dest.rules[i], source->rules[i]);
		}
	}
	return dest;
}

imag copy_img(const imag *source)
{
	imag dest;
	dest.w = source->w;
	dest.h = source->h;
	dest.data = malloc(dest.h * sizeof(pixel *));
	for (int y = 0; y < dest.h; y++) {
		dest.data[y] = malloc(dest.w * sizeof(pixel));
		for (int x = 0; x < dest.w; x++) {
			dest.data[y][x] = source->data[y][x];
		}
	}
	return dest;
}

void draw_pixel(imag *img, int x, int y,
				unsigned char r, unsigned char g, unsigned char b)
{
	//Pune un pixel de o anumita culoare la coordonatele date
	if (!img || !img->data) {
		return;
	}
	if (x < 0 || x >= img->w || y < 0 || y >= img->h) {
		return;
	}
	img->data[y][x].r = r;
	img->data[y][x].g = g;
	img->data[y][x].b = b;
}

void draw_line(imag *img, int x0, int y0, int x1, int y1,
			   unsigned char r, unsigned char g, unsigned char b)
{
	//Deseneaza o linie intre doua puncte folosind algoritmul lui Bresenham
	int dx = abs(x1 - x0);
	int sx;
	if (x0 < x1) {
		sx = 1;
	} else {
		sx = -1;
	}
	int dy = -abs(y1 - y0);
	int sy;
	if (y0 < y1) {
		sy = 1;
	} else {
		sy = -1;
	}
	int err = dx + dy;
	while (1) {
		draw_pixel(img, x0, y0, r, g, b);
		if (x0 == x1 && y0 == y1) {
			break;
		}
		int e2 = 2 * err;
		if (e2 >= dy) {
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void free_image(imag *img)
{
	//Elibereaza memoria ocupata de pixelii imaginii
	if (!img || !img->data) {
		return;
	}
	free(img->data);
	if (img->block) {
		free(img->block);
	}
	img->data = NULL;
	img->block = NULL;
	img->w = 0;
	img->h = 0;
}

void skip_comments(FILE *fp)
{
	//Sare peste comentariile si spatiile goale din fisierele PPM
	int ch;
	char line[1024];
	while (1) {
		ch = fgetc(fp);
		if (ch == EOF) {
			break;
		}
		if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r') {
			continue;
		} else if (ch == '#') {
			fgets(line, sizeof(line), fp);
			continue;
		} else {
			ungetc(ch, fp);
			break;
		}
	}
}

int alloc_mem(imag *img)
{
	int y;
	img->block = malloc(img->w * img->h * sizeof(pixel));
	img->data = malloc(img->h * sizeof(pixel *));
	if (!img->block || !img->data) {
		if (img->block) {
			free(img->block);
		}
		if (img->data) {
			free(img->data);
		}
		img->block = NULL;
		img->data = NULL;
		return 0;
	}
	for (y = 0; y < img->h; y++) {
		img->data[y] = img->block + (y * img->w);
	}
	return 1;
}

int read_p3(FILE *f, imag *img)
{
	int i, r, g, b, res;
	for (i = 0; i < img->w * img->h; i++) {
		res = fscanf(f, "%d %d %d", &r, &g, &b);
		if (res != 3) {
			return 0;
		}
		img->block[i].r = (unsigned char)r;
		img->block[i].g = (unsigned char)g;
		img->block[i].b = (unsigned char)b;
	}
	return 1;
}

imag load_image(const char *filename, int *succes, int silent)
{
	//Incarca o imagine PPM (P3 sau P6) din fisier in memorie
	FILE *f = fopen(filename, "rb");
	imag img = {0, 0, NULL, NULL};
	int ok = 1;
	if (!f) {
		*succes = 0;
		if (!silent) {
			printf("Failed to load %s\n", filename);
		}
		return img;
	}
	char magic[3];
	fscanf(f, "%2s", magic);
	skip_comments(f);
	int w, h, maxval;
	fscanf(f, "%d %d", &w, &h);
	skip_comments(f);
	fscanf(f, "%d", &maxval);
	fgetc(f);
	img.w = w;
	img.h = h;
	if (!alloc_mem(&img)) {
		ok = 0;
	} else if (strcmp(magic, "P6") == 0) {
		if (fread(img.block, sizeof(pixel), w * h, f) != (size_t)(w * h)) {
			ok = 0;
		}
	} else if (strcmp(magic, "P3") == 0) {
		if (!read_p3(f, &img)) {
			ok = 0;
		}
	} else {
		ok = 0;
	}
	fclose(f);
	if (!ok) {
		*succes = 0;
		if (img.data) {
			free(img.data);
		}
		if (img.block) {
			free(img.block);
		}
		img.data = NULL;
		img.block = NULL;
		if (!silent) {
			printf("Failed to load %s\n", filename);
		}
	} else {
		*succes = 1;
		if (!silent) {
			printf("Loaded %s (PPM image %dx%d)\n", filename, w, h);
		}
	}
	return img;
}

void save_image(const imag *img, const char *filename)
{
	//Salveaza imaginea curenta intr-un fisier format PPM (P6)
	if (!img || !img->data) {
		printf("No image loaded\n");
		return;
	}
	FILE *f = fopen(filename, "wb");
	if (!f) {
		return;
	}
	fprintf(f, "P6\n%d %d\n255\n", img->w, img->h);
		for (int y = 0; y < img->h; y++) {
			for (int x = 0; x < img->w; x++) {
				fputc(img->data[y][x].r, f);
				fputc(img->data[y][x].g, f);
				fputc(img->data[y][x].b, f);
			}
		}
	fclose(f);
	printf("Saved %s\n", filename);
}

void interpret_turtle(imag *img, const char *deriv, turtle t,
					  unsigned char r, unsigned char g,
					  unsigned char b, int silent)
{
	//Executa comenzile testoasei pentru a desena liniile pe imagine
	stack S;
	S.size = 1024;
	S.v = malloc(S.size * sizeof(state));
	S.top = -1;
	for (int i = 0; deriv[i]; i++) {
		char c = deriv[i];
		if (c == 'F') {
			double x1 = t.x + t.pas * cos(t.dir * PI / 180.0);
			double y1 = t.y + t.pas * sin(t.dir * PI / 180.0);
			draw_line(img, (int)round(t.x),
					  img->h - 1 - (int)round(t.y),
					  (int)round(x1),
					  img->h - 1 - (int)round(y1),
					  r, g, b);
			t.x = x1;
			t.y = y1;
		} else if (c == '+') {
			t.dir += t.turn;
			t.dir = fmod(t.dir, 360.0);
			if (t.dir < 0) {
				t.dir += 360.0;
			}
		} else if (c == '-') {
			t.dir -= t.turn;
			t.dir = fmod(t.dir, 360.0);
			if (t.dir < 0) {
				t.dir += 360.0;
			}
		} else if (c == '[') {
			if (S.top + 1 >= S.size) {//verific daca stiva e plina
				S.size *= 2;
				state *temp = realloc(S.v, S.size * sizeof(state));
				if (!temp) {
					printf("Memory error\n");
					free(S.v);
					return;
				}
				S.v = temp;
			}
				S.top++;
				S.v[S.top].x = t.x;
				S.v[S.top].y = t.y;
				S.v[S.top].dir = t.dir;
		} else if (c == ']') {
			if (S.top >= 0) {
				t.x = S.v[S.top].x;
				t.y = S.v[S.top].y;
				t.dir = S.v[S.top].dir;
				S.top--;
			}
		}
	}
	free(S.v);
	if (!silent) {
		printf("Drawing done\n");
	}
}

void free_font(font *f)
{
	//Elibereaza memoria alocata pentru caracterele fontului incarcat
	if (!f->loaded) {
		return;
	}
	if (f->name) {
		free(f->name);
		f->name = NULL;
	}
	if (f->file) {
		free(f->file);
		f->file = NULL;
	}
	for (int i = 0; i < 256; i++) {
		if (f->chars[i]) {
			for (int j = 0; j < f->chars[i]->h; j++) {
				free(f->chars[i]->mat[j]);
			}
			free(f->chars[i]->mat);
			free(f->chars[i]);
			f->chars[i] = NULL;
		}
	}
	f->loaded = 0;
}

void hex_to_bin(const char *hex, unsigned char *row, int width)
{
	//Converteste un sir hexazecimal in biti pt citirea bitmap-ului
	int idx = 0;
	for (int i = 0; hex[i] && hex[i] != '\n' && idx < width; i++) {
		int val = 0;
		char c = hex[i];
		if (c >= '0' && c <= '9') {
			val = c - '0';
		} else if (c >= 'A' && c <= 'F') {
			val = c - 'A' + 10;
		} else if (c >= 'a' && c <= 'f') {
			val = c - 'a' + 10;
		}
		for (int k = 3; k >= 0; k--) {
			if (idx < width) {
				row[idx] = (val >> k) & 1;
				idx++;
			}
		}
	}
}

void free_glyph(glyph *g)
{
	//elibereaza memoria
	int k;
	if (!g) {
		return;
	}
	if (g->mat) {
		for (k = 0; k < g->h; k++) {
			free(g->mat[k]);
		}
		free(g->mat);
	}
	free(g);
}

void p_bbx(char *buf, glyph *g)
{
	int k;
	if (!g) {
		return;
	}
	sscanf(buf + 4, "%d %d %d %d", &g->w, &g->h, &g->x_off, &g->y_off);
	g->mat = malloc(g->h * sizeof(unsigned char *));
	for (k = 0; k < g->h; k++) {
		g->mat[k] = calloc(g->w, sizeof(unsigned char));
	}
}

void init_font(font *f, const char *filename)
{
	//initializeaza structura de font la inceput
	int i;
	free_font(f);
	f->loaded = 1;
	f->file = malloc(strlen(filename) + 1);
	strcpy(f->file, filename);
	f->name = NULL;
	for (i = 0; i < 256; i++) {
		f->chars[i] = NULL;
	}
}

int load_font(const char *filename, font *f, int silent)
{
	//Citeste si parseaza un fisier de font
	FILE *fin = fopen(filename, "r");
	if (!fin) {
		if (!silent) {
			printf("Failed to load %s\n", filename);
		}
		return 0;
	}
	char buf[100000];
	glyph *g = NULL;
	int in_bitmap = 0;
	int row = 0;
	int r;
	init_font(f, filename);
	while (fgets(buf, sizeof(buf), fin)) {
		if (buf[strlen(buf) - 1] == '\n') {
			buf[strlen(buf) - 1] = 0;
		}
		if (strncmp(buf, "FONT ", 5) == 0) {
			f->name = malloc(strlen(buf + 5) + 1);
			strcpy(f->name, buf + 5);
		} else if (strncmp(buf, "STARTCHAR", 9) == 0) {
			g = calloc(1, sizeof(glyph));
		} else if (strncmp(buf, "ENCODING", 8) == 0) {
			if (g) {
				r = sscanf(buf + 8, "%d", &g->code);
			}
		} else if (strncmp(buf, "DWIDTH", 6) == 0) {
			if (g) {
				r = sscanf(buf + 6, "%d %d", &g->dx, &g->dy);
				(void)r;
			}

		} else if (strncmp(buf, "BBX", 3) == 0) {
			p_bbx(buf, g);
		} else if (strncmp(buf, "BITMAP", 6) == 0) {
			in_bitmap = 1;
			row = 0;
		} else if (strncmp(buf, "ENDCHAR", 7) == 0) {
			if (g && g->code >= 0 && g->code < 256) {
				f->chars[g->code] = g;
			} else {
				free_glyph(g);
			}
			g = NULL;
			in_bitmap = 0;
		} else if (in_bitmap && g) {
			if (row < g->h) {
				hex_to_bin(buf, g->mat[row], g->w);
				row++;
			}
		}
	}
	free_glyph(g);
	fclose(fin);
	if (!silent) {
		printf("Loaded %s (bitmap font %s)\n", filename, f->name);
	}
	return 1;
}

void draw_glyph(imag *img, glyph *g, int x, int y, unsigned char r,
				unsigned char col_g, unsigned char b)
{
	//Deseneaza un singur caracter pe imagine la pozitia data
	if (!g) {
		return;
	}
	int sy = y - g->y_off - g->h + 1;
	int sx = x + g->x_off;
	for (int i = 0; i < g->h; i++) {
		for (int j = 0; j < g->w; j++) {
			if (g->mat[i][j] == 1) {
				// Verific limite imagine
				if (sy + i >= 0 && sy + i < img->h && sx + j >= 0 &&
					sx + j < img->w) {
					draw_pixel(img, sx + j, sy + i, r, col_g, b);
				}
			}
		}
	}
}

void draw_text(imag *img, font *f, const char *txt, int x, int y,
			   unsigned char r, unsigned char g, unsigned char b)
{
	//Scrie un sir de caractere pe imagine folosind fontul incarcat
	int cx, cy;
	cx = x;
	cy = y;
	for (int i = 0; txt[i] != '\0'; i++) {
		int cod = (unsigned char)txt[i];
		glyph *gl = f->chars[cod];
		if (gl) {
			draw_glyph(img, gl, cx, cy, r, g, b);
			cx += gl->dx;
			cy += gl->dy;
		}
	}
}

void replay_history(imag *img, action *history, int current,
					const char *bg_filename, font *fontp)
{
	int ok;
	imag new_img = load_image(bg_filename, &ok, 1);
	if (ok) {
		free_image(img);
		*img = new_img;
	} else {
		return;
	}
	for (int i = 0; i <= current; i++) {
		if (history[i].turtle_ran) {
			if (history[i].cached_deriv) {
				interpret_turtle(img,
								 history[i].cached_deriv,
								 history[i].t,
								 history[i].r, history[i].g, history[i].b,
								 1);
			}
		}
		if (history[i].font_loaded) {
			load_font(history[i].font_file, fontp, 1);
		}
		if (history[i].type_ran) {
			draw_text(img, fontp, history[i].text,
					  history[i].tx, history[i].ty, history[i].r,
					  history[i].g, history[i].b);
		}
	}
}

pixel *create_img_state(imag *img)
{
	//Creeaza o copie a imaginii curente pentru Undo
	if (!img || !img->block) {
		return NULL;
	}
	size_t size = img->w * img->h * sizeof(pixel);
	pixel *state = malloc(size);
	memcpy(state, img->block, size);
	return state;
}

int get_bit(const imag *img, long bit_idx)
{
	//Extrage valoarea unui anumit bit
	unsigned char *raw_data = (unsigned char *)img->block;
	long byte_idx = bit_idx / 8;//al câtelea octet din stream
	int bit_offset = 7 - (bit_idx % 8);//al câtelea bit
	unsigned char masca = (1 << bit_offset);
	unsigned char rezultat_mascat = raw_data[byte_idx] & masca;
	if (rezultat_mascat != 0) {
		return 1;
	} else {
		return 0;
	}
}

void bitcheck(const imag *img)
{
	//Verifica daca exista anumite tipare de biti ascunse in imagine
	if (!img || !img->data) {
		printf("No image loaded\n");
		return;
	}
	long total_bits = (long)img->w * img->h * 3 * 8;
	for (long i = 0; i <= total_bits - 4; i++) {
		//citesc cei 4 biti curenti
		int b0 = get_bit(img, i);
		int b1 = get_bit(img, i + 1);
		int b2 = get_bit(img, i + 2);
		int b3 = get_bit(img, i + 3);
		int pattern1 = (b0 == 0 && b1 == 0 && b2 == 1 && b3 == 0);
		int pattern2 = (b0 == 1 && b1 == 1 && b2 == 0 && b3 == 1);
		if (pattern1 || pattern2) {
			//bitul care se strică este al treilea din secvență
			long target_bit_idx = i + 2;
			//aflu pixelul și coordonatele
			long byte_idx = target_bit_idx / 8;
			long pixel_idx = byte_idx / 3;
			int x = pixel_idx % img->w;
			int row = pixel_idx / img->w;
			int y = img->h - 1 - row;//conversie la coordonate carteziene
			int channel = byte_idx % 3;//aflu canalul de culoare (0=R, 1=G, 2=B)
			//copiez valorile curente
			unsigned char vals[3];
			vals[0] = img->block[pixel_idx].r;
			vals[1] = img->block[pixel_idx].g;
			vals[2] = img->block[pixel_idx].b;
			//modific bitul pentru a simula eroarea
			int bit_in_byte = 7 - (target_bit_idx % 8);
			vals[channel] ^= (1 << bit_in_byte);
			printf("Warning: pixel at (%d, %d) may be read as (%d, %d, %d)\n",
				   x, y, vals[0], vals[1], vals[2]);
		}
	}
}

void init_game(game *gs)
{
	//Seteaza valorile initiale pentru a incepe jocul curat
	gs->img.data = NULL;
	gs->img.block = NULL;
	gs->img.w = 0;
	gs->img.h = 0;
	gs->sys.loaded = 0;
	gs->sys.axiom = NULL;
	gs->sys.rules = NULL;
	gs->sys.filename = NULL;
	gs->fontp.loaded = 0;
	gs->fontp.name = NULL;
	gs->fontp.file = NULL;
	gs->history = NULL;
	gs->history_size = 0;
	gs->current = -1;
	gs->last_bg_file[0] = '\0';
	gs->running = 1;
}

void close_game(game *gs)
{
	//Elibereaza toata memoria si inchide programul corect
	cleanup(&gs->sys);
	free_image(&gs->img);
	free_font(&gs->fontp);
	for (int i = 0; i < gs->history_size; i++) {
		free_action(&gs->history[i]);
	}
	free(gs->history);
}

void do_load(game *gs, const char *command)
{
	char filename[256];
	strcpy(filename, command + 5);
	strcpy(gs->last_bg_file, command + 5);
	action act = {0};
	act.img_state = create_img_state(&gs->img);
	act.state_w = gs->img.w;
	act.state_h = gs->img.h;
	act.sys = copy_lsystem(&gs->sys);
	int ok;
	imag new_img = load_image(filename, &ok, 0);
	if (ok) {
		free_image(&gs->img);
		gs->img = new_img;
		act.load_ran = 1;
		act.load_file = malloc(strlen(filename) + 1);
		strcpy(act.load_file, filename);
		for (int k = gs->current + 1; k < gs->history_size; k++) {
			free_action(&gs->history[k]);
		}
		gs->history_size = gs->current + 1;
		gs->history = realloc(gs->history,
							  (gs->history_size + 1) * sizeof(action));
		gs->history[gs->history_size] = act;
		gs->current = gs->history_size;
		gs->history_size++;
	} else {
		//dacă a eșuat încărcarea, eliberez acțiunea pregătită
		free_action(&act);
	}
}

void do_font(game *gs, const char *command)
{
	char fname[256];
	strcpy(fname, command + 5);
	if (load_font(fname, &gs->fontp, 0)) {
		action act = {0};
		act.sys = copy_lsystem(&gs->sys);
		act.font_loaded = 1;
		act.font_file = malloc(strlen(fname) + 1);
		strcpy(act.font_file, fname);
		//cleanup Undo-uri vechi
		for (int k = gs->current + 1; k < gs->history_size; k++) {
			free_action(&gs->history[k]);
		}
		gs->history_size = gs->current + 1;
		gs->history = realloc(gs->history,
							  (gs->history_size + 1) * sizeof(action));
		gs->history[gs->history_size] = act;
		gs->current = gs->history_size;
		gs->history_size++;
	}
}

void do_type(game *gs, const char *command)
{
	if (!gs->img.data) {
		printf("No image loaded\n");
		return;
	}
	if (!gs->fontp.loaded) {
		printf("No font loaded\n");
		return;
	}
	const char *p = command + 5;
	const char *start = strchr(p, '"');
	if (!start) {
		return;
	}
	const char *end = strchr(start + 1, '"');
	if (!end) {
		return;
	}
	int len = end - (start + 1);
	char *txt = malloc(len + 1);
	strncpy(txt, start + 1, len);
	txt[len] = '\0';
	int tx, ty;
	unsigned char r, g, b;
	if (sscanf(end + 1, "%d %d %hhu %hhu %hhu",
			   &tx, &ty, &r, &g, &b) != 5) {
		free(txt);
		printf("Invalid TYPE command\n");
		return;
	}
	action act = {0};
	act.img_state = create_img_state(&gs->img);//Salvez cum arata poza acum
	act.state_w = gs->img.w;
	act.state_h = gs->img.h;
	draw_text(&gs->img, &gs->fontp, txt, tx, gs->img.h - 1 - ty, r, g, b);
	printf("Text written\n");
	act.sys = copy_lsystem(&gs->sys);
	act.type_ran = 1;
	act.text = txt;
	act.tx = tx;
	act.ty = ty;
	act.r = r;
	act.g = g;
	act.b = b;
	for (int k = gs->current + 1; k < gs->history_size; k++) {
		free_action(&gs->history[k]);
	}
	gs->history_size = gs->current + 1;
	gs->history = realloc(gs->history, (gs->history_size + 1) * sizeof(action));
	gs->history[gs->history_size] = act;
	gs->current = gs->history_size;
	gs->history_size++;
}

void do_lsystem(game *gs, const char *command)
{
	char filename[256];
	strcpy(filename, command + 8);
	if (load_lsystem(filename, &gs->sys)) {
		for (int i = gs->current + 1; i < gs->history_size; i++) {
			free_action(&gs->history[i]);
		}
		gs->history_size = gs->current + 1;
		action act = {0};
		act.sys = copy_lsystem(&gs->sys);
		act.turtle_ran = 0;
		gs->history = realloc(gs->history,
							  (gs->history_size + 1) * sizeof(action));
		gs->history[gs->history_size] = act;
		gs->current = gs->history_size;
		gs->history_size++;
		int nrules = 0;
		for (int i = 0; i < 256; i++) {
			if (gs->sys.rules[i]) {
				nrules++;
			}
		}
		printf("Loaded %s (L-system with %d rules)\n", filename, nrules);
	} else {
		printf("Failed to load %s\n", filename);
	}
}

void do_undo(game *gs)
{
	if (gs->current < 0) {
		printf("Nothing to undo\n");
	} else {
		if (gs->history[gs->current].img_state) {
			if (!gs->img.data ||
				gs->img.w != gs->history[gs->current].state_w ||
				gs->img.h != gs->history[gs->current].state_h) {
				free_image(&gs->img);
				gs->img.w = gs->history[gs->current].state_w;
				gs->img.h = gs->history[gs->current].state_h;
				gs->img.block = malloc(gs->img.w * gs->img.h * sizeof(pixel));
				gs->img.data = malloc(gs->img.h * sizeof(pixel *));
				//refacere pointeri linii
				for (int y = 0; y < gs->img.h; y++) {
					gs->img.data[y] = gs->img.block + y * gs->img.w;
				}
			}
			memcpy(gs->img.block, gs->history[gs->current].img_state,
				   gs->img.w * gs->img.h * sizeof(pixel));
		}
		cleanup(&gs->sys);
		gs->current--;
		if (gs->current >= 0) {
			gs->sys = copy_lsystem(&gs->history[gs->current].sys);
		} else {
			gs->sys.loaded = 0;//reset la starea fără L-system;
		}
		int found_font = 0;
		for (int i = gs->current; i >= 0; i--) {
			if (gs->history[i].font_loaded) {
				//reincarc fontul
				load_font(gs->history[i].font_file, &gs->fontp, 1);
				found_font = 1;
				break;
			}
		}
		if (!found_font) {
			free_font(&gs->fontp);
		}
	}
}

void do_redo(game *gs)
{
	if (gs->current + 1 >= gs->history_size) {
		printf("Nothing to redo\n");
	} else {
		gs->current++;
		cleanup(&gs->sys);
		gs->sys = copy_lsystem(&gs->history[gs->current].sys);
		if (!gs->history[gs->current].turtle_ran &&
			!gs->history[gs->current].font_loaded &&
			!gs->history[gs->current].type_ran &&
			!gs->history[gs->current].load_ran && gs->sys.loaded) {
			int nrules = 0;
			for (int i = 0; i < 256; i++) {
				if (gs->sys.rules[i]) {
					nrules++;
				}
			}
			printf("Loaded %s (L-system with %d rules)\n",
				   gs->sys.filename, nrules);
		}
		if (gs->history[gs->current].load_ran) {
			int ok;
			imag new_img = load_image(gs->history[gs->current].load_file,
									  &ok, 0);
			if (ok) {
				free_image(&gs->img);
				gs->img = new_img;
			}
		}
		if (gs->history[gs->current].turtle_ran) {
			if (gs->history[gs->current].cached_deriv) {
				interpret_turtle(&gs->img,
								 gs->history[gs->current].cached_deriv,
								 gs->history[gs->current].t,
								 gs->history[gs->current].r,
								 gs->history[gs->current].g,
								 gs->history[gs->current].b, 0);
			}
		}
		if (gs->history[gs->current].font_loaded) {
			load_font(gs->history[gs->current].font_file, &gs->fontp, 0);
		}
		if (gs->history[gs->current].type_ran) {
			draw_text(&gs->img, &gs->fontp, gs->history[gs->current].text,
					  gs->history[gs->current].tx,
					  gs->img.h - 1 - gs->history[gs->current].ty,
					  gs->history[gs->current].r, gs->history[gs->current].g,
					  gs->history[gs->current].b);
			printf("Text written\n");
		}
	}
}

void do_derive(game *gs, const char *command)
{
	int n = atoi(command + 7);
	if (!gs->sys.loaded) {
		printf("No L-system loaded\n");
	} else {
		char *result = derive(&gs->sys, n);
		if (result) {
			printf("%s\n", result);
			free(result);
		}
	}
}

void do_turtle(game *gs, const char *command)
{
	turtle t;
	if (!gs->img.data) {
		printf("No image loaded\n");
		return;
	}
	if (!gs->sys.loaded) {
		printf("No L-system loaded\n");
		return;
	}
	int iter;
	unsigned char r, g, b;
	if (sscanf(command + 7, "%lf %lf %lf %lf %lf %d %hhu %hhu %hhu",
			   &t.x, &t.y, &t.pas, &t.dir, &t.turn, &iter, &r, &g, &b) != 9) {
		printf("Invalid TURTLE command\n");
		return;
	}
	char *deriv = derive(&gs->sys, iter);
	if (!deriv) {
		printf("No L-system loaded\n");
		return;
	}
	action act = {0};
	act.img_state = create_img_state(&gs->img);
	act.state_w = gs->img.w;
	act.state_h = gs->img.h;
	interpret_turtle(&gs->img, deriv, t, r, g, b, 0);
	act.sys = copy_lsystem(&gs->sys);//copie L-system
	act.turtle_ran = 1;//TURTLE a fost rulat
	act.t = t;//parametrii TURTLE
	act.iter = iter;//număr de derivări
	act.r = r;
	act.g = g;
	act.b = b;//culoarea
	//șterg orice "redo" după poziția curentă
	act.cached_deriv = deriv;
	for (int k = gs->current + 1; k < gs->history_size; k++) {
		free_action(&gs->history[k]);
	}
	gs->history_size = gs->current + 1;
	gs->history = realloc(gs->history, (gs->history_size + 1) * sizeof(action));
	gs->history[gs->history_size] = act;
	gs->current = gs->history_size;
	gs->history_size++;
}

void do_save(game *g, const char *command)
{
	char filename[256];
	strcpy(filename, command + 5);
	save_image(&g->img, filename);
}
