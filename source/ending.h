#ifndef ENDING__
#define ENDING__

#include "util.h"

#define LEAVE_SHORE 0
#define GET_MEDAL 2
#define PRE_ENDING_ANIME 4
#define ENDING_ANIME 5
#define WARNING 6
#define ANIME_GOD 7
#define LAST_STORY 8
#define LAST_ENDING 9

void initEnding();
void initMedalAward(int n);
void initEndingAnime();
void initEndingMainAnime();
void initLastStory();
void initLastStory2();
void initLastEnding();
void keyEnding();
void timerEnding();
void drawEnding(SDL_Surface* scr);
void endEnding();

#endif
