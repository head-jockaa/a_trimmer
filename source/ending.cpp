#include "ending.h"

int which_medal;

void initEnding(){
	mode=ENDING;
	count=0;
	phase=LEAVE_SHORE;
	sprintf_s(str,"file/data/cartoon/end%d.json",which_season);
	loadCartoon(&cartoonJson,str);
	readCartoon(&cartoonJson,0);
	readCartoon(&cartoonJson,4);
}

void initMedalAward(int n){
	count=0;
	which_medal=n;
	phase=GET_MEDAL;
	sprintf_s(str,"file/data/cartoon/medal%d.json",which_medal+1);
	loadCartoon(&cartoonJson, str);
}

void initEndingAnime(){
	bgm=NULL;
	mode=ENDING;
	phase=ENDING_ANIME;
	count=0;
	loadCartoon(&cartoonJson, "file/data/cartoon/ending.json");
}

void endEnding(){
	freeCartoon(&cartoonJson);
	freeImage(img.back);
	freeSound(sf.alarm);
	freeMusic();
	kick_count=1;
}

void keyEndingAnime(){
	if(key.z && !key_stop(key.z)){
		if(movie_test){
			endEnding();
			backToMiyazaki();
			for(int i=0 ; i<35 ; i++)menu[BGM_TEST].cursorDown();
		}else{
			if(which_medal!=0){
				freeImage(img.back);
				getImage(img.back,"file/img/warning.gif",0,0,255);
				Mix_HaltMusic();
				sf.alarm=Mix_LoadWAV("file/se/20.wav");
				Mix_PlayChannel(1,sf.alarm,2);
				phase=WARNING;
			}else{
				endEnding();
				initGameMenu();
			}
		}
	}
}

void keyWarning(){
	if(key.z && !key_stop(key.z)){
		endEnding();
		initGameMenu();
	}
}

void keyEnding(){
	if(phase==LEAVE_SHORE || phase==GET_MEDAL){
		if(key.z && !key_stop(key.z)){
			if(nextTalk(&cartoonJson)){
			}
		}
	}
	else if(phase==ENDING_ANIME)keyEndingAnime();
	else if(phase==WARNING)keyWarning();
}

void timerLeaveShore(){
	if(nextCut(&cartoonJson)){
		int a=100*gd.crops/entries;
		if(a>=100)initMedalAward(3);
		else if(a>=80)initMedalAward(2);
		else if(a>=60)initMedalAward(1);
		else initMedalAward(3);
	}
}

void timerGetMedal(){
	if(nextCut(&cartoonJson)){
		if(movie_test){
			endEnding();
			backToMiyazaki();
			for(int i=0 ; i<29+which_medal ; i++)menu[BGM_TEST].cursorDown();
		}else{
			endEnding();
			if(which_medal==0){
				initGame();
				initGame2();
				kick_count=0;
			}else{
				kick_count=0;
				initEndingAnime();
			}
		}
	}
}

void timerEndingAnime(){
	if(phase==ENDING_ANIME){
		if(nextCut(&cartoonJson)){
			if(movie_test){
				endEnding();
				backToMiyazaki();
				for(int i=0 ; i<35 ; i++)menu[BGM_TEST].cursorDown();
			}else{
				if(which_medal!=0 && which_season!=season_num){//ÅIƒ{ƒX–Ê‚Í•Û—¯
					freeImage(img.back);
					Mix_HaltMusic();
					getImage(img.back,"file/img/warning.gif",0,0,255);
					sf.alarm=Mix_LoadWAV("file/se/20.wav");
					Mix_PlayChannel(1,sf.alarm,2);
					phase=WARNING;
				}else{
					endEnding();
					initGameMenu();
				}
			}
		}
	}
}

void timerEnding(){
	if(phase==LEAVE_SHORE)timerLeaveShore();
	else if(phase==GET_MEDAL)timerGetMedal();
	else if(phase==ENDING_ANIME)timerEndingAnime();
}

void drawEndingExplain(SDL_Surface* scr){
	if(EXPLAIN){
		if(phase==GET_MEDAL){
			drawKeyboard(scr,key.zC,0,0);
			drawText(scr,20,0,text[EPILOGUE+1]);
		}
		else if(phase==ENDING_ANIME){
			drawKeyboard(scr,key.zC,0,460);
			drawText(scr,20,460,text[EPILOGUE]);
		}
		else if(phase==WARNING){
			drawKeyboard(scr,key.zC,0,0);
			drawText(scr,20,0,text[MENUTEXT+4]);
		}
	}
}

void drawEnding(SDL_Surface* scr){
	if(phase==ENDING_ANIME){
		drawAnimationCut(&cartoonJson,scr);
	}
	else if(phase==WARNING){
		fillRect(scr,0,0,640,480,0,0,64,255);
		for(int i=0 ; i<16 ; i++)fillRect(scr,i*40,0,1,480,0,128,0,255);
		for(int i=0 ; i<12 ; i++)fillRect(scr,0,i*40,640,1,0,128,0,255);
		drawImage(scr,img.back,0,70,((count/5)%8)*110,(((count/5)/8)%2)*205,110,205,255);
		drawImage(scr,img.back,320,160,880,0,80,80,abs(128-count%255)*2);
		if(which_season==season_num){
			drawText(scr,280,360,text[EPILOGUE+8]);
			drawText(scr,280,400,text[EPILOGUE+9]);
		}else{
			drawText(scr,280,360,text[EPILOGUE+6]);
			drawText(scr,280,400,text[EPILOGUE+7]);
		}
	}
	else drawAnimationCut(&cartoonJson,scr);

	if(phase==GET_MEDAL){
		if(which_medal!=0)drawText(scr,200,0,text[EPILOGUE+3]);
	}
}
