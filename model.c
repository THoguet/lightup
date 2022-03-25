// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include "model.h"
#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_mixer.h>
#include <SDL_ttf.h>  // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_tools.h"

/* **************************************************************** */

/*	names of assets fichier*/
#define LIGHTBULB_WHITE "lightbulb_white.png"
#define LIGHTBULB_RED "lightbulb_red.png"
#define LB1 "lb1.mp3"
#define LB2 "lb2.mp3"
#define LB3 "lb3.mp3"
#define ERR1 "error1.mp3"
#define ERR2 "error2.mp3"
#define ERR3 "error3.mp3"
#define MARK1 "mark1.wav"
#define MARK2 "mark2.wav"
#define MARK3 "mark3.wav"
#define WIN "win.mp3"
#define FONT "Roboto-Regular.ttf"

#define NB_MUSIC 10
#define NB_MUSIC_PER_ARRAY 3
#define NB_BUTTONS 5
#define FONTSIZE 200
// size of each array (except musics) to allocate
#define SIZE_ARRAY uint sizeof_array[] = {2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 2, 1, 1, 1, 1, 1, 1}

#ifdef __ANDROID__
static void copy_asset(char* src, char* dst) {
	SDL_RWops* file = SDL_RWFromFile(src, "r");
	if (!file)
		ERROR("[ERROR] SDL_RWFromFile: %s\n", src);
	int size = SDL_RWsize(file);
	PRINT("copy file %s (%d bytes) into %s\n", src, size, dst);
	char* buf = (char*)malloc(size + 1);
	if (!buf)
		ERROR("%s", "[ERROR] malloc\n");
	int r = SDL_RWread(file, buf, 1, size);
	PRINT("read %d\n", r);
	if (r != size)
		ERROR("[ERROR] fail to read all file (%d bytes)\n", r);
	FILE* out = fopen(dst, "w+");
	if (!out)
		ERROR("[ERROR] fail to create file %s\n", dst);
	int w = fwrite(buf, 1, r, out);
	if (r != w)
		ERROR("[ERROR] fail to write all file (%d bytes)\n", w);
	fclose(out);
	SDL_RWclose(file);
	free(buf);
}
#endif

/* **************************************************************** */

/*		Fonction Minimum		*/
int int_min_intero(int a, int b) { /*interopérabilité*/
	if (a < b)
		return a;
	return b;
}

bool game_has_error_general_intero(cgame g) {
	for (uint i = 0; i < game_nb_rows(g); i++) {
		for (uint j = 0; j < game_nb_cols(g); j++) {
			if (game_has_error(g, i, j))
				return true;
		}
	}
	return false;
}

struct Env_t {
	game g;

	SDL_Texture** zero;          // array of two texture one white and one red for each number (red color = r:255 g:50 b:50)
	SDL_Texture** one;           // Same
	SDL_Texture** two;           // Same
	SDL_Texture** three;         // Same
	SDL_Texture** four;          // Same
	SDL_Texture** lightbulb;     // array of the two lightbulb images first white second red
	SDL_Texture** text_restart;  // array of two textures one when the button is up (the first one) and one when the button is down (the second one)
	SDL_Texture** text_undo;     // array of two texture one white (the first one) and one black (the second one)
	SDL_Texture** text_redo;     // Same
	SDL_Texture** text_solve;    // Same
	SDL_Texture** text_save;     // This one have three textures one white, one black and one green

	bool pressed_restart;
	bool pressed_undo;
	bool pressed_redo;
	bool pressed_solve;
	bool pressed_save;
	bool pressed_savED;
	bool win;

	SDL_Rect* rec_game;     // rectangle of the grid
	SDL_Rect* rec_redo;     // rectangle of each buttons
	SDL_Rect* rec_undo;     // ^^
	SDL_Rect* rec_restart;  // ^^
	SDL_Rect* rec_solve;    // ^^
	SDL_Rect* rec_save;     // ^^

	Mix_Music** lb_music;    // array of three musics for each action
	Mix_Music** err_music;   // Same
	Mix_Music** mark_music;  // Same
	Mix_Music* win_music;    // Only one music when wining

	uint lb_music_cpt;
	uint err_music_cpt;
	uint mark_music_cpt;
};

/* **************************************************************** */
void usage(char* argv[]) {
	PRINT("Usage : %s            => Joue la partie par défaut\n", argv[0]);
	PRINT("        %s <filename> => Charge la partie sur le fichier <filename>\n", argv[0]);
	exit(EXIT_FAILURE);
}

/*	Fonction to return the texture of the given path	*/
SDL_Texture* render_blended_text(SDL_Renderer* ren, SDL_Color color, char* text) {
	TTF_Font* font = TTF_OpenFont(FONT, FONTSIZE);  // TO EDIT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! (ANDROID)
	if (!font)
		ERROR("TTF_OpenFont: %s\n", FONT);
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);

	SDL_Surface* surf = TTF_RenderText_Blended(font, text, color);
	SDL_Texture* tmp = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);

	return tmp;
}

/*		Fonction to allocate each array of env		*/
void init_malloc(void** tab[], unsigned long* sizeof_tab, uint size_tab) {
	for (uint i = 0; i < size_tab; i++) {
		*(tab[i]) = malloc(sizeof_tab[i]);  // malloc with the corressponding size
		if (*(tab[i]) == NULL)
			ERROR("%s", "Not enough memory.\n");
	}
}

void init_musics(Mix_Music**** music_to_load, char** path_to_musics, uint nb_music, uint nb_music_per_array) {
	for (uint i = 0; i < nb_music / nb_music_per_array; i++) {
		*(music_to_load[i]) = malloc(sizeof(Mix_Music*) * nb_music_per_array);
		for (uint j = 0; j < nb_music_per_array; j++) {
			(*(music_to_load[i]))[j] = Mix_LoadMUS(path_to_musics[i * nb_music_per_array + j]);
			if ((*(music_to_load[i]))[j] == NULL)
				ERROR("Cannot load music %s\n", path_to_musics[i * nb_music_per_array + j]);
		}
	}
}

Env* init(SDL_Renderer* ren, int argc, char* argv[]) {
	Env* env = malloc(sizeof(struct Env_t));
	Mix_Music*** music_to_load[] = {&(env->lb_music), &(env->err_music), &(env->mark_music)};
#ifdef __ANDROID__
	char* path_to_get[NB_MUSIC] = {LB1, LB2, LB3, ERR1, ERR2, ERR3, MARK1, MARK2, MARK3, WIN};
	char* paths[NB_MUSIC];
	const char* dir = SDL_AndroidGetInternalStoragePath();
	char new_path[1024];
	for (uint i = 0; i < NB_MUSIC; i++) {
		paths[i] = malloc(sizeof(char) * 1024);
		sprintf(new_path, "%s/%s", dir, path_to_get[i]);
		copy_asset(path_to_get[i], new_path);
		strcpy(paths[i], new_path);
	}
#else
	char* paths[NB_MUSIC] = {LB1, LB2, LB3, ERR1, ERR2, ERR3, MARK1, MARK2, MARK3, WIN};
#endif
	env->pressed_restart = false;
	env->pressed_undo = false;
	env->pressed_redo = false;
	env->pressed_solve = false;
	env->pressed_save = false;
	env->pressed_savED = false;
	PRINT("%s", "To win the game, you must satisfy the following conditions:\n\n");
	PRINT("%s",
	      "-All non-black squares are lit.\n-No light is lit by another light.\n-Each numbered black square must be orthogonally adjacent to exactly the given "
	      "number of lights.\n-Non-numbered black squares may have any number of lights adjacent to them.\n");
	if (argc == 1) {
		// Create new game
		env->g = game_default();
	} else if (argc == 2) {
		char* gameFile = argv[1];
		env->g = game_load(gameFile);
	} else {
		usage(argv);
	}
	SIZE_ARRAY;
	uint cpt = 0;
	uint index_rec = 11;
	void** env_tab[] = {(void**)&(env->zero),       (void**)&(env->one),          (void**)&(env->two),       (void**)&(env->three),
	                    (void**)&(env->four),       (void**)&(env->text_restart), (void**)&(env->text_undo), (void**)&(env->text_redo),
	                    (void**)&(env->text_solve), (void**)&(env->text_save),    (void**)&(env->lightbulb), (void**)&(env->rec_game),
	                    (void**)&(env->rec_redo),   (void**)&(env->rec_restart),  (void**)&(env->rec_solve), (void**)&(env->rec_undo),
	                    (void**)&(env->rec_save)};
	unsigned long sizeof_env_tab[] = {
	    sizeof(SDL_Texture*) * sizeof_array[cpt++], sizeof(SDL_Texture*) * sizeof_array[cpt++], sizeof(SDL_Texture*) * sizeof_array[cpt++],
	    sizeof(SDL_Texture*) * sizeof_array[cpt++], sizeof(SDL_Texture*) * sizeof_array[cpt++], sizeof(SDL_Texture*) * sizeof_array[cpt++],
	    sizeof(SDL_Texture*) * sizeof_array[cpt++], sizeof(SDL_Texture*) * sizeof_array[cpt++], sizeof(SDL_Texture*) * sizeof_array[cpt++],
	    sizeof(SDL_Texture*) * sizeof_array[cpt++], sizeof(SDL_Texture*) * sizeof_array[cpt++], sizeof(SDL_Rect) * sizeof_array[cpt++],
	    sizeof(SDL_Rect) * sizeof_array[cpt++],     sizeof(SDL_Rect) * sizeof_array[cpt++],     sizeof(SDL_Rect) * sizeof_array[cpt++],
	    sizeof(SDL_Rect) * sizeof_array[cpt++],     sizeof(SDL_Rect) * sizeof_array[cpt++]};
	init_malloc(env_tab, sizeof_env_tab, sizeof(env_tab) / sizeof(env_tab[0]));
	for (uint i = index_rec; i < sizeof(env_tab) / sizeof(env_tab[0]); i++) {
		((SDL_Rect*)(*(env_tab[i])))->h = 0;
		((SDL_Rect*)(*(env_tab[i])))->w = 0;
		((SDL_Rect*)(*(env_tab[i])))->x = 0;
		((SDL_Rect*)(*(env_tab[i])))->y = 0;
	}
	// the normal lightbulb
	env->lightbulb[0] = IMG_LoadTexture(ren, LIGHTBULB_WHITE);
	if (!env->lightbulb[0])
		ERROR("IMG_LoadTexture: %s\n", LIGHTBULB_WHITE);
	// the error one
	env->lightbulb[1] = IMG_LoadTexture(ren, LIGHTBULB_RED);
	if (!env->lightbulb[1])
		ERROR("IMG_LoadTexture: %s\n", LIGHTBULB_RED);
	init_musics(music_to_load, paths, NB_MUSIC, 3);
	env->lb_music_cpt = 0;
	env->err_music_cpt = 0;
	env->mark_music_cpt = 0;
	// Music win
	env->win_music = Mix_LoadMUS(WIN);
	if (env->win_music == NULL)
		ERROR("Cannot load music %s\n", WIN);

	env->win = false;

#ifdef __ANDROID__
	for (uint i = 0; i < NB_MUSIC; i++) {
		free(paths[i]);
	}
#endif

	/*Creation of the differents textures and place them in env*/
	SDL_Color color_w = {255, 255, 255, SDL_ALPHA_OPAQUE};  // color of 0 in black wall with error
	SDL_Color color_r = {255, 50, 50, SDL_ALPHA_OPAQUE};    /* blue color in RGBA */
	SDL_Color color_g = {0, 150, 0, SDL_ALPHA_OPAQUE};      /* green color in RGBA */
	SDL_Color color_b = {0, 0, 0, SDL_ALPHA_OPAQUE};        /* black color in RGBA*/

	char* tab_texts_double[] = {"0", "1", "2", "3", "4", "Restart", "Undo", "Redo", "Solve", "Save"};
	SDL_Color color_of_texts[] = {color_w, color_r, color_w, color_r, color_w, color_r, color_w, color_r, color_w, color_r, color_w,
	                              color_b, color_w, color_b, color_w, color_b, color_w, color_b, color_w, color_b, color_g};
	SDL_Texture** tab_textures_double[] = {env->zero,         env->one,       env->two,       env->three,      env->four,
	                                       env->text_restart, env->text_undo, env->text_redo, env->text_solve, env->text_save};
	cpt = 0;
	for (uint i = 0; i < sizeof(tab_textures_double) / sizeof(tab_textures_double[0]); i++) {
		for (uint j = 0; j < sizeof_array[i]; j++) {
			tab_textures_double[i][j] = render_blended_text(ren, color_of_texts[cpt++], tab_texts_double[i]);
		}
	}
	return env;
}

/* **************************************************************** */

void render_blank(SDL_Renderer* ren, SDL_Rect* rec, bool lighted, bool victory) {
	if (victory)
		SDL_SetRenderDrawColor(ren, 0, 150, 0, SDL_ALPHA_OPAQUE);  // green
	else if (lighted)
		SDL_SetRenderDrawColor(ren, 255, 255, 0, SDL_ALPHA_OPAQUE);  // yellow
	else
		SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);  // white
	SDL_RenderFillRect(ren, rec);

	SDL_SetRenderDrawColor(ren, 127, 127, 127, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(ren, rec);
}

void render_wall(SDL_Renderer* ren, Env* env, SDL_Rect* rec, int nb, bool error) {
	SDL_Rect number;
	SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(ren, rec);
	switch (nb) {
		case 0:
			SDL_QueryTexture(env->zero[error ? 1 : 0], NULL, NULL, &number.w, &number.h);
			number.x = rec->x + rec->w / 4;
			number.y = rec->y + rec->h / 4;
			number.w = rec->w / 2;
			number.h = rec->h / 2;
			SDL_RenderCopy(ren, env->zero[error ? 1 : 0], NULL, &number);
			break;
		case 1:
			SDL_QueryTexture(env->one[error ? 1 : 0], NULL, NULL, &number.w, &number.h);
			number.x = rec->x + rec->w / 4;
			number.y = rec->y + rec->h / 4;
			number.w = rec->w / 2;
			number.h = rec->h / 2;
			SDL_RenderCopy(ren, env->one[error ? 1 : 0], NULL, &number);
			break;
		case 2:
			SDL_QueryTexture(env->two[error ? 1 : 0], NULL, NULL, &number.w, &number.h);
			number.x = rec->x + rec->w / 4;
			number.y = rec->y + rec->h / 4;
			number.w = rec->w / 2;
			number.h = rec->h / 2;
			SDL_RenderCopy(ren, env->two[error ? 1 : 0], NULL, &number);
			break;
		case 3:
			SDL_QueryTexture(env->three[error ? 1 : 0], NULL, NULL, &number.w, &number.h);
			number.x = rec->x + rec->w / 4;
			number.y = rec->y + rec->h / 4;
			number.w = rec->w / 2;
			number.h = rec->h / 2;
			SDL_RenderCopy(ren, env->three[error ? 1 : 0], NULL, &number);
			break;
		case 4:
			SDL_QueryTexture(env->four[error ? 1 : 0], NULL, NULL, &number.w, &number.h);
			number.x = rec->x + rec->w / 4;
			number.y = rec->y + rec->h / 4;
			number.w = rec->w / 2;
			number.h = rec->h / 2;
			SDL_RenderCopy(ren, env->four[error ? 1 : 0], NULL, &number);
			break;
	}
}

void render_lightbulb(SDL_Renderer* ren, SDL_Rect* rec, SDL_Texture* lightbulb_texture, bool victory) {
	SDL_Rect lightbulb;
	if (victory)
		SDL_SetRenderDrawColor(ren, 0, 150, 0, SDL_ALPHA_OPAQUE);
	else
		SDL_SetRenderDrawColor(ren, 255, 255, 0, SDL_ALPHA_OPAQUE);
	SDL_RenderFillRect(ren, rec);
	SDL_SetRenderDrawColor(ren, 127, 127, 127, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(ren, rec);
	lightbulb.h = rec->h - rec->h / 4;
	lightbulb.w = rec->w - rec->w / 4;
	lightbulb.x = rec->x + rec->w / 2 - lightbulb.w / 2;
	lightbulb.y = rec->y + rec->h / 2 - lightbulb.h / 2;
	SDL_RenderCopy(ren, lightbulb_texture, NULL, &lightbulb);
}

void render_mark(SDL_Renderer* ren, SDL_Rect* rec, bool lighted, bool victory) {
	if (victory)
		SDL_SetRenderDrawColor(ren, 0, 150, 0, SDL_ALPHA_OPAQUE);  // green
	else if (lighted)
		SDL_SetRenderDrawColor(ren, 255, 255, 0, SDL_ALPHA_OPAQUE);  // yellow
	else
		SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);  // white
	SDL_RenderFillRect(ren, rec);
	SDL_SetRenderDrawColor(ren, 127, 127, 127, SDL_ALPHA_OPAQUE);
	SDL_RenderDrawRect(ren, rec);
	SDL_SetRenderDrawColor(ren, 0, 0, 0, SDL_ALPHA_OPAQUE);
	SDL_Rect mark;
	mark.h = rec->h / 4;
	mark.w = rec->w / 4;
	mark.x = rec->x + rec->w / 2 - mark.w / 2;
	mark.y = rec->y + rec->h / 2 - mark.h / 2;
	SDL_RenderFillRect(ren, &mark);
}

void render(SDL_Window* win, SDL_Renderer* ren, Env* env) {
	int win_w, win_h, h, w;
	SDL_GetWindowSize(win, &win_w, &win_h);

	if (win_h > win_w) {  // used to keep the ratio of the buttons
		h = win_w;
		w = win_w;
	} else {
		h = win_h;
		w = win_h;
	}

	int marge_h = (win_h - h) / 2;
	int marge_w = (win_w - w) / 2;

	h = h - h / 10;
	// set background color
	SDL_SetRenderDrawColor(ren, 24, 26, 27, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(ren);
	SDL_Rect rec;
	int size_rec = int_min_intero(w / game_nb_cols(env->g), h / game_nb_rows(env->g));
	rec.h = size_rec;
	rec.w = size_rec;
	int rec_x = w / 2 - size_rec * game_nb_cols(env->g) / 2 + marge_w;
	int rec_y = h / 2 - size_rec * game_nb_rows(env->g) / 2 + h / 10 + marge_h;
	rec.x = rec_x;
	rec.y = rec_y;
	env->rec_game->x = rec.x;
	env->rec_game->y = rec.y;
	env->rec_game->h = size_rec * game_nb_rows(env->g);
	env->rec_game->w = size_rec * game_nb_cols(env->g);
	// buttons
	SDL_Rect buttons;
	buttons.w = w / (NB_BUTTONS + 1);
	buttons.h = h / 10 / 1.5;  // 1.5 => used to keep margin around buttons
	buttons.y = (h / 10 - buttons.h) / 2 + marge_h;
	buttons.x = (w / NB_BUTTONS - w / (NB_BUTTONS + 1)) / 2 + marge_w;
	*(env->rec_undo) = buttons;
	SDL_RenderCopy(ren, env->text_undo[env->pressed_undo ? 1 : 0], NULL, &buttons);
	buttons.x += buttons.w + (w / NB_BUTTONS - w / (NB_BUTTONS + 1));
	*(env->rec_redo) = buttons;
	SDL_RenderCopy(ren, env->text_redo[env->pressed_redo ? 1 : 0], NULL, &buttons);
	buttons.x += buttons.w + (w / NB_BUTTONS - w / (NB_BUTTONS + 1));
	*(env->rec_restart) = buttons;
	SDL_RenderCopy(ren, env->text_restart[env->pressed_restart ? 1 : 0], NULL, &buttons);
	buttons.x += buttons.w + (w / NB_BUTTONS - w / (NB_BUTTONS + 1));
	*(env->rec_solve) = buttons;
	SDL_RenderCopy(ren, env->text_solve[env->pressed_solve ? 1 : 0], NULL, &buttons);
	buttons.x += buttons.w + (w / NB_BUTTONS - w / (NB_BUTTONS + 1));
	*(env->rec_save) = buttons;
	SDL_RenderCopy(ren, env->text_save[env->pressed_savED ? 2 : env->pressed_save ? 1 : 0], NULL, &buttons);
	// render cases
	for (uint i = 0; i < game_nb_rows(env->g); i++) {
		for (uint j = 0; j < game_nb_cols(env->g); j++) {
			if (game_is_lightbulb(env->g, i, j))
				render_lightbulb(ren, &rec, env->lightbulb[game_has_error(env->g, i, j) ? 1 : 0], env->win);
			else if (game_is_black(env->g, i, j))
				render_wall(ren, env, &rec, game_get_black_number(env->g, i, j), game_has_error(env->g, i, j));
			else if (game_is_blank(env->g, i, j))
				render_blank(ren, &rec, game_is_lighted(env->g, i, j), env->win);
			else if (game_is_marked(env->g, i, j))
				render_mark(ren, &rec, game_is_lighted(env->g, i, j), env->win);
			rec.x += size_rec;
		}
		rec.x = rec_x;
		rec.y += size_rec;
	}
	if (env->win)
		env->win = true;
}

/* **************************************************************** */

void play_Music(Env* env, bool lightbulb, bool error, bool mark) {
	if (error && game_has_error_general_intero(env->g)) {
		Mix_PlayMusic(env->err_music[env->err_music_cpt % 3], 0);
		env->err_music_cpt++;
	} else if (lightbulb) {
		Mix_PlayMusic(env->lb_music[env->lb_music_cpt % 3], 0);
		env->lb_music_cpt++;
	} else if (mark) {
		Mix_PlayMusic(env->mark_music[env->mark_music_cpt % 3], 0);
		env->mark_music_cpt++;
	}
}

void ToggleFullscreen(SDL_Window* Window) {
	Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
	bool IsFullscreen = SDL_GetWindowFlags(Window) & FullscreenFlag;
	SDL_SetWindowFullscreen(Window, IsFullscreen ? 0 : FullscreenFlag);
	SDL_ShowCursor(IsFullscreen);
}

void play_light(uint i, uint j, Env* env) {
	if (game_is_blank(env->g, i, j) || game_is_marked(env->g, i, j)) {
		game_play_move(env->g, i, j, S_LIGHTBULB);
		play_Music(env, true, true, false);
	} else if (game_is_lightbulb(env->g, i, j)) {
		game_play_move(env->g, i, j, S_BLANK);
		play_Music(env, true, true, false);
	}
}

void play_mark(uint i, uint j, Env* env) {
	if (game_is_blank(env->g, i, j) || game_is_lightbulb(env->g, i, j)) {
		game_play_move(env->g, i, j, S_MARK);
		play_Music(env, false, true, true);
	} else if (game_is_marked(env->g, i, j)) {
		game_play_move(env->g, i, j, S_BLANK);
		play_Music(env, false, true, true);
	}
}

void update_pressed(Env* env, bool undo, bool redo, bool restart, bool solve, bool save, bool savED) {
	env->pressed_undo = undo;
	env->pressed_redo = redo;
	env->pressed_restart = restart;
	env->pressed_solve = solve;
	env->pressed_save = save;
	env->pressed_savED = savED;
}

bool process(SDL_Window* win, Env* env, SDL_Event* e, SDL_Event* prec_e, int* nb_coups, int* nb_undo) {
	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	if (e->type == SDL_QUIT) {
		return true;
	}
	// Tests of the differents types of inputs to interact with the game
	if (e->type == SDL_KEYDOWN) {  // differents shortcuts for differents types of actions
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_F11]) {  // F11 to toggle fullscreen (pretty obvious)
			ToggleFullscreen(win);
		} else if (state[SDL_SCANCODE_W]) {  // (W becuase sdl2 is in QWERTY) Z to undo a move only if you have done at least 1 move before
			if ((*nb_coups) > 0) {
				game_undo(env->g);
				(*nb_undo)++;
				(*nb_coups)--;
				update_pressed(env, true, false, false, false, false, false);
			}
		} else if (state[SDL_SCANCODE_Y]) {  // Y to redo a move can only if you have done at least 1 undo before
			if ((*nb_undo) > 0) {
				game_redo(env->g);
				(*nb_undo)--;
				(*nb_coups)++;
				update_pressed(env, false, true, false, false, false, false);
			}
		} else if (state[SDL_SCANCODE_S]) {  // S to solving the game immediately
			game_solve(env->g);
			(*nb_undo) = 0;
			(*nb_coups)++;
			update_pressed(env, false, false, false, true, false, false);
		} else if (state[SDL_SCANCODE_R] || state[SDL_SCANCODE_F5]) {  // R or F5 to restart the game
			game_restart(env->g);
			(*nb_undo) = 0;
			(*nb_coups) = 0;
			update_pressed(env, false, false, true, false, false, false);
		} else if (state[SDL_SCANCODE_Z]) {  // (Z becuase sdl2 is in QWERTY) W to save the game in the file save.txt
			game_save(env->g, "save.txt");
			update_pressed(env, false, false, false, false, false, true);
		}
	} else if (e->type == SDL_KEYUP) {
		update_pressed(env, false, false, false, false, false, false);
	}
	if (game_is_over(env->g) && !env->win) {
		Mix_PlayMusic(env->win_music, 1);
		env->win = true;
	} else if (env->win && !game_is_over(env->g)) {
		env->win = false;
	}
	if (e->type == SDL_MOUSEMOTION || e->type == SDL_FINGERDOWN) {
		SDL_Point mouse;
		if (e->type == SDL_MOUSEMOTION) {
			SDL_GetMouseState(&mouse.x, &mouse.y);
		} else {
			mouse.x = e->tfinger.x;
			mouse.y = e->tfinger.y;
		}
		if (SDL_PointInRect(&mouse, env->rec_restart)) {
			update_pressed(env, false, false, true, false, false, false);
		} else if (SDL_PointInRect(&mouse, env->rec_undo)) {
			if ((*nb_coups) > 0)
				update_pressed(env, true, false, false, false, false, false);
			else
				update_pressed(env, false, false, false, false, false, false);
		} else if (SDL_PointInRect(&mouse, env->rec_redo)) {
			if ((*nb_undo) > 0)
				update_pressed(env, false, true, false, false, false, false);
			else
				update_pressed(env, false, false, false, false, false, false);

		} else if (SDL_PointInRect(&mouse, env->rec_solve)) {
			update_pressed(env, false, false, false, true, false, false);

		} else if (SDL_PointInRect(&mouse, env->rec_save)) {
			update_pressed(env, false, false, false, false, true, false);
		} else {
			update_pressed(env, false, false, false, false, false, false);
		}

#ifdef _ANDROID_
		if (SDL_PointInRect(&mouse, env->rec_game)) {
			if (e->type != prec_e->type) {
				prec_e->type = e->type;
				prec_e->tfinger.timestamp = e->tfinger.timestamp;
			}
		}
#endif
	} else if (e->type == SDL_MOUSEBUTTONDOWN || e->type == SDL_FINGERUP) {
		SDL_Point mouse;
		if (e->type == SDL_MOUSEBUTTONDOWN) {
			SDL_GetMouseState(&mouse.x, &mouse.y);
		} else {
			mouse.x = e->tfinger.x;
			mouse.y = e->tfinger.y;
		}

		if (SDL_PointInRect(&mouse, env->rec_restart)) {
			game_restart(env->g);
			(*nb_undo) = 0;
			(*nb_coups) = 0;
		} else if (SDL_PointInRect(&mouse, env->rec_undo)) {
			if ((*nb_coups) > 0) {
				game_undo(env->g);
				(*nb_undo)++;
				(*nb_coups)--;
			}
		} else if (SDL_PointInRect(&mouse, env->rec_redo)) {
			if ((*nb_undo) > 0) {
				game_redo(env->g);
				(*nb_undo)--;
				(*nb_coups)++;
			}
		} else if (SDL_PointInRect(&mouse, env->rec_solve)) {
			game_solve(env->g);
			(*nb_undo) = 0;
			(*nb_coups)++;
		} else if (SDL_PointInRect(&mouse, env->rec_save)) {
			game_save(env->g, "save.txt");
			update_pressed(env, false, false, false, false, false, true);
		} else if (SDL_PointInRect(&mouse, env->rec_game)) {
			uint i = (((float)mouse.y - (float)env->rec_game->y) / (float)env->rec_game->h * game_nb_rows(env->g)) -
			         0.00001 /*to avoid if clicked exacly on the bottom right corner to result a 7 (in case of a game 7x7)*/;
			uint j = (((float)mouse.x - (float)env->rec_game->x) / (float)env->rec_game->w * game_nb_cols(env->g)) -
			         0.00001 /*to avoid if clicked exacly on the bottom right corner to result a 7 (in case of a game 7x7)*/;

			if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_LEFT) {  // only execute when type == SDL_MOUSEBUTTONDOWN
				play_light(i, j, env);
			} else if (e->type == SDL_MOUSEBUTTONDOWN && e->button.button == SDL_BUTTON_RIGHT) {
				play_mark(i, j, env);
			}

			if (e->type == SDL_FINGERUP && (e->tfinger.timestamp - prec_e->tfinger.timestamp) < 300) {  // only execute when type == SDL_FINGERUP
				play_light(i, j, env);
			} else if (e->type == SDL_FINGERUP) {
				play_mark(i, j, env);
			}
			(*nb_undo) = 0;
			(*nb_coups)++;
		}
	}
	return false;
}

/* **************************************************************** */

void clean_destroy(SDL_Texture** tab[], uint* sizePerArray, uint size_tab) {
	for (uint i = 0; i < size_tab; i++) {
		for (uint j = 0; j < sizePerArray[i]; j++) {
			SDL_DestroyTexture(tab[i][j]);
		}
		free(tab[i]);
	}
}

void freeall(SDL_Rect* tab[], uint size_tab) {
	for (uint i = 0; i < size_tab; i++) {
		free(tab[i]);
	}
}

void clean(Env* env) {
	SIZE_ARRAY;
	game_delete(env->g);
	// destroy all array of texture
	SDL_Texture** env_tab[] = {env->zero,      env->one,       env->two,        env->three,     env->four,     env->text_restart,
	                           env->text_undo, env->text_redo, env->text_solve, env->text_save, env->lightbulb};
	clean_destroy(env_tab, sizeof_array, sizeof(env_tab) / sizeof(env_tab[0]));
	SDL_Rect* free_tab[] = {env->rec_game, env->rec_redo, env->rec_restart, env->rec_solve, env->rec_undo};
	freeall(free_tab, sizeof(free_tab) / sizeof(free_tab[0]));
	for (uint i = 0; i < NB_MUSIC_PER_ARRAY; i++) {
		Mix_FreeMusic(env->lb_music[i]);
		Mix_FreeMusic(env->mark_music[i]);
		Mix_FreeMusic(env->err_music[i]);
	}
	Mix_FreeMusic(env->win_music);
	free(env->lb_music);
	free(env->mark_music);
	free(env->err_music);
	free(env);
}

/* **************************************************************** */
