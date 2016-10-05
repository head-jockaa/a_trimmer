#ifndef MAPDRAWING__
#define MAPDRAWING__

#include "util.h"

#define SLOPE_45 1
#define SLOPE_135 2
#define SLOPE_225 3
#define SLOPE_315 4
#define SUMMIT 5
#define VOLCANO 6
#define CITY 7
#define TOWN 8
#define ROAD 9
#define SEA 10
#define SHORE 11
#define TOWER_S 12
#define TOWER_M 13
#define TOWER_L 14
#define BEYOND 30000

void createMap();
void createMap_tower();
void createMap_color(int bright);
void drawMap(SDL_Surface* scr, int X, int Y);
void drawMap2(SDL_Surface* scr, int X, int Y);
void drawGround(SDL_Surface *scr, int x, int y, int x2, int y2, int w, int h, int bright, bool buf);
void drawTowerSpot(SDL_Surface *scr, int x, int y, int x2, int y2, int w, int h, bool buf);
void drawVolcano(int x, int y, int w, int h, bool buf);
void drawFireWork(SDL_Surface *scr);
void drawColorLight(int x, int y, int w, int h, bool buf);
SDL_Color getSkyColor(int h, int m);
void make3dview(double X, double Y, int D);
void make3dview_tower(double X, double Y, int D);
void make3dview_sky();
void resetZ();

struct Map{
	int mapW,mapH,ruralW,ruralH,volcanoNum,slideX,slideY;
	int airX,airY,airW,airH,volX,volY,volW,volH;
	Uint8 rural_size;
	Uint8 **rural, *shore, *type, **smr;
	Uint16 **h, **slope;
	Uint16 *volcano, *volcano_id, *volcanoX, *volcanoY, *rural_rate;
	Image *rgb;
	bool moved,buffered,buffered2,rural_loaded;
	Image *rpg;
	Uint8 c[640*480];
	Uint16 z[640*480];
	void set();
	void set2();
	void reset();
	void setRPGchip(int bright);
};
extern Map map;

#endif
