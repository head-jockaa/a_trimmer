#ifndef GAME__
#define GAME__

#include "util.h"

#define MENU 0
#define BS_CH 2
#define SAVEMENU 3
#define GAME_OPTION 4
#define ANTENNA_MENU 5
#define PREF_LIST 6
#define SMR 7
#define TOWN_LIST 8
#define MANEKI 9
#define READY 20
#define GAMESTART 21
#define ANTENNA 22
#define PLAYING 23
#define CALLING 24
#define TALKING 25
#define FISHUP 26
#define GRADEUP 27
#define BS_ATTACK 28
#define FINISH 29
#define TODAYS_CROP 30
#define RESULT 31
#define GET_HAZIA 32
#define CROP 33
#define PREF_LIST_SMR 34
#define SMR_RESULT 35
#define END_YN 36
#define END_RECORD 37
#define SAVING 38
#define SAVING_RECORD 39
#define LEAVE_MAP 40
#define PREF_LIST_TRANS 41
#define HIT_BOSS 42
#define SUMMERWARS 43
#define MANEKI_CONFIRM 44
#define MANEKI_FISHUP 45
#define MANEKI_GRADEUP 46
#define ADJUST_HEIGHT 47
#define ADJUST_ZOOM 48
#define GAME_OPTION_SETTING 49
#define GAME_KEY_SETTING 50
#define CARRYING 1
#define PLUGGED_IN 2

void setAntenna();
void initGame();
void initGame2();
void endGame();
void initBoss();
void keyGame();
void drawGame(SDL_Surface* scr);
void timerGame();
void drawTerop(SDL_Surface* scr, String str, int rcv, int mg_rcv);
void drawM(SDL_Surface* scr, int w, int m, int x, int y);
int getScore(int n, double pw, int x, int y);
BOOL in_time(int w, int h, int m, int t);
void head_of_talking();
void head_of_timeslot();
void setManekiData();
void makeColorLight();
void walking();

extern int mwave[68];

struct GameData{
	int scrX,scrY,startX,startY,save_cursor,player_dir,ant_dir,hazia,hazia2;
	int second,score,get_score,gradeup,crops,m_wave,firework,pre_rural,count_rural,location;
	int text_count,talk_count,face_count,talk_open_count,scene_count,shake_count,ta_count,town_count,timeslot_count;
	int talk_kulisap,face_kulisap,kirby_count,kirby_count2,gaze_count,gaze_num;
	int summerX_start,summerY_start,summerX,summerY,memmaX,memmaY,memma_count;
	double dotX,dotY;
	Uint8 week,minute,hour,sunset_hour,sunrise_hour,game_mode;
	Uint8 menu_selected,antenna_selected,option_selected,mainmenu_selected;
	Uint8 *timeslot;
	int bs_ch;
	int zoom,height;
	double x,y,speed,real_speed,speedX,speedY;
	BOOL doze,m_waved,kulisaped,bought[18],memma_type;
};
extern GameData gd;

struct ManekiData{
	Fish fish[300];
	BOOL maneki_mode, bs[300];
	double manekiX,manekiY;
	int maneki_count[300],fish_num;
	int ch[300],tower[300],station[300],rcv[300],mg_rcv[300],st_num;
};
extern ManekiData md;

struct BossData{
	int bossHP,pre_bossHP,bossX,bossY,talking;
	int atk_count[100],station[100],tower[100],color[100],num,damage;
	double hitX,hitY,hit_speedX,hit_speedY;
	double atkX[100],atkY[100],atx_speedX[100],atx_speedY[100];
};
extern BossData bd;

#endif