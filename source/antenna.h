#ifndef ANTENNA__
#define ANTENNA__

#include "util.h"

#define TUNE 0
#define TURN 1
#define TROLL 2

class Antenna{
protected:
	BOOL v;
	Uint8 ch,m_wave2;
	int rcv,mg_rcv, A,T,C;
	void setTmpFish();
	void receive();
	void catching();
	void drawExplain2(SDL_Surface* scr);
	void drawAntennaMode(SDL_Surface* scr);
	void drawBuoy(SDL_Surface* scr);
	void drawWaterBall(SDL_Surface* scr);
	void drawRadioWaveMeter(SDL_Surface* scr);
	void drawMountainHeight(SDL_Surface* scr);
	void drawDirectionLine(SDL_Surface* scr);
public:
	Uint8 ant_mode,change_mode_count;
	int station;
	void turnL();
	void turnR();
	void rader();
	void drawChannel(SDL_Surface* scr);
	void drawRader(SDL_Surface* scr);
	void drawTable(SDL_Surface* scr);
	virtual ~Antenna(){}
	virtual void hoist()=0;
	virtual void lower()=0;
	virtual void pushU()=0;
	virtual void pushD()=0;
	virtual void pushL()=0;
	virtual void pushR()=0;
	virtual void pushButton()=0;
	virtual void trolling()=0;
	virtual void drawExplain(SDL_Surface* scr)=0;
	virtual void drawAntenna(SDL_Surface* scr)=0;
	virtual void drawTrolling(SDL_Surface* scr)=0;
};

class SimpleRod : public Antenna{
	int *rc;
public:
	SimpleRod();
	~SimpleRod();
	void hoist();
	void lower();
	void pushD();
	void pushU();
	void pushL();
	void pushR();
	void pushButton();
	void trolling();
	void drawExplain(SDL_Surface* scr);
	void drawAntenna(SDL_Surface* scr);
	void drawTrolling(SDL_Surface* scr);
};

class ButtonRod : public Antenna{
	Uint8 push, set, b_num, button[12], button2[12], **codes;
	String *areaname;
	int *rc, *st;
	size_t area_num, code;
	void code_set();
	void auto_set();
	void set_button();
	void ch_up();
	void ch_down();
public:
	ButtonRod();
	~ButtonRod();
	void hoist();
	void lower();
	void pushD();
	void pushU();
	void pushL();
	void pushR();
	void pushButton();
	void trolling();
	void drawExplain(SDL_Surface* scr);
	void drawAntenna(SDL_Surface* scr);
	void drawTrolling(SDL_Surface* scr);
};

class UVRod : public Antenna{
	Uint8 vhf,uhf,part;
	int net[10],net2[10];
	Uint8 *st;
	void setCH();
public:
	UVRod();
	~UVRod();
	void hoist();
	void lower();
	void pushD();
	void pushU();
	void pushL();
	void pushR();
	void pushButton();
	void trolling();
	void drawExplain(SDL_Surface* scr);
	void drawAntenna(SDL_Surface* scr);
	void drawTrolling(SDL_Surface* scr);
};

class MHzRod : public Antenna{
	int mh, *rc;
public:
	MHzRod();
	~MHzRod();
	void hoist();
	void lower();
	void pushD();
	void pushU();
	void pushL();
	void pushR();
	void pushButton();
	void trolling();
	void drawExplain(SDL_Surface* scr);
	void drawAntenna(SDL_Surface* scr);
	void drawTrolling(SDL_Surface* scr);
};

class ConvenientRod : public Antenna{
	void makeList();
	void make_st_list();
protected:
	int *rc, *st, *tw, *chn, *mr, st_num;
	Menu st_list;
public:
	ConvenientRod();
	~ConvenientRod();
	void hoist();
	void lower();
	void pushD();
	void pushU();
	void pushL();
	void pushR();
	void pushButton();
	void setTable();
	void trolling();
	void drawExplain(SDL_Surface* scr);
	void drawAntenna(SDL_Surface* scr);
	void drawTrolling(SDL_Surface* scr);
};

class SuperHandyRod : public ConvenientRod{
	void makeList2();
	void make_st_list2();
	int rc2[16], tw2[16];
public:
	SuperHandyRod();
	void hoist();
	void trolling();
	void drawAntenna(SDL_Surface* scr);
	void drawTrolling(SDL_Surface* scr);
};

extern class Antenna *ant;

#endif