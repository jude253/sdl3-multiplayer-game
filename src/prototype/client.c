#include "prototype.h"

static char debug_string[32];

SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[])
{
    AppState *as = SDL_calloc(1, sizeof(AppState));
    if (!as)
    {
        return SDL_APP_FAILURE;
    }
    else
    {
        *appstate = as;
    }

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        return SDL_APP_FAILURE;
    }
    if (!SDL_CreateWindowAndRenderer("client", 640, 480, 0, &as->window, &as->renderer))
    {
        return SDL_APP_FAILURE;
    }

    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event)
{
    AppState *as = appstate;
    switch (event->type)
    {
    case SDL_EVENT_QUIT:
        return SDL_APP_SUCCESS;
        break;
    }
    return SDL_APP_CONTINUE;
}

SDL_AppResult SDL_AppIterate(void *appstate)
{
    AppState *as = appstate;
    static Uint64 accu = 0;
    static Uint64 last = 0;
    static Uint64 past = 0;
    Uint64 now = SDL_GetTicksNS();
    Uint64 dt_ns = now - past;

    /*
    Not exactly sure what this does, but thinking its to prevent issues
    with roll over. 
    */
    if (now - last > 999999999) {
        last = now;
        SDL_snprintf(debug_string, sizeof(debug_string), "%" SDL_PRIu64 " fps", accu);
        accu = 0;
    }
    past = now;
    accu += 1;
    Uint64 elapsed = SDL_GetTicksNS() - now;
    if (elapsed < 999999)
    {
        SDL_DelayNS(999999 - elapsed);
    }
    return SDL_APP_CONTINUE;
}

void SDL_AppQuit(void *appstate, SDL_AppResult result)
{
    SDL_free(appstate); // just free the memory, SDL will clean up the window/renderer for us.
}