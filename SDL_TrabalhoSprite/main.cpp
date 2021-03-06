#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "GTexture.h"

#define SCREEN_WIDTH 368
#define SCREEN_HEIGHT 322
#define SPRITE_SIZE 50
#define WALKING_ANIMATION_OFFSET 0
#define WALKING_ANIMATION_FRAMES 9
#define SPAWNING_ANIMATION_OFFSET 10
#define SPAWNING_ANIMATION_FRAMES 8
#define STANDING_ANIMATION_OFFSET 20
#define STANDING_ANIMATION_FRAMES 8
#define SHOOTING_ANIMATION_OFFSET 30
#define SHOOTING_ANIMATION_FRAMES 3

using namespace std;

void loadAnimation(int offset, int frames, SDL_Rect rect[]);
bool init();
void close();
void handleEvent(SDL_Event event);

SDL_Window* gWindow;
GTexture gBackgroundTexture;
GTexture gSpriteTexture;
SDL_Rect rectWalkingSprite[WALKING_ANIMATION_FRAMES];
SDL_Rect rectSpawningSprite[SPAWNING_ANIMATION_FRAMES];
SDL_Rect rectStandingSprite[STANDING_ANIMATION_FRAMES];
SDL_Rect rectShootingSprite[SHOOTING_ANIMATION_FRAMES];
SDL_Renderer* gRenderer = NULL;
Mix_Music* gMusic = NULL;
Mix_Chunk* gShootingSound = NULL;
Mix_Chunk* gSpawnSound = NULL;

SDL_Event event;
bool flip = false;
bool walking = false;
bool standAnim = false;
bool shooting = false;
SDL_RendererFlip flipType = SDL_FLIP_NONE;

int quit;
int key = 0; 
int posX = (SCREEN_WIDTH / 2) - (SPRITE_SIZE / 2); 
int posY = (SCREEN_HEIGHT / 2) - (SPRITE_SIZE / 2);

Mix_Music* loadMusic(char* path)
{
    Mix_Music* music = Mix_LoadMUS(path);
    if (music == NULL)
    {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
    }
    return music;
}

Mix_Chunk* loadSoundFx(char* path)
{
    Mix_Chunk* sound = Mix_LoadWAV(path);
    if (sound == NULL)
    {
        printf("Failed to load scratch sound effect! SDL_mixer Error: %s\n", Mix_GetError());
    }
    return sound;
}

void loadAnimation(int offset, int frames, SDL_Rect rect[])
{
    for (int i = 0; i < frames; i++)
    {
        (i < 5) ?
            (rect[i].x = i * SPRITE_SIZE) : (rect[i].x = (i - 5) * SPRITE_SIZE);
        (i < 5) ?
            (rect[i].y = (offset / 5) * SPRITE_SIZE) : (rect[i].y = ((offset / 5) + 1) * SPRITE_SIZE);
        rect[i].w = SPRITE_SIZE;
        rect[i].h = SPRITE_SIZE;
    }
}

bool init()
{
    bool success = true;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0)
    {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        gWindow = SDL_CreateWindow("SDL Trabalho Sprite", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
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

            if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
            {
                printf("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
                success = false;
            }

            gBackgroundTexture.onLoad("Images/imagem.png", gRenderer);
            gSpriteTexture.onLoad("Images/sprite.png", gRenderer);
            loadAnimation(WALKING_ANIMATION_OFFSET, WALKING_ANIMATION_FRAMES, rectWalkingSprite);
            loadAnimation(SPAWNING_ANIMATION_OFFSET, SPAWNING_ANIMATION_FRAMES, rectSpawningSprite);
            loadAnimation(STANDING_ANIMATION_OFFSET, STANDING_ANIMATION_FRAMES, rectStandingSprite);
            loadAnimation(SHOOTING_ANIMATION_OFFSET, SHOOTING_ANIMATION_FRAMES, rectShootingSprite);
            gMusic = loadMusic("Sounds/XTheme.mp3");
            gSpawnSound = loadSoundFx("Sounds/spawn.wav");
            gShootingSound =  loadSoundFx("Sounds/shoot.wav");
        }
    }
    return success;
}

void close()
{
    gSpriteTexture.free();
    gBackgroundTexture.free();
 
    SDL_DestroyRenderer(gRenderer);
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;
    gRenderer = NULL;

    Mix_FreeMusic(gMusic);
    Mix_FreeChunk(gShootingSound);
    Mix_FreeChunk(gSpawnSound);

    Mix_CloseAudio();

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
            quit = 1;
            break;
        case SDLK_q:
            quit = 1;
            break;
        case SDLK_RIGHT:
            if (flip) { flip = false; key = 0; }
            walking = true;
            standAnim = false;
            key++;
            if (key > WALKING_ANIMATION_FRAMES - 1) { key = 0; }
            posX += 10;
            cout << posX << endl;
            cout << "Flip?" << flip << endl;
            break;
        case SDLK_LEFT:
            if (!flip) { flip = true; key = 0; }
            walking = true;
            standAnim = false;
            key++;
            if (key > WALKING_ANIMATION_FRAMES - 1) { key = 0; }
            posX -= 10;
            cout << posX << endl;
            cout << "Flip?" << flip << endl;
            break;
        case SDLK_SPACE:
            shooting = true;
            walking = false;
            standAnim = false;
        }
        break;
    case SDL_KEYUP:
        switch (event.key.keysym.sym)
        {
        case SDLK_RIGHT:
            key = 7;
            walking = false;
            break;
        case SDLK_LEFT:
            key = 7;
            walking = false;
            break;
        case SDLK_SPACE:
            shooting = false;
            key = 7;
        }
    }
    cout << key << endl;
    if (posX >= (SCREEN_WIDTH - SPRITE_SIZE)) { posX = (SCREEN_WIDTH - SPRITE_SIZE); }
    if (posX < 0) { posX = 0; }
    if (posY >= (SCREEN_HEIGHT - SPRITE_SIZE)) { posY = (SCREEN_HEIGHT - SPRITE_SIZE); }
    if (posY < 0) { posY = 0; }
}

int main(int argc, char* args[])
{
    srand(time(NULL));
    if (!init()) { printf("Failed to initialize!\n"); }
    else
    {
        Mix_PlayMusic(gMusic, -1);
        Mix_PlayChannel(-1, gSpawnSound, 0);
        while (key < SPAWNING_ANIMATION_FRAMES)
        {
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);
            gBackgroundTexture.onDraw(0, 0, gRenderer);
            gSpriteTexture.onDraw(posX, posY, gRenderer, &rectSpawningSprite[key], 0.0, NULL, flipType);
            SDL_RenderPresent(gRenderer);
            key++;
            SDL_Delay(80);
        }
        key = 7;
        while (!quit)
        {
            while (SDL_PollEvent(&event) != 0)
            {
                handleEvent(event);
            }
            SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
            SDL_RenderClear(gRenderer);

            gBackgroundTexture.onDraw(0, 0, gRenderer);

            (flip) ? (flipType = SDL_FLIP_HORIZONTAL) : (flipType = SDL_FLIP_NONE);

            if (walking)
            {
                gSpriteTexture.onDraw(posX, posY, gRenderer, &rectWalkingSprite[key], 0.0, NULL, flipType);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(50);
            }
            else if (shooting)
            {
                gSpriteTexture.onDraw(posX, posY, gRenderer, &rectShootingSprite[1], 0.0, NULL, flipType);
                Mix_PlayChannel(-1, gShootingSound, 0);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(200);
            }
            else
            {
                if (key >= STANDING_ANIMATION_FRAMES - 1) { standAnim = false; }
                if ((rand() % 100) > 85 && standAnim == false) { standAnim = true; key = 0; }
                if (standAnim && key < STANDING_ANIMATION_FRAMES - 1) 
                { 
                    key++;
                }
                gSpriteTexture.onDraw(posX, posY, gRenderer, &rectStandingSprite[key], 0.0, NULL, flipType);
                SDL_RenderPresent(gRenderer);
                SDL_Delay(200);
            }
        }
    }
    close();
    return 0;
}