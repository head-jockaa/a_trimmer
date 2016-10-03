#ifndef OPTION__
#define OPTION__

#include "util.h"

#define OPTION_MENU 0
#define OPTION_SETTING 1
#define OPTION_KEY_SETTING 2

void initOption();
void keySetting();
void keyOption();
void keyOptionMenu();
void keyOptionSetting();
void timerOption();
void drawOption(SDL_Surface* scr);
void endOption();
void drawKeySetting(SDL_Surface* scr);
void drawOptionMenuBox(SDL_Surface* scr);
void drawOptionSetting(SDL_Surface* scr, int x, int y);

extern Uint8 key_setting;
extern Uint32 key_enable[82];

#endif