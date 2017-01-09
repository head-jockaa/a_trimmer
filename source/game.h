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
#define CROP 33
#define PREF_LIST_SMR 34
#define SMR_RESULT 35
#define END_YN 36
#define END_RECORD 37
#define SAVING 38
#define SAVING_RECORD 39
#define LEAVE_MAP 40
#define PREF_LIST_TRANS 41
#define MANEKI_CONFIRM 42
#define MANEKI_FISHUP 43
#define MANEKI_GRADEUP 44
#define ADJUST_HEIGHT 45
#define ADJUST_ZOOM 46
#define GAME_OPTION_SETTING 47
#define GAME_KEY_SETTING 48
#define THROW_PHOTO 49
#define MANEKI_THROW_PHOTO 50
#define BS_THROW_PHOTO 51
#define CARRYING 1
#define PLUGGED_IN 2
#define MISPLACED 3

#define HAZIA_DEALER_COMES 52
#define COUNTING_HAZIA 53
#define FINISH_COUNTING_HAZIA 54

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
bool in_time(int w, int h, int m, int t);
void setManekiData();
void makeColorLight();
void walking();
void startThread(int id, char *query);
void manageThread();
void drawNetworkStatus(SDL_Surface* scr);
void drawThrowPhoto(SDL_Surface *scr);
bool createSearchImage(int n);

extern int mwave[68];

struct GameData{
	int scrX,scrY,startX,startY,save_cursor,player_dir,ant_dir,hazia,hazia2;
	int second,score,get_score,gradeup,crops,m_wave,firework,this_rural_rate,show_rural_rate,show_rural_area;
	int current_area,current_town;
	int ta_count,town_count;
	int talk_kulisap,face_kulisap,kirby_count,kirby_count2,timeslot_count,timeslot_type;
	int summerX_start,summerY_start,summerX,summerY,memmaX,memmaY,memma_count;
	int bgm_timestamp;
	double dotX,dotY;
	Uint8 week,minute,hour,sunset_hour,sunrise_hour,game_mode;
	Uint8 menu_selected,antenna_selected,option_selected,mainmenu_selected;
	int bs_ch;
	int zoom,height;
	double x,y,speed,real_speed,speedX,speedY;
	bool doze,m_waved,rural_shown,kulisaped,memma_type,bought[18];
	bool secondMusic;
	Uint8 randomNumber;
};
extern GameData gd;

struct ManekiData{
	Fish fish[300];
	Uint8 maneki_mode;
	bool bs[300];
	double manekiX,manekiY;
	int maneki_count[300],fish_num;
	int ch[300],area[300],tower[300],station[300],rcv[300],mg_rcv[300],st_num;
};
extern ManekiData md;

#endif
