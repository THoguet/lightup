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
#include "game_private.h"
#include "game_tools.h"

/* **************************************************************** */

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
#define FONTSIZE 200

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

struct Env_t {
	game g;
	SDL_Texture** zero;  // array of two texture one white and one red for each number (red color = r:255 g:50 b:50)
	SDL_Texture** one;
	SDL_Texture** two;
	SDL_Texture** three;
	SDL_Texture** four;
	SDL_Texture** lightbulb;     // array of the two lightbulb images first white second red
	SDL_Texture** text_restart;  // up fist then down
	SDL_Texture** text_undo;
	SDL_Texture** text_redo;
	SDL_Texture** text_solve;
	bool pressed_restart;
	bool pressed_undo;
	bool pressed_redo;
	bool pressed_solve;
	SDL_Rect* rec_redo;  // rectangle of each buttons
	SDL_Rect* rec_undo;
	SDL_Rect* rec_restart;
	SDL_Rect* rec_solve;
	SDL_Rect* rec_game;  // rectangle of the grid
	Mix_Music** lb_music;
	uint lb_music_cpt;
	Mix_Music** err_music;
	uint err_music_cpt;
	Mix_Music** mark_music;
	uint mark_music_cpt;
	Mix_Music* win_music;
};

/* **************************************************************** */
void usage(char* argv[]) {
	PRINT("Usage : %s            => Joue la partie par défaut\n", argv[0]);
	PRINT("        %s <filename> => Charge la partie sur le fichier <filename>\n", argv[0]);
	exit(EXIT_FAILURE);
}

void render_blended_text(SDL_Renderer* ren, Env* env) {
#ifdef __ANDROID__
	const char* dir = SDL_AndroidGetInternalStoragePath();
	char font_path[1024];
	sprintf(font_path, "%s/%s", dir, FONT);
	copy_asset(FONT, font_path);
	TTF_Font* font = TTF_OpenFont(font_path, FONTSIZE);
#else
	TTF_Font* font = TTF_OpenFont(FONT, FONTSIZE);
#endif
	if (!font)
		ERROR("TTF_OpenFont: %s\n", FONT);
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	// color of 0 in black wall
	SDL_Color color_w = {255, 255, 255, 255};
	// color of 0 in black wall with error
	SDL_Color color_r = {255, 50, 50, 255}; /* blue color in RGBA */

	SDL_Color color_b = {0, 0, 0, 255};

	/*rendu restart*/
	SDL_Surface* surf = TTF_RenderText_Blended(font, "Restart", color_b);
	env->text_restart[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	surf = TTF_RenderText_Blended(font, "Restart", color_w);
	env->text_restart[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/*rendu undo*/
	surf = TTF_RenderText_Blended(font, "Undo", color_b);
	env->text_undo[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	surf = TTF_RenderText_Blended(font, "Undo", color_w);
	env->text_undo[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/*rerndu redo*/
	surf = TTF_RenderText_Blended(font, "Redo", color_b);
	env->text_redo[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	surf = TTF_RenderText_Blended(font, "Redo", color_w);
	env->text_redo[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/*rendu solve*/
	surf = TTF_RenderText_Blended(font, "Solve", color_b);
	env->text_solve[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	surf = TTF_RenderText_Blended(font, "Solve", color_w);
	env->text_solve[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/* init zero texture double tab*/
	// if the case has not error
	surf = TTF_RenderText_Blended(font, "0", color_w);
	env->zero[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	// if the case has error
	surf = TTF_RenderText_Blended(font, "0", color_r);
	env->zero[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/* init one texture double tab*/
	// if the case has not error
	surf = TTF_RenderText_Blended(font, "1", color_w);
	env->one[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	// if the case has error
	surf = TTF_RenderText_Blended(font, "1", color_r);
	env->one[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/* init two texture double tab*/
	// if the case has not error
	surf = TTF_RenderText_Blended(font, "2", color_w);
	env->two[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	// if the case has error
	surf = TTF_RenderText_Blended(font, "2", color_r);
	env->two[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/* init three texture double tab*/
	// if the case has not error
	surf = TTF_RenderText_Blended(font, "3", color_w);
	env->three[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	// if the case has error
	surf = TTF_RenderText_Blended(font, "3", color_r);
	env->three[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/* init four texture double tab*/
	// if the case has not error
	surf = TTF_RenderText_Blended(font, "4", color_w);
	env->four[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	// if the case has error
	surf = TTF_RenderText_Blended(font, "4", color_r);
	env->four[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	TTF_CloseFont(font);
}

Env* init(SDL_Renderer* ren, int argc, char* argv[]) {
	Env* env = malloc(sizeof(struct Env_t));
	env->pressed_restart = false;
	env->pressed_undo = false;
	env->pressed_redo = false;
	env->pressed_solve = false;
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
	/* init zero texture double tab*/
	env->zero = malloc(sizeof(SDL_Texture*) * 2);
	if (env->zero == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	/* init one texture double tab*/
	env->one = malloc(sizeof(SDL_Texture*) * 2);
	if (env->one == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	/* init two texture double tab*/
	env->two = malloc(sizeof(SDL_Texture*) * 2);
	if (env->two == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	/* init three texture double tab*/
	env->three = malloc(sizeof(SDL_Texture*) * 2);
	if (env->three == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	/* init four texture double tab*/
	env->four = malloc(sizeof(SDL_Texture*) * 2);
	if (env->four == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	/* init lightbulb texture double tab*/
	env->lightbulb = malloc(sizeof(SDL_Texture*) * 2);
	if (env->lightbulb == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");
	// if the case has not error
	env->lightbulb[0] = IMG_LoadTexture(ren, LIGHTBULB_WHITE);
	if (!env->lightbulb[0])
		ERROR("IMG_LoadTexture: %s\n", LIGHTBULB_WHITE);
	// if the case has error
	env->lightbulb[1] = IMG_LoadTexture(ren, LIGHTBULB_RED);
	if (!env->lightbulb[1])
		ERROR("IMG_LoadTexture: %s\n", LIGHTBULB_RED);

	/* init text_restart texture double tab*/
	env->text_restart = malloc(sizeof(SDL_Texture*) * 2);
	if (env->text_restart == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	/* init text_undo texture double tab*/
	env->text_undo = malloc(sizeof(SDL_Texture*) * 2);
	if (env->text_undo == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	/* init text_redo texture double tab*/
	env->text_redo = malloc(sizeof(SDL_Texture*) * 2);
	if (env->text_redo == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	/* init text_solve texture double tab*/
	env->text_solve = malloc(sizeof(SDL_Texture*) * 2);
	if (env->text_solve == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	render_blended_text(ren, env);

	env->rec_redo = malloc(sizeof(SDL_Rect));
	if (env->rec_redo == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	env->rec_game = malloc(sizeof(SDL_Rect));
	if (env->rec_game == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	env->rec_restart = malloc(sizeof(SDL_Rect));
	if (env->rec_restart == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	env->rec_solve = malloc(sizeof(SDL_Rect));
	if (env->rec_solve == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");

	env->rec_undo = malloc(sizeof(SDL_Rect));
	if (env->rec_undo == NULL)
		ERROR("%s", "NOT ENOUGTH MEMORY\n");
	// Musics lightbulb
	env->lb_music = malloc(sizeof(Mix_Music*) * 3);
	if (env->lb_music == NULL)
		ERROR("%s", "NOT ENOUGH MEMORY\n");
	env->lb_music[0] = Mix_LoadMUS(LB1);
	if (env->lb_music[0] == NULL)
		ERROR("Cannot load music %s\n", LB1);
	env->lb_music[1] = Mix_LoadMUS(LB2);
	if (env->lb_music[1] == NULL)
		ERROR("Cannot load music %s\n", LB2);
	env->lb_music[2] = Mix_LoadMUS(LB3);
	if (env->lb_music[2] == NULL)
		ERROR("Cannot load music %s\n", LB3);
	env->lb_music_cpt = 0;

	// Musics errors
	env->err_music = malloc(sizeof(Mix_Music*) * 3);
	if (env->err_music == NULL)
		ERROR("%s", "NOT ENOUGH MEMORY\n");
	env->err_music[0] = Mix_LoadMUS(ERR1);
	if (env->err_music[0] == NULL)
		ERROR("Cannot load music %s\n", ERR1);
	env->err_music[1] = Mix_LoadMUS(ERR2);
	if (env->err_music[1] == NULL)
		ERROR("Cannot load music %s\n", ERR2);
	env->err_music[2] = Mix_LoadMUS(ERR3);
	if (env->err_music[2] == NULL)
		ERROR("Cannot load music %s\n", ERR3);
	env->err_music_cpt = 0;

	// Musics marks
	env->mark_music = malloc(sizeof(Mix_Music*) * 3);
	if (env->mark_music == NULL)
		ERROR("%s", "NOT ENOUGH MEMORY\n");
	env->mark_music[0] = Mix_LoadMUS(MARK1);
	if (env->mark_music[0] == NULL)
		ERROR("Cannot load music %s\n", MARK1);
	env->mark_music[1] = Mix_LoadMUS(MARK2);
	if (env->mark_music[1] == NULL)
		ERROR("Cannot load music %s\n", MARK2);
	env->mark_music[2] = Mix_LoadMUS(MARK3);
	if (env->mark_music[2] == NULL)
		ERROR("Cannot load music %s\n", MARK3);
	env->mark_music_cpt = 0;

	// Music win
	env->win_music = Mix_LoadMUS(WIN);
	if (env->mark_music[0] == NULL)
		ERROR("Cannot load music %s\n", WIN);

	return env;
}

/* **************************************************************** */

void render_blank(SDL_Renderer* ren, SDL_Rect* rec, bool lighted) {
	if (lighted)
		SDL_SetRenderDrawColor(ren, 255, 255, 0, SDL_ALPHA_OPAQUE);
	else
		SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
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

void render_lightbulb(SDL_Renderer* ren, SDL_Rect* rec, SDL_Texture* lightbulb_texture) {
	SDL_Rect lightbulb;
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

void render_mark(SDL_Renderer* ren, SDL_Rect* rec, bool lighted) {
	if (lighted)
		SDL_SetRenderDrawColor(ren, 255, 255, 0, SDL_ALPHA_OPAQUE);
	else
		SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
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

	if (win_h >
	    win_w) {  // sert a avoir un rapport de 1/1 pour la game et les boutons et ne pas avoir de compressions sur quelconques axes des images des boutons
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
	int size_rec = int_min(w / game_nb_cols(env->g), h / game_nb_rows(env->g));
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
	buttons.w = w / 5;
	buttons.h = h / 10 / 1.5;
	buttons.y = (h / 10 - buttons.h) / 2 + marge_h;
	buttons.x = (w / 4 - w / 5) / 2 + marge_w;
	*(env->rec_undo) = buttons;
	SDL_RenderCopy(ren, env->text_undo[env->pressed_undo ? 1 : 0], NULL, &buttons);
	buttons.x = buttons.x + buttons.w + (w / 4 - w / 5);
	*(env->rec_redo) = buttons;
	SDL_RenderCopy(ren, env->text_redo[env->pressed_redo ? 1 : 0], NULL, &buttons);
	buttons.x = buttons.x + buttons.w + (w / 4 - w / 5);
	*(env->rec_restart) = buttons;
	SDL_RenderCopy(ren, env->text_restart[env->pressed_restart ? 1 : 0], NULL, &buttons);
	buttons.x = buttons.x + buttons.w + (w / 4 - w / 5);
	*(env->rec_solve) = buttons;
	SDL_RenderCopy(ren, env->text_solve[env->pressed_solve ? 1 : 0], NULL, &buttons);
	// render cases
	for (uint i = 0; i < game_nb_rows(env->g); i++) {
		for (uint j = 0; j < game_nb_cols(env->g); j++) {
			if (game_is_lightbulb(env->g, i, j))
				render_lightbulb(ren, &rec, env->lightbulb[game_has_error(env->g, i, j) ? 1 : 0]);
			else if (game_is_black(env->g, i, j))
				render_wall(ren, env, &rec, game_get_black_number(env->g, i, j), game_has_error(env->g, i, j));
			else if (game_is_blank(env->g, i, j))
				render_blank(ren, &rec, game_is_lighted(env->g, i, j));
			else if (game_is_marked(env->g, i, j))
				render_mark(ren, &rec, game_is_lighted(env->g, i, j));
			rec.x += size_rec;
		}
		rec.x = rec_x;
		rec.y += size_rec;
	}
}

/* **************************************************************** */

void play_Music(Env* env, bool lightbulb, bool error, bool mark) {
	if (error && game_has_error_general(env->g)) {
		Mix_PlayMusic(env->err_music[env->err_music_cpt % 3], 1);
		env->err_music_cpt++;
	} else if (lightbulb) {
		Mix_PlayMusic(env->lb_music[env->lb_music_cpt % 3], 1);
		env->lb_music_cpt++;
	} else if (mark) {
		Mix_PlayMusic(env->mark_music[env->mark_music_cpt % 3], 1);
		env->mark_music_cpt++;
	}
}

void ToggleFullscreen(SDL_Window* Window) {
	Uint32 FullscreenFlag = SDL_WINDOW_FULLSCREEN;
	bool IsFullscreen = SDL_GetWindowFlags(Window) & FullscreenFlag;
	SDL_SetWindowFullscreen(Window, IsFullscreen ? 0 : FullscreenFlag);
	SDL_ShowCursor(IsFullscreen);
}

bool process(SDL_Renderer* ren, SDL_Window* win, Env* env, SDL_Event* e, SDL_Event* prec_e) {
	if (game_is_over(env->g)) {
		Mix_PlayMusic(env->win_music, 0);
	}
	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	if (e->type == SDL_QUIT) {
		return true;
	}
#ifdef _ANDROID_
	if (e->type == SDL_FINGERDOWN) {
		SDL_Point coord;
		coord.x = e->x;
		coord.y = e->y;
		if (SDL_PointInRect(&coord, env->rec_restart)) {
			env->pressed_restart = true;
			env->pressed_undo = false;
			env->pressed_redo = false;
			env->pressed_solve = false;
		} else if (SDL_PointInRect(&coord, env->rec_undo)) {
			env->pressed_undo = true;
			env->pressed_restart = false;
			env->pressed_redo = false;
			env->pressed_solve = false;
		} else if (SDL_PointInRect(&coord, env->rec_redo)) {
			env->pressed_redo = true;
			env->pressed_undo = false;
			env->pressed_restart = false;
			env->pressed_solve = false;
		} else if (SDL_PointInRect(&coord, env->rec_solve)) {
			env->pressed_solve = true;
			env->pressed_undo = false;
			env->pressed_redo = false;
			env->pressed_restart = false;
		} else if (SDL_PointInRect(&coord, env->rec_solve)) {
			env->pressed_restart = false;
			env->pressed_undo = false;
			env->pressed_redo = false;
			env->pressed_solve = false;
			if (e->type != prec_e->type) {
				prec_e->type = e->type;
				prec_e->timestamp = e->timestamp;
			}
		} else {
			env->pressed_restart = false;
			env->pressed_undo = false;
			env->pressed_redo = false;
			env->pressed_solve = false;
		}
	} else if (e->type == SDL_FINGERUP) {
		SDL_Point coord;
		coord.x = e->x;
		coord.y = e->y;
		if (SDL_PointInRect(&coord, env->rec_restart)) {
			game_restart(env->g);
		} else if (SDL_PointInRect(&coord, env->rec_undo)) {
			game_undo(env->g);
		} else if (SDL_PointInRect(&coord, env->rec_redo)) {
			game_redo(env->g);
		} else if (SDL_PointInRect(&coord, env->rec_solve)) {
			game_solve(env->g);
		} else if (SDL_PointInRect(&coord, env->rec_game)) {
			uint i = (((float)coord.y - (float)env->rec_game->y) / (float)env->rec_game->h * game_nb_rows(env->g)) -
			         0.00001 /*to avoid if clicked exacly on the bottom right corner to result a 7*/;
			uint j = (((float)coord.x - (float)env->rec_game->x) / (float)env->rec_game->w * game_nb_cols(env->g)) -
			         0.00001 /*to avoid if clicked exacly on the bottom right corner to result a 7*/;
			if ((e->timestamp - prec_e->timestamp) < 300) {
				if (game_is_blank(env->g, i, j) || game_is_marked(env->g, i, j)) {
					game_play_move(env->g, i, j, S_LIGHTBULB);
				} else if (game_is_lightbulb(env->g, i, j)) {
					game_play_move(env->g, i, j, S_BLANK);
				}
			} else {
				if (game_is_blank(env->g, i, j) || game_is_lightbulb(env->g, i, j)) {
					game_play_move(env->g, i, j, S_MARK);
				} else if (game_is_marked(env->g, i, j)) {
					game_play_move(env->g, i, j, S_BLANK);
				}
			}
		}
	}
#else
	if (prec_e != NULL) {
		prec_e = NULL;
	}
	if (e->type == SDL_MOUSEMOTION) {
		SDL_Point mouse;
		SDL_GetMouseState(&mouse.x, &mouse.y);
		if (SDL_PointInRect(&mouse, env->rec_restart)) {
			env->pressed_restart = true;
			env->pressed_undo = false;
			env->pressed_redo = false;
			env->pressed_solve = false;
		} else if (SDL_PointInRect(&mouse, env->rec_undo)) {
			env->pressed_undo = true;
			env->pressed_restart = false;
			env->pressed_redo = false;
			env->pressed_solve = false;
		} else if (SDL_PointInRect(&mouse, env->rec_redo)) {
			env->pressed_redo = true;
			env->pressed_undo = false;
			env->pressed_restart = false;
			env->pressed_solve = false;
		} else if (SDL_PointInRect(&mouse, env->rec_solve)) {
			env->pressed_solve = true;
			env->pressed_undo = false;
			env->pressed_redo = false;
			env->pressed_restart = false;
		} else {
			env->pressed_restart = false;
			env->pressed_undo = false;
			env->pressed_redo = false;
			env->pressed_solve = false;
		}
	} else if (e->type == SDL_MOUSEBUTTONDOWN) {
		SDL_Point mouse;
		SDL_GetMouseState(&mouse.x, &mouse.y);
		if (SDL_PointInRect(&mouse, env->rec_restart)) {
			game_restart(env->g);
		} else if (SDL_PointInRect(&mouse, env->rec_undo)) {
			game_undo(env->g);
		} else if (SDL_PointInRect(&mouse, env->rec_redo)) {
			game_redo(env->g);
		} else if (SDL_PointInRect(&mouse, env->rec_solve)) {
			game_solve(env->g);
		} else if (SDL_PointInRect(&mouse, env->rec_game)) {
			uint i = (((float)mouse.y - (float)env->rec_game->y) / (float)env->rec_game->h * game_nb_rows(env->g)) -
			         0.00001 /*to avoid if clicked exacly on the bottom right corner to result a 7*/;
			uint j = (((float)mouse.x - (float)env->rec_game->x) / (float)env->rec_game->w * game_nb_cols(env->g)) -
			         0.00001 /*to avoid if clicked exacly on the bottom right corner to result a 7*/;
			if (e->button.button == SDL_BUTTON_LEFT) {
				if (game_is_blank(env->g, i, j) || game_is_marked(env->g, i, j)) {
					game_play_move(env->g, i, j, S_LIGHTBULB);
					play_Music(env, true, true, false);
				} else if (game_is_lightbulb(env->g, i, j)) {
					game_play_move(env->g, i, j, S_BLANK);
					play_Music(env, true, true, false);
				}
			} else if (e->button.button == SDL_BUTTON_RIGHT) {
				if (game_is_blank(env->g, i, j) || game_is_lightbulb(env->g, i, j)) {
					game_play_move(env->g, i, j, S_MARK);
					play_Music(env, false, true, true);
				} else if (game_is_marked(env->g, i, j)) {
					game_play_move(env->g, i, j, S_BLANK);
					play_Music(env, false, true, true);
				}
			}
		}
	} else if (e->type == SDL_KEYDOWN) {
		const Uint8* state = SDL_GetKeyboardState(NULL);
		if (state[SDL_SCANCODE_F11])
			ToggleFullscreen(win);
	} else if (e->type == SDL_WINDOWEVENT) {
		render_blended_text(ren, env);
	}
#endif
	return false;
}

/* **************************************************************** */

void clean(Env* env) {
	game_delete(env->g);
	SDL_DestroyTexture(env->zero[0]);
	SDL_DestroyTexture(env->zero[1]);
	free(env->zero);
	SDL_DestroyTexture(env->one[0]);
	SDL_DestroyTexture(env->one[1]);
	free(env->one);
	SDL_DestroyTexture(env->two[0]);
	SDL_DestroyTexture(env->two[1]);
	free(env->two);
	SDL_DestroyTexture(env->three[0]);
	SDL_DestroyTexture(env->three[1]);
	free(env->three);
	SDL_DestroyTexture(env->four[0]);
	SDL_DestroyTexture(env->four[1]);
	free(env->four);
	SDL_DestroyTexture(env->lightbulb[0]);
	SDL_DestroyTexture(env->lightbulb[1]);
	free(env->lightbulb);
	SDL_DestroyTexture(env->text_restart[0]);
	SDL_DestroyTexture(env->text_restart[1]);
	free(env->text_restart);
	SDL_DestroyTexture(env->text_undo[0]);
	SDL_DestroyTexture(env->text_undo[1]);
	free(env->text_undo);
	SDL_DestroyTexture(env->text_redo[0]);
	SDL_DestroyTexture(env->text_redo[1]);
	free(env->text_redo);
	SDL_DestroyTexture(env->text_solve[0]);
	SDL_DestroyTexture(env->text_solve[1]);
	free(env->text_solve);
	free(env->rec_redo);
	free(env->rec_undo);
	free(env->rec_restart);
	free(env->rec_solve);
	free(env->rec_game);
	for (uint i = 0; i < 3; i++) {
		Mix_FreeMusic(env->lb_music[i]);
	}
	free(env->lb_music);
	free(env);
}

/* **************************************************************** */
