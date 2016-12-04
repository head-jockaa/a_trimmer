#include "animation.h"

#define CARTOON_DRAWIMAGE 1
#define CARTOON_FILLRECT 2
#define CARTOON_FILLRECT_GRAD 3
#define CARTOON_DRAWIMAGE_WAVE 4
#define CARTOON_CARLIGHT 5
#define CARTOON_ILLUMINATE 6
#define JSON_ARRAY_START 1
#define JSON_STARTTIME 2
#define JSON_DATA_START 3
#define JSON_GETNAME 4
#define JSON_COLON 5
#define JSON_GETVALUE 6
#define JSON_NEXTDATA 7
#define JSON_ENDDATA 8

bool readCartoonAct(char *json, int timer);
void resetObject(char *json, int timer);
void readJsonObject(char *json, char *name, int timer);
void readJsonSubObject(char *json, char *name1, char *name2, int timer);
void readJsonArray(char *json, char *name, int timer);

int playtime,cartoonNextTime;
int timestamp,loadtime,pausetime;
int max_obj;
int call_week,call_hour,call_minute;
bool talkmode=false,skipThisTime=false,cartoonSync=false;
size_t cartoonJsonSize,cartoonPointer;
char *cartoonJson, cartoonBgmName[200];

char jsonName[100][100], jsonValueString[100][1000];
int jsonValueNum;
double jsonValueDouble[100];
bool jsonValueBool[100];

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
CartoonObject obj[1000];

void resetObjectSetting(ObjectSetting &s){
	s.x=0;s.y=0;s.ix=0;s.iy=0;s.w=0;s.h=0;
	s.mag=1;s.alpha=255;s.shake=0;s.type=0;s.drawTo=-1;s.lang=-1;
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
	while((*s>='0' && *s<='9') || *s=='-') {
		if(*s=='-') {
			minus=-1;
		}
		else if(value==0 && *s=='0') {
		} else {
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
	while((*s>='0' && *s<='9') || *s=='-' || *s=='.') {
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
		else if(value==0 && *s=='0') {
		} else {
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
	cartoonPointer-=2;
}

void parseColor(char *s, SDL_Color *col) {
	int a=0;
	while(*s && a<6) {
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
		s++;
	}
}

void parseDate(char *s) {
	if(*s=='M' && *(s+1)=='o' && *(s+2)=='n')call_week=0;
	else if(*s=='T' && *(s+1)=='u' && *(s+2)=='e')call_week=1;
	else if(*s=='W' && *(s+1)=='e' && *(s+2)=='d')call_week=2;
	else if(*s=='T' && *(s+1)=='h' && *(s+2)=='u')call_week=3;
	else if(*s=='F' && *(s+1)=='r' && *(s+2)=='i')call_week=4;
	else if(*s=='S' && *(s+1)=='u' && *(s+2)=='t')call_week=5;
	else if(*s=='S' && *(s+1)=='u' && *(s+2)=='n')call_week=6;
	s+=3;
	while(true){
		if(*s>='0' && *s<='9')break;
		s++;
	}
	call_hour=fetchInt(s);
	while(true){
		if(*s>='0' && *s<='9')break;
		s++;
	}
	call_minute=fetchInt(s);
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
	skipThisTime=false;
	talkmode=false;
	cartoonSync=false;
	call_week=-1;call_hour=-1;call_minute=-1;
	if(cartoonJsonSize){
		delete [] cartoonJson;
		cartoonJsonSize=0;
	}
	for(int i=0 ; i<1000 ; i++){
		resetObject(i);
	}
	for(int i=0 ; i<10 ; i++){
		freeImage(img.bg[i]);
		freeSound(sf.sound[i]);
	}
}

void loadCartoon(const char *filename){
	freeCartoon();
	loadFile(filename);
	cartoonJsonSize=fsize;
	if(fsize){
		cartoonJson=new char[cartoonJsonSize];
		strcpy_s(cartoonJson,fstr);
		nextCut();
	}
}

bool readCartoon(char *json, int timer){
	int mode=JSON_ARRAY_START;
	char dataname[20];
	bool waitForNextTime=false, end=false;

	if(talkmode) {
		mode=JSON_GETVALUE;
		sprintf_s(dataname,"act");
	}
	else if(cartoonPointer!=0) {
		mode=JSON_NEXTDATA;
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
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			fetchString(c,dataname);
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(strcmp(dataname, "time")==0) {
				cartoonNextTime=fetchInt(c);
				if(cartoonNextTime!=playtime || end) {
					waitForNextTime=true;
				}
			}
			else if(strcmp(dataname, "date")==0) {
				fetchString(c,str);
				parseDate(str);
				if(call_week>gd.week
				|| (call_week==gd.week && call_hour>gd.hour)
				|| (call_week==gd.week && call_hour==gd.hour && call_minute>gd.minute)) {
					waitForNextTime=true;
					skipThisTime=false;
				}
				if(call_week<gd.week
				   || (call_week==gd.week && call_hour<gd.hour)
				   || (call_week==gd.week && call_hour==gd.hour && call_minute<gd.minute)) {
					skipThisTime=true;
				}
			}
			else if(strcmp(dataname, "act")==0) {
				end = readCartoonAct(json,timer);
				if(talkmode){
					break;
				}
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				if(waitForNextTime){
					break;
				} else {
					mode=JSON_GETNAME;
				}
			}
			else if(*c=='}') {
				mode=JSON_ENDDATA;
			}
		}
		else if(mode==JSON_ENDDATA) {
			if(*c==',') {
				mode=JSON_STARTTIME;
			}
			else if(*c==']') {
			}
		}
		cartoonPointer++;
	}
	return end;
}

bool readCartoonAct(char *json, int timer){
	int mode=JSON_DATA_START;
	char dataname[20];
	bool end=false;

	if(talkmode) {
		mode=JSON_GETVALUE;
	}

	while(cartoonPointer < cartoonJsonSize) {
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
			fetchString(c,dataname);
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(talkmode) {
				sprintf_s(dataname,"talk");
				readJsonArray(json, dataname, timer);
				if(talkmode){
					cartoonPointer++;
					break;
				}
			}
			else if(strcmp(dataname, "image")==0) {
				readJsonArray(json, dataname, timer);
			}
			else if(strcmp(dataname, "bgm")==0) {
				readJsonObject(json, dataname, timer);
			}
			else if(strcmp(dataname, "load-sound")==0) {
				readJsonArray(json, dataname, timer);
			}
			else if(strcmp(dataname, "sound")==0) {
				readJsonObject(json, dataname, timer);
			}
			else if(strcmp(dataname, "load-text")==0) {
				readJsonArray(json, dataname, timer);
			}
			else if(strcmp(dataname, "set")==0) {
				readJsonArray(json,dataname,timer);
			}
			else if(strcmp(dataname, "move")==0) {
				readJsonArray(json,dataname,timer);
			}
			else if(strcmp(dataname, "talk")==0) {
				readJsonArray(json, dataname, timer);
				if(talkmode){
					cartoonPointer++;
					break;
				}
			}
			else if(strcmp(dataname, "reset-timer")==0) {
				playtime=fetchInt(c);
			}
			else if(strcmp(dataname, "reset")==0) {
				resetObject(json,timer);
			}
			else if(strcmp(dataname, "sync")==0) {
				if(startsWith(c,"true")){
					loadtime=SDL_GetTicks();
					cartoonSync=true;
				}
				else if(startsWith(c,"false")){
					cartoonSync=false;
				}
			}
			else if(strcmp(dataname, "end")==0) {
				if(startsWith(c,"true")){
					end=true;
				}
			}
			else if(strcmp(dataname, "note")==0) {
				fetchString(c, str);
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				return end;
			}
		}
		cartoonPointer++;
	}
	return false;
}

void applyJsonData(char *json, char *name, int timer){
	if(strcmp(name,"set")==0){
		SDL_Color col;
		int id=0;
		for(int i=0 ; i<jsonValueNum ; i++){
			if(strcmp(jsonName[i],"id")==0)id=jsonValueDouble[i];
		}
		if(max_obj<id) {
			max_obj=id;
		}
		for(int i=0 ; i<jsonValueNum ; i++){
			if(strcmp(jsonName[i],"img_id")==0)obj[id].set.img_id=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"x")==0)obj[id].set.x=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"y")==0)obj[id].set.y=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"ix")==0){
				obj[id].set.ix=jsonValueDouble[i];
				obj[id].set.type=CARTOON_DRAWIMAGE;
			}
			else if(strcmp(jsonName[i],"iy")==0){
				obj[id].set.iy=jsonValueDouble[i];
				obj[id].set.type=CARTOON_DRAWIMAGE;
			}
			else if(strcmp(jsonName[i],"w")==0)obj[id].set.w=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"h")==0)obj[id].set.h=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"a")==0)obj[id].set.alpha=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"mag")==0)obj[id].set.mag=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"col")==0){
				parseColor(jsonValueString[i],&col);
				obj[id].set.R=col.r;
				obj[id].set.G=col.g;
				obj[id].set.B=col.b;
				obj[id].set.type=CARTOON_FILLRECT;
			}
			else if(strcmp(jsonName[i],"col_grad_y from")==0){
				parseColor(jsonValueString[i],&col);
				obj[id].set.gradRfrom=col.r;
				obj[id].set.gradGfrom=col.g;
				obj[id].set.gradBfrom=col.b;
				obj[id].set.type=CARTOON_FILLRECT_GRAD;
			}
			else if(strcmp(jsonName[i],"col_grad_y to")==0){
				parseColor(jsonValueString[i],&col);
				obj[id].set.gradRto=col.r;
				obj[id].set.gradGto=col.g;
				obj[id].set.gradBto=col.b;
				obj[id].set.type=CARTOON_FILLRECT_GRAD;
			}
			else if(strcmp(jsonName[i],"shake")==0)obj[id].set.shake=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"draw_to")==0)obj[id].set.drawTo=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"type")==0){
				if(strcmp(jsonValueString[i],"carlight")==0)obj[id].set.type=CARTOON_CARLIGHT;
				else if(strcmp(jsonValueString[i],"illuminate")==0)obj[id].set.type=CARTOON_ILLUMINATE;
			}
			else if(strcmp(jsonName[i],"lang")==0){
				if(strcmp(jsonValueString[i],"jp")==0)obj[id].set.lang=JAPANESE;
				else if(strcmp(jsonValueString[i],"en")==0)obj[id].set.lang=EUROPEAN;
			}
			else if(strcmp(jsonName[i],"wave sin")==0){
				obj[id].waveSIN=jsonValueDouble[i];
				obj[id].set.type=CARTOON_DRAWIMAGE_WAVE;
			}
			else if(strcmp(jsonName[i],"wave range")==0){
				obj[id].waveRange=jsonValueDouble[i];
				obj[id].set.type=CARTOON_DRAWIMAGE_WAVE;
			}
		}
	}
	if(strcmp(name,"move")==0){
		int id=0;
		for(int i=0 ; i<jsonValueNum ; i++){
			if(strcmp(jsonName[i],"id")==0)id=jsonValueDouble[i];
		}
		for(int i=0 ; i<jsonValueNum ; i++){
			if(strcmp(jsonName[i],"x")==0)obj[id].move.x=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"y")==0)obj[id].move.y=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"mag")==0)obj[id].move.mag=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"a")==0)obj[id].move.alpha=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide x step")==0){
				obj[id].slideX.step=jsonValueDouble[i];
				if(obj[id].slideX.step<0){
					obj[id].slideX.step*=-1;
					obj[id].slideX.back=true;
				}else{
					obj[id].slideX.back=false;
				}
			}
			else if(strcmp(jsonName[i],"slide x from")==0)obj[id].slideX.from=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide x to")==0)obj[id].slideX.to=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide x pos")==0)obj[id].slideX.position=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide x turnback")==0)obj[id].slideX.turnBack=jsonValueBool[i];
			else if(strcmp(jsonName[i],"slide y step")==0){
				obj[id].slideY.step=jsonValueDouble[i];
				if(obj[id].slideY.step<0){
					obj[id].slideY.step*=-1;
					obj[id].slideY.back=true;
				}else{
					obj[id].slideY.back=false;
				}
			}
			else if(strcmp(jsonName[i],"slide y from")==0)obj[id].slideY.from=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide y to")==0)obj[id].slideY.to=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide y pos")==0)obj[id].slideY.position=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide y turnback")==0)obj[id].slideY.turnBack=jsonValueBool[i];
			else if(strcmp(jsonName[i],"slide ix step")==0){
				obj[id].slideIX.step=jsonValueDouble[i];
				if(obj[id].slideIX.step<0){
					obj[id].slideIX.step*=-1;
					obj[id].slideIX.back=true;
				}else{
					obj[id].slideIX.back=false;
				}
			}
			else if(strcmp(jsonName[i],"slide ix from")==0)obj[id].slideIX.from=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide ix to")==0)obj[id].slideIX.to=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide ix pos")==0)obj[id].slideIX.position=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide ix turnback")==0)obj[id].slideIX.turnBack=jsonValueBool[i];
			else if(strcmp(jsonName[i],"slide iy step")==0){
				obj[id].slideIY.step=jsonValueDouble[i];
				if(obj[id].slideIY.step<0){
					obj[id].slideIY.step*=-1;
					obj[id].slideIY.back=true;
				}else{
					obj[id].slideIY.back=false;
				}
			}
			else if(strcmp(jsonName[i],"slide iy from")==0)obj[id].slideIY.from=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide iy to")==0)obj[id].slideIY.to=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide iy pos")==0)obj[id].slideIY.position=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide iy turnback")==0)obj[id].slideIY.turnBack=jsonValueBool[i];
			else if(strcmp(jsonName[i],"slide a step")==0){
				obj[id].slideAlpha.step=jsonValueDouble[i];
				if(obj[id].slideAlpha.step<0){
					obj[id].slideAlpha.step*=-1;
					obj[id].slideAlpha.back=true;
				}else{
					obj[id].slideAlpha.back=false;
				}
			}
			else if(strcmp(jsonName[i],"slide a from")==0)obj[id].slideAlpha.from=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide a to")==0)obj[id].slideAlpha.to=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide a pos")==0)obj[id].slideAlpha.position=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide a turnback")==0)obj[id].slideAlpha.turnBack=jsonValueBool[i];
			else if(strcmp(jsonName[i],"slide mag step")==0){
				obj[id].slideMag.step=jsonValueDouble[i];
				if(obj[id].slideMag.step<0){
					obj[id].slideMag.step*=-1;
					obj[id].slideMag.back=true;
				}else{
					obj[id].slideMag.back=false;
				}
			}
			else if(strcmp(jsonName[i],"slide mag from")==0)obj[id].slideMag.from=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide mag to")==0)obj[id].slideMag.to=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide mag pos")==0)obj[id].slideMag.position=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"slide mag turnback")==0)obj[id].slideMag.turnBack=jsonValueBool[i];
			else if(strcmp(jsonName[i],"jump g")==0)obj[id].jumpG=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"jump y")==0)obj[id].jumpY=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"ix")==0)obj[id].move.ix=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"iy")==0)obj[id].move.iy=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"w")==0)obj[id].move.w=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"h")==0)obj[id].move.h=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"shake")==0)obj[id].move.shake=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"flip in")==0){
				obj[id].flip.interval=jsonValueDouble[i];
				obj[id].flip.count=0;
			}
			else if(strcmp(jsonName[i],"flip ix")==0){
				obj[id].flip.ix=jsonValueDouble[i];
				obj[id].flip.count=0;
			}
			else if(strcmp(jsonName[i],"flip iy")==0){
				obj[id].flip.iy=jsonValueDouble[i];
				obj[id].flip.count=0;
			}
			else if(strcmp(jsonName[i],"flip n")==0){
				obj[id].flip.num=jsonValueDouble[i];
				obj[id].flip.count=0;
			}
			else if(strcmp(jsonName[i],"flip turnback")==0){
				obj[id].flip.turnBack=jsonValueDouble[i];
				obj[id].flip.count=0;
			}
			else if(strcmp(jsonName[i],"col_grad_y from col_r")==0)obj[id].move.gradRfrom=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"col_grad_y from col_g")==0)obj[id].move.gradGfrom=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"col_grad_y from col_b")==0)obj[id].move.gradBfrom=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"col_grad_y to col_r")==0)obj[id].move.gradRto=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"col_grad_y to col_g")==0)obj[id].move.gradGto=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"col_grad_y to col_b")==0)obj[id].move.gradBto=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"col_r")==0)obj[id].move.R=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"col_g")==0)obj[id].move.G=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"col_b")==0)obj[id].move.B=jsonValueDouble[i];
		}
		for(int i=0 ; i<jsonValueNum ; i++){
			if(strcmp(jsonName[i],"slide x fade")==0){
				if(obj[id].slideX.to!=obj[id].slideX.from){
					obj[id].slideX.fadeRate=jsonValueDouble[i]/(obj[id].slideX.to-obj[id].slideX.from);
				}
			}
			else if(strcmp(jsonName[i],"slide y fade")==0){
				if(obj[id].slideY.to!=obj[id].slideY.from){
					obj[id].slideY.fadeRate=jsonValueDouble[i]/(obj[id].slideY.to-obj[id].slideY.from);
				}
			}
			else if(strcmp(jsonName[i],"slide ix fade")==0){
				if(obj[id].slideIX.to!=obj[id].slideIX.from){
					obj[id].slideIX.fadeRate=jsonValueDouble[i]/(obj[id].slideIX.to-obj[id].slideIX.from);
				}
			}
			else if(strcmp(jsonName[i],"slide iy fade")==0){
				if(obj[id].slideIY.to!=obj[id].slideIY.from){
					obj[id].slideIY.fadeRate=jsonValueDouble[i]/(obj[id].slideIY.to-obj[id].slideIY.from);
				}
			}
			else if(strcmp(jsonName[i],"slide a fade")==0){
				if(obj[id].slideAlpha.to!=obj[id].slideAlpha.from){
					obj[id].slideAlpha.fadeRate=jsonValueDouble[i]/(obj[id].slideAlpha.to-obj[id].slideAlpha.from);
				}
			}
			else if(strcmp(jsonName[i],"slide mag fade")==0){
				if(obj[id].slideMag.to!=obj[id].slideMag.from){
					obj[id].slideMag.fadeRate=jsonValueDouble[i]/(obj[id].slideMag.to-obj[id].slideMag.from);
				}
			}
		}
	}
	if(strcmp(name,"image")==0){
		int id=0,w=0,h=0;
		char file[100];
		file[0]=0;
		for(int i=0 ; i<jsonValueNum ; i++){
			if(strcmp(jsonName[i],"id")==0)id=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"file")==0)strcpy_s(file,jsonValueString[i]);
			else if(strcmp(jsonName[i],"w")==0)w=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"h")==0)h=jsonValueDouble[i];
		}
		freeImage(img.bg[id]);
		if(file[0]){
			getImage(img.bg[id],file,0,0,255);
		}
		if(w!=0 && h!=0){
			img.bg[id]=new Image(w,h);
		}
	}
	else if(strcmp(name,"load-sound")==0){
		int id=0;
		char file[100];
		for(int i=0 ; i<jsonValueNum ; i++){
			if(strcmp(jsonName[i],"id")==0)id=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"file")==0)strcpy_s(file,jsonValueString[i]);
		}
		freeSound(sf.sound[id]);
		sf.sound[id]=Mix_LoadWAV(file);
	}
	else if(strcmp(name,"sound")==0){
		int id=0,ch=0,repeat=0;
		for(int i=0 ; i<jsonValueNum ; i++){
			if(strcmp(jsonName[i],"id")==0)id=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"ch")==0)ch=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"repeat")==0)repeat=jsonValueDouble[i];
		}
		Mix_PlayChannel(ch, sf.sound[id], repeat);
	}
	else if(strcmp(name,"bgm")==0){
		int repeat=-1;
		char file[100];
		for(int i=0 ; i<jsonValueNum ; i++){
			if(strcmp(jsonName[i],"file")==0)strcpy_s(file,jsonValueString[i]);
			else if(strcmp(jsonName[i],"repeat")==0)repeat=jsonValueDouble[i];
		}
		if(strcmp(cartoonBgmName,str)!=0){
			freeMusic();
			strcpy_s(cartoonBgmName,file);
			bgm=Mix_LoadMUS(file);
			Mix_PlayMusic(bgm, repeat);
		}
	}
	else if(strcmp(name,"load-text")==0){
		int which_lang=0;
		char file[100], lang[20];
		file[0]=0;
		for(int i=0 ; i<jsonValueNum ; i++){
			if(strcmp(jsonName[i],"file")==0)strcpy_s(file,jsonValueString[i]);
			else if(strcmp(jsonName[i],"lang")==0)strcpy_s(lang,jsonValueString[i]);
		}
		if(strcmp(lang, "jp")==0) {
			which_lang=0;
		}
		else if(strcmp(lang, "en")==0) {
			which_lang=1;
		}
		loadFile(file);
		size_t fc=0;
		for(int i=0 ; i<1000 ; i++){
			if(fc>=fsize)break;
			for(int j=0 ; j<200 ; j++){
				talk[i].str[which_lang][j]=0;
			}
			for(int j=0 ; j<200 ; j++){
				talk[i].str[which_lang][j]=fstr[fc];fc++;
				if(talk[i].str[which_lang][j]==0)break;
			}
		}
		for(int i=0 ; i<1000 ; i++){
			padSpace(i,which_lang,30);
		}
	}
	else if(strcmp(name,"talk")==0){
		int face=0,text=0,shake=0;
		for(int i=0 ; i<jsonValueNum ; i++){
			if(strcmp(jsonName[i],"face")==0)face=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"text")==0)text=jsonValueDouble[i];
			else if(strcmp(jsonName[i],"shake")==0)shake=jsonValueBool[i];
		}
		gd.face_count=face;
		gd.talk_count=text;
		if(shake){
			gd.shake_count=50;
			Mix_PlayChannel(0,sf.decide,0);
		}
	}
}

void readJsonArray(char *json, char *name, int timer){
	int mode=JSON_ARRAY_START;

	if(talkmode) {
		mode=JSON_NEXTDATA;
	}

	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_ARRAY_START) {
			if(*c=='[') {
				readJsonObject(json,name,timer);
				if(talkmode){
					break;
				}
				mode=JSON_NEXTDATA;
			}
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				cartoonPointer++;
				readJsonObject(json,name,timer);
				if(talkmode){
					break;
				}
			}
			else if(*c==']') {
				if(strcmp(name,"talk")==0){
					talkmode=false;
					playtime=0;
				}
				break;
			}
		}
		cartoonPointer++;
	}
}

void readJsonObject(char *json, char *name, int timer){
	int mode=JSON_DATA_START;
	while(json[cartoonPointer]) {
		char *c = &json[cartoonPointer];
		if(*c==' ' || *c==10 || *c==13) {
			cartoonPointer++;
			continue;
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				jsonValueNum=0;
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			fetchString(c,jsonName[jsonValueNum]);
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(*c=='"'){
				fetchString(c,jsonValueString[jsonValueNum]);
			}
			else if((*c>='0' && *c<='9') || *c=='-'){
				jsonValueDouble[jsonValueNum]=fetchDouble(c);
			}
			else if(*c=='{'){
				char basename[20];
				strcpy_s(basename,jsonName[jsonValueNum]);
				readJsonSubObject(json,name,basename,timer);
			}
			else if(startsWith(c,"true")){
				jsonValueBool[jsonValueNum]=true;
			}
			else if(startsWith(c,"false")){
				jsonValueBool[jsonValueNum]=false;
			}
			jsonValueNum++;
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				applyJsonData(json,name,timer);
				if(strcmp(name,"talk")==0){
					if(!skipThisTime)talkmode=true;
					gd.text_count=0;
				}
				break;
			}
		}
		cartoonPointer++;
	}
}

void readJsonSubObject(char *json, char *name1, char *name2, int timer){
	int mode=JSON_DATA_START;
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
			sprintf_s(jsonName[jsonValueNum],"%s %s",name2,str);
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(*c=='"'){
				fetchString(c,jsonValueString[jsonValueNum]);
			}
			else if((*c>='0' && *c<='9') || *c=='-'){
				jsonValueDouble[jsonValueNum]=fetchDouble(c);
			}
			else if(*c=='{'){
				char basename[20];
				strcpy_s(basename,jsonName[jsonValueNum]);
				readJsonSubObject(json,name2,basename,timer);
			}
			else if(startsWith(c,"true")){
				jsonValueBool[jsonValueNum]=true;
			}
			else if(startsWith(c,"false")){
				jsonValueBool[jsonValueNum]=false;
			}
			jsonValueNum++;
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				jsonValueNum--;
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

bool _nextCut(){
	_moveObject();
	if(!talkmode){
		bool end=false;
		if(playtime==cartoonNextTime ||
		   (gd.week==call_week && gd.hour==call_hour && gd.minute==call_minute)){
			end=readCartoon(cartoonJson,playtime);
		}
		playtime++;
		if(end)return true;
	}else{
		gd.text_count++;
		if(gd.shake_count>0)gd.shake_count--;
		if(gd.talk_open_count>0){
			gd.talk_open_count++;
			if(gd.talk_open_count==20)gd.talk_open_count=0;
		}
	}
	return false;
}

bool nextCut(){
	bool end=false;
	int t=1;
	if(cartoonSync){
		t=(int)((SDL_GetTicks()-loadtime)/16)-playtime;
	}
	for(int i=0 ; i<t ; i++){
		end=_nextCut();
		if(end)break;
	}
	return end;
}

bool nextTalk(){
	if(talkmode){
		bool end=false;
		if(gd.text_count<(int)strlen(talk[gd.talk_count].str[CHAR_CODE])){
			gd.text_count=90;
		}else{
			end=readCartoon(cartoonJson,playtime);
		}
		if(end)return true;
	}
	return false;
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
		if(obj[i].set.drawTo!=-1){
			if(obj[i].set.type==CARTOON_DRAWIMAGE){
				if(mag==1){
					drawImage(img.bg[obj[i].set.drawTo],img.bg[obj[i].set.img_id],x,y,ix,iy,w,h,a);
				}else{
					drawImage_x(img.bg[obj[i].set.drawTo],img.bg[obj[i].set.img_id],x,y,mag,ix,iy,w,h,a);
				}
			}
			if(obj[i].set.type==CARTOON_ILLUMINATE){
				if(mag==1){
					illuminateImage(img.bg[obj[i].set.drawTo],img.bg[obj[i].set.img_id],x,y,ix,iy,w,h,a);
				}else{
					illuminateImage_x(img.bg[obj[i].set.drawTo],img.bg[obj[i].set.img_id],x,y,mag,ix,iy,w,h,a);
				}
			}
			else if(obj[i].set.type==CARTOON_FILLRECT){
				fillRect(img.bg[obj[i].set.drawTo],x,y,w,h,(int)obj[i].set.R,(int)obj[i].set.G,(int)obj[i].set.B,a);
			}
			else if(obj[i].set.type==CARTOON_CARLIGHT){
				drawLightBall(img.bg[obj[i].set.drawTo],x,y,w,(int)obj[i].set.R,(int)obj[i].set.G,(int)obj[i].set.B);
			}
			else if(obj[i].set.type==CARTOON_FILLRECT_GRAD){
				for(int j=0 ; j<h ; j++){
					int R=(int)(obj[i].set.gradRfrom+(obj[i].set.gradRto-obj[i].set.gradRfrom)*j/h);
					int G=(int)(obj[i].set.gradGfrom+(obj[i].set.gradGto-obj[i].set.gradGfrom)*j/h);
					int B=(int)(obj[i].set.gradBfrom+(obj[i].set.gradBto-obj[i].set.gradBfrom)*j/h);
					fillRect(img.bg[obj[i].set.drawTo],x,y+j,w,1,R,G,B,a);
				}
			}
			else if(obj[i].set.type==CARTOON_DRAWIMAGE_WAVE){
				for(int j=0 ; j<h ; j++){
					drawImage(img.bg[obj[i].set.drawTo],img.bg[obj[i].set.img_id],x+(int)(sin((count+j)*obj[i].waveSIN)*obj[i].waveRange),y+j,ix,iy+j,w,1,a);
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
			if(obj[i].set.type==CARTOON_ILLUMINATE){
				if(mag==1){
					illuminateImage(scr,img.bg[obj[i].set.img_id],x,y,ix,iy,w,h,a);
				}else{
					illuminateImage_x(scr,img.bg[obj[i].set.img_id],x,y,mag,ix,iy,w,h,a);
				}
			}
			else if(obj[i].set.type==CARTOON_FILLRECT){
				fillRect(scr,x,y,w,h,(int)obj[i].set.R,(int)obj[i].set.G,(int)obj[i].set.B,a);
			}
			else if(obj[i].set.type==CARTOON_CARLIGHT){
				drawLightBall(scr,x,y,w,(int)obj[i].set.R,(int)obj[i].set.G,(int)obj[i].set.B);
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

