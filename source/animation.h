#ifndef ANIMATION__
#define ANIMATION__

#include "util.h"

struct ObjectSetting{
	double x,y,ix,iy,w,h,mag,alpha,shake;
	int type,drawTo,lang,img_id;
	double R,G,B;
	double gradRfrom,gradGfrom,gradBfrom;
	double gradRto,gradGto,gradBto;
};
struct ObjectMoving{
	double x,y,ix,iy,mag,w,h,alpha,shake;
	double R,G,B,gradRfrom,gradGfrom,gradBfrom,gradRto,gradGto,gradBto;
};
struct ObjectFlipping{
	double ix,iy;
	int interval,num,count;
	bool turnBack;
};
struct ObjectSliding{
	double step,from,to,position,fade,fadeRate;
	bool turnBack,back;
};
struct CartoonObject{
	int id;
	ObjectSetting set;
	ObjectMoving move;
	ObjectFlipping flip;
	ObjectSliding slideX,slideY,slideIX,slideIY,slideMag,slideAlpha;
	double jumpG,jumpY;
	double waveSIN,waveRange;
};

struct JsonData{
	char cartoonBgmName[200];
	char *text;
	size_t size, pointer;
	char name[100][100], valueString[100][1000];
	int valueNum;
	double valueDouble[100];
	bool valueBool[100];
	int playtime, nextTime;
	int timestamp, pausetime;
	int max_obj;
	int call_week, call_hour, call_minute;
	bool talkmode, skipThisTime, cartoonSync;
	CartoonObject obj[1000];
};
extern JsonData cartoonJson, talkingJson;

void drawAnimationCut(JsonData *json, SDL_Surface* scr);
bool nextCut(JsonData *json);
bool nextTalk(JsonData *json);
void freeCartoon(JsonData *json);
void loadCartoon(JsonData *json, const char *filename);
int fetchInt(char *s, int *value);
int fetchDouble(char *s, double *value);
int fetchString(char *s, char brace, char *value);
bool startsWith(char *s, const char *target);

#endif
