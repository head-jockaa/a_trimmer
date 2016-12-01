#ifndef ANIMATION__
#define ANIMATION__

#include "util.h"

extern int playtime;
extern int timestamp,loadtime,pausetime;
extern int call_week,call_hour,call_minute;
void drawAnimationCut(SDL_Surface* scr);
bool nextCut();
bool nextTalk();
void freeCartoon();
void loadCartoon(const char *filename);

void drawLastEndingAnim(SDL_Surface* scr, int cn);
void drawSummerWars(SDL_Surface* scr);

bool startsWith(char *s, const char *target);
int fetchInt(char *&s);
double fetchDouble(char *&s);

#endif
