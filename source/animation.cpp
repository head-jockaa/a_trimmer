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

bool readCartoonAct(JsonData *json);
void resetObject(JsonData *json);
void resetObject(JsonData *json, CartoonObject &obj);
void readJsonObject(JsonData *json, char *name);
void readJsonSubObject(JsonData *json, char *name1, char *name2);
void readJsonArray(JsonData *json, char *name);

JsonData cartoonJson,talkingJson;

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
void resetObject(CartoonObject &obj){
	resetObjectSetting(obj.set);
	resetObjectMoving(obj.move);
	resetObjectFlipping(obj.flip);
	resetObjectSliding(obj.slideX);
	resetObjectSliding(obj.slideY);
	resetObjectSliding(obj.slideIX);
	resetObjectSliding(obj.slideIY);
	resetObjectSliding(obj.slideMag);
	resetObjectSliding(obj.slideAlpha);
	obj.jumpG=0;obj.jumpY=0;
	obj.waveSIN=0;obj.waveRange=0;
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

int fetchInt(char *s, int *value) {
	int minus=1, length=0;
	*value=0;
	while((*s>='0' && *s<='9') || *s=='-') {
		if(*s=='-') {
			minus=-1;
		}
		else if(*value==0 && *s=='0') {
		} else {
			*value*=10;
			*value+=(*s-48);
		}
		length++;
		s++;
	}
	*value *= minus;
	return length;
}

int fetchDouble(char *s, double *value) {
	double digit=1;
	int minus=1, length=0;
	bool decimal=false;
	*value=0;
	while((*s>='0' && *s<='9') || *s=='-' || *s=='.') {
		if(*s=='-') {
			minus=-1;
		}
		else if(*s=='.') {
			decimal=true;
			digit=0.1;
		}
		else if(decimal) {
			*value += (*s-48)*digit;
			digit*=0.1;
		}
		else if(value==0 && *s=='0') {
		} else {
			*value*=10;
			*value+=(*s-48);
		}
		length++;
		s++;
	}
	*value *= minus;
	return length;
}

int fetchString(char *s, char brace, char *value) {
	bool start=false;
	int pointer=0, length=0;
	while(*s) {
		if(start) {
			if(*s==brace) {
				value[pointer]=0;
				break;
			}
			else if(*s=='\\' && *(s+1)==brace) {
				value[pointer]=brace;
				s++;
			}else{
				value[pointer]=*s;
			}
			pointer++;
		}
		else if(*s==brace) {
			start=true;
		}else{
			break;
		}
		length++;
		s++;
	}
	return length;
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

void parseDate(JsonData *json, char *s) {
	if(*s=='M' && *(s+1)=='o' && *(s+2)=='n')json->call_week=0;
	else if(*s=='T' && *(s+1)=='u' && *(s+2)=='e')json->call_week=1;
	else if(*s=='W' && *(s+1)=='e' && *(s+2)=='d')json->call_week=2;
	else if(*s=='T' && *(s+1)=='h' && *(s+2)=='u')json->call_week=3;
	else if(*s=='F' && *(s+1)=='r' && *(s+2)=='i')json->call_week=4;
	else if(*s=='S' && *(s+1)=='u' && *(s+2)=='t')json->call_week=5;
	else if(*s=='S' && *(s+1)=='u' && *(s+2)=='n')json->call_week=6;
	s+=3;
	while(true){
		if(*s>='0' && *s<='9')break;
		s++;
	}
	fetchInt(s,&json->call_hour);
	while(true){
		if(*s>='0' && *s<='9')break;
		s++;
	}
	fetchInt(s,&json->call_minute);
}

bool startsWith(char *s, const char *target) {
	for(size_t i=0 ; i<strlen(target) ; i++) {
		if(s[i]!=target[i]) {
			return false;
		}
	}
	return true;
}

void freeCartoon(JsonData *json){
	json->max_obj=0;
	json->playtime=0;
	json->pointer=0;
	json->nextTime=0;
	json->skipThisTime=false;
	json->talkmode=false;
	json->cartoonSync=false;
	json->call_week=-1;json->call_hour=-1;json->call_minute=-1;
	if(json->size){
		delete [] json->text;
		json->size=0;
	}
	for(int i=0 ; i<1000 ; i++){
		resetObject(json->obj[i]);
	}
	for(int i=0 ; i<10 ; i++){
		freeImage(img.bg[i]);
		freeSound(sf.sound[i]);
	}
}

void loadCartoon(JsonData *json, const char *filename){
	freeCartoon(json);
	loadFile(filename);
	json->size=fsize;
	if(fsize){
		json->text=new char[json->size];
		strcpy_s(json->text,fstr);
		nextCut(json);
	}
}

bool readCartoon(JsonData *json){
	int mode=JSON_ARRAY_START;
	char dataname[20];
	bool waitForNextTime=false, end=false;

	if(json->talkmode) {
		mode=JSON_GETVALUE;
		sprintf_s(dataname,"act");
	}
	else if(json->pointer!=0) {
		mode=JSON_NEXTDATA;
	}

	while(json->pointer < json->size) {
		char *c = &json->text[json->pointer];
		if(*c==' ' || *c==10 || *c==13) {
			json->pointer++;
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
			json->pointer+=fetchString(c,'"',dataname)-1;
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(strcmp(dataname, "time")==0) {
				json->pointer+=fetchInt(c,&json->nextTime)-1;
				if(json->nextTime!=json->playtime || end) {
					waitForNextTime=true;
				}
			}
			else if(strcmp(dataname, "date")==0) {
				json->pointer+=fetchString(c,'"',str)-1;
				parseDate(json,str);
				if(json->call_week>gd.week
				|| (json->call_week==gd.week && json->call_hour>gd.hour)
				|| (json->call_week==gd.week && json->call_hour==gd.hour && json->call_minute>gd.minute)) {
					waitForNextTime=true;
					json->skipThisTime=false;
				}
				if(json->call_week<gd.week
				   || (json->call_week==gd.week && json->call_hour<gd.hour)
				   || (json->call_week==gd.week && json->call_hour==gd.hour && json->call_minute<gd.minute)) {
					json->skipThisTime=true;
				}
			}
			else if(strcmp(dataname, "act")==0) {
				end = readCartoonAct(json);
				if(json->talkmode){
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
		json->pointer++;
	}
	return end;
}

bool readCartoonAct(JsonData *json){
	int mode=JSON_DATA_START;
	char dataname[20];
	bool end=false;

	if(json->talkmode) {
		mode=JSON_GETVALUE;
	}

	while(json->pointer < json->size) {
		char *c = &json->text[json->pointer];
		if(*c==' ' || *c==10 || *c==13) {
			json->pointer++;
			continue;
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			json->pointer+=fetchString(c,'"',dataname)-1;
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(json->talkmode) {
				sprintf_s(dataname,"talk");
				readJsonArray(json,dataname);
				if(json->talkmode){
					json->pointer++;
					break;
				}
			}
			else if(strcmp(dataname, "image")==0) {
				readJsonArray(json,dataname);
			}
			else if(strcmp(dataname, "bgm")==0) {
				readJsonObject(json,dataname);
			}
			else if(strcmp(dataname, "load-sound")==0) {
				readJsonArray(json,dataname);
			}
			else if(strcmp(dataname, "sound")==0) {
				readJsonObject(json,dataname);
			}
			else if(strcmp(dataname, "load-text")==0) {
				readJsonArray(json,dataname);
			}
			else if(strcmp(dataname, "set")==0) {
				readJsonArray(json,dataname);
			}
			else if(strcmp(dataname, "move")==0) {
				readJsonArray(json,dataname);
			}
			else if(strcmp(dataname, "talk")==0) {
				readJsonArray(json,dataname);
				if(json->talkmode){
					json->pointer++;
					break;
				}
			}
			else if(strcmp(dataname, "reset-timer")==0) {
				json->pointer+=fetchInt(c,&json->playtime)-1;
			}
			else if(strcmp(dataname, "reset")==0) {
				resetObject(json);
			}
			else if(strcmp(dataname, "sync")==0) {
				if(startsWith(c,"true")){
					json->pointer+=3;
					json->timestamp=SDL_GetTicks();
					json->cartoonSync=true;
				}
				else if(startsWith(c,"false")){
					json->pointer+=4;
					json->cartoonSync=false;
				}
			}
			else if(strcmp(dataname, "end")==0) {
				if(startsWith(c,"true")){
					json->pointer+=3;
					end=true;
				}
			}
			else if(strcmp(dataname, "note")==0) {
				json->pointer+=fetchString(c,'"',str)-1;
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
		json->pointer++;
	}
	return false;
}

void applyJsonData(JsonData *json, char *name){
	if(strcmp(name,"set")==0){
		SDL_Color col;
		int id=0;
		for(int i=0 ; i<json->valueNum ; i++){
			if(strcmp(json->name[i],"id")==0)id=json->valueDouble[i];
		}
		if(json->max_obj<id) {
			json->max_obj=id;
		}
		for(int i=0 ; i<json->valueNum ; i++){
			if(strcmp(json->name[i],"img_id")==0)json->obj[id].set.img_id=json->valueDouble[i];
			else if(strcmp(json->name[i],"x")==0)json->obj[id].set.x=json->valueDouble[i];
			else if(strcmp(json->name[i],"y")==0)json->obj[id].set.y=json->valueDouble[i];
			else if(strcmp(json->name[i],"ix")==0){
				json->obj[id].set.ix=json->valueDouble[i];
				json->obj[id].set.type=CARTOON_DRAWIMAGE;
			}
			else if(strcmp(json->name[i],"iy")==0){
				json->obj[id].set.iy=json->valueDouble[i];
				json->obj[id].set.type=CARTOON_DRAWIMAGE;
			}
			else if(strcmp(json->name[i],"w")==0)json->obj[id].set.w=json->valueDouble[i];
			else if(strcmp(json->name[i],"h")==0)json->obj[id].set.h=json->valueDouble[i];
			else if(strcmp(json->name[i],"a")==0)json->obj[id].set.alpha=json->valueDouble[i];
			else if(strcmp(json->name[i],"mag")==0)json->obj[id].set.mag=json->valueDouble[i];
			else if(strcmp(json->name[i],"col")==0){
				parseColor(json->valueString[i],&col);
				json->obj[id].set.R=col.r;
				json->obj[id].set.G=col.g;
				json->obj[id].set.B=col.b;
				json->obj[id].set.type=CARTOON_FILLRECT;
			}
			else if(strcmp(json->name[i],"col_grad_y from")==0){
				parseColor(json->valueString[i],&col);
				json->obj[id].set.gradRfrom=col.r;
				json->obj[id].set.gradGfrom=col.g;
				json->obj[id].set.gradBfrom=col.b;
				json->obj[id].set.type=CARTOON_FILLRECT_GRAD;
			}
			else if(strcmp(json->name[i],"col_grad_y to")==0){
				parseColor(json->valueString[i],&col);
				json->obj[id].set.gradRto=col.r;
				json->obj[id].set.gradGto=col.g;
				json->obj[id].set.gradBto=col.b;
				json->obj[id].set.type=CARTOON_FILLRECT_GRAD;
			}
			else if(strcmp(json->name[i],"shake")==0)json->obj[id].set.shake=json->valueDouble[i];
			else if(strcmp(json->name[i],"draw_to")==0)json->obj[id].set.drawTo=json->valueDouble[i];
			else if(strcmp(json->name[i],"type")==0){
				if(strcmp(json->valueString[i],"carlight")==0)json->obj[id].set.type=CARTOON_CARLIGHT;
				else if(strcmp(json->valueString[i],"illuminate")==0)json->obj[id].set.type=CARTOON_ILLUMINATE;
			}
			else if(strcmp(json->name[i],"lang")==0){
				if(strcmp(json->valueString[i],"jp")==0)json->obj[id].set.lang=JAPANESE;
				else if(strcmp(json->valueString[i],"en")==0)json->obj[id].set.lang=EUROPEAN;
			}
			else if(strcmp(json->name[i],"wave sin")==0){
				json->obj[id].waveSIN=json->valueDouble[i];
				json->obj[id].set.type=CARTOON_DRAWIMAGE_WAVE;
			}
			else if(strcmp(json->name[i],"wave range")==0){
				json->obj[id].waveRange=json->valueDouble[i];
				json->obj[id].set.type=CARTOON_DRAWIMAGE_WAVE;
			}
		}
	}
	if(strcmp(name,"move")==0){
		int id=0;
		for(int i=0 ; i<json->valueNum ; i++){
			if(strcmp(json->name[i],"id")==0)id=json->valueDouble[i];
		}
		for(int i=0 ; i<json->valueNum ; i++){
			if(strcmp(json->name[i],"x")==0)json->obj[id].move.x=json->valueDouble[i];
			else if(strcmp(json->name[i],"y")==0)json->obj[id].move.y=json->valueDouble[i];
			else if(strcmp(json->name[i],"mag")==0)json->obj[id].move.mag=json->valueDouble[i];
			else if(strcmp(json->name[i],"a")==0)json->obj[id].move.alpha=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide x step")==0){
				json->obj[id].slideX.step=json->valueDouble[i];
				if(json->obj[id].slideX.step<0){
					json->obj[id].slideX.step*=-1;
					json->obj[id].slideX.back=true;
				}else{
					json->obj[id].slideX.back=false;
				}
			}
			else if(strcmp(json->name[i],"slide x from")==0)json->obj[id].slideX.from=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide x to")==0)json->obj[id].slideX.to=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide x pos")==0)json->obj[id].slideX.position=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide x turnback")==0)json->obj[id].slideX.turnBack=json->valueBool[i];
			else if(strcmp(json->name[i],"slide y step")==0){
				json->obj[id].slideY.step=json->valueDouble[i];
				if(json->obj[id].slideY.step<0){
					json->obj[id].slideY.step*=-1;
					json->obj[id].slideY.back=true;
				}else{
					json->obj[id].slideY.back=false;
				}
			}
			else if(strcmp(json->name[i],"slide y from")==0)json->obj[id].slideY.from=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide y to")==0)json->obj[id].slideY.to=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide y pos")==0)json->obj[id].slideY.position=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide y turnback")==0)json->obj[id].slideY.turnBack=json->valueBool[i];
			else if(strcmp(json->name[i],"slide ix step")==0){
				json->obj[id].slideIX.step=json->valueDouble[i];
				if(json->obj[id].slideIX.step<0){
					json->obj[id].slideIX.step*=-1;
					json->obj[id].slideIX.back=true;
				}else{
					json->obj[id].slideIX.back=false;
				}
			}
			else if(strcmp(json->name[i],"slide ix from")==0)json->obj[id].slideIX.from=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide ix to")==0)json->obj[id].slideIX.to=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide ix pos")==0)json->obj[id].slideIX.position=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide ix turnback")==0)json->obj[id].slideIX.turnBack=json->valueBool[i];
			else if(strcmp(json->name[i],"slide iy step")==0){
				json->obj[id].slideIY.step=json->valueDouble[i];
				if(json->obj[id].slideIY.step<0){
					json->obj[id].slideIY.step*=-1;
					json->obj[id].slideIY.back=true;
				}else{
					json->obj[id].slideIY.back=false;
				}
			}
			else if(strcmp(json->name[i],"slide iy from")==0)json->obj[id].slideIY.from=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide iy to")==0)json->obj[id].slideIY.to=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide iy pos")==0)json->obj[id].slideIY.position=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide iy turnback")==0)json->obj[id].slideIY.turnBack=json->valueBool[i];
			else if(strcmp(json->name[i],"slide a step")==0){
				json->obj[id].slideAlpha.step=json->valueDouble[i];
				if(json->obj[id].slideAlpha.step<0){
					json->obj[id].slideAlpha.step*=-1;
					json->obj[id].slideAlpha.back=true;
				}else{
					json->obj[id].slideAlpha.back=false;
				}
			}
			else if(strcmp(json->name[i],"slide a from")==0)json->obj[id].slideAlpha.from=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide a to")==0)json->obj[id].slideAlpha.to=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide a pos")==0)json->obj[id].slideAlpha.position=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide a turnback")==0)json->obj[id].slideAlpha.turnBack=json->valueBool[i];
			else if(strcmp(json->name[i],"slide mag step")==0){
				json->obj[id].slideMag.step=json->valueDouble[i];
				if(json->obj[id].slideMag.step<0){
					json->obj[id].slideMag.step*=-1;
					json->obj[id].slideMag.back=true;
				}else{
					json->obj[id].slideMag.back=false;
				}
			}
			else if(strcmp(json->name[i],"slide mag from")==0)json->obj[id].slideMag.from=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide mag to")==0)json->obj[id].slideMag.to=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide mag pos")==0)json->obj[id].slideMag.position=json->valueDouble[i];
			else if(strcmp(json->name[i],"slide mag turnback")==0)json->obj[id].slideMag.turnBack=json->valueBool[i];
			else if(strcmp(json->name[i],"jump g")==0)json->obj[id].jumpG=json->valueDouble[i];
			else if(strcmp(json->name[i],"jump y")==0)json->obj[id].jumpY=json->valueDouble[i];
			else if(strcmp(json->name[i],"ix")==0)json->obj[id].move.ix=json->valueDouble[i];
			else if(strcmp(json->name[i],"iy")==0)json->obj[id].move.iy=json->valueDouble[i];
			else if(strcmp(json->name[i],"w")==0)json->obj[id].move.w=json->valueDouble[i];
			else if(strcmp(json->name[i],"h")==0)json->obj[id].move.h=json->valueDouble[i];
			else if(strcmp(json->name[i],"shake")==0)json->obj[id].move.shake=json->valueDouble[i];
			else if(strcmp(json->name[i],"flip in")==0){
				json->obj[id].flip.interval=json->valueDouble[i];
				json->obj[id].flip.count=0;
			}
			else if(strcmp(json->name[i],"flip ix")==0){
				json->obj[id].flip.ix=json->valueDouble[i];
				json->obj[id].flip.count=0;
			}
			else if(strcmp(json->name[i],"flip iy")==0){
				json->obj[id].flip.iy=json->valueDouble[i];
				json->obj[id].flip.count=0;
			}
			else if(strcmp(json->name[i],"flip n")==0){
				json->obj[id].flip.num=json->valueDouble[i];
				json->obj[id].flip.count=0;
			}
			else if(strcmp(json->name[i],"flip turnback")==0){
				json->obj[id].flip.turnBack=json->valueDouble[i];
				json->obj[id].flip.count=0;
			}
			else if(strcmp(json->name[i],"col_grad_y from col_r")==0)json->obj[id].move.gradRfrom=json->valueDouble[i];
			else if(strcmp(json->name[i],"col_grad_y from col_g")==0)json->obj[id].move.gradGfrom=json->valueDouble[i];
			else if(strcmp(json->name[i],"col_grad_y from col_b")==0)json->obj[id].move.gradBfrom=json->valueDouble[i];
			else if(strcmp(json->name[i],"col_grad_y to col_r")==0)json->obj[id].move.gradRto=json->valueDouble[i];
			else if(strcmp(json->name[i],"col_grad_y to col_g")==0)json->obj[id].move.gradGto=json->valueDouble[i];
			else if(strcmp(json->name[i],"col_grad_y to col_b")==0)json->obj[id].move.gradBto=json->valueDouble[i];
			else if(strcmp(json->name[i],"col_r")==0)json->obj[id].move.R=json->valueDouble[i];
			else if(strcmp(json->name[i],"col_g")==0)json->obj[id].move.G=json->valueDouble[i];
			else if(strcmp(json->name[i],"col_b")==0)json->obj[id].move.B=json->valueDouble[i];
		}
		for(int i=0 ; i<json->valueNum ; i++){
			if(strcmp(json->name[i],"slide x fade")==0){
				if(json->obj[id].slideX.to!=json->obj[id].slideX.from){
					json->obj[id].slideX.fadeRate=json->valueDouble[i]/(json->obj[id].slideX.to-json->obj[id].slideX.from);
				}
			}
			else if(strcmp(json->name[i],"slide y fade")==0){
				if(json->obj[id].slideY.to!=json->obj[id].slideY.from){
					json->obj[id].slideY.fadeRate=json->valueDouble[i]/(json->obj[id].slideY.to-json->obj[id].slideY.from);
				}
			}
			else if(strcmp(json->name[i],"slide ix fade")==0){
				if(json->obj[id].slideIX.to!=json->obj[id].slideIX.from){
					json->obj[id].slideIX.fadeRate=json->valueDouble[i]/(json->obj[id].slideIX.to-json->obj[id].slideIX.from);
				}
			}
			else if(strcmp(json->name[i],"slide iy fade")==0){
				if(json->obj[id].slideIY.to!=json->obj[id].slideIY.from){
					json->obj[id].slideIY.fadeRate=json->valueDouble[i]/(json->obj[id].slideIY.to-json->obj[id].slideIY.from);
				}
			}
			else if(strcmp(json->name[i],"slide a fade")==0){
				if(json->obj[id].slideAlpha.to!=json->obj[id].slideAlpha.from){
					json->obj[id].slideAlpha.fadeRate=json->valueDouble[i]/(json->obj[id].slideAlpha.to-json->obj[id].slideAlpha.from);
				}
			}
			else if(strcmp(json->name[i],"slide mag fade")==0){
				if(json->obj[id].slideMag.to!=json->obj[id].slideMag.from){
					json->obj[id].slideMag.fadeRate=json->valueDouble[i]/(json->obj[id].slideMag.to-json->obj[id].slideMag.from);
				}
			}
		}
	}
	if(strcmp(name,"image")==0){
		int id=0,w=0,h=0;
		char file[100];
		file[0]=0;
		for(int i=0 ; i<json->valueNum ; i++){
			if(strcmp(json->name[i],"id")==0)id=json->valueDouble[i];
			else if(strcmp(json->name[i],"file")==0)strcpy_s(file,json->valueString[i]);
			else if(strcmp(json->name[i],"w")==0)w=json->valueDouble[i];
			else if(strcmp(json->name[i],"h")==0)h=json->valueDouble[i];
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
		for(int i=0 ; i<json->valueNum ; i++){
			if(strcmp(json->name[i],"id")==0)id=json->valueDouble[i];
			else if(strcmp(json->name[i],"file")==0)strcpy_s(file,json->valueString[i]);
		}
		freeSound(sf.sound[id]);
		sf.sound[id]=Mix_LoadWAV(file);
	}
	else if(strcmp(name,"sound")==0){
		int id=0,ch=0,repeat=0;
		for(int i=0 ; i<json->valueNum ; i++){
			if(strcmp(json->name[i],"id")==0)id=json->valueDouble[i];
			else if(strcmp(json->name[i],"ch")==0)ch=json->valueDouble[i];
			else if(strcmp(json->name[i],"repeat")==0)repeat=json->valueDouble[i];
		}
		Mix_PlayChannel(ch, sf.sound[id], repeat);
	}
	else if(strcmp(name,"bgm")==0){
		int repeat=-1;
		char file[100];
		for(int i=0 ; i<json->valueNum ; i++){
			if(strcmp(json->name[i],"file")==0)strcpy_s(file,json->valueString[i]);
			else if(strcmp(json->name[i],"repeat")==0)repeat=json->valueDouble[i];
		}
		if(strcmp(json->cartoonBgmName,str)!=0){
			freeMusic();
			strcpy_s(json->cartoonBgmName,file);
			bgm=Mix_LoadMUS(file);
			Mix_PlayMusic(bgm, repeat);
		}
	}
	else if(strcmp(name,"load-text")==0){
		int which_lang=0;
		char file[100], lang[20];
		file[0]=0;
		for(int i=0 ; i<json->valueNum ; i++){
			if(strcmp(json->name[i],"file")==0)strcpy_s(file,json->valueString[i]);
			else if(strcmp(json->name[i],"lang")==0)strcpy_s(lang,json->valueString[i]);
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
		for(int i=0 ; i<json->valueNum ; i++){
			if(strcmp(json->name[i],"face")==0)face=json->valueDouble[i];
			else if(strcmp(json->name[i],"text")==0)text=json->valueDouble[i];
			else if(strcmp(json->name[i],"shake")==0)shake=json->valueBool[i];
		}
		gd.face_count=face;
		gd.talk_count=text;
		if(shake){
			gd.shake_count=50;
			Mix_PlayChannel(0,sf.decide,0);
		}
	}
}

void readJsonArray(JsonData *json, char *name){
	int mode=JSON_ARRAY_START;

	if(json->talkmode) {
		mode=JSON_NEXTDATA;
	}

	while(json->text[json->pointer]) {
		char *c = &json->text[json->pointer];
		if(*c==' ' || *c==10 || *c==13) {
			json->pointer++;
			continue;
		}
		if(mode==JSON_ARRAY_START) {
			if(*c=='[') {
				readJsonObject(json,name);
				if(json->talkmode){
					break;
				}
				mode=JSON_NEXTDATA;
			}
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				json->pointer++;
				readJsonObject(json,name);
				if(json->talkmode){
					break;
				}
			}
			else if(*c==']') {
				if(strcmp(name,"talk")==0){
					json->talkmode=false;
					json->playtime=0;
				}
				break;
			}
		}
		json->pointer++;
	}
}

void readJsonObject(JsonData *json, char *name){
	int mode=JSON_DATA_START;
	while(json->text[json->pointer]) {
		char *c = &json->text[json->pointer];
		if(*c==' ' || *c==10 || *c==13) {
			json->pointer++;
			continue;
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				json->valueNum=0;
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			json->pointer+=fetchString(c,'"',json->name[json->valueNum])-1;
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(*c=='"'){
				json->pointer+=fetchString(c,'"',json->valueString[json->valueNum])-1;
			}
			else if((*c>='0' && *c<='9') || *c=='-'){
				json->pointer+=fetchDouble(c,&json->valueDouble[json->valueNum])-1;
			}
			else if(*c=='{'){
				char basename[20];
				strcpy_s(basename,json->name[json->valueNum]);
				readJsonSubObject(json,name,basename);
			}
			else if(startsWith(c,"true")){
				json->pointer+=3;
				json->valueBool[json->valueNum]=true;
			}
			else if(startsWith(c,"false")){
				json->pointer+=4;
				json->valueBool[json->valueNum]=false;
			}
			json->valueNum++;
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				applyJsonData(json,name);
				if(strcmp(name,"talk")==0){
					if(!json->skipThisTime)json->talkmode=true;
					gd.text_count=0;
				}
				break;
			}
		}
		json->pointer++;
	}
}

void readJsonSubObject(JsonData *json, char *name1, char *name2){
	int mode=JSON_DATA_START;
	while(json->text[json->pointer]) {
		char *c = &json->text[json->pointer];
		if(*c==' ' || *c==10 || *c==13) {
			json->pointer++;
			continue;
		}
		if(mode==JSON_DATA_START) {
			if(*c=='{') {
				mode=JSON_GETNAME;
			}
		}
		else if(mode==JSON_GETNAME) {
			json->pointer+=fetchString(c,'"',str)-1;
			sprintf_s(json->name[json->valueNum],"%s %s",name2,str);
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(*c=='"'){
				json->pointer+=fetchString(c,'"',json->valueString[json->valueNum])-1;
			}
			else if((*c>='0' && *c<='9') || *c=='-'){
				json->pointer+=fetchDouble(c,&json->valueDouble[json->valueNum])-1;
			}
			else if(*c=='{'){
				char basename[20];
				strcpy_s(basename,json->name[json->valueNum]);
				readJsonSubObject(json,name2,basename);
			}
			else if(startsWith(c,"true")){
				json->pointer+=3;
				json->valueBool[json->valueNum]=true;
			}
			else if(startsWith(c,"false")){
				json->pointer+=4;
				json->valueBool[json->valueNum]=false;
			}
			json->valueNum++;
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				json->valueNum--;
				break;
			}
		}
		json->pointer++;
	}
}

void resetObject(JsonData *json){
	int mode=JSON_ARRAY_START;
	while(json->text[json->pointer]) {
		char *c = &json->text[json->pointer];
		if(*c==' ' || *c==10 || *c==13) {
			json->pointer++;
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
				int n=0;
				json->pointer+=fetchInt(c,&n)-1;
				resetObject(json->obj[n]);
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
		json->pointer++;
	}
}

void _moveObject(JsonData *json){
	for(int i=0 ; i<=json->max_obj ; i++){
		json->obj[i].set.x+=json->obj[i].move.x;
		json->obj[i].set.y+=json->obj[i].move.y;
		json->obj[i].set.ix+=json->obj[i].move.ix;
		json->obj[i].set.iy+=json->obj[i].move.iy;
		json->obj[i].set.w+=json->obj[i].move.w;
		json->obj[i].set.h+=json->obj[i].move.h;
		json->obj[i].set.mag+=json->obj[i].move.mag;
		json->obj[i].set.alpha+=json->obj[i].move.alpha;
		json->obj[i].set.shake+=json->obj[i].move.shake;
		json->obj[i].set.R+=json->obj[i].move.R;
		json->obj[i].set.G+=json->obj[i].move.G;
		json->obj[i].set.B+=json->obj[i].move.B;
		json->obj[i].set.gradRfrom+=json->obj[i].move.gradRfrom;
		json->obj[i].set.gradGfrom+=json->obj[i].move.gradGfrom;
		json->obj[i].set.gradBfrom+=json->obj[i].move.gradBfrom;
		json->obj[i].set.gradRto+=json->obj[i].move.gradRto;
		json->obj[i].set.gradGto+=json->obj[i].move.gradGto;
		json->obj[i].set.gradBto+=json->obj[i].move.gradBto;

		json->obj[i].flip.count++;
		if(json->obj[i].flip.num!=0){
			if(json->obj[i].flip.turnBack){
				if(json->obj[i].flip.count%json->obj[i].flip.interval==0){
					if(json->obj[i].flip.count<json->obj[i].flip.interval*json->obj[i].flip.num){
						json->obj[i].set.ix+=json->obj[i].flip.ix;
						json->obj[i].set.iy+=json->obj[i].flip.iy;
					}else{
						json->obj[i].set.ix-=json->obj[i].flip.ix;
						json->obj[i].set.iy-=json->obj[i].flip.iy;
					}
					if(json->obj[i].flip.count==json->obj[i].flip.interval*(json->obj[i].flip.num-1)*2){
						json->obj[i].flip.count=0;
					}
				}
			}else{
				if(json->obj[i].flip.count%json->obj[i].flip.interval==0){
					json->obj[i].set.ix+=json->obj[i].flip.ix;
					json->obj[i].set.iy+=json->obj[i].flip.iy;
				}
				if(json->obj[i].flip.count==json->obj[i].flip.interval*json->obj[i].flip.num){
					json->obj[i].set.ix-=json->obj[i].flip.ix*json->obj[i].flip.num;
					json->obj[i].set.iy-=json->obj[i].flip.iy*json->obj[i].flip.num;
					json->obj[i].flip.count=0;
				}
			}
		}

		if(json->obj[i].slideAlpha.step!=0){
			if(json->obj[i].slideAlpha.back){
				json->obj[i].slideAlpha.position-=json->obj[i].slideAlpha.step;
			}else{
				json->obj[i].slideAlpha.position+=json->obj[i].slideAlpha.step;
			}
			if(json->obj[i].slideAlpha.position>=json->obj[i].slideAlpha.to){
				if(json->obj[i].slideAlpha.turnBack){
					json->obj[i].slideAlpha.position=json->obj[i].slideAlpha.to;
					json->obj[i].slideAlpha.back=true;
				}else{
					json->obj[i].slideAlpha.position=json->obj[i].slideAlpha.from;
				}
			}
			else if(json->obj[i].slideAlpha.position<=json->obj[i].slideAlpha.from){
				json->obj[i].slideAlpha.position=json->obj[i].slideAlpha.from;
				json->obj[i].slideAlpha.back=false;
			}
		}
		if(json->obj[i].slideX.step!=0){
			if(json->obj[i].slideX.back){
				json->obj[i].slideX.position-=json->obj[i].slideX.step;
			}else{
				json->obj[i].slideX.position+=json->obj[i].slideX.step;
			}
			if(json->obj[i].slideX.position>=json->obj[i].slideX.to){
				if(json->obj[i].slideX.turnBack){
					json->obj[i].slideX.position=json->obj[i].slideX.to;
					json->obj[i].slideX.back=true;
				}else{
					json->obj[i].slideX.position=json->obj[i].slideX.from;
				}
			}
			else if(json->obj[i].slideX.position<=json->obj[i].slideX.from){
				if(json->obj[i].slideX.turnBack){
					json->obj[i].slideX.position=json->obj[i].slideX.from;
					json->obj[i].slideX.back=false;
				}else{
					json->obj[i].slideX.position=json->obj[i].slideX.to;
				}
			}
		}
		if(json->obj[i].slideY.step!=0){
			if(json->obj[i].slideY.back){
				json->obj[i].slideY.position-=json->obj[i].slideY.step;
			}else{
				json->obj[i].slideY.position+=json->obj[i].slideY.step;
			}
			if(json->obj[i].slideY.position>=json->obj[i].slideY.to){
				if(json->obj[i].slideY.turnBack){
					json->obj[i].slideY.position=json->obj[i].slideY.to;
					json->obj[i].slideY.back=true;
				}else{
					json->obj[i].slideY.position=json->obj[i].slideY.from;
				}
				json->obj[i].slideY.back=true;
			}
			else if(json->obj[i].slideY.position<=json->obj[i].slideY.from){
				json->obj[i].slideY.position=json->obj[i].slideY.from;
				json->obj[i].slideY.back=false;
			}
		}
		if(json->obj[i].slideIX.step!=0){
			if(json->obj[i].slideIX.back){
				json->obj[i].slideIX.position-=json->obj[i].slideIX.step;
			}else{
				json->obj[i].slideIX.position+=json->obj[i].slideIX.step;
			}
			if(json->obj[i].slideIX.position>=json->obj[i].slideIX.to){
				if(json->obj[i].slideIX.turnBack){
					json->obj[i].slideIX.position=json->obj[i].slideIX.to;
					json->obj[i].slideIX.back=true;
				}else{
					json->obj[i].slideIX.position=json->obj[i].slideIX.from;
				}
				json->obj[i].slideIX.back=true;
			}
			else if(json->obj[i].slideIX.position<=json->obj[i].slideIX.from){
				json->obj[i].slideIX.position=json->obj[i].slideIX.from;
				json->obj[i].slideIX.back=false;
			}
		}
		if(json->obj[i].slideIY.step!=0){
			if(json->obj[i].slideIY.back){
				json->obj[i].slideIY.position-=json->obj[i].slideIY.step;
			}else{
				json->obj[i].slideIY.position+=json->obj[i].slideIY.step;
			}
			if(json->obj[i].slideIY.position>=json->obj[i].slideIY.to){
				if(json->obj[i].slideIY.turnBack){
					json->obj[i].slideIY.position=json->obj[i].slideIY.to;
					json->obj[i].slideIY.back=true;
				}else{
					json->obj[i].slideIY.position=json->obj[i].slideIY.from;
				}
				json->obj[i].slideIY.back=true;
			}
			else if(json->obj[i].slideIY.position<=json->obj[i].slideIY.from){
				json->obj[i].slideIY.position=json->obj[i].slideIY.from;
				json->obj[i].slideIY.back=false;
			}
		}
		if(json->obj[i].slideMag.step!=0){
			if(json->obj[i].slideMag.back){
				json->obj[i].slideMag.position-=json->obj[i].slideMag.step;
			}else{
				json->obj[i].slideMag.position+=json->obj[i].slideMag.step;
			}
			if(json->obj[i].slideMag.position>=json->obj[i].slideMag.to){
				if(json->obj[i].slideMag.turnBack){
					json->obj[i].slideMag.position=json->obj[i].slideMag.to;
					json->obj[i].slideMag.back=true;
				}else{
					json->obj[i].slideMag.position=json->obj[i].slideMag.from;
				}
			}
			else if(json->obj[i].slideMag.position<=json->obj[i].slideMag.from){
				json->obj[i].slideMag.position=json->obj[i].slideMag.from;
				json->obj[i].slideMag.back=false;
			}
		}
		if(json->obj[i].jumpG!=0){
			json->obj[i].set.y-=json->obj[i].jumpY;
			json->obj[i].jumpY-=json->obj[i].jumpG;
		}
		if(json->obj[i].slideAlpha.fadeRate!=0){
			json->obj[i].slideAlpha.fade+=json->obj[i].slideAlpha.fadeRate;
		}
	}
}

bool _nextCut(JsonData *json){
	_moveObject(json);
	if(!json->talkmode){
		bool end=false;
		if(json->playtime==json->nextTime ||
		   (gd.week==json->call_week && gd.hour==json->call_hour && gd.minute==json->call_minute)){
			end=readCartoon(json);
		}
		json->playtime++;
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

bool nextCut(JsonData *json){
	bool end=false;
	int t=1;
	if(json->cartoonSync){
		t=(int)((SDL_GetTicks()-json->timestamp)/16)-json->playtime;
	}
	for(int i=0 ; i<t ; i++){
		end=_nextCut(json);
		if(end)break;
	}
	return end;
}

bool nextTalk(JsonData *json){
	if(json->talkmode){
		bool end=false;
		if(gd.text_count<(int)strlen(talk[gd.talk_count].str[CHAR_CODE])){
			gd.text_count=90;
		}else{
			end=readCartoon(json);
		}
		if(end)return true;
	}
	return false;
}

void drawAnimationCut(JsonData *json, SDL_Surface* scr){
	for(int i=0 ; i<=json->max_obj ; i++){
		if(json->obj[i].set.lang!=-1 && json->obj[i].set.lang!=CHAR_CODE){
			continue;
		}

		int dx=0,dy=0;
		double fade=1;
		if(json->obj[i].set.shake!=0){
			dx=(int)(cos(count*0.3)*json->obj[i].set.shake);
			dy=(int)(sin(count*0.3)*json->obj[i].set.shake);
		}
		if(json->obj[i].slideAlpha.fadeRate!=0){
			fade=json->obj[i].slideAlpha.fade;
		}

		int x=(int)(json->obj[i].set.x+json->obj[i].slideX.position+dx);
		int y=(int)(json->obj[i].set.y+json->obj[i].slideY.position+dy);
		int ix=(int)(json->obj[i].set.ix+json->obj[i].slideIX.position);
		int iy=(int)(json->obj[i].set.iy+json->obj[i].slideIY.position);
		int w=(int)json->obj[i].set.w;
		int h=(int)json->obj[i].set.h;
		int a=(int)((json->obj[i].set.alpha+json->obj[i].slideAlpha.position)*fade);
		double mag=json->obj[i].set.mag+json->obj[i].slideMag.position;
		if(json->obj[i].set.drawTo!=-1){
			if(json->obj[i].set.type==CARTOON_DRAWIMAGE){
				if(mag==1){
					drawImage(img.bg[json->obj[i].set.drawTo],img.bg[json->obj[i].set.img_id],x,y,ix,iy,w,h,a);
				}else{
					drawImage_x(img.bg[json->obj[i].set.drawTo],img.bg[json->obj[i].set.img_id],x,y,mag,ix,iy,w,h,a);
				}
			}
			if(json->obj[i].set.type==CARTOON_ILLUMINATE){
				if(mag==1){
					illuminateImage(img.bg[json->obj[i].set.drawTo],img.bg[json->obj[i].set.img_id],x,y,ix,iy,w,h,a);
				}else{
					illuminateImage_x(img.bg[json->obj[i].set.drawTo],img.bg[json->obj[i].set.img_id],x,y,mag,ix,iy,w,h,a);
				}
			}
			else if(json->obj[i].set.type==CARTOON_FILLRECT){
				fillRect(img.bg[json->obj[i].set.drawTo],x,y,w,h,(int)json->obj[i].set.R,(int)json->obj[i].set.G,(int)json->obj[i].set.B,a);
			}
			else if(json->obj[i].set.type==CARTOON_CARLIGHT){
				drawLightBall(img.bg[json->obj[i].set.drawTo],x,y,w,(int)json->obj[i].set.R,(int)json->obj[i].set.G,(int)json->obj[i].set.B);
			}
			else if(json->obj[i].set.type==CARTOON_FILLRECT_GRAD){
				for(int j=0 ; j<h ; j++){
					int R=(int)(json->obj[i].set.gradRfrom+(json->obj[i].set.gradRto-json->obj[i].set.gradRfrom)*j/h);
					int G=(int)(json->obj[i].set.gradGfrom+(json->obj[i].set.gradGto-json->obj[i].set.gradGfrom)*j/h);
					int B=(int)(json->obj[i].set.gradBfrom+(json->obj[i].set.gradBto-json->obj[i].set.gradBfrom)*j/h);
					fillRect(img.bg[json->obj[i].set.drawTo],x,y+j,w,1,R,G,B,a);
				}
			}
			else if(json->obj[i].set.type==CARTOON_DRAWIMAGE_WAVE){
				for(int j=0 ; j<h ; j++){
					drawImage(img.bg[json->obj[i].set.drawTo],img.bg[json->obj[i].set.img_id],x+(int)(sin((count+j)*json->obj[i].waveSIN)*json->obj[i].waveRange),y+j,ix,iy+j,w,1,a);
				}
			}
		}else{
			if(json->obj[i].set.type==CARTOON_DRAWIMAGE){
				if(mag==1){
					drawImage(scr,img.bg[json->obj[i].set.img_id],x,y,ix,iy,w,h,a);
				}else{
					drawImage_x(scr,img.bg[json->obj[i].set.img_id],x,y,mag,ix,iy,w,h,a);
				}
			}
			if(json->obj[i].set.type==CARTOON_ILLUMINATE){
				if(mag==1){
					illuminateImage(scr,img.bg[json->obj[i].set.img_id],x,y,ix,iy,w,h,a);
				}else{
					illuminateImage_x(scr,img.bg[json->obj[i].set.img_id],x,y,mag,ix,iy,w,h,a);
				}
			}
			else if(json->obj[i].set.type==CARTOON_FILLRECT){
				fillRect(scr,x,y,w,h,(int)json->obj[i].set.R,(int)json->obj[i].set.G,(int)json->obj[i].set.B,a);
			}
			else if(json->obj[i].set.type==CARTOON_CARLIGHT){
				drawLightBall(scr,x,y,w,(int)json->obj[i].set.R,(int)json->obj[i].set.G,(int)json->obj[i].set.B);
			}
			else if(json->obj[i].set.type==CARTOON_FILLRECT_GRAD){
				for(int j=0 ; j<h ; j++){
					int R=(int)(json->obj[i].set.gradRfrom+(json->obj[i].set.gradRto-json->obj[i].set.gradRfrom)*j/h);
					int G=(int)(json->obj[i].set.gradGfrom+(json->obj[i].set.gradGto-json->obj[i].set.gradGfrom)*j/h);
					int B=(int)(json->obj[i].set.gradBfrom+(json->obj[i].set.gradBto-json->obj[i].set.gradBfrom)*j/h);
					fillRect(scr,x,y+j,w,1,R,G,B,a);
				}
			}
			else if(json->obj[i].set.type==CARTOON_DRAWIMAGE_WAVE){
				for(int j=0 ; j<h ; j++){
					drawImage(scr,img.bg[json->obj[i].set.img_id],x+(int)(sin((count+j)*json->obj[i].waveSIN)*json->obj[i].waveRange),y+j,ix,iy+j,w,1,a);
				}
			}
		}
	}
	if(json->talkmode){
		drawTalking(scr,gd.face_count,talk[gd.talk_count]);
	}
}
