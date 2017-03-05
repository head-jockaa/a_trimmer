#include "util.h"

int *prgs_of_each_season;
int this_area[10],this_tower[10];
int shopPlayerX,shopScrX;
Uint8 shopPlayerDir;
int which_tower;
bool talk_3dtv=false, talk_seoiha=false,movie_test=false;
bool showSearchImage=false;
Uint8 scr_design=NULL;

void makeGuideBook();
void drawMiyazakiExplain(SDL_Surface *scr);

void initMiyazaki(){
	count=0;start=0;
	mode=MIYAZAKI;
	phase=COME_MIYAZAKI;
	prgs_of_each_season=NULL;
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
	menu[GUIDE_SEASON].setMenu(100,80,10,8,season_num);
	for(int i=0 ; i<season_num ; i++)menu[GUIDE_SEASON].stack(season[i].name);
	menu[BGM_TEST].setMenu(40,120,25,7,37);
	for(int i=1 ; i<=NUM_OF_BGM ; i++){
		sprintf_s(str,"Music No.%2d",i);
		menu[BGM_TEST].stack(str);
	}
	for(int i=0 ; i<16 ; i++){//‰º‚R‚Â‚ÌƒAƒjƒ‚Í•Û—¯
		menu[BGM_TEST].stack(text[MIYAZAKITEXT+52+i]);
	}
	getImage(img.back,"file/img/miyazaki.png",BLUE);
	loadCartoon(&cartoonJson, "file/data/cartoon/miyazaki_in.json");
	loadCartoon(&talkingJson, "file/data/cartoon/miyazaki.json");
	readCartoon(&talkingJson,0);
	bgm=Mix_LoadMUS("file/bgm/17.ogg");
	Mix_PlayMusic(bgm,-1);
}

void backToMiyazaki(){
	initMiyazaki();
	readCartoon(&cartoonJson,1);
	phase=MIYAZAKI_MUSEUM;
}

void endMiyazaki(){
	movie_test=false;
	freeMusic();
	freeImage(img.back);
	freeCartoon(&cartoonJson);
	freeCartoon(&talkingJson);
	if(prgs_of_each_season)delete [] prgs_of_each_season;
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
		readCartoon(&talkingJson,19);
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
		sprintf_s(str,"file/data/cartoon/weekly%d.json",gd.week);
		loadCartoon(&cartoonJson, str);
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
		initOpeningAnime();
	}
	else if(n==12){
		initGameMenu();
		loadCartoon(&cartoonJson, "file/data/cartoon/story1.json");
		phase=PROLOGUE;
	}
	else if(n==13){
		initEndingAnime();
	}
	else if(n==14){
		initGameMenu();
		loadCartoon(&cartoonJson, "file/data/cartoon/story13.json");
		phase=PROLOGUE;
	}
	kick_count=0;
}

void timerMiyazaki(){
	if(nextCut(&cartoonJson)){
		if(phase==COME_MIYAZAKI){
			readCartoon(&cartoonJson,1);
			phase=MIYAZAKI_MUSEUM;
			start=200;
			shopPlayerX=30;
			shopScrX=0;
			shopPlayerDir=1;
		}
		else if(phase==LEAVE_MIYAZAKI){
			endMiyazaki();
			initGameMenu();
		}
	}
	if(phase==GOTO_STROLL && count==1)gotoStroll();
	else if(phase==GOTO_TOWERLIST && count==1)gotoTowerList();
	else if(phase==GOTO_MOVIE && count==100)gotoMovieTest();
	else if(phase==SEOI_HA)timerSeoiHa();
	else if(phase==GAMESTART){
		nextCut(&cartoonJson);
	}

	if(phase==MIYAZAKI_TALK){
		nextCut(&talkingJson);
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
		if(shopPlayerX>180 && ((int)shopPlayerX-180)%200<120){
			phase=YNFORM;
			menu[YNFORM].setViewMode(VISIBLE);
			if(shopPlayerX>180 && shopPlayerX<300){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+14]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+15]);
			}
			if(shopPlayerX>380 && shopPlayerX<500){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+16]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+17]);
			}
			if(shopPlayerX>580 && shopPlayerX<700){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+18]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+19]);
			}
			if(shopPlayerX>780 && shopPlayerX<900){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+20]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+21]);
			}
			if(shopPlayerX>980 && shopPlayerX<1100){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+16]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+17]);
			}
			if(shopPlayerX>1180 && shopPlayerX<1300 && gd.bought[6]){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+16]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+17]);
			}
			if(shopPlayerX>1380 && shopPlayerX<1500 && gd.bought[15]){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+22]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+23]);
			}
			if(shopPlayerX>1580 && shopPlayerX<1700 && gd.bought[12]){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+16]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+17]);
			}
			if(shopPlayerX>1780 && shopPlayerX<1900 && gd.bought[16]){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+24]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+25]);
			}
			if(shopPlayerX>1980 && shopPlayerX<2100 && gd.bought[9]){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+16]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+17]);
			}
			if(shopPlayerX>2180 && shopPlayerX<2300 && gd.bought[11]){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+18]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+19]);
			}
			if(shopPlayerX>2380 && shopPlayerX<2500){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+26]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+27]);
			}
			if(shopPlayerX>2580 && shopPlayerX<2700){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+28]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+29]);
			}
			if(shopPlayerX>2780 && shopPlayerX<2900){
				menu[YNFORM].input(1,text[MIYAZAKITEXT+18]);
				menu[YNFORM].input(2,text[MIYAZAKITEXT+19]);
			}
		}
	}
	if(key.up && !key_stop(key.up)){
		shopPlayerDir=2;
	}
	if(key.down && !key_stop(key.down)){
		shopPlayerDir=0;
	}
	if(key.left){
		shopPlayerX-=6;
		shopPlayerDir=3;
	}
	if(key.right){
		shopPlayerX+=6;
		if(shopPlayerX>2800)shopPlayerX=2800;
		shopPlayerDir=1;
	}
	shopScrX=(int)shopPlayerX-320;
	if(shopScrX<0)shopScrX=0;
	if(shopScrX>2160)shopScrX=2160;
	if(talk_seoiha && std::abs(shopPlayerX-1440)>200){
		readCartoon(&talkingJson,24);
		phase=MIYAZAKI_TALK;
		talk_seoiha=false;
	}
	if(shopPlayerX<0){
		phase=LEAVE_MIYAZAKI;
		loadCartoon(&cartoonJson, "file/data/cartoon/miyazaki_out.json");
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
		if(collection==animedex_num){
			phase=MIYAZAKI_TALK;
			readCartoon(&talkingJson,25);
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
			menu[GUIDE_STALIST].setMenu(10,30,30,9,allofworks[n].prg_num+season_num*2);
			int line_num=0;
			String s;
			bool *on_air=new bool[season_num];
			for(int i=0; i<season_num ; i++){
				on_air[i]=false;
				for(int j=0; j<allofworks[n].prg_num ; j++){
					if(allofworks[n].prg[j].season_index==i){
						on_air[i]=true;
						break;
					}
				}
			}
			for(int i=0; i<season_num ; i++){
				if(!on_air[i])continue;
				sprintf_s(s.str[0],"=== %s ===",season[i].name.str[0]);
				sprintf_s(s.str[1],"=== %s ===",season[i].name.str[1]);
				menu[GUIDE_STALIST].stack(s);
				for(int j=0; j<allofworks[n].prg_num ; j++){
					if(allofworks[n].prg[j].season_index!=i)continue;
					sprintf_s(s.str[0],"%2d:%2d(%s)%s",allofworks[n].prg[j].hour,allofworks[n].prg[j].minute,weekChar[allofworks[n].prg[j].week][0],sta[allofworks[n].prg[j].station_index].name.str[0]);
					sprintf_s(s.str[1],"%2d:%2d(%s)%s",allofworks[n].prg[j].hour,allofworks[n].prg[j].minute,weekChar[allofworks[n].prg[j].week][1],sta[allofworks[n].prg[j].station_index].name.str[1]);
					menu[GUIDE_STALIST].stack(s);
					menu[GUIDE_STALIST].inputMark(line_num,sta[allofworks[n].prg[j].station_index].mark);
				}
				menu[GUIDE_STALIST].stack("");
			}
			delete [] on_air;
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
			startThread(allofworks[menu[GUIDE_ALL].selected()].cartoon_id, allofworks[menu[GUIDE_ALL].selected()].query);
		}
	}
	if(key.up && !key_wait(key.up))menu[GUIDE_ALL].cursorUp();
	if(key.down && !key_wait(key.down))menu[GUIDE_ALL].cursorDown();
	if(key.left && !key_wait(key.left))menu[GUIDE_ALL].pageUp();
	if(key.right && !key_wait(key.right))menu[GUIDE_ALL].pageDown();
}

void keyGuideSeason(){
	if(key.z && !key_stop(key.z)){
		which_season=season[menu[GUIDE_SEASON].selected()].id;
		load_entries(which_season);
		if(menu[GUIDE_TOP].selected()==1){
			menu[GUIDE_ANIME].setMenu(0,20,40,10,entries);
			menu[GUIDE_ANIME].lang=JAPANESE;
			for(int i=0 ; i<entries ; i++){
				if(animebook[entry[i].cartoon_index]){
					menu[GUIDE_ANIME].stack(entry[i].title);
					menu[GUIDE_ANIME].inputMark(i,entry[i].mark);
					menu[GUIDE_ANIME].inputColor(i,entry[i].r,entry[i].g,entry[i].b);
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
			int line_num=0;
			for(int i=0 ; i<entries ; i++){
				line_num+=entry[i].prg_num;
			}
			menu[GUIDE_TIME].setMenu(0,20,40,10,line_num*2);
			menu[GUIDE_TIME].setCombo(2);
			menu[GUIDE_TIME].lang=JAPANESE;
			String s;
			for(int w=0 ; w<7 ; w++)for(int h=4 ; h<=27 ; h++)for(int m=0 ; m<60 ; m++){
				for(int i=0 ; i<entries ; i++)for(int j=0 ; j<entry[i].prg_num ; j++){
					if(entry[i].prg[j].week==w && entry[i].prg[j].hour==h && entry[i].prg[j].minute==m){
						if(animebook[entry[i].cartoon_index]){
							for(int k=0 ; k<2 ; k++)sprintf_s(s.str[k],"%s",entry[i].title.str[k]);
							menu[GUIDE_TIME].stack(s);
						}else{
							menu[GUIDE_TIME].stack("----------");
						}
						for(int k=0 ; k<2 ; k++){
							sprintf_s(s.str[k],"    (%s)%2d:%2d %s",weekChar[entry[i].prg[j].week][k],h,m,sta[entry[i].prg[j].station_index].name.str[k]);
						}
						menu[GUIDE_TIME].stack(s);
					}
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
		int n=menu[PREF_LIST_MIYAZAKI].selected();
		menu[GUIDE_STA].setMenu(0,20,40,10,area[n].st_num);
		for(int i=0 ; i<area[n].st_num ; i++){
			menu[GUIDE_STA].stack(sta[area[n].station[i]].name);
			menu[GUIDE_STA].inputMark(i,sta[area[n].station[i]].mark);
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
		if(animebook[entry[n].cartoon_index]){
			String s;
			menu[GUIDE_STALIST].setMenu(20,60,30,9,entry[n].prg_num);
			for(int i=0; i<entry[n].prg_num ; i++){
				sprintf_s(s.str[0],"%2d:%2d(%s)%s",entry[n].prg[i].hour,entry[n].prg[i].minute,weekChar[entry[n].prg[i].week][0],sta[entry[n].prg[i].station_index].name.str[0]);
				sprintf_s(s.str[1],"%2d:%2d(%s)%s",entry[n].prg[i].hour,entry[n].prg[i].minute,weekChar[entry[n].prg[i].week][1],sta[entry[n].prg[i].station_index].name.str[1]);
				menu[GUIDE_STALIST].stack(s);
				menu[GUIDE_STALIST].inputMark(i,sta[entry[n].prg[i].station_index].mark);
			}
			menu[GUIDE_STALIST].setBG(0);
			fishbox.panelColor(entry[n].r,entry[n].g,entry[n].b);
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
		int cartoon_num=0;
		String s;
		for(int i=0; i<entries ; i++){
			for(int j=0; j<entry[i].prg_num ; j++){
				if(entry[i].prg[j].station_index==n)cartoon_num++;
			}
		}
		int *work_idx=new int[cartoon_num];
		int *prg_idx=new int[cartoon_num];
		menu[GUIDE_PRGLIST].setMenu(20,60,30,9,cartoon_num);
		menu[GUIDE_PRGLIST].lang=JAPANESE;
		cartoon_num=0;
		for(int i=0; i<entries ; i++){
			for(int j=0; j<entry[i].prg_num ; j++){
				if(entry[i].prg[j].station_index==n){
					work_idx[cartoon_num]=i;
					prg_idx[cartoon_num]=j;
					cartoon_num++;
				}
			}
		}

		for(int i=cartoon_num-2 ; i>=0 ; i--)for(int j=cartoon_num-1 ; j>i ; j--){
			int a1=work_idx[i];
			int b1=prg_idx[i];
			int a2=work_idx[j];
			int b2=prg_idx[j];
			if(entry[a1].prg[b1].week>entry[a2].prg[b2].week ||
			   (entry[a1].prg[b1].week==entry[a2].prg[b2].week && entry[a1].prg[b1].hour>entry[a2].prg[b2].hour) ||
			   (entry[a1].prg[b1].week==entry[a2].prg[b2].week && entry[a1].prg[b1].hour==entry[a2].prg[b2].hour && entry[a1].prg[b1].minute>entry[a2].prg[b2].minute)
			  ){
				int c=work_idx[i];
				work_idx[i]=work_idx[j];
				work_idx[j]=c;
				c=prg_idx[i];
				prg_idx[i]=prg_idx[j];
				prg_idx[j]=c;
			}
		}

		for(int i=0 ; i<cartoon_num ; i++){
			int a=work_idx[i];
			int b=prg_idx[i];
			if(animebook[entry[a].cartoon_index]){
				sprintf_s(s.str[0],"%2d:%2d(%s)%s",entry[a].prg[b].hour,entry[a].prg[b].minute,weekChar[entry[a].prg[b].week][0],entry[a].title.str[0]);
				sprintf_s(s.str[1],"%2d:%2d(%s)%s",entry[a].prg[b].hour,entry[a].prg[b].minute,weekChar[entry[a].prg[b].week][1],entry[a].title.str[1]);
			}else{
				sprintf_s(s.str[0],"%2d:%2d(%s)----------",entry[a].prg[b].hour,entry[a].prg[b].minute,weekChar[entry[a].prg[b].week][0]);
				sprintf_s(s.str[1],"%2d:%2d(%s)----------",entry[a].prg[b].hour,entry[a].prg[b].minute,weekChar[entry[a].prg[b].week][1]);
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
		for(int i=0 ; i<areas ; i++){
			for(int j=0 ; j<area[i].tower_num ; j++){
				area[i].tower[j].remove=false;
				if(i!=menu[PREF_LIST_MIYAZAKI].selected())area[i].tower[j].remove=true;
				if(menu[GUIDE_STA].selected()!=0){
					if(area[i].tower[j].ch[menu[GUIDE_STA].selected()-1]==0){
						area[i].tower[j].remove=true;
					}
					if(area[i].tower[j].ch[menu[GUIDE_STA].selected()-1]==CHANNELS+1){
						area[i].tower[j].remove=true;
					}
				}
				if(i==menu[PREF_LIST_MIYAZAKI].selected() && !area[i].tower[j].remove){
					gd.x=area[i].tower[j].x*2;
					gd.y=area[i].tower[j].y*2;
					break;
				}
			}
		}
		for(int i=0 ; i<10 ; i++){
			this_area[i]=EOF;
			this_tower[i]=EOF;
		}
		int n=0;
		for(int i=0 ; i<areas ; i++){
			for(int j=0 ; j<area[i].tower_num ; j++)if(!area[i].tower[j].remove){
				if(area[i].tower[j].x==gd.x && area[i].tower[j].y==gd.y){
					this_area[n]=i;
					this_tower[n]=j;
					if(n==9)break;
					n++;
				}
			}
		}
		MAGNIFY=2;
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
			String s;
			menu[WHICH_TOWER].setMenu(60,60,20,5,n);
			for(int i=0 ; i<n ; i++){
				sprintf_s(s.str[0],"%s %.6gkw",area[this_area[i]].tower[this_tower[i]].name.str[0],area[this_area[i]].tower[this_tower[i]].kw);
				sprintf_s(s.str[1],"%s %.6gkw",area[this_area[i]].tower[this_tower[i]].name.str[1],area[this_area[i]].tower[this_tower[i]].kw);
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
		for(int i=0 ; i<areas ; i++){
			for(int j=0 ; j<area[i].tower_num ; j++){
				area[i].tower[j].remove=false;
			}
		}
		if(NHK_REMOVE){
			bool ok;
			for(int i=0 ; i<areas ; i++){
				for(int j=0 ; j<area[i].tower_num ; j++)if(area[i].tower[j].kw<1){
					ok=false;
					for(int k=0 ; k<10 ; k++)if(area[i].tower[j].ch[k]!=0){
						if(sta[area[i].station[k]].mark!=5 && sta[area[i].station[k]].mark!=6){
							ok=true;break;
						}
					}
					if(ok)area[i].tower[j].remove=false;
					else area[i].tower[j].remove=true;
				}
			}
		}
		MAGNIFY=pre_magnify;
		createMap_tower();
		menu[GUIDE_STA].setViewMode(VISIBLE);
		phase=STA_LIST_TOWER;
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
		for(int i=0 ; i<10 ; i++){
			this_area[i]=EOF;
			this_tower[i]=EOF;
		}
		int n=0;
		for(int i=0 ; i<areas ; i++){
			for(int j=0 ; j<area[i].tower_num ; j++)if(!area[i].tower[j].remove){
				if(std::abs(area[i].tower[j].x*MAGNIFY-gd.x)<2 && std::abs(area[i].tower[j].y*MAGNIFY-gd.y)<2){
					this_area[n]=i;
					this_tower[n]=j;
					if(n==9)break;
					n++;
				}
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
		if(nextTalk(&talkingJson)){
			if(shopPlayerX>180 && shopPlayerX<300 && menu[YNFORM].selected()==1){
				endMiyazaki();
				initHaziaShop();
				kick_count++;
			}
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
				load_season();
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
			phase=MIYAZAKI_TALK;
			int a=((int)shopPlayerX-180)/200;
			if((a==6 && !gd.bought[15]) || (a==8 && !gd.bought[16]) || (a==5 && !gd.bought[6])
			   || (a==9 && !gd.bought[9]) || (a==10 && !gd.bought[11]) || (a==7 && !gd.bought[12])){
				readCartoon(&talkingJson,1);
			}else{
				readCartoon(&talkingJson,a+2);
				if(a==9)talk_3dtv=true;
			}
		}
		if(menu[YNFORM].selected()==1){
			phase=MIYAZAKI_MUSEUM;
			if(shopPlayerX>180 && shopPlayerX<300){
				phase=MIYAZAKI_TALK;
				readCartoon(&talkingJson,16);
			}
			if(shopPlayerX>380 && shopPlayerX<500){
				phase=GUIDE_TOP;
				menu[GUIDE_TOP].setViewMode(VISIBLE);
				load_animebook();
			}
			if(shopPlayerX>580 && shopPlayerX<700){
				gd.game_mode=WALKING;
				phase=GOTO_STROLL;
				count=0;
			}
			if(shopPlayerX>780 && shopPlayerX<900){
				phase=MIYAZAKI_TALK;
				readCartoon(&talkingJson,17);
			}
			if(shopPlayerX>980 && shopPlayerX<1100){
				phase=MIYAZAKI_TALK;
				readCartoon(&talkingJson,18);
			}
			if(shopPlayerX>1180 && shopPlayerX<1300 && gd.bought[6]){
				load_towers();
				load_station();
				menu[PREF_LIST_MIYAZAKI].setMenu(100,40,12,10,areas-1);
				for(int i=0 ; i<areas-1 ; i++)menu[PREF_LIST_MIYAZAKI].stack(area[i].name);
				phase=PREF_LIST_TOWER;
				menu[PREF_LIST_MIYAZAKI].setViewMode(VISIBLE);
			}
			if(shopPlayerX>1380 && shopPlayerX<1500 && gd.bought[15]){
				Mix_PlayChannel(1,sf.decide,0);
				shopPlayerDir=2;
				phase=SEOI_HA;
				count=0;
			}
			if(shopPlayerX>1580 && shopPlayerX<1700 && gd.bought[12]){
				phase=BGM_TEST;
				menu[BGM_TEST].setViewMode(VISIBLE);
			}
			if(shopPlayerX>1780 && shopPlayerX<1900 && gd.bought[16]){
				if(scr_design==MONO){
					scr_design=NULL;
					phase=MIYAZAKI_MUSEUM;
				}else{
					scr_design=MONO;
					phase=MIYAZAKI_TALK;
					readCartoon(&talkingJson,20);
				}
			}
			if(shopPlayerX>1980 && shopPlayerX<2100 && gd.bought[9]){
				gd.game_mode=WALKING;
				MAP3D=true;
				phase=GOTO_STROLL;
				count=0;
			}
			if(shopPlayerX>2180 && shopPlayerX<2300 && gd.bought[11]){
				gd.game_mode=NO_RELAY;
				phase=GOTO_STROLL;
				count=0;
			}
			if(shopPlayerX>2380 && shopPlayerX<2500){
				phase=MIYAZAKI_TALK;
				readCartoon(&talkingJson,21);
			}
			if(shopPlayerX>2580 && shopPlayerX<2700){
				phase=DELETE_MENU;
				menu[DELETE_MENU].setViewMode(VISIBLE);
			}
			if(shopPlayerX>2780 && shopPlayerX<2900){
				phase=MIYAZAKI_TALK;
				readCartoon(&talkingJson,22);
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
		case MIYAZAKI_TALK:keyMiyazakiTalk();break;
		case YNFORM:keyYNForm();break;
		default:break;
	}
	if(phase==GAMESTART && key.z && !key_stop(key.z)){
		freeCartoon(&cartoonJson);
		phase=MIYAZAKI_MUSEUM;
		freeImage(img.back);
		getImage(img.back,"file/img/miyazaki.png",BLUE);
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
		drawText2(scr,0,0,tower[this_tower[which_tower]].name);
		sprintf_s(str,"%.6gkw",tower[this_tower[which_tower]].kw);
		drawText2(scr,0,40,str);
		for(int i=0 ; i<10 ; i++){
			drawImage(scr,img.symbol,0,i*40+80,
					  (sta[area[ this_area[which_tower] ].station[i]].mark%17)*34,
					  (sta[area[ this_area[which_tower] ].station[i]].mark/17)*34,
					  34,34,255);
			drawText2(scr,40,i*40+80,sta[area[ this_area[which_tower] ].station[i]].name);
			if(area[this_area[which_tower]].tower[this_tower[which_tower]].ch[i]==0)drawText2(scr,400,i*40+80,"----");
			else if(area[this_area[which_tower]].tower[this_tower[which_tower]].ch[i]==CHANNELS+1)drawText(scr,400,i*40+80,"****");
			else{
				sprintf_s(str,"ch%2d",area[this_area[which_tower]].tower[this_tower[which_tower]].ch[i]);
				drawText2(scr,400,i*40+80,str);
			}
		}
		if(area[this_area[which_tower]].tower[this_tower[which_tower]].v)drawImage(scr,img.back,560,0,1240,640,80,80,255);
		else drawImage(scr,img.back,560,0,1240,560,80,80,255);
		if(((count/20)%4)!=3){
			for(int i=0 ; i<8 ; i++){
				if(((area[this_area[which_tower]].tower[this_tower[which_tower]].bias>>i)&1)==1){
					drawImage(scr,img.back,480,0,600+i*80,560+((count/20)%4)*80,80,80,255);
				}
			}
		}
	}else{
		for(int i=0 ; i<10 ; i++){
			if(this_tower[i]==EOF)break;
			drawText2(scr,0,i*40,area[this_area[i]].tower[this_tower[i]].name);
		}
	}
}

void drawMiyazakiMuseum(SDL_Surface* scr){
	cartoonJson.scrX=shopScrX;
	drawAnimationCut(&cartoonJson,scr);
	if(start>0){
		int a=255;
		if(start>180)a=(200-start)*13;
		if(start<20)a=start*13;
		drawText2(scr,start/3,120,text[MIYAZAKITEXT+71],100,a);
		drawText2(scr,50+start/2,200,text[MIYAZAKITEXT+72],100,a);
	}
	for(int i=0 ; i<14 ; i++){
		if((i==7 && !gd.bought[15]) || (i==9 && !gd.bought[16]) || (i==6 && !gd.bought[6])
		   || (i==10 && !gd.bought[9]) || (i==11 && !gd.bought[11]) || (i==8 && !gd.bought[12])){
			drawImage(scr,img.back,i*200-shopScrX,400,0,480,80,80,255);
		}
		else drawImage(scr,img.back,i*200-shopScrX,400,80+i*80,480,80,80,255);
	}
	if(phase==SEOI_HA){
		drawImage(scr,img.back,1400-shopScrX,380,1200+(count/30)*80,480,80,80,255);
	}
	drawImage(scr,img.chr,(int)shopPlayerX-shopScrX-14,420,shopPlayerDir*30,0,30,60,255);
	if(key.left || key.right)drawImage(scr,img.chr,(int)shopPlayerX-shopScrX-26,440,((count/5)%2)*60,110,60,60,255);
	if(phase==GUIDE_ALL){
		sprintf_s(str,"%4d/%4d",menu[GUIDE_ALL].selected()+1,animedex_num);
		drawText2(scr,0,0,str,(int)strlen(str));
	}
	if(phase==GUIDE_TOP){
		drawImage(scr,img.back,300,240,0,660,240,120,255);
		sprintf_s(str,"%4d/%4d",collection,animedex_num);
		drawText2(scr,380,300,str);
	}
}

void drawMiyazaki(SDL_Surface* scr){
	if(phase==TOWERLIST || phase==SHOW_TOWERDATA || phase==WHICH_TOWER)drawTowerList(scr);
	else if(phase==COME_MIYAZAKI || phase==LEAVE_MIYAZAKI)drawAnimationCut(&cartoonJson,scr);
	else if(phase==GOTO_MOVIE){
		fillRect(scr,0,0,640,480,0,0,0,255);
		for(int i=0 ; i<640 ; i++)fillRect(scr,i,216,1,42,0,0,255-(int)(i*0.4),255);
		drawText2(scr,60,220,text[MIYAZAKITEXT+menu[BGM_TEST].selected()+30]);
		if(count>=50)fillRect(scr,0,216,640,42,0,0,0,(count-50)*5);
	}
	else if(phase==GAMESTART){
		drawAnimationCut(&cartoonJson,scr);
	}
	else drawMiyazakiMuseum(scr);
	if(phase==GUIDE_STALIST || phase==GUIDE_STALIST_ALL)drawImage(scr,fishbox.panel,0,40,0,0,640,400,255);
	for(int i=0 ; i<20 ; i++){
		if(i==GUIDE_ALL || i==GUIDE_ANIME)menu[i].drawColorMenu(scr);
		else menu[i].drawMenu(scr);
	}
	if(phase==MIYAZAKI_TALK){
		drawAnimationCut(&talkingJson, scr);
	}
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
	if(phase==GOTO_MOVIE)return;
	if(EXPLAIN){
		if(phase==MIYAZAKI_MUSEUM){
			drawKeyboard(scr,key.leftC,0,0);
			drawKeyboard(scr,key.rightC,20,0);
			drawText(scr,40,0,text[GAMETEXT+12]);
			if(shopPlayerX>180 && ((int)shopPlayerX-180)%200<120){
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
		else if(phase==GAMESTART){
			drawKeyboard(scr,key.zC,0,460);
			drawText(scr,20,460,text[MENUTEXT+4]);
		}
		else if(phase==TOWERLIST){
			drawKeyboard(scr,key.upC,360,460);
			drawKeyboard(scr,key.downC,380,460);
			drawKeyboard(scr,key.leftC,400,460);
			drawKeyboard(scr,key.rightC,420,460);
			drawText(scr,440,460,text[GAMETEXT+12]);
			if(this_tower[0]!=EOF){
				drawKeyboard(scr,key.zC,400,440);
				drawText(scr,420,440,text[MENUTEXT+15]);
			}
		}
		else if(phase==SHOW_TOWERDATA){
			drawKeyboard(scr,key.zC,480,460);
			drawKeyboard(scr,key.xC,500,460);
			drawText(scr,520,460,text[MENUTEXT+4]);
		}
		else if(phase==MIYAZAKI_TALK){
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
	readSQL("file/data/sql/cartoon.sql");
	menu[GUIDE_ALL].setMenu(0,40,40,10,(int)allofworks_num);
	menu[GUIDE_ALL].lang=JAPANESE;
	for(int i=0 ; i<allofworks_num ; i++){
		if(animebook[i])menu[GUIDE_ALL].stack(allofworks[i].title.str[0]);
		else menu[GUIDE_ALL].stack("----------");
	}
	for(int i=0 ; i<allofworks_num ; i++){
		if(animebook[i]){
			menu[GUIDE_ALL].inputMark(i,allofworks[i].mark);
			menu[GUIDE_ALL].inputColor(i,allofworks[i].r,allofworks[i].g,allofworks[i].b);
		}
	}
	menu[GUIDE_ALL].setBG(192);
	if(prgs_of_each_season){
		delete [] prgs_of_each_season;
	}
	prgs_of_each_season=new int[season_num];
	for(int i=0 ; i<season_num ; i++){
		sprintf_s(str,"file/data/sql/timetable%d.sql",season[i].id);
		readSQL(str);
		prgs_of_each_season[i]=prgs;
		for(int j=0 ; j<prgs ; j++){
			allofworks[prg[j].cartoon_index].prg_num++;
		}
	}
	for(int i=0 ; i<allofworks_num ; i++){
		allofworks[i].prg=new Program[allofworks[i].prg_num];
		allofworks[i].prg_num=0;
	}
	for(int i=0 ; i<season_num ; i++){
		sprintf_s(str,"file/data/sql/timetable%d.sql",season[i].id);
		readSQL(str);
		for(int j=0 ; j<prgs ; j++){
			int n=allofworks[prg[j].cartoon_index].prg_num;
			allofworks[prg[j].cartoon_index].prg[n].season_index=prg[j].season_index;
			allofworks[prg[j].cartoon_index].prg[n].station_index=prg[j].station_index;
			allofworks[prg[j].cartoon_index].prg[n].week=prg[j].week;
			allofworks[prg[j].cartoon_index].prg[n].hour=prg[j].hour;
			allofworks[prg[j].cartoon_index].prg[n].minute=prg[j].minute;
			allofworks[prg[j].cartoon_index].prg[n].time=prg[j].time;
			allofworks[prg[j].cartoon_index].prg_num++;
		}
	}
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
