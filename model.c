// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include "model.h"
#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_tools.h"

/* **************************************************************** */

#define RESTART_UP "restart_up.png"
#define RESTART_DOWN "restart_down.png"
#define UNDO_UP "undo_up.png"
#define UNDO_DOWN "undo_down.png"
#define REDO_UP "redo_up.png"
#define REDO_DOWN "redo_down.png"
#define SOLVE_UP "solve_up.png"
#define SOLVE_DOWN "solve_down.png"
#define LIGHTBULB_WHITE "lightbulb_white.png"
#define LIGHTBULB_RED "lightbulb_red.png"
#define FONT "arial.ttf"

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
		ERROR("[ERROR] malloc\n");
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
};

/* **************************************************************** */
void usage(char* argv[]) {
	PRINT("Usage : %s            => Joue la partie par défaut\n", argv[0]);
	PRINT("        %s <filename> => Charge la partie sur le fichier <filename>\n", argv[0]);
	exit(EXIT_FAILURE);
}

Env* init(SDL_Renderer* ren, int argc, char* argv[]) {
	Env* env = malloc(sizeof(struct Env_t));
	env->pressed_restart = false;
	env->pressed_undo = false;
	env->pressed_redo = false;
	env->pressed_solve = false;
	PRINT("To win the game, you must satisfy the following conditions:\n\n");
	PRINT(
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

	/* init zero texture double tab*/
	env->zero = malloc(sizeof(SDL_Texture*) * 2);
	if (env->zero == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");
	// if the case has not error
	SDL_Surface* surf = TTF_RenderText_Blended(font, "0", color_w);
	env->zero[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	// if the case has error
	surf = TTF_RenderText_Blended(font, "0", color_r);
	env->zero[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/* init one texture double tab*/
	env->one = malloc(sizeof(SDL_Texture*) * 2);
	if (env->one == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");
	// if the case has not error
	surf = TTF_RenderText_Blended(font, "1", color_w);
	env->one[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	// if the case has error
	surf = TTF_RenderText_Blended(font, "1", color_r);
	env->one[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/* init two texture double tab*/
	env->two = malloc(sizeof(SDL_Texture*) * 2);
	if (env->two == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");
	// if the case has not error
	surf = TTF_RenderText_Blended(font, "2", color_w);
	env->two[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	// if the case has error
	surf = TTF_RenderText_Blended(font, "2", color_r);
	env->two[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/* init three texture double tab*/
	env->three = malloc(sizeof(SDL_Texture*) * 2);
	if (env->three == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");
	// if the case has not error
	surf = TTF_RenderText_Blended(font, "3", color_w);
	env->three[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	// if the case has error
	surf = TTF_RenderText_Blended(font, "3", color_r);
	env->three[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/* init four texture double tab*/
	env->four = malloc(sizeof(SDL_Texture*) * 2);
	if (env->four == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");
	// if the case has not error
	surf = TTF_RenderText_Blended(font, "4", color_w);
	env->four[0] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);
	// if the case has error
	surf = TTF_RenderText_Blended(font, "4", color_r);
	env->four[1] = SDL_CreateTextureFromSurface(ren, surf);
	SDL_FreeSurface(surf);

	/* init lightbulb texture double tab*/
	env->lightbulb = malloc(sizeof(SDL_Texture*) * 2);
	if (env->lightbulb == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");
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
		ERROR("NOT ENOUGTH MEMORY\n");
	// if the case has not error
	env->text_restart[0] = IMG_LoadTexture(ren, RESTART_UP);
	if (!env->text_restart[0])
		ERROR("IMG_LoadTexture: %s\n", RESTART_UP);
	// if the case has error
	env->text_restart[1] = IMG_LoadTexture(ren, RESTART_DOWN);
	if (!env->text_restart[1])
		ERROR("IMG_LoadTexture: %s\n", RESTART_DOWN);

	/* init text_undo texture double tab*/
	env->text_undo = malloc(sizeof(SDL_Texture*) * 2);
	if (env->text_undo == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");
	// if the case has not error
	env->text_undo[0] = IMG_LoadTexture(ren, UNDO_UP);
	if (!env->text_undo[0])
		ERROR("IMG_LoadTexture: %s\n", UNDO_UP);
	// if the case has error
	env->text_undo[1] = IMG_LoadTexture(ren, UNDO_DOWN);
	if (!env->text_undo[1])
		ERROR("IMG_LoadTexture: %s\n", UNDO_DOWN);

	/* init text_redo texture double tab*/
	env->text_redo = malloc(sizeof(SDL_Texture*) * 2);
	if (env->text_redo == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");
	// if the case has not error
	env->text_redo[0] = IMG_LoadTexture(ren, REDO_UP);
	if (!env->text_redo[0])
		ERROR("IMG_LoadTexture: %s\n", REDO_UP);
	// if the case has error
	env->text_redo[1] = IMG_LoadTexture(ren, REDO_DOWN);
	if (!env->text_redo[1])
		ERROR("IMG_LoadTexture: %s\n", UNDO_DOWN);

	/* init text_solve texture double tab*/
	env->text_solve = malloc(sizeof(SDL_Texture*) * 2);
	if (env->text_solve == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");
	// if the case has not error
	env->text_solve[0] = IMG_LoadTexture(ren, SOLVE_UP);
	if (!env->text_solve[0])
		ERROR("IMG_LoadTexture: %s\n", SOLVE_UP);
	// if the case has error
	env->text_solve[1] = IMG_LoadTexture(ren, SOLVE_DOWN);
	if (!env->text_solve[1])
		ERROR("IMG_LoadTexture: %s\n", SOLVE_DOWN);

	env->rec_redo = malloc(sizeof(SDL_Rect));
	if (env->rec_redo == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");

	env->rec_game = malloc(sizeof(SDL_Rect));
	if (env->rec_game == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");

	env->rec_restart = malloc(sizeof(SDL_Rect));
	if (env->rec_restart == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");

	env->rec_solve = malloc(sizeof(SDL_Rect));
	if (env->rec_solve == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");

	env->rec_undo = malloc(sizeof(SDL_Rect));
	if (env->rec_undo == NULL)
		ERROR("NOT ENOUGTH MEMORY\n");

	TTF_CloseFont(font);
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
	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	h = h - h / 10;
	// set background color
	SDL_SetRenderDrawColor(ren, 24, 26, 27, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(ren);
	SDL_Rect rec;
	int size_rec = int_min(w / game_nb_cols(env->g), h / game_nb_rows(env->g));
	rec.h = size_rec;
	rec.w = size_rec;
	int rec_x = w / 2 - size_rec * game_nb_cols(env->g) / 2;
	int rec_y = h / 2 - size_rec * game_nb_rows(env->g) / 2 + h / 10;
	rec.x = rec_x;
	rec.y = rec_y;
	env->rec_game->x = rec.x;
	env->rec_game->y = rec.y;
	env->rec_game->h = size_rec * game_nb_rows(env->g);
	env->rec_game->w = size_rec * game_nb_cols(env->g);
	SDL_Rect buttons;
	buttons.w = w / 5;
	buttons.h = h / 10 / 1.5;
	buttons.y = (h / 10 - buttons.h) / 2;
	buttons.x = (w / 4 - w / 5) / 2;
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

bool process(SDL_Window* win, Env* env, SDL_Event* e) {
	int w, h;
	SDL_GetWindowSize(win, &w, &h);
	if (e->type == SDL_QUIT) {
		return true;
	}
#ifdef _ANDROID_
#else
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
				} else if (game_is_lightbulb(env->g, i, j)) {
					game_play_move(env->g, i, j, S_BLANK);
				}
			}
			if (e->button.button == SDL_BUTTON_RIGHT) {
				if (game_is_blank(env->g, i, j) || game_is_lightbulb(env->g, i, j)) {
					game_play_move(env->g, i, j, S_MARK);
				} else if (game_is_marked(env->g, i, j)) {
					game_play_move(env->g, i, j, S_BLANK);
				}
			}
		}
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
	free(env);
}

/* **************************************************************** */
