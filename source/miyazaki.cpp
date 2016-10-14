#include "util.h"

int *prgs2;
int this_tower[10],which_tower;
bool talk_3dtv=false, talk_seoiha=false,movie_test=false;
bool showSearchImage=false;
Uint8 scr_design=NULL;

void makeGuideBook();
void drawMiyazakiExplain(SDL_Surface *scr);

void initMiyazaki(){
	count=0;start=0;
	mode=MIYAZAKI;
	phase=COME_MIYAZAKI;
	prgs2=NULL;
	gd.x=30;gd.scrX=0;
	gd.talk_count=EOF;
	menu[GUIDE_TOP].setMenu(60,60,26,4,4);
	menu[GUIDE_TOP].stack(text[MIYAZAKITEXT+1]);
	menu[GUIDE_TOP].stack(text[MIYAZAKITEXT+2]);
	menu[GUIDE_TOP].stack(text[MIYAZAKITEXT+3]);
	if(gd.bought[7])menu[GUIDE_TOP].stack(text[MIYAZAKITEXT+4]);
	else menu[GUIDE_TOP].stack("???");
	menu[DELETE_MENU].setMenu(160,160,20,6,6);
	menu[DELETE_MENU].stack(text[TITLETEXT+1]);
	menu[DELETE_MENU].stack(text[MIYAZAKITEXT+32]);
	menu[DELETE_MENU].stack(text[MIYAZAKITEXT+33]);
	menu[DELETE_MENU].stack(text[MENUTEXT+1]);
	menu[DELETE_MENU].stack(text[MENUTEXT+2]);
	menu[DELETE_MENU].stack(text[GAMETEXT+21]);
	menu[YNFORM].setMenu(320,200,12,3,3);
	menu[YNFORM].stack(text[MIYAZAKITEXT]);
	menu[YNFORM].stack("");
	menu[YNFORM].stack("");
	menu[DELETE_YN].setMenu(320,60,12,2,2);
	menu[DELETE_YN].stack(text[MIYAZAKITEXT+29]);
	menu[DELETE_YN].stack(text[MIYAZAKITEXT+28]);
	menu[GUIDE_SEASON].setMenu(100,80,10,8,index_num);
	for(int i=0 ; i<index_num ; i++)menu[GUIDE_SEASON].stack(indexName[i].name);
	menu[BGM_TEST].setMenu(40,120,25,7,37);
	for(int i=1 ; i<=NUM_OF_BGM ; i++){
		sprintf_s(str,"Music No.%2d",i);
		menu[BGM_TEST].stack(str);
	}
	for(int i=0 ; i<16 ; i++){//‰º‚R‚Â‚ÌƒAƒjƒ‚Í•Û—¯
		menu[BGM_TEST].stack(text[MIYAZAKITEXT+52+i]);
	}
	load_story(0);
	getImage(img.back,"file/img/miyazaki.png",0,0,255);
	bgm=Mix_LoadMUS("file/bgm/17.ogg");
	sf.thunder=Mix_LoadWAV("file/se/14.wav");
	sf.swish=Mix_LoadWAV("file/se/25.wav");
	sf.meow=Mix_LoadWAV("file/se/16.wav");
	sf.drag_s=Mix_LoadWAV("file/se/4.wav");
	sf.tub=Mix_LoadWAV("file/se/13.wav");
	sf.noize=Mix_LoadWAV("file/se/23.wav");
	Mix_PlayMusic(bgm,-1);
}

void endMiyazaki(){
	movie_test=false;
	freeMusic();
	Mix_FreeChunk(sf.thunder);
	Mix_FreeChunk(sf.swish);
	Mix_FreeChunk(sf.meow);
	Mix_FreeChunk(sf.drag_s);
	Mix_FreeChunk(sf.tub);
	Mix_FreeChunk(sf.noize);
	freeImage(img.back);
	if(prgs2)delete [] prgs2;
	for(int i=0 ; i<20 ; i++)menu[i].reset();
}

void toMiyazakiMuseum(){
	phase=MIYAZAKI_MUSEUM;
	freeMusic();
	bgm=Mix_LoadMUS("file/bgm/17.ogg");
	Mix_PlayMusic(bgm,-1);
	kick_count=1;
}

void timerSeoiHa(){
	if(count==60){
		phase=MIYAZAKI_TALK;
		gd.talk_open_count=1;
		TalkingAt(18);
		freeMusic();
		if(scr_design==GAMEBOY){
			bgm=Mix_LoadMUS("file/bgm/17.ogg");
			talk_seoiha=false;
			scr_design=NULL;
		}else{
			bgm=Mix_LoadMUS("file/bgm/19.ogg");
			talk_seoiha=true;
			scr_design=GAMEBOY;
		}
		Mix_PlayMusic(bgm,-1);
	}
}

void timerSummerWars_miyazaki(){
	if(count==401)Mix_PlayChannel(0,sf.swish,0);
	if(count>=420 && count<600){
		if(count%10==0)Mix_PlayChannel(0,sf.thunder,0);
	}
	if(count==500)Mix_PlayChannel(1,sf.meow,0);
	if(count==599){
		phase=MIYAZAKI_MUSEUM;
		freeImage(img.back);
		getImage(img.back,"file/img/miyazaki.png",0,0,255);
		Mix_PlayMusic(bgm,-1);
	}
}

void gotoStroll(){
	endMiyazaki();
	initGame();
	gd.hour=12;
	gd.sunrise_hour=6;
	gd.sunset_hour=18;
	initGame2();
	kick_count++;
}

void gotoTowerList(){
	createMap();
	createMap_color(1000);
	createMap_tower();
	gd.scrX=(int)gd.x*MAGNIFY-320;
	gd.scrY=(int)gd.y*MAGNIFY-240;
	map.buffered=false;
	phase=TOWERLIST;
}

void gotoMovieTest(){
	int n=menu[BGM_TEST].selected()-NUM_OF_BGM;
	if(n>=7 && n!=15){
		endMiyazaki();
		movie_test=true;
	}
	if(n<=6){
		gd.week=n;
		freeImage(img.back);
		sprintf_s(str,"file/img/weekly%d.png",n);
		getImage(img.back,str,0,0,255);
		freeMusic();
		sprintf_s(str,"file/bgm/%d.ogg",n+5);
		bgm=Mix_LoadMUS(str);
		Mix_PlayMusic(bgm,-1);
		phase=GAMESTART;
		count=0;start=75;
	}
	else if(n>=7 && n<=10){
		initEnding();
		initMedalAward(n-7);
	}
	else if(n==11){
		initOpening();
		phase=1;count=0;
	}
	else if(n==12){
		initGameMenu();
		initPrologue();
	}
	else if(n==13){
		initEndingAnime();
	}
	else if(n==14){
		initGameMenu();
		initKomoro();
	}
	else if(n==15){
		freeImage(img.back);
		getImage(img.back,"file/img/epilogue.png",0,0,255);
		phase=SUMMERWARS;
		count=400;
	}
	else if(n==16){
		initLastStory();
	}
	else if(n==17){
		initLastStory2();
	}
	else if(n==18){
		initLastEnding();
	}
	kick_count=0;
}

void timerMiyazaki(){
	if(phase==GOTO_STROLL && count==1)gotoStroll();
	else if(phase==GOTO_TOWERLIST && count==1)gotoTowerList();
	else if(phase==GOTO_MOVIE && count==100)gotoMovieTest();
	else if(phase==COME_MIYAZAKI){
		if(count==100)Mix_PlayChannel(0,sf.drag_s,0);
		if(count==120)Mix_PlayChannel(1,sf.tub,0);
		if(count==150){
			phase=MIYAZAKI_MUSEUM;
			start=200;
		}
	}
	else if(phase==LEAVE_MIYAZAKI){
		if(count==0)Mix_PlayChannel(0,sf.decide,0);
		if(count==30)Mix_PlayChannel(0,sf.noize,0);
		if(count==60)Mix_PlayChannel(0,sf.tub,0);
		if(count==150){
			endMiyazaki();
			initGameMenu();
		}
	}
	else if(phase==SUMMERWARS)timerSummerWars_miyazaki();
	else if(phase==SEOI_HA)timerSeoiHa();
	if(gd.talk_open_count>0){
		gd.talk_open_count++;
		if(gd.talk_open_count==20)gd.talk_open_count=0;
	}
	if(phase==MIYAZAKI_TALK || phase==DEPLOMA_TALK){
		controlTextCount(true);
	}else{
		controlTextCount(false);
	}

	if(phase==GUIDE_ALL || phase==GUIDE_ANIME){
		if(start==0 && tm.finish && !tm.failure && !showSearchImage){
			if (createSearchImage(tm.selected)) {
				tm.finish = false;
				showSearchImage = true;
			}
		}
	}

	manageThread();
}

void keyMiyazakiMuseum(){
	if(kick_count>0)return;
	if(key.z && !key_stop(key.z)){
		if(gd.x>180 && ((int)gd.x-180)%200<120){
			phase=YNFORM;
			menu[YNFORM].setViewMode(VISIBLE);
			if(gd.x>180 && gd.x<300){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+14]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+15]);
			}
			if(gd.x>380 && gd.x<500){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+16]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+17]);
			}
			if(gd.x>580 && gd.x<700){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+18]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+19]);
			}
			if(gd.x>780 && gd.x<900){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+20]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+21]);
			}
			if(gd.x>980 && gd.x<1100){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+16]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+17]);
			}
			if(gd.x>1180 && gd.x<1300 && gd.bought[6]){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+16]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+17]);
			}
			if(gd.x>1380 && gd.x<1500 && gd.bought[15]){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+22]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+23]);
			}
			if(gd.x>1580 && gd.x<1700 && gd.bought[12]){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+16]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+17]);
			}
			if(gd.x>1780 && gd.x<1900 && gd.bought[16]){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+24]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+25]);
			}
			if(gd.x>1980 && gd.x<2100 && gd.bought[9]){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+16]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+17]);
			}
			if(gd.x>2180 && gd.x<2300 && gd.bought[11]){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+18]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+19]);
			}
			if(gd.x>2380 && gd.x<2500){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+26]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+27]);
			}
			if(gd.x>2580 && gd.x<2700){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+28]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+29]);
			}
			if(gd.x>2780 && gd.x<2900){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+18]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+19]);
			}
		}
	}
	if(key.up && !key_stop(key.up)){
		gd.player_dir=2;
	}
	if(key.down && !key_stop(key.down)){
		gd.player_dir=0;
	}
	if(key.left){
		gd.x-=6;
		gd.player_dir=3;
	}
	if(key.right){
		gd.x+=6;
		if(gd.x>2800)gd.x=2800;
		gd.player_dir=1;
	}
	gd.scrX=(int)gd.x-320;
	if(gd.scrX<0)gd.scrX=0;
	if(gd.scrX>2160)gd.scrX=2160;
	if(talk_seoiha && abs(gd.x-1440)>200){
		TalkingAt(23);
		phase=MIYAZAKI_TALK;
		gd.talk_open_count=1;
		talk_seoiha=false;
	}
	if(gd.x<0){
		phase=LEAVE_MIYAZAKI;
		count=0;
	}
}

void keyGuideTop(){
	if(key.z && !key_stop(key.z)){
		if(menu[GUIDE_TOP].selected()==0){
			makeGuideBook();
			phase=GUIDE_ALL;
			menu[GUIDE_ALL].setViewMode(VISIBLE);
			menu[GUIDE_TOP].setViewMode(HIDE);
		}
		else if(gd.bought[7] || menu[GUIDE_TOP].selected()!=3){
			phase=GUIDE_SEASON;
			menu[GUIDE_SEASON].setViewMode(VISIBLE);
			menu[GUIDE_TOP].setViewMode(GRAY);
		}
	}
	if(key.x && !key_stop(key.x)){
		menu[GUIDE_TOP].setViewMode(HIDE);
		if(collection==allworks){
			phase=DEPLOMA_TALK;
			TalkingAt(24);
			gd.text_count=0;
			gd.talk_open_count=1;
		}else{
			phase=MIYAZAKI_MUSEUM;
		}
	}
	if(key.up && !key_wait(key.up))menu[GUIDE_TOP].cursorUp();
	if(key.down && !key_wait(key.down))menu[GUIDE_TOP].cursorDown();
}

void keyGuideAll(){
	if(key.z && !key_stop(key.z)){
		int n=menu[GUIDE_ALL].selected();
		if(animebook[n]){
			int a=0,b=0;
			bool *ok;
			ok=new bool[index_num];
			for(int i=0 ; i<index_num ; i++)ok[i]=false;
			for(int i=0 ; i<prgs ; i++){
				if(i==prgs2[b]){
					a+=2;b++;
				}
				if(prg[i].work==n){
					a++;ok[b-1]=true;
				}
			}
			String s;
			menu[GUIDE_STALIST].setMenu(10,30,30,9,a);
			a=0;b=0;
			for(int i=0; i<prgs ; i++){
				if(i==prgs2[b]){
					if(ok[b]){
						if(a!=0){menu[GUIDE_STALIST].stack("");a++;}
						sprintf_s(s.str[0],"=== %s ===",indexName[b].name.str[0]);
						sprintf_s(s.str[1],"=== %s ===",indexName[b].name.str[1]);
						menu[GUIDE_STALIST].stack(s);
						a++;
					}
					b++;
				}
				if(prg[i].work==n){
					sprintf_s(s.str[0],"%2d:%2d(%s)%s",prg[i].hour,prg[i].minute,weekChar[prg[i].week][0],sta[prg[i].station].name.str[0]);
					sprintf_s(s.str[1],"%2d:%2d(%s)%s",prg[i].hour,prg[i].minute,weekChar[prg[i].week][1],sta[prg[i].station].name.str[1]);
					menu[GUIDE_STALIST].stack(s);
					menu[GUIDE_STALIST].inputMark(a,sta[prg[i].station].mark);
					a++;
				}
			}
			delete [] ok;
			menu[GUIDE_STALIST].setBG(0);
			fishbox.panelColor(menu[GUIDE_ALL].getR(n),menu[GUIDE_ALL].getG(n),menu[GUIDE_ALL].getB(n));
			phase=GUIDE_STALIST_ALL;
			menu[GUIDE_STALIST].setViewMode(VISIBLE);
			menu[GUIDE_ALL].setViewMode(HIDE);
		}
		showSearchImage=false;
	}
	if(key.x && !key_stop(key.x)){
		phase=GUIDE_TOP;
		menu[GUIDE_ALL].setViewMode(HIDE);
		menu[GUIDE_TOP].setViewMode(VISIBLE);
		showSearchImage=false;
	}
	if(key.c && !key_stop(key.c)){
		if(showSearchImage){
			showSearchImage=false;
		}
		else if(tm.tcpsock==NULL){
			startThread(menu[GUIDE_ALL].selected());
		}
	}
	if(key.up && !key_wait(key.up))menu[GUIDE_ALL].cursorUp();
	if(key.down && !key_wait(key.down))menu[GUIDE_ALL].cursorDown();
	if(key.left && !key_wait(key.left))menu[GUIDE_ALL].pageUp();
	if(key.right && !key_wait(key.right))menu[GUIDE_ALL].pageDown();
}

void keyGuideSeason(){
	if(key.z && !key_stop(key.z)){
		dataNo=menu[GUIDE_SEASON].selected()+1;
		load_works(dataNo);
		if(menu[GUIDE_TOP].selected()==1){
			menu[GUIDE_ANIME].setMenu(0,20,40,10,works);
			menu[GUIDE_ANIME].lang=JAPANESE;
			for(int i=0 ; i<works ; i++){
				if(animebook[work[i].tnum]){
					menu[GUIDE_ANIME].stack(work[i].title);
					menu[GUIDE_ANIME].inputMark(i,work[i].mark);
					menu[GUIDE_ANIME].inputColor(i,work[i].r,work[i].g,work[i].b);
				}else{
					menu[GUIDE_ANIME].stack("----------");
				}
			}
			menu[GUIDE_ANIME].setBG(192);
			phase=GUIDE_ANIME;
			menu[GUIDE_ANIME].setViewMode(VISIBLE);
			menu[GUIDE_TOP].setViewMode(HIDE);
			menu[GUIDE_SEASON].setViewMode(HIDE);
		}
		if(menu[GUIDE_TOP].selected()==2){
			load_towers();
			load_station();
			menu[PREF_LIST_MIYAZAKI].setMenu(50,20,12,10,areas-1);
			for(int i=0 ; i<areas-1 ; i++)menu[PREF_LIST_MIYAZAKI].stack(area[i].name);
			phase=PREF_LIST_MIYAZAKI;
			menu[PREF_LIST_MIYAZAKI].setViewMode(VISIBLE);
			menu[GUIDE_SEASON].setViewMode(GRAY);
		}
		if(menu[GUIDE_TOP].selected()==3){
			String s;
			menu[GUIDE_TIME].setMenu(0,20,40,10,(prgs-1)*2);
			menu[GUIDE_TIME].setCombo(2);
			menu[GUIDE_TIME].lang=JAPANESE;
			for(int w=0 ; w<7 ; w++)for(int h=4 ; h<=27 ; h++)for(int m=0 ; m<60 ; m++){
				for(int i=0 ; i<prgs-1 ; i++)if(prg[i].week==w && prg[i].hour==h && prg[i].minute==m){
					if(animebook[work[prg[i].work].tnum]){
						for(int k=0 ; k<2 ; k++)sprintf_s(s.str[k],"%s",work[prg[i].work].title.str[k]);
						menu[GUIDE_TIME].stack(s);
					}
					else menu[GUIDE_TIME].stack("----------");
					for(int k=0 ; k<2 ; k++)sprintf_s(s.str[k],"    (%s)%2d:%2d %s",weekChar[prg[i].week][k],h,m,sta[prg[i].station].name.str[k]);
					menu[GUIDE_TIME].stack(s);
				}
			}
			phase=GUIDE_TIME;
			menu[GUIDE_TIME].setViewMode(VISIBLE);
			menu[GUIDE_SEASON].setViewMode(GRAY);
		}
	}
	if(key.x && !key_stop(key.x)){
		phase=GUIDE_TOP;
		menu[GUIDE_SEASON].setViewMode(HIDE);
		menu[GUIDE_TOP].setViewMode(VISIBLE);
	}
	if(key.up && !key_wait(key.up))menu[GUIDE_SEASON].cursorUp();
	if(key.down && !key_wait(key.down))menu[GUIDE_SEASON].cursorDown();
}

void keyPrefList(){
	if(key.z && !key_stop(key.z)){
		menu[GUIDE_STA].setMenu(0,20,40,10,area[menu[PREF_LIST_MIYAZAKI].selected()].st_num);
		for(int i=0 ; i<area[menu[PREF_LIST_MIYAZAKI].selected()].st_num ; i++){
			menu[GUIDE_STA].stack(sta[area[menu[PREF_LIST_MIYAZAKI].selected()].station[i]].name);
			menu[GUIDE_STA].inputMark(i,sta[area[menu[PREF_LIST_MIYAZAKI].selected()].station[i]].mark);
		}
		phase=GUIDE_STA;
		menu[PREF_LIST_MIYAZAKI].setViewMode(GRAY);
		menu[GUIDE_STA].setViewMode(VISIBLE);
	}
	if(key.x && !key_stop(key.x)){
		phase=GUIDE_SEASON;
		menu[PREF_LIST_MIYAZAKI].setViewMode(HIDE);
		menu[GUIDE_SEASON].setViewMode(VISIBLE);
	}
	if(key.up && !key_wait(key.up))menu[PREF_LIST_MIYAZAKI].cursorUp();
	if(key.down && !key_wait(key.down))menu[PREF_LIST_MIYAZAKI].cursorDown();
	if(key.left && !key_wait(key.left))menu[PREF_LIST_MIYAZAKI].pageUp();
	if(key.right && !key_wait(key.right))menu[PREF_LIST_MIYAZAKI].pageDown();
}

void keyGuideAnime(){
	if(key.z && !key_stop(key.z)){
		int n=menu[GUIDE_ANIME].selected();
		if(animebook[work[n].tnum]){
			String s;
			menu[GUIDE_STALIST].setMenu(10,30,30,9,work[n].num);
			for(int i=work[n].prg; i<+work[n].prg+work[n].num ; i++){
				sprintf_s(s.str[0],"%2d:%2d(%s)%s",prg[i].hour,prg[i].minute,weekChar[prg[i].week][0],sta[prg[i].station].name.str[0]);
				sprintf_s(s.str[1],"%2d:%2d(%s)%s",prg[i].hour,prg[i].minute,weekChar[prg[i].week][1],sta[prg[i].station].name.str[1]);
				menu[GUIDE_STALIST].stack(s);
				menu[GUIDE_STALIST].inputMark(i-work[n].prg,sta[prg[i].station].mark);
			}
			menu[GUIDE_STALIST].setBG(0);
			fishbox.panelColor(work[n].r,work[n].g,work[n].b);
			phase=GUIDE_STALIST;
			menu[GUIDE_STALIST].setViewMode(VISIBLE);
			menu[GUIDE_ANIME].setViewMode(HIDE);
		}
	}
	if(key.x && !key_stop(key.x)){
		phase=GUIDE_SEASON;
		menu[GUIDE_ANIME].setViewMode(HIDE);
		menu[GUIDE_SEASON].setViewMode(VISIBLE);
	}
	if(key.up && !key_wait(key.up))menu[GUIDE_ANIME].cursorUp();
	if(key.down && !key_wait(key.down))menu[GUIDE_ANIME].cursorDown();
	if(key.left && !key_wait(key.left))menu[GUIDE_ANIME].pageUp();
	if(key.right && !key_wait(key.right))menu[GUIDE_ANIME].pageDown();
}

void keyGuideStaList(){
	if(key.x && !key_stop(key.x)){
		menu[GUIDE_STALIST].setViewMode(HIDE);
		if(phase==GUIDE_STALIST_ALL){
			phase=GUIDE_ALL;
			menu[GUIDE_ALL].setViewMode(VISIBLE);
		}else{
			phase=GUIDE_ANIME;
			menu[GUIDE_ANIME].setViewMode(VISIBLE);
		}
	}
	if(key.up && !key_wait(key.up))menu[GUIDE_STALIST].cursorUp();
	if(key.down && !key_wait(key.down))menu[GUIDE_STALIST].cursorDown();
	if(key.left && !key_wait(key.left))menu[GUIDE_STALIST].pageUp();
	if(key.right && !key_wait(key.right))menu[GUIDE_STALIST].pageDown();
}

void keyGuideSta(){
	if(key.z && !key_stop(key.z)){
		int n=area[menu[PREF_LIST_MIYAZAKI].selected()].station[menu[GUIDE_STA].selected()];
		int a=0;
		String s;
		int *b;
		for(int i=0; i<prgs ; i++){
			if(prg[i].station==n)a++;
		}
		b=new int[a];
		menu[GUIDE_PRGLIST].setMenu(20,60,30,9,a);
		menu[GUIDE_PRGLIST].lang=JAPANESE;
		a=0;
		for(int i=0; i<prgs ; i++)if(prg[i].station==n){
			b[a]=i;a++;
		}
		for(int i=a-2 ; i>=0 ; i--)for(int j=a-1 ; j>i ; j--){
			if(prg[b[i]].week>prg[b[j]].week ||
			   (prg[b[i]].week==prg[b[j]].week && prg[b[i]].hour>prg[b[j]].hour) ||
			   (prg[b[i]].week==prg[b[j]].week && prg[b[i]].hour==prg[b[j]].hour && prg[b[i]].minute>prg[b[j]].minute)
			  ){
				int c=b[i];
				b[i]=b[j];
				b[j]=c;
			}
		}
		for(int i=0 ; i<a ; i++){
			if(animebook[work[prg[b[i]].work].tnum]){
				sprintf_s(s.str[0],"%2d:%2d(%s)%s",prg[b[i]].hour,prg[b[i]].minute,weekChar[prg[b[i]].week][0],work[prg[b[i]].work].title.str[0]);
				sprintf_s(s.str[1],"%2d:%2d(%s)%s",prg[b[i]].hour,prg[b[i]].minute,weekChar[prg[b[i]].week][1],work[prg[b[i]].work].title.str[1]);
			}else{
				sprintf_s(s.str[0],"%2d:%2d(%s)----------",prg[b[i]].hour,prg[b[i]].minute,weekChar[prg[b[i]].week][0]);
				sprintf_s(s.str[1],"%2d:%2d(%s)----------",prg[b[i]].hour,prg[b[i]].minute,weekChar[prg[b[i]].week][1]);
			}
			menu[GUIDE_PRGLIST].stack(s);
		}
		phase=GUIDE_PRGLIST;
		menu[GUIDE_PRGLIST].setViewMode(VISIBLE);
		menu[GUIDE_STA].setViewMode(GRAY);
	}
	if(key.x && !key_stop(key.x)){
		phase=PREF_LIST_MIYAZAKI;
		menu[GUIDE_STA].setViewMode(HIDE);
		menu[PREF_LIST_MIYAZAKI].setViewMode(VISIBLE);
	}
	if(key.up && !key_wait(key.up))menu[GUIDE_STA].cursorUp();
	if(key.down && !key_wait(key.down))menu[GUIDE_STA].cursorDown();
}

void keyGuidePrgList(){
	if(key.x && !key_stop(key.x)){
		phase=GUIDE_STA;
		menu[GUIDE_STA].setViewMode(VISIBLE);
		menu[GUIDE_PRGLIST].setViewMode(HIDE);
	}
	if(key.up && !key_wait(key.up))menu[GUIDE_PRGLIST].cursorUp();
	if(key.down && !key_wait(key.down))menu[GUIDE_PRGLIST].cursorDown();
	if(key.left && !key_wait(key.left))menu[GUIDE_PRGLIST].pageUp();
	if(key.right && !key_wait(key.right))menu[GUIDE_PRGLIST].pageDown();
}

void keyGuideTime(){
	if(key.x && !key_stop(key.x)){
		phase=GUIDE_SEASON;
		menu[GUIDE_SEASON].setViewMode(VISIBLE);
		menu[GUIDE_TIME].setViewMode(HIDE);
	}
	if(key.up && !key_wait(key.up))menu[GUIDE_TIME].cursorUp();
	if(key.down && !key_wait(key.down))menu[GUIDE_TIME].cursorDown();
	if(key.left && !key_wait(key.left))menu[GUIDE_TIME].pageUp();
	if(key.right && !key_wait(key.right))menu[GUIDE_TIME].pageDown();
}

void keyBgmTest(){
	if(key.z && !key_stop(key.z)){
		if(menu[BGM_TEST].selected()>=NUM_OF_BGM){
			Mix_FadeOutMusic(2000);
			menu[BGM_TEST].setViewMode(HIDE);
			phase=GOTO_MOVIE;
			count=0;
		}else{
			freeMusic();
			sprintf_s(str,"file/bgm/%d.ogg",menu[BGM_TEST].selected()+1);
			bgm=Mix_LoadMUS(str);
			Mix_PlayMusic(bgm,-1);
		}
	}
	if(key.x && !key_stop(key.x)){
		movie_test=false;
		phase=MIYAZAKI_MUSEUM;
		menu[BGM_TEST].setViewMode(HIDE);
	}
	if(key.up && !key_wait(key.up))menu[BGM_TEST].cursorUp();
	if(key.down && !key_wait(key.down))menu[BGM_TEST].cursorDown();
	if(key.left && !key_wait(key.left))menu[BGM_TEST].pageUp();
	if(key.right && !key_wait(key.right))menu[BGM_TEST].pageDown();
}

void keyPrefList_tower(){
	if(key.z && !key_stop(key.z)){
		int n=menu[PREF_LIST_MIYAZAKI].selected();
		menu[GUIDE_STA].setMenu(60,40,20,8,area[n].st_num+1);
		menu[GUIDE_STA].stack(text[MIYAZAKITEXT+5]);
		for(int i=0 ; i<area[n].st_num ; i++){
			menu[GUIDE_STA].stack(sta[area[n].station[i]].name);
		}
		menu[PREF_LIST_MIYAZAKI].setViewMode(GRAY);
		menu[GUIDE_STA].setViewMode(VISIBLE);
		phase=STA_LIST_TOWER;
	}
	if(key.x && !key_stop(key.x)){
		phase=MIYAZAKI_MUSEUM;
		menu[PREF_LIST_MIYAZAKI].setViewMode(HIDE);
	}
	if(key.up && !key_wait(key.up))menu[PREF_LIST_MIYAZAKI].cursorUp();
	if(key.down && !key_wait(key.down))menu[PREF_LIST_MIYAZAKI].cursorDown();
	if(key.left && !key_wait(key.left))menu[PREF_LIST_MIYAZAKI].pageUp();
	if(key.right && !key_wait(key.right))menu[PREF_LIST_MIYAZAKI].pageDown();
}

void keyStaList_tower(){
	if(key.z && !key_stop(key.z)){
		for(int i=0 ; i<towers ; i++)tower[i].remove=false;
		for(int i=0 ; i<towers ; i++){
			if(tower[i].area!=menu[PREF_LIST_MIYAZAKI].selected())tower[i].remove=true;
			if(menu[GUIDE_STA].selected()!=0){
				if(tower[i].ch[menu[GUIDE_STA].selected()-1]==0)tower[i].remove=true;
				if(tower[i].ch[menu[GUIDE_STA].selected()-1]==CHANNELS+1)tower[i].remove=true;
			}
		}
		for(int i=0 ; i<towers; i++){
			if(tower[i].area==menu[PREF_LIST_MIYAZAKI].selected() && !tower[i].remove){
				gd.x=tower[i].x;
				gd.y=tower[i].y;
				break;
			}
		}
		for(int i=0 ; i<10 ; i++)this_tower[i]=EOF;
		int n=0;
		for(int i=0 ; i<towers ; i++)if(!tower[i].remove){
			if(tower[i].x==gd.x && tower[i].y==gd.y){
				this_tower[n]=i;
				if(n==9)break;
				n++;
			}
		}
		menu[PREF_LIST_MIYAZAKI].setViewMode(HIDE);
		menu[GUIDE_STA].setViewMode(HIDE);
		phase=GOTO_TOWERLIST;
		count=0;
	}
	if(key.x && !key_stop(key.x)){
		phase=PREF_LIST_TOWER;
		menu[GUIDE_STA].setViewMode(HIDE);
		menu[PREF_LIST_MIYAZAKI].setViewMode(VISIBLE);
	}
	if(key.up && !key_wait(key.up))menu[GUIDE_STA].cursorUp();
	if(key.down && !key_wait(key.down))menu[GUIDE_STA].cursorDown();
}

void keyTowerList(){
	if(key.z && !key_stop(key.z)){
		if(this_tower[1]!=EOF){
			int n=0;
			for(int i=0 ; i<10 ; i++){
				if(this_tower[i]==EOF)break;
				n++;
			}
			double a;
			String s;
			menu[WHICH_TOWER].setMenu(60,60,20,5,n);
			for(int i=0 ; i<n ; i++){
				a=tower[this_tower[i]].kw*1.0;
				for(int j=0 ; j<tower[this_tower[i]].kw2 ; j++)a/=10;
				sprintf_s(s.str[0],"%s %.6gkw",tower[this_tower[i]].name.str[0],a);
				sprintf_s(s.str[1],"%s %.6gkw",tower[this_tower[i]].name.str[1],a);
				menu[WHICH_TOWER].stack(s);
			}
			menu[WHICH_TOWER].setViewMode(VISIBLE);
			phase=WHICH_TOWER;
		}
		else if(this_tower[0]!=EOF){
			phase=SHOW_TOWERDATA;
			which_tower=0;
		}
	}
	if(key.x && !key_stop(key.x)){
		for(int i=0 ; i<towers ; i++)tower[i].remove=false;
		if(NHK_REMOVE){
			bool ok;
			for(int i=0 ; i<towers ; i++)if(tower[i].kw2!=0){
				ok=false;
				for(int j=0 ; j<10 ; j++)if(tower[i].ch[j]!=0){
					if(sta[area[tower[i].area].station[j]].mark!=5 && sta[area[tower[i].area].station[j]].mark!=6){
						ok=true;break;
					}
				}
				if(ok)tower[i].remove=false;
				else tower[i].remove=true;
			}
		}
		createMap_tower();
		menu[GUIDE_STA].setViewMode(VISIBLE);
		phase=STA_LIST_TOWER;
		gd.x=1200;
		gd.scrX=880;
	}
	if(!key_wait(key.up) || !key_wait(key.down) || !key_wait(key.left) || !key_wait(key.right)){
		if(key.up)gd.y--;
		if(key.down)gd.y++;
		if(key.left)gd.x--;
		if(key.right)gd.x++;
		gd.scrX=(int)gd.x*MAGNIFY-320;
		gd.scrY=(int)gd.y*MAGNIFY-240;
		map.buffered=false;
	}
	if(key.left||key.right||key.up||key.down){
		for(int i=0 ; i<10 ; i++)this_tower[i]=EOF;
		int n=0;
		for(int i=0 ; i<towers ; i++)if(!tower[i].remove){
			if(tower[i].x==gd.x && tower[i].y==gd.y){
				this_tower[n]=i;
				if(n==9)break;
				n++;
			}
		}
	}
}

void keyWhichTower(){
	if(key.z && !key_stop(key.z)){
		which_tower=menu[WHICH_TOWER].selected();
		menu[WHICH_TOWER].setViewMode(HIDE);
		phase=SHOW_TOWERDATA;
	}
	if(key.x && !key_stop(key.x)){
		phase=TOWERLIST;
		menu[WHICH_TOWER].setViewMode(HIDE);
	}
	if(key.up && !key_wait(key.up))menu[WHICH_TOWER].cursorUp();
	if(key.down && !key_wait(key.down))menu[WHICH_TOWER].cursorDown();
}

void keyShowTowerData(){
	if((key.z||key.x) && (!key_stop(key.z) || !key_stop(key.x))){
		if(this_tower[1]!=EOF){
			phase=WHICH_TOWER;
			menu[WHICH_TOWER].setViewMode(VISIBLE);
		}
		else phase=TOWERLIST;
	}
}

void keyDeleteMenu(){
	if(key.z && !key_stop(key.z)){
		if(menu[DELETE_MENU].selected()==3){
			if(makeSaveMenu(SAVEDATA_DELETE)){
				phase=SAVEDATA_DELETE;
				menu[SAVEDATA_DELETE].setViewMode(VISIBLE);
			}
			else phase=NODATA1;
		}
		else if(menu[DELETE_MENU].selected()==4){
			if(makeRecordMenu(RECORD_DELETE)){
				phase=RECORD_DELETE;
				menu[RECORD_DELETE].setViewMode(VISIBLE);
			}
			else phase=NODATA1;
		}
		else if(menu[DELETE_MENU].selected()==5){
			load_towers();
			load_station();
			menu[PREF_LIST_MIYAZAKI].setMenu(100,40,12,10,areas-1);
			for(int i=0 ; i<areas-1 ; i++)menu[PREF_LIST_MIYAZAKI].stack(area[i].name);
			phase=SMR_DELETE_PREF;
			menu[PREF_LIST_MIYAZAKI].setViewMode(VISIBLE);
		}else{
			phase=DELETE_YN;
			menu[DELETE_YN].setViewMode(VISIBLE);
		}
		if(phase!=NODATA1)menu[DELETE_MENU].setViewMode(GRAY);
	}
	if(key.x && !key_stop(key.x)){
		phase=MIYAZAKI_MUSEUM;
		menu[DELETE_MENU].setViewMode(HIDE);
	}
	if(key.up && !key_wait(key.up))menu[DELETE_MENU].cursorUp();
	if(key.down && !key_wait(key.down))menu[DELETE_MENU].cursorDown();
}

void keySavedataDelete(){
	if(key.z && !key_stop(key.z)){
		phase=DELETE_YN;
		menu[SAVEDATA_DELETE].setViewMode(GRAY);
		menu[DELETE_YN].setViewMode(VISIBLE);
	}
	if(key.x && !key_stop(key.x)){
		phase=DELETE_MENU;
		menu[SAVEDATA_DELETE].setViewMode(HIDE);
		menu[DELETE_MENU].setViewMode(VISIBLE);
	}
	if(key.up && !key_wait(key.up))menu[SAVEDATA_DELETE].cursorUp();
	if(key.down && !key_wait(key.down))menu[SAVEDATA_DELETE].cursorDown();
}

void keyRecordDelete(){
	if(key.z && !key_stop(key.z)){
		phase=DELETE_YN;
		menu[RECORD_DELETE].setViewMode(GRAY);
		menu[DELETE_YN].setViewMode(VISIBLE);
	}
	if(key.x && !key_stop(key.x)){
		phase=DELETE_MENU;
		menu[RECORD_DELETE].setViewMode(HIDE);
		menu[DELETE_MENU].setViewMode(VISIBLE);
	}
	if(key.up && !key_wait(key.up))menu[RECORD_DELETE].cursorUp();
	if(key.down && !key_wait(key.down))menu[RECORD_DELETE].cursorDown();
}

void keySmrDelete_pref(){
	if(key.z && !key_stop(key.z)){
		String str2;
		menu[GUIDE_STA].setMenu(0,40,40,10,area[menu[PREF_LIST_MIYAZAKI].selected()].st_num);
		for(int i=0 ; i<area[menu[PREF_LIST_MIYAZAKI].selected()].st_num ; i++){
			int n=area[ menu[PREF_LIST_MIYAZAKI].selected() ].station[i];
			sprintf_s(str2.str[0],"%s",sta[n].name.str[0]);
			sprintf_s(str2.str[1],"%s",sta[n].name.str[1]);
			sprintf_s(str,"save/smr/smr%d.dat",n);
			if(loadFile(str)){
				sprintf_s(str2.str[0],"%s (%dbyte)",str2.str[0],fsize);
				sprintf_s(str2.str[1],"%s (%dbyte)",str2.str[1],fsize);
			}
			menu[GUIDE_STA].stack(str2);
			menu[GUIDE_STA].inputMark(i,sta[area[menu[PREF_LIST_MIYAZAKI].selected()].station[i]].mark);
		}
		phase=SMR_DELETE;
		menu[PREF_LIST_MIYAZAKI].setViewMode(GRAY);
		menu[GUIDE_STA].setViewMode(VISIBLE);
	}
	if(key.x && !key_stop(key.x)){
		phase=DELETE_MENU;
		menu[PREF_LIST_MIYAZAKI].setViewMode(HIDE);
		menu[DELETE_MENU].setViewMode(VISIBLE);
	}
	if(key.up && !key_wait(key.up))menu[PREF_LIST_MIYAZAKI].cursorUp();
	if(key.down && !key_wait(key.down))menu[PREF_LIST_MIYAZAKI].cursorDown();
	if(key.left && !key_wait(key.left))menu[PREF_LIST_MIYAZAKI].pageUp();
	if(key.right && !key_wait(key.right))menu[PREF_LIST_MIYAZAKI].pageDown();
}

void keySmrDelete(){
	if(key.z && !key_stop(key.z)){
		phase=DELETE_YN;
		menu[GUIDE_STA].setViewMode(GRAY);
		menu[DELETE_YN].setViewMode(VISIBLE);
	}
	if(key.x && !key_stop(key.x)){
		phase=SMR_DELETE_PREF;
		menu[GUIDE_STA].setViewMode(HIDE);
		menu[PREF_LIST_MIYAZAKI].setViewMode(VISIBLE);
	}
	if(key.up && !key_wait(key.up))menu[GUIDE_STA].cursorUp();
	if(key.down && !key_wait(key.down))menu[GUIDE_STA].cursorDown();
}


void keyMiyazakiTalk(){
	if(key.z && !key_stop(key.z)){
		int a=controlTalking();
		if(a==EOF){
			if(gd.x>180 && gd.x<300 && menu[YNFORM].selected()==1){
				endMiyazaki();
				initHaziaShop();
				kick_count++;
			}
			else if(phase==DEPLOMA_TALK)phase=DEPLOMA;
			else phase=MIYAZAKI_MUSEUM;
		}
	}
}

void keyDeleteYN(){
	if(key.z && !key_stop(key.z) && menu[DELETE_YN].selected()==1){
		FILE *hf;
		if(menu[DELETE_MENU].selected()==0){
			fopen_s(&hf,"save/option.dat","rb");
			if(hf==NULL)phase=NODATA2;
			else{
				fclose(hf);
				remove("save/option.dat");
				load_option();
				phase=DELETE_MENU;
				menu[DELETE_MENU].setViewMode(VISIBLE);
			}
		}
		if(menu[DELETE_MENU].selected()==1){
			fopen_s(&hf,"save/animebook.dat","rb");
			if(hf==NULL)phase=NODATA2;
			else{
				fclose(hf);
				remove("save/animebook.dat");
				load_animebook();
				phase=DELETE_MENU;
				menu[DELETE_MENU].setViewMode(VISIBLE);
			}
		}
		if(menu[DELETE_MENU].selected()==2){
			fopen_s(&hf,"save/clear.dat","rb");
			if(hf==NULL)phase=NODATA2;
			else{
				fclose(hf);
				remove("save/clear.dat");
				load_index();
				phase=DELETE_MENU;
				menu[DELETE_MENU].setViewMode(VISIBLE);
			}
		}
		if(menu[DELETE_MENU].selected()==3){
			int n1=menu[SAVEDATA_DELETE].selected();
			int n2=menu[SAVEDATA_DELETE].count();
			sprintf_s(str,"save/save%d.dat",n1);
			remove(str);
			char str1[20],str2[20];
			for(int i=n1 ; i<n2-1 ; i++){
				sprintf_s(str1,"save/save%d.dat",i+1);
				sprintf_s(str2,"save/save%d.dat",i);
				rename(str1,str2);
			}
			menu[SAVEDATA_DELETE].delet(n1);
			if(menu[SAVEDATA_DELETE].count()==0){
				phase=DELETE_MENU;
				menu[SAVEDATA_DELETE].setViewMode(HIDE);
				menu[DELETE_MENU].setViewMode(VISIBLE);
			}else{
				phase=SAVEDATA_DELETE;
				menu[SAVEDATA_DELETE].setViewMode(VISIBLE);
			}
		}
		if(menu[DELETE_MENU].selected()==4){
			int n1=menu[RECORD_DELETE].selected();
			int n2=menu[RECORD_DELETE].count();
			sprintf_s(str,"save/record%d.dat",n1);
			remove(str);
			char str1[20],str2[20];
			for(int i=n1 ; i<n2-1 ; i++){
				sprintf_s(str1,"save/record%d.dat",i+1);
				sprintf_s(str2,"save/record%d.dat",i);
				rename(str1,str2);
			}
			menu[RECORD_DELETE].delet(n1);
			if(menu[RECORD_DELETE].count()==0){
				phase=DELETE_MENU;
				menu[RECORD_DELETE].setViewMode(HIDE);
				menu[DELETE_MENU].setViewMode(VISIBLE);
			}else{
				phase=RECORD_DELETE;
				menu[RECORD_DELETE].setViewMode(VISIBLE);
			}
		}
		if(menu[DELETE_MENU].selected()==5){
			int n=area[ menu[PREF_LIST_MIYAZAKI].selected() ].station[ menu[GUIDE_STA].selected() ];
			sprintf_s(str,"save/smr/smr%d.dat",n);
			fopen_s(&hf,str,"rb");
			if(hf==NULL)phase=NODATA2;
			else{
				fclose(hf);
				remove(str);
				String str2;
				sprintf_s(str2.str[0],"%s",sta[n].name.str[0]);
				sprintf_s(str2.str[1],"%s",sta[n].name.str[1]);
				menu[GUIDE_STA].input(menu[GUIDE_STA].selected(),str2);
				phase=SMR_DELETE;
				menu[GUIDE_STA].setViewMode(VISIBLE);
			}
		}
		if(phase!=NODATA2)menu[DELETE_YN].setViewMode(HIDE);
	}
	if((key.x && !key_stop(key.x)) || (key.z && !key_stop(key.z) && menu[DELETE_YN].selected()==0)){
		if(menu[DELETE_MENU].selected()==3){
			phase=SAVEDATA_DELETE;
			menu[SAVEDATA_DELETE].setViewMode(VISIBLE);
		}
		else if(menu[DELETE_MENU].selected()==4){
			phase=RECORD_DELETE;
			menu[RECORD_DELETE].setViewMode(VISIBLE);
		}
		else if(menu[DELETE_MENU].selected()==5){
			phase=SMR_DELETE;
			menu[GUIDE_STA].setViewMode(VISIBLE);
		}else{
			phase=DELETE_MENU;
			menu[DELETE_MENU].setViewMode(VISIBLE);
		}
		menu[DELETE_YN].setViewMode(HIDE);
	}
	if(key.up && !key_wait(key.up))menu[DELETE_YN].cursorUp();
	if(key.down && !key_wait(key.down))menu[DELETE_YN].cursorDown();
}

void keyYNForm(){
	if(key.z && !key_stop(key.z)){
		if(menu[YNFORM].selected()==0){
			gd.text_count=0;
			phase=MIYAZAKI_TALK;
			int a=((int)gd.x-180)/200;
			if((a==6 && !gd.bought[15]) || (a==8 && !gd.bought[16]) || (a==5 && !gd.bought[6])
			   || (a==9 && !gd.bought[9]) || (a==10 && !gd.bought[11]) || (a==7 && !gd.bought[12])){
				TalkingAt(0);
			}else{
				TalkingAt(a+1);
				if(a==9)talk_3dtv=true;
			}
			gd.talk_open_count=1;
		}
		if(menu[YNFORM].selected()==1){
			phase=MIYAZAKI_MUSEUM;
			if(gd.x>180 && gd.x<300){
				phase=MIYAZAKI_TALK;
				gd.talk_open_count=1;
				TalkingAt(15);
			}
			if(gd.x>380 && gd.x<500){
				phase=GUIDE_TOP;
				menu[GUIDE_TOP].setViewMode(VISIBLE);
				load_animebook();
			}
			if(gd.x>580 && gd.x<700){
				gd.game_mode=WALKING;
				phase=GOTO_STROLL;
				count=0;
			}
			if(gd.x>780 && gd.x<900){
				phase=MIYAZAKI_TALK;
				gd.talk_open_count=1;
				TalkingAt(16);
			}
			if(gd.x>980 && gd.x<1100){
				phase=MIYAZAKI_TALK;
				gd.talk_open_count=1;
				TalkingAt(17);
			}
			if(gd.x>1180 && gd.x<1300 && gd.bought[6]){
				load_towers();
				load_station();
				menu[PREF_LIST_MIYAZAKI].setMenu(100,40,12,10,areas-1);
				for(int i=0 ; i<areas-1 ; i++)menu[PREF_LIST_MIYAZAKI].stack(area[i].name);
				phase=PREF_LIST_TOWER;
				menu[PREF_LIST_MIYAZAKI].setViewMode(VISIBLE);
			}
			if(gd.x>1380 && gd.x<1500 && gd.bought[15]){
				Mix_PlayChannel(1,sf.decide,0);
				gd.player_dir=2;
				phase=SEOI_HA;
				count=0;
			}
			if(gd.x>1580 && gd.x<1700 && gd.bought[12]){
				phase=BGM_TEST;
				menu[BGM_TEST].setViewMode(VISIBLE);
			}
			if(gd.x>1780 && gd.x<1900 && gd.bought[16]){
				if(scr_design==MONO){
					scr_design=NULL;
					phase=MIYAZAKI_MUSEUM;
				}else{
					scr_design=MONO;
					phase=MIYAZAKI_TALK;
					gd.talk_open_count=1;
					TalkingAt(19);
				}
			}
			if(gd.x>1980 && gd.x<2100 && gd.bought[9]){
				gd.game_mode=WALKING;
				MAP3D=true;
				phase=GOTO_STROLL;
				count=0;
			}
			if(gd.x>2180 && gd.x<2300 && gd.bought[11]){
				gd.game_mode=NO_RELAY;
				phase=GOTO_STROLL;
				count=0;
			}
			if(gd.x>2380 && gd.x<2500){
				phase=MIYAZAKI_TALK;
				gd.talk_open_count=1;
				TalkingAt(20);
			}
			if(gd.x>2580 && gd.x<2700){
				phase=DELETE_MENU;
				menu[DELETE_MENU].setViewMode(VISIBLE);
			}
			if(gd.x>2780 && gd.x<2900){
				phase=MIYAZAKI_TALK;
				gd.talk_open_count=1;
				TalkingAt(21);
			}
		}
		if(menu[YNFORM].selected()==2){
			phase=MIYAZAKI_MUSEUM;
		}
		menu[YNFORM].setViewMode(HIDE);
	}
	if(key.x && !key_stop(key.x)){
		phase=MIYAZAKI_MUSEUM;
		menu[YNFORM].setViewMode(HIDE);
	}
	if(key.up && !key_wait(key.up))menu[YNFORM].cursorUp();
	if(key.down && !key_wait(key.down))menu[YNFORM].cursorDown();
}

void keyDeploma(){
	if(key.z && !key_stop(key.z)){
		phase=MIYAZAKI_MUSEUM;
	}
}

void keyMiyazaki(){
	switch(phase){
		case MIYAZAKI_MUSEUM:keyMiyazakiMuseum();break;
		case GUIDE_TOP:keyGuideTop();break;
		case GUIDE_ALL:keyGuideAll();break;
		case GUIDE_SEASON:keyGuideSeason();break;
		case GUIDE_ANIME:keyGuideAnime();break;
		case PREF_LIST_MIYAZAKI:keyPrefList();break;
		case GUIDE_STALIST:
		case GUIDE_STALIST_ALL:keyGuideStaList();break;
		case GUIDE_STA:keyGuideSta();break;
		case GUIDE_PRGLIST:keyGuidePrgList();break;
		case GUIDE_TIME:keyGuideTime();break;
		case BGM_TEST:keyBgmTest();break;
		case PREF_LIST_TOWER:keyPrefList_tower();break;
		case STA_LIST_TOWER:keyStaList_tower();break;
		case TOWERLIST:keyTowerList();break;
		case SHOW_TOWERDATA:keyShowTowerData();break;
		case WHICH_TOWER:keyWhichTower();break;
		case NODATA1:
		case DELETE_MENU:keyDeleteMenu();break;
		case SAVEDATA_DELETE:keySavedataDelete();break;
		case RECORD_DELETE:keyRecordDelete();break;
		case SMR_DELETE_PREF:keySmrDelete_pref();break;
		case SMR_DELETE:keySmrDelete();break;
		case NODATA2:
		case DELETE_YN:keyDeleteYN();break;
		case DEPLOMA_TALK:
		case MIYAZAKI_TALK:keyMiyazakiTalk();break;
		case DEPLOMA:keyDeploma();break;
		case YNFORM:keyYNForm();break;
		default:break;
	}
	if(phase==GAMESTART && key.z && !key_stop(key.z)){
		phase=MIYAZAKI_MUSEUM;
		freeImage(img.back);
		getImage(img.back,"file/img/miyazaki.png",0,0,255);
		freeMusic();
		bgm=Mix_LoadMUS("file/bgm/17.ogg");
		Mix_PlayMusic(bgm,-1);
		start=0;
	}
}

void drawTowerList(SDL_Surface* scr){
	fillRect(scr,0,0,640,480,192,192,255,255);
	drawMap(scr,(int)gd.x-320,(int)gd.y-240);
	drawImage(scr,img.chr,290,210,180,60,60,60,255);
	if(phase==SHOW_TOWERDATA){
		drawText(scr,0,0,tower[this_tower[which_tower]].name);
		double n=tower[this_tower[which_tower]].kw*1.0;
		for(int m=0 ; m<tower[this_tower[which_tower]].kw2 ; m++)n/=10.0;
		sprintf_s(str,"%.6gkw",n);
		drawText(scr,0,40,str);
		for(int i=0 ; i<area[tower[this_tower[which_tower]].area].st_num ; i++){
			drawImage(scr,img.symbol,0,i*40+80,
					  (sta[area[tower[this_tower[which_tower]].area].station[i]].mark%17)*34,
					  (sta[area[tower[this_tower[which_tower]].area].station[i]].mark/17)*34,
					  34,34,255);
			drawText(scr,40,i*40+80,sta[area[tower[this_tower[which_tower]].area].station[i]].name);
			if(tower[this_tower[which_tower]].ch[i]==0)drawText(scr,400,i*40+80,"----");
			else if(tower[this_tower[which_tower]].ch[i]==CHANNELS+1)drawText(scr,400,i*40+80,"****");
			else{
				sprintf_s(str,"ch%2d",tower[this_tower[which_tower]].ch[i]);
				drawText(scr,400,i*40+80,str);
			}
		}
		if(tower[this_tower[which_tower]].v)drawImage(scr,img.back,560,0,1240,640,80,80,255);
		else drawImage(scr,img.back,560,0,1240,560,80,80,255);
		if(((count/20)%4)!=3){
			for(int i=0 ; i<8 ; i++){
				if(((tower[this_tower[which_tower]].bias>>i)&1)==1){
					drawImage(scr,img.back,480,0,600+i*80,560+((count/20)%4)*80,80,80,255);
				}
			}
		}
	}else{
		for(int i=0 ; i<10 ; i++){
			if(this_tower[i]==EOF)break;
			drawText(scr,0,i*40,tower[this_tower[i]].name);
		}
	}
}

void drawMiyazakiMuseum(SDL_Surface* scr){
	for(int i=0 ; i<480 ; i++){
		fillRect(scr,0,i,640,1,abs(30-(i+count)%60)*4,0,0,255);
	}
	drawImage(scr,img.back,0,start,0,0,640,480,255-start*255/200);
	if(start>0){
		if(start>180)fontA=(200-start)*13;
		if(start<20)fontA=start*13;
		drawText2(scr,start/3,120,text[MIYAZAKITEXT+71]);
		drawText2(scr,50+start/2,200,text[MIYAZAKITEXT+72]);
		fontA=255;
	}
	int a=0;
	if(start>40)a=480;
	else if(start>0)a=start*12;
	for(int i=0 ; i<16 ; i++){
		drawImage(scr,img.back,i*160-gd.scrX,abs(25-(count/2+(i%4)*8)%50)*2+a,((count/10)%2)*100,560,100,100,255);
		drawImage(scr,img.back,i*160-gd.scrX,abs(25-(count/2+(i%4)*8)%50)*2+a,(i%4)*100+200,560,100,100,255);
	}
	for(int i=0 ; i<14 ; i++){
		if((i==7 && !gd.bought[15]) || (i==9 && !gd.bought[16]) || (i==6 && !gd.bought[6])
		   || (i==10 && !gd.bought[9]) || (i==11 && !gd.bought[11]) || (i==8 && !gd.bought[12])){
			drawImage(scr,img.back,i*200-gd.scrX,400,0,480,80,80,255);
		}
		else drawImage(scr,img.back,i*200-gd.scrX,400,80+i*80,480,80,80,255);
	}
	if(phase==SEOI_HA){
		drawImage(scr,img.back,1400-gd.scrX,380,1200+(count/30)*80,480,80,80,255);
	}
	drawImage(scr,img.chr,(int)gd.x-gd.scrX-14,420,gd.player_dir*30,0,30,60,255);
	if(key.left || key.right)drawImage(scr,img.chr,(int)gd.x-gd.scrX-26,440,((count/5)%2)*60,110,60,60,255);
	if(phase==GUIDE_ALL){
		sprintf_s(str,"%4d/%4d",menu[GUIDE_ALL].selected()+1,allworks);
		drawText2(scr,0,0,str,(int)strlen(str));
	}
	if(phase==GUIDE_TOP){
		drawImage(scr,img.back,300,240,0,660,240,120,255);
		sprintf_s(str,"%4d/%4d",collection,allworks);
		drawText2(scr,380,300,str);
	}
}

void drawComeMiyazaki(SDL_Surface* scr){
	int x,y,x2,y2;
	double mag;
	if(count<100)x=660-count*4;
	else if(count<130)x=260;
	else x=260-(count-130);
	if(count<80)y=250+count/8;
	else if(count<100)y=260+(count-80)*2;
	else if(count<130)y=300;
	else y=300-(count-130);
	if(count<100){x2=258;y2=208;mag=0.85;}
	else if(count<120){x2=258-(count-100);y2=208-(count-100)/2;mag=0.85+(count-100)*0.0075;}
	else{x2=238;y2=198;mag=1;}
	drawImage(scr,img.back,0,0,640,0,640,480,255);
	drawImage(scr,img.back,238,198,1280,0,18,216,255);
	drawImage(scr,img.back,x,y,240,660,60,120,255);
	drawImage(scr,img.back,460,240,1280,330,180,140,255);
	drawImage_x(scr,img.back,x2,y2,mag,1280,0,18,216,255);
	if(count<110)drawImage(scr,img.back,180,360,1320,560,80,80,255);
	else if(count<120)drawImage(scr,img.back,200,370,1320,640,80,80,255);
	else drawImage(scr,img.back,220,380,1320,720,80,80,255);
}

void drawLeaveMiyazaki(SDL_Surface* scr){
	int x,y,a,x2,y2,x3=0,y3=0;
	if(count<30)x=240+count*4;
	else if(count<60)x=360;
	else x=360-(count-60)*4;
	if(count<60)y=280;
	else y=280+(count-60)*2;
	if(count<50)a=300;
	else a=240;
	if(count<30){x2=238+count*8;y2=198-count*2;}
	else if(count<60){x2=438-(count-30)*12;y2=88+(count-40)*(count-40)/2;}
	else{x2=78;y2=288;}
	if(count>60){x3=120;y3=420;}
	else if(count>30){x3=(count-30)*4;y3=240+(count-30)*(count-30)/5;}
	drawImage(scr,img.back,0,0,640,0,640,480,255);
	if(count<30)drawImage(scr,img.back,x2,y2,1280,0,18,216,255);
	else drawImage(scr,img.back,x2,y2,1300,0,100,160,255);
	drawImage(scr,img.back,80,370,1280,220,80,110,255);
	drawImage(scr,img.back,x,y,a,660,60,120,255);
	if(count>30){
		drawImage(scr,img.back,440-x3,y3,1400,0,60,60,255);
		drawImage(scr,img.back,440+x3,y3,1400,0,60,60,255);
	}
}

void drawDeploma(SDL_Surface* scr){
	fillRect(scr,0,0,640,480,255,255,255,255);
	for(int i=0 ; i<16 ; i++){
		drawImage(scr,img.back,i*40,0,1060,800,40,40,255);
		drawImage(scr,img.back,i*40,440,1060,800,40,40,255);
	}
	for(int i=0 ; i<12 ; i++){
		drawImage(scr,img.back,0,i*40,1060,800,40,40,255);
		drawImage(scr,img.back,600,i*40,1060,800,40,40,255);
	}
	drawImage(scr,img.back,480,260,960,800,100,160,255);
	if(CHAR_CODE==JAPANESE)drawImage(scr,img.back,80,80,0,800,480,320,255);
	else drawImage(scr,img.back,80,80,480,800,480,320,255);
}

void drawMiyazaki(SDL_Surface* scr){
	if(phase==TOWERLIST || phase==SHOW_TOWERDATA || phase==WHICH_TOWER)drawTowerList(scr);
	else if(phase==COME_MIYAZAKI)drawComeMiyazaki(scr);
	else if(phase==LEAVE_MIYAZAKI)drawLeaveMiyazaki(scr);
	else if(phase==DEPLOMA)drawDeploma(scr);
	else if(phase==GOTO_MOVIE){
		fillRect(scr,0,0,640,480,0,0,0,255);
		for(int i=0 ; i<640 ; i++)fillRect(scr,i,216,1,42,0,0,255-(int)(i*0.4),255);
		drawText2(scr,60,220,text[MIYAZAKITEXT+menu[BGM_TEST].selected()+30]);
		if(count>=50)fillRect(scr,0,216,640,42,0,0,0,(count-50)*5);
	}
	else if(phase==GAMESTART){
		drawWeeklyComic(scr);
	}
	else if(phase==SUMMERWARS){
		drawSummerWars(scr);
	}
	else drawMiyazakiMuseum(scr);
	if(phase==GUIDE_STALIST || phase==GUIDE_STALIST_ALL)drawImage(scr,fishbox.panel,0,40,0,0,640,400,255);
	for(int i=0 ; i<20 ; i++){
		if(i==GUIDE_ALL || i==GUIDE_ANIME)menu[i].drawColorMenu(scr);
		else menu[i].drawMenu(scr);
	}
	drawTalking(scr);
	if(phase==NODATA1 || phase==NODATA2)drawText(scr,100,400,text[MENUTEXT+18]);
	drawMiyazakiExplain(scr);
	if(phase==GOTO_STROLL || phase==GOTO_TOWERLIST){
		drawImage(scr,img.menuback,0,180,0,0,640,120,128);
		drawText2(scr,240,200,text[MENUTEXT+16]);
		drawText2(scr,100,240,text[MENUTEXT+17]);
	}
	drawNetworkStatus(scr);
	if(showSearchImage){
		drawImage(scr,img.searchImage,0,0,0,0,640,480,255);
		if(strlen(tm.targetURL)>80){
			drawImage(scr,img.menuback,0,440,0,0,320,40,128);
			drawImage(scr,img.menuback,320,440,0,0,320,40,128);
			drawText(scr,0,440+start/2,tm.targetURL,80);
			drawText(scr,0,460+start/2,&tm.targetURL[80],80);
		}else{
			drawImage(scr,img.menuback,0,460,0,0,320,20,128);
			drawImage(scr,img.menuback,320,460,0,0,320,20,128);
			drawText(scr,0,460+start/2,tm.targetURL,80);
		}
	}
}

void drawMiyazakiExplain(SDL_Surface *scr){
	if(phase==COME_MIYAZAKI || phase==LEAVE_MIYAZAKI)return;
	if(phase==GOTO_MOVIE || phase==SUMMERWARS)return;
	if(EXPLAIN){
		if(phase==MIYAZAKI_MUSEUM){
			drawKeyboard(scr,key.leftC,0,0);
			drawKeyboard(scr,key.rightC,20,0);
			drawText(scr,40,0,text[GAMETEXT+12]);
			if(gd.x>180 && ((int)gd.x-180)%200<120){
				drawKeyboard(scr,key.zC,100,0);
				drawText(scr,120,0,text[OPTIONTEXT+1]);
			}
		}
		else if(phase==GUIDE_ALL || phase==GUIDE_ANIME || phase==GUIDE_STALIST_ALL || phase==PREF_LIST || phase==BGM_TEST || phase==PREF_LIST_TOWER || phase==SMR_DELETE_PREF){
			if(count%600<200){
				drawKeyboard(scr,key.upC,0,460);
				drawKeyboard(scr,key.downC,20,460);
				drawKeyboard(scr,key.leftC,40,460);
				drawKeyboard(scr,key.rightC,60,460);
				drawText(scr,80,460,text[MENUTEXT+15]);
			}
			else if(count%600<400){
				drawKeyboard(scr,key.zC,0,460);
				drawText(scr,20,460,text[OPTIONTEXT+1]);
			}else{
				drawKeyboard(scr,key.xC,0,460);
				drawText(scr,20,460,text[MENUTEXT+4]);
			}
		}
		else if(phase==GUIDE_STALIST || phase==GUIDE_STALIST_ALL || phase==GUIDE_PRGLIST || phase==GUIDE_TIME){
			if(count%400<200){
				drawKeyboard(scr,key.upC,0,460);
				drawKeyboard(scr,key.downC,20,460);
				drawKeyboard(scr,key.leftC,40,460);
				drawKeyboard(scr,key.rightC,60,460);
				drawText(scr,80,460,text[MENUTEXT+15]);
			}else{
				drawKeyboard(scr,key.xC,0,460);
				drawText(scr,20,460,text[MENUTEXT+4]);
			}
		}
		else if(phase==GAMESTART || phase==DEPLOMA){
			drawKeyboard(scr,key.zC,0,460);
			drawText(scr,20,460,text[MENUTEXT+4]);
		}
		else if(phase==TOWERLIST){
			drawKeyboard(scr,key.upC,180,460);
			drawKeyboard(scr,key.downC,200,460);
			drawKeyboard(scr,key.leftC,220,460);
			drawKeyboard(scr,key.rightC,240,460);
			drawText(scr,260,460,text[GAMETEXT+12]);
			if(this_tower[0]!=EOF){
				drawKeyboard(scr,key.zC,200,440);
				drawText(scr,220,440,text[MENUTEXT+15]);
			}
		}
		else if(phase==SHOW_TOWERDATA){
			drawKeyboard(scr,key.zC,240,460);
			drawKeyboard(scr,key.xC,260,460);
			drawText(scr,280,460,text[MENUTEXT+4]);
		}
		else if(phase==MIYAZAKI_TALK || phase==DEPLOMA_TALK){
			drawKeyboard(scr,key.zC,0,0);
			drawText(scr,20,0,text[EPILOGUE+1]);
		}
		else{
			if(count%600<200){
				drawKeyboard(scr,key.upC,0,0);
				drawKeyboard(scr,key.downC,20,0);
				drawText(scr,40,0,text[MENUTEXT+15]);
			}
			else if(count%600<400){
				drawKeyboard(scr,key.zC,0,0);
				drawText(scr,20,0,text[OPTIONTEXT+1]);
			}else{
				drawKeyboard(scr,key.xC,0,0);
				drawText(scr,20,0,text[MENUTEXT+4]);
			}
		}
	}
}

void makeGuideBook(){
	load_animebook();
	load_station();
	menu[GUIDE_ALL].setMenu(0,40,40,10,allworks);
	menu[GUIDE_ALL].lang=JAPANESE;
	size_t fc=0;
	loadFile("file/data/work/work_name_jp.dat");
	for(int i=0 ; i<allworks ; i++){
		if(animebook[i])menu[GUIDE_ALL].stack(&fstr[fc]);
		else menu[GUIDE_ALL].stack("----------");
		while(fstr[fc]!=0)fc++;
		fc++;
	}
	fc=0;
	loadFile("file/data/work/works.dat");
	for(int i=0 ; i<allworks ; i++){
		if(animebook[i]){
			menu[GUIDE_ALL].inputMark(i,to16int(fstr[fc],fstr[fc+1]));
			menu[GUIDE_ALL].inputColor(i,fstr[fc+2],fstr[fc+3],fstr[fc+4]);
		}
		fc+=5;
	}
	menu[GUIDE_ALL].setBG(192);
	char fn[50];

	if(works){
		delete [] work;
		delete [] prg;
	}
	if(prgs2){
		delete [] prgs2;
	}

	work=new Work[allworks];
	for(int i=0 ; i<allworks ; i++){
		work[i].tnum=i;
	}
	works=allworks;
	load_searchQueries();

	prgs=0;
	prgs2=new int[index_num];
	int *works2;
	works2=new int[index_num];
	for(int i=0 ; i<index_num ; i++){
		sprintf_s(fn,"file/data/work/work%d.dat",i+1);
		loadFile(fn);
		fc=0;
		works2[i]=0;
		while(fc<fsize){
			fc+=2;
			while(fstr[fc]!=EOF || fstr[fc+1]!=EOF){
				prgs++;
				fc+=6;
			}
			works2[i]++;
			fc+=2;
		}
	}

	prg=new Prg[prgs];
	prgs=0;
	int w;
	for(int i=0 ; i<index_num ; i++){
		sprintf_s(fn,"file/data/work/work%d.dat",i+1);
		loadFile(fn);
		fc=0;
		prgs2[i]=0;
		for(int j=0 ; j<works2[i] ; j++){
			w=to16int(fstr[fc],fstr[fc+1]);
			fc+=2;
			while(fstr[fc]!=EOF || fstr[fc+1]!=EOF){
				prg[prgs].work=w;
				prg[prgs].station=to16int(fstr[fc],fstr[fc+1]);
				fc+=2;
				prg[prgs].week=fstr[fc];
				fc++;
				prg[prgs].hour=fstr[fc];
				fc++;
				prg[prgs].minute=fstr[fc];
				fc++;
				prg[prgs].time=to8int(fstr[fc]);
				fc++;
				prgs++;
				prgs2[i]++;
			}
			fc+=2;
		}
	}

	delete [] works2;
	for(int i=index_num-1 ; i>0 ; i--){
		prgs2[i]=0;
		for(int j=0 ; j<i ; j++)prgs2[i]+=prgs2[j];
	}
	prgs2[0]=0;
}

void changeScreenColor(SDL_Surface* scr){
	Uint32 *px;
	SDL_Color col;
	int Y;
	SDL_LockSurface(scr);

	px = (Uint32*)scr->pixels;

	for(int i=0 ; i<640*480 ; i++){
		getRGB( *px, &col.r, &col.g, &col.b);
		Y=257*col.r+504*col.g+98*col.b+16000;
		if(scr_design==MONO){
			if(Y<96000){col.r=0;col.g=0;col.b=0;}
			else{col.r=255;col.g=255;col.b=255;}
		}
		else if(scr_design==GAMEBOY){
			if(Y>160000){col.r=190;col.g=170;col.b=40;}
			else if(Y>96000){col.r=145;col.g=185;col.b=85;}
			else if(Y>32000){col.r=100;col.g=135;col.b=85;}
			else{col.r=40;col.g=80;col.b=75;}
		}
		*px=setRGB( col.r, col.g, col.b);
		px++;
	}

	SDL_UnlockSurface(scr);
}
