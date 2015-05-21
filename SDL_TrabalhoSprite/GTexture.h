#ifndef GTEXTURE_H
    #define GTEXTURE_H

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

using namespace std;

class GTexture
{
    private:
        SDL_Texture* mTexture;
        int mWidth;
        int mHeight;

    public:
        GTexture();
        ~GTexture();

    public:
        bool onLoad(char* path, SDL_Renderer* gRenderer);
        void onDraw(int x, int y, SDL_Renderer* gRenderer, SDL_Rect* clip = NULL, double angle = 0.0,
            SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE);

        void free();
        void setColor(Uint8 red, Uint8 green, Uint8 blue);
        void setBlendMode(SDL_BlendMode blending);
        void setAlpha(Uint8 alpha);
        void setPath(char* path);
        int getWidth();
        int getHeight();
        char* getPath();
};

#endif