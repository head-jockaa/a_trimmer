#ifndef DRAWIMAGE__
#define DRAWIMAGE__

#include "util.h"


void drawImage(Image* scr, Image* img, int x, int y, int x2, int y2, int w2, int h2, int a);
void drawImage(SDL_Surface* scr, Image* img, int x, int y, int x2, int y2, int w2, int h2, int a);
void drawImage(Image* img, SDL_Surface* scr, int x, int y, int x2, int y2, int w2, int h2, int a);
void drawImage_r(SDL_Surface* scr, Image* img, int x, int y, int x2, int y2, int w2, int h2, int a);
void drawImage_r(Image* scr, Image* img, int x, int y, int x2, int y2, int w2, int h2, int a);
void drawImage_r2(SDL_Surface* scr, Image* img, int x, int y, int x2, int y2, int w2, int h2, int a);
void drawImage_r2(Image* scr, Image* img, int x, int y, int x2, int y2, int w2, int h2, int a);
void drawImage_t(Image* scr, Image* img, int x, int y, int x2, int y2, int w2, int h2, int a);
void drawImage_x(SDL_Surface* scr, Image* img, int x, int y, double mag, int x2, int y2, int w2, int h2, int a);
void drawImage_x(Image* scr, Image* img, int x, int y, double mag, int x2, int y2, int w2, int h2, int a);
void drawImage_xr(SDL_Surface* scr, Image* img, int x, int y, double mag, int x2, int y2, int w2, int h2, int a);
void rotateImage(SDL_Surface* scr, Image* img, int x, int y, double mag, int x2, int y2, int cx, int cy, int w2, int h2, int a);
void rotateImage(Image* scr, Image* img, int x, int y, double rot, int x2, int y2, int cx, int cy, int w2, int h2, int a);
void rotateImage_x(SDL_Surface* scr, Image* img, int x, int y, double rot, double mag, int x2, int y2, int cx, int cy, int w2, int h2, int a);
void rotateImage_x(Image* scr, Image* img, int x, int y, double rot, double mag, int x2, int y2, int cx, int cy, int w2, int h2, int a);
void illuminateImage(SDL_Surface* scr, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int a);
void illuminateImage(Image* scr, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int a);
void illuminateImage_x(SDL_Surface* scr, Image* ima, int x, int y, double mag, int x2, int y2, int w2, int h2, int a);
void illuminateImage_x(Image* scr, Image* ima, int x, int y, double mag, int x2, int y2, int w2, int h2, int a);
void illuminateImage_quick(SDL_Surface* scr, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int c, int a);
void illuminateImage_quick(Image* scr, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int c, int a);
void slideImage(Image* scr, int slideX, int slideY);
void fillRect(SDL_Surface* scr, int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B, int a);
void fillRect(Image* scr, int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B, int a);
void drawLightBall(SDL_Surface* scr, int x, int y, int w, int R, int G, int B);
void drawLightBall(Image* scr, int x, int y, int w, int R, int G, int B);

#endif
