#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "SDL2/SDL_image.h"
#include "isometric.h"

#define ASSERT(_e, ...)              \
    if (!(_e))                       \
    {                                \
        fprint(stderr, __VA_ARGS__); \
        exit(1);                     \
    }

typedef float f32;
typedef double f64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef size_t usize;
typedef ssize_t isize;

#define PI 3.14159265359f
#define TAU (2.0f * PI)
#define PI_2 (PI / 2.0f)
#define PI_4 (PI / 4.0f)

#define kGameName "SDL2 Game"
#define kGameWidth 1600
#define kGameHeight 900
#define TILE_WIDTH 64
#define TILE_HEIGHT 64
#define SCENE_WIDTH 10
#define SCENE_HEIGHT 10

SDL_Texture *
LoadTexture(SDL_Renderer *renderer, char *path)
{
    SDL_Texture *texture = NULL;
    SDL_Surface *loadedSurface = IMG_Load(path);

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s", path, IMG_GetError());
        return NULL;
    }
    else
    {
        texture = SDL_CreateTextureFromSurface(renderer, loadedSurface);
        if (texture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s", path, SDL_GetError());
            return NULL;
        }
        else
        {
            SDL_FreeSurface(loadedSurface);
            loadedSurface = NULL;
        }

        return texture;
    }
}

SDL_Texture *LoadTexture2(SDL_Renderer *renderer, char *path)
{
    SDL_Texture *texture = NULL;

    texture = IMG_LoadTexture(renderer, path);
    if (texture == NULL)
    {
        printf("Unable to create texture from %s! SDL Error: %s", path, SDL_GetError());
        return NULL;
    }

    return texture;
}

int main(int argc, char const *argv[])
{
    setvbuf(stdout, NULL, _IONBF, 0);

    printf("Starting SDL2\n");

    if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
    {
        printf("SDL_Init Error\n%s", SDL_GetError());
        return -1;
    }

    // Create a window
    SDL_Window *window = SDL_CreateWindow(
        kGameName,
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        kGameWidth,
        kGameHeight,
        SDL_WINDOW_SHOWN);

    if (!window)
    {
        printf("SDL_CreateWindow Error\n%s", SDL_GetError());
        return -1;
    }

    // Create a renderer
    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        -1,
        SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (!renderer)
    {
        printf("SDL_CreateRenderer Error\n%s", SDL_GetError());
        return -1;
    }
    else
    {
        int imgFlags = IMG_INIT_PNG | IMG_INIT_JPG;
        if (!(IMG_Init(imgFlags) & imgFlags))
        {
            printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
            return -1;
        }

        // Scene creation
        int scene[SCENE_HEIGHT][SCENE_WIDTH];
        int offsetX = (kGameWidth - SCENE_WIDTH * TILE_WIDTH) / 2;
        int offsetY = (kGameHeight - SCENE_HEIGHT * TILE_HEIGHT) / 2;

        SDL_Texture *tile_texture_0 = LoadTexture(renderer, "/Users/jdrouin/CDA/GameDev/Projet_Black_Forest/assets/Isometric_Tiles_Pixel_Art/Blocks/blocks_19.png");
        SDL_Texture *tile_texture_1 = LoadTexture(renderer, "/Users/jdrouin/CDA/GameDev/Projet_Black_Forest/assets/Isometric_Tiles_Pixel_Art/Blocks/blocks_19.png");

        // Main Game loop
        while (true)
        {
            // Handle events
            SDL_Event event;
            SDL_PollEvent(&event);

            switch (event.type)
            {
            case SDL_QUIT:
                return 0;
                break;
            }

            // Clear the screen
            SDL_RenderClear(renderer);

            // Draw the tiles
            for (int x = 0; x < SCENE_WIDTH; x++)
            {
                for (int y = 0; y < SCENE_HEIGHT; y++)
                {
                    int i, j;
                    iso_3d_to_2d(x, y, 0, &i, &j, TILE_WIDTH, TILE_HEIGHT);

                    // Ajoutez le décalage pour centrer les tuiles sur l'écran
                    i += offsetX;
                    j += offsetY;

                    // Choisissez la texture de la tuile en fonction du type de tuile
                    SDL_Texture *tile_texture;
                    if (scene[y][x] == 0)
                    {
                        tile_texture = tile_texture_0;
                    }
                    else
                    {
                        tile_texture = tile_texture_1;
                    }

                    // Draw the tile
                    SDL_Rect tile_rect = {i, j, TILE_WIDTH, TILE_HEIGHT};
                    SDL_RenderCopy(renderer, tile_texture, NULL, &tile_rect);
                }
            }

            // Draw the screen
            SDL_RenderPresent(renderer);
        }

        SDL_DestroyTexture(tile_texture_0);
        SDL_DestroyTexture(tile_texture_1);
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        IMG_Quit();
        SDL_Quit();

        return 0;
    }
}
