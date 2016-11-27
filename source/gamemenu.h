#ifndef GAMEMENU__
#define GAMEMENU__

#include "util.h"

#define MAIN 0
#define SEASON 1
#define GAMEMODE 2
#define SAVEDATA 3
#define RECORDMENU 4
#define RECORD 5
#define GOTO_GAME 6
#define GOTO_RECORD 7
#define PROLOGUE 8
#define SEASIDE 9
#define KOMORO 10
#define NODATA 11

void initGameMenu();
void endGameMenu();
void timerGameMenu();
void keyGameMenu();
void drawGameMenu(SDL_Surface* scr);
bool makeSaveMenu(int n);
bool makeRecordMenu(int n);

#endif
