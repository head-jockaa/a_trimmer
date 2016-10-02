#include "ending.h"

int cameraX,cameraY,cameraZ,camera_speedX,camera_speedY,camera_speedZ;
int camera_count,cameraX2;
int which_medal;
Image *mirror;

void drawEndingAnim(SDL_Surface* scr, int cn);
void drawLastEndingAnim(SDL_Surface* scr, int cn);
void drawAnimeGod(SDL_Surface* scr);
void drawLastStory(SDL_Surface* scr);
void makeMirrorImage(SDL_Surface* scr);

void initEnding(){
	mode=ENDING;
	count=0;
	start=100;
	load_story(dataNo);
	if(dataNo==1)TalkingAt(4);
	else TalkingAt(3);
	phase=LEAVE_SHORE;
	getImage(img.back,"file/img/shore.png",0,0,255);
	bgm=Mix_LoadMUS("file/bgm/4.ogg");
	Mix_PlayMusic(bgm,-1);
	sf.swish=Mix_LoadWAV("file/se/25.wav");
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
	getImage(img.back,"file/img/ending.png",0,0,255);
	bgm=Mix_LoadMUS("file/bgm/16.ogg");
	Mix_PlayMusic(bgm,-1);
	mode=ENDING;
	phase=ENDING_ANIME;
	mirror=NULL;
	sf.thunder=NULL;sf.tub=NULL;sf.alarm=NULL;sf.noize=NULL;sf.sunset=NULL;sf.swish=NULL;
	timestamp=SDL_GetTicks();
	loadtime=0;
	count=0;
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
	mirror=new Image(320,1440);
	fillRect(mirror,0,0,320,1440,0,0,255,255);
	setColorKey(mirror,0,0,255);
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
	if(bgm!=NULL)Mix_FreeMusic(bgm);
	kick_count=1;
}

void keyEndingAnime(){
	if(key.z && !key_stop(key.z)){
		if(movie_test){
			endEnding();
			initMiyazaki();
			gd.x=800;
			gd.scrX=(int)gd.x-160;
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
	if(phase==LEAVE_SHORE || phase==GET_MEDAL || phase==ANIME_GOD || phase==LAST_STORY){
		BOOL ok=TRUE;
		if(phase==ANIME_GOD && gd.scene_count==1)ok=FALSE;
		if(phase==LAST_STORY && start!=0)ok=FALSE;
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
						gd.x=800;
						gd.scrX=(int)gd.x-160;
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
	if(face[gd.face_count]==EOF){
		if(start==1){
			Mix_FreeChunk(sf.swish);
			int a=100*gd.crops/works;
			if(a>=100)initMedalAward(3);
			else if(a>=80)initMedalAward(2);
			else if(a>=60)initMedalAward(1);
			else initMedalAward(0);
		}
	}else{
		if(start==50)Mix_PlayChannel(0,sf.swish,0);
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
	if(which_medal!=0 && count==100 || count==500){
		if(movie_test){
			endEnding();
			initMiyazaki();
			gd.x=800;
			gd.scrX=(int)gd.x-160;
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
				gd.x=800;
				gd.scrX=(int)gd.x-160;
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
		if((SDL_GetTicks()-timestamp)/16>=4360){
			if(movie_test){
				endEnding();
				initMiyazaki();
				gd.x=800;
				gd.scrX=(int)gd.x-160;
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
			gd.x=800;
			gd.scrX=(int)gd.x-160;
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
	  if(start==0 || phase==ANIME_GOD && start<50)controlTextCount(TRUE);
	  else controlTextCount(FALSE);
	}else{
		controlTextCount(FALSE);
	}
}

void drawTryAgain(SDL_Surface* scr){
	int a=512;
	if(count<200)a=0;
	else if(count<250)a=(count-200)*10;
	fillRect(scr,0,0,320,240,0,0,0,255);
	drawImage(scr,img.back,210,180,520,560,110,30,255);
	for(int i=0 ; i<110 ; i++){
		drawImage(scr,img.back,210+i,180,520+i,590,1,30,-10*abs(25-i%50)+a);
	}
	if(count>250){
		if(count<300){
			for(int i=-2 ; i<=2 ; i++)drawImage(scr,img.back,210+i*(300-count)/10,180,520,620,15,30,50);
		}
		else drawImage(scr,img.back,210,180,520,620,15,30,255);
	}
	if(count>270){
		if(count<320){
			for(int i=-2 ; i<=2 ; i++)drawImage(scr,img.back,270+i*(320-count)/10,180,580,620,10,30,50);
		}
		else drawImage(scr,img.back,270,180,580,620,10,30,255);
	}
	if(count>260){
		if(count<310){
			for(int i=-2 ; i<=2 ; i++)drawImage(scr,img.back,300+i*(310-count)/10,180,610,620,15,30,50);
		}
		else drawImage(scr,img.back,300,180,610,620,15,30,255);
	}
	if(count>300)drawImage(scr,img.back,210,180,520,620,110,30,(count-300)*15);
}

void drawTorishima(SDL_Surface *scr){
	fillRect(scr,0,30,320,180,96,136,216,255);
	drawImage(scr,img.back,0,120,0,400,320,120,255);
	if(face[gd.face_count]==12)drawImage(scr,img.back,290,150,320,620+abs((count/5)%2)*30,30,30,255);
	else drawImage(scr,img.back,290,150,320,530+abs(2-(count/10)%4)*30,30,30,255);
	if(which_medal==0){
		if(gd.scene_count>=1)drawImage(scr,img.back,280,70,320,490,40,40,255);
		if(gd.scene_count==2 && phase==GET_MEDAL)drawImage(scr,img.back,120,90,360,400+((count/5)%2)*120,160,120,255);
	}
	if(phase==COME_TORISHIMA){
		drawImage(scr,img.back,-250+count*2,110-abs(20-(count/5)%40),380,150,245,90,255);
		drawImage(scr,img.back,-250+count*2,110-abs(20-(count/5)%40),380,240,245,90,255);
		drawImage(scr,img.back,-160+count*2,120-abs(20-(count/5)%40),380,330,60,70,255);
		drawImage(scr,img.back,-250+count*2,110-abs(20-(count/5)%40),380,0,245,150,255);
	}
	else if(phase==LEAVE_TORISHIMA){
		if(which_medal==0){
			if(count<150){
				drawImage(scr,img.back,80,120,520,400+((count/5)%2)*80,100,80,255);
				if(count>100)fillRect(scr,0,0,320,240,0,0,0,(count-100)*5);
			}
			else drawTryAgain(scr);
		}else{
			drawImage(scr,img.back,-count*2,110-abs(20-(count/5)%40),380,150,245,90,255);
			drawImage(scr,img.back,-count*2,110-abs(20-(count/5)%40),380,240,245,90,255);
			drawImage(scr,img.back,90-count*2,120-abs(20-(count/5)%40),380,330,60,70,255);
			drawImage(scr,img.back,-count*2,110-abs(20-(count/5)%40),380,0,245,150,255);
		}
	}else{
		drawImage(scr,img.back,0,110-abs(20-(count/5)%40),380,150,245,90,255);
		drawImage(scr,img.back,0,110-abs(20-(count/5)%40),380,240,245,90,255);
		drawImage(scr,img.back,90,120-abs(20-(count/5)%40),380,330,60,70,255);
		drawImage(scr,img.back,0,110-abs(20-(count/5)%40),380,0,245,150,255);
	}
	if(which_medal!=0){
		int a=(which_medal-1);
		if(phase==LEAVE_TORISHIMA){
			drawImage_x2(scr,img.back,120-count*2,100-abs(20-(count/5)%40),2,320,400+a*30,30,30);
		}else{
			if(gd.scene_count==0)drawImage(scr,img.back,250,140-abs(10-(count/3)%20),320,400+a*30,30,30,255);
			if(gd.scene_count==1){
				if(count<20)drawImage_x2(scr,img.back,240-count*6,140-count*2-abs(20-(count/5)%40),1.0+count/20.0,320,400+a*30,30,30);
				else drawImage_x2(scr,img.back,120,100-abs(20-(count/5)%40),2,320,400+a*30,30,30);
			}
		}
	}
	fillRect(scr,0,0,320,30,0,0,0,255);
	fillRect(scr,0,210,320,30,0,0,0,255);
	if(which_medal!=0)TextOut(scr,100,0,text[EPILOGUE+3]);
	if(phase==GET_MEDAL)drawTalking(scr);
}

void drawEndingExplain(SDL_Surface* scr){
	if(EXPLAIN){
		if(phase==GET_MEDAL || (phase==ANIME_GOD||phase==LAST_STORY)&&start==0){
			drawKeyboard(scr,key.zC,0,0);
			TextOut(scr,20,0,text[EPILOGUE+1]);
		}
		else if(phase==ENDING_ANIME){
			drawKeyboard(scr,key.zC,0,220);
			TextOut(scr,20,220,text[EPILOGUE]);
		}
		else if(phase==WARNING){
			drawKeyboard(scr,key.zC,0,0);
			TextOut(scr,20,0,text[MENUTEXT+4]);
		}
	}
}

void drawEnding(SDL_Surface* scr){
	if(phase==PRE_ENDING_ANIME){
		int a=255;
		if(count<20)a=count*13;
		if(count>130)a=(150-count)*13;
		fillRect(scr,0,0,320,240,0,0,0,255);
		drawImage(scr,img.back,60,20,0,440,200,150,a);
		if(CHAR_CODE==JAPANESE)drawImage(scr,img.back,100,190,200,440,120,40,a);
		else drawImage(scr,img.back,100,190,200,480,120,40,a);
	}
	else if(phase==ENDING_ANIME){
		if(pause)drawEndingAnim(scr,(pausetime-timestamp)/16);
		else drawEndingAnim(scr,(SDL_GetTicks()-timestamp)/16);
	}
	else if(phase==LAST_ENDING){
		if(pause)drawLastEndingAnim(scr,(pausetime-timestamp)/16);
		else drawLastEndingAnim(scr,(SDL_GetTicks()-timestamp)/16);
	}
	else if(phase==WARNING){
		fillRect(scr,0,0,320,240,0,0,64,255);
		for(int i=0 ; i<16 ; i++)fillRect(scr,i*20,0,1,240,0,128,0,255);
		for(int i=0 ; i<12 ; i++)fillRect(scr,0,i*20,320,1,0,128,0,255);
		drawImage(scr,img.back,0,35,((count/5)%8)*110,(((count/5)/8)%2)*205,110,205,255);
		drawImage(scr,img.back,160,80,880,0,80,80,abs(128-count%255)*2);
		if(dataNo==index_num){
			TextOut(scr,140,180,text[EPILOGUE+8]);
			TextOut(scr,140,200,text[EPILOGUE+9]);	
		}else{
			TextOut(scr,140,180,text[EPILOGUE+6]);
			TextOut(scr,140,200,text[EPILOGUE+7]);	
		}
	}
	else if(phase==ANIME_GOD){
		drawAnimeGod(scr);
	}
	else if(phase==LAST_STORY){
		drawLastStory(scr);
	}
	else if(phase==LEAVE_SHORE){
		drawSeaSide(scr);
	}
	else drawTorishima(scr);
}

void drawAnimeGod(SDL_Surface* scr){
	fillRect(scr,0,0,320,240,96,136,216,255);
	drawImage_x2(scr,img.back,-cameraX,-cameraY,(330.0+cameraZ)/420.0,880,170,420,340);
	drawImage_x2(scr,img.back,-cameraX-cameraX2+150,-cameraY-cameraX2+80,(cameraZ+cameraX2*2)/110.0,((count/5)%8)*110,(((count/5)/8)%2)*205,110,205);
	drawImage(scr,img.back,120,90,880,80,80,60,255);
	drawImage(scr,img.back,20,20,880,140,20,30,255);
	if(count%60<30)drawImage(scr,img.back,40,20,900,140,30,20,255);
	if((count/60)%60<10)sprintf_s(str,"%2d:0%d",count/3600,(count/60)%60);
	else sprintf_s(str,"%2d:%d",count/3600,(count/60)%60);
	TextOut(scr,260,20,str);
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
			if(a/5>0)TextOut(scr,90,60,"Anime Trimmer X",a/5);
			if(a/5-15>0)TextOut(scr,70,90,"Episode7 Eng Sub (1/3)",a/5-15);
			if(a/5-37>0)TextOut(scr,120,120,"uploaded",a/5-37);
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

	for(int i=0 ; i<320 ; i++)for(int j=0 ; j<240 ; j++){
		getRGB(img.back->RGB[(j+720)*(img.back->w)+(i+320)],&col.r,&col.g,&col.b);
		pixel3 = *(Uint32*)(px3 + pitch3*j + bypp3*i);
		SDL_GetRGB(pixel3,f3,&col3.r,&col3.g,&col3.b);
		if(col.r==255 && col.g==0 && col.b==0){
			mirror->RGB[j*320+i]=setRGB(255,255,255);
		}
		else if(col.r==0){
			mirror->RGB[(j+240)*320+i]=setRGB(col3.r,col3.g,col3.b);
		}
		else if(col.r==64){
			mirror->RGB[(j+480)*320+i]=setRGB(col3.r,col3.g,col3.b);
		}
		else if(col.r==128){
			mirror->RGB[(j+720)*320+i]=setRGB(col3.r,col3.g,col3.b);
		}
		else if(col.r==192){
			mirror->RGB[(j+960)*320+i]=setRGB(col3.r,col3.g,col3.b);
		}
		else if(col.r==255){
			mirror->RGB[(j+1200)*320+i]=setRGB(col3.r,col3.g,col3.b);
		}
	}

	SDL_UnlockSurface(scr);
}
