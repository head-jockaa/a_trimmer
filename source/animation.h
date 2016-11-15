#ifndef ANIMATION__
#define ANIMATION__

#include "util.h"

extern int playtime;
extern int timestamp,loadtime,pausetime;
void drawAnimationCut(SDL_Surface* scr);
bool nextCut();
void nextTalk();
void freeCartoon();
void loadCartoon(const char *filename);

void drawLastEndingAnim(SDL_Surface* scr, int cn);
void drawSummerWars(SDL_Surface* scr);

#endif
