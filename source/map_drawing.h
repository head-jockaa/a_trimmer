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
#define SEA 8
#define TOWN 16
#define TOWER_S 32
#define TOWER_M 48
#define TOWER_L 64
#define ROAD 128
#define BEYOND 30000

void createMap();
void createMap_tower();
void createMap_color(int bright);
void drawSea(SDL_Surface* scr, int X, int Y);
void drawMap(SDL_Surface* scr, int X, int Y);
void drawMap2(SDL_Surface* scr, int X, int Y);
void drawGround(int scrX, int scrY, int x2, int y2, int w, int h, int bright);
void drawCityLight(int scrX, int scrY, int x2, int y2, int w, int h);
void drawTowerSpot(SDL_Surface *scr, int x, int y, int x2, int y2, int w, int h, bool buf);
void drawVolcano(SDL_Surface *scr, int x, int y, int x2, int y2, int w, int h, bool buf);
void drawFireWork(SDL_Surface *scr);
void drawColorLight(int scrX, int scrY, int x, int y, int w, int h);
SDL_Color getSkyColor(int h, int m);
void refreshGroundImage();
void make3dview(double X, double Y, int D);
void make3dview_tower(double X, double Y, int D);
void make3dview_sky();
void resetZ();

struct Map{
	int rural_num,**rural_tv;
	int mapW,mapH,ruralW,ruralH,volcanoNum;
	int airMinX,airMaxX,airMinY,airMaxY,volX,volY,volW,volH;
	Uint8 rural_size;
	Uint8 **rural, *type, **smr;
	Uint16 **h, **slope, *shore, *road, *foothill;
	Uint16 *volcano, *volcano_id, *volcanoX, *volcanoY, *rural_rate;
	Image *rgb;
	bool moved, bufferedTowerSpotImage, bufferedVolcanoImage;
	Image *rpg;
	Image *bufferGround, *bufferTowerSpot, *bufferColorLight, *bufferVolcano, *bufferCity;
	Uint8 c[640*480];
	Uint16 z[640*480];
	void set();
	void addType(int i, Uint8 t);
	void reset();
	void setRPGchip(int bright);
};
extern Map map;

#endif
