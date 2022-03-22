// SDL2 Model by aurelien.esnard@u-bordeaux.fr

#ifndef MODEL_H
#define MODEL_H

#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>

typedef struct Env_t Env;

/* **************************************************************** */

#ifdef __ANDROID__
#define PRINT(STR, ...)              \
	do {                             \
		SDL_Log(STR, ##__VA_ARGS__); \
	} while (0)
#define ERROR(STR, ...)              \
	do {                             \
		SDL_Log(STR, ##__VA_ARGS__); \
		exit(EXIT_FAILURE);          \
	} while (0)
#else
#define PRINT(STR, ...)             \
	do {                            \
		printf(STR, ##__VA_ARGS__); \
	} while (0)
#define ERROR(STR, ...)                      \
	do {                                     \
		fprintf(stderr, STR, ##__VA_ARGS__); \
		exit(EXIT_FAILURE);                  \
	} while (0)
#endif

/* **************************************************************** */

#define APP_NAME "SDL2 Demo"
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
#define DELAY 100

/* **************************************************************** */

Env* init(SDL_Renderer* ren, int argc, char* argv[]);
void render(SDL_Window* win, SDL_Renderer* ren, Env* env);
void clean(Env* env);
bool process(SDL_Renderer* ren, SDL_Window* win, Env* env, SDL_Event* e, SDL_Event* prec_e);
void render_blended_numbered_wall(SDL_Renderer* ren, Env* env);

/* **************************************************************** */

#endif
