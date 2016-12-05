#ifndef ENDING__
#define ENDING__

#include "util.h"

#define LEAVE_SHORE 0
#define GET_MEDAL 2
#define ENDING_ANIME 5
#define WARNING 6

void initEnding();
void initMedalAward(int n);
void initEndingAnime();
void initEndingMainAnime();
void keyEnding();
void timerEnding();
void drawEnding(SDL_Surface* scr);
void endEnding();

#endif
