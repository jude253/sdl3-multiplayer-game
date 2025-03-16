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

    /* Just draw the static texture a few times. You can think of it like a
       stamp, there isn't a limit to the number of times you can draw with it. */
    const Uint64 nowMs = SDL_GetTicks();

    /* we'll have the triangle grow and shrink over a few seconds. */
    const float direction = ((nowMs % 2000) >= 1000) ? 1.0f : -1.0f;
    const float scale = ((float)(((int)(nowMs % 1000)) - 500) / 500.0f) * direction;
    const float size = 200.0f + (200.0f * scale);

    // Add some affine stretch thing. Can maybe do this with Triangle?
    // or directly onto a wall/floor?  Having trouble understanding it
    // fully at the moment.  Why are there only 3 coordinates to specify?
    // Where would the 4th coordinate end up going?
    SDL_FPoint origin = {.x = 0.0f, .y = 200.0f};
    SDL_FPoint right = {.x = 100.0f, .y = 250.0f};
    SDL_FPoint down = {.x = 50.0f, .y = 400.0f};
    SDL_RenderTextureAffine(renderer, texture, NULL, &origin, &right, &down);
    SDL_Vertex vertices[4];
    int i;

    /* Draw a single triangle with a different color at each vertex. Center this one and make it grow and shrink. */
    /* You always draw triangles with this, but you can string triangles together to form polygons. */
    SDL_zeroa(vertices);
    vertices[0].position.x = ((float)WINDOW_WIDTH) / 2.0f;
    vertices[0].position.y = (((float)WINDOW_HEIGHT) - size) / 2.0f;
    vertices[0].color.r = 1.0f;
    vertices[0].color.a = 1.0f;
    vertices[1].position.x = (((float)WINDOW_WIDTH) + size) / 2.0f;
    vertices[1].position.y = (((float)WINDOW_HEIGHT) + size) / 2.0f;
    vertices[1].color.g = 1.0f;
    vertices[1].color.a = 1.0f;
    vertices[2].position.x = (((float)WINDOW_WIDTH) - size) / 2.0f;
    vertices[2].position.y = (((float)WINDOW_HEIGHT) + size) / 2.0f;
    vertices[2].color.b = 1.0f;
    vertices[2].color.a = 1.0f;

    SDL_RenderGeometry(renderer, NULL, vertices, 3, NULL, 0);

    /* you can also map a texture to the geometry! Texture coordinates go from 0.0f to 1.0f. That will be the location
       in the texture bound to this vertex. */
    SDL_zeroa(vertices);
    vertices[0].position.x = 10.0f;
    vertices[0].position.y = 10.0f;
    vertices[0].color.r = vertices[0].color.g = vertices[0].color.b = vertices[0].color.a = 1.0f;
    vertices[0].tex_coord.x = 0.0f;
    vertices[0].tex_coord.y = 0.0f;
    vertices[1].position.x = 150.0f;
    vertices[1].position.y = 10.0f;
    vertices[1].color.r = vertices[1].color.g = vertices[1].color.b = vertices[1].color.a = 1.0f;
    vertices[1].tex_coord.x = 1.0f;
    vertices[1].tex_coord.y = 0.0f;
    vertices[2].position.x = 10.0f;
    vertices[2].position.y = 150.0f;
    vertices[2].color.r = vertices[2].color.g = vertices[2].color.b = vertices[2].color.a = 1.0f;
    vertices[2].tex_coord.x = 0.0f;
    vertices[2].tex_coord.y = 1.0f;
    SDL_RenderGeometry(renderer, texture, vertices, 3, NULL, 0);

    /* Did that only draw half of the texture? You can do multiple triangles sharing some vertices,
       using indices, to get the whole thing on the screen: */

    /* Let's just move this over so it doesn't overlap... */
    for (i = 0; i < 3; i++)
    {
        vertices[i].position.x += 450;
    }

    /* we need one more vertex, since the two triangles can share two of them. */
    vertices[3].position.x = 600.0f;
    vertices[3].position.y = 150.0f;
    vertices[3].color.r = vertices[3].color.g = vertices[3].color.b = vertices[3].color.a = 1.0f;
    vertices[3].tex_coord.x = 1.0f;
    vertices[3].tex_coord.y = 1.0f;

    /* And an index to tell it to reuse some of the vertices between triangles... */
    {
        /* 4 vertices, but 6 actual places they used. Indices need less bandwidth to transfer and can reorder vertices easily! */
        const int indices[] = {0, 1, 2, 1, 2, 3};
        SDL_RenderGeometry(renderer, texture, vertices, 4, indices, SDL_arraysize(indices));
    }

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