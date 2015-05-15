#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "LTexture.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SPRITE_SIZE 50
#define WALKING_ANIMATION_FRAMES 9

using namespace std;

SDL_Texture* loadTexture(string path);
bool init();
void close();
void handleEvent(SDL_Event event);

SDL_Window* gWindow;
LTexture gBackgroundTexture;
LTexture gModulatedTexture;
LTexture gSpriteTexture;
SDL_Rect rectSrc, rectSprite[WALKING_ANIMATION_FRAMES];
SDL_Renderer* gRenderer = NULL;

SDL_Event event;
bool flip = false;
SDL_RendererFlip flipType = SDL_FLIP_NONE;

int quit;
int key = 0, posX = 0, posY = 0;

SDL_Texture* loadTexture(string path)
{
    SDL_Texture* newTexture = NULL;

    SDL_Surface* loadedSurface = IMG_Load(path.c_str());
    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError());
    }
    else
    {
        newTexture = SDL_CreateTextureFromSurface(gRenderer, loadedSurface);
        if (newTexture == NULL)
        {
            printf("Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError());
        }
        SDL_FreeSurface(loadedSurface);
    }
    return newTexture;
}

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("SDL Trabalho Sprite", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_FULLSCREEN);
        if (gWindow == NULL)
        {
            printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
            success = false;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                printf("Renderer could not be created! SDL Error: %s\n", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);

                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) & imgFlags))
                {
                    printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
                    success = false;
                }
            }

            gBackgroundTexture.loadFromFile("D:/Codes/SDL/SDL_TrabalhoSprite/SDL_TrabalhoSprite/imagem.png", gRenderer);
            gSpriteTexture.loadFromFile("D:/Codes/SDL/SDL_TrabalhoSprite/SDL_TrabalhoSprite/sprite.png", gRenderer);
            rectSrc.w = SPRITE_SIZE;
            rectSrc.h = SPRITE_SIZE;

            for (int i = 0; i < 9; i++)
            {
                (i < 5) ?
                    (rectSprite[i].x = i * SPRITE_SIZE) : (rectSprite[i].x = (i - 5) * SPRITE_SIZE);
                (i < 5) ?
                    (rectSprite[i].y = 0) : (rectSprite[i].y = SPRITE_SIZE);
                rectSprite[i].w = SPRITE_SIZE;
                rectSprite[i].h = SPRITE_SIZE;
            }
        }
    }
    return success;
}

void close()
{
    gSpriteTexture.free();
    gBackgroundTexture.free();
    gModulatedTexture.free();
 
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    IMG_Quit();
    SDL_Quit();
}

void handleEvent(SDL_Event event)
{
    switch (event.type)
    {
    case SDL_QUIT:
        quit = 1;
        break;
    case SDL_KEYDOWN:
        switch (event.key.keysym.sym)
        {
        case SDLK_ESCAPE:
        case SDLK_q:
            quit = 1;
            break;
        case SDLK_RIGHT:
            if (flip) { flip = false; key = 0; }
            key++;
            if (key > WALKING_ANIMATION_FRAMES - 1) { key = 0; }
            posX += 15;
            cout << posX << endl;
            cout << "Flip?" << flip << endl;
            break;
        case SDLK_LEFT:
            if (!flip) { flip = true; key = 0; }
            key++;
            if (key > WALKING_ANIMATION_FRAMES - 1) { key = 0; }
            posX -= 15;
            cout << posX << endl;
            cout << "Flip?" << flip << endl;
            break;
        case SDLK_DOWN:
            posY += 15;
            break;
        case SDLK_UP:
            posY -= 15;
            break;
        }
        break;
    }
    cout << key << endl;
    if (posX >= (SCREEN_WIDTH - SPRITE_SIZE)) { posX = (SCREEN_WIDTH - SPRITE_SIZE); }
    if (posX < 0) { posX = 0; }
    if (posY >= (SCREEN_HEIGHT - SPRITE_SIZE)) { posY = (SCREEN_HEIGHT - SPRITE_SIZE); }
    if (posY < 0) { posY = 0; }
}

int main(int argc, char* args[])
{
    if (!init()) { printf("Failed to initialize!\n"); }
    else
    {
        while (!quit)
        {
            while (SDL_PollEvent(&event) != 0)
            {
                handleEvent(event);
            }
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            gBackgroundTexture.render(0, 0, gRenderer);

            (flip) ? (flipType = SDL_FLIP_HORIZONTAL) : (flipType = SDL_FLIP_NONE);
            gSpriteTexture.render(posX, posY, gRenderer, &rectSprite[key], 0.0, NULL, flipType);

            SDL_RenderPresent(gRenderer);
        }
    }
    close();
    return 0;
}