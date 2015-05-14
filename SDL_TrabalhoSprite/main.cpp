#include <SDL.h>
#include <SDL_image.h>
#include <iostream>
#include "LTexture.h"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define SPRITE_SIZE 50
#define WALKING_ANIMATION_FRAMES 18
#define WALKING_LEFT_LAST_FRAME 9
#define WALKING_RIGHT_LAST_FRAME 18

using namespace std;

bool init();
bool loadMedia();
void close();
SDL_Texture* loadTexture(string path);

SDL_Window* gWindow;
LTexture gBackgroundTexture;
LTexture gModulatedTexture;
LTexture gSpriteTexture;
SDL_Event event;
SDL_Rect rectSrc, rectSprite[WALKING_ANIMATION_FRAMES];
SDL_Renderer* gRenderer = NULL;

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
        gWindow = SDL_CreateWindow("SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

            gBackgroundTexture.loadFromFile("D:/Codes/SDL/SDLBlog/ExemploSprite/imagem.png", gRenderer);
            gSpriteTexture.loadFromFile("D:/Codes/SDL/SDLBlog/ExemploSprite/sprite.png", gRenderer);
            rectSrc.w = SPRITE_SIZE;
            rectSrc.h = SPRITE_SIZE;

            for (int i = 0; i < 18; i++)
            {
                if (i < 9)
                {
                    (i < 5) ?
                        (rectSprite[i].x = i * SPRITE_SIZE) : (rectSprite[i].x = (i - 5) * SPRITE_SIZE);
                    (i < 5) ?
                        (rectSprite[i].y = 0) : (rectSprite[i].y = SPRITE_SIZE);
                }
                else
                {
                    (i < 14) ?
                        (rectSprite[i].x = (13 - i) * SPRITE_SIZE) : (rectSprite[i].x = (18 - i) * SPRITE_SIZE);
                    (i < 14) ?
                        (rectSprite[i].y = 2 * SPRITE_SIZE) : (rectSprite[i].y = 3 * SPRITE_SIZE);
                }
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
            key++;
            if (key > 8) { key = 0; }
            posX += 15;
            cout << posX << endl;
            break;
        case SDLK_LEFT:
            key++;
            if (key < 9 || key > 17) { key = 9; }
            posX -= 15;
            cout << posX << endl;
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
            gSpriteTexture.render(posX, posY, gRenderer, &rectSprite[key]);

            SDL_RenderPresent(gRenderer);
        }
    }
    close();
    return 0;
}