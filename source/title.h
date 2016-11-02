#ifndef TITLE__
#define TITLE__

#include "util.h"

extern SDL_Surface *fade;
void initOpening();
void endOpening();
void drawOpening(SDL_Surface* scr);
void keyOpening();
void timerOpening();
void initOpeningMainAnime();
void initTitle2();
void keyTitle();
void timerTitle();
void drawTitle(SDL_Surface* scr);
void endTitle();

#endif
