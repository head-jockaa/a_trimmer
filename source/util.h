#ifndef UTILITY__
#define UTILITY__

#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_net.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string>
#include <iostream>

#pragma comment(lib, "SDL.lib")
#pragma comment(lib, "SDLmain.lib")
#pragma comment(lib, "SDL_image.lib")
#pragma comment(lib, "SDL_mixer.lib")
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
#define BOSS 4
#define HIDE 0
#define GRAY 1
#define VISIBLE 2
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
int to32int(char c1, char c2, char c3, char c4);
bool key_wait(int a);
bool key_stop(int a);
void TextOut(SDL_Surface* scr, int X, int Y, char* st);
void TextOut(SDL_Surface* scr, int X, int Y, char* st, int strl);
void TextOut_lang(SDL_Surface* scr, int x, int y, char* st, int l, int lang);
void TextOut2(SDL_Surface* scr, int X, int Y, char* st);
void TextOut2(SDL_Surface* scr, int X, int Y, char* st, int strl);
void TextOut2_lang(SDL_Surface* scr, int x, int y, char* st, int l, int lang);
void getSymbolImage();
void TalkingAt(int n);
void drawKeyboard(SDL_Surface* scr, int k, int X, int Y);
void drawKeyboard2(SDL_Surface* scr, int k, int X, int Y);
bool loadFile(char* fn);
void fix_scrXY();
void fix_XY();
void reset_key();
void initFont();
void endFont();
void setKick(SDL_Surface* scr);
void drawKick(SDL_Surface* scr);
void padSpace(int n, int k, int x);
int controlTalking();
void controlTextCount(bool ok);
inline Uint32 setRGB(Uint8 r, Uint8 g, Uint8 b);
inline void getRGB(Uint32 px, Uint8 *r, Uint8 *g, Uint8 *b);

extern SDL_Rect scr;
extern bool pause,run,setSMR,map_loaded,*animebook,ABGR;
extern Uint8 mode,mode2,dataNo,phase,kick_count;
extern int stas,works,prgs,allworks,collection,areas,towers,mounts,towns,index_num,clear_num,mapW,mapH;
extern int count,bg_count,face[1000],start;
extern int talk_kulisap,face_kulisap,ant_dir;
extern Uint8 fontA;
extern Uint8 CHAR_CODE,AIR_IMG,WALKING_TYPE,SHOW_TOWER,ROD_TYPE,ADJ_DIR,CHANNELS,EXPLAIN,FULLSCR,SCRSIZE,BGM_VOLUME,SE_VOLUME,CHOSEON,CURVE_TOP,MAGNIFY,MAP3D,DASH_TYPE,NHK_REMOVE;
extern double CURVE_RISE;
extern double test;
extern double DIS1CH; // 1Ch 500kw �̓d�g����ԋ���
extern double DIS62CH; // 
extern double RCV_LEVEL; //
extern double MAP_SCALE; //
extern char weekChar[7][2][4], str[1000], *fstr;
extern size_t fsize;
extern int BSchannel[12],BSstation[12];

struct String{
	char str[2][200];
	bool head[2][200];
};
extern String text[1000],talk[1000];
char* toChar(String s);
void TextOut(SDL_Surface* scr, int X, int Y, String st);
void TextOut(SDL_Surface* scr, int x, int y, String str, int strl);
void TextOut_lang(SDL_Surface* scr, int x, int y, String st, int l, int lang);
void TextOut2(SDL_Surface* scr, int X, int Y, String st);
void TextOut2(SDL_Surface* scr, int x, int y, String str, int strl);
void TextOut2_lang(SDL_Surface* scr, int x, int y, String st, int l, int lang);
void putHeadMark(String &s);
String jummingText(String s, int strl, int rcv, int mg_rcv);

struct Index{
	String name;
	Uint8 rate;
	int hiscore;
};
extern Index *indexName;

struct Mhz{
	int mhz,dis;
	double dif,ori,shadow,ground;
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
	void stack(char *c);
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
	int ontv;
};

struct Prg{
	Uint8 week,hour,minute;
	int station,time,work;
};

struct Work{
	String title;
	char query[600];
	int mark,prg,num,tnum;
	Uint8 r,g,b;
	bool notExist;
};

struct Area{
	String name;
	Uint8 st_num, button[10];
	int tower,num,town,town_num,station[10];
};

struct Tower{
	String name;
	Uint8 ch[10], kw, kw2, erp, erp2, c1, c2, bias, r_num;
	int rcv[10];
	int area;
	short r_h[20];
	Uint16 x, y, h, x_3d, y_3d, work[10], r_dis[20],r_m[20];
	float power[10];
	float dir;
	bool out[10],remove,v;
};

struct Mount{
	Uint16 x, y, h, slope;
	Uint8 range, city;
	bool volcano;
	float dis, dir, dir1, dir2;
};

struct Town{
	String name;
	Uint16 x,y;
};

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
	int x,y,title_num,sta,tower,score;
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
	int getSC(int i);
	int getData(int i, int k);
	bool loaded();
	void setData(int i, int k, int data);
};

struct SoundFile{
	Mix_Chunk *decide, *cursor_move, *decide2, *coin, *swish, *get, *lamp, *bubble;
	Mix_Chunk *snore, *fw_sound, *noon, *sunset, *hold, *drag_s, *calling, *grumble;
	Mix_Chunk *water, *thunder, *meow, *noize, *bupyo, *knob, *alarm, *tub, *gaze;
};
extern SoundFile sf;

struct ImageFile{
	SDL_Surface *iwa, *screen, *screen2;
	Image *back, *chr, *menuback, *facechip, *boss, *keyboard, *symbol, *pre_scr;
	Image *rod, *circle, *fishup, *scr2, *colorlight, *tv_asahi;
	Image *buffer[4], *photo[14];
	Image *searchImage;
	Uint8 alphaR, alphaG, alphaB;
};
extern ImageFile img;

void TextOut(Image* img, int X, int Y, char* st);
void TextOut(Image* img, int X, int Y, char* st, int strl);
void TextOut_lang(Image* img, int x, int y, char* st, int l, int lang);
void TextOut(Image* img, int X, int Y, String st);
void TextOut(Image* img, int x, int y, String str, int strl);
void TextOut_lang(Image* img, int x, int y, String st, int l, int lang);
void TextOut2(Image* img, int X, int Y, char* st);
void TextOut2(Image* img, int X, int Y, char* st, int strl);
void TextOut2_lang(Image* img, int x, int y, char* st, int l, int lang);
void TextOut2(Image* img, int X, int Y, String st);
void TextOut2(Image* img, int x, int y, String str, int strl);
void TextOut2_lang(Image* img, int x, int y, String st, int l, int lang);
void setColorKey(Image *img, Uint8 r, Uint8 g, Uint8 b);
void getImage(Image*& img, char* st, int r, int g, int b);
void getImage(Image*& im, char* st);
void freeImage(Image* img);
void drawTalking(SDL_Surface* scr);
void drawTalking(SDL_Surface* scr, int fc, String st);
void drawRect(SDL_Surface* scr, int x, int y, int w, int h, int R, int G, int B, int a);
void drawRect(Image* scr, int x, int y, int w, int h, int R, int G, int B, int a);
void fillRect(SDL_Surface* scr, int x, int y, int w, int h, int R, int G, int B, int a);
void fillRect(Image* scr, int x, int y, int w, int h, int R, int G, int B, int a);
void setAlpha(Image* scr, int R,int G,int B);
void resetAlpha(Image* scr);
void drawSurface(SDL_Surface* scr, SDL_Surface* img, int x, int y, int x2, int y2, int w2, int h2, int a);
void drawLight(SDL_Surface* scr, Image* img, int x, int y, int x2, int y2, int w2, int h2,int a);
void drawLight(Image* scr, Image* img, int x, int y, int x2, int y2, int w2, int h2, int a);

#ifdef __APPLE__
void sprintf_s(char *s, char *c, ...);
void sprintf_s(char *s, int n, char *c, ...);
void fopen_s(FILE **f, char* c1, char* c2);
void strcpy_s(char *s1, char *s2);
#endif

extern Key key;
extern Station *sta;
extern Prg *prg;
extern Work *work;
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

#endif