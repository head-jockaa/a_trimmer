#ifndef ANIMATION__
#define ANIMATION__

#include "util.h"

extern int playtime, cartoonNextTime;
extern int timestamp,loadtime,pausetime;
extern size_t cartoonJsonSize;
extern char *cartoonJson;
void drawAnimationCut(SDL_Surface* scr);
void nextCut();
void resetObject(int n);

void drawEndingAnim(SDL_Surface *scr, int cn);
void drawLastEndingAnim(SDL_Surface* scr, int cn);
void drawWeeklyComic(SDL_Surface* scr);
void drawSummerWars(SDL_Surface* scr);
void drawPrologue(SDL_Surface* scr);
void drawKomoro(SDL_Surface* scr);

#endif
