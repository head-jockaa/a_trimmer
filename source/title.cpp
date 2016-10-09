#include "title.h"

void initTitle2();
void drawOpeningAnim(SDL_Surface* scr, int cn);
int timestamp,loadtime,pausetime;

void initOpening(){
	mode=OPENING;
	count=0;phase=0;
	getImage(img.back,"file/img/testcard.png",0,0,255);
}
void endOpening(){
	freeImage(img.back);
	if(phase==2)freeMusic();
}

void drawOpening(SDL_Surface* scr){
	if(phase==0){
		drawImage(scr,img.back,0,0,0,0,640,480,255);
	}
	else if(phase==1){
		double a=1,b=1;
		if(count<80)a=count/80.0;
		if(count>170 && count<200)b=1+(200-count)/30.0;
		fillRect(scr,0,40,640,400,255,255,255,255);
		if(a==1)drawImage(scr,img.back,0,40,0,480,640,400,255);
		else drawImage_x(scr,img.back,(int)((1.0-a)*240),40+(int)((1.0-a)*200),a,0,480,640,400,255);
		if(count>170){
			if(b==1)drawImage(scr,img.back,0,40,640,480,160,400,255);
			else drawImage_x(scr,img.back,(int)((1.0-b)*160),40+(int)((1.0-b)*200),b,640,480,160,400,255);
		}
		fillRect(scr,0,0,640,40,0,0,0,255);
		fillRect(scr,0,440,640,40,0,0,0,255);
	}
	else if(pauseGame==2){
		if(pauseGame)drawOpeningAnim(scr,(pausetime-timestamp)/16);
		else drawOpeningAnim(scr,(SDL_GetTicks()-timestamp)/16);
	}
	if(EXPLAIN){
		drawKeyboard(scr,key.zC,0,460);
		if(phase==0)TextOut(scr,20,460,text[TITLETEXT]);
		else TextOut(scr,20,460,text[EPILOGUE]);
	}
}

void keyOpening(){
	if(key.z && !key_stop(key.z)){
		if(phase==0){
			phase++;count=0;
		}
		else if(phase>=1){
			endOpening();
			if(movie_test){
				initMiyazaki();
				gd.x=1600;
				gd.scrX=(int)gd.x-320;
				phase=MIYAZAKI_MUSEUM;
				start=0;
				for(int i=0 ; i<29 ; i++)menu[BGM_TEST].cursorDown();
			}else{
				initTitle2();
			}
		}
	}
}

void timerOpening(){
	if(phase==1 && count==300){
		freeImage(img.back);
		phase++;
		initOpeningMainAnime();
	}
	else if(phase==2){
		if((SDL_GetTicks()-timestamp)/16>=3350){
			if(movie_test){
				endOpening();
				initMiyazaki();
				gd.x=1600;
				gd.scrX=(int)gd.x-320;
				phase=MIYAZAKI_MUSEUM;
				start=0;
				for(int i=0 ; i<29 ; i++)menu[BGM_TEST].cursorDown();
			}else{
				endOpening();
				initTitle2();
			}
		}
	}
}

void initOpeningMainAnime(){
	getImage(img.back,"file/img/opening.png",0,0,255);
	bgm=Mix_LoadMUS("file/bgm/1.ogg");
	Mix_PlayMusic(bgm,-1);
	count=-1;
	timestamp=SDL_GetTicks();
	loadtime=0;
}

void initTitle2(){
	mode=TITLE;
	menu[0].setMenu(240,300,10,3,3);
	menu[0].stack(text[TITLETEXT]);
	menu[0].stack(text[TITLETEXT+1]);
	menu[0].stack(text[TITLETEXT+2]);
	menu[0].setBG(192);
	getImage(img.back,"file/img/title.png",0,0,255);
	bgm=Mix_LoadMUS("file/bgm/2.ogg");
	sf.gaze=Mix_LoadWAV("file/se/29.wav");
	start=200;phase=0;count=-1;
}

void endTitle(){
	menu[0].setViewMode(HIDE);
	freeImage(img.back);
	freeMusic();
	Mix_FreeChunk(sf.gaze);
	kick_count=1;
}

void timerTitle(){
	if(start==170 || start==140 || start==110)Mix_PlayChannel(0,sf.decide,0);
	if(start==60)Mix_PlayChannel(0,sf.gaze,0);
	if(start==20){
		menu[0].setViewMode(VISIBLE);
		Mix_PlayMusic(bgm,0);
	}
}

void keyTitle(){
	if(key.z && !key_stop(key.z)){
		if(start==0){
			if(menu[0].selected()==0){
				endTitle();
				initGameMenu();
			}
			else if(menu[0].selected()==1){
				endTitle();
				initOption();
			}
			else if(menu[0].selected()==2){
				run=false;
			}
		}
	}
	if(key.up && !key_stop(key.up))menu[0].cursorUp();
	if(key.down && !key_stop(key.down))menu[0].cursorDown();
}

void drawTitle(SDL_Surface* scr){
	fillRect(scr,0,0,640,480,0,0,0,255);

	if(start<20){
		for(int i=0 ; i<400 ; i++)fillRect(scr,0,40+i,640,1,0,0,i/4,255);
		drawImage(scr,img.back,270-count/75,220-count/75,360,400,64,32,255);
		if(count%50==0)drawImage(scr,img.back,270-count/75,220-count/75,360,432,64,32,255);
		drawImage(scr,img.back,0,40,0,0,640,400,255);
	}
	if(start>170)drawImage_x(scr,img.back,-(start-170)*6,40-(start-170)*2,((start-170)*3+40.0)/40.0,0,480,80,120,255);
	else drawImage(scr,img.back,0,40,0,480,80,120,255);
	if(start<=170){
		if(start>140)drawImage_x(scr,img.back,220-(start-140)*6,40-(start-140)*2,((start-140)*3+40.0)/40.0,80,480,80,120,255);
		else drawImage(scr,img.back,220,40,80,480,80,120,255);
	}
	if(start<=140){
		if(start>110)drawImage_x(scr,img.back,470-(start-110)*6,40-(start-110)*2,((start-110)*3+80.0)/80.0,160,480,160,180,255);
		else drawImage(scr,img.back,460,40,160,480,160,180,255);
	}
	if(start<=60 && start>=20){
		drawImage(scr,img.back,0,280,440,400,180,40,(60-start)*14);
		if(start>=30)drawImage(scr,img.back,180,260,440+((30-(start-30))/10)*40,440,40,40,255);
	}
	if(start<20){
		for(int i=1 ; i<=4 ; i++)drawImage(scr,img.back,40+i*36-(i*36)*start/20,90,(i-1)*36,400,36,68,255);
		for(int i=1 ; i<=6 ; i++)drawImage(scr,img.back,230+i*36-(i*36)*start/20,90,(i+3)*36,400,36,68,255);
		menu[0].drawMenu(scr);
	}
	if(EXPLAIN){
		if(start==0){
			if(count%400<200){
				drawKeyboard(scr,key.zC,0,460);
				TextOut(scr,20,460,text[OPTIONTEXT+1]);
			}
			else{
				drawKeyboard(scr,key.upC,0,460);
				drawKeyboard(scr,key.downC,20,460);
				TextOut(scr,40,460,text[MENUTEXT+15]);
			}
		}
	}
}
