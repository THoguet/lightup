// SDL2 Demo by aurelien.esnard@u-bordeaux.fr

#include "model.h"
#include <SDL.h>
#include <SDL_image.h>  // required to load transparent texture from PNG
#include <SDL_ttf.h>    // required to use TTF fonts
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

/* **************************************************************** */

struct Env_t {
	SDL_Texture* but_new_game_up;
	SDL_Texture* but_new_game_down;
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

	/* PUT YOUR CODE HERE TO INIT TEXTURES, ... */

	return env;
}

/* **************************************************************** */

void render(SDL_Window* win, SDL_Renderer* ren, Env* env) { /* PUT YOUR CODE HERE TO RENDER TEXTURES, ... */
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

	else if (e->type == SDL_WINDOWEVENT){
		init(win, ren , w, h/*(w, h) a changer*/);
	}
	else if (e->type == SDL_SDL_MOUSEMOTION){
		SDL_Point mouse;
		SDL_GetMouseState(&mouse.x, &mouse.y);
	}
	else if (e->type == SDL_MOUSEBUTTONDOWN){
		
	}
#endif
	return false;
}

/* **************************************************************** */

void clean(SDL_Window* win, SDL_Renderer* ren, Env* env) {
	/* PUT YOUR CODE HERE TO CLEAN MEMORY */

	free(env);
}

/* **************************************************************** */
