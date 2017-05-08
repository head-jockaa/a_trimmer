#ifndef UTILITY__
#define UTILITY__

#define _CRT_SECURE_NO_WARNINGS 

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>
#include <cmath>

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "SDL2_image.lib")
#pragma comment(lib, "SDL2_mixer.lib")
#pragma comment(lib, "SDL2_net.lib")

#define PI 3.1415
#define OPENING 0
#define TITLE 1
#define GAMEMENU 2
#define STORY 3
#define GAME 4
#define ENDING 5
#define OPTION 6
#define MIYAZAKI 7
#define HAZIASHOP 8
#define STORYMODE 0
#define SELECT 1
#define WALKING 2
#define NO_RELAY 3
#define HIDE 0
#define GRAY 1
#define VISIBLE 2
#define BLACK 0
#define BLUE 1
#define SMOOTH 0
#define BUMPY 1
#define TYPE1_EDGE 1
#define TYPE1_BARE 2
#define TYPE2_EDGE 3
#define TYPE2_BARE 4
#define TYPE_MARIO 0
#define TYPE_KIRBY 1
#define TYPE_MEGAMAN 2
#define TYPE_SONIC 3
#define JAPANESE 0
#define EUROPEAN 1
#define SIMPLEROD 0
#define CONVENIENTROD 1
#define UVROD 2
#define MHZROD 3
#define BUTTONROD 4
#define SUPERHANDYROD 5
#define MANUAL 0
#define AUTO 1
#define COMMA -2
#define HANGUP -3
#define SHAKE -4
#define NONE 255

#define TITLETEXT 1
#define MENUTEXT TITLETEXT+3
#define OPTIONTEXT MENUTEXT+20
#define GAMETEXT OPTIONTEXT+26
#define ANTENNATEXT GAMETEXT+30
#define EPILOGUE ANTENNATEXT+26
#define MIYAZAKITEXT EPILOGUE+11

int to8int(char c);
int to16int(char c1, char c2);
int to16int_signed(char c1, char c2);
Uint32 to32int(char c1, char c2, char c3, char c4);
bool toBool(char c);
bool key_wait(int a);
bool key_stop(int a);
void drawText(SDL_Surface* scr, int X, int Y, const char* st);
void drawText(SDL_Surface* scr, int X, int Y, const char* st, int strl);
void drawText(SDL_Surface* scr, int X, int Y, const char* st, int strl, Uint8 a);
void drawText_lang(SDL_Surface* scr, int x, int y, const char* st, int l, Uint8 a, int lang);
void drawText2(SDL_Surface* scr, int X, int Y, const char* st);
void drawText2(SDL_Surface* scr, int X, int Y, const char* st, int strl);
void drawText2(SDL_Surface* scr, int X, int Y, const char* st, int strl, Uint8 a);
void drawText2_lang(SDL_Surface* scr, int x, int y, const char* st, int l, Uint8 a, int lang);
void getSymbolImage();
void drawKeyboard(SDL_Surface* scr, int k, int X, int Y);
void drawKeyboard2(SDL_Surface* scr, int k, int X, int Y);
bool loadFile(const char* fn);
void fix_scrXY();
void fix_XY();
void reset_key();
void initFont();
void endFont();
void setKick(SDL_Surface* scr);
void drawKick(SDL_Surface* scr);
Uint32 setRGB(Uint8 r, Uint8 g, Uint8 b);
void getRGB(Uint32 px, Uint8 *r, Uint8 *g, Uint8 *b);

extern SDL_Rect scr;
extern bool run,setSMR,map_loaded,*animebook,ABGR;
extern Uint8 mode,mode2,which_season,phase,kick_count,pauseGame;
extern int stas,entries,prgs,animedex_num,collection,areas,towers,mounts,towns,season_num,clear_num,mapW,mapH;
extern int count,bg_count,face[1000],start;
extern int talk_kulisap,face_kulisap,ant_dir;
extern bool SHOW_TOWER, EXPLAIN, NHK_REMOVE;
extern Uint8 CHAR_CODE,AIR_IMG,WALKING_TYPE,ROD_TYPE,ADJ_DIR,CHANNELS,FULLSCR,SCRSIZE,BGM_VOLUME,SE_VOLUME,CHOSEON,MAGNIFY,MAP3D,DASH_TYPE;
extern Uint8 X_OF_GRAPH_VERTEX;
extern double FILL_GRAPH_HEIGHT;
extern int pre_magnify;
extern double test;
extern double DISTANCE_1CH;
extern double DISTANCE_62CH;
extern double RECEIPTION_LEVEL;
extern double MAP_SCALE;
extern char weekChar[7][2][4], str[1000], *fstr;
extern size_t fsize;
extern int BSchannel[12],BSstation[12];

struct String{
	char str[2][200];
	bool head[2][200];
};
extern String text[1000];
char* toChar(String s);
void drawText(SDL_Surface* scr, int X, int Y, String st);
void drawText(SDL_Surface* scr, int x, int y, String str, int strl);
void drawText(SDL_Surface* scr, int x, int y, String str, int strl, Uint8 a);
void drawText_lang(SDL_Surface* scr, int x, int y, String st, int l, Uint8 a, int lang);
void drawText2(SDL_Surface* scr, int X, int Y, String st);
void drawText2(SDL_Surface* scr, int x, int y, String str, int strl);
void drawText2(SDL_Surface* scr, int x, int y, String str, int strl, Uint8 a);
void drawText2_lang(SDL_Surface* scr, int x, int y, String st, int l, Uint8 a, int lang);
void putHeadMark(String &s);
String jummingText(String s, int strl, int rcv, int mg_rcv);
void padSpace(String &st, int x);

struct Season{
	String name;
	Uint8 rate;
	int id,hiscore;
};
extern Season *season;

struct Mhz{
	int mhz,dis;
	double diffract,directivity,shadow_length,magnify_sight_distance;
};
extern Mhz *mhz;

class Menu{
	String *st;
	Uint8 v,width,raw,bg,combo,open_count,*R,*G,*B;
	Uint16 *mark;
	int show,pageOff,head,lim,x,y;
	void drawAnimation(SDL_Surface* scr);
public:
	bool *gray;
	int lang;
	Menu(){lim=0;}
	void setMenu(int X, int Y, int w, int r, int n);
	void setBG(int a);
	void setViewMode(int a);
	void setCombo(int a);
	void animation();
	void drawMenu(SDL_Surface* scr);
	void drawColorMenu(SDL_Surface* scr);
	void drawMenuText(SDL_Surface* scr);
	void stack(const char *c);
	void input(int n, char *c);
	void insert(int n, char *c);
	void stack(String s);
	void input(int n, String s);
	void insert(int n, String s);
	void delet(int n);
	void swap(int n1, int n2);
	void inputMark(int n, int m);
	void inputColor(int n, Uint8 r, Uint8 g, Uint8 b);
	Uint8 getR(int n);
	Uint8 getG(int n);
	Uint8 getB(int n);
	void cursorUp();
	void cursorDown();
	void pageUp();
	void pageDown();
	int selected();
	int count();
	void reset();
};

struct Station{
	String name,talk;
	Uint16 mark;
	int id,ontv;
};

struct Program{
	Uint8 week,hour,minute;
	int id,season_index,station_index,time,cartoon_index;
};

struct Entry{
	String title;
	char query[600];
	int mark,prg_num,cartoon_id,cartoon_index;
	Program *prg;
	Uint8 r,g,b;
	bool exist;
};

struct Town{
	String name;
	Uint16 x,y;
	int id,area_index;
};

struct Tower{
	String name;
	Uint8 ch[10], onair_num, colorlight_size, bias, r_num;
	double kw, erp;
	int rcv[10];
	int id,area_index;
	short r_h[20];
	Uint16 x, y, h, x_3d, y_3d, r_dis[20],r_m[20];
	float power[10];
	float dir;
	bool remove, v;
	Uint8 out[10];
};

struct Area{
	String name;
	Uint8 st_num, button[10];
	int id,tower_num,town_num,station[10];
	Tower *tower;
	Town *town;
};

struct Mount{
	Uint16 x, y, h, slope;
	Uint8 range, city;
	int id;
	bool volcano;
	float dis, dir, dir1, dir2;
};

struct Areacode{
	String name;
	int id;
	Uint8 code[12];
};
extern Areacode *areacode;

struct Timeslot{
	int id, season_index;
	Uint8 week,hour,minute,type;
};
extern Timeslot *timeslot;

extern int areacode_num, allofworks_num, timeslot_num;
extern Entry *allofworks;

struct Key{
	Uint8 left,right,up,down,z,x,c,a,F1,F4,F5,F10;
	int leftC,rightC,upC,downC,zC,xC,cC,aC,count,pressed;
};

struct Image{
	Uint32 *RGB;
	Uint8 *A;
	int w,h;
	Image(int W, int H);
};

struct Fish{
	Uint8 ch,hour,minute,week,rcv,mg_rcv;
	int x,y,which_work,sta,area,tower,score;
	bool bs;
};

class FishBox{
	Fish *fish;
	Uint8 preR,preG,preB;
	int n,cx,cy,offset,max;
	Image *mold;
public:
	Image *panel;
	int *today;
	int text_count;
	void initAll();
	void endAll();
	void initFishBox(int a);
	void endFishBox();
	void setFish(Fish f);
	void drawFishBox(SDL_Surface* scr);
	void drawTable(SDL_Surface* scr, Fish f);
	void panelColor(int r, int g, int b);
	void panelColor(int N);
	void cursorUp();
	void cursorDown();
	void cursorLeft();
	void cursorRight();
	int getNum();
	int getX();
	int getY();
	int getH();
	int getM();
	int getSC();
	int getSC(int i);
	int getRCV(int i);
	int getData(int i, int k);
	bool loaded();
	void setData(int i, int k, int data);
};

struct SoundFile{
	Mix_Chunk *decide, *cursor_move, *decide2, *coin, *swish, *get, *lamp, *bubble;
	Mix_Chunk *snore, *fw_sound, *noon, *sunset, *hold, *drag_s, *calling;
	Mix_Chunk *water, *thunder, *grumble, *meow, *noize, *bupyo, *knob, *alarm, *gaze;
};
extern SoundFile sf;

struct ImageFile{
	SDL_Surface *iwa, *screen;
	Image *back, *chr, *menuback, *facechip, *keyboard, *symbol, *pre_scr;
	Image *rod, *fishup, *colorlight, *tv_asahi;
	Image *searchImage;
	Uint8 alphaR, alphaG, alphaB;
};
extern ImageFile img;

void drawText(Image* img, int X, int Y, const char* st);
void drawText(Image* img, int X, int Y, const char* st, int strl);
void drawText(Image* img, int X, int Y, const char* st, int strl, Uint8 a);
void drawText_lang(Image* img, int x, int y, const char* st, int l, Uint8 a, int lang);
void drawText(Image* img, int X, int Y, String st);
void drawText(Image* img, int x, int y, String str, int strl);
void drawText(Image* img, int x, int y, String str, int strl, Uint8 a);
void drawText_lang(Image* img, int x, int y, String st, int l, Uint8 a, int lang);
void drawText2(Image* img, int X, int Y, const char* st);
void drawText2(Image* img, int X, int Y, const char* st, int strl);
void drawText2(Image* img, int X, int Y, const char* st, int strl, Uint8 a);
void drawText2_lang(Image* img, int x, int y, const char* st, int l, Uint8 a, int lang);
void drawText2(Image* img, int X, int Y, String st);
void drawText2(Image* img, int x, int y, String str, int strl);
void drawText2(Image* img, int x, int y, String str, int strl, Uint8 a);
void drawText2_lang(Image* img, int x, int y, String st, int l, Uint8 a, int lang);
void getImage(Image*& img, const char* st, int alpha);
void getImage(Image*& im, char* st);
void freeImage(Image*& im);
void freeMusic();
void freeSound(Mix_Chunk*& s);
void drawRect(SDL_Surface* scr, int x, int y, int w, int h, int R, int G, int B, int a);
void drawRect(Image* scr, int x, int y, int w, int h, int R, int G, int B, int a);
void setAlpha(Image* scr, int R,int G,int B);
void resetAlpha(Image* scr);
void drawSurface(SDL_Surface* scr, SDL_Surface* img, int x, int y, int x2, int y2, int w2, int h2, int a);

void strcpy_no_const(char* destination, char* source, size_t size);

#ifndef __WIN32__
void sprintf_s(char *s, const char *c, ...);
void sprintf_s(char *s, long n, const char *c, ...);
void fopen_s(FILE **f, const char* c1, const char* c2);
void strcpy_s(char *s1, const char *s2);
void strcpy_s(char *s1, long n, const char *s2);
#endif

extern Key key;
extern Station *sta;
extern Program *prg;
extern Entry *entry;
extern Area *area;
extern Tower *tower;
extern Mount *mount;
extern Town *town;
extern Menu menu[20];
extern FishBox fishbox;
extern Fish tmp_fish;
extern Image *font[47];
extern Mix_Music *bgm;
extern SDL_Event ev;

#include "file_io.h"
#include "antenna.h"
#include "title.h"
#include "gamemenu.h"
#include "game.h"
#include "radio.h"
#include "ending.h"
#include "option.h"
#include "miyazaki.h"
#include "haziashop.h"
#include "map_drawing.h"
#include "animation.h"
#include "drawimage.h"
#include "image_search.h"
#include "sql.h"

#endif
