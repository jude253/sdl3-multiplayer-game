#include "prototype.h"

static char debug_string[32];
static SDL_FPoint points[500];
static SDL_Texture *texture = NULL;
static int texture_width = 0;
static int texture_height = 0;

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480

static void draw(SDL_Renderer *renderer)
{
    const double now = ((double)SDL_GetTicks()) / 1000.0; /* convert from milliseconds to seconds. */
    /* choose the color for the frame we will draw. The sine wave trick makes it fade between colors smoothly. */
    const float red = (float)(0.5 + 0.5 * SDL_sin(now));
    const float green = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 2 / 3));
    const float blue = (float)(0.5 + 0.5 * SDL_sin(now + SDL_PI_D * 4 / 3));
    SDL_SetRenderDrawColorFloat(renderer, red, green, blue, SDL_ALPHA_OPAQUE_FLOAT); /* new color, full alpha. */

    /* clear the window to the draw color. */
    SDL_RenderClear(renderer);

    /* draw a filled rectangle in the middle of the canvas. */
    SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE); /* blue, full alpha */
    SDL_FRect rect;
    rect.x = rect.y = 100;
    rect.w = 440;
    rect.h = 280;
    SDL_RenderFillRect(renderer, &rect);

    /* draw some points across the canvas. */
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE); /* red, full alpha */
    SDL_RenderPoints(renderer, points, SDL_arraysize(points));

    /* draw a unfilled rectangle in-set a little bit. */
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE); /* green, full alpha */
    rect.x += 30;
    rect.y += 30;
    rect.w -= 60;
    rect.h -= 60;
    SDL_RenderRect(renderer, &rect);

    /* draw two lines in an X across the whole canvas. */
    SDL_SetRenderDrawColor(renderer, 255, 255, 0, SDL_ALPHA_OPAQUE); /* yellow, full alpha */
    SDL_RenderLine(renderer, 0, 0, 640, 480);
    SDL_RenderLine(renderer, 0, 480, 640, 0);

    /* Just draw the static texture a few times. You can think of it like a
       stamp, there isn't a limit to the number of times you can draw with it. */
    SDL_FRect dst_rect;
    const Uint64 nowMs = SDL_GetTicks();
    const float direction = ((nowMs % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float)(((int)(nowMs % 1000)) - 500) / 500.0f) * direction;

    /* top left */
    dst_rect.x = (100.0f * scale);
    dst_rect.y = 0.0f;
    dst_rect.w = (float)texture_width;
    dst_rect.h = (float)texture_height;
    SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    /* center this one. */
    dst_rect.x = ((float)(WINDOW_WIDTH - texture_width)) / 2.0f;
    dst_rect.y = ((float)(WINDOW_HEIGHT - texture_height)) / 2.0f;
    dst_rect.w = (float)texture_width;
    dst_rect.h = (float)texture_height;
    SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    /* bottom right. */
    dst_rect.x = ((float)(WINDOW_WIDTH - texture_width)) - (100.0f * scale);
    dst_rect.y = (float)(WINDOW_HEIGHT - texture_height);
    dst_rect.w = (float)texture_width;
    dst_rect.h = (float)texture_height;
    SDL_RenderTexture(renderer, texture, NULL, &dst_rect);

    /* Draw FPS */
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDebugText(renderer, 5, 5, debug_string);

    /* put the newly-cleared rendering on the screen. */
    SDL_RenderPresent(renderer);
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

    /* set up some random points */
    for (int i = 0; i < SDL_arraysize(points); i++)
    {
        points[i].x = (SDL_randf() * 440.0f) + 100.0f;
        points[i].y = (SDL_randf() * 280.0f) + 100.0f;
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

    draw(as->renderer);
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