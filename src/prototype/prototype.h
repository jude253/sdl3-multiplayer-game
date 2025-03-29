/*
Enforce prototypes to define these
*/
#ifndef __PROTOTYPE_H
#define __PROTOTYPE_H

#define SDL_MAIN_USE_CALLBACKS
#include <SDL3/SDL_main.h>
#include <SDL3/SDL.h>

typedef struct {
    int8_t w;
    int8_t a;
    int8_t s;
    int8_t d;
    int8_t space;
} ButtonPressed;

typedef struct {
    int64_t pos[2];
    int64_t vel[2];
    unsigned char color[3];
    ButtonPressed buttonPressed;
} Player;

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    Player player;
} AppState;

SDL_AppResult SDL_AppInit(void **appstate, int argc, char **argv);
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event);
SDL_AppResult SDL_AppIterate(void *appstate);
void SDL_AppQuit(void *appstate, SDL_AppResult result);

#endif // __PROTOTYPE_H