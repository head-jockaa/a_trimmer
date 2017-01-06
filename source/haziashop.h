#ifndef HAZIASHOP__
#define HAZIASHOP__

#include "util.h"

#define SHOP_FLOOR 0
#define SHOPPING 1
#define SHOP_TALK 2
#define SHOP_LASTTALK 3
#define BUYING 4
#define SELLING 5
#define SHOP_FLOOR_TALK 6
#define OHANA_TALK 7
#define OHANA_MENU 8
#define SHOP_CONFIRM 10
//#define YNFORM 11
#define SHOP_ITEMS 18

#define TRYING_TO_PAY_MUCH 12
#define MERELY_YES 13
#define NOD_AND_BUY 14

extern int price[SHOP_ITEMS], shop_icon[SHOP_ITEMS];

void initHaziaShop();
void keyHaziaShop();
void timerHaziaShop();
void drawHaziaShop(SDL_Surface* scr);
void endHaziaShop();

#endif
