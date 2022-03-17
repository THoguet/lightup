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

struct Env_t {
	game g;
	SDL_Texture** zero;  // array of two texture one white and one red for each number (red color = r:255 g:50 b:50)
	SDL_Texture** one;
	SDL_Texture** two;
	SDL_Texture** three;
	SDL_Texture** four;
	SDL_Texture** lightbulb;  // array of the two lightbulb images first white second red
	SDL_Texture* but_restart_up;
	SDL_Texture* but_restart_down;
	SDL_Texture* but_undo_up;
	SDL_Texture* but_undo_down;
	SDL_Texture* but_redo_up;
	SDL_Texture* but_redo_down;
	SDL_Texture* but_solve_up;
	SDL_Texture* but_solve_down;
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

	/* init but_restart_up texture */
	env->but_restart_up = SDL_LoadTexture(ren, env->but_restart_up);
	if (!env->but_restart_up)
		ERROR("SDL_LoadTexture: %s\n", env->but_restart_up);

	/* init but_restart_down texture */
	env->but_restart_down = SDL_LoadTexture(ren, env->but_restart_down);
	if (!env->but_restart_down)
		ERROR("SDL_LoadTexture: %s\n", env->but_restart_down);

	/* init but_undo_up texture */
	env->but_undo_up = SDL_LoadTexture(ren, env->but_undo_up);
	if (!env->but_undo_up)
		ERROR("SDL_LoadTexture: %s\n", env->but_undo_up);

	/* init but_undo_down texture */
	env->but_undo_down = SDL_LoadTexture(ren, env->but_undo_down);
	if (!env->but_undo_down)
		ERROR("SDL_LoadTexture: %s\n", env->but_undo_down);

	/* init but_redo_up texture */
	env->but_redo_up = SDL_LoadTexture(ren, env->but_redo_up);
	if (!env->but_redo_up)
		ERROR("SDL_LoadTexture: %s\n", env->but_redo_up);

	/* init but_redo_down texture */
	env->but_redo_down = SDL_LoadTexture(ren, env->but_redo_down);
	if (!env->but_redo_down)
		ERROR("SDL_LoadTexture: %s\n", env->but_redo_down);

	/* init but_solve_up texture */
	env->but_solve_up = SDL_LoadTexture(ren, env->but_solve_up);
	if (!env->but_solve_up)
		ERROR("SDL_LoadTexture: %s\n", env->but_solve_up);

	/* init but_solve_down texture */
	env->but_solve_down = SDL_LoadTexture(ren, env->but_solve_down);
	if (!env->but_solve_down)
		ERROR("SDL_LoadTexture: %s\n", env->but_solve_down);

	return env;
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
		init(win, ren, w, h /*(w, h) a changer*/);
	} else if (e->type == SDL_MOUSEMOTION) {
		SDL_Point mouse;
		SDL_GetMouseState(&mouse.x, &mouse.y);
	} else if (e->type == SDL_MOUSEBUTTONDOWN) {
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
	SDL_DestroyTexture(env->but_restart_up);
	SDL_DestroyTexture(env->but_restart_down);
	SDL_DestroyTexture(env->but_undo_up);
	SDL_DestroyTexture(env->but_undo_down);
	SDL_DestroyTexture(env->but_redo_up);
	SDL_DestroyTexture(env->but_redo_down);
	SDL_DestroyTexture(env->but_solve_up);
	SDL_DestroyTexture(env->but_solve_down);

	free(env);
}

/* **************************************************************** */
