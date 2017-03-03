#include "animation.h"

#define CARTOON_DRAWIMAGE 1
#define CARTOON_FILLRECT 2
#define CARTOON_FILLRECT_GRAD 3
#define CARTOON_DRAWIMAGE_WAVE 4
#define CARTOON_CARLIGHT 5
#define CARTOON_ILLUMINATE 6
#define CARTOON_DIVISION 7
#define JSON_ARRAY_START 1
#define JSON_STARTTIME 2
#define JSON_DATA_START 3
#define JSON_GETNAME 4
#define JSON_COLON 5
#define JSON_GETVALUE 6
#define JSON_NEXTDATA 7
#define JSON_ENDDATA 8

void readCartoon(JsonData *json);
void readCartoonAct(JsonData *json);
void resetObject(JsonData *json);
void resetObject(JsonData *json, CartoonObject &obj);
void readJsonObject(JsonData *json, char *basename, char *name);
void readJsonArray(JsonData *json, char *basename, char *name);
void applyJsonData(JsonData *json);

JsonData cartoonJson,talkingJson,manekitvJson;

void JsonDataReader::free(){
	for(int i=0 ; i<arraySize; i++){
		delete [] name[i];
		delete [] valueString[i];
	}
	if(arraySize!=0){
		delete [] name;
		delete [] valueString;
		delete [] valueDouble;
		delete [] valueBool;
	}
	valueNum=0;
	maxValueNum=0;
	arraySize=0;
}

void JsonDataReader::init(int size){
	free();
	if(size<=0)return;
	name=new char*[size];
	valueString=new char*[size];
	for(int i=0 ; i<size ; i++){
		name[i]=new char[100];
		valueString[i]=new char[1000];
	}
	valueDouble=new double[size];
	valueBool=new bool[size];
	arraySize=size;
}

void resetObjectSetting(ObjectSetting &s){
	s.x=0;s.y=0;s.ix=0;s.iy=0;s.w=0;s.h=0;
	s.mag=1;s.alpha=255;s.shake=0;s.type=0;s.drawTo=-1;s.lang=-1;
	s.R=0;s.G=0;s.B=0;s.img_id=0;
	s.gradRfrom=0;s.gradGfrom=0;s.gradBfrom=0;
	s.gradRto=0;s.gradGto=0;s.gradBto=0;
	s.fixed=false;
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
	s+=fetchInt(s,&json->call_hour);
	while(true){
		if(*s>='0' && *s<='9')break;
		s++;
	}
	s+=fetchInt(s,&json->call_minute);
}

bool startsWith(char *s, const char *target) {
	for(size_t i=0 ; i<strlen(target) ; i++) {
		if(s[i]!=target[i]) {
			return false;
		}
	}
	return true;
}

void resetCartoon(JsonData *json){
	json->playtime=0;
	json->pointer=0;
	json->nextTime=0;
	json->talkmode=false;
	json->cartoonSync=false;
	json->end=false;
	json->scrX=0;json->scrY=0;
	json->call_week=-1;json->call_hour=-1;json->call_minute=-1;
	json->jr.set.valueNum=0;
	json->jr.move.valueNum=0;
	json->jr.talk.valueNum=0;
	json->jr.image.valueNum=0;
	json->jr.bgm.valueNum=0;
	json->jr.loadSound.valueNum=0;
	json->jr.sound.valueNum=0;
}

void freeCartoon(JsonData *json){
	resetCartoon(json);
	json->initializedObjArray=false;
	json->initializedReaderArray=false;
	json->toPlayMusic=false;
	json->playingMusic=false;
	json->musicRepeat=-1;
	json->cartoonBgmName[0]=0;
	json->talk_open_count=0;
	json->text_count=0;
	json->shake_count=0;
	if(json->size){
		delete [] json->jsonText;
		json->size=0;
	}
	if(json->max_obj){
		delete [] json->obj;
		json->max_obj=0;
	}
	if(json->max_image){
		for(int i=0 ; i<=json->max_image ; i++){
			freeImage(json->bg[i]);
		}
		delete [] json->bg;
		json->max_image=0;
	}
	if(json->max_sound){
		for(int i=0 ; i<=json->max_sound ; i++){
			freeSound(json->sound[i]);
		}
		delete [] json->sound;
		json->max_sound=0;
	}
	if(json->talk_size){
		json->talk_size=0;
	}
	if(json->jr.resetNum){
		delete [] json->jr.reset;
		json->jr.resetNum=0;
	}
	json->jr.set.free();
	json->jr.move.free();
	json->jr.talk.free();
	json->jr.image.free();
	json->jr.bgm.free();
	json->jr.loadSound.free();
	json->jr.sound.free();
}

void loadCartoon(JsonData *json, const char *filename){
	freeCartoon(json);
	loadFile(filename);
	json->size=fsize;
	if(fsize){
		json->jsonText=new char[json->size];
		strcpy_s(json->jsonText,fstr);

		readCartoon(json);
		resetCartoon(json);
		json->jr.set.init(json->jr.set.maxValueNum);
		json->jr.move.init(json->jr.move.maxValueNum);
		json->jr.talk.init(json->jr.talk.maxValueNum);
		json->jr.image.init(json->jr.image.maxValueNum);
		json->jr.bgm.init(json->jr.bgm.maxValueNum);
		json->jr.loadSound.init(json->jr.loadSound.maxValueNum);
		json->jr.sound.init(json->jr.sound.maxValueNum);
		json->initializedReaderArray=true;

		readCartoon(json);
		resetCartoon(json);
		json->jr.reset=new int[json->max_obj+1];
		json->obj=new CartoonObject[json->max_obj+1];
		for(int i=0 ; i<=json->max_obj ; i++){
			resetObject(json->obj[i]);
		}
		json->bg=new Image*[json->max_image+1];
		for(int i=0 ; i<=json->max_image ; i++){
			json->bg[i]=NULL;
		}
		json->sound=new Mix_Chunk*[json->max_sound+1];
		for(int i=0 ; i<=json->max_sound ; i++){
			json->sound[i]=NULL;
		}
		json->initializedObjArray=true;

		resetCartoon(json);
		nextCut(json);
	}
}

void readCartoon(JsonData *json, int index){
	resetCartoon(json);
	json->targetIndex=index;
	json->searchingIndex=true;
	readCartoon(json);
	json->searchingIndex=false;
}

void readCartoon(JsonData *json){
	int mode=JSON_ARRAY_START;
	char dataname[20];
	bool waitForNextTime=false;

	if(json->pointer!=0) {
		mode=JSON_NEXTDATA;
	}

	while(json->pointer < json->size) {
		char *c = &json->jsonText[json->pointer];
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
				if(!json->searchingIndex) {
					if(json->nextTime!=json->playtime || json->talkmode || json->end) {
						waitForNextTime=true;
					}
				}
			}
			else if(strcmp(dataname, "date")==0) {
				json->pointer+=fetchString(c,'"',str)-1;
				parseDate(json,str);
				if(json->call_week>gd.week
				|| (json->call_week==gd.week && json->call_hour>gd.hour)
				|| (json->call_week==gd.week && json->call_hour==gd.hour && json->call_minute>gd.minute)) {
					waitForNextTime=true;
				}
			}
			else if(strcmp(dataname, "index")==0) {
				int n=0;
				json->pointer+=fetchInt(c,&n)-1;
				if(json->searchingIndex && json->targetIndex==n) {
					json->searchingIndex=false;
				}
				else if(!json->searchingIndex && json->initializedObjArray) {
					break;
				}
			}
			else if(strcmp(dataname, "act")==0) {
				int pre_talk_size=json->jr.talk.valueNum;
				json->jr.image.valueNum=0;
				json->jr.loadSound.valueNum=0;
				json->jr.bgm.valueNum=0;
				json->jr.set.valueNum=0;
				json->jr.move.valueNum=0;
				json->jr.sound.valueNum=0;
				json->jr.talk.valueNum=0;
				json->jr.resetNum=0;
				json->jr.hasSync=false;
				json->jr.hasResetTimer=false;
				json->jr.talkPointer=0;
				json->jr.end=false;
				readJsonObject(json, NULL, NULL);
				if(json->initializedObjArray){
					if(pre_talk_size<=1 && json->jr.talk.valueNum>1){
						json->talk_open_count=20;
					}
					else if(pre_talk_size>1 && json->jr.talk.valueNum<=1){
						json->talk_open_count=-20;
					}
				}
				if(json->jr.image.maxValueNum < json->jr.image.valueNum) {
					json->jr.image.maxValueNum=json->jr.image.valueNum;
				}
				if(json->jr.loadSound.maxValueNum < json->jr.loadSound.valueNum) {
					json->jr.loadSound.maxValueNum=json->jr.loadSound.valueNum;
				}
				if(json->jr.bgm.maxValueNum < json->jr.bgm.valueNum) {
					json->jr.bgm.maxValueNum=json->jr.bgm.valueNum;
				}
				if(json->jr.set.maxValueNum < json->jr.set.valueNum) {
					json->jr.set.maxValueNum=json->jr.set.valueNum;
				}
				if(json->jr.move.maxValueNum < json->jr.move.valueNum) {
					json->jr.move.maxValueNum=json->jr.move.valueNum;
				}
				if(json->jr.sound.maxValueNum < json->jr.sound.valueNum) {
					json->jr.sound.maxValueNum=json->jr.sound.valueNum;
				}
				if(json->jr.talk.maxValueNum < json->jr.talk.valueNum) {
					json->jr.talk.maxValueNum=json->jr.talk.valueNum;
				}
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				if(waitForNextTime && json->initializedObjArray){
					break;
				} else {
					mode=JSON_GETNAME;
				}
			}
			else if(*c=='}') {
				if(json->initializedReaderArray && (!json->initializedObjArray || !json->searchingIndex)){
					applyJsonData(json);
				}
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
}

void fetchNextTalking(JsonData *json){
	int face=0,shake=0;
	for(int i=json->jr.talkPointer ; i<json->jr.talk.valueNum ; i++){
		if(json->jr.talk.name[i][0]==';')break;
		if(strcmp(json->jr.talk.name[i],"face")==0)face=json->jr.talk.valueDouble[i];
		else if(strcmp(json->jr.talk.name[i],"text jp")==0)strcpy_s(json->talk.str[0],json->jr.talk.valueString[i]);
		else if(strcmp(json->jr.talk.name[i],"text en")==0)strcpy_s(json->talk.str[1],json->jr.talk.valueString[i]);
		else if(strcmp(json->jr.talk.name[i],"shake")==0)shake=json->jr.talk.valueBool[i];
		json->jr.talkPointer++;
	}
	UTF8toSJIS(json->talk.str[0]);
	UTF8toLatin(json->talk.str[1]);
	padSpace(json->talk, 30);
	for(size_t i=strlen(json->talk.str[0]) ; i<200 ; i++){
		json->talk.str[0][i]=0;
	}
	for(size_t i=strlen(json->talk.str[1]) ; i<200 ; i++){
		json->talk.str[1][i]=0;
	}
	json->jr.talkPointer++;
	json->face_count=face;
	json->text_count=0;
	if(shake){
		json->shake_count=50;
	}
}

void applyJsonData(JsonData *json){
	if(json->initializedObjArray){
		for(int i=0 ; i<json->jr.resetNum ; i++){
			resetObject(json->obj[json->jr.reset[i]]);
		}
	}
	SDL_Color col;
	int id=0, num=0;
	for(int i=0 ; i<json->jr.set.valueNum ; i++){
		num=0;
		for(int j=i ; j<json->jr.set.valueNum ; j++){
			if(json->jr.set.name[j][0]==';')break;
			if(strcmp(json->jr.set.name[j],"id")==0)id=json->jr.set.valueDouble[j];
			num++;
		}
		if(json->max_obj<id) {
			json->max_obj=id;
		}
		if(!json->initializedObjArray){
			i+=num;
			continue;
		}
		for(int j=i ; j<i+num ; j++){
			if(strcmp(json->jr.set.name[j],"img_id")==0)json->obj[id].set.img_id=json->jr.set.valueDouble[j];
			else if(strcmp(json->jr.set.name[j],"x")==0)json->obj[id].set.x=json->jr.set.valueDouble[j];
			else if(strcmp(json->jr.set.name[j],"y")==0)json->obj[id].set.y=json->jr.set.valueDouble[j];
			else if(strcmp(json->jr.set.name[j],"ix")==0){
				json->obj[id].set.ix=json->jr.set.valueDouble[j];
				json->obj[id].set.type=CARTOON_DRAWIMAGE;
			}
			else if(strcmp(json->jr.set.name[j],"iy")==0){
				json->obj[id].set.iy=json->jr.set.valueDouble[j];
				json->obj[id].set.type=CARTOON_DRAWIMAGE;
			}
			else if(strcmp(json->jr.set.name[j],"w")==0)json->obj[id].set.w=json->jr.set.valueDouble[j];
			else if(strcmp(json->jr.set.name[j],"h")==0)json->obj[id].set.h=json->jr.set.valueDouble[j];
			else if(strcmp(json->jr.set.name[j],"a")==0)json->obj[id].set.alpha=json->jr.set.valueDouble[j];
			else if(strcmp(json->jr.set.name[j],"mag")==0)json->obj[id].set.mag=json->jr.set.valueDouble[j];
			else if(strcmp(json->jr.set.name[j],"col")==0){
				parseColor(json->jr.set.valueString[j],&col);
				json->obj[id].set.R=col.r;
				json->obj[id].set.G=col.g;
				json->obj[id].set.B=col.b;
				json->obj[id].set.type=CARTOON_FILLRECT;
			}
			else if(strcmp(json->jr.set.name[j],"col_grad_y from")==0){
				parseColor(json->jr.set.valueString[j],&col);
				json->obj[id].set.gradRfrom=col.r;
				json->obj[id].set.gradGfrom=col.g;
				json->obj[id].set.gradBfrom=col.b;
				json->obj[id].set.type=CARTOON_FILLRECT_GRAD;
			}
			else if(strcmp(json->jr.set.name[j],"col_grad_y to")==0){
				parseColor(json->jr.set.valueString[j],&col);
				json->obj[id].set.gradRto=col.r;
				json->obj[id].set.gradGto=col.g;
				json->obj[id].set.gradBto=col.b;
				json->obj[id].set.type=CARTOON_FILLRECT_GRAD;
			}
			else if(strcmp(json->jr.set.name[j],"shake")==0)json->obj[id].set.shake=json->jr.set.valueDouble[j];
			else if(strcmp(json->jr.set.name[j],"draw_to")==0)json->obj[id].set.drawTo=json->jr.set.valueDouble[j];
			else if(strcmp(json->jr.set.name[j],"type")==0){
				if(strcmp(json->jr.set.valueString[j],"carlight")==0)json->obj[id].set.type=CARTOON_CARLIGHT;
				else if(strcmp(json->jr.set.valueString[j],"illuminate")==0)json->obj[id].set.type=CARTOON_ILLUMINATE;
				else if(strcmp(json->jr.set.valueString[j],"division")==0)json->obj[id].set.type=CARTOON_DIVISION;
			}
			else if(strcmp(json->jr.set.name[j],"lang")==0){
				if(strcmp(json->jr.set.valueString[j],"jp")==0)json->obj[id].set.lang=JAPANESE;
				else if(strcmp(json->jr.set.valueString[j],"en")==0)json->obj[id].set.lang=EUROPEAN;
			}
			else if(strcmp(json->jr.set.name[j],"wave sin")==0){
				json->obj[id].waveSIN=json->jr.set.valueDouble[j];
				json->obj[id].set.type=CARTOON_DRAWIMAGE_WAVE;
			}
			else if(strcmp(json->jr.set.name[j],"wave range")==0){
				json->obj[id].waveRange=json->jr.set.valueDouble[j];
				json->obj[id].set.type=CARTOON_DRAWIMAGE_WAVE;
			}
			else if(strcmp(json->jr.set.name[j],"fixed")==0){
				json->obj[id].set.fixed=json->jr.set.valueBool[j];
			}
		}
		i+=num;
	}

	id=0;num=0;
	for(int i=0 ; i<json->jr.move.valueNum ; i++){
		if(!json->initializedObjArray){
			break;
		}
		num=0;
		for(int j=i ; j<json->jr.move.valueNum ; j++){
			if(json->jr.move.name[j][0]==';')break;
			if(strcmp(json->jr.move.name[j],"id")==0)id=json->jr.move.valueDouble[j];
			num++;
		}
		for(int j=i ; j<i+num ; j++){
			if(strcmp(json->jr.move.name[j],"x")==0)json->obj[id].move.x=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"y")==0)json->obj[id].move.y=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"mag")==0)json->obj[id].move.mag=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"a")==0)json->obj[id].move.alpha=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide x step")==0){
				json->obj[id].slideX.step=json->jr.move.valueDouble[j];
				if(json->obj[id].slideX.step<0){
					json->obj[id].slideX.step*=-1;
					json->obj[id].slideX.back=true;
				}else{
					json->obj[id].slideX.back=false;
				}
			}
			else if(strcmp(json->jr.move.name[j],"slide x from")==0)json->obj[id].slideX.from=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide x to")==0)json->obj[id].slideX.to=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide x pos")==0)json->obj[id].slideX.position=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide x turnback")==0)json->obj[id].slideX.turnBack=json->jr.move.valueBool[j];
			else if(strcmp(json->jr.move.name[j],"slide y step")==0){
				json->obj[id].slideY.step=json->jr.move.valueDouble[j];
				if(json->obj[id].slideY.step<0){
					json->obj[id].slideY.step*=-1;
					json->obj[id].slideY.back=true;
				}else{
					json->obj[id].slideY.back=false;
				}
			}
			else if(strcmp(json->jr.move.name[j],"slide y from")==0)json->obj[id].slideY.from=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide y to")==0)json->obj[id].slideY.to=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide y pos")==0)json->obj[id].slideY.position=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide y turnback")==0)json->obj[id].slideY.turnBack=json->jr.move.valueBool[j];
			else if(strcmp(json->jr.move.name[j],"slide ix step")==0){
				json->obj[id].slideIX.step=json->jr.move.valueDouble[j];
				if(json->obj[id].slideIX.step<0){
					json->obj[id].slideIX.step*=-1;
					json->obj[id].slideIX.back=true;
				}else{
					json->obj[id].slideIX.back=false;
				}
			}
			else if(strcmp(json->jr.move.name[j],"slide ix from")==0)json->obj[id].slideIX.from=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide ix to")==0)json->obj[id].slideIX.to=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide ix pos")==0)json->obj[id].slideIX.position=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide ix turnback")==0)json->obj[id].slideIX.turnBack=json->jr.move.valueBool[j];
			else if(strcmp(json->jr.move.name[j],"slide iy step")==0){
				json->obj[id].slideIY.step=json->jr.move.valueDouble[j];
				if(json->obj[id].slideIY.step<0){
					json->obj[id].slideIY.step*=-1;
					json->obj[id].slideIY.back=true;
				}else{
					json->obj[id].slideIY.back=false;
				}
			}
			else if(strcmp(json->jr.move.name[j],"slide iy from")==0)json->obj[id].slideIY.from=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide iy to")==0)json->obj[id].slideIY.to=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide iy pos")==0)json->obj[id].slideIY.position=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide iy turnback")==0)json->obj[id].slideIY.turnBack=json->jr.move.valueBool[j];
			else if(strcmp(json->jr.move.name[j],"slide a step")==0){
				json->obj[id].slideAlpha.step=json->jr.move.valueDouble[j];
				if(json->obj[id].slideAlpha.step<0){
					json->obj[id].slideAlpha.step*=-1;
					json->obj[id].slideAlpha.back=true;
				}else{
					json->obj[id].slideAlpha.back=false;
				}
			}
			else if(strcmp(json->jr.move.name[j],"slide a from")==0)json->obj[id].slideAlpha.from=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide a to")==0)json->obj[id].slideAlpha.to=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide a pos")==0)json->obj[id].slideAlpha.position=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide a turnback")==0)json->obj[id].slideAlpha.turnBack=json->jr.move.valueBool[j];
			else if(strcmp(json->jr.move.name[j],"slide mag step")==0){
				json->obj[id].slideMag.step=json->jr.move.valueDouble[j];
				if(json->obj[id].slideMag.step<0){
					json->obj[id].slideMag.step*=-1;
					json->obj[id].slideMag.back=true;
				}else{
					json->obj[id].slideMag.back=false;
				}
			}
			else if(strcmp(json->jr.move.name[j],"slide mag from")==0)json->obj[id].slideMag.from=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide mag to")==0)json->obj[id].slideMag.to=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide mag pos")==0)json->obj[id].slideMag.position=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"slide mag turnback")==0)json->obj[id].slideMag.turnBack=json->jr.move.valueBool[j];
			else if(strcmp(json->jr.move.name[j],"jump g")==0)json->obj[id].jumpG=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"jump y")==0)json->obj[id].jumpY=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"ix")==0)json->obj[id].move.ix=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"iy")==0)json->obj[id].move.iy=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"w")==0)json->obj[id].move.w=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"h")==0)json->obj[id].move.h=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"shake")==0)json->obj[id].move.shake=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"flip in")==0){
				json->obj[id].flip.interval=json->jr.move.valueDouble[j];
				json->obj[id].flip.count=0;
			}
			else if(strcmp(json->jr.move.name[j],"flip ix")==0){
				json->obj[id].flip.ix=json->jr.move.valueDouble[j];
				json->obj[id].flip.count=0;
			}
			else if(strcmp(json->jr.move.name[j],"flip iy")==0){
				json->obj[id].flip.iy=json->jr.move.valueDouble[j];
				json->obj[id].flip.count=0;
			}
			else if(strcmp(json->jr.move.name[j],"flip n")==0){
				json->obj[id].flip.num=json->jr.move.valueDouble[j];
				json->obj[id].flip.count=0;
			}
			else if(strcmp(json->jr.move.name[j],"flip turnback")==0){
				json->obj[id].flip.turnBack=json->jr.move.valueDouble[j];
				json->obj[id].flip.count=0;
			}
			else if(strcmp(json->jr.move.name[j],"col_grad_y from col_r")==0)json->obj[id].move.gradRfrom=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"col_grad_y from col_g")==0)json->obj[id].move.gradGfrom=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"col_grad_y from col_b")==0)json->obj[id].move.gradBfrom=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"col_grad_y to col_r")==0)json->obj[id].move.gradRto=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"col_grad_y to col_g")==0)json->obj[id].move.gradGto=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"col_grad_y to col_b")==0)json->obj[id].move.gradBto=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"col_r")==0)json->obj[id].move.R=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"col_g")==0)json->obj[id].move.G=json->jr.move.valueDouble[j];
			else if(strcmp(json->jr.move.name[j],"col_b")==0)json->obj[id].move.B=json->jr.move.valueDouble[j];
		}
		for(int j=i ; j<i+num ; j++){
			if(strcmp(json->jr.move.name[j],"slide x fade")==0){
				if(json->obj[id].slideX.to!=json->obj[id].slideX.from){
					json->obj[id].slideX.fadeRate=json->jr.move.valueDouble[j]/(json->obj[id].slideX.to-json->obj[id].slideX.from);
				}
			}
			else if(strcmp(json->jr.move.name[j],"slide y fade")==0){
				if(json->obj[id].slideY.to!=json->obj[id].slideY.from){
					json->obj[id].slideY.fadeRate=json->jr.move.valueDouble[j]/(json->obj[id].slideY.to-json->obj[id].slideY.from);
				}
			}
			else if(strcmp(json->jr.move.name[j],"slide ix fade")==0){
				if(json->obj[id].slideIX.to!=json->obj[id].slideIX.from){
					json->obj[id].slideIX.fadeRate=json->jr.move.valueDouble[j]/(json->obj[id].slideIX.to-json->obj[id].slideIX.from);
				}
			}
			else if(strcmp(json->jr.move.name[j],"slide iy fade")==0){
				if(json->obj[id].slideIY.to!=json->obj[id].slideIY.from){
					json->obj[id].slideIY.fadeRate=json->jr.move.valueDouble[j]/(json->obj[id].slideIY.to-json->obj[id].slideIY.from);
				}
			}
			else if(strcmp(json->jr.move.name[j],"slide a fade")==0){
				if(json->obj[id].slideAlpha.to!=json->obj[id].slideAlpha.from){
					json->obj[id].slideAlpha.fadeRate=json->jr.move.valueDouble[j]/(json->obj[id].slideAlpha.to-json->obj[id].slideAlpha.from);
				}
			}
			else if(strcmp(json->jr.move.name[j],"slide mag fade")==0){
				if(json->obj[id].slideMag.to!=json->obj[id].slideMag.from){
					json->obj[id].slideMag.fadeRate=json->jr.move.valueDouble[j]/(json->obj[id].slideMag.to-json->obj[id].slideMag.from);
				}
			}
		}
		i+=num;
	}

	id=0;
	int w=0,h=0;
	char file[100];
	file[0]=0;
	for(int i=0 ; i<json->jr.image.valueNum ; i++){
		for(int j=i ; j<json->jr.image.valueNum ; j++){
			if(json->jr.image.name[j][0]==';')break;
			if(strcmp(json->jr.image.name[j],"id")==0)id=json->jr.image.valueDouble[j];
			else if(strcmp(json->jr.image.name[j],"file")==0)strcpy_s(file,json->jr.image.valueString[j]);
			else if(strcmp(json->jr.image.name[j],"w")==0)w=json->jr.image.valueDouble[j];
			else if(strcmp(json->jr.image.name[j],"h")==0)h=json->jr.image.valueDouble[j];
			i++;
		}
		if(json->max_image<id) {
			json->max_image=id;
		}
		if(!json->initializedObjArray){
			continue;
		}
		freeImage(json->bg[id]);
		if(file[0]){
			getImage(json->bg[id],file,BLUE);
		}
		if(w!=0 && h!=0){
			json->bg[id]=new Image(w,h);
		}
	}

	id=0;
	file[0]=0;
	for(int i=0 ; i<json->jr.loadSound.valueNum ; i++){
		for(int j=i ; j<json->jr.loadSound.valueNum ; j++){
			if(json->jr.loadSound.name[j][0]==';')break;
			if(strcmp(json->jr.loadSound.name[j],"id")==0)id=json->jr.loadSound.valueDouble[j];
			else if(strcmp(json->jr.loadSound.name[j],"file")==0)strcpy_s(file,json->jr.loadSound.valueString[j]);
			i++;
		}
		if(json->max_sound<id) {
			json->max_sound=id;
		}
		if(!json->initializedObjArray){
			continue;
		}
		freeSound(json->sound[id]);
		json->sound[id]=Mix_LoadWAV(file);
	}

	id=0;
	int ch=0,repeat=0;
	for(int i=0 ; i<json->jr.sound.valueNum ; i++){
		if(!json->initializedObjArray){
			break;
		}
		for(int j=i ; j<json->jr.sound.valueNum ; j++){
			if(json->jr.sound.name[j][0]==';')break;
			if(strcmp(json->jr.sound.name[j],"id")==0)id=json->jr.sound.valueDouble[j];
			else if(strcmp(json->jr.sound.name[j],"ch")==0)ch=json->jr.sound.valueDouble[j];
			else if(strcmp(json->jr.sound.name[j],"repeat")==0)repeat=json->jr.sound.valueDouble[j];
			i++;
		}
		Mix_PlayChannel(ch, json->sound[id], repeat);
	}

	int fadeout=-1;
	json->musicRepeat=-1;
	file[0]=0;
	for(int i=0 ; i<json->jr.bgm.valueNum ; i++){
		if(!json->initializedObjArray){
			break;
		}
		for(int j=i ; j<json->jr.bgm.valueNum ; j++){
			if(json->jr.bgm.name[j][0]==';')break;
			if(strcmp(json->jr.bgm.name[j],"file")==0)strcpy_s(file,json->jr.bgm.valueString[j]);
			else if(strcmp(json->jr.bgm.name[j],"repeat")==0)json->musicRepeat=json->jr.bgm.valueDouble[j];
			else if(strcmp(json->jr.bgm.name[j],"fadeout")==0)fadeout=json->jr.bgm.valueDouble[j];
			i++;
		}
		if(fadeout==0){
			Mix_HaltMusic();
			json->playingMusic=false;
			json->toPlayMusic=false;
		}
		else if(fadeout!=-1){
			Mix_FadeOutMusic(fadeout);
			json->playingMusic=false;
			json->toPlayMusic=false;
		}
		if(strcmp(json->cartoonBgmName,file)!=0){
			freeMusic();
			strcpy_s(json->cartoonBgmName,file);
			bgm=Mix_LoadMUS(file);
			json->playingMusic=false;
			json->toPlayMusic=true;
		}
	}

	if(json->jr.talk.valueNum && json->initializedObjArray){
		json->talkmode=true;
		fetchNextTalking(json);
	}

	if(json->jr.hasResetTimer && json->initializedObjArray){
		json->playtime=json->jr.resetTimer;
	}

	if(json->jr.hasSync && json->initializedObjArray){
		if(json->jr.sync){
			json->timestamp=SDL_GetTicks();
		}
		json->cartoonSync=json->jr.sync;
	}

	json->end=json->jr.end;
}

void changeJsonReaderPointer(JsonData *json, char *basename){
	if(strcmp(basename,"image")==0)json->jr.which=&json->jr.image;
	else if(strcmp(basename,"load-sound")==0)json->jr.which=&json->jr.loadSound;
	else if(strcmp(basename,"bgm")==0)json->jr.which=&json->jr.bgm;
	else if(strcmp(basename,"sound")==0)json->jr.which=&json->jr.sound;
	else if(strcmp(basename,"set")==0)json->jr.which=&json->jr.set;
	else if(strcmp(basename,"move")==0)json->jr.which=&json->jr.move;
	else if(strcmp(basename,"talk")==0)json->jr.which=&json->jr.talk;
}

void readJsonArray(JsonData *json, char *basename, char *name){
	int mode=JSON_ARRAY_START;

	while(json->jsonText[json->pointer]) {
		char *c = &json->jsonText[json->pointer];
		if(*c==' ' || *c==10 || *c==13) {
			json->pointer++;
			continue;
		}
		if(mode==JSON_ARRAY_START) {
			if(*c=='[') {
				if(basename!=NULL){
					changeJsonReaderPointer(json,basename);
				}
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(*c=='{') {
				readJsonObject(json,basename,NULL);
				mode=JSON_NEXTDATA;
			}
			else if(*c==']') {
				if(basename!=NULL && name==NULL) {
					if(json->initializedReaderArray){
						sprintf_s(json->jr.which->name[json->jr.which->valueNum],";");
					}
					json->jr.which->valueNum++;
				}
				break;
			}
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETVALUE;
			}
			else if(*c==']') {
				break;
			}
		}
		json->pointer++;
	}
}

void readJsonObject(JsonData *json, char *basename, char *name){
	int mode=JSON_DATA_START;
	while(json->jsonText[json->pointer]) {
		char *c = &json->jsonText[json->pointer];
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
			if(basename==NULL){
				strcpy_s(json->jr.basename,str);
			}
			else if(name==NULL && json->initializedReaderArray){
				strcpy_s(json->jr.which->name[json->jr.which->valueNum],str);
			}
			else if(json->initializedReaderArray){
				sprintf_s(json->jr.which->name[json->jr.which->valueNum],"%s %s",name,str);
			}
			mode=JSON_COLON;
		}
		else if(mode==JSON_COLON) {
			if(*c==':') {
				mode=JSON_GETVALUE;
			}
		}
		else if(mode==JSON_GETVALUE) {
			if(strcmp(json->jr.basename,"end")==0){
				json->jr.end=true;
			}
			else if(strcmp(json->jr.basename,"reset")==0){
				json->jr.resetNum=0;
				resetObject(json);
			}
			else if(strcmp(json->jr.basename,"reset-timer")==0){
				json->pointer+=fetchInt(c,&json->jr.resetTimer)-1;
				json->jr.hasResetTimer=true;
			}
			else if(strcmp(json->jr.basename,"sync")==0){
				if(startsWith(c,"true")){
					json->pointer+=3;
					json->jr.sync=true;
				}
				else if(startsWith(c,"false")){
					json->pointer+=4;
					json->jr.sync=false;
				}
				json->jr.hasSync=true;
			}
			else if(*c=='"'){
				json->pointer+=fetchString(c,'"',str)-1;
				if(json->initializedReaderArray){
					strcpy_s(json->jr.which->valueString[json->jr.which->valueNum],str);
				}
				json->jr.which->valueNum++;
			}
			else if((*c>='0' && *c<='9') || *c=='-'){
				double n=0;
				json->pointer+=fetchDouble(c,&n)-1;
				if(json->initializedReaderArray){
					json->jr.which->valueDouble[json->jr.which->valueNum]=n;
				}
				json->jr.which->valueNum++;
			}
			else if(*c=='['){
				readJsonArray(json,json->jr.basename,name);
			}
			else if(*c=='{'){
				if(basename==NULL){
					changeJsonReaderPointer(json,json->jr.basename);
					mode=JSON_GETNAME;
					readJsonObject(json,json->jr.basename,NULL);
				}else{
					char name2[100];
					if(json->initializedReaderArray){
						strcpy_s(name2,json->jr.which->name[json->jr.which->valueNum]);
					}
					readJsonObject(json,json->jr.basename,name2);
				}
			}
			else if(startsWith(c,"true")){
				json->pointer+=3;
				if(json->initializedReaderArray){
					json->jr.which->valueBool[json->jr.which->valueNum]=true;
				}
				json->jr.which->valueNum++;
			}
			else if(startsWith(c,"false")){
				json->pointer+=4;
				if(json->initializedReaderArray){
					json->jr.which->valueBool[json->jr.which->valueNum]=false;
				}
				json->jr.which->valueNum++;
			}
			mode=JSON_NEXTDATA;
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETNAME;
			}
			else if(*c=='}') {
				if(basename!=NULL && name==NULL) {
					if(json->initializedReaderArray){
						sprintf_s(json->jr.which->name[json->jr.which->valueNum],";");
					}
					json->jr.which->valueNum++;
				}
				break;
			}
		}
		json->pointer++;
	}
}

void resetObject(JsonData *json){
	int mode=JSON_ARRAY_START;
	while(json->jsonText[json->pointer]) {
		char *c = &json->jsonText[json->pointer];
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
				if(json->initializedObjArray){
					json->jr.reset[json->jr.resetNum]=n;
				}
				json->jr.resetNum++;
				mode=JSON_NEXTDATA;
			}
		}
		else if(mode==JSON_NEXTDATA) {
			if(*c==',') {
				mode=JSON_GETVALUE;
			}
			else if(*c==']') {
				break;
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
				if(json->obj[i].slideAlpha.turnBack){
					json->obj[i].slideAlpha.position=json->obj[i].slideAlpha.from;
					json->obj[i].slideAlpha.back=false;
				}else{
					json->obj[i].slideAlpha.position=json->obj[i].slideAlpha.to;
				}
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
			}
			else if(json->obj[i].slideY.position<=json->obj[i].slideY.from){
				if(json->obj[i].slideY.turnBack){
					json->obj[i].slideY.position=json->obj[i].slideY.from;
					json->obj[i].slideY.back=false;
				}else{
					json->obj[i].slideY.position=json->obj[i].slideY.to;
				}
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
			}
			else if(json->obj[i].slideIX.position<=json->obj[i].slideIX.from){
				if(json->obj[i].slideIX.turnBack){
					json->obj[i].slideIX.position=json->obj[i].slideIX.from;
					json->obj[i].slideIX.back=false;
				}else{
					json->obj[i].slideIX.position=json->obj[i].slideIX.to;
				}
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
			}
			else if(json->obj[i].slideIY.position<=json->obj[i].slideIY.from){
				if(json->obj[i].slideIY.turnBack){
					json->obj[i].slideIY.position=json->obj[i].slideIY.from;
					json->obj[i].slideIY.back=false;
				}else{
					json->obj[i].slideIY.position=json->obj[i].slideIY.to;
				}
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
				if(json->obj[i].slideMag.turnBack){
					json->obj[i].slideMag.position=json->obj[i].slideMag.from;
					json->obj[i].slideMag.back=false;
				}else{
					json->obj[i].slideMag.position=json->obj[i].slideMag.to;
				}
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

void controlTextCount(JsonData *json){
	if(json->talk_open_count>0){
		json->talk_open_count--;
	}
	else if(json->talk_open_count<0){
			json->talk_open_count++;
	}else{
		while(json->talk.str[CHAR_CODE][json->text_count]==' ' && json->talk.str[CHAR_CODE][json->text_count+1]==' '){
			json->text_count++;
		}
		json->text_count++;
		if(json->shake_count>0){
			if(json->shake_count==50){
				Mix_PlayChannel(0,sf.decide,0);
			}
			json->shake_count--;
		}
	}
}

void _nextCut(JsonData *json){
	_moveObject(json);
	if(!json->talkmode && !json->end){
		if(json->playtime==json->nextTime ||
		   (gd.week==json->call_week && gd.hour==json->call_hour && gd.minute==json->call_minute)){
			readCartoon(json);
		}
		json->playtime++;
		if(json->talk_open_count<0)json->talk_open_count++;
	}else{
		controlTextCount(json);
	}
	if(json->toPlayMusic && kick_count==0){
		Mix_PlayMusic(bgm, json->musicRepeat);
		json->playingMusic=true;
		json->toPlayMusic=false;
	}
}

bool nextCut(JsonData *json){
	int t=1;
	if(json->cartoonSync){
		t=(int)((SDL_GetTicks()-json->timestamp)/16)-json->playtime;
	}
	for(int i=0 ; i<t ; i++){
		_nextCut(json);
		if(json->end)break;
	}
	return !json->talkmode && json->end;
}

bool nextTalk(JsonData *json){
	if(json->talkmode){
		if(json->text_count<(int)strlen(json->talk.str[CHAR_CODE])){
			json->text_count=90;
		}else{
			if(json->jr.talkPointer>=json->jr.talk.valueNum){
				json->talkmode=false;
				json->playtime=0;
				if(json->end){
					json->talk_open_count=-20;
					return true;
				}else{
					nextCut(json);
				}
			}else{
				fetchNextTalking(json);
			}
		}
	}
	return false;
}

void drawTalking(SDL_Surface* scr, JsonData *json, String st, int face, int length, int shake){
	if(json->talk_open_count>0){
		int w=600*(20-json->talk_open_count)/20;
		int h=120*(20-json->talk_open_count)/20;
		drawRect(scr,20,360,w,h,128,128,255,255);
	}
	else if(json->talk_open_count<0){
		int w=600*(-json->talk_open_count)/20;
		int h=120*(-json->talk_open_count)/20;
		drawRect(scr,20,360,w,h,128,128,255,255);
	}else{
		int line1=0,line2=0,line3=0,offset=0;
		if(length<30)line1=length;
		else if(length<60){line1=30;line2=length-30;}
		else if(length<90){line1=30;line2=30;line3=length-60;}
		else{line1=30;line2=30;line3=30;}
		if(shake>0)offset=(3-abs(6-shake%12))*shake/6;
		drawImage(scr,img.menuback,20+offset,360,0,0,600,120,192);
		drawImage(scr,img.facechip,20+offset,370,((face-1)%5)*100,((face-1)/5)*100,100,100,255);
		if(line1!=0)drawText2(scr,120+offset,360,&(st.str[CHAR_CODE][0]),line1);
		if(line2!=0)drawText2(scr,120+offset,400,&(st.str[CHAR_CODE][30]),line2);
		if(line3!=0)drawText2(scr,120+offset,440,&(st.str[CHAR_CODE][60]),line3);
	}
}

void _drawAnimationCut(JsonData *json, SDL_Surface* scr, int from, int to){
	for(int i=from ; i<=to ; i++){
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
		if(!json->obj[i].set.fixed){
			x-=json->scrX;
			x-=json->scrY;
		}
		Image *bg=json->bg[json->obj[i].set.img_id];

		if(json->obj[i].set.drawTo!=-1){
			Image *ima=json->bg[json->obj[i].set.drawTo];
			if(json->obj[i].set.type==CARTOON_DRAWIMAGE){
				if(mag==1){
					drawImage(ima,bg,x,y,ix,iy,w,h,a);
				}else{
					drawImage_x(ima,bg,x,y,mag,ix,iy,w,h,a);
				}
			}
			if(json->obj[i].set.type==CARTOON_ILLUMINATE){
				if(mag==1){
					illuminateImage(ima,bg,x,y,ix,iy,w,h,a);
				}else{
					illuminateImage_x(ima,bg,x,y,mag,ix,iy,w,h,a);
				}
			}
			else if(json->obj[i].set.type==CARTOON_FILLRECT){
				fillRect(ima,x,y,w,h,(int)json->obj[i].set.R,(int)json->obj[i].set.G,(int)json->obj[i].set.B,a);
			}
			else if(json->obj[i].set.type==CARTOON_CARLIGHT){
				drawLightBall(ima,x,y,w,(int)json->obj[i].set.R,(int)json->obj[i].set.G,(int)json->obj[i].set.B);
			}
			else if(json->obj[i].set.type==CARTOON_FILLRECT_GRAD){
				for(int j=0 ; j<h ; j++){
					int R=(int)(json->obj[i].set.gradRfrom+(json->obj[i].set.gradRto-json->obj[i].set.gradRfrom)*j/h);
					int G=(int)(json->obj[i].set.gradGfrom+(json->obj[i].set.gradGto-json->obj[i].set.gradGfrom)*j/h);
					int B=(int)(json->obj[i].set.gradBfrom+(json->obj[i].set.gradBto-json->obj[i].set.gradBfrom)*j/h);
					fillRect(ima,x,y+j,w,1,R,G,B,a);
				}
			}
			else if(json->obj[i].set.type==CARTOON_DRAWIMAGE_WAVE){
				for(int j=0 ; j<h ; j++){
					drawImage(ima,bg,x+(int)(sin((count+j)*json->obj[i].waveSIN)*json->obj[i].waveRange),y+j,ix,iy+j,w,1,a);
				}
			}
		}else{
			if(json->obj[i].set.type==CARTOON_DRAWIMAGE){
				if(mag==1){
					drawImage(scr,bg,x,y,ix,iy,w,h,a);
				}else{
					drawImage_x(scr,bg,x,y,mag,ix,iy,w,h,a);
				}
			}
			if(json->obj[i].set.type==CARTOON_ILLUMINATE){
				if(mag==1){
					illuminateImage(scr,bg,x,y,ix,iy,w,h,a);
				}else{
					illuminateImage_x(scr,bg,x,y,mag,ix,iy,w,h,a);
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
					drawImage(scr,bg,x+(int)(sin((count+j)*json->obj[i].waveSIN)*json->obj[i].waveRange),y+j,ix,iy+j,w,1,a);
				}
			}
		}
	}
	if((json->talkmode && json->jr.talk.valueNum>1) || json->talk_open_count!=0){
		drawTalking(scr,json,json->talk,json->face_count,json->text_count,json->shake_count);
	}
}

void drawAnimationCut(JsonData *json, SDL_Surface* scr){
	_drawAnimationCut(json,scr,0,json->max_obj);
}

void drawAnimationCutBeforeDivision(JsonData *json, SDL_Surface* scr){
	int n=0;
	for(int i=0 ; i<json->max_obj ; i++){
		if(json->obj[i].set.type==CARTOON_DIVISION){
			n=i;
			break;
		}
	}
	_drawAnimationCut(json,scr,0,n-1);
}

void drawAnimationCutAfterDivision(JsonData *json, SDL_Surface* scr){
	int n=0;
	for(int i=0 ; i<json->max_obj ; i++){
		if(json->obj[i].set.type==CARTOON_DIVISION){
			n=i;
			break;
		}
	}
	_drawAnimationCut(json,scr,n+1,json->max_obj);
}
