#ifndef HAZIASHOP__
#define HAZIASHOP__

#include "util.h"

#define SHOP_FLOOR 0
#define SHOPPING 1
#define SHOP_TALK 2
#define SHOP_LASTTALK 3
#define BUYING 4
#define SELLING 5
#define MEMMA_TALK 6
#define OHANA_TALK 7
#define SHOP_CONFIRM_TALK 8
#define SHOP_CONFIRM 9
#define THROW_MEMMA 10
//#define YNFORM 11
#define OHANA_MENU 12
#define NAGANO_APPLE 13
#define ARANCIA 14
#define STAY_IN 15

extern int price[18], shop_icon[18];

void initHaziaShop();
void keyHaziaShop();
void timerHaziaShop();
void drawHaziaShop(SDL_Surface* scr);
void endHaziaShop();

#endif