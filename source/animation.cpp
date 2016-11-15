#include "animation.h"

#define CARTOON_IMAGE 1
#define CARTOON_BGM 2
#define CARTOON_SET 3
#define CARTOON_MOVE 4
#define CARTOON_RESET 5
#define CARTOON_NOTE 6
#define CARTOON_ID 7
#define CARTOON_X 8
#define CARTOON_Y 9
#define CARTOON_IX 10
#define CARTOON_IY 11
#define CARTOON_W 12
#define CARTOON_H 13
#define CARTOON_ALPHA 14
#define CARTOON_MAG 15
#define CARTOON_COL_GRAD_Y 16
#define CARTOON_COL 17
#define CARTOON_SHAKE 18
#define CARTOON_WAVE 19
#define CARTOON_SLIDE 20
#define CARTOON_FLIP 21
#define CARTOON_JUMP 22
#define CARTOON_FROM 23
#define CARTOON_TO 24
#define CARTOON_SIN 25
#define CARTOON_RANGE 26
#define CARTOON_INTERVAL 27
#define CARTOON_NUM 28
#define CARTOON_STEP 29
#define CARTOON_FADE 30
#define CARTOON_G 31
#define CARTOON_DRAWIMAGE 32
#define CARTOON_FILLRECT 33
#define CARTOON_FILLRECT_GRAD 34
#define CARTOON_DRAWIMAGE_WAVE 35
#define CARTOON_CACHE 36
#define CARTOON_CACHE_TO 37
#define CARTOON_TURNBACK 38
#define CARTOON_COL_R 39
#define CARTOON_COL_G 40
#define CARTOON_COL_B 41
#define CARTOON_POS 42
#define CARTOON_LANG 43
#define CARTOON_FILE 44
#define CARTOON_LOAD_TEXT 45
#define CARTOON_TALK 46
#define CARTOON_FACE 47
#define CARTOON_TEXT_ID 48
#define CARTOON_IMG_ID 49
#define CARTOON_END 50
#define CARTOON_REPEAT 51
#define CARTOON_LOAD_SOUND 52
#define CARTOON_SOUND 53
#define CARTOON_CHANNEL 54

#define JSON_ARRAY_START 1
#define JSON_STARTTIME 2
#define JSON_GETTIME 3
#define JSON_DATA_START 4
#define JSON_GETNAME 5
#define JSON_COLON 6
#define JSON_GETVALUE 7
#define JSON_NEXTDATA 8
#define JSON_ENDDATA 9
#define JSON_ENDTIME 10
#define JSON_DATA_START2 11
#define JSON_GETNAME2 12
#define JSON_COLON2 13
#define JSON_GETVALUE2 14

void readSetObject(char *json, int timer);
void readMoveObject(char *json, int timer);
void readSlideAnime(char *json, int timer);
void readFlipAnime(char *json, int timer);
void readJumpAnime(char *json, int timer);
void readColorGrad(char *json, int timer);
void readColorGrad_shift(char *json, int timer);
void readStoryFile(char *json, int timer);
void readCartoonTalk(char *json, int timer);
void readCartoonImage(char *json, int timer);
void readCartoonBgm(char *json, int timer);
void readCartoonLoadSound(char *json, int timer);
void readCartoonSound(char *json, int timer);
void readWaveAnime(char *json, int timer);
void resetObject(char *json, int timer);

int playtime,cartoonNextTime;
int timestamp,loadtime,pausetime;
int max_obj,this_id;
bool talkmode;
size_t cartoonJsonSize,cartoonPointer;
char *cartoonJson;

struct ObjectSetting{
	double x,y,ix,iy,w,h,mag,alpha,shake;
	int type,cacheTo,lang,img_id;
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
CartoonObject obj[1000];

void resetObjectSetting(ObjectSetting &s){
	s.x=0;s.y=0;s.ix=0;s.iy=0;s.w=0;s.h=0;
	s.mag=1;s.alpha=255;s.shake=0;s.type=0;s.cacheTo=-1;s.lang=-1;
	s.R=0;s.G=0;s.B=0;s.img_id=0;
	s.gradRfrom=0;s.gradGfrom=0;s.gradBfrom=0;
	s.gradRto=0;s.gradGto=0;s.gradBto=0;
}
void resetObjectMoving(ObjectMoving &m){
	m.x=0;m.y=0;m.ix=0;m.iy=0;m.mag=0;m.w=0;m.h=0;m.alpha=0;m.shake=0;
	m.R=0;m.G=0;m.B=0;
	m.gradRfrom=0;m.gradGfrom=0;m.gradBfrom=0;
	m.gradRto=0;m.gradGto=0;m.gradBto=0;
}
void resetObjectFlipping(ObjectFlipping &f){
	f.ix=0;f.iy=0;f.interval=1;f.num=0;f.count=0;f.turnBack=true;
}
void resetObjectSliding(ObjectSliding &s){
	s.step=0;s.from=0;s.to=0;s.fade=1;s.fadeRate=0;s.turnBack=true;
	s.back=false;s.position=0;
}
void resetObject(int n){
	resetObjectSetting(obj[n].set);
	resetObjectMoving(obj[n].move);
	resetObjectFlipping(obj[n].flip);
	resetObjectSliding(obj[n].slideX);
	resetObjectSliding(obj[n].slideY);
	resetObjectSliding(obj[n].slideIX);
	resetObjectSliding(obj[n].slideIY);
	resetObjectSliding(obj[n].slideMag);
	resetObjectSliding(obj[n].slideAlpha);
	obj[n].jumpG=0;obj[n].jumpY=0;
	obj[n].waveSIN=0;obj[n].waveRange=0;
}

int hextoint(char c){
	switch(c){
		case '0': return 0;
		case '1': return 1;
		case '2': return 2;
		case '3': return 3;
		case '4': return 4;
		case '5': return 5;
		case '6': return 6;
		case '7': return 7;
		case '8': return 8;
		case '9': return 9;
		case 'a': case 'A': return 10;
		case 'b': case 'B': return 11;
		case 'c': case 'C': return 12;
		case 'd': case 'D': return 13;
		case 'e': case 'E': return 14;
		case 'f': case 'F': return 15;
		default: return 0;
	}
}

int fetchInt(char *&s) {
	int value=0,minus=1;
	while(*s && *s!=',' && *s!=']' && *s!='}') {
		if(*s=='-') {
			minus=-1;
		}else{
			value*=10;
			value+=(*s-48);
		}
		cartoonPointer++;
		s++;
	}
	cartoonPointer--;
	return value*minus;
}

double fetchDouble(char *&s) {
	double value=0,digit=1;
	int minus=1;
	bool decimal=false;
	while(*s && *s!=',' && *s!=']' && *s!='}') {
		if(*s=='-') {
			minus=-1;
		}
		else if(*s=='.') {
			decimal=true;
			digit=0.1;
		}
		else if(decimal) {
			value += (*s-48)*digit;
			digit*=0.1;
		}
		else{
			value*=10;
			value+=(*s-48);
		}
		cartoonPointer++;
		s++;
	}
	cartoonPointer--;
	return value*minus;
}

void fetchString(char *&s, char *value) {
	bool start=false;
	int pointer=0;
	while(*s) {
		if(start) {
			if(*s=='"') {
				value[pointer]=0;
				break;
			}else{
				value[pointer]=*s;
				pointer++;
			}
		}
		else if(*s=='"') {
			start=true;
		}
		cartoonPointer++;
		s++;
	}
	cartoonPointer--;
}

void fetchColor(char *&s, SDL_Color *col) {
	int a=0;
	bool start=false;
	while(*s && *s!=',' && *s!=']' && *s!='}' && a<6) {
		if(*s=='"'){
			start=true;
		}
		else if(start){
			if(a==1){
				sprintf_s(str,"%d",hextoint(*(s-1))*16+hextoint(*s));
				col->r=hextoint(*(s-1))*16+hextoint(*s);
			}
			else if(a==3){
				sprintf_s(str,"%d",hextoint(*(s-1))*16+hextoint(*s));
				col->g=hextoint(*(s-1))*16+hextoint(*s);
			}
			else if(a==5){
				sprintf_s(str,"%d",hextoint(*(s-1))*16+hextoint(*s));
				col->b=hextoint(*(s-1))*16+hextoint(*s);
			}
			a++;
		}
		cartoonPointer++;
		s++;
	}
	cartoonPointer--;
}

bool startsWith(char *s, const char *target) {
	for(size_t i=0 ; i<strlen(target) ; i++) {
		if(s[i]!=target[i]) {
			return false;
		}
	}
	cartoonPointer+=(int)strlen(target)-1;
	return true;
}

void freeCartoon(){
	max_obj=0;
	playtime=0;
	cartoonPointer=0;
	cartoonNextTime=0;
	talkmode=false;
	if(cartoonJsonSize)delete [] cartoonJson;
	for(int i=0 ; i<1000 ; i++){
		resetObject(i);
	}
	for(int i=0 ; i<10 ; i++){
		freeImage(img.bg[i]);
		freeSound(sf.sound[i]);
	}
	freeMusic();
}

void loadCartoon(const char *filename){
	freeCartoon();
	loadFile(filename);
	cartoonJsonSize=fsize;
	cartoonJson=new char[cartoonJsonSize];
	strcpy_s(cartoonJson,fstr);
	nextCut();
	loadtime=SDL_GetTicks();
}

bool readCartoon(char *json, int timer){
	int mode=JSON_ARRAY_START;
	int datamode=0;
	bool endThisTime=false;

	if(talkmode) {
		mode=JSON_GETVALUE;
	}
	else if(cartoonPointer!=0) {
		mode=JSON_DATA_START;
	}

	while(cartoonPointer < cartoonJsonSize) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_ARRAY_START) {
			if(*c=='[') {
				mode=JSON_STARTTIME;
			}
		}
		else if(mode==JSON_STARTTIME) {
			if(*c=='{') {
				mode=JSON_GETTIME;
			}
		}
		else if(mode==JSON_GETTIME) {
			if(*c==',') {
				mode=JSON_DATA_START;
			}else{
				cartoonNextTime=fetchInt(c);
			}
		}
		else if(mode==JSON_DATA_START) {
			if(endThisTime){
				return false;
			}
			else if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			fetchString(c,str);
			if(strcmp(str, "image")==0) {
				datamode=CARTOON_IMAGE;
			}
			else if(strcmp(str, "bgm")==0) {
				datamode=CARTOON_BGM;
			}
			else if(strcmp(str, "load-sound")==0) {
				datamode=CARTOON_LOAD_SOUND;
			}
			else if(strcmp(str, "sound")==0) {
				datamode=CARTOON_SOUND;
			}
			else if(strcmp(str, "load-text")==0) {
				datamode=CARTOON_LOAD_TEXT;
			}
			else if(strcmp(str, "set")==0) {
				datamode=CARTOON_SET;
			}
			else if(strcmp(str, "move")==0) {
				datamode=CARTOON_MOVE;
			}
			else if(strcmp(str, "talk")==0) {
				datamode=CARTOON_TALK;
			}
			else if(strcmp(str, "reset")==0) {
				datamode=CARTOON_RESET;
			}
			else if(strcmp(str, "end")==0) {
				datamode=CARTOON_END;
			}
			else if(strcmp(str, "note")==0) {
				datamode=CARTOON_NOTE;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(talkmode) {
				readCartoonTalk(json,timer);
				if(talkmode){
					cartoonPointer++;
					break;
				}
			}
			else if(datamode==CARTOON_IMAGE) {
				readCartoonImage(json,timer);
			}
			else if(datamode==CARTOON_BGM) {
				readCartoonBgm(json,timer);
			}
			else if(datamode==CARTOON_LOAD_SOUND) {
				readCartoonLoadSound(json,timer);
			}
			else if(datamode==CARTOON_SOUND) {
				readCartoonSound(json,timer);
			}
			else if(datamode==CARTOON_LOAD_TEXT) {
				readStoryFile(json,timer);
			}
			else if(datamode==CARTOON_SET) {
				readSetObject(json,timer);
			}
			else if(datamode==CARTOON_MOVE) {
				readMoveObject(json,timer);
			}
			else if(datamode==CARTOON_TALK) {
				readCartoonTalk(json,timer);
				if(talkmode){
					cartoonPointer++;
					break;
				}
			}
			else if(datamode==CARTOON_RESET) {
				resetObject(json,timer);
			}
			else if(datamode==CARTOON_END) {
				return true;
			}
			else if(datamode==CARTOON_NOTE) {
				fetchString(c, str);
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				mode=JSON_ENDDATA;
			}
		}
		else if(mode==JSON_ENDDATA) {
			if(*c=='}') {
				mode=JSON_ENDTIME;
			}
		}
		else if(mode==JSON_ENDTIME) {
			if(*c==',') {
				endThisTime=true;
				mode=JSON_STARTTIME;
			}
		}
		cartoonPointer++;
	}
	return false;
}

void readSetObject(char *json, int timer){
	int mode=JSON_ARRAY_START;
	int datamode=0;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_ARRAY_START) {
			if(*c=='[') {
				mode=JSON_DATA_START;
			}
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			if(*c=='}') {
				mode=JSON_ENDDATA;
				continue;
			}
			fetchString(c,str);
			if(strcmp(str, "id")==0) {
				datamode=CARTOON_ID;
			}
			if(strcmp(str, "img_id")==0) {
				datamode=CARTOON_IMG_ID;
			}
			else if(strcmp(str, "x")==0) {
				datamode=CARTOON_X;
			}
			else if(strcmp(str, "y")==0) {
				datamode=CARTOON_Y;
			}
			else if(strcmp(str, "ix")==0) {
				datamode=CARTOON_IX;
			}
			else if(strcmp(str, "iy")==0) {
				datamode=CARTOON_IY;
			}
			else if(strcmp(str, "wave")==0) {
				datamode=CARTOON_WAVE;
			}
			else if(strcmp(str, "w")==0) {
				datamode=CARTOON_W;
			}
			else if(strcmp(str, "h")==0) {
				datamode=CARTOON_H;
			}
			else if(strcmp(str, "a")==0) {
				datamode=CARTOON_ALPHA;
			}
			else if(strcmp(str, "mag")==0) {
				datamode=CARTOON_MAG;
			}
			else if(strcmp(str, "col_grad_y")==0) {
				datamode=CARTOON_COL_GRAD_Y;
			}
			else if(strcmp(str, "col")==0) {
				datamode=CARTOON_COL;
			}
			else if(strcmp(str, "shake")==0) {
				datamode=CARTOON_SHAKE;
			}
			else if(strcmp(str, "cache_to")==0) {
				datamode=CARTOON_CACHE_TO;
			}
			else if(strcmp(str, "cache")==0) {
				datamode=CARTOON_CACHE;
			}
			else if(strcmp(str, "lang")==0) {
				datamode=CARTOON_LANG;
			}
			else if(strcmp(str, "note")==0) {
				datamode=CARTOON_NOTE;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_ENDDATA) {
			if(*c==',') {
				mode=JSON_DATA_START;
			}
			else if(*c==']') {
				return;
			}
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(datamode==CARTOON_ID) {
				int id=fetchInt(c);
				if(max_obj<id) {
					max_obj=id;
				}
				this_id=id;
			}
			else if(datamode==CARTOON_IMG_ID) {
				obj[this_id].set.img_id=fetchDouble(c);
			}
			else if(datamode==CARTOON_X) {
				obj[this_id].set.x=fetchDouble(c);
			}
			else if(datamode==CARTOON_Y) {
				obj[this_id].set.y=fetchDouble(c);
			}
			else if(datamode==CARTOON_IX) {
				obj[this_id].set.ix=fetchDouble(c);
				obj[this_id].set.type=CARTOON_DRAWIMAGE;
			}
			else if(datamode==CARTOON_IY) {
				obj[this_id].set.iy=fetchDouble(c);
				obj[this_id].set.type=CARTOON_DRAWIMAGE;
			}
			else if(datamode==CARTOON_W) {
				obj[this_id].set.w=fetchDouble(c);
			}
			else if(datamode==CARTOON_H) {
				obj[this_id].set.h=fetchDouble(c);
			}
			else if(datamode==CARTOON_ALPHA) {
				obj[this_id].set.alpha=fetchDouble(c);
			}
			else if(datamode==CARTOON_MAG) {
				obj[this_id].set.mag=fetchDouble(c);
			}
			else if(datamode==CARTOON_COL_GRAD_Y) {
				readColorGrad(json,timer);
				obj[this_id].set.type=CARTOON_FILLRECT_GRAD;
			}
			else if(datamode==CARTOON_COL) {
				SDL_Color col;
				fetchColor(c,&col);
				obj[this_id].set.type=CARTOON_FILLRECT;
				obj[this_id].set.R=col.r;
				obj[this_id].set.G=col.g;
				obj[this_id].set.B=col.b;
			}
			else if(datamode==CARTOON_SHAKE) {
				obj[this_id].set.shake=fetchDouble(c);
			}
			else if(datamode==CARTOON_WAVE) {
				readWaveAnime(json,timer);
				obj[this_id].set.type=CARTOON_DRAWIMAGE_WAVE;
			}
			else if(datamode==CARTOON_CACHE_TO) {
				obj[this_id].set.cacheTo=fetchInt(c);
			}
			else if(datamode==CARTOON_CACHE) {
				if(startsWith(c,"true")){
					obj[this_id].set.type=CARTOON_CACHE;
				}
			}
			else if(datamode==CARTOON_LANG) {
				fetchString(c,str);
				if(strcmp(str, "jp")==0)obj[this_id].set.lang=JAPANESE;
				else if(strcmp(str, "en")==0)obj[this_id].set.lang=EUROPEAN;
			}
			else if(datamode==CARTOON_NOTE) {
				fetchString(c,str);
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				mode=JSON_ENDDATA;
			}
		}
		cartoonPointer++;
	}
}

void readMoveObject(char *json, int timer){
	int mode=JSON_ARRAY_START;
	int datamode=0;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_ARRAY_START) {
			if(*c=='[') {
				mode=JSON_DATA_START;
			}
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			if(*c=='}') {
				mode=JSON_ENDDATA;
				continue;
			}
			fetchString(c,str);
			if(strcmp(str, "id")==0) {
				datamode=CARTOON_ID;
			}
			else if(strcmp(str, "x")==0) {
				datamode=CARTOON_X;
			}
			else if(strcmp(str, "y")==0) {
				datamode=CARTOON_Y;
			}
			else if(strcmp(str, "mag")==0) {
				datamode=CARTOON_MAG;
			}
			else if(strcmp(str, "slide")==0) {
				datamode=CARTOON_SLIDE;
			}
			else if(strcmp(str, "flip")==0) {
				datamode=CARTOON_FLIP;
			}
			else if(strcmp(str, "jump")==0) {
				datamode=CARTOON_JUMP;
			}
			else if(strcmp(str, "a")==0) {
				datamode=CARTOON_ALPHA;
			}
			else if(strcmp(str, "ix")==0) {
				datamode=CARTOON_IX;
			}
			else if(strcmp(str, "iy")==0) {
				datamode=CARTOON_IY;
			}
			else if(strcmp(str, "w")==0) {
				datamode=CARTOON_W;
			}
			else if(strcmp(str, "h")==0) {
				datamode=CARTOON_H;
			}
			else if(strcmp(str, "shake")==0) {
				datamode=CARTOON_SHAKE;
			}
			else if(strcmp(str, "note")==0) {
				datamode=CARTOON_NOTE;
			}
			else if(strcmp(str, "col_grad_y")==0) {
				datamode=CARTOON_COL_GRAD_Y;
			}
			else if(strcmp(str, "col_r")==0) {
				datamode=CARTOON_COL_R;
			}
			else if(strcmp(str, "col_g")==0) {
				datamode=CARTOON_COL_G;
			}
			else if(strcmp(str, "col_b")==0) {
				datamode=CARTOON_COL_B;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_ENDDATA) {
			if(*c==',') {
				mode=JSON_DATA_START;
			}
			else if(*c==']') {
				return;
			}
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(datamode==CARTOON_ID) {
				int id=fetchInt(c);
				if(max_obj<id) {
					max_obj=id;
				}
				this_id=id;
			}
			else if(datamode==CARTOON_X) {
				obj[this_id].move.x=fetchDouble(c);
			}
			else if(datamode==CARTOON_Y) {
				obj[this_id].move.y=fetchDouble(c);
			}
			else if(datamode==CARTOON_MAG) {
				obj[this_id].move.mag=fetchDouble(c);
			}
			else if(datamode==CARTOON_ALPHA) {
				obj[this_id].move.alpha=fetchDouble(c);
			}
			else if(datamode==CARTOON_SLIDE) {
				readSlideAnime(json,timer);
			}
			else if(datamode==CARTOON_JUMP) {
				readJumpAnime(json,timer);
			}
			else if(datamode==CARTOON_IX) {
				obj[this_id].move.ix=fetchDouble(c);
			}
			else if(datamode==CARTOON_IY) {
				obj[this_id].move.iy=fetchDouble(c);
			}
			else if(datamode==CARTOON_W) {
				obj[this_id].move.w=fetchDouble(c);
			}
			else if(datamode==CARTOON_H) {
				obj[this_id].move.h=fetchDouble(c);
			}
			else if(datamode==CARTOON_SHAKE) {
				obj[this_id].move.shake=fetchDouble(c);
			}
			else if(datamode==CARTOON_FLIP) {
				obj[this_id].flip.count=0;
				readFlipAnime(json,timer);
			}
			else if(datamode==CARTOON_NOTE) {
				fetchString(c,str);
			}
			else if(datamode==CARTOON_COL_GRAD_Y) {
				readColorGrad_shift(json,timer);
			}
			else if(datamode==CARTOON_COL_R) {
				obj[this_id].move.R=fetchDouble(c);
			}
			else if(datamode==CARTOON_COL_G) {
				obj[this_id].move.R=fetchDouble(c);
			}
			else if(datamode==CARTOON_COL_B) {
				obj[this_id].move.R=fetchDouble(c);
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				mode=JSON_ENDDATA;
			}
		}
		cartoonPointer++;
	}
}

void readColorGrad(char *json, int timer){
	int mode=JSON_DATA_START;
	int datamode=0;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			if(*c=='}') {
				return;
			}
			fetchString(c,str);
			if(strcmp(str, "from")==0) {
				datamode=CARTOON_FROM;
			}
			else if(strcmp(str, "to")==0) {
				datamode=CARTOON_TO;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(datamode==CARTOON_FROM) {
				SDL_Color col;
				fetchColor(c,&col);
				obj[this_id].set.gradRfrom=col.r;
				obj[this_id].set.gradGfrom=col.g;
				obj[this_id].set.gradBfrom=col.b;
			}
			else if(datamode==CARTOON_TO) {
				SDL_Color col;
				fetchColor(c,&col);
				obj[this_id].set.gradRto=col.r;
				obj[this_id].set.gradGto=col.g;
				obj[this_id].set.gradBto=col.b;
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				return;
			}
		}
		cartoonPointer++;
	}
}

void readColorGrad_shift(char *json, int timer){
	int mode=JSON_DATA_START;
	int datamode1=0,datamode2=0;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			fetchString(c,str);
			if(strcmp(str, "from")==0) {
				datamode1=CARTOON_FROM;
			}
			else if(strcmp(str, "to")==0) {
				datamode1=CARTOON_TO;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_DATA_START2;
			}
		}
		else if(mode==JSON_DATA_START2) {
			if(*c=='{') {
				mode=JSON_GETNAME2;
			}
		}
		else if(mode==JSON_GETNAME2) {
			fetchString(c,str);
			if(strcmp(str, "col_r")==0) {
				datamode2=CARTOON_COL_R;
			}
			else if(strcmp(str, "col_g")==0) {
				datamode2=CARTOON_COL_G;
			}
			else if(strcmp(str, "col_b")==0) {
				datamode2=CARTOON_COL_B;
			}
			mode=JSON_COLON2;
		}
		else if(mode==JSON_COLON2) {
			if(*c==':') {
				mode=JSON_GETVALUE2;
			}
		}
		else if(mode==JSON_GETVALUE2) {
			if(datamode1==CARTOON_FROM) {
				if(datamode2==CARTOON_COL_R)obj[this_id].move.gradRfrom=fetchDouble(c);
				else if(datamode2==CARTOON_COL_G)obj[this_id].move.gradGfrom=fetchDouble(c);
				else if(datamode2==CARTOON_COL_B)obj[this_id].move.gradBfrom=fetchDouble(c);
			}
			else if(datamode1==CARTOON_TO) {
				if(datamode2==CARTOON_COL_R)obj[this_id].move.gradRto=fetchDouble(c);
				else if(datamode2==CARTOON_COL_G)obj[this_id].move.gradGto=fetchDouble(c);
				else if(datamode2==CARTOON_COL_B)obj[this_id].move.gradBto=fetchDouble(c);
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME2;
			}
			else if(*c=='}') {
				mode=JSON_ENDDATA;
			}
		}
		else if(mode==JSON_ENDDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				return;
			}
		}
		cartoonPointer++;
	}
}

void readWaveAnime(char *json, int timer){
	int mode=JSON_DATA_START;
	int datamode=0;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			if(*c=='}') {
				return;
			}
			fetchString(c,str);
			if(strcmp(str, "sin")==0) {
				datamode=CARTOON_SIN;
			}
			else if(strcmp(str, "range")==0) {
				datamode=CARTOON_RANGE;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(datamode==CARTOON_SIN) {
				obj[this_id].waveSIN=fetchDouble(c);
			}
			if(datamode==CARTOON_RANGE) {
				obj[this_id].waveRange=fetchDouble(c);
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				return;
			}
		}
		cartoonPointer++;
	}
}

void readFlipAnime(char *json, int timer){
	int mode=JSON_DATA_START;
	int datamode=0;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			if(*c=='}') {
				return;
			}
			fetchString(c,str);
			if(strcmp(str, "in")==0) {
				datamode=CARTOON_INTERVAL;
			}
			else if(strcmp(str, "ix")==0) {
				datamode=CARTOON_IX;
			}
			else if(strcmp(str, "iy")==0) {
				datamode=CARTOON_IY;
			}
			else if(strcmp(str, "n")==0) {
				datamode=CARTOON_NUM;
			}
			else if(strcmp(str, "turnBack")==0) {
				datamode=CARTOON_TURNBACK;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(datamode==CARTOON_INTERVAL) {
				obj[this_id].flip.interval=fetchInt(c);
			}
			else if(datamode==CARTOON_IX) {
				obj[this_id].flip.ix=fetchInt(c);
			}
			else if(datamode==CARTOON_IY) {
				obj[this_id].flip.iy=fetchInt(c);
			}
			else if(datamode==CARTOON_NUM) {
				obj[this_id].flip.num=fetchInt(c);
			}
			else if(datamode==CARTOON_TURNBACK) {
				if(startsWith(c,"true")){
					obj[this_id].flip.turnBack=true;
				}
				else if(startsWith(c,"false")){
					obj[this_id].flip.turnBack=false;
				}
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				return;
			}
		}
		cartoonPointer++;
	}
}

void readSlideAnime(char *json, int timer){
	int mode=JSON_DATA_START;
	int datamode1=0,datamode2=0;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			fetchString(c,str);
			if(strcmp(str, "a")==0) {
				datamode1=CARTOON_ALPHA;
			}
			else if(strcmp(str, "x")==0) {
				datamode1=CARTOON_X;
			}
			else if(strcmp(str, "y")==0) {
				datamode1=CARTOON_Y;
			}
			else if(strcmp(str, "ix")==0) {
				datamode1=CARTOON_IX;
			}
			else if(strcmp(str, "iy")==0) {
				datamode1=CARTOON_IY;
			}
			else if(strcmp(str, "mag")==0) {
				datamode1=CARTOON_MAG;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_DATA_START2;
			}
		}
		else if(mode==JSON_DATA_START2) {
			if(*c=='{') {
				mode=JSON_GETNAME2;
			}
		}
		else if(mode==JSON_GETNAME2) {
			fetchString(c,str);
			if(strcmp(str, "step")==0) {
				datamode2=CARTOON_STEP;
			}
			else if(strcmp(str, "from")==0) {
				datamode2=CARTOON_FROM;
			}
			else if(strcmp(str, "to")==0) {
				datamode2=CARTOON_TO;
			}
			else if(strcmp(str, "pos")==0) {
				datamode2=CARTOON_POS;
			}
			else if(strcmp(str, "turnBack")==0) {
				datamode2=CARTOON_TURNBACK;
			}
			else if(strcmp(str, "fade")==0) {
				datamode2=CARTOON_FADE;
			}
			mode=JSON_COLON2;
		}
		else if(mode==JSON_COLON2) {
			if(*c==':') {
				mode=JSON_GETVALUE2;
			}
		}
		else if(mode==JSON_GETVALUE2) {
			if(datamode2==CARTOON_STEP) {
				if(datamode1==CARTOON_X){
					obj[this_id].slideX.step=fetchDouble(c);
					if(obj[this_id].slideX.step<0){
						obj[this_id].slideX.step*=-1;
						obj[this_id].slideX.back=true;
					}
				}
				else if(datamode1==CARTOON_Y){
					obj[this_id].slideY.step=fetchDouble(c);
					if(obj[this_id].slideY.step<0){
						obj[this_id].slideY.step*=-1;
						obj[this_id].slideY.back=true;
					}
				}
				else if(datamode1==CARTOON_IX){
					obj[this_id].slideIX.step=fetchDouble(c);
					if(obj[this_id].slideIX.step<0){
						obj[this_id].slideIX.step*=-1;
						obj[this_id].slideIX.back=true;
					}
				}
				else if(datamode1==CARTOON_IY){
					obj[this_id].slideIY.step=fetchDouble(c);
					if(obj[this_id].slideIY.step<0){
						obj[this_id].slideIY.step*=-1;
						obj[this_id].slideIY.back=true;
					}
				}
				else if(datamode1==CARTOON_MAG){
					obj[this_id].slideMag.step=fetchDouble(c);
					if(obj[this_id].slideMag.step<0){
						obj[this_id].slideMag.step*=-1;
						obj[this_id].slideMag.back=true;
					}
				}
				else if(datamode1==CARTOON_ALPHA){
					obj[this_id].slideAlpha.step=fetchDouble(c);
					if(obj[this_id].slideAlpha.step<0){
						obj[this_id].slideAlpha.step*=-1;
						obj[this_id].slideAlpha.back=true;
					}
				}
			}
			else if(datamode2==CARTOON_FROM) {
				if(datamode1==CARTOON_X)obj[this_id].slideX.from=fetchDouble(c);
				else if(datamode1==CARTOON_Y)obj[this_id].slideY.from=fetchDouble(c);
				else if(datamode1==CARTOON_IX)obj[this_id].slideIX.from=fetchDouble(c);
				else if(datamode1==CARTOON_IY)obj[this_id].slideIY.from=fetchDouble(c);
				else if(datamode1==CARTOON_MAG)obj[this_id].slideMag.from=fetchDouble(c);
				else if(datamode1==CARTOON_ALPHA)obj[this_id].slideAlpha.from=fetchDouble(c);
			}
			else if(datamode2==CARTOON_TO) {
				if(datamode1==CARTOON_X)obj[this_id].slideX.to=fetchDouble(c);
				else if(datamode1==CARTOON_Y)obj[this_id].slideY.to=fetchDouble(c);
				else if(datamode1==CARTOON_IX)obj[this_id].slideIX.to=fetchDouble(c);
				else if(datamode1==CARTOON_IY)obj[this_id].slideIY.to=fetchDouble(c);
				else if(datamode1==CARTOON_MAG)obj[this_id].slideMag.to=fetchDouble(c);
				else if(datamode1==CARTOON_ALPHA)obj[this_id].slideAlpha.to=fetchDouble(c);
			}
			else if(datamode2==CARTOON_POS) {
				if(datamode1==CARTOON_X)obj[this_id].slideX.position=fetchDouble(c);
				else if(datamode1==CARTOON_Y)obj[this_id].slideY.position=fetchDouble(c);
				else if(datamode1==CARTOON_IX)obj[this_id].slideIX.position=fetchDouble(c);
				else if(datamode1==CARTOON_IY)obj[this_id].slideIY.position=fetchDouble(c);
				else if(datamode1==CARTOON_MAG)obj[this_id].slideMag.position=fetchDouble(c);
				else if(datamode1==CARTOON_ALPHA)obj[this_id].slideAlpha.position=fetchDouble(c);
			}
			else if(datamode2==CARTOON_TURNBACK) {
				bool b=false;
				if(startsWith(c,"true"))b=true;
				else if(startsWith(c,"false"))b=false;
				if(datamode1==CARTOON_X)obj[this_id].slideX.turnBack=b;
				else if(datamode1==CARTOON_Y)obj[this_id].slideY.turnBack=b;
				else if(datamode1==CARTOON_IX)obj[this_id].slideIX.turnBack=b;
				else if(datamode1==CARTOON_IY)obj[this_id].slideIY.turnBack=b;
				else if(datamode1==CARTOON_MAG)obj[this_id].slideMag.turnBack=b;
				else if(datamode1==CARTOON_ALPHA)obj[this_id].slideAlpha.turnBack=b;
			}
			else if(datamode2==CARTOON_FADE) {
				if(datamode1==CARTOON_ALPHA){
					obj[this_id].slideAlpha.fadeRate=fetchDouble(c)/(obj[this_id].set.alpha+obj[this_id].slideAlpha.to);
					obj[this_id].slideAlpha.fade=1;
				}
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME2;
			}
			else if(*c=='}') {
				mode=JSON_ENDDATA;
			}
		}
		else if(mode==JSON_ENDDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				return;
			}
		}
		cartoonPointer++;
	}
}

void readJumpAnime(char *json, int timer){
	int mode=JSON_DATA_START;
	int datamode=0;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			if(*c=='}') {
				return;
			}
			fetchString(c,str);
			if(strcmp(str, "g")==0) {
				datamode=CARTOON_G;
			}
			else if(strcmp(str, "y")==0) {
				datamode=CARTOON_Y;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(datamode==CARTOON_G) {
				obj[this_id].jumpG=fetchDouble(c);
			}
			else if(datamode==CARTOON_Y) {
				obj[this_id].jumpY=fetchDouble(c);
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				return;
			}
		}
		cartoonPointer++;
	}
}

void readStoryFile(char *json, int timer){
	int mode=JSON_ARRAY_START;
	int datamode=0,lang=0;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_ARRAY_START) {
			if(*c=='[') {
				mode=JSON_DATA_START;
			}
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			fetchString(c,str);
			if(strcmp(str, "lang")==0) {
				datamode=CARTOON_LANG;
			}
			else if(strcmp(str, "file")==0) {
				datamode=CARTOON_FILE;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(datamode==CARTOON_LANG) {
				fetchString(c,str);
				if(strcmp(str, "jp")==0) {
					lang=0;
				}
				else if(strcmp(str, "en")==0) {
					lang=1;
				}
			}
			else if(datamode==CARTOON_FILE) {
				fetchString(c,str);
				loadFile(str);
				size_t fc=0;
				for(int i=0 ; i<1000 ; i++){
					if(fc>=fsize)break;
					for(int j=0 ; j<200 ; j++){
						talk[i].str[lang][j]=fstr[fc];fc++;
						if(talk[i].str[lang][j]==0)break;
					}
				}
				for(int i=0 ; i<1000 ; i++){
					padSpace(i,lang,30);
				}
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				mode=JSON_ENDDATA;
			}
		}
		else if(mode==JSON_ENDDATA) {
			if(*c==',') {
				mode=JSON_DATA_START;
			}
			else if(*c==']') {
				return;
			}
		}
		cartoonPointer++;
	}
}

void readCartoonTalk(char *json, int timer){
	int mode=JSON_ARRAY_START;
	int datamode=0;

	if(talkmode) {
		mode=JSON_ENDDATA;
	}

	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_ARRAY_START) {
			if(*c=='[') {
				mode=JSON_DATA_START;
			}
		}
		else if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			fetchString(c,str);
			if(strcmp(str, "face")==0) {
				datamode=CARTOON_FACE;
			}
			else if(strcmp(str, "text")==0) {
				datamode=CARTOON_TEXT_ID;
			}
			else if(strcmp(str, "shake")==0) {
				datamode=CARTOON_SHAKE;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(datamode==CARTOON_FACE) {
				gd.face_count=fetchInt(c);
			}
			else if(datamode==CARTOON_TEXT_ID) {
				gd.talk_count=fetchInt(c);
			}
			else if(datamode==CARTOON_SHAKE) {
				if(startsWith(c,"true")){
					gd.shake_count=50;
					Mix_PlayChannel(0,sf.decide,0);
				}
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				talkmode=true;
				gd.text_count=0;
				break;
			}
		}
		else if(mode==JSON_ENDDATA) {
			if(*c==',') {
				mode=JSON_DATA_START;
			}
			else if(*c==']') {
				talkmode=false;
				playtime=0;
				break;
			}
		}
		cartoonPointer++;
	}
}

void readCartoonImage(char *json, int timer){
	int mode=JSON_ARRAY_START;
	int datamode=0, img_id=0;

	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_ARRAY_START) {
			if(*c=='[') {
				mode=JSON_DATA_START;
			}
		}
		else if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			fetchString(c,str);
			if(strcmp(str, "id")==0) {
				datamode=CARTOON_ID;
			}
			else if(strcmp(str, "file")==0) {
				datamode=CARTOON_FILE;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(datamode==CARTOON_ID) {
				img_id=fetchInt(c);
			}
			else if(datamode==CARTOON_FILE) {
				fetchString(c,str);
				freeImage(img.bg[img_id]);
				getImage(img.bg[img_id],str,0,0,255);
				gd.talk_count=fetchInt(c);
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				mode=JSON_ENDDATA;
			}
		}
		else if(mode==JSON_ENDDATA) {
			if(*c==',') {
				mode=JSON_DATA_START;
			}
			else if(*c==']') {
				break;
			}
		}
		cartoonPointer++;
	}
}

void readCartoonLoadSound(char *json, int timer){
	int mode=JSON_ARRAY_START;
	int datamode=0, sound_id=0;

	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_ARRAY_START) {
			if(*c=='[') {
				mode=JSON_DATA_START;
			}
		}
		else if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			fetchString(c,str);
			if(strcmp(str, "id")==0) {
				datamode=CARTOON_ID;
			}
			else if(strcmp(str, "file")==0) {
				datamode=CARTOON_FILE;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(datamode==CARTOON_ID) {
				sound_id=fetchInt(c);
			}
			else if(datamode==CARTOON_FILE) {
				fetchString(c,str);
				freeSound(sf.sound[sound_id]);
				sf.sound[sound_id]=Mix_LoadWAV(str);
				gd.talk_count=fetchInt(c);
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				mode=JSON_ENDDATA;
			}
		}
		else if(mode==JSON_ENDDATA) {
			if(*c==',') {
				mode=JSON_DATA_START;
			}
			else if(*c==']') {
				break;
			}
		}
		cartoonPointer++;
	}
}

void readCartoonBgm(char *json, int timer){
	int mode=JSON_DATA_START;
	int datamode=0,repeat=-1;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			if(*c=='}') {
				break;
			}
			fetchString(c,str);
			if(strcmp(str, "repeat")==0) {
				datamode=CARTOON_REPEAT;
			}
			else if(strcmp(str, "file")==0) {
				datamode=CARTOON_FILE;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(datamode==CARTOON_REPEAT) {
				repeat=fetchInt(c);
			}
			else if(datamode==CARTOON_FILE) {
				fetchString(c, str);
				freeMusic();
				bgm=Mix_LoadMUS(str);
				Mix_PlayMusic(bgm, repeat);
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				break;
			}
		}
		cartoonPointer++;
	}
}

void readCartoonSound(char *json, int timer){
	int mode=JSON_DATA_START;
	int datamode=0,sound_id=0,repeat=0;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			if(*c=='}') {
				break;
			}
			fetchString(c,str);
			if(strcmp(str, "id")==0) {
				datamode=CARTOON_ID;
			}
			else if(strcmp(str, "repeat")==0) {
				datamode=CARTOON_REPEAT;
			}
			else if(strcmp(str, "ch")==0) {
				datamode=CARTOON_CHANNEL;
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(datamode==CARTOON_ID) {
				sound_id=fetchInt(c);
			}
			else if(datamode==CARTOON_REPEAT) {
				repeat=fetchInt(c);
			}
			else if(datamode==CARTOON_CHANNEL) {
				Mix_PlayChannel(fetchInt(c), sf.sound[sound_id], repeat);
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				break;
			}
		}
		cartoonPointer++;
	}
}

void resetObject(char *json, int timer){
	int mode=JSON_ARRAY_START;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_ARRAY_START) {
			if(*c=='[') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(*c==']') {
				return;
			}else{
				resetObject(fetchInt(c));
				mode=JSON_NEXTDATA;
			}
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETVALUE;
			}
			else if(*c==']') {
				return;
			}
		}
		cartoonPointer++;
	}
}

void _moveObject(){
	for(int i=0 ; i<=max_obj ; i++){
		obj[i].set.x+=obj[i].move.x;
		obj[i].set.y+=obj[i].move.y;
		obj[i].set.ix+=obj[i].move.ix;
		obj[i].set.iy+=obj[i].move.iy;
		obj[i].set.w+=obj[i].move.w;
		obj[i].set.h+=obj[i].move.h;
		obj[i].set.mag+=obj[i].move.mag;
		obj[i].set.alpha+=obj[i].move.alpha;
		obj[i].set.shake+=obj[i].move.shake;
		obj[i].set.R+=obj[i].move.R;
		obj[i].set.G+=obj[i].move.G;
		obj[i].set.B+=obj[i].move.B;
		obj[i].set.gradRfrom+=obj[i].move.gradRfrom;
		obj[i].set.gradGfrom+=obj[i].move.gradGfrom;
		obj[i].set.gradBfrom+=obj[i].move.gradBfrom;
		obj[i].set.gradRto+=obj[i].move.gradRto;
		obj[i].set.gradGto+=obj[i].move.gradGto;
		obj[i].set.gradBto+=obj[i].move.gradBto;

		obj[i].flip.count++;
		if(obj[i].flip.num!=0){
			if(obj[i].flip.turnBack){
				if(obj[i].flip.count%obj[i].flip.interval==0){
					if(obj[i].flip.count<obj[i].flip.interval*obj[i].flip.num){
						obj[i].set.ix+=obj[i].flip.ix;
						obj[i].set.iy+=obj[i].flip.iy;
					}else{
						obj[i].set.ix-=obj[i].flip.ix;
						obj[i].set.iy-=obj[i].flip.iy;
					}
					if(obj[i].flip.count==obj[i].flip.interval*(obj[i].flip.num-1)*2){
						obj[i].flip.count=0;
					}
				}
			}else{
				if(obj[i].flip.count%obj[i].flip.interval==0){
					obj[i].set.ix+=obj[i].flip.ix;
					obj[i].set.iy+=obj[i].flip.iy;
				}
				if(obj[i].flip.count==obj[i].flip.interval*obj[i].flip.num){
					obj[i].set.ix-=obj[i].flip.ix*obj[i].flip.num;
					obj[i].set.iy-=obj[i].flip.iy*obj[i].flip.num;
					obj[i].flip.count=0;
				}
			}
		}

		if(obj[i].slideAlpha.step!=0){
			if(obj[i].slideAlpha.back){
				obj[i].slideAlpha.position-=obj[i].slideAlpha.step;
			}else{
				obj[i].slideAlpha.position+=obj[i].slideAlpha.step;
			}
			if(obj[i].slideAlpha.position>=obj[i].slideAlpha.to){
				if(obj[i].slideAlpha.turnBack){
					obj[i].slideAlpha.position=obj[i].slideAlpha.to;
					obj[i].slideAlpha.back=true;
				}else{
					obj[i].slideAlpha.position=obj[i].slideAlpha.from;
				}
			}
			else if(obj[i].slideAlpha.position<=obj[i].slideAlpha.from){
				obj[i].slideAlpha.position=obj[i].slideAlpha.from;
				obj[i].slideAlpha.back=false;
			}
		}
		if(obj[i].slideX.step!=0){
			if(obj[i].slideX.back){
				obj[i].slideX.position-=obj[i].slideX.step;
			}else{
				obj[i].slideX.position+=obj[i].slideX.step;
			}
			if(obj[i].slideX.position>=obj[i].slideX.to){
				if(obj[i].slideX.turnBack){
					obj[i].slideX.position=obj[i].slideX.to;
					obj[i].slideX.back=true;
				}else{
					obj[i].slideX.position=obj[i].slideX.from;
				}
			}
			else if(obj[i].slideX.position<=obj[i].slideX.from){
				if(obj[i].slideX.turnBack){
					obj[i].slideX.position=obj[i].slideX.from;
					obj[i].slideX.back=false;
				}else{
					obj[i].slideX.position=obj[i].slideX.to;
				}
			}
		}
		if(obj[i].slideY.step!=0){
			if(obj[i].slideY.back){
				obj[i].slideY.position-=obj[i].slideY.step;
			}else{
				obj[i].slideY.position+=obj[i].slideY.step;
			}
			if(obj[i].slideY.position>=obj[i].slideY.to){
				if(obj[i].slideY.turnBack){
					obj[i].slideY.position=obj[i].slideY.to;
					obj[i].slideY.back=true;
				}else{
					obj[i].slideY.position=obj[i].slideY.from;
				}
				obj[i].slideY.back=true;
			}
			else if(obj[i].slideY.position<=obj[i].slideY.from){
				obj[i].slideY.position=obj[i].slideY.from;
				obj[i].slideY.back=false;
			}
		}
		if(obj[i].slideIX.step!=0){
			if(obj[i].slideIX.back){
				obj[i].slideIX.position-=obj[i].slideIX.step;
			}else{
				obj[i].slideIX.position+=obj[i].slideIX.step;
			}
			if(obj[i].slideIX.position>=obj[i].slideIX.to){
				if(obj[i].slideIX.turnBack){
					obj[i].slideIX.position=obj[i].slideIX.to;
					obj[i].slideIX.back=true;
				}else{
					obj[i].slideIX.position=obj[i].slideIX.from;
				}
				obj[i].slideIX.back=true;
			}
			else if(obj[i].slideIX.position<=obj[i].slideIX.from){
				obj[i].slideIX.position=obj[i].slideIX.from;
				obj[i].slideIX.back=false;
			}
		}
		if(obj[i].slideIY.step!=0){
			if(obj[i].slideIY.back){
				obj[i].slideIY.position-=obj[i].slideIY.step;
			}else{
				obj[i].slideIY.position+=obj[i].slideIY.step;
			}
			if(obj[i].slideIY.position>=obj[i].slideIY.to){
				if(obj[i].slideIY.turnBack){
					obj[i].slideIY.position=obj[i].slideIY.to;
					obj[i].slideIY.back=true;
				}else{
					obj[i].slideIY.position=obj[i].slideIY.from;
				}
				obj[i].slideIY.back=true;
			}
			else if(obj[i].slideIY.position<=obj[i].slideIY.from){
				obj[i].slideIY.position=obj[i].slideIY.from;
				obj[i].slideIY.back=false;
			}
		}
		if(obj[i].slideMag.step!=0){
			if(obj[i].slideMag.back){
				obj[i].slideMag.position-=obj[i].slideMag.step;
			}else{
				obj[i].slideMag.position+=obj[i].slideMag.step;
			}
			if(obj[i].slideMag.position>=obj[i].slideMag.to){
				if(obj[i].slideMag.turnBack){
					obj[i].slideMag.position=obj[i].slideMag.to;
					obj[i].slideMag.back=true;
				}else{
					obj[i].slideMag.position=obj[i].slideMag.from;
				}
			}
			else if(obj[i].slideMag.position<=obj[i].slideMag.from){
				obj[i].slideMag.position=obj[i].slideMag.from;
				obj[i].slideMag.back=false;
			}
		}
		if(obj[i].jumpG!=0){
			obj[i].set.y-=obj[i].jumpY;
			obj[i].jumpY-=obj[i].jumpG;
		}
		if(obj[i].slideAlpha.fadeRate!=0){
			obj[i].slideAlpha.fade+=obj[i].slideAlpha.fadeRate;
		}
	}
}

bool nextCut(){
	_moveObject();
	if(!talkmode){
		bool end=false;
		if(playtime==cartoonNextTime){
			end=readCartoon(cartoonJson,playtime);
		}
		playtime++;
		if(end)return true;
	}else{
		gd.text_count++;
		if(gd.shake_count>0)gd.shake_count--;
	}
	return false;
}

void nextTalk(){
	if(talkmode){
		if(gd.text_count<(int)strlen(talk[gd.talk_count].str[CHAR_CODE])){
			gd.text_count=90;
		}else{
			readCartoon(cartoonJson,playtime);
		}
	}
}

void drawAnimationCut(SDL_Surface* scr){
	for(int i=0 ; i<=max_obj ; i++){
		if(obj[i].set.lang!=-1 && obj[i].set.lang!=CHAR_CODE){
			continue;
		}

		int dx=0,dy=0;
		double fade=1;
		if(obj[i].set.shake!=0){
			dx=(int)(cos(count*0.3)*obj[i].set.shake);
			dy=(int)(sin(count*0.3)*obj[i].set.shake);
		}
		if(obj[i].slideAlpha.fadeRate!=0){
			fade=obj[i].slideAlpha.fade;
		}

		int x=(int)(obj[i].set.x+obj[i].slideX.position+dx);
		int y=(int)(obj[i].set.y+obj[i].slideY.position+dy);
		int ix=(int)(obj[i].set.ix+obj[i].slideIX.position);
		int iy=(int)(obj[i].set.iy+obj[i].slideIY.position);
		int w=(int)obj[i].set.w;
		int h=(int)obj[i].set.h;
		int a=(int)((obj[i].set.alpha+obj[i].slideAlpha.position)*fade);
		double mag=obj[i].set.mag+obj[i].slideMag.position;
		if(obj[i].set.type==CARTOON_CACHE){
			if(mag==1){
				drawImage(scr,img.cache,x,y,ix,iy,w,h,a);
			}else{
				drawImage_x(scr,img.cache,x,y,mag,ix,iy,w,h,a);
			}
		}else{
			if(obj[i].set.cacheTo!=-1){
				if(obj[i].set.type==CARTOON_DRAWIMAGE){
					if(mag==1){
						drawImage(img.cache,img.bg[obj[i].set.img_id],x,y,ix,iy,w,h,a);
					}else{
						drawImage_x(img.cache,img.bg[obj[i].set.img_id],x,y,mag,ix,iy,w,h,a);
					}
				}
				else if(obj[i].set.type==CARTOON_FILLRECT){
					fillRect(img.cache,x,y,w,h,(int)obj[i].set.R,(int)obj[i].set.G,(int)obj[i].set.B,a);
				}
				else if(obj[i].set.type==CARTOON_FILLRECT_GRAD){
					for(int j=0 ; j<h ; j++){
						int R=(int)(obj[i].set.gradRfrom+(obj[i].set.gradRto-obj[i].set.gradRfrom)*j/h);
						int G=(int)(obj[i].set.gradGfrom+(obj[i].set.gradGto-obj[i].set.gradGfrom)*j/h);
						int B=(int)(obj[i].set.gradBfrom+(obj[i].set.gradBto-obj[i].set.gradBfrom)*j/h);
						fillRect(img.cache,x,y+j,w,1,R,G,B,a);
					}
				}
				else if(obj[i].set.type==CARTOON_DRAWIMAGE_WAVE){
					for(int j=0 ; j<h ; j++){
						drawImage(img.cache,img.bg[obj[i].set.img_id],x+(int)(sin((count+j)*obj[i].waveSIN)*obj[i].waveRange),y+j,ix,iy+j,w,1,a);
					}
				}
			}else{
				if(obj[i].set.type==CARTOON_DRAWIMAGE){
					if(mag==1){
						drawImage(scr,img.bg[obj[i].set.img_id],x,y,ix,iy,w,h,a);
					}else{
						drawImage_x(scr,img.bg[obj[i].set.img_id],x,y,mag,ix,iy,w,h,a);
					}
				}
				else if(obj[i].set.type==CARTOON_FILLRECT){
					fillRect(scr,x,y,w,h,(int)obj[i].set.R,(int)obj[i].set.G,(int)obj[i].set.B,a);
				}
				else if(obj[i].set.type==CARTOON_FILLRECT_GRAD){
					for(int j=0 ; j<h ; j++){
						int R=(int)(obj[i].set.gradRfrom+(obj[i].set.gradRto-obj[i].set.gradRfrom)*j/h);
						int G=(int)(obj[i].set.gradGfrom+(obj[i].set.gradGto-obj[i].set.gradGfrom)*j/h);
						int B=(int)(obj[i].set.gradBfrom+(obj[i].set.gradBto-obj[i].set.gradBfrom)*j/h);
						fillRect(scr,x,y+j,w,1,R,G,B,a);
					}
				}
				else if(obj[i].set.type==CARTOON_DRAWIMAGE_WAVE){
					for(int j=0 ; j<h ; j++){
						drawImage(scr,img.bg[obj[i].set.img_id],x+(int)(sin((count+j)*obj[i].waveSIN)*obj[i].waveRange),y+j,ix,iy+j,w,1,a);
					}
				}
			}
		}
	}
	if(talkmode){
		drawTalking(scr,gd.face_count,talk[gd.talk_count]);
	}
}


// ending2.png
// このバージョンではまだ使わない
void drawLastEndingAnim(SDL_Surface* scr, int cn){
	if(cn>=1300 && cn<2800)fillRect(scr,0,20,320,120,255,192,192,255);
	if((cn>=2800 && cn<3350) || cn>=3750)fillRect(scr,0,20,320,200,255,255,255,255);
	if(cn>=3350 && cn<3750)fillRect(scr,0,20,320,200,0,0,0,255);
	if(cn>=1300 && cn<2800){
		fillRect(scr,0,140,320,80,128,128,255,255);
		if(cn%230>=30)drawImage(scr,img.back,60,160,1280,(cn/230)*60,200,60,255);
	}
	int a;
	if(cn<50){
		drawImage(scr,img.back,0,20,0,0,320,120,255);
		if(cn<40)drawImage(scr,img.back,150+cn,10+cn*cn/20,0,120,40,40,255);
		else drawImage(scr,img.back,150+cn,90,0,120,40,40,255);
		if(cn>=10)drawImage(scr,img.back,80+(cn-10),10+(cn-10)*(cn-10)/20,120,120,40,40,255);
	}
	else if(cn<1030){
		a=cn-50;
		for(int i=0 ; i<4 ; i++)drawImage(scr,img.back,320*i-a,20,0,0,320,120,255);
		drawImage(scr,img.back,200,90,abs(2-(a/10)%4)*40,120,40,40,255);
		drawImage(scr,img.back,120,90,120+abs(2-(a/10)%4)*40,120,40,40,255);
	}
	else if(cn<1100){
		a=cn-1030;
		drawImage(scr,img.back,0,20,0,600,320,120,255);
		drawImage(scr,img.back,-70+a*6,170-a*4,abs(2-(a/10)%4)*100,1800,100,100,255);
		drawImage(scr,img.back,-200+a*6,250-a*4,300+abs(2-(a/10)%4)*100,1800,100,100,255);
		drawImage(scr,img.back,220,20,460,480,120,120,255);
	}
	else if(cn<1170){
		a=cn-1100;
		drawImage(scr,img.back,0,20,320,600,270,120,255);
		if(a<20)drawImage(scr,img.back,150-(int)(a*4.5),20-(int)(a*0.75),810,360,120,120,255);
		else{
			drawImage(scr,img.back,60,5,810,360,120,120,255);
			drawImage(scr,img.back,270-(a-20)*6,-10+(a-20)*2,abs(2-(a/10)%4)*100,1900,100,100,255);
			drawImage(scr,img.back,370-(a-20)*6,-30+(a-20)*2,300+abs(2-(a/10)%4)*100,1900,100,100,255);
		}
		drawImage(scr,img.back,270,20,590,600,50,120,255);
	}
	else if(cn<1230){
		a=cn-1170;
		drawImage(scr,img.back,0,20,640,600,320,120,255);
		drawImage(scr,img.back,320-a*7,40-(45-(a-30)*(a-30)/20),abs(2-(a/10)%4)*100,1900,100,100,255);
		drawImage(scr,img.back,420-a*7,40-(45-(a-30)*(a-30)/20),300+abs(2-(a/10)%4)*100,1900,100,100,255);
	}
	else if(cn<1300){
		a=cn-1230;
		drawImage(scr,img.back,0,20,960,600,320,120,255);
		drawImage(scr,img.back,320-a*7,40,abs(2-(a/10)%4)*100,1900,100,100,255);
		drawImage(scr,img.back,420-a*7,40,300+abs(2-(a/10)%4)*100,1900,100,100,255);
	}
	else if(cn<1800){
		a=cn-1300;
		if(a<60){
			drawImage(scr,img.back,0,20,320,0,320,120,255);
			drawImage(scr,img.back,0,140,0,160,320,15,255);
		}
		else if(a<180){
			drawImage(scr,img.back,0,20,320,120,320,120,255);
			if(a<80)fillRect(scr,0,20,90,120,255,192,192,255);
			if(a<120)fillRect(scr,200,20,30,60,255,192,192,255);
			drawImage(scr,img.back,0,140,0,175,320,30,255);
		}
		else if(a<300){
			drawImage(scr,img.back,0,20,320,240,320,120,255);
			if(a<195)fillRect(scr,0,20,220,30,255,192,192,255);
			if(a<210)fillRect(scr,0,50,220,30,255,192,192,255);
			if(a<225)fillRect(scr,0,80,220,30,255,192,192,255);
			if(a<240)fillRect(scr,240,20,80,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,205,320,30,255);
		}
		else if(a<420){
			drawImage(scr,img.back,0,20,320,360,320,120,255);
			if(a<320)fillRect(scr,240,20,80,120,255,192,192,255);
			if(a<360)fillRect(scr,0,20,110,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,235,320,30,255);
		}else{
			drawImage(scr,img.back,0,20,320,120,320,120,255);
			fillRect(scr,0,20,90,120,255,192,192,255);
			fillRect(scr,200,20,30,60,255,192,192,255);
			if(a>=440)drawImage(scr,img.back,0,20,320,480,110,120,255);
			if(a>=460)drawImage(scr,img.back,200,20,430,480,30,60,255);
			drawImage(scr,img.back,0,140,0,265,320,30,255);
		}
	}
	else if(cn<2300){
		a=cn-1800;
		if(a<60){
			drawImage(scr,img.back,0,20,640,0,320,120,255);
			drawImage(scr,img.back,0,140,0,295,320,15,255);
		}
		else if(a<160){
			drawImage(scr,img.back,0,20,640,120,320,120,255);
			if(a<80)fillRect(scr,220,20,60,80,255,192,192,255);
			if(a<120)fillRect(scr,0,20,110,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,310,320,30,255);
		}
		else if(a<280){
			if(a<210)drawImage(scr,img.back,80,20,640,240,160,120,255);
			else drawImage(scr,img.back,80,20,800,240,160,120,255);
		}
		else if(a<380){
			drawImage(scr,img.back,0,20,640,120,320,120,255);
			fillRect(scr,220,20,60,80,255,192,192,255);
			fillRect(scr,0,20,110,120,255,192,192,255);
			if(a>=300)drawImage(scr,img.back,220,20,720,360,50,70,255);
			if(a>=340)drawImage(scr,img.back,0,20,640,360,80,120,255);
			drawImage(scr,img.back,0,140,0,340,320,15,255);
		}else{
			drawImage(scr,img.back,0,20,640,480,320,80,255);
			drawImage(scr,img.back,110+abs(5-a%10),50+abs(5-a%10),770,360,40,50,255);
			drawImage(scr,img.back,0,100,640,560,320,40,255);
			if(a<400)fillRect(scr,230,20,90,120,255,192,192,255);
			if(a<440)fillRect(scr,0,20,90,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,355,320,30,255);
		}
	}
	else if(cn<2800){
		a=cn-2300;
		if(a<60){
			drawImage(scr,img.back,0,20,960,0,320,120,255);
			drawImage(scr,img.back,0,140,0,385,320,30,255);
		}
		else if(a<160){
			drawImage(scr,img.back,0,20,960,120,320,120,255);
			if(a<80)fillRect(scr,270,20,50,120,255,192,192,255);
			if(a<120)fillRect(scr,0,20,70,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,415,320,30,255);
		}
		else if(a<260){
			drawImage(scr,img.back,0,20,960,240,320,120,255);
			if(a<180)fillRect(scr,0,20,100,120,255,192,192,255);
			if(a<220)fillRect(scr,240,20,80,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,445,320,30,255);
		}
		else if(a<380){
			drawImage(scr,img.back,0,60,960,400,270,80,255);
			if(a>=280)drawImage(scr,img.back,270,20,1230,360,50,120,255);
			if(a>=310){
				drawImage(scr,img.back,110,100,960,360+((a/2)%2)*20,60,20,255);
				drawImage(scr,img.back,170,100,1020,360+((a/2)%2)*20,60,20,255);
			}
			drawImage(scr,img.back,0,140,0,475,320,15,255);
		}else{
			drawImage(scr,img.back,0,20,960,480,320,120,255);
			if(a<400)fillRect(scr,60,20,30,120,255,192,192,255);
			if(a<420)fillRect(scr,30,20,30,120,255,192,192,255);
			if(a<440)fillRect(scr,0,20,30,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,490,320,30,255);
		}
	}
	else if(cn<3000){
		a=cn-2800;
		int b=0;
		if(a>=60)b=a-60;
		drawImage_x(scr,img.back,130+a+b*4,120-(int)(b*0.8),(100.0+b*2)/100.0,0,1560,100,100,255);
		if(a<40)drawImage_x(scr,img.back,240-(int)(a*0.5),200-a-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1600,40,40,255);
		else if(a<135)drawImage_x(scr,img.back,240-(int)(a*0.5),160-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1600,40,40,255);
		else drawImage_x(scr,img.back,240-(int)(a*0.5),160-(int)(b*0.5)-(a-135),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1600,40,40,255);
		if(a<20)drawImage_x(scr,img.back,280-(int)(a*0.5),200-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1560,40,40,255);
		else if(a<60)drawImage_x(scr,img.back,280-(int)(a*0.5),200-(a-20)-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1560,40,40,255);
		else if(a<135)drawImage_x(scr,img.back,280-(int)(a*0.5),160-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1560,40,40,255);
		else drawImage_x(scr,img.back,280-(int)(a*0.5),160-(int)(b*0.5)-(a-135),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1560,40,40,255);
		drawImage_x(scr,img.back,120+a/3+(int)(b*3.5),140-(int)(b*1.5),(20.0+b)/80.0,abs(2-(a/10)%4)*80,720,80,160,255);
		drawImage_x(scr,img.back,-100+a,140-b,(40.0+(b*1.5))/100.0,720,720,100,100,255);
		fillRect(scr,0,180+(int)(b*0.5),130+a+b*4,40,0,0,0,255);
	}
	else if(cn<3050){
		a=cn-3000;
		int b=a*2,c;
		if(a<10)c=1;
		else if(a<20)c=2;
		else if(a<30)c=0;
		else if(a<40)c=3;
		else c=0;
		drawImage_x(scr,img.back,100-b/2,b/2,(250.0-b)/100.0,720+c*100,720,100,100,255);
		drawImage_x(scr,img.back,320-(int)(a*2.5)-(int)(b*0.5),60+(int)(b*0.5),(120.0-(b*0.5))/120.0,360,720,120,160,255);
		fillRect(scr,0,250-(int)(b*0.5),320,40,0,0,0,255);
	}
	else if(cn<3100){
		a=cn-3050;
		int b=a;
		drawImage_x(scr,img.back,50+b/2,50-(int)(b*0.8),(150.0+(b*1.6))/100.0,720,720,100,100,255);
		drawImage_x(scr,img.back,145+b*3,110-b*2,(70.0+b*3)/120.0,360,720,120,160,255);
		fillRect(scr,0,170+b*2,320,50,0,0,0,255);
	}
	else if(cn<3250){
		a=cn-3100;
		double b;
		int c,d;
		if(a<50){b=1-a/166.6;c=50-a;d=a;}
		else if(a<100){b=0.70+(a-50)/166.6;c=a-50;d=50-(a-50);}
		else{b=1+(a-100)/100.0;c=50+(a-100)*2;d=(a-100)*7;}
		drawImage_x(scr,img.back,c,40-d+(int)(50*b),b,0,880,100,100,255);
		drawImage_x(scr,img.back,c+(int)(50*b),40-d+(int)(95*b),b,0,980,100,150,255);
		drawImage_x(scr,img.back,c+(int)(110*b),40-d+(int)(75*b),b,500,880,170,180,255);
		drawImage_x(scr,img.back,c+(int)(80*b),40-d,b,1010,880,90,90,255);
	}
	else if(cn<3350){
		a=cn-3250;
		int b=0,c=0;
		if(a<85)b=a/3;
		else c=(a-85)*4;
		fillRect(scr,0,320-a*4,320,300,0,0,0,255);
		if(a<50)drawImage_x(scr,img.back,250-a,-40+(int)(a*2.5),(60.0-b)/60.0,((a/5)%6)*60,1680,60,60,255);
		else if(a<85)drawImage_x(scr,img.back,250-a,-40+(int)(a*2.5),(60.0-b)/60.0,((a/5)%6)*60,1740,60,60,255);
		else drawImage_x(scr,img.back,165+(int)((a-85)*2.4),172-(int)((a-85)*0.7),(32.0-c/8)/60.0,0,1740,60,60,255);
		drawImage_x(scr,img.back,-570+a*6,-60+a,(250.0-c)/250.0,0,1130,250,180,255);
	}
	else if(cn<3400){
		a=cn-3350;
		drawImage_x(scr,img.back,30,40,(190.0+a*1.2)/250.0,0,1130,250,180,255);
	}
	else if(cn<3550){
		a=cn-3400;
		double b=1+a/75.0;
		drawImage_x(scr,img.back,(int)(95*b)-a*4,20-a,b,0,1310,100,100,255);
		drawImage_x(scr,img.back,(int)(105*b)-a*4,20+(int)(65*b)-a,b,570,1440,180,120,255);
		drawImage_x(scr,img.back,-a*4,20+(int)(30*b)-a,b,300,1310,130,120,255);
		drawImage_x(scr,img.back,(int)(70*b)-a*4,20+(int)(110*b)-a,b,0,1430,190,130,255);
	}
	else if(cn<3750){
		a=cn-3550;
		drawImage_x(scr,img.back,80,110-(int)(a*0.25),(100.0+a/5)/120.0,600,1680,120,60,255);
		drawImage_x(scr,img.back,80,60-(int)(a*0.25),(100.0+a/5)/120.0,360,1680,120,60,255);
		drawImage_x(scr,img.back,80,110+a/10,(100.0+a/5)/120.0,360,1740,120,60,255);
	}
	else if(cn<3900){
		a=cn-3750;
		drawImage_x(scr,img.back,80,70,140/120.0,600,1740,120,60,255);
		drawImage_x(scr,img.back,80,10,140/120.0,480,1680,120,60,255);
		drawImage_x(scr,img.back,80,130,140/120.0,480,1740,120,60,255);
	}
	else if(cn<3950){
		a=cn-3900;
		drawImage_x(scr,img.back,80+(int)(a*1.4),70+(int)(a*0.7),(140.0-a*2.8)/120.0,600,1740,120,60,255);
		drawImage_x(scr,img.back,80+(int)(a*1.4),10+(int)(a*2.0),(140.0-a*2.8)/120.0,480,1680,120,60,255);
		drawImage_x(scr,img.back,80+(int)(a*1.4),130-(int)(a*0.5),(140.0-a*2.8)/120.0,480,1740,120,60,255);
	}else{
		a=cn-3950;
		drawImage(scr,img.back,0,20,960,1680,320,200,255);
		if(a>=100 && a<650){
			drawImage(scr,img.back,110,30,720,1800,110,40,255);
			drawImage(scr,img.back,50,90,720,1840,220,90,255);
		}
	}
	fillRect(scr,0,0,320,20,0,0,0,255);
	fillRect(scr,0,220,320,20,0,0,0,255);
	if(cn<1300){
		fillRect(scr,0,140,320,80,128,128,255,255);
		if(cn%230>=30)drawImage(scr,img.back,60,160,1280,(cn/230)*60,200,60,255);
	}
}

// epilogue.png
void drawSummerWars(SDL_Surface* scr){
	if(count<200){
		fillRect(scr,0,0,320,240,0,0,0,255);
		drawImage(scr,img.back,0,20,0,0,320,200,255);
		drawImage(scr,img.back,60+count/2,60+count/4,320,40+(count%2)*40,60,40,255);
	}
	else if(count<300){
		if(count==200){
			map.buffered=false;
		}
		else{
			walking();
		}
		SDL_Color col=getSkyColor(gd.hour,gd.minute);
		fillRect(scr,0,0,320,240,col.r,col.g,col.b,255);
		drawMap(scr,gd.scrX,gd.scrY);
		if(count==299){
			drawImage(img.cache,scr,0,0,0,0,320,240,255);
		}
	}
	else if(count<400){
		drawImage_x(scr,img.cache,0,0,(320.0+(count-300)*4)/320.0,count-300,(int)((count-300)/1.2),320,240,255);
	}
	else if(count<500){
		int a=count-400;
		int dx=0,dy=0;
		if(a>=20){
			dx=(int)(cos(a*0.7)*5);
			dy=(int)(sin(a*0.7)*5);
		}
		drawImage(scr,img.back,-5+dx,15+dy,0,350,330,160,255);
		if(a<20){
			drawImage(scr,img.back,160-a*3,-100+a*12,320,120,40,120,255);
			drawImage(scr,img.back,140-a*3,a*12,360,120,40,40,128);
			drawImage(scr,img.back,50-a*a,80-a*a,360,180,60,60,a*13);
			drawImage(scr,img.back,80-a*a/2,70-a*a,360,180,60,60,a*13);
			drawImage(scr,img.back,120,60-a*a,360,180,60,60,a*13);
			drawImage(scr,img.back,150-a*a/2,80-a*a,360,180,60,60,a*13);
			drawImage(scr,img.back,170+a*a,100-a*a/2,360,180,60,60,a*13);
		}else{
			drawImage(scr,img.back,80+dx-a,140+dy-a/2,400,0,120,120,255);
			drawImage(scr,img.back,80+dx-a/2,160+dy-a,520,0,120,120,255);
			drawImage(scr,img.back,80+dx,220+dy-a*2,640,0,120,120,255);
			drawImage(scr,img.back,80+dx+a/2,170+dy-a,760,0,120,120,255);
			drawImage(scr,img.back,80+dx+a,140+dy-a/2,880,0,120,120,255);
			drawImage(scr,img.back,80+dx,300+dy-a*2,400,120,120,60,255);
			drawImage(scr,img.back,60+dx-a/2,190+dy-a,420,180,60,60,255);
			drawImage(scr,img.back,120+dx-a,170+dy-a,420,180,60,60,255);
			drawImage(scr,img.back,90+dx,240+dy-a*2,420,180,60,60,255);
			drawImage(scr,img.back,60+dx+a,160+dy-a,420,180,60,60,255);
			drawImage(scr,img.back,130+dx+a/2,180+dy-a,420,180,60,60,255);
			drawImage(scr,img.back,160+dx-a,220+dy-a,420,180,60,60,255);
			drawImage(scr,img.back,20+dx+a,200+dy-a,420,180,60,60,255);
			drawImage(scr,img.back,60+dx-a/2,200+dy-a*2,540,120,80,160,a*2);
		}
		drawImage(scr,img.back,-5+dx,75+dy,0,200,330,150,255);
		fillRect(scr,0,0,320,20,0,0,0,255);
		fillRect(scr,0,220,320,20,0,0,0,255);
	}
	else if(count<600){
		int a=count-500;
		int dx=0,dy=0;
		dx=(int)(cos(a*0.7)*5);
		dy=(int)(sin(a*0.7)*5);
		drawImage(scr,img.back,-5+dx,15+dy,0,510,330,210,255);
		if(a<30)drawImage(scr,img.back,50+dx+a/3,30+dy-a/3,620,120,240,100,255);
		else drawImage(scr,img.back,60+dx+(a-30)/2,20+dy-(a-30)/2,620,220,240,100,255);
		//砂嵐
		drawImage(scr,img.back,-250+a*3+dx,240-a*6+dy,580,320,200,400,255);
		drawImage(scr,img.back,-130+a*3+dx,280-a*6+dy,580,320,200,400,255);
		drawImage(scr,img.back,-10+a*3+dx,310-a*6+dy,580,320,200,400,255);
		//じゃり
		drawImage(scr,img.back,20-a/4+dx,140-a+dy,480,180,60,60,255);
		drawImage(scr,img.back,90+dx,160-a*2+dy,480,180,60,60,255);
		drawImage(scr,img.back,130+a/4+dx,140-a*2+dy,480,180,60,60,255);
		drawImage(scr,img.back,160+a/2+dx,110-a+dy,480,180,60,60,255);
		drawImage(scr,img.back,200+a+dx,110-a/2+dy,480,180,60,60,255);
		//岩
		drawImage(scr,img.back,dx,190-a*2+dy,420,240,80,80,255);
		drawImage(scr,img.back,100+a/4+dx,260-a*4+dy,420,240,80,80,255);
		drawImage(scr,img.back,140+a/2+dx,230-a*3+dy,420,240,80,80,255);
		drawImage(scr,img.back,230+a+dx,210-a*2+dy,420,240,80,80,255);
		//大岩
		drawImage(scr,img.back,-120+a*2+dx,360-a*4+dy,330,520,180,100,255);
		drawImage(scr,img.back,40+a*2+dx,380-a*4+dy,330,520,180,100,255);
		//砂嵐
		drawImage(scr,img.back,-390+a*4+dx,620-a*8+dy,580,320,200,400,255);
		drawImage(scr,img.back,-270+a*4+dx,560-a*8+dy,580,320,200,400,255);
		drawImage(scr,img.back,-150+a*4+dx,600-a*8+dy,580,320,200,400,255);
		//大砂嵐
		drawImage(scr,img.back,-560+a*5+dx,930-a*10+dy,780,320,200,400,255);
		drawImage(scr,img.back,-440+a*5+dx,900-a*10+dy,780,320,200,400,255);
		drawImage(scr,img.back,-320+a*5+dx,870-a*10+dy,780,320,200,400,255);
		//黒岩
		drawImage(scr,img.back,-600+a*6+dx,1260-a*12+dy,420,320,100,100,255);
		drawImage(scr,img.back,-470+a*6+dx,1200-a*12+dy,420,320,100,100,255);
		drawImage(scr,img.back,-340+a*6+dx,1220-a*12+dy,420,320,100,100,255);
		//風
		drawImage(scr,img.back,(a*2)%25+dx,200-(a%25)*16+dy,330,240,80,200,128);
		drawImage(scr,img.back,70+a%50+dx,190-(a%50)*8+dy,330,240,80,200,128);
		drawImage(scr,img.back,150+a%50+dx,210-(a%50)*8+dy,330,240,80,200,128);
		drawImage(scr,img.back,260+(a*2)%25+dx,220-(a%25)*16+dy,330,240,80,200,128);
		drawImage(scr,img.back,-200+(a%50)*12+dx,150-a%50+dy,330,440,250,80,128);
		fillRect(scr,0,0,320,20,0,0,0,255);
		fillRect(scr,0,220,320,20,0,0,0,255);
	}else{
		if(count==600){
			gd.scrX=gd.summerX-160;
			gd.scrY=gd.summerY-120;
			map.buffered=false;
		}
		SDL_Color col=getSkyColor(gd.hour,gd.minute);
		fillRect(scr,0,0,320,240,col.r,col.g,col.b,255);
		drawMap(scr,gd.scrX,gd.scrY);
		if(count<660)drawImage(scr,img.back,150,110,380,((count-600)/20)*20,20,20,255);
		else drawTalking(scr);
	}
}

