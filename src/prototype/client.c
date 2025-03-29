#include "prototype.h"
#include <SDL3/SDL_render.h>
#include <SDL3/SDL_surface.h>
#include <stdbool.h>

static char debug_string[32];
static SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

static void draw(AppState *as)
{
    const double now = ((double)SDL_GetTicks()) / 1000.0; /* convert from milliseconds to seconds. */
    /* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
    const float red = (float)(0.5 + 0.5 * SDL_sin(now));
    const float green = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    const float blue = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
    SDL_SetRenderDrawColorFloat(as->renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT); /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(as->renderer);

    /* Just draw the static texture a few times. You can think of it like a
       stamp, there isn't a limit to the number of times you can draw with it. */
    const Uint64 nowMs = SDL_GetTicks();

    /* Draw Player */
    Player *player = &as->player;
    SDL_SetRenderDrawColor(as->renderer, player->color[0], player->color[1], player->color[2], 255);
    SDL_FRect playerRect = {
        .x = player->pos[0],
        .y = player->pos[1],
        .w = 50,
        .h = 100,
    };
    SDL_RenderFillRect(as->renderer, &playerRect);
    SDL_RenderPoint(as->renderer, player->pos[0], player->pos[1]);

    /* Draw FPS */
    SDL_SetRenderDrawColor(as->renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(as->renderer, 5, 5, debug_string);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(as->renderer);
}

static void update(AppState *as, Uint64 dt_ns)
{
    Player *player = &as->player;

    // Not sure how to use the time to keep frame rate constant yet
    double time = (double)dt_ns * 1e-9;
    double dirX = player->buttonPressed.d - player->buttonPressed.a;
    double dirY = player->buttonPressed.s - player->buttonPressed.w;

    double velX = player->vel[0];
    double velY = player->vel[1];

    double posX = (player->pos[0] + dirX * velX * time);
    double posY = (player->pos[1] + dirY * velY * time);

    player->pos[0] = posX;
    player->pos[1] = posY;
    // printf("(posX, posY): (%d, %d)\n", posX, posY);
}

static void player_init(Player *player)
{
    player->pos[0] = 100;
    player->pos[1] = 100;
    player->vel[0] = 1000; // Found by trial and error with dt
    player->vel[1] = 1000; // Found by trial and error with dt
    player->color[0] = 0;
    player->color[1] = 0;
    player->color[2] = 0;
}

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
    if (!SDL_CreateWindowAndRenderer("client", WINDOW_WIDTH, WINDOW_HEIGHT, 0, &as->window, &as->renderer))
    {
        return SDL_APP_FAILURE;
    }

    SDL_Surface *surface = NULL;
    char *bmp_path = NULL;

    SDL_asprintf(&bmp_path, "%sasset/image/sample.bmp", SDL_GetBasePath()); /* allocate a string of the full file path */
    surface = SDL_LoadBMP(bmp_path);
    if (!surface)
    {
        SDL_Log("Couldn't load bitmap: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_free(bmp_path); /* done with this, the file is loaded. */

    texture_width = surface->w;
    texture_height = surface->h;

    texture = SDL_CreateTextureFromSurface(as->renderer, surface);
    if (!texture)
    {
        SDL_Log("Couldn't create static texture: %s", SDL_GetError());
        return SDL_APP_FAILURE;
    }

    SDL_DestroySurface(surface); /* done with this, the texture has a copy of the pixels now. */
    SDL_SetDefaultTextureScaleMode(as->renderer, SDL_SCALEMODE_NEAREST);

    player_init(&as->player);
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
    case SDL_EVENT_KEY_DOWN:
    {
        SDL_Keycode sym = event->key.key;
        // printf("SDL_EVENT_KEY_DOWN: %d\n", sym);
        if (sym == SDLK_W)
            as->player.buttonPressed.w = 1;
        if (sym == SDLK_A)
            as->player.buttonPressed.a = 1;
        if (sym == SDLK_S)
            as->player.buttonPressed.s = 1;
        if (sym == SDLK_D)
            as->player.buttonPressed.d = 1;
        if (sym == SDLK_SPACE)
            as->player.buttonPressed.space = 1;
        break;
    }
    case SDL_EVENT_KEY_UP:
    {
        SDL_Keycode sym = event->key.key;
        // printf("SDL_EVENT_KEY_UP: %d\n", sym);
        if (sym == SDLK_W)
            as->player.buttonPressed.w = 0;
        if (sym == SDLK_A)
            as->player.buttonPressed.a = 0;
        if (sym == SDLK_S)
            as->player.buttonPressed.s = 0;
        if (sym == SDLK_D)
            as->player.buttonPressed.d = 0;
        if (sym == SDLK_SPACE)
            as->player.buttonPressed.space = 0;
        break;
    }
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

    update(as, dt_ns);
    draw(as);
    /*
    Not exactly sure what this does, but thinking its to prevent issues
    with roll over.
    */
    if (now - last > 999999999)
    {
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