#include "gamemenu.h"

void keyGameMainMenu();
void keyGameSeason();
void keySaveData();
void keyRecordMenu();
void keyGameRecord();
void keyPrologue();
void keyNoData();

void initGameMenu(){
	mode=GAMEMENU;
	menu[GAMEMODE].setMenu(160,120,10,8,8);
	menu[GAMEMODE].stack(text[MENUTEXT+5]);
	menu[GAMEMODE].stack(text[MENUTEXT+6]);
	menu[GAMEMODE].stack(text[MENUTEXT+7]);
	menu[GAMEMODE].stack(text[MENUTEXT+8]);
	menu[GAMEMODE].stack(text[MENUTEXT+9]);
	menu[GAMEMODE].stack(text[MENUTEXT+10]);
	menu[GAMEMODE].stack(text[MENUTEXT+11]);
	menu[GAMEMODE].stack(text[MENUTEXT+12]);
	phase=MAIN;
	count=-1;start=20;
	img.boss=NULL;
	getImage(img.back,"file/img/gamemenu.png",0,0,255);
	bgm=Mix_LoadMUS("file/bgm/3.ogg");
	sf.thunder=Mix_LoadWAV("file/se/14.wav");
	sf.swish=Mix_LoadWAV("file/se/25.wav");
	Mix_PlayMusic(bgm,-1);
}

void initPrologue(){
	loadCartoon("file/data/cartoon/story1.json");
	phase=PROLOGUE;
	kick_count++;
}

void initKomoro(){
	freeMusic();
	bgm=Mix_LoadMUS("file/bgm/4.ogg");
	freeImage(img.back);
	getImage(img.back,"file/img/komoro.png",0,0,255);
	for(int i=0 ; i<14 ; i++){
		sprintf_s(str,"file/img/komoro/%d.jpg",i+1);
		getImage(img.photo[i],str,0,0,0);
	}
	load_story(13);
	TalkingAt(0);
	phase=KOMORO;
	gd.scene_count=0;
	start=100;
	kick_count++;
}

void endGameMenu(){
	if(phase==RECORD)fishbox.endFishBox();
	if(phase==KOMORO){
		for(int i=0 ; i<14 ; i++)freeImage(img.photo[i]);
	}
	if(img.boss!=NULL)freeImage(img.boss);
	freeImage(img.back);
	if(phase!=GOTO_GAME || dataNo!=index_num+1){
		freeMusic();
	}
	Mix_FreeChunk(sf.thunder);
	Mix_FreeChunk(sf.swish);
	for(int i=0 ; i<15 ; i++)menu[i].reset();
}

void gotoRecord(){
	createMap();
	createMap_color(1000);
	phase=RECORDMENU;
	menu[RECORDMENU].setViewMode(VISIBLE);
}

void gotoGame(){
	bool c=false;
	int n=0;
	if(gd.mainmenu_selected==1){
		c=true;
		n=menu[SAVEDATA].selected();
	}
	if(dataNo==index_num+1)gd.game_mode=BOSS;
	else gd.game_mode=STORYMODE;
	endGameMenu();
	initGame();
	load_story(dataNo);
	if(c){
		load_game(n);
		phase=READY;
	}
	if((dataNo-1)%4==1){gd.sunrise_hour=5;gd.sunset_hour=19;}
	else if((dataNo-1)%4==3){gd.sunrise_hour=7;gd.sunset_hour=17;}
	else {gd.sunrise_hour=6;gd.sunset_hour=18;}
	if(dataNo==index_num+1)gd.hour=12;//last stage
	initGame2();
	head_of_talking();
	head_of_timeslot();
	gd.talk_count=EOF;
	kick_count=1;
}

void timerSeaSide(){
	if(dataNo==index_num+1 && gd.scene_count==1 && start==1)Mix_PlayMusic(bgm,-1);
	if(face[gd.face_count]==EOF){
		if(start==99){
			Mix_PlayChannel(0,sf.swish,0);
		}
		if(start==1){
			if(movie_test){
				endGameMenu();
				initMiyazaki();
				start=0;
				gd.x=1600;gd.y=1280;
			}else{
				phase=GOTO_GAME;
				freeImage(img.back);
				getImage(img.back,"file/img/back.png",0,0,255);
				freeMusic();
				count=0;
			}
		}
	}
}

void timerKomoro(){
	if(gd.scene_count==6){
		if(count==200)Mix_PlayChannel(0,sf.decide,0);
	}
	if(face[gd.face_count]==EOF && start==1){
		if(movie_test){
			endGameMenu();
			initMiyazaki();
			phase=MIYAZAKI_MUSEUM;
			start=0;
			gd.x=1600;gd.y=1280;
			for(int i=0 ; i<32 ; i++)menu[BGM_TEST].cursorDown();
		}else{
			phase=GOTO_GAME;
			freeImage(img.back);
			getImage(img.back,"file/img/back.png",0,0,255);
			for(int i=0 ; i<14 ; i++)freeImage(img.photo[i]);
			freeMusic();
			count=0;
		}
	}
}

void timerPrologue(){
	if(nextCut()){
		if(movie_test){
			endGameMenu();
			initMiyazaki();
			gd.x=1600;
			gd.scrX=(int)gd.x-320;
			phase=MIYAZAKI_MUSEUM;
			start=0;
			for(int i=0 ; i<30 ; i++)menu[BGM_TEST].cursorDown();
		}else{
			phase=GOTO_GAME;
			freeImage(img.back);
			getImage(img.back,"file/img/back.png",0,0,255);
			freeMusic();
			count=0;
		}
	}
}

void timerGameMenu(){
	if(phase==GOTO_GAME && count==1)gotoGame();
	else if(phase==GOTO_RECORD && count==1)gotoRecord();
	if(phase==PROLOGUE || phase==SEASIDE || phase==KOMORO){
	}else{
		controlTextCount(false);
	}
	if(phase==PROLOGUE)timerPrologue();
	if(phase==SEASIDE)timerSeaSide();
	if(phase==KOMORO)timerKomoro();
}

void keyGameMenu(){
	switch(phase){
		case MAIN:keyGameMainMenu();break;
		case SEASON:keyGameSeason();break;
		case SAVEDATA:keySaveData();break;
		case RECORDMENU:keyRecordMenu();break;
		case RECORD:keyGameRecord();break;
		case PROLOGUE:
		case SEASIDE:
		case KOMORO:keyPrologue();break;
		case NODATA:keyNoData();break;
		default:break;
	}
}

void keyGameMainMenu(){
	if(key.z && !key_stop(key.z)){
		Mix_PlayChannel(1, sf.decide2, 0);
		if(gd.mainmenu_selected==0){
			gd.scrX=(clear_num-1)/4;gd.scrY=(clear_num-1)%4;
			phase=SEASON;
			start=30;
			if(gd.scrY==0)fishbox.panelColor(255,96,96);
			else if(gd.scrY==1)fishbox.panelColor(0,128,0);
			else if(gd.scrY==2)fishbox.panelColor(128,64,0);
			else fishbox.panelColor(96,96,255);
		}
		if(gd.mainmenu_selected==1){
			if(makeSaveMenu(SAVEDATA)){
				for(int i=0 ; i<gd.save_cursor ; i++)menu[SAVEDATA].cursorDown();
				menu[SAVEDATA].setViewMode(VISIBLE);
				phase=SAVEDATA;
			}
			else phase=NODATA;
		}
		if(gd.mainmenu_selected==2){
			if(makeRecordMenu(RECORDMENU)){
				phase=GOTO_RECORD;
				count=0;
			}
			else phase=NODATA;
		}
		if(gd.mainmenu_selected==3){
			endGameMenu();
			initMiyazaki();
		}
	}
	if(key.x && !key_stop(key.x)){
		endGameMenu();
		initTitle2();
		kick_count=1;
	}
	if(key.left && !key_wait(key.left)){
		if(gd.mainmenu_selected==0)gd.mainmenu_selected=3;
		else gd.mainmenu_selected--;
	}
	if(key.right && !key_wait(key.right)){
		if(gd.mainmenu_selected==3)gd.mainmenu_selected=0;
		else gd.mainmenu_selected++;
	}
}

void keyGameSeason(){
	if(start!=0)return;
	if(key.z && !key_stop(key.z)){
		count=0;
		dataNo=gd.scrX*4+gd.scrY+1;
		if(dataNo==1){
			initPrologue();
		}
		else if(dataNo==13){
			initKomoro();
		}else{
			freeMusic();
			bgm=Mix_LoadMUS("file/bgm/4.ogg");
			freeImage(img.back);
			getImage(img.back,"file/img/shore.png",0,0,255);
			if(dataNo==index_num+1)load_story(25);
			else load_story(dataNo);
			if(dataNo==1)TalkingAt(1);
			else TalkingAt(0);
			phase=SEASIDE;
			start=100;
			kick_count++;
		}
	}
	if(key.x && !key_stop(key.x)){
		phase=MAIN;
		menu[SEASON].setViewMode(HIDE);
	}
	if(key.up && !key_stop(key.up)){
		if(gd.scrY>0)gd.scrY--;
	}
	if(key.down && !key_stop(key.down)){
		if(gd.scrX<(clear_num-1)/4 && gd.scrY<3)gd.scrY++;
		else if(gd.scrY<(clear_num-1)%4)gd.scrY++;
	}
	if(key.left && !key_stop(key.left)){
		if(gd.scrX>0){
			gd.scrX--;
			start-=32;
		}
	}
	if(key.right && !key_stop(key.right)){
		if(gd.scrX<(clear_num-1)/4){
			gd.scrX++;
			if(gd.scrX*4+gd.scrY>=clear_num)gd.scrY=(clear_num-1)%4;
			start=32;
		}
	}
	if((key.up||key.down||key.right) && (!key_stop(key.up) || !key_stop(key.down) || !key_stop(key.right))){
		if(gd.scrY==0)fishbox.panelColor(255,96,96);
		else if(gd.scrY==1)fishbox.panelColor(0,128,0);
		else if(gd.scrY==2)fishbox.panelColor(128,64,0);
		else fishbox.panelColor(96,96,255);
	}
}

void keySaveData(){
	if(key.z && !key_stop(key.z)){
		count=0;
		gd.save_cursor=menu[SAVEDATA].selected();
		phase=GOTO_GAME;
	}
	if(key.x && !key_stop(key.x)){
		menu[SAVEDATA].setViewMode(HIDE);
		phase=MAIN;
	}
	if(key.up && !key_wait(key.up))menu[SAVEDATA].cursorUp();
	if(key.down && !key_wait(key.down))menu[SAVEDATA].cursorDown();
}

void keyRecordMenu(){
	if(key.z && !key_stop(key.z)){
		fillRect(img.buffer[1],0,0,640,480,0,0,0,255);
		int n=menu[RECORDMENU].selected();
		load_record(n);
		gd.scrX=fishbox.getX()-320;
		gd.scrY=fishbox.getY()-240;
		map.buffered=false;
		phase=RECORD;
		menu[RECORDMENU].setViewMode(GRAY);
	}
	if(key.x && !key_stop(key.x)){
		menu[RECORDMENU].setViewMode(HIDE);
		phase=MAIN;
	}
	if(key.up && !key_wait(key.up))menu[RECORDMENU].cursorUp();
	if(key.down && !key_wait(key.down))menu[RECORDMENU].cursorDown();
}

void keyGameRecord(){
	if(key.x && !key_stop(key.x)){
		fishbox.endFishBox();
		phase=RECORDMENU;
		menu[RECORDMENU].setViewMode(VISIBLE);
	}
	int preX=fishbox.getX(), preY=fishbox.getY();
	if(key.up && !key_wait(key.up))fishbox.cursorUp();
	if(key.down && !key_wait(key.down))fishbox.cursorDown();
	if(key.left && !key_wait(key.left))fishbox.cursorLeft();
	if(key.right && !key_wait(key.right))fishbox.cursorRight();
	if(preX!=fishbox.getX() || preY!=fishbox.getY()){
		gd.scrX=fishbox.getX()-320;
		gd.scrY=fishbox.getY()-240;
		map.buffered=false;
	}
}

void keyPrologue(){
	if(key.z && !key_stop(key.z)){
		nextTalk();
	}
/*
	if(key.z && !key_stop(key.z) && start==0){
		int a=controlTalking();
		if(a==COMMA){
			count=2;
			if(phase==PROLOGUE && gd.scene_count==1)start=100;
			if(phase==KOMORO && gd.scene_count==4)start=20;
			if(dataNo==index_num+1){//最終ボス
				if(gd.scene_count==1){
					freeMusic();
					bgm=Mix_LoadMUS("file/bgm/14.ogg");
					Mix_PlayChannel(0,sf.thunder,0);
					start=100;
				}
				else if(gd.scene_count==2){
					getImage(img.boss,"file/img/warning.gif",0,0,255);
					start=200;
				}
			}
		}
		else if(a==EOF){
			if(phase==KOMORO)start=200;
			else start=100;
		}
	}
*/
	if(key.c && !key_stop(key.c)){
		if(movie_test){
			endGameMenu();
			initMiyazaki();
			gd.x=1600;
			gd.scrX=(int)gd.x-320;
			phase=MIYAZAKI_MUSEUM;
			start=0;
			for(int i=0 ; i<30 ; i++)menu[BGM_TEST].cursorDown();
		}else{
			if(phase==PROLOGUE){
				freeMusic();
				bgm=Mix_LoadMUS("file/bgm/4.ogg");
				freeImage(img.back);
				getImage(img.back,"file/img/shore.png",0,0,255);
				TalkingAt(1);
				phase=SEASIDE;
				start=100;
			}else{
				freeImage(img.back);
				getImage(img.back,"file/img/back.png",0,0,255);
				if(phase==KOMORO){
					for(int i=0 ; i<14 ; i++)freeImage(img.photo[i]);
				}
				if(dataNo==index_num+1){
					freeMusic();
					bgm=Mix_LoadMUS("file/bgm/14.ogg");
					Mix_PlayMusic(bgm,-1);
				}
				phase=GOTO_GAME;
			}
			count=0;
		}
	}
}

void keyNoData(){
	if(key.left || key.right)phase=MAIN;
	keyGameMainMenu();
}

void drawGameMainMenuBox(SDL_Surface* scr){
	if(start<15)drawImage(scr,img.chr,60,200,240,760,80,80,255);
	if(start<10)drawImage(scr,img.chr,180,200,320,600,80,80,255);
	if(start<5)drawImage(scr,img.chr,300,200,240,600,80,80,255);
	if(start==0)drawImage(scr,img.chr,420,200,560,680,80,80,255);
	drawImage(scr,img.chr,gd.mainmenu_selected*120+60,200,320,300+((count/20)%2)*80,80,80,255);
	drawText2(scr,120,320,text[MENUTEXT+gd.mainmenu_selected]);
}

void drawSeaSide(SDL_Surface* scr){
	int a=0,b=0,c=0,d=0;
	if(dataNo==index_num+1){//最終ボス
		if(start>0 && gd.scene_count==1)a=10-2*abs(10-start%20);
		if(gd.scene_count==2){
			c=(int)((200-start)*1.7);
			d=start;
		}
	}
	if(mode==GAMEMENU && start>0 && gd.scene_count==0 && face[gd.face_count]!=EOF)b=start*4;
	if(mode==ENDING && face[gd.face_count]==EOF)b=400-start*4;
	if(dataNo==index_num+1)fillRect(scr,0,0,640,480,192,272,432,255);
	else fillRect(scr,0,0,640,480,0,0,0,255);
	if(img.boss!=NULL){
		drawImage(scr,img.boss,400,40+d,((bg_count/5)%8)*220,(((bg_count/5)/8)%2)*410,220,410,255);
	}
	drawImage(scr,img.back,a-20,40,0,340-c,680,400,192);
	drawImage(scr,img.back,a-20,180+c,0,540,680,260,255);
	drawImage(scr,img.back,a-b-20,220+c-2*abs(20-(bg_count/5)%40),760,300,490,180,255);
	drawImage(scr,img.back,a-b-20,220+c-2*abs(20-(bg_count/5)%40),760,480,490,180,255);
	if(mode==GAMEMENU && face[gd.face_count]==EOF)drawImage(scr,img.back,a+200-b,-560+start*8+c,880,660,20,140,255);
	else if(mode==ENDING && start>0 && gd.scene_count==0 && face[gd.face_count]!=EOF)drawImage(scr,img.back,a+200-b,240-start*8+c,880,660,20,140,255);
	else drawImage(scr,img.back,a+160-b,240+c-2*abs(20-(bg_count/5)%40),760,660,120,140,255);
	drawImage(scr,img.back,a-b-20,220+c-2*abs(20-(bg_count/5)%40),760,0,490,300,255);
	if(gd.scene_count==1){
		if(dataNo==1 && face[gd.face_count]!=EOF)drawImage(scr,img.back,-b+60,160-2*abs(20-(bg_count/5)%40),0,1040,320,160,255);
	}
	fillRect(scr,0,0,640,40,0,0,0,255);
	fillRect(scr,0,440,640,40,0,0,0,255);
	if(start==0)drawTalking(scr);
}

void drawGameRecord(SDL_Surface* scr){
	SDL_Color col=getSkyColor(fishbox.getH(),fishbox.getM());
	fillRect(scr,0,0,640,480,col.r,col.g,col.b,255);
	drawMap(scr,fishbox.getX()-320,fishbox.getY()-240);
	sprintf_s(str,"%10d",gd.score);
	drawText(scr,320,0,str);
	fishbox.drawFishBox(scr);
}

void drawGamemenuExplain(SDL_Surface* scr){
	if(EXPLAIN){
		if(phase==PROLOGUE || phase==SEASIDE || phase==KOMORO){
			drawKeyboard(scr,key.zC,0,0);
			drawText(scr,20,0,text[EPILOGUE+1]);
			drawKeyboard(scr,key.cC,400,0);
			drawText(scr,420,0,text[EPILOGUE]);
		}
		else if(phase==RECORD){
			if(count%400<200){
				drawKeyboard(scr,key.xC,0,0);
				drawText(scr,20,0,text[MENUTEXT+4]);
			}else{
				drawKeyboard(scr,key.upC,0,0);
				drawKeyboard(scr,key.downC,20,0);
				drawKeyboard(scr,key.leftC,40,0);
				drawKeyboard(scr,key.rightC,60,0);
				drawText(scr,80,0,text[MENUTEXT+15]);
			}
		}else{
			if(count%600<200){
				drawKeyboard(scr,key.zC,0,0);
				drawText(scr,20,0,text[OPTIONTEXT+1]);
			}
			else if(count%600<400){
				drawKeyboard(scr,key.xC,0,0);
				drawText(scr,20,0,text[MENUTEXT+4]);
			}else{
				drawKeyboard(scr,key.leftC,0,0);
				drawKeyboard(scr,key.rightC,20,0);
				if(phase==SEASON){
					drawKeyboard(scr,key.upC,40,0);
					drawKeyboard(scr,key.downC,60,0);
					drawText(scr,80,0,text[MENUTEXT+15]);
				}else{
					drawText(scr,40,0,text[MENUTEXT+15]);
				}
			}
		}
	}
}

void drawGameMenu(SDL_Surface* scr){
	if(phase==RECORD)drawGameRecord(scr);
	else if(phase==PROLOGUE)drawAnimationCut(scr);
	else if(phase==SEASIDE)drawSeaSide(scr);
	else if(phase==KOMORO)drawKomoro(scr);
	else{
		fillRect(scr,0,0,640,480,0,0,0,255);
		drawImage(scr,img.back,0,0,640+(count/2)%600,(count/2)%600,640,480,255);
		drawImage(scr,img.back,0,320,0,((count/10)%4)*160,640,160,255);
		if(phase==MAIN || phase==NODATA)drawGameMainMenuBox(scr);
		for(int i=0 ; i<5 ; i++)menu[i].drawMenu(scr);
		if(phase==SEASON){
			for(int k=-1 ; k<=1 ; k++){
				if(gd.scrX+k<0)continue;
				drawImage(scr,fishbox.panel,120+k*640+start*20,40,0,0,200,400,255);
				drawImage(scr,fishbox.panel,320+k*640+start*20,40,440,0,200,400,255);
				drawImage(scr,img.back,180+k*640+start*20,60,0,640,280,60,255);
				drawImage(scr,img.chr,140+k*640+start*20,120+gd.scrY*80,280,240,40,40,255);
				for(int i=0 ; i<4 ; i++){
					if((gd.scrX+k)*4+i>=clear_num)break;
					if((gd.scrX+k)*4+i==index_num){
						drawText2(scr,180+k*640+start*20,120+i*80,text[MENUTEXT+19]);
					}else{
						drawText2(scr,180+k*640+start*20,120+i*80,indexName[(gd.scrX+k)*4+i].name);
						sprintf_s(str,"%3d%c",indexName[(gd.scrX+k)*4+i].rate,37);
						drawText2(scr,360+k*640+start*20,120+i*80,str);
						sprintf_s(str,"%10d",indexName[(gd.scrX+k)*4+i].hiscore);
						drawText2(scr,266+k*640+start*20,154+i*80,str);
						if(indexName[(gd.scrX+k)*4+i].rate==100)drawImage(scr,img.symbol,440+k*640+start*20,120+i*80,102,0,34,34,255);
						else if(indexName[(gd.scrX+k)*4+i].rate>=80)drawImage(scr,img.symbol,440+k*640+start*20,120+i*80,64,0,34,34,255);
						else if(indexName[(gd.scrX+k)*4+i].rate>=60)drawImage(scr,img.symbol,440+k*640+start*20,120+i*80,34,0,34,34,255);
					}
				}
			}
			if(start==0 && count%40<20){
				if(gd.scrX>0)drawImage(scr,img.back,0,280,280,640,60,40,255);
				if(gd.scrX<(clear_num-1)/4)drawImage(scr,img.back,580,260,340,640,60,40,255);
			}
		}
	}
	if(phase==GOTO_GAME || phase==GOTO_RECORD){
		fillRect(scr,0,0,640,480,0,0,0,255);
		drawImage(scr,img.menuback,0,180,0,0,640,120,128);
		drawText2(scr,240,200,text[MENUTEXT+16]);
		drawText2(scr,100,240,text[MENUTEXT+17]);
	}
	if(phase==NODATA)drawText2(scr,100,400,text[MENUTEXT+18]);
	drawGamemenuExplain(scr);
}

bool makeSaveMenu(int n){
	FILE* hf;
	int a=0;
	while(true){
		sprintf_s(str,"save/save%d.dat",a);
		fopen_s(&hf,str,"rb");
		if(hf==NULL)break;
		fclose(hf);
		a++;
	}
	String s;
	menu[n].setMenu(80,100,22,8,a+1);
	for(int i=0 ; i<a ; i++){
		sprintf_s(str,"save/save%d.dat",i);
		if(!loadFile(str))break;
		for(int k=0 ; k<2 ; k++){
			sprintf_s(s.str[k],"%s (%s) %2d:",indexName[fstr[0]].name.str[k],weekChar[fstr[3]][k],fstr[4]);
			if(fstr[5]<10)sprintf_s(s.str[k],"%s0%d",s.str[k],fstr[5]);
			else sprintf_s(s.str[k],"%s%2d",s.str[k],fstr[5]);
		}
		menu[n].stack(s);
	}
	if(a==0)return false;
	else return true;
}

bool makeRecordMenu(int n){
	FILE* hf;
	int a=0;
	while(true){
		sprintf_s(str,"save/record%d.dat",a);
		fopen_s(&hf,str,"rb");
		if(hf==NULL)break;
		fclose(hf);
		a++;
	}
	int sc;
	String s;
	menu[n].setMenu(80,100,22,8,a+1);
	for(int i=0 ; i<a ; i++){
		sc=0;
		sprintf_s(str,"save/record%d.dat",i);
		if(!loadFile(str))break;
		for(int i=17 ; i<(int)fsize ; i+=18){
			sc+=to16int(fstr[i],fstr[i+1]);
		}
		sprintf_s(s.str[0],"%s %10d",indexName[fstr[0]].name.str[0],sc);
		sprintf_s(s.str[1],"%s %10d",indexName[fstr[0]].name.str[1],sc);
		menu[n].stack(s);
	}
	if(a==0)return false;
	else return true;
}
