#ifndef ANIMATION__
#define ANIMATION__

#include "util.h"

struct ObjectSetting{
	double x,y,ix,iy,w,h,mag,alpha,shake;
	int type,drawTo,lang,img_id;
	double R,G,B;
	double gradRfrom,gradGfrom,gradBfrom;
	double gradRto,gradGto,gradBto;
	bool fixed, reverse;
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

struct JsonDataReader{
	int valueNum,maxValueNum,arraySize;
	char **name, **valueString;
	double *valueDouble;
	bool *valueBool;
	void free();
	void init(int size);
};

struct JsonReader{
	JsonDataReader set,move,talk,image,bgm,sound,loadSound;
	char basename[100];
	JsonDataReader *which;
	bool sync,hasSync,hasResetTimer,end;
	int resetTimer,resetNum,*reset;
	int talkPointer;
};

struct JsonData{
	char cartoonBgmName[200];
	char *jsonText;
	size_t size, pointer;
	int playtime, nextTime, targetIndex;
	int timestamp, pausetime;
	int max_obj,max_sound,max_image,musicRepeat;
	int call_week, call_hour, call_minute;
	int scrX, scrY;
	bool initializedObjArray, initializedReaderArray, searchingIndex, talkmode, cartoonSync, playingMusic, toPlayMusic, end;
	CartoonObject *obj;
	Mix_Chunk **sound;
	Image **bg;
	JsonReader jr;
	int talk_size,talk_open_count,text_count,face_count,shake_count;
	String talk;
};
extern JsonData cartoonJson, talkingJson, manekitvJson;

void readCartoon(JsonData *json, int index);
void drawAnimationCut(JsonData *json, SDL_Surface* scr);
void drawAnimationCutBeforeDivision(JsonData *json, SDL_Surface* scr);
void drawAnimationCutAfterDivision(JsonData *json, SDL_Surface* scr);
bool nextCut(JsonData *json);
bool nextTalk(JsonData *json);
void freeCartoon(JsonData *json);
void loadCartoon(JsonData *json, const char *filename);
int fetchInt(char *s, int *value);
int fetchDouble(char *s, double *value);
int fetchString(char *s, char brace, char *value);
bool startsWith(char *s, const char *target);

#endif
