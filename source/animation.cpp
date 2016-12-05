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

bool readCartoonAct(char *json);
void resetObject(char *json);
void readJsonObject(char *json, char *name);
void readJsonSubObject(char *json, char *name1, char *name2);
void readJsonArray(char *json, char *name);

int playtime,cartoonNextTime;
int timestamp,pausetime;
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

bool readCartoon(char *json){
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
				end = readCartoonAct(json);
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

bool readCartoonAct(char *json){
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
				readJsonArray(json, dataname);
				if(talkmode){
					cartoonPointer++;
					break;
				}
			}
			else if(strcmp(dataname, "image")==0) {
				readJsonArray(json, dataname);
			}
			else if(strcmp(dataname, "bgm")==0) {
				readJsonObject(json, dataname);
			}
			else if(strcmp(dataname, "load-sound")==0) {
				readJsonArray(json, dataname);
			}
			else if(strcmp(dataname, "sound")==0) {
				readJsonObject(json, dataname);
			}
			else if(strcmp(dataname, "load-text")==0) {
				readJsonArray(json, dataname);
			}
			else if(strcmp(dataname, "set")==0) {
				readJsonArray(json,dataname);
			}
			else if(strcmp(dataname, "move")==0) {
				readJsonArray(json,dataname);
			}
			else if(strcmp(dataname, "talk")==0) {
				readJsonArray(json, dataname);
				if(talkmode){
					cartoonPointer++;
					break;
				}
			}
			else if(strcmp(dataname, "reset-timer")==0) {
				playtime=fetchInt(c);
			}
			else if(strcmp(dataname, "reset")==0) {
				resetObject(json);
			}
			else if(strcmp(dataname, "sync")==0) {
				if(startsWith(c,"true")){
					timestamp=SDL_GetTicks();
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

void applyJsonData(char *json, char *name){
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

void readJsonArray(char *json, char *name){
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
				readJsonObject(json,name);
				if(talkmode){
					break;
				}
				mode=JSON_NEXTDATA;
			}
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				cartoonPointer++;
				readJsonObject(json,name);
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

void readJsonObject(char *json, char *name){
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
				readJsonSubObject(json,name,basename);
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
				applyJsonData(json,name);
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

void readJsonSubObject(char *json, char *name1, char *name2){
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
				readJsonSubObject(json,name2,basename);
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

void resetObject(char *json){
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
			end=readCartoon(cartoonJson);
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
		t=(int)((SDL_GetTicks()-timestamp)/16)-playtime;
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
			end=readCartoon(cartoonJson);
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
