#ifndef MIYAZAKI__
#define MIYAZAKI__

#define MIYAZAKI_MUSEUM 0
#define GUIDE_TOP 1
#define GUIDE_ALL 2
#define GUIDE_SEASON 3
#define GUIDE_ANIME 4
#define GUIDE_STALIST 5
#define DELETE_MENU 6
#define PREF_LIST_MIYAZAKI 7
#define GUIDE_STA 8
#define GUIDE_PRGLIST 9
#define GUIDE_TIME 10
#define YNFORM 11
#define BGM_TEST 12
#define WHICH_TOWER 13
#define GUIDE_STALIST_ALL 14
#define SAVEDATA_DELETE 15
#define RECORD_DELETE 16
#define DELETE_YN 17
#define MIYAZAKI_TALK 18
#define GOTO_STROLL 19
#define GOTO_TOWERLIST 20
//#define GAMESTART 21
#define GOTO_MOVIE 22
#define TOWERLIST 23
#define SHOW_TOWERDATA 24
#define PREF_LIST_TOWER 25
#define STA_LIST_TOWER 26
#define SMR_DELETE_PREF 27
#define SMR_DELETE 28
#define SEOI_HA 29
#define COME_MIYAZAKI 30
#define LEAVE_MIYAZAKI 31
#define DEPLOMA_TALK 32
#define DEPLOMA 33
#define NODATA1 34
#define NODATA2 35
#define GAMEBOY 1
#define MONO 2

#define NUM_OF_BGM 22

#include "util.h"

void initMiyazaki();
void endMiyazaki();
void timerMiyazaki();
void keyMiyazaki();
void drawMiyazaki(SDL_Surface* scr);
void changeScreenColor(SDL_Surface* scr);

extern BOOL talk_3dtv,movie_test;
extern Uint8 scr_design;

#endif