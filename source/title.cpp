#include "title.h"

void initTitle2();

void initOpening(){
	mode=OPENING;
	count=0;phase=0;
	getImage(img.back,"file/img/testcard.png",0,0,255);
}
void endOpening(){
	freeCartoon();
	freeImage(img.back);
	freeMusic();
}

void drawOpening(SDL_Surface* scr){
	if(phase==0){
		drawImage(scr,img.back,0,0,0,0,640,480,255);
	}
	else if(phase==1){
		drawAnimationCut(scr);
	}
	if(EXPLAIN){
		drawKeyboard(scr,key.zC,0,460);
		if(phase==0)drawText(scr,20,460,text[TITLETEXT]);
		else drawText(scr,20,460,text[EPILOGUE]);
	}
}

void keyOpening(){
	if(key.z && !key_stop(key.z)){
		if(phase==0){
			initOpeningAnime();
		}
		else if(phase==1){
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
	if(phase==1){
		if(nextCut()){
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

void initOpeningAnime(){
	mode=OPENING;
	count=0;phase=1;
	loadCartoon("file/data/cartoon/opening.json");
}

void initTitle2(){
	mode=TITLE;
	menu[0].setMenu(240,300,10,3,3);
	menu[0].stack(text[TITLETEXT]);
	menu[0].stack(text[TITLETEXT+1]);
	menu[0].stack(text[TITLETEXT+2]);
	menu[0].setBG(192);
	phase=0;
	loadCartoon("file/data/cartoon/title.json");
}

void endTitle(){
	menu[0].setViewMode(HIDE);
	freeImage(img.back);
	freeMusic();
	freeCartoon();
	freeSound(sf.gaze);
	kick_count=1;
}

void timerTitle(){
	if(nextCut()){
		menu[0].setViewMode(VISIBLE);
		phase=1;
	}
}

void keyTitle(){
	if(phase==0)return;
	if(key.z && !key_stop(key.z)){
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
	if(key.up && !key_stop(key.up))menu[0].cursorUp();
	if(key.down && !key_stop(key.down))menu[0].cursorDown();
}

void drawTitle(SDL_Surface* scr){
	drawAnimationCut(scr);
	if(phase==1){
		menu[0].drawMenu(scr);
	}
	if(EXPLAIN){
		if(phase==1){
			if(count%400<200){
				drawKeyboard(scr,key.zC,0,460);
				drawText(scr,20,460,text[OPTIONTEXT+1]);
			}
			else{
				drawKeyboard(scr,key.upC,0,460);
				drawKeyboard(scr,key.downC,20,460);
				drawText(scr,40,460,text[MENUTEXT+15]);
			}
		}
	}
}
