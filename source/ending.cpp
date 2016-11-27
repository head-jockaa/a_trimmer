#include "ending.h"

int cameraX,cameraY,cameraZ,camera_speedX,camera_speedY,camera_speedZ;
int camera_count,cameraX2;
int which_medal;
Image *mirror;

void drawLastEndingAnim(SDL_Surface* scr, int cn);
void drawAnimeGod(SDL_Surface* scr);
void drawLastStory(SDL_Surface* scr);
void makeMirrorImage(SDL_Surface* scr);

void initEnding(){
	mode=ENDING;
	count=0;
	playtime=0;
	phase=LEAVE_SHORE;
	getImage(img.back,"file/img/shore.png",0,0,255);
	mirror=NULL;
	sf.thunder=NULL;sf.tub=NULL;sf.alarm=NULL;sf.noize=NULL;sf.sunset=NULL;
}

void initMedalAward(int n){
	count=0;
	which_medal=n;
	phase=COME_TORISHIMA;
	load_story(0);
	TalkingAt(25+n);
	sf.thunder=Mix_LoadWAV("file/se/14.wav");
	sf.tub=Mix_LoadWAV("file/se/13.wav");
	sf.sunset=Mix_LoadWAV("file/se/15.ogg");
	sf.alarm=NULL;sf.noize=NULL;sf.swish=NULL;
}

void initEndingAnime(){
	mode=ENDING;
	phase=PRE_ENDING_ANIME;
	count=0;
	bgm=NULL;
	mirror=NULL;
	sf.thunder=NULL;sf.tub=NULL;sf.alarm=NULL;sf.noize=NULL;sf.sunset=NULL;sf.swish=NULL;
	getImage(img.back,"file/img/testcard.png",0,0,255);
}

void initEndingMainAnime(){
	mode=ENDING;
	phase=ENDING_ANIME;
	mirror=NULL;
	sf.thunder=NULL;sf.tub=NULL;sf.alarm=NULL;sf.noize=NULL;sf.sunset=NULL;sf.swish=NULL;
	count=0;
	loadCartoon("file/data/cartoon/ending.json");
}

void initLastStory(){
	mode=ENDING;
	count=0;
	phase=ANIME_GOD;
	load_story(25);
	TalkingAt(25);
	getImage(img.back,"file/img/warning.gif",0,0,255);
	bgm=Mix_LoadMUS("file/bgm/4.ogg");
	Mix_PlayMusic(bgm,-1);
	mirror=NULL;
	sf.thunder=NULL;sf.tub=NULL;sf.alarm=NULL;sf.noize=NULL;sf.sunset=NULL;sf.swish=NULL;
	cameraX=50;cameraY=50;cameraZ=110;camera_count=10;cameraX2=0;
}

void initLastStory2(){
	mode=ENDING;
	load_story(25);
	TalkingAt(26);
	getImage(img.back,"file/img/epilogue2.png",0,0,255);
	mirror=new Image(640,2880);
	fillRect(mirror,0,0,640,2880,0,0,255,255);
	setAlpha(mirror,0,0,255);
	sf.thunder=NULL;sf.tub=NULL;sf.alarm=NULL;sf.sunset=NULL;sf.swish=NULL;
	sf.noize=Mix_LoadWAV("file/se/21.ogg");
	Mix_PlayChannel(0,sf.noize,0);
	phase=LAST_STORY;
	start=850;
}

void initLastEnding(){
	mode=ENDING;
	getImage(img.back,"file/img/ending2.png",0,0,255);
	bgm=Mix_LoadMUS("file/bgm/18.ogg");
	sf.thunder=NULL;sf.tub=NULL;sf.alarm=NULL;sf.noize=NULL;sf.sunset=NULL;sf.swish=NULL;
	mirror=NULL;
	phase=LAST_ENDING;
	timestamp=SDL_GetTicks();
	loadtime=0;
	count=0;
}

void endEnding(){
	freeImage(img.back);
	if(mirror!=NULL)freeImage(mirror);
	if(sf.noize!=NULL)Mix_FreeChunk(sf.noize);
	if(sf.thunder!=NULL)Mix_FreeChunk(sf.thunder);
	if(sf.tub!=NULL)Mix_FreeChunk(sf.tub);
	if(sf.alarm!=NULL)Mix_FreeChunk(sf.alarm);
	if(sf.sunset!=NULL)Mix_FreeChunk(sf.sunset);
	if(sf.swish!=NULL)Mix_FreeChunk(sf.swish);
	freeMusic();
	kick_count=1;
}

void keyEndingAnime(){
	if(key.z && !key_stop(key.z)){
		if(movie_test){
			endEnding();
			initMiyazaki();
			gd.x=1600;
			gd.scrX=(int)gd.x-1320;
			phase=MIYAZAKI_MUSEUM;
			start=0;
			for(int i=0 ; i<31 ; i++)menu[BGM_TEST].cursorDown();
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
	if(phase==LEAVE_SHORE){
		if(key.z && !key_stop(key.z)){
			if(nextTalk()){
			}
		}
	}
	else if(phase==GET_MEDAL || phase==ANIME_GOD || phase==LAST_STORY){
		bool ok=true;
		if(phase==ANIME_GOD && gd.scene_count==1)ok=false;
		if(phase==LAST_STORY && start!=0)ok=false;
		if(key.z && !key_stop(key.z) && ok){
			int a=controlTalking();
			if(a==COMMA){
				if(phase==GET_MEDAL){
					if(gd.scene_count==1){
						gd.shake_count=50;count=0;
					}
					if(gd.scene_count==2){
						gd.shake_count=50;
						Mix_PlayChannel(1,sf.thunder,0);
					}
				}
				else if(phase==ANIME_GOD)start=150;
				else if(phase==LAST_STORY){
					if(gd.scene_count==4)start=800;
					else start=100;
				}
				else count=0;
			}
			else if(a==EOF){
				if(phase==LEAVE_SHORE){
					start=100;
				}
				else if(phase==GET_MEDAL){
					if(which_medal!=0 && count<20)Mix_PlayChannel(1,sf.decide,0);
					phase=LEAVE_TORISHIMA;
					count=0;
				}
				else if(phase==LAST_STORY){
					if(movie_test){
						endTitle();
						initMiyazaki();
						gd.x=1600;
						gd.scrX=(int)gd.x-320;
						phase=MIYAZAKI_MUSEUM;
						start=0;
						for(int i=0 ; i<34 ; i++)menu[BGM_TEST].cursorDown();
					}else{
						endEnding();
						kick_count=0;
						initLastEnding();
					}
				}
			}
		}
	}
	else if(phase==PRE_ENDING_ANIME || phase==ENDING_ANIME)keyEndingAnime();
	else if(phase==WARNING)keyWarning();
}

void timerLeaveShore(){
	if(nextCut()){
		int a=100*gd.crops/works;
		if(a>=100)initMedalAward(3);
		else if(a>=80)initMedalAward(2);
		else if(a>=60)initMedalAward(1);
		else initMedalAward(0);
	}
}

void timerComeTorishima(){
	if(count==125)phase=GET_MEDAL;
}

void timerGetMedal(){
	if(which_medal!=0){
		if(gd.scene_count==1 && count==20)Mix_PlayChannel(1,sf.tub,0);
	}
}

void timerLeaveTorishima(){
	if(which_medal==0){
		if(count==1)Mix_PlayChannel(1,sf.tub,0);
		if(count==150)Mix_PlayChannel(1,sf.sunset,0);
	}
	if((which_medal!=0 && count==100) || count==500){
		if(movie_test){
			endEnding();
			initMiyazaki();
			gd.x=1600;
			gd.scrX=(int)gd.x-320;
			phase=MIYAZAKI_MUSEUM;
			start=0;
			for(int i=0 ; i<25+which_medal ; i++)menu[BGM_TEST].cursorDown();
		}else{
			endEnding();
			if(which_medal==0){
				initGame();
				load_story(dataNo);
				initGame2();
				head_of_talking();
				head_of_timeslot();
				gd.talk_count=EOF;
				kick_count=0;
			}else{
				kick_count=0;
				initEndingAnime();
			}
		}
	}
}

void timerAnimeGod(){
	if(count%3==0){
		cameraX-=camera_speedX;
		cameraY-=camera_speedY;
		cameraZ-=camera_speedZ;
		camera_count--;
		if(cameraX<0)cameraX=0;
		if(cameraY<0)cameraY=0;
		if(cameraZ<90)cameraZ=90;
		if(cameraX>100)cameraX=100;
		if(cameraY>100)cameraY=100;
		if(cameraZ>130)cameraZ=130;
		if(camera_count==0){
			srand(SDL_GetTicks());
			camera_speedX=5-rand()%11;
			camera_speedY=5-rand()%11;
			camera_speedZ=1-rand()%3;
			camera_count=rand()%10+1;
		}
	}
	if(gd.scene_count==1){
		cameraX2++;
		if(start==1){
			Mix_PlayChannel(0,sf.decide,0);
			if(movie_test){
				endEnding();
				initMiyazaki();
				gd.x=1600;
				gd.scrX=(int)gd.x-320;
				phase=MIYAZAKI_MUSEUM;
				start=0;
				for(int i=0 ; i<33 ; i++)menu[BGM_TEST].cursorDown();
			}else{
				endEnding();
				initLastStory2();
			}
			kick_count=0;
		}
	}
}

void timerLastStory(){
	if(gd.scene_count==0){
		if(start==100)Mix_PlayChannel(0,sf.decide,0);
	}
	else if(gd.scene_count==1){
		if(start==99){
			sf.noize=Mix_LoadWAV("file/se/22.wav");
			Mix_PlayChannel(0,sf.noize,0);
		}
	}
	else if(gd.scene_count==2){
		if(start==99){
			bgm=Mix_LoadMUS("file/bgm/15.ogg");
			Mix_PlayMusic(bgm,-1);
			Mix_FreeChunk(sf.noize);
			sf.noize=Mix_LoadWAV("file/se/4.wav");
			Mix_PlayChannel(0,sf.noize,0);
		}
		if(start==50){
			Mix_FreeChunk(sf.noize);
			sf.noize=Mix_LoadWAV("file/se/17.wav");
			Mix_PlayChannel(0,sf.noize,0);
		}
	}
	else if(gd.scene_count==3){
		if(start==1){
			Mix_FreeChunk(sf.noize);
			sf.noize=Mix_LoadWAV("file/se/7.wav");
		}
	}
	else if(gd.scene_count==4){
		if(start>=560 && start%5==0)Mix_PlayChannel(0,sf.noize,0);
		if(start==500){
			Mix_FadeOutMusic(2000);
			Mix_FreeChunk(sf.noize);
			sf.noize=Mix_LoadWAV("file/se/5.wav");
			Mix_PlayChannel(0,sf.noize,0);
		}
		if(start==450){
			Mix_FreeChunk(sf.noize);
			sf.noize=Mix_LoadWAV("file/se/23.wav");
			Mix_PlayChannel(0,sf.noize,0);
		}
		if(start==350){
			Mix_FreeChunk(sf.noize);
			sf.noize=Mix_LoadWAV("file/se/24.wav");
			Mix_PlayChannel(0,sf.noize,0);
		}
		if(start==50){
			Mix_FreeChunk(sf.noize);
			sf.noize=Mix_LoadWAV("file/se/14.wav");
			Mix_PlayChannel(0,sf.noize,0);
		}
	}
}

void timerEndingAnime(){
	if(phase==PRE_ENDING_ANIME){
		if(count==150){
			freeImage(img.back);
			initEndingMainAnime();
		}
	}
	else if(phase==ENDING_ANIME){
		int t=(int)((SDL_GetTicks()-loadtime)/16);
		for(int i=0 ; i<t-playtime ; i++){
			if(nextCut()){
				if(movie_test){
					endEnding();
					initMiyazaki();
					gd.x=1600;
					gd.scrX=(int)gd.x-320;
					phase=MIYAZAKI_MUSEUM;
					start=0;
					for(int i=0 ; i<31 ; i++)menu[BGM_TEST].cursorDown();
				}else{
					if(which_medal!=0 && dataNo!=index_num){//最終ボス面は保留
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
				break;
			}
		}
	}
}

void timerLastEnding(){
	if((SDL_GetTicks()-timestamp)/16>=50 && loadtime==0){
		Mix_PlayMusic(bgm,-1);
		loadtime=50;
	}
	if(count==1 || count==11)Mix_PlayChannel(0,sf.decide,0);
	if((SDL_GetTicks()-timestamp)/16>=4900){
		if(movie_test){
			endEnding();
			initMiyazaki();
			gd.x=1600;
			gd.scrX=(int)gd.x-320;
			phase=MIYAZAKI_MUSEUM;
			start=0;
			for(int i=0 ; i<34 ; i++)menu[BGM_TEST].cursorDown();
		}else{
			endEnding();
			initGameMenu();
		}
	}
}

void timerEnding(){
	if(phase==LEAVE_SHORE)timerLeaveShore();
	else if(phase==COME_TORISHIMA)timerComeTorishima();
	else if(phase==GET_MEDAL)timerGetMedal();
	else if(phase==LEAVE_TORISHIMA)timerLeaveTorishima();
	else if(phase==ANIME_GOD)timerAnimeGod();
	else if(phase==LAST_STORY)timerLastStory();
	else if(phase==PRE_ENDING_ANIME || phase==ENDING_ANIME)timerEndingAnime();
	else if(phase==LAST_ENDING)timerLastEnding();
	if(phase==LEAVE_SHORE || phase==GET_MEDAL || phase==ANIME_GOD || phase==LAST_STORY){
	  if(start==0 || (phase==ANIME_GOD && start<50))controlTextCount(true);
	  else controlTextCount(false);
	}else{
		controlTextCount(false);
	}
}

void drawTryAgain(SDL_Surface* scr){
	int a=512;
	if(count<200)a=0;
	else if(count<250)a=(count-200)*10;
	fillRect(scr,0,0,640,480,0,0,0,255);
	drawImage(scr,img.back,420,360,1040,1120,220,60,255);
	for(int i=0 ; i<220 ; i++){
		drawImage(scr,img.back,420+i,360,1040+i,1180,2,60,-10*abs(25-i%50)+a);
	}
	if(count>250){
		if(count<300){
			for(int i=-2 ; i<=2 ; i++)drawImage(scr,img.back,420+i*(300-count)/5,360,1040,1240,30,60,50);
		}
		else drawImage(scr,img.back,420,360,1040,1240,30,60,255);
	}
	if(count>270){
		if(count<320){
			for(int i=-2 ; i<=2 ; i++)drawImage(scr,img.back,540+i*(320-count)/5,360,1160,1240,20,60,50);
		}
		else drawImage(scr,img.back,540,360,1160,1240,20,60,255);
	}
	if(count>260){
		if(count<310){
			for(int i=-2 ; i<=2 ; i++)drawImage(scr,img.back,600+i*(310-count)/5,360,1220,1240,30,60,50);
		}
		else drawImage(scr,img.back,600,360,1220,1240,30,60,255);
	}
	if(count>300)drawImage(scr,img.back,420,360,1040,1240,220,60,(count-300)*15);
}

void drawTorishima(SDL_Surface *scr){
	fillRect(scr,0,60,640,1360,96,136,216,255);
	drawImage(scr,img.back,0,240,0,800,640,240,255);
	if(face[gd.face_count]==12)drawImage(scr,img.back,580,300,640,1240+abs((count/5)%2)*60,60,60,255);
	else drawImage(scr,img.back,580,300,640,1060+abs(2-(count/10)%4)*60,60,60,255);
	if(which_medal==0){
		if(gd.scene_count>=1)drawImage(scr,img.back,560,140,640,980,80,80,255);
		if(gd.scene_count==2 && phase==GET_MEDAL)drawImage(scr,img.back,240,180,720,800+((count/5)%2)*240,320,240,255);
	}
	if(phase==COME_TORISHIMA){
		drawImage(scr,img.back,-500+count*4,220-abs(20-(count/5)%40)*2,760,300,490,180,255);
		drawImage(scr,img.back,-500+count*4,220-abs(20-(count/5)%40)*2,760,480,490,180,255);
		drawImage(scr,img.back,-320+count*4,240-abs(20-(count/5)%40)*2,760,660,120,140,255);
		drawImage(scr,img.back,-500+count*4,220-abs(20-(count/5)%40)*2,760,0,490,300,255);
	}
	else if(phase==LEAVE_TORISHIMA){
		if(which_medal==0){
			if(count<150){
				drawImage(scr,img.back,160,240,1040,800+((count/5)%2)*160,200,160,255);
				if(count>100)fillRect(scr,0,0,640,480,0,0,0,(count-100)*5);
			}
			else drawTryAgain(scr);
		}else{
			drawImage(scr,img.back,-count*4,220-abs(20-(count/5)%40)*2,760,300,490,180,255);
			drawImage(scr,img.back,-count*4,220-abs(20-(count/5)%40)*2,760,480,490,180,255);
			drawImage(scr,img.back,180-count*4,240-abs(20-(count/5)%40)*2,760,660,120,140,255);
			drawImage(scr,img.back,-count*4,220-abs(20-(count/5)%40)*2,760,0,490,300,255);
		}
	}else{
		drawImage(scr,img.back,0,220-abs(20-(count/5)%40)*2,760,300,490,180,255);
		drawImage(scr,img.back,0,220-abs(20-(count/5)%40)*2,760,480,490,180,255);
		drawImage(scr,img.back,180,240-abs(20-(count/5)%40)*2,760,660,120,140,255);
		drawImage(scr,img.back,0,220-abs(20-(count/5)%40)*2,760,0,490,300,255);
	}
	if(which_medal!=0){
		int a=(which_medal-1);
		if(phase==LEAVE_TORISHIMA){
			drawImage_x(scr,img.back,240-count*4,200-abs(20-(count/5)%40)*2,4,640,800+a*60,60,60,255);
		}else{
			if(gd.scene_count==0)drawImage(scr,img.back,500,280-abs(10-(count/3)%20)*2,640,800+a*60,60,60,255);
			if(gd.scene_count==1){
				if(count<20)drawImage_x(scr,img.back,480-count*12,280-count*4-abs(20-(count/5)%40)*2,2.0+count/10.0,640,800+a*60,60,60,255);
				else drawImage_x(scr,img.back,240,200-abs(20-(count/5)%40)*2,4,640,800+a*60,60,60,255);
			}
		}
	}
	fillRect(scr,0,0,640,60,0,0,0,255);
	fillRect(scr,0,420,640,60,0,0,0,255);
	if(which_medal!=0)drawText(scr,200,0,text[EPILOGUE+3]);
	if(phase==GET_MEDAL)drawTalking(scr);
}

void drawEndingExplain(SDL_Surface* scr){
	if(EXPLAIN){
		if(phase==GET_MEDAL || ((phase==ANIME_GOD || phase==LAST_STORY) && start==0)){
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
	if(phase==PRE_ENDING_ANIME){
		int a=255;
		if(count<20)a=count*13;
		if(count>130)a=(150-count)*13;
		fillRect(scr,0,0,640,480,0,0,0,255);
		drawImage(scr,img.back,120,40,0,880,400,300,a);
		if(CHAR_CODE==JAPANESE)drawImage(scr,img.back,200,380,400,880,240,80,a);
		else drawImage(scr,img.back,200,380,400,960,240,80,a);
	}
	else if(phase==ENDING_ANIME){
		drawAnimationCut(scr);
	}
	else if(phase==LAST_ENDING){
		if(pauseGame)drawLastEndingAnim(scr,(pausetime-timestamp)/16);
		else drawLastEndingAnim(scr,(SDL_GetTicks()-timestamp)/16);
	}
	else if(phase==WARNING){
		fillRect(scr,0,0,640,480,0,0,64,255);
		for(int i=0 ; i<16 ; i++)fillRect(scr,i*40,0,1,240,0,128,0,255);
		for(int i=0 ; i<12 ; i++)fillRect(scr,0,i*40,640,1,0,128,0,255);
		drawImage(scr,img.back,0,70,((count/5)%8)*220,(((count/5)/8)%2)*410,220,410,255);
		drawImage(scr,img.back,320,160,1760,0,160,160,abs(128-count%255)*2);
		if(dataNo==index_num){
			drawText(scr,280,360,text[EPILOGUE+8]);
			drawText(scr,280,400,text[EPILOGUE+9]);
		}else{
			drawText(scr,280,360,text[EPILOGUE+6]);
			drawText(scr,280,400,text[EPILOGUE+7]);
		}
	}
	else if(phase==ANIME_GOD){
		drawAnimeGod(scr);
	}
	else if(phase==LAST_STORY){
		drawLastStory(scr);
	}
	else if(phase==LEAVE_SHORE){
		drawAnimationCut(scr);
	}
	else drawTorishima(scr);
}

void drawAnimeGod(SDL_Surface* scr){
	fillRect(scr,0,0,640,480,96,136,216,255);
	drawImage_x(scr,img.back,-cameraX,-cameraY,(330.0+cameraZ)/420.0,880,170,420,340,255);
	drawImage_x(scr,img.back,-cameraX-cameraX2+150,-cameraY-cameraX2+80,(cameraZ+cameraX2*2)/110.0,((count/5)%8)*110,(((count/5)/8)%2)*205,110,205,255);
	drawImage(scr,img.back,240,180,1760,160,160,120,255);
	drawImage(scr,img.back,40,40,1760,280,40,60,255);
	if(count%60<30)drawImage(scr,img.back,80,40,1800,280,60,40,255);
	if((count/60)%60<10)sprintf_s(str,"%2d:0%d",count/3600,(count/60)%60);
	else sprintf_s(str,"%2d:%d",count/3600,(count/60)%60);
	drawText(scr,520,40,str);
	if(start<50)drawTalking(scr);
}

//このバージョンではまだ使わない
void drawLastStory(SDL_Surface* scr){
	int a;
	fillRect(scr,0,0,320,180,0,0,0,255);
	if(gd.scene_count==0){
		a=850-start;
		if(a<650){
			if(a%30<15)drawImage(scr,img.back,0,0,0,((a%30)/5)*180,320,180,(int)(a*0.39));
			else drawImage(scr,img.back,0,0,0,0,320,180,(int)(a*0.39));
		}
		else if(a<750)fillRect(scr,0,0,320,240,0,0,0,255);
		else{
			if(a<800)drawImage(scr,img.back,0,0,0,720,320,360,255);
			else drawImage(scr,img.back,0,0,0,720-(int)((a-800)*3.6),320,360,255);
			if(a<770)fillRect(scr,0,0,(770-a)*16,180,0,0,0,255);
		}
	}
	else if(gd.scene_count==1){
		drawImage(scr,img.back,0,0,320,0,320,180,255);
	}
	else if(gd.scene_count==2){
		a=100-start;
		if(a<50)drawImage(scr,img.back,0,0,320,180,320,180,255);
		else drawImage(scr,img.back,0,0,320,360,320,180,255);
		if(a>=50 && a<100)drawImage(scr,img.back,0,0,640,360+((a/2)%2)*180,320,180,255);
	}
	else if(gd.scene_count==3){
		a=100-start;
		drawImage(scr,img.back,0,0,320,540,320,180,255);
		drawImage(scr,img.back,220,200-a,640,720,80,80,255);
	}
	else if(gd.scene_count==4){
		a=800-start;
		if(a<350)fillRect(scr,0,0,320,240,0,0,192,255);
		else fillRect(scr,0,180,320,60,0,0,0,255);
		if(a<350){
			if(a/5>0)drawText(scr,90,60,"Anime Trimmer X",a/5);
			if(a/5-15>0)drawText(scr,70,90,"Episode7 Eng Sub (1/3)",a/5-15);
			if(a/5-37>0)drawText(scr,120,120,"uploaded",a/5-37);
		}else{
			if(a<450)drawImage(scr,img.back,0,0,640,180,320,180,255);
			else if(a<750){
				drawImage(scr,img.back,0,0,640,180-(int)((a-450)*0.6),320,180,255);
				if(count%20<10)drawImage(scr,img.back,80,-(int)((750-a)*0.6),720,720,240,140,255);
			}
			else{
				drawImage(scr,img.back,0,0,640,0,320,180,255);
				if(count%20<10)drawImage(scr,img.back,80,0,720,720,240,140,255);
				if(a<800)drawImage(scr,img.back,0,0,640,360+((a/2)%2)*180,320,180,255);
			}
		}
		if(a==250)makeMirrorImage(scr);
		if(a>=300){
			if(a<350){
				int b=(int)((a-300)*3.2), c=(int)((a-300)*2.4);
				drawImage(scr,mirror,160-b,120-c,160-b,120-c,b*2,c*2,255);
			}
			else if(a<450){
				double b=(a-350)/2, c=200-(a-370)*(a-370)/4, d=(a-350)*(a-350)/4;
				drawImage(scr,mirror,(int)b,(int)(-c/2),0,240,320,240,255);
				drawImage(scr,mirror,(int)(b*2),(int)(-c/4),0,480,320,240,255);
				drawImage(scr,mirror,(int)b,(int)(d/6),0,720,320,240,255);
				drawImage(scr,mirror,(int)-b,(int)(d/6),0,960,320,240,255);
				drawImage(scr,mirror,(int)(-b*2),(int)(-c/4),0,1200,320,240,255);
			}
		}
	}
	if(gd.scene_count!=4)fillRect(scr,0,180,320,60,0,0,0,255);
	if(start==0)drawTalking(scr);
}

void makeMirrorImage(SDL_Surface *scr){
	SDL_PixelFormat *f3;
	Uint8 *px3;
	Uint16 pitch3;
	Uint8 bypp3;
	SDL_Color col,col3;
	Uint32 pixel3;

	f3 = scr->format;
	px3 = (Uint8*)scr->pixels;
	pitch3 = scr->pitch;
	bypp3 = f3->BytesPerPixel;

	for(int i=0 ; i<640 ; i++)for(int j=0 ; j<480 ; j++){
		getRGB(img.back->RGB[(j+1440)*(img.back->w)+(i+640)],&col.r,&col.g,&col.b);
		pixel3 = *(Uint32*)(px3 + pitch3*j + bypp3*i);
		SDL_GetRGB(pixel3,f3,&col3.r,&col3.g,&col3.b);
		if(col.r==255 && col.g==0 && col.b==0){
			mirror->RGB[j*640+i]=setRGB(255,255,255);
		}
		else if(col.r==0){
			mirror->RGB[(j+480)*640+i]=setRGB(col3.r,col3.g,col3.b);
		}
		else if(col.r==64){
			mirror->RGB[(j+960)*640+i]=setRGB(col3.r,col3.g,col3.b);
		}
		else if(col.r==128){
			mirror->RGB[(j+1440)*640+i]=setRGB(col3.r,col3.g,col3.b);
		}
		else if(col.r==192){
			mirror->RGB[(j+1920)*640+i]=setRGB(col3.r,col3.g,col3.b);
		}
		else if(col.r==255){
			mirror->RGB[(j+2400)*640+i]=setRGB(col3.r,col3.g,col3.b);
		}
	}

	SDL_UnlockSurface(scr);
}
