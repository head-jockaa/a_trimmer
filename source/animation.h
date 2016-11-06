#ifndef ANIMATION__
#define ANIMATION__

#include "util.h"

extern int playtime;
extern int timestamp,loadtime,pausetime;
void drawAnimationCut(SDL_Surface* scr);
void nextCut();
void freeCartoon();
void loadCartoon(const char *filename);

void drawLastEndingAnim(SDL_Surface* scr, int cn);
void drawWeeklyComic(SDL_Surface* scr);
void drawSummerWars(SDL_Surface* scr);
void drawPrologue(SDL_Surface* scr);
void drawKomoro(SDL_Surface* scr);

#endif
