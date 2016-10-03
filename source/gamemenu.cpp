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
	menu[GAMEMODE].setMenu(80,60,10,8,8);
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
	Mix_FreeMusic(bgm);
	bgm=Mix_LoadMUS("file/bgm/15.ogg");
	freeImage(img.back);
	getImage(img.back,"file/img/prologue.png",0,0,255);
	load_story(1);
	TalkingAt(0);
	phase=PROLOGUE;
	start=100;
	kick_count++;
}

void initKomoro(){
	Mix_FreeMusic(bgm);
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
		if(bgm!=NULL){
			Mix_FreeMusic(bgm);
			bgm=NULL;
		}
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
	BOOL c=FALSE;
	int n;
	if(gd.mainmenu_selected==1){
		c=TRUE;
		n=menu[SAVEDATA].selected();
	}
	if(dataNo==index_num+1)gd.game_mode=BOSS;
	else gd.game_mode=STORYMODE;
	endGameMenu();
	initGame();
	if(c){
		load_game(n);
		load_story(dataNo);
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
				gd.x=800;gd.y=640;
			}else{
				phase=GOTO_GAME;
				freeImage(img.back);
				getImage(img.back,"file/img/back.png",0,0,255);
				Mix_FreeMusic(bgm);
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
			gd.x=800;gd.y=640;
			for(int i=0 ; i<32 ; i++)menu[BGM_TEST].cursorDown();
		}else{
			phase=GOTO_GAME;
			freeImage(img.back);
			getImage(img.back,"file/img/back.png",0,0,255);
			for(int i=0 ; i<14 ; i++)freeImage(img.photo[i]);
			Mix_FreeMusic(bgm);
			count=0;
		}
	}
}

void timerPrologue(){
	if(face[gd.face_count]==EOF && start==1){
		if(movie_test){
			endGameMenu();
			initMiyazaki();
			gd.x=800;
			gd.scrX=(int)gd.x-160;
			phase=MIYAZAKI_MUSEUM;
			start=0;
			for(int i=0 ; i<30 ; i++)menu[BGM_TEST].cursorDown();
		}else{
			Mix_FreeMusic(bgm);
			bgm=Mix_LoadMUS("file/bgm/4.ogg");
			freeImage(img.back);
			getImage(img.back,"file/img/shore.png",0,0,255);
			TalkingAt(1);
			phase=SEASIDE;
			start=100;
			count=0;
		}
	}
}

void timerGameMenu(){
	if(phase==GOTO_GAME && count==1)gotoGame();
	else if(phase==GOTO_RECORD && count==1)gotoRecord();
	if(phase==PROLOGUE || phase==SEASIDE || phase==KOMORO){
		if(start==0)controlTextCount(TRUE);
		else controlTextCount(FALSE);
		if(count==1)Mix_PlayMusic(bgm,-1);
	}else{
		controlTextCount(FALSE);
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
			Mix_FreeMusic(bgm);
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
		fillRect(img.buffer[1],0,0,320,240,0,0,0,255);
		int n=menu[RECORDMENU].selected();
		load_record(n);
		gd.scrX=fishbox.getX()-160;
		gd.scrY=fishbox.getY()-120;
		map.buffered=FALSE;
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
		gd.scrX=fishbox.getX()-160;
		gd.scrY=fishbox.getY()-120;
		map.buffered=FALSE;
	}
}

void keyPrologue(){
	if(key.z && !key_stop(key.z) && start==0){
		int a=controlTalking();
		if(a==COMMA){
			count=2;
			if(phase==PROLOGUE && gd.scene_count==1)start=100;
			if(phase==KOMORO && gd.scene_count==4)start=20;
			if(dataNo==index_num+1){//最終ボス
				if(gd.scene_count==1){
					Mix_FreeMusic(bgm);
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
	if(key.c && !key_stop(key.c)){
		if(movie_test){
			endGameMenu();
			initMiyazaki();
			gd.x=800;
			gd.scrX=(int)gd.x-160;
			phase=MIYAZAKI_MUSEUM;
			start=0;
			for(int i=0 ; i<30 ; i++)menu[BGM_TEST].cursorDown();
		}else{
			if(phase==PROLOGUE){
				Mix_FreeMusic(bgm);
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
					Mix_FreeMusic(bgm);
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
	if(start<15)drawImage(scr,img.chr,30,100,120,380,40,40,255);
	if(start<10)drawImage(scr,img.chr,90,100,160,300,40,40,255);
	if(start<5)drawImage(scr,img.chr,150,100,120,300,40,40,255);
	if(start==0)drawImage(scr,img.chr,210,100,280,340,40,40,255);
	drawImage(scr,img.chr,gd.mainmenu_selected*60+30,100,160,150+((count/20)%2)*40,40,40,255);
	TextOut(scr,60,160,text[MENUTEXT+gd.mainmenu_selected]);
}

void drawSeaSide(SDL_Surface* scr){
	int a=0,b=0,c=0,d=0;
	if(dataNo==index_num+1){//最終ボス
		if(start>0 && gd.scene_count==1)a=5-abs(10-start%20);
		if(gd.scene_count==2){
			c=(int)((200-start)*0.85);
			d=start;
		}
	}
	if(mode==GAMEMENU && start>0 && gd.scene_count==0 && face[gd.face_count]!=EOF)b=start*2;
	if(mode==ENDING && face[gd.face_count]==EOF)b=200-start*2;
	if(dataNo==index_num+1)fillRect(scr,0,0,320,240,96,136,216,255);
	else fillRect(scr,0,0,320,240,0,0,0,255);
	if(img.boss!=NULL){
		drawImage(scr,img.boss,200,20+d,((count/5)%8)*110,(((count/5)/8)%2)*205,110,205,255);
	}
	drawImage(scr,img.back,a-10,20,0,170-c,340,200,192);
	drawImage(scr,img.back,a-10,90+c,0,270,340,130,255);
	drawImage(scr,img.back,a-b-10,110+c-abs(20-(count/5)%40),380,150,245,90,255);
	drawImage(scr,img.back,a-b-10,110+c-abs(20-(count/5)%40),380,240,245,90,255);
	if(mode==GAMEMENU && face[gd.face_count]==EOF)drawImage(scr,img.back,a+100-b,-280+start*4+c,440,330,10,70,255);
	else if(mode==ENDING && start>0 && gd.scene_count==0 && face[gd.face_count]!=EOF)drawImage(scr,img.back,a+100-b,120-start*4+c,440,330,10,70,255);
	else drawImage(scr,img.back,a+80-b,120+c-abs(20-(count/5)%40),380,330,60,70,255);
	drawImage(scr,img.back,a-b-10,110+c-abs(20-(count/5)%40),380,0,245,150,255);
	if(gd.scene_count==1){
		if(dataNo==1 && face[gd.face_count]!=EOF)drawImage(scr,img.back,-b+30,80-abs(20-(count/5)%40),0,520,160,80,255);
	}
	fillRect(scr,0,0,320,20,0,0,0,255);
	fillRect(scr,0,220,320,20,0,0,0,255);
	if(start==0)drawTalking(scr);
}

void drawGameRecord(SDL_Surface* scr){
	SDL_Color col=getSkyColor(fishbox.getH(),fishbox.getM());
	fillRect(scr,0,0,320,240,col.r,col.g,col.b,255);
	drawMap(scr,fishbox.getX()-160,fishbox.getY()-120);
	sprintf_s(str,"%10d",gd.score);
	TextOut(scr,160,0,str);
	fishbox.drawFishBox(scr);
}

void drawGamemenuExplain(SDL_Surface* scr){
	if(EXPLAIN){
		if(phase==PROLOGUE || phase==SEASIDE || phase==KOMORO){
			drawKeyboard(scr,key.zC,0,0);
			TextOut(scr,20,0,text[EPILOGUE+1]);
			drawKeyboard(scr,key.cC,200,0);
			TextOut(scr,220,0,text[EPILOGUE]);
		}
		else if(phase==RECORD){
			if(count%400<200){
				drawKeyboard(scr,key.xC,0,0);
				TextOut(scr,20,0,text[MENUTEXT+4]);
			}else{
				drawKeyboard(scr,key.upC,0,0);
				drawKeyboard(scr,key.downC,20,0);
				drawKeyboard(scr,key.leftC,40,0);
				drawKeyboard(scr,key.rightC,60,0);
				TextOut(scr,80,0,text[MENUTEXT+15]);
			}
		}else{
			if(count%600<200){
				drawKeyboard(scr,key.zC,0,0);
				TextOut(scr,20,0,text[OPTIONTEXT+1]);
			}
			else if(count%600<400){
				drawKeyboard(scr,key.xC,0,0);
				TextOut(scr,20,0,text[MENUTEXT+4]);
			}else{
				drawKeyboard(scr,key.leftC,0,0);
				drawKeyboard(scr,key.rightC,20,0);
				if(phase==SEASON){
					drawKeyboard(scr,key.upC,40,0);
					drawKeyboard(scr,key.downC,60,0);
					TextOut(scr,80,0,text[MENUTEXT+15]);
				}else{
					TextOut(scr,40,0,text[MENUTEXT+15]);
				}
			}
		}
	}
}

void drawGameMenu(SDL_Surface* scr){
	if(phase==RECORD)drawGameRecord(scr);
	else if(phase==PROLOGUE)drawPrologue(scr);
	else if(phase==SEASIDE)drawSeaSide(scr);
	else if(phase==KOMORO)drawKomoro(scr);
	else{
		fillRect(scr,0,0,320,240,0,0,0,255);
		drawImage(scr,img.back,0,0,320+(count/2)%300,0+(count/2)%300,320,240,255);
		drawImage(scr,img.back,0,160,0,((count/10)%4)*80,320,80,255);
		if(phase==MAIN || phase==NODATA)drawGameMainMenuBox(scr);
		for(int i=0 ; i<5 ; i++)menu[i].drawMenu(scr);
		if(phase==SEASON){
			for(int k=-1 ; k<=1 ; k++){
				if(gd.scrX+k<0)continue;
				drawImage(scr,fishbox.panel,60+k*320+start*10,20,0,0,100,200,255);
				drawImage(scr,fishbox.panel,160+k*320+start*10,20,220,0,100,200,255);
				drawImage(scr,img.back,90+k*320+start*10,30,0,320,140,30,255);
				drawImage(scr,img.chr,70+k*320+start*10,60+gd.scrY*40,140,120,20,20,255);
				for(int i=0 ; i<4 ; i++){
					if((gd.scrX+k)*4+i>=clear_num)break;
					if((gd.scrX+k)*4+i==index_num){
						TextOut(scr,90+k*320+start*10,60+i*40,text[MENUTEXT+19]);
					}else{
						TextOut(scr,90+k*320+start*10,60+i*40,indexName[(gd.scrX+k)*4+i].name);
						sprintf_s(str,"%3d%c",indexName[(gd.scrX+k)*4+i].rate,37);
						TextOut(scr,180+k*320+start*10,60+i*40,str);
						sprintf_s(str,"%10d",indexName[(gd.scrX+k)*4+i].hiscore);
						TextOut(scr,133+k*320+start*10,77+i*40,str);
						if(indexName[(gd.scrX+k)*4+i].rate==100)drawImage(scr,img.symbol,220+k*320+start*10,60+i*40,51,0,17,17,255);
						else if(indexName[(gd.scrX+k)*4+i].rate>=80)drawImage(scr,img.symbol,220+k*320+start*10,60+i*40,34,0,17,17,255);
						else if(indexName[(gd.scrX+k)*4+i].rate>=60)drawImage(scr,img.symbol,220+k*320+start*10,60+i*40,17,0,17,17,255);
					}
				}
			}
			if(start==0 && count%40<20){
				if(gd.scrX>0)drawImage(scr,img.back,0,140,140,320,30,20,255);
				if(gd.scrX<(clear_num-1)/4)drawImage(scr,img.back,290,130,170,320,30,20,255);
			}
		}
	}
	if(phase==GOTO_GAME || phase==GOTO_RECORD){
		fillRect(scr,0,0,320,240,0,0,0,255);
		drawImage(scr,img.menuback,0,90,0,0,320,60,128);
		TextOut(scr,120,100,text[MENUTEXT+16]);
		TextOut(scr,50,120,text[MENUTEXT+17]);
	}
	if(phase==NODATA)TextOut(scr,50,200,text[MENUTEXT+18]);
	drawGamemenuExplain(scr);
}

BOOL makeSaveMenu(int n){
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
	menu[n].setMenu(40,50,22,8,a+1);
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
	if(a==0)return FALSE;
	else return TRUE;
}

BOOL makeRecordMenu(int n){
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
	menu[n].setMenu(40,50,22,8,a+1);
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
	if(a==0)return FALSE;
	else return TRUE;
}
