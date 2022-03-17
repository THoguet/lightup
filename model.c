// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include "model.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>  // required to load transparent texture from PNG
#include <SDL2/SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include "game.h"
#include "game_aux.h"
#include "game_ext.h"
#include "game_private.h"
#include "game_tools.h"

/* **************************************************************** */

#define RESTART_UP "../img/restart_up.png"
#define RESTART_DOWN "../img/restart_down.png"
#define UNDO_UP "../img/undo_up.png"
#define UNDO_DOWN "../img/undo_down.png"
#define REDO_UP "../img/redo_up.png"
#define REDO_DOWN "../img/redo_down.png"
#define SOLVE_UP "../img/solve_up.png"
#define SOLVE_DOWN "../img/solve_down.png"

/* **************************************************************** */

struct Env_t {
	game g;
	SDL_Texture** zero;  // array of two texture one white and one red for each number (red color = r:255 g:50 b:50)
	SDL_Texture** one;
	SDL_Texture** two;
	SDL_Texture** three;
	SDL_Texture** four;
	SDL_Texture** lightbulb;  // array of the two lightbulb images first white second red
	SDL_Texture* text_restart;
	SDL_Texture* text_undo;
	SDL_Texture* text_redo;
	SDL_Texture* text_solve;
	bool pressed_restart;
	bool pressed_undo;
	bool pressed_redo;
	bool pressed_solve;
	SDL_Rect* rec_redo;  // rectangle of each buttons
	SDL_Rect* rec_undo;
	SDL_Rect* rec_restart;
	SDL_Rect* rec_solve;
	SDL_Rect* rec_game;  // rectangle of the grid
	uint size_game;      // contain the number of squares in the game
};

/* **************************************************************** */

Env* init(SDL_Window* win, SDL_Renderer* ren, int argc, char* argv[]) {
	Env* env = malloc(sizeof(struct Env_t));

	PRINT("To win the game, you must satisfy the following conditions:\n");
	PRINT(
	    "All non-black squares are lit.\nNo light is lit by another light.\nEach numbered black square must be orthogonally adjacent to exactly the given "
	    "number of lights.\nNon-numbered black squares may have any number of lights adjacent to them.\n");
	/* get current window size */
	int w, h;
	SDL_GetWindowSize(win, &w, &h);

	/* Select the color for drawing. It is set to red here. */
	// SDL_SetRenderDrawColor(ren, 255, 255, 255, 255);

	/* init game texture */
	env->g = SDL_LoadTexture(ren, env->g);
	if (!env->g)
		ERROR("SDL_LoadTexture: %s\n", env->g);

	/* init zero texture */
	env->zero = SDL_LoadTexture(ren, env->zero);
	if (!env->zero)
		ERROR("SDL_LoadTexture: %s\n", env->zero);

	/* init one texture */
	env->one = SDL_LoadTexture(ren, env->one);
	if (!env->one)
		ERROR("SDL_LoadTexture: %s\n", env->one);

	/* init two texture */
	env->two = SDL_LoadTexture(ren, env->two);
	if (!env->two)
		ERROR("SDL_LoadTexture: %s\n", env->two);

	/* init three texture */
	env->three = SDL_LoadTexture(ren, env->three);
	if (!env->three)
		ERROR("SDL_LoadTexture: %s\n", env->three);

	/* init four texture */
	env->four = SDL_LoadTexture(ren, env->four);
	if (!env->four)
		ERROR("SDL_LoadTexture: %s\n", env->four);

	/* init lightbulb texture */
	env->lightbulb = SDL_LoadTexture(ren, env->lightbulb);
	if (!env->lightbulb)
		ERROR("SDL_LoadTexture: %s\n", env->lightbulb);

	/* init text_restart texture */
	env->text_restart = SDL_LoadTexture(ren, env->text_restart);
	if (!env->text_restart)
		ERROR("SDL_LoadTexture: %s\n", env->text_restart);

	/* init text_undo texture */
	env->text_undo = SDL_LoadTexture(ren, env->text_undo);
	if (!env->text_undo)
		ERROR("SDL_LoadTexture: %s\n", env->text_undo);

	/* init text_solve texture */
	env->text_solve = SDL_LoadTexture(ren, env->text_solve);
	if (!env->text_solve)
		ERROR("SDL_LoadTexture: %s\n", env->text_solve);

	/* init but_undo_down texture */
	// env->but_undo_down = SDL_LoadTexture(ren, env->but_undo_down);
	// if (!env->but_undo_down)
	//	ERROR("SDL_LoadTexture: %s\n", env->but_undo_down);

	/* init but_redo_up texture */
	// env->but_redo_up = SDL_LoadTexture(ren, env->but_redo_up);
	// if (!env->but_redo_up)
	//	ERROR("SDL_LoadTexture: %s\n", env->but_redo_up);

	/* init but_redo_down texture */
	// env->but_redo_down = SDL_LoadTexture(ren, env->but_redo_down);
	// if (!env->but_redo_down)
	//	ERROR("SDL_LoadTexture: %s\n", env->but_redo_down);

	/* init but_solve_up texture */
	// env->but_solve_up = SDL_LoadTexture(ren, env->but_solve_up);
	// if (!env->but_solve_up)
	//	ERROR("SDL_LoadTexture: %s\n", env->but_solve_up);

	/* init but_solve_down texture */
	// env->but_solve_down = SDL_LoadTexture(ren, env->but_solve_down);
	// if (!env->but_solve_down)
	//	ERROR("SDL_LoadTexture: %s\n", env->but_solve_down);

	// return env;
}

/* **************************************************************** */

void render_blank(SDL_Renderer* ren, SDL_Rect* rec, bool lighted) {
	if (lighted) {
		SDL_SetRenderDrawColor(ren, 255, 255, 0, SDL_ALPHA_OPAQUE);
		SDL_RenderFillRect(ren, rec);
	}
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
			number.x = rec->x + rec->w / 2 - number.x / 2;
			number.y = rec->y + rec->h / 2 - number.y / 2;
			SDL_RenderCopy(ren, env->zero[error ? 1 : 0], NULL, &number);
			break;
		case 1:
			SDL_QueryTexture(env->one[error ? 1 : 0], NULL, NULL, &number.w, &number.h);
			number.x = rec->x + rec->w / 2 - number.x / 2;
			number.y = rec->y + rec->h / 2 - number.y / 2;
			SDL_RenderCopy(ren, env->one[error ? 1 : 0], NULL, &number);
			break;
		case 2:
			SDL_QueryTexture(env->two[error ? 1 : 0], NULL, NULL, &number.w, &number.h);
			number.x = rec->x + rec->w / 2 - number.x / 2;
			number.y = rec->y + rec->h / 2 - number.y / 2;
			SDL_RenderCopy(ren, env->two[error ? 1 : 0], NULL, &number);
			break;
		case 3:
			SDL_QueryTexture(env->three[error ? 1 : 0], NULL, NULL, &number.w, &number.h);
			number.x = rec->x + rec->w / 2 - number.x / 2;
			number.y = rec->y + rec->h / 2 - number.y / 2;
			SDL_RenderCopy(ren, env->three[error ? 1 : 0], NULL, &number);
			break;
		case 4:
			SDL_QueryTexture(env->four[error ? 1 : 0], NULL, NULL, &number.w, &number.h);
			number.x = rec->x + rec->w / 2 - number.x / 2;
			number.y = rec->y + rec->h / 2 - number.y / 2;
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
	lightbulb.h = rec->h - rec->h / 10;
	lightbulb.w = rec->w - rec->w / 10;
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
	// set background color
	SDL_SetRenderDrawColor(ren, 255, 255, 255, SDL_ALPHA_OPAQUE);
	SDL_RenderClear(ren);
	SDL_Rect rec;
	int size_rec = int_min(w / game_nb_cols(env->g), h / game_nb_rows(env->g));
	rec.h = size_rec;
	rec.w = size_rec;
	int rec_x = w / 2 - size_rec * game_nb_cols(env->g) / 2;
	int rec_y = h / 2 - size_rec * game_nb_rows(env->g) / 2;
	rec.x = rec_x;
	rec.y = rec_y;
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
		}
	}
	rec.x = rec_x;
	rec.y += size_rec;
}

/* **************************************************************** */

bool process(SDL_Window* win, SDL_Renderer* ren, Env* env, SDL_Event* e) {
	int w, h;
	SDL_GetWindowSize(win, &w, &h);

	if (e->type == SDL_QUIT) {
		return true;
	}

#ifdef _ANDROID_
#else

	else if (e->type == SDL_WINDOWEVENT) {
		/*a changer en rapport avec le placement des boutons*/
	} else if (e->type == SDL_MOUSEMOTION) {
		SDL_Point mouse;
		SDL_GetMouseState(&mouse.x, &mouse.y);
		if (SDL_PointInRect(&mouse, env->rec_restart)) {
			env->pressed_restart = true;
		} else if (SDL_PointInRect(&mouse, env->rec_undo)) {
			env->pressed_undo = true;
		} else if (SDL_PointInRect(&mouse, env->rec_redo)) {
			env->pressed_redo;
		} else if (SDL_PointInRect(&mouse, env->rec_solve)) {
			env->pressed_solve = true;
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
			uint i = mouse.y - ((h - (env->rec_game->y)) / 2) / (env->g->height);
			uint j = mouse.x - ((w - (env->rec_game->x)) / 2) / (env->g->width);
			if (game_is_blank(env->g, i, j)) {
				game_play_move(env->g, i, j, S_LIGHTBULB);
			} else if (game_is_lightbulb(env->g, i, j)) {
				game_play_move(env->g, i, j, S_BLANK);
			}
		}
	}
#endif
	return false;
}

/* **************************************************************** */

void clean(SDL_Window* win, SDL_Renderer* ren, Env* env) {
	SDL_DestroyTexture(env->zero);
	SDL_DestroyTexture(env->one);
	SDL_DestroyTexture(env->two);
	SDL_DestroyTexture(env->three);
	SDL_DestroyTexture(env->four);
	SDL_DestroyTexture(env->lightbulb);
	SDL_DestroyTexture(env->text_restart);
	SDL_DestroyTexture(env->text_undo);
	SDL_DestroyTexture(env->text_redo);
	SDL_DestroyTexture(env->text_solve);
	/*SDL_DestroyTexture(env->but_redo_up);
	SDL_DestroyTexture(env->but_redo_down);
	SDL_DestroyTexture(env->but_solve_up);
	SDL_DestroyTexture(env->but_solve_down);*/

	free(env);
}

/* **************************************************************** */
