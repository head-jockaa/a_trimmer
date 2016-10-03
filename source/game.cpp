#include "game.h"

GameData gd;
BossData bd;
ManekiData md;
int pre_magnify;

int mwave[68]={
11,4,0,0,12,10,2,0,13,11,7,0,11,12,10,3,10,12,11,8,8,11,12,10,
2,9,11,12,0,3,10,12,3,0,7,11,4,3,2,7,3,4,1,4,1,4,3,0,1,3,4,3,
0,1,3,4,0,0,2,4,0,0,0,3,0,0,0,1
};

void getH();
void setTmpFish_bs();
void shiftFish_maneki();
void receiveBS();
void televise();
void toStory();
int getMissing();
void ManekiTV_throw();
void ManekiTV_catch();
BOOL checkManekiTV();
void save_game(int n);
void load_game();
void drawPlayer(SDL_Surface *scr);
void drawFishup(SDL_Surface *scr);
void drawGameExplain(SDL_Surface *scr);
void drawSMR(SDL_Surface* scr);
void estimate_rural();
void initManekiTV();
void setManekiData();
void boss_attack();
void timer_boss();

void setAntenna(){
	if(ROD_TYPE==SIMPLEROD){
		ant=new SimpleRod();
		getImage(img.rod,"file/img/srod.png",255,255,255);
	}
	else if(ROD_TYPE==BUTTONROD){
		ant=new ButtonRod();
		getImage(img.rod,"file/img/brod.png",0,0,255);
	}
	else if(ROD_TYPE==UVROD){
		ant=new UVRod();
		getImage(img.rod,"file/img/uvrod.png",0,0,255);
	}
	else if(ROD_TYPE==MHZROD){
		getImage(img.rod,"file/img/mhzrod.png",0,0,255);
		ant=new MHzRod();
	}
	else if(ROD_TYPE==CONVENIENTROD)ant=new ConvenientRod();
	else if(ROD_TYPE==SUPERHANDYROD)ant=new SuperHandyRod();
}

void initGame(){
	mode=GAME;
	load_station();

	img.colorlight=new Image(640,240);
	makeColorLight();

	getImage(img.fishup,"file/img/fishup.png",0,0,255);
	sf.water=Mix_LoadWAV("file/se/9.wav");
	sf.thunder=Mix_LoadWAV("file/se/14.wav");
	sf.meow=Mix_LoadWAV("file/se/16.wav");
	sf.snore=Mix_LoadWAV("file/se/10.wav");
	sf.fw_sound=Mix_LoadWAV("file/se/18.ogg");
	sf.noon=Mix_LoadWAV("file/se/11.ogg");
	sf.sunset=Mix_LoadWAV("file/se/15.ogg");
	sf.hold=Mix_LoadWAV("file/se/5.wav");
	sf.drag_s=Mix_LoadWAV("file/se/4.wav");
	sf.noize=Mix_LoadWAV("file/se/8.wav");
	sf.bupyo=Mix_LoadWAV("file/se/6.wav");
	sf.knob=Mix_LoadWAV("file/se/7.wav");
	sf.calling=Mix_LoadWAV("file/se/12.wav");
	sf.coin=Mix_LoadWAV("file/se/17.wav");
	sf.swish=Mix_LoadWAV("file/se/25.wav");
	sf.get=Mix_LoadWAV("file/se/26.wav");
	sf.lamp=Mix_LoadWAV("file/se/27.wav");
	sf.bubble=Mix_LoadWAV("file/se/28.wav");
	sf.gaze=Mix_LoadWAV("file/se/29.wav");

	count=0;
	gd.x=gd.startX;gd.y=gd.startY;
	gd.zoom=15;
	gd.height=10;
	fix_scrXY();
	pre_magnify=EOF;
	if(MAP3D){
		pre_magnify=MAGNIFY;
		MAGNIFY=1;
	}
	gd.week=0;gd.hour=4;gd.minute=0;gd.second=0;gd.score=0;gd.get_score=0;gd.gradeup=0;gd.crops=0;gd.player_dir=0;
	gd.ta_count=0;start=75;count=0;gd.speed=0;gd.real_speed=0;gd.town_count=400;gd.timeslot_count=0;
	gd.pre_rural=0;gd.count_rural=0;gd.kirby_count=0;gd.kirby_count2=0;
	gd.kulisaped=0;gd.ant_dir=270;gd.memma_count=0;gd.location=EOF;gd.gaze_count=0;
	bd.bossHP=30000;bd.pre_bossHP=30000;bd.talking=0;
	rd.received=FALSE;
	setSMR=TRUE;
	if(gd.game_mode==STORYMODE||gd.game_mode==SELECT)phase=GAMESTART;
	else phase=READY;

	gd.bs_ch=0;
	menu[YNFORM].setMenu(20,100,6,2,2);
	menu[YNFORM].stack(text[MENUTEXT+13]);
	menu[YNFORM].stack(text[MENUTEXT+14]);
	menu[GAME_OPTION].setMenu(40,20,30,9,13);
	for (int i=0 ; i<13 ; i++)menu[GAME_OPTION].stack("");
	menu[MANEKI].setMenu(100,30,20,2,2);
	menu[MANEKI].stack(text[ANTENNATEXT+20]);
	menu[MANEKI].stack(text[ANTENNATEXT+21]);
	load_animebook();
	load_towers();
	load_rural();
	load_mounts();
	load_towns();
	estimate();
	setAntenna();
	initManekiTV();
	menu[PREF_LIST].setMenu(30,20,20,8,areas);
	for(int i=0 ; i<areas-1 ; i++)menu[PREF_LIST].stack(area[i].name);
	if(gd.game_mode!=NO_RELAY){
		BOOL ok;
		for(int i=0 ; i<towers ; i++){
			ok=FALSE;
			if(NHK_REMOVE && tower[i].kw2!=0){
				for(int j=0 ; j<10 ; j++)if(tower[i].ch[j]!=0){
					if(sta[area[tower[i].area].station[j]].mark!=5 && sta[area[tower[i].area].station[j]].mark!=6){
						ok=TRUE;break;
					}
				}
			}
			else ok=TRUE;
			if(ok)tower[i].remove=FALSE;
			else tower[i].remove=TRUE;
		}
	}
	createMap();
	if(gd.game_mode==NO_RELAY){
		for(int i=0 ; i<towers ; i++){
			if(tower[i].kw2!=0)tower[i].remove=TRUE;
			else tower[i].remove=FALSE;
		}
		createMap_tower();
	}
	if(gd.game_mode==BOSS){
		pre_magnify=MAGNIFY;
		MAGNIFY=1;
		TalkingAt(1);
	}
	if(gd.game_mode!=STORYMODE)gd.week=rand()%7;
	gd.talk_count=EOF;
}

void initGame2(){
	if(gd.hour<=5 || gd.hour>=23)createMap_color(200);
	else createMap_color(1000);
	createMap_tower();
	if(gd.game_mode==STORYMODE||gd.game_mode==SELECT||gd.game_mode==BOSS){
		if(gd.game_mode==BOSS){
			srand(SDL_GetTicks());
			dataNo=rand()%index_num+1;
		}
		load_works(dataNo);
		if(!fishbox.loaded())fishbox.initFishBox(works);
		for(int i=0 ; i<works ; i++){
			gd.score+=fishbox.getSC(i);
		}
		estimate_rural();
	}
	if(gd.game_mode==STORYMODE||gd.game_mode==SELECT){
		televise();
	}else{
		for(int i=0 ; i<stas ; i++)sta[i].ontv=EOF;
	}
	sprintf_s(str,"file/img/ready%d.png",gd.week%6);
	getImage(img.tv_asahi,str,0,0,0);

	if(md.maneki_mode==PLUGGED_IN){
		receiveAll(md.manekiX,md.manekiY);
		setManekiData();
	}

	if(gd.game_mode==BOSS){
		getImage(img.back,"file/img/epilogue.png",0,0,255);
	}else{
		sprintf_s(str,"file/img/weekly%d.png",gd.week);
		getImage(img.back,str,0,0,255);
	}
	if(gd.game_mode==STORYMODE||gd.game_mode==SELECT){
		sprintf_s(str,"file/bgm/%d.ogg",gd.week+5);
		bgm=Mix_LoadMUS(str);
	}

	if(MAP3D)make3dview(gd.x,gd.y,gd.ant_dir);
	else{
		map.buffered=FALSE;
	}

	if(gd.game_mode==SELECT)for(int i=0 ; i<works ; i++){
		work[i].notExist=TRUE;
		for(int j=work[i].prg ; j<work[i].prg+work[i].num ; j++){
			if(prg[j].week==gd.week || prg[j].week==(gd.week+6)%7 && in_time(prg[j].week,prg[j].hour,prg[j].minute,prg[j].time)){
				work[i].notExist=FALSE;
				break;
			}
		}
	}
}

void endGame(){
	freeImage(img.back);
	freeImage(img.fishup);
	freeImage(img.colorlight);
	if(img.rod!=NULL){freeImage(img.rod);img.rod=NULL;}
	Mix_FreeChunk(sf.water);
	Mix_FreeChunk(sf.thunder);
	Mix_FreeChunk(sf.meow);
	Mix_FreeChunk(sf.snore);
	Mix_FreeChunk(sf.fw_sound);
	Mix_FreeChunk(sf.noon);
	Mix_FreeChunk(sf.sunset);
	Mix_FreeChunk(sf.hold);
	Mix_FreeChunk(sf.drag_s);
	Mix_FreeChunk(sf.noize);
	Mix_FreeChunk(sf.bupyo);
	Mix_FreeChunk(sf.knob);
	Mix_FreeChunk(sf.calling);
	Mix_FreeChunk(sf.coin);
	Mix_FreeChunk(sf.swish);
	Mix_FreeChunk(sf.get);
	Mix_FreeChunk(sf.lamp);
	Mix_FreeChunk(sf.bubble);
	Mix_FreeChunk(sf.gaze);
	gd.ta_count=0;
	if(gd.game_mode==STORYMODE||gd.game_mode==SELECT||gd.game_mode==BOSS)Mix_FreeMusic(bgm);
	if(pre_magnify!=EOF)MAGNIFY=pre_magnify;
	if(gd.game_mode==NO_RELAY){
		for(int i=0 ; i<towers ; i++){
			tower[i].remove=FALSE;
		}
		createMap_tower();
	}
	for(int i=0 ; i<towers ; i++){
		tower[i].c1=0;
		tower[i].c2=0;
	}
	delete ant;
	fishbox.endFishBox();
	kick_count=1;
	for(int i=0 ; i<15 ; i++)menu[i].reset();
}

void keyGameStart(){
	if((key.z || key.x || key.c) && (!key_stop(key.z) || !key_stop(key.x) || !key_stop(key.c))){
		if(count>=10){
			map.buffered2=FALSE;
			if(gd.week==0)phase=READY;
			else phase=PLAYING;
			count=2;
		}
	}
}

void keyFishup(){
	if((key.z||key.x||key.c||key.left||key.right||key.up||key.down)){
		if(!key_stop(key.z) || !key_stop(key.x) || !key_stop(key.c) || !key_stop(key.left) || !key_stop(key.right) || !key_stop(key.up) || !key_stop(key.down)){
			if(gd.game_mode!=BOSS && start<40){
				int n;
				if(phase==FISHUP || phase==GRADEUP){
					phase=ANTENNA;
					n=work[sta[ant->station].ontv].tnum;
				}
				else if(phase==MANEKI_FISHUP || phase==MANEKI_GRADEUP){
					phase=PLAYING;
					n=work[md.fish[0].title_num].tnum;
					shiftFish_maneki();
				}
				if(!animebook[n]){
					animebook[n]=TRUE;
					collection++;
					save_animebook();
				}
			}
		}
	}
}

void keyBSAttack(){
	if((key.z||key.c||key.x||key.left||key.right||key.up||key.down)){
		if(!key_stop(key.z) || !key_stop(key.x) || !key_stop(key.c) || !key_stop(key.left) || !key_stop(key.right) || !key_stop(key.up) || !key_stop(key.down)){
			if(start==0){
				phase=BS_CH;
				if(!animebook[ work[sta[BSstation[gd.bs_ch]].ontv].tnum ]){
					animebook[ work[sta[BSstation[gd.bs_ch]].ontv].tnum ]=TRUE;
					collection++;
					save_animebook();
				}
			}
		}
	}
}

void keyFinish(){
	if(key.z && !key_stop(key.z)){
		gd.score=0;gd.crops=0;gd.get_score=0;
		for(int i=0 ; i<works ; i++){
			gd.score+=fishbox.getSC(i);
			if(fishbox.getSC(i)!=0)gd.crops++;
		}
		for(int i=0 ; i<works ; i++){
			if(fishbox.today[i]==EOF)break;
			gd.crops--;
		}
		phase=TODAYS_CROP;
		freeImage(img.back);
		getImage(img.back,"file/img/result.png",0,0,255);
		Mix_FreeMusic(bgm);
		bgm=Mix_LoadMUS("file/bgm/12.ogg");
		kick_count=1;count=-60;
	}
}

void keyGetHazia(){
	if(key.z && !key_stop(key.z)){
		if(gd.scene_count==0 || gd.scene_count==2){
			if(gd.text_count<(int)strlen(talk[gd.talk_count].str[CHAR_CODE])){
				gd.text_count=90;
			}
			else if(gd.talk_open_count==0){
				gd.text_count=0;
				gd.talk_count++;
				gd.face_count++;
				if(face[gd.face_count]==COMMA){
					if(gd.scene_count==0){
						start=300;count=2;
						gd.scene_count++;
						gd.face_count++;
						gd.hazia2=gd.score-indexName[dataNo-1].hiscore;
						if(gd.hazia2<0)gd.hazia2=0;
						Mix_FreeMusic(bgm);
						bgm=Mix_LoadMUS("file/bgm/13.ogg");
						Mix_PlayMusic(bgm,-1);
					}
				}
				if(face[gd.face_count]==SHAKE){
					Mix_PlayChannel(0,sf.decide,0);
					gd.shake_count=50;
					gd.face_count++;
				}
				if(face[gd.face_count]==EOF){
					gd.talk_count=EOF;
					gd.talk_open_count=1;
					gd.scene_count++;
					count=2;
				}
			}
		}
		else if(gd.scene_count==1){
			if(start==0 && gd.hazia2==0){
				gd.scene_count++;
				gd.text_count=0;
				Mix_FreeMusic(bgm);
				bgm=Mix_LoadMUS("file/bgm/12.ogg");
				Mix_PlayMusic(bgm,-1);
			}
		}
	}
}

void keyResult(){
	if((key.z || key.x || key.c) && (!key_stop(key.z) || !key_stop(key.x) || !key_stop(key.c))){
		if(start==0){
			if(gd.game_mode==SELECT){
				endGame();
				initGameMenu();
			}
			else if(gd.game_mode==STORYMODE){
				if(gd.week==6){
					phase=GET_HAZIA;
					start=200;
					gd.scene_count=0;
				}else{
					Mix_FreeMusic(bgm);
					freeImage(img.back);
					gd.week++;gd.hour=4;
					sprintf_s(str,"file/img/weekly%d.png",gd.week);
					getImage(img.back,str,0,0,255);
					sprintf_s(str,"file/bgm/%d.ogg",gd.week+5);
					bgm=Mix_LoadMUS(str);
					freeImage(img.tv_asahi);
					sprintf_s(str,"file/img/ready%d.png",gd.week%6);
					getImage(img.tv_asahi,str,0,0,0);
					createMap_color(1000);
					phase=GAMESTART;
					for(int i=0 ; i<works ; i++)fishbox.today[i]=EOF;
					gd.ta_count=0;start=75;count=-1;
					Mix_PlayChannel(1, sf.decide, 0);
					kick_count=1;
				}
			}
		}
	}
}

void keyRecordYN(){
	if(key.z && !key_stop(key.z)){
		if(menu[YNFORM].selected()==0){
			phase=END_RECORD;
			makeRecordMenu(1);
			menu[1].stack("NEW");
			menu[YNFORM].setViewMode(GRAY);
			menu[1].setViewMode(VISIBLE);
		}
		else if(menu[YNFORM].selected()==1){
			menu[YNFORM].setViewMode(HIDE);
			phase=LEAVE_MAP;
			count=2;
		}
	}
	if(key.up && !key_wait(key.up))menu[YNFORM].cursorUp();
	if(key.down && !key_wait(key.down))menu[YNFORM].cursorDown();
}

void keyEndingRecordMenu(){
	if(key.z && !key_stop(key.z)){
		save_record(menu[1].selected());
		phase=SAVING_RECORD;
		count=2;
	}
	if(key.x && !key_stop(key.x)){
		phase=END_YN;
		menu[1].setViewMode(HIDE);
		menu[YNFORM].setViewMode(VISIBLE);
	}
	if(key.up && !key_wait(key.up))menu[1].cursorUp();
	if(key.down && !key_wait(key.down))menu[1].cursorDown();
}

void walking_3d(){
	if(MAP3D && (key.up||key.down||key.left||key.right)){
		if(key.up){
			gd.x+=cos(gd.ant_dir*PI/180);
			gd.y-=sin(gd.ant_dir*PI/180);
		}
		if(key.down){
			gd.x-=cos(gd.ant_dir*PI/180);
			gd.y+=sin(gd.ant_dir*PI/180);
		}
		if(key.left)gd.ant_dir+=3;
		if(key.right)gd.ant_dir-=3;
		if(gd.ant_dir<0)gd.ant_dir=359;
		if(gd.ant_dir>=360)gd.ant_dir=0;
		if(gd.x<0)gd.x=0;
		if(gd.x>map.mapW)gd.x=map.mapW;
		if(gd.y<0)gd.y=0;
		if(gd.y>map.mapH)gd.y=map.mapH;

		make3dview(gd.x,gd.y,gd.ant_dir);

		rd.received=FALSE;
		fix_XY();
	}
}

void walking_bumpy(){
	double add=0.005*map.slope[(int)gd.x][(int)gd.y];
	if(map.type[(int)gd.x+(int)gd.y*map.mapW]==SLOPE_45){
		if(key.up)gd.speedY=-gd.speed;
		if(key.right)gd.speedX=gd.speed;
		if(key.down){
			gd.speedY=gd.speed-add;
			if(gd.speedY<0){
				gd.speedY=0;
				if(!key.left)gd.speedX=gd.speed;
			}
		}
		if(key.left){
			gd.speedX=-(gd.speed-add);
			if(gd.speedX>0){
				gd.speedX=0;
				if(!key.down)gd.speedY=-gd.speed;
			}
		}
	}
	else if(map.type[(int)gd.x+(int)gd.y*map.mapW]==SLOPE_135){
		if(key.up)gd.speedY=-gd.speed;
		if(key.left)gd.speedX=-gd.speed;
		if(key.down){
			gd.speedY=gd.speed-add;
			if(gd.speedY<0){
				gd.speedY=0;
				if(!key.right)gd.speedX=-gd.speed;
			}
		}
		if(key.right){
			gd.speedX=gd.speed-add;
			if(gd.speedX<0){
				gd.speedX=0;
				if(!key.down)gd.speedY=-gd.speed;
			}
		}
	}
	else if(map.type[(int)gd.x+(int)gd.y*map.mapW]==SLOPE_225){
		if(key.down)gd.speedY=gd.speed;
		if(key.left)gd.speedX=-gd.speed;
		if(key.up){
			gd.speedY=-(gd.speed-add);
			if(gd.speedY>0){
				gd.speedY=0;
				if(!key.right)gd.speedX=-gd.speed;
			}
		}
		if(key.right){
			gd.speedX=gd.speed-add;
			if(gd.speedX<0){
				gd.speedX=0;
				if(!key.up)gd.speedY=gd.speed;
			}
		}
	}
	else if(map.type[(int)gd.x+(int)gd.y*map.mapW]==SLOPE_315){
		if(key.down)gd.speedY=gd.speed;
		if(key.right)gd.speedX=gd.speed;
		if(key.up){
			gd.speedY=-(gd.speed-add);
			if(gd.speedY>0){
				gd.speedY=0;
				if(!key.left)gd.speedX=gd.speed;
			}
		}
		if(key.left){
			gd.speedX=-(gd.speed-add);
			if(gd.speedX>0){
				gd.speedX=0;
				if(!key.up)gd.speedY=gd.speed;
			}
		}
	}
}

void walking(){
	if(MAP3D && (key.up||key.down||key.left||key.right)){
		walking_3d();
		return;
	}
	if(gd.game_mode==BOSS && bd.bossHP==0){
		if((int)gd.x==0 || (int)gd.y==0 || (int)gd.x==map.mapW || (int)gd.y==map.mapH){
			phase=SUMMERWARS;
			TalkingAt(8);
			count=0;
		}
	}

	gd.speedX=0;
	gd.speedY=0;
	if(key.up||key.down||key.left||key.right){
		if(DASH_TYPE==TYPE_MARIO){
			if(key.x){
				if(key.x>2)key.x=2;
				if(!key_stop(key.x))Mix_PlayChannel(0,sf.drag_s,0);
				if(gd.kirby_count<10){
					gd.speed+=3;
					gd.kirby_count=10;
				}
			}
			if(!key.x){
				if(gd.kirby_count>0)gd.kirby_count--;
				else if(gd.speed>1)gd.speed=1;
			}
		}
		if(DASH_TYPE==TYPE_KIRBY && gd.kirby_count2>0 && gd.kirby_count2<7){
			Mix_PlayChannel(0,sf.drag_s,0);
			gd.speed+=3;
			gd.kirby_count=0;
		}
		if(gd.speed>0.05 || key.up>=30 || key.down>=30 || key.left>=30 || key.right>=30){
			if(DASH_TYPE==TYPE_MARIO){
				if(gd.speed<2)gd.speed=1;
			}
			else if(DASH_TYPE==TYPE_KIRBY){
				if(gd.speed<2)gd.speed=1;
			}
			else if(DASH_TYPE==TYPE_MEGAMAN){
				gd.speed=1;
				if(key.x){
					if(gd.kirby_count==0){
						Mix_PlayChannel(0,sf.drag_s,0);
						gd.kirby_count=20;
					}
					if(gd.kirby_count>1){
						gd.speed=10;
						gd.kirby_count--;
					}
				}
				else gd.kirby_count=0;
			}
			else if(DASH_TYPE==TYPE_SONIC){
				gd.speed+=0.01;
				if((int)(gd.speed*100)==150)Mix_PlayChannel(0,sf.drag_s,0);
			}
			if(gd.speed>15)gd.speed=15;
		}
		if(key.up)gd.speedY=-gd.speed;
		if(key.down)gd.speedY=gd.speed;
		if(key.left)gd.speedX=-gd.speed;
		if(key.right)gd.speedX=gd.speed;
		if((key.up||key.down) && (key.left||key.right)){
			gd.speedX*=0.707;
			gd.speedY*=0.707;
		}
		if(WALKING_TYPE==BUMPY)walking_bumpy();
		gd.real_speed=sqrt(gd.speedX*gd.speedX+gd.speedY*gd.speedY);
		rd.received=FALSE;
		gd.m_wave=0;
		gd.m_waved=FALSE;
		if(DASH_TYPE==TYPE_KIRBY){
			if(gd.kirby_count==0){gd.kirby_count=7;gd.kirby_count2=7;}
			else if(gd.kirby_count>1)gd.kirby_count--;
			else if(gd.kirby_count==1)gd.kirby_count2=0;
		}
	}else{
		if(DASH_TYPE==TYPE_SONIC){
			if(gd.speed>0)gd.speed-=0.05;
			if(gd.speed<0)gd.speed=0;
		}
		else if(gd.kirby_count2==0)gd.speed=0;
		gd.real_speed=0;
		if(gd.kirby_count2>0)gd.kirby_count2--;
		gd.kirby_count=0;
		if(!gd.m_waved){
			if(gd.game_mode!=BOSS){
				gd.m_wave=70;
				gd.m_waved=TRUE;
			}
			getH();
		}
	}
	int preX=gd.scrX, preY=gd.scrY;
	if(gd.speed<0.05){
		if(!key_stop(key.up) || !key_stop(key.down) || !key_stop(key.left) || !key_stop(key.right)){
			if(key.up)gd.y-=1.0/MAGNIFY;
			if(key.down)gd.y+=1.0/MAGNIFY;
			if(key.left)gd.x-=1.0/MAGNIFY;
			if(key.right)gd.x+=1.0/MAGNIFY;
		}
	}else{
		gd.y+=gd.speedY/MAGNIFY;
		gd.x+=gd.speedX/MAGNIFY;
	}
	if(key.up && key.left)gd.player_dir=6;
	else if(key.up && key.right)gd.player_dir=7;
	else if(key.down && key.left)gd.player_dir=4;
	else if(key.down && key.right)gd.player_dir=5;
	else if(key.up)gd.player_dir=2;
	else if(key.down)gd.player_dir=0;
	else if(key.left)gd.player_dir=3;
	else if(key.right)gd.player_dir=1;
	fix_XY();
	fix_scrXY();
	map.moved=FALSE;
	map.slideX=gd.scrX-preX;
	map.slideY=gd.scrY-preY;
	if(!MAP3D && (map.slideX!=0 || map.slideY!=0)){
		map.moved=TRUE;
		map.buffered=FALSE;
		map.buffered2=FALSE;
	}

	if(MAGNIFY>=8 && (key.up||key.down||key.left||key.right)){
		double a=3;
		int loc=EOF;
		for(int i=0 ; i<towns ; i++){
			if(abs(town[i].x-gd.x)>3)continue;
			if(abs(town[i].y-gd.y)>3)continue;
			double X=town[i].x-gd.x, Y=town[i].y-gd.y;
			if(sqrt(X*X+Y*Y)<a){
				a=sqrt(X*X+Y*Y);
				loc=i;
			}
		}
		if(a==3){
			gd.location=EOF;
			if(gd.town_count<30)gd.town_count=400-gd.town_count;
			else if(gd.town_count<370)gd.town_count=370;
		}
		else if(gd.location!=loc){
			gd.location=loc;
			gd.town_count=0;
		}
	}
}

void keyPlaying(){
	if(key.z && !key_stop(key.z)){
		if(md.maneki_mode!=CARRYING && md.manekiX-16<gd.x && gd.x<md.manekiX+16 && md.manekiY-20<gd.y && gd.y<md.manekiY+10){
			if(md.maneki_mode==NULL){
				phase=MANEKI;
				gd.scene_count=0;
				gd.talk_count=0;
				gd.text_count=0;
				gd.talk_open_count=1;
			}
			else if(md.maneki_mode==PLUGGED_IN){
				phase=MANEKI_CONFIRM;
				menu[YNFORM].setViewMode(VISIBLE);
				gd.talk_count=0;
				gd.text_count=0;
				gd.scene_count=1;
				gd.talk_open_count=1;
			}
		}else{
			Mix_PlayChannel(1, sf.hold, 0);
			phase=ANTENNA;
			if(!rd.received)receiveAll(gd.dotX,gd.dotY);
			ant->ant_mode=TUNE;
			ant->hoist();
			rd.received=TRUE;
		}
	}
	if(key.c && !key_stop(key.c)){
		phase=MENU;
		start=30;
		Mix_PlayChannel(1, sf.decide2, 0);
	}
	if(key.x && gd.game_mode!=BOSS && !key_wait(key.x)){
		if(!key.up && !key.down && !key.left && !key.right){
			if(key.x==50)Mix_PlayChannel(1, sf.snore, 0);
			gd.doze=TRUE;
			gd.speed-=0.5;
			if(gd.speed<0)gd.speed=0;
		}
	}
	else gd.doze=FALSE;
	walking();
}

void keyAntenna(){
	if(key.z && !key_stop(key.z)){
		if(ant->ant_mode==TUNE)ant->pushButton();
	}
	if(key.c && !key_stop(key.c)){
		if(ant->ant_mode==TUNE){
			ant->ant_mode=TURN;
		}
		else if(ant->ant_mode==TURN){
			if(gd.bought[5]){
				ant->ant_mode=TROLL;
				receiveAll(gd.dotX,gd.dotY);
				if(ROD_TYPE!=CONVENIENTROD && ROD_TYPE!=SUPERHANDYROD)ant->trolling();
			}
			else ant->ant_mode=TUNE;
		}
		else{
			ant->ant_mode=TUNE;
		}
		ant->change_mode_count=20;
	}
	if(key.x && !key_stop(key.x)){
		ant->lower();
		if(md.maneki_mode==CARRYING && map.rural[(int)gd.dotX/map.rural_size][(int)gd.dotY/map.rural_size]<254){
			phase=MANEKI_CONFIRM;
			menu[YNFORM].setViewMode(VISIBLE);
			gd.talk_count=0;
			gd.text_count=0;
			gd.scene_count=0;
			gd.talk_open_count=1;
		}
		else phase=PLAYING;
	}
	if(ant->ant_mode==TROLL){
		double preX=gd.x,preY=gd.y;
		walking();
		if(preX!=gd.x || preY!=gd.y){
			receiveAll(gd.dotX,gd.dotY);
			ant->trolling();
		}
	}
	else{
		if(key.up)ant->pushU();
		if(key.down)ant->pushD();
		if(key.left)ant->pushL();
		if(key.right)ant->pushR();
	}
}

void keyMenu(){
	if(key.z && !key_stop(key.z)){
		Mix_PlayChannel(1, sf.decide2, 0);
		if(gd.menu_selected==0){
			phase=BS_CH;
			receiveBS();
		}
		else if(gd.menu_selected==1){
			phase=ANTENNA_MENU;
			start=35;
		}
		else if(gd.menu_selected==2){
			phase=GAME_OPTION;
			start=65;
		}
		else if(gd.menu_selected==3){
			if(gd.game_mode==STORYMODE||gd.game_mode==SELECT){
				fishbox.text_count=60;
				phase=CROP;
			}
			else if(MAP3D){
				phase=ADJUST_HEIGHT;
			}else{
				phase=PREF_LIST_TRANS;
				menu[PREF_LIST].setViewMode(VISIBLE);
			}
		}
		else if(gd.menu_selected==4){
			if(gd.game_mode==STORYMODE||gd.game_mode==SELECT){
				phase=SAVEMENU;
				makeSaveMenu(SAVEMENU);
				for(int i=0 ; i<gd.save_cursor ; i++)menu[SAVEMENU].cursorDown();
				menu[SAVEMENU].setViewMode(VISIBLE);
				menu[SAVEMENU].stack("NEW");
			}
			else if(MAP3D){
				phase=ADJUST_ZOOM;
			}
		}
		else if(gd.menu_selected==5){
			gd.menu_selected=0;
			endGame();
			if(gd.game_mode==STORYMODE||gd.game_mode==SELECT||gd.game_mode==BOSS){
				initGameMenu();
			}else{
				initMiyazaki();
				if(MAP3D)gd.x=1000;
				else if(gd.game_mode==WALKING)gd.x=300;
				else if(gd.game_mode==NO_RELAY)gd.x=1100;
				gd.scrX=(int)gd.x-160;
				phase=MIYAZAKI_MUSEUM;
				start=0;
				MAP3D=FALSE;
			}
		}
	}
	if((key.x || key.c) && (!key_stop(key.x) || !key_stop(key.c))){
		if(MAP3D)rd.received=FALSE;
		phase=PLAYING;
	}

	if((key.up || key.down) && (!key_wait(key.up) || !key_wait(key.down))){
		if(gd.menu_selected/3==0)gd.menu_selected+=3;
		else gd.menu_selected-=3;
		Mix_PlayChannel(0,sf.cursor_move,0);
	}
	if(key.left && !key_wait(key.left)){
		if(gd.menu_selected%3==0)gd.menu_selected+=2;
		else gd.menu_selected--;
		Mix_PlayChannel(0,sf.cursor_move,0);
	}
	if(key.right && !key_wait(key.right)){
		if(gd.menu_selected%3==2)gd.menu_selected-=2;
		else gd.menu_selected++;
		Mix_PlayChannel(0,sf.cursor_move,0);
	}
}

void keySaveMenu(){
	if(key.z && !key_stop(key.z)){
		save_game(menu[SAVEMENU].selected());
		gd.save_cursor=menu[SAVEMENU].selected();
		phase=SAVING;
		count=2;
	}
	if(key.x && !key_stop(key.x)){
		phase=MENU;
		menu[SAVEMENU].setViewMode(HIDE);
	}
	if(key.up && !key_wait(key.up))menu[SAVEMENU].cursorUp();
	if(key.down && !key_wait(key.down))menu[SAVEMENU].cursorDown();
}

void keySaving(){
	if((key.z||key.x) && (!key_stop(key.z)||!key_stop(key.x))){
		if(count>=150){
			phase=MENU;
			menu[SAVEMENU].setViewMode(HIDE);
		}
	}
}

void keySavingRecord(){
	if((key.z||key.x) && (!key_stop(key.z)||!key_stop(key.x))){
		if(count>=150){
			menu[YNFORM].setViewMode(HIDE);
			menu[1].setViewMode(HIDE);
			phase=LEAVE_MAP;
			count=2;
		}
	}
}

void keyAntennaMenu(){
	if(key.z && !key_stop(key.z)){
		if(gd.antenna_selected==6){
			if(gd.bought[8]){
				phase=PREF_LIST_SMR;
				menu[PREF_LIST].setViewMode(VISIBLE);
			}
		}else{
			int n=gd.antenna_selected;
			if(n<2 || n>=2 && gd.bought[n-2]){
				delete ant;
				if(img.rod!=NULL){
					freeImage(img.rod);
					img.rod=NULL;
				}
				ROD_TYPE=n;
				setAntenna();
				save_option();
				Mix_PlayChannel(1, sf.hold, 0);
				phase=ANTENNA;
				receiveAll(gd.dotX,gd.dotY);
				ant->hoist();
				rd.received=TRUE;
			}
		}
	}
	if(key.x && !key_stop(key.x)){
		phase=MENU;
	}
	if((key.up || key.down) && (!key_wait(key.up) || !key_wait(key.down))){
		if(gd.antenna_selected!=6){
			if(gd.antenna_selected/3==0)gd.antenna_selected+=3;
			else gd.antenna_selected-=3;
		}
		Mix_PlayChannel(0,sf.cursor_move,0);
	}
	if(key.left && !key_wait(key.left)){
		if(gd.antenna_selected==0)gd.antenna_selected=2;
		else if(gd.antenna_selected==3)gd.antenna_selected=6;
		else gd.antenna_selected--;
		Mix_PlayChannel(0,sf.cursor_move,0);
	}
	if(key.right && !key_wait(key.right)){
		if(gd.antenna_selected==2)gd.antenna_selected=0;
		else if(gd.antenna_selected==6)gd.antenna_selected=3;
		else gd.antenna_selected++;
		Mix_PlayChannel(0,sf.cursor_move,0);
	}
}

void keyPrefList_SMR(){
	if(key.z && !key_stop(key.z)){
		menu[SMR].setMenu(0,20,40,10,area[menu[PREF_LIST].selected()].st_num);
		for(int i=0 ; i<area[menu[PREF_LIST].selected()].st_num ; i++){
			if(NHK_REMOVE && gd.game_mode!=NO_RELAY){
				if(sta[area[menu[PREF_LIST].selected()].station[i]].mark==5 || sta[area[menu[PREF_LIST].selected()].station[i]].mark==6){
					menu[SMR].stack("------");
					continue;
				}
			}
			menu[SMR].stack(sta[area[menu[PREF_LIST].selected()].station[i]].name);
			menu[SMR].inputMark(i,sta[area[menu[PREF_LIST].selected()].station[i]].mark);
		}
		menu[SMR].setViewMode(VISIBLE);
		phase=SMR;
		menu[PREF_LIST].setViewMode(GRAY);
	}
	if(key.x && !key_stop(key.x)){
		fix_XY();
		fix_scrXY();
		map.buffered=FALSE;
		map.buffered2=FALSE;
		phase=ANTENNA_MENU;
		menu[PREF_LIST].setViewMode(HIDE);
	}
	if(key.up && !key_wait(key.up))menu[PREF_LIST].cursorUp();
	if(key.down && !key_wait(key.down))menu[PREF_LIST].cursorDown();
	if(key.left && !key_wait(key.left))menu[PREF_LIST].pageUp();
	if(key.right && !key_wait(key.right))menu[PREF_LIST].pageDown();
}

void keyPrefList_trans(){
	if(key.z && !key_stop(key.z)){
		int n=menu[PREF_LIST].selected();
		menu[TOWN_LIST].setMenu(50,30,20,8,area[n].town_num);
		for(int i=0 ; i<area[n].town_num ; i++)menu[TOWN_LIST].stack(town[area[n].town+i].name);
		menu[TOWN_LIST].setViewMode(VISIBLE);
		phase=TOWN_LIST;
		menu[PREF_LIST].setViewMode(GRAY);
	}
	if(key.x && !key_stop(key.x)){
		phase=MENU;
		menu[PREF_LIST].setViewMode(HIDE);
	}
	if(key.up && !key_wait(key.up))menu[PREF_LIST].cursorUp();
	if(key.down && !key_wait(key.down))menu[PREF_LIST].cursorDown();
	if(key.left && !key_wait(key.left))menu[PREF_LIST].pageUp();
	if(key.right && !key_wait(key.right))menu[PREF_LIST].pageDown();
}

void keyTownList(){
	if(key.z && !key_stop(key.z)){
		int m=menu[PREF_LIST].selected();
		int n=menu[TOWN_LIST].selected();
		gd.x=town[area[m].town+n].x;
		gd.y=town[area[m].town+n].y;
		fix_XY();
		fix_scrXY();
		map.buffered=FALSE;
		map.buffered2=FALSE;
		gd.location=area[m].town+n;
		gd.town_count=0;
		rd.received=FALSE;
		phase=READY;
		count=100;
		menu[PREF_LIST].setViewMode(HIDE);
		menu[TOWN_LIST].setViewMode(HIDE);
	}
	if(key.x && !key_stop(key.x)){
		phase=PREF_LIST_TRANS;
		menu[PREF_LIST].setViewMode(VISIBLE);
		menu[TOWN_LIST].setViewMode(HIDE);
	}
	if(key.up && !key_wait(key.up))menu[TOWN_LIST].cursorUp();
	if(key.down && !key_wait(key.down))menu[TOWN_LIST].cursorDown();
	if(key.left && !key_wait(key.left))menu[TOWN_LIST].pageUp();
	if(key.right && !key_wait(key.right))menu[TOWN_LIST].pageDown();
}

void keySMR(){
	if(key.z && !key_stop(key.z)){
		BOOL ok=TRUE;
		if(NHK_REMOVE && gd.game_mode!=NO_RELAY){
			if(sta[area[menu[PREF_LIST].selected()].station[menu[SMR].selected()]].mark==5
			|| sta[area[menu[PREF_LIST].selected()].station[menu[SMR].selected()]].mark==6){
				ok=FALSE;
			}
		}
		if(ok){
			saveSMR=FALSE;
			for(int i=0 ; i<map.mapW ; i++)for(int j=0 ; j<map.mapH ; j++)map.smr[i][j]=0;
			if(gd.game_mode!=NO_RELAY)load_smr(area[menu[PREF_LIST].selected()].station[menu[SMR].selected()]);
			SMRcount++;
			phase=SMR_RESULT;
			pre_magnify=MAGNIFY;
			MAGNIFY=1;
			fix_XY();
			fix_scrXY();
			map.buffered=FALSE;
			map.buffered2=FALSE;
			menu[SMR].setViewMode(HIDE);
			menu[PREF_LIST].setViewMode(HIDE);
		}
	}
	if(key.x && !key_stop(key.x)){
		phase=PREF_LIST_SMR;
		menu[SMR].setViewMode(HIDE);
		menu[PREF_LIST].setViewMode(VISIBLE);
	}
	if(key.up && !key_wait(key.up))menu[SMR].cursorUp();
	if(key.down && !key_wait(key.down))menu[SMR].cursorDown();
}

void keySmrResult(){
	if(key.x && !key_stop(key.x)){
		if(gd.game_mode!=NO_RELAY)save_smr(area[menu[PREF_LIST].selected()].station[menu[SMR].selected()]);
		MAGNIFY=pre_magnify;
		SMRcount=0;
		phase=SMR;
		menu[SMR].setViewMode(VISIBLE);
		menu[PREF_LIST].setViewMode(GRAY);
	}
}

void keyCrop(){
	if(key.x && !key_stop(key.x)){
		phase=MENU;
	}
	if(key.up && !key_wait(key.up))fishbox.cursorUp();
	if(key.down && !key_wait(key.down))fishbox.cursorDown();
	if(key.left && !key_wait(key.left))fishbox.cursorLeft();
	if(key.right && !key_wait(key.right))fishbox.cursorRight();
}

void keyBSCH(){
	if(key.x && !key_stop(key.x)){
		phase=MENU;
	}
	if(key.left && !key_stop(key.left)){
		if(gd.bs_ch!=0 && gd.bs_ch!=4 && gd.bs_ch!=8)gd.bs_ch--;
	}
	if(key.right && !key_stop(key.right)){
		if(gd.bs_ch!=3 && gd.bs_ch!=7 && gd.bs_ch!=11)gd.bs_ch++;
	}
	if(key.up && !key_stop(key.up)){
		if(gd.bs_ch>3)gd.bs_ch-=4;
	}
	if(key.down && !key_stop(key.down)){
		if(gd.bs_ch<8)gd.bs_ch+=4;
	}
	if((key.left||key.right||key.up||key.down) && (!key_wait(key.left) || !key_wait(key.right) || !key_wait(key.up) || !key_wait(key.down))){
		receiveBS();
	}
}

void keyManekiTalking(){
	if(gd.scene_count<3){
		if(key.z && !key_stop(key.z)){
			if(gd.text_count<(int)strlen(text[ANTENNATEXT+17+gd.scene_count].str[CHAR_CODE])){
				gd.text_count=90;
			}else{
				gd.text_count=0;
				gd.scene_count++;
				if(gd.scene_count==3){
					menu[MANEKI].setViewMode(VISIBLE);
					gd.talk_count=EOF;
					gd.talk_open_count=1;
				}
			}
		}
	}
	else if(gd.scene_count==3){
		if(key.z && !key_stop(key.z)){
			menu[MANEKI].setViewMode(HIDE);
			phase=PLAYING;
			if(menu[MANEKI].selected()==0){
				md.maneki_mode=CARRYING;
				Mix_PlayChannel(0,sf.water,0);
			}
		}
		if(key.up && !key_wait(key.up))menu[MANEKI].cursorUp();
		if(key.down && !key_wait(key.down))menu[MANEKI].cursorDown();
	}
	else if(gd.scene_count>=5){
		if(key.z && !key_stop(key.z)){
			if(gd.text_count<(int)strlen(text[ANTENNATEXT+17+gd.scene_count].str[CHAR_CODE])){
				gd.text_count=90;
			}else{
				gd.text_count=0;
				gd.scene_count++;
				if(gd.scene_count==7){
					gd.talk_count=EOF;
					gd.talk_open_count=1;
					phase=PLAYING;
				}
			}
		}
	}
}

void keyGameTalking(){
	if(phase==SUMMERWARS && count<660)return;
	if(key.z && !key_stop(key.z) && start==0){
		int a=controlTalking();
		if(a==COMMA){
			if(gd.game_mode==BOSS && bd.talking==7){
				if(gd.scene_count==0){
					phase=CALLING;
					Mix_HaltMusic();
					Mix_PlayChannel(0,sf.calling,0);
					count=0;
				}
				if(gd.scene_count==1){
					Mix_FreeMusic(bgm);
					bgm=Mix_LoadMUS("file/bgm/1.ogg");
					Mix_PlayMusic(bgm,-1);
				}
			}
		}
		else if(a==EOF || a==HANGUP){
			if(phase==SUMMERWARS){
				endGame();
				initLastStory();
			}else{
				if(a==EOF)gd.face_count++;
				Mix_PlayChannel(0, sf.noize, 0);
				phase=PLAYING;
			}
		}
	}
}

void keyManekiConfirm(){
	if(key.z && !key_stop(key.z)){
		menu[YNFORM].setViewMode(HIDE);
		gd.talk_count=EOF;
		gd.talk_open_count=1;
		phase=PLAYING;
		if(menu[YNFORM].selected()==0){
			if(md.maneki_mode==CARRYING){
				md.manekiX=gd.dotX;md.manekiY=gd.dotY;
				setManekiData();
				md.maneki_mode=PLUGGED_IN;
				if(!checkManekiTV()){
					gd.text_count=0;
					gd.talk_count=0;
					gd.talk_open_count=1;
					gd.scene_count=5;
					phase=MANEKI;
				}
			}else{
				md.maneki_mode=CARRYING;
			}
			Mix_PlayChannel(0,sf.decide,0);
		}
	}
	if(key.up && !key_wait(key.up))menu[YNFORM].cursorUp();
	if(key.down && !key_wait(key.down))menu[YNFORM].cursorDown();
}

void keyAdjustHeight(){
	if(key.c && !key_stop(key.c)){
		phase=MENU;
	}
	if(key.up && !key_wait(key.up)){
		gd.height+=10;
		make3dview(gd.x,gd.y,gd.ant_dir);
	}
	if(key.down && !key_wait(key.down)){
		gd.height-=10;
		make3dview(gd.x,gd.y,gd.ant_dir);
	}
}

void keyAdjustZoom(){
	if(key.c && !key_stop(key.c)){
		phase=MENU;
	}
	if(key.up && !key_wait(key.up)){
		if(gd.zoom<360){
			gd.zoom++;
			make3dview(gd.x,gd.y,gd.ant_dir);
		}
	}
	if(key.down && !key_wait(key.down)){
		if(gd.zoom>0){
			gd.zoom--;
			make3dview(gd.x,gd.y,gd.ant_dir);
		}
	}
}

void keyGame(){
	switch(phase){
		case PLAYING:keyPlaying();break;
		case GAMESTART:keyGameStart();break;
		case FISHUP:
		case GRADEUP:
		case MANEKI_FISHUP:
		case MANEKI_GRADEUP:keyFishup();break;
		case FINISH:keyFinish();break;
		case RESULT:keyResult();break;
		case GET_HAZIA:keyGetHazia();break;
		case ANTENNA:keyAntenna();break;
		case MENU:keyMenu();break;
		case SAVEMENU:keySaveMenu();break;
		case PREF_LIST_SMR:keyPrefList_SMR();break;
		case SMR:keySMR();break;
		case SMR_RESULT:keySmrResult();break;
		case CROP:keyCrop();break;
		case BS_CH:keyBSCH();break;
		case GAME_OPTION:keyOptionMenu();break;
		case GAME_OPTION_SETTING:keyOptionSetting();break;
		case GAME_KEY_SETTING:keySetting();break;
		case PREF_LIST_TRANS:keyPrefList_trans();break;
		case TOWN_LIST:keyTownList();break;
		case ANTENNA_MENU:keyAntennaMenu();break;
		case BS_ATTACK:keyBSAttack();break;
		case TALKING:
		case SUMMERWARS:keyGameTalking();break;
		case MANEKI:keyManekiTalking();break;
		case MANEKI_CONFIRM:keyManekiConfirm();break;
		case END_YN:keyRecordYN();break;
		case END_RECORD:keyEndingRecordMenu();break;
		case SAVING:keySaving();break;
		case SAVING_RECORD:keySavingRecord();break;
		case ADJUST_HEIGHT:keyAdjustHeight();break;
		case ADJUST_ZOOM:keyAdjustZoom();break;
		default:break;
	}
}

void setTmpFish_bs(){
	fishbox.text_count=60;
	tmp_fish.title_num=sta[ BSstation[gd.bs_ch] ].ontv;
	tmp_fish.x=(int)gd.x;
	tmp_fish.y=(int)gd.y;
	tmp_fish.week=gd.week;
	tmp_fish.hour=gd.hour;
	tmp_fish.minute=gd.minute;
	tmp_fish.sta=BSstation[gd.bs_ch];
	tmp_fish.bs=TRUE;
	tmp_fish.ch=gd.bs_ch+1;
	tmp_fish.rcv=100;
	tmp_fish.mg_rcv=0;
	tmp_fish.score=1;
}

void receiveBS(){
	if(BSstation[gd.bs_ch]!=EOF && sta[ BSstation[gd.bs_ch] ].ontv!=EOF){
		setTmpFish_bs();
		if(fishbox.getSC(tmp_fish.title_num)==0){
			fishbox.text_count=1;
			fishbox.setFish(tmp_fish);
			for(int i=0 ; i<works ; i++)if(fishbox.today[i]==EOF){
				fishbox.today[i]=tmp_fish.title_num;
				break;
			}
			Mix_PlayChannel(0,sf.thunder,0);
			Mix_PlayChannel(1,sf.meow,0);
			gd.get_score++;
			gd.gradeup=1;
			phase=BS_ATTACK;
			start=30;
		}
		fishbox.panelColor(tmp_fish.title_num);
	}
}

void getH(){
	rd.seeH[0]=10;
	Mount *mt=mount;
	for(int k=0 ; k<mounts ; k++){
		if(mt->city){mt++;continue;}
		int mX=mt->x,mY=mt->y;
		double mount_dis=sqrt(1.0*(gd.dotX-mX)*(gd.dotX-mX)+(gd.dotY-mY)*(gd.dotY-mY));
		if(mX-mt->range-2<=gd.dotX && gd.dotX<=mX+mt->range+2 && mY-mt->range-2<=gd.dotY && gd.dotY<=mY+mt->range+2){
			int a=mt->h - (int)((mt->h-mt->slope)*mount_dis/mt->range);
			if(rd.seeH[0]<a)rd.seeH[0]=a;
		}

		if(mt->volcano && abs(mX-gd.dotX)<3 && abs(mY-gd.dotY)<3){
			for(int i=0 ; i<map.volcanoNum ; i++){
				if(map.volcano_id[i]==k)break;
				if(map.volcano[i]!=0)continue;
				map.volcano_id[i]=k;
				map.volcano[i]=2000;
				map.volcanoX[i]=mX;
				map.volcanoY[i]=mY;
				gd.firework++;
				break;
			}
		}
		mt++;
	}
}

int getScore(int n, double pw, int x, int y){
	int sc=0;
	int a=map.rural[x/map.rural_size][y/map.rural_size];
	if(a==EOF)return 0;
	if(n==-1)sc=(int)( sqrt(10000.0/pw) * map.rural_rate[a] );
	else sc=(int)( sqrt(10000.0/pw) * map.rural_rate[a] * 25/sqrt(1.0*work[n].num) );
	return sc;
}

int getMissing(){
	int a=0;
	BOOL hit=FALSE;
	for(int i=0 ; i<works ; i++){
		if(fishbox.getSC(i)==0){
			hit=FALSE;
			for(int j=work[i].prg ; j<work[i].prg+work[i].num ; j++){
				if(prg[j].week>gd.week||
				   prg[j].week==gd.week&&prg[j].hour*100+prg[j].minute>=gd.hour*100+gd.minute
				  )hit=TRUE;
				if(in_time(prg[j].week,prg[j].hour,prg[j].minute,prg[j].time))hit=TRUE;
			}
			if(!hit)a++;
		}
	}
	return a;
}

BOOL in_time(int w, int h, int m, int t){
	int t1,t2,t3;
	t1=h*100+m;
	t2=t1;
	t3=gd.hour*100+gd.minute;
	for(int i=0 ; i<t ; i++){
		t2++;
		if(t2%100==60)t2+=40;
	}
	if(w==gd.week-1 && t2>2800){
		t1-=2400;t2-=2400;
	}
	else if(w!=gd.week)return FALSE;
	if(t1<=t3 && t3<t2)return TRUE;
	else return FALSE;
}

void televise(){
	int *pre;
	pre=new int[stas];
	if(gd.game_mode!=BOSS){
		for(int i=0 ; i<stas ; i++){
			pre[i]=sta[i].ontv;
			sta[i].ontv=EOF;
		}
		for(int i=0 ; i<works ; i++)for(int j=work[i].prg ; j<work[i].prg+work[i].num ; j++){
			if( in_time(prg[j].week, prg[j].hour, prg[j].minute, prg[j].time) ){
				sta[ prg[j].station ].ontv=i;
			}
		}
	}
	for(int i=0 ; i<towers ; i++){
		tower[i].c1=0;tower[i].c2=0;
		for(int k=0 ; k<area[tower[i].area].st_num ; k++){
			if(tower[i].ch[k]!=0 && sta[ area[tower[i].area].station[k] ].ontv!=EOF){
				if(gd.game_mode==BOSS){
					tower[i].c1=sta[ area[tower[i].area].station[k] ].ontv;
				}
				else if(tower[i].c1<3)tower[i].c1++;
				if(tower[i].ch[k]!=CHANNELS+1){
					int n=0;
					if(tower[i].kw2==0 && tower[i].kw>=30)n=8;
					else if(tower[i].kw2==0 && tower[i].kw>=20)n=7;
					else if(tower[i].kw2==0 && tower[i].kw>=10)n=6;
					else if(tower[i].kw2<=3)n=5-tower[i].kw2;
					if(tower[i].c2<n)tower[i].c2=n;
				}
			}
		}
	}
	BOOL a=FALSE, b=FALSE;
	for(int i=0 ; i<stas ; i++){
		if(pre[i]!=sta[i].ontv){
			if(sta[i].ontv!=EOF){
				BOOL hit=FALSE;
				for(int j=0 ; j<12 ; j++){
					if(i==BSstation[j]){
						hit=TRUE;
						break;
					}
				}
				if(hit)b=TRUE;
				else a=TRUE;
			}
			map.buffered2=FALSE;
		}
	}
	if(a)Mix_PlayChannel(1,sf.lamp,0);
	if(b)Mix_PlayChannel(2,sf.bubble,0);

	delete [] pre;
}

void drawSMR(SDL_Surface* scr){
	int Y1=120+(SMRcount/(320/SMRfast)+1)/2;
	int Y2=120-(SMRcount/(320/SMRfast)+1)/2;
	fillRect(scr,0,Y1,320,1,255,128,128,255);
	fillRect(scr,0,Y2,320,1,255,128,128,255);
	int a;
	for(int i=0 ; i<320 ; i++)for(int j=0 ; j<240 ; j++){
		a=map.smr[gd.scrX+i][gd.scrY+j];
		if(a==7)fillRect(scr,i,j,1,1,255,255,255,255);
		else if(a==6)fillRect(scr,i,j,1,1,255,255,255,170);
		else if(a==5)fillRect(scr,i,j,1,1,255,255,255,113);
		else if(a==4)fillRect(scr,i,j,1,1,255,255,255,75);
		else if(a==3)fillRect(scr,i,j,1,1,255,255,255,50);
		else if(a==2)fillRect(scr,i,j,1,1,255,255,255,33);
	}
	TextOut(scr,60,0,sta[area[menu[PREF_LIST].selected()].station[menu[SMR].selected()]].name);
	if(SMRcount<240*(320/SMRfast))TextOut(scr,60,20,text[GAMETEXT+22]);
	if(EXPLAIN){
		drawKeyboard(scr,key.xC,0,220);
		TextOut(scr,20,220,text[GAMETEXT+23]);
	}
}

void drawBossGame(SDL_Surface* scr){
	drawImage(scr,img.chr,0,0,190,60,30,20,255);
	if(bd.bossHP<=bd.pre_bossHP){
		drawImage(scr,img.chr,30,10,0,280,(int)(bd.pre_bossHP*28/3000.0),10,255);
		drawImage(scr,img.chr,30,10,0,290,(int)(bd.bossHP*28/3000.0),10,255);
	}else{
		drawImage(scr,img.chr,30,10,0,280,(int)(bd.bossHP*28/3000.0),10,255);
		drawImage(scr,img.chr,30,10,0,290,(int)(bd.pre_bossHP*28/3000.0),10,255);
	}
	if(bd.bossHP>0)drawImage(scr,img.chr,30,10,0,290,1,10,255);
	drawImage(scr,img.chr,200,60,140+((count/5)%2)*25,85,25,35,255);
	for(int i=0 ; i<bd.num ; i++){
		if(bd.color[i]==1)drawImage(scr,img.chr,(int)(bd.atkX[i]-gd.scrX),(int)(bd.atkY[i]-gd.scrY)-20,190,85,20,20,255);
		else drawImage(scr,img.chr,(int)(bd.atkX[i]-gd.scrX),(int)(bd.atkY[i]-gd.scrY)-20,190,105,20,20,255);
	}
	if(phase==HIT_BOSS){
		int a=0;
		if(bd.damage<0)a=20;
		if(start>70)drawImage(scr,img.chr,(int)bd.hitX-gd.scrX,(int)bd.hitY-gd.scrY,210+((count/5)%2)*20,85+a,20,20,255);
		else drawM(scr,start,abs(bd.damage),210,90);
	}
}

void drawClock(SDL_Surface* scr){
	drawImage(scr,img.chr,3,3,120+((count/100)%3)*30,0,30,30,255);
	if(gd.hour>=10)drawImage(scr,img.chr,5,10,(gd.hour/10)*10,223,10,17,255);
	drawImage(scr,img.chr,15,10,(gd.hour%10)*10,223,10,17,255);
	drawImage(scr,img.chr,25,10,100,223,10,17,255);
	drawImage(scr,img.chr,35,10,(gd.minute/10)*10,223,10,17,255);
	drawImage(scr,img.chr,45,10,(gd.minute%10)*10,223,10,17,255);
}

void drawScore(SDL_Surface * scr){
	if(gd.score>=100000)drawImage(scr,img.chr,180,0,(gd.score/100000)*8,202,8,9,255);
	if(gd.score>=10000)drawImage(scr,img.chr,188,0,((gd.score/10000)%10)*8,202,8,9,255);
	if(gd.score>=1000)drawImage(scr,img.chr,196,0,((gd.score/1000)%10)*8,202,8,9,255);
	if(gd.score>=100)drawImage(scr,img.chr,204,0,((gd.score/100)%10)*8,202,8,9,255);
	if(gd.score>=10)drawImage(scr,img.chr,212,0,((gd.score/10)%10)*8,202,8,9,255);
	drawImage(scr,img.chr,220,0,(gd.score%10)*8,202,8,9,255);
	drawImage(scr,img.chr,228,0,93,202,12,9,255);
}

void drawResult(SDL_Surface* scr){
	fillRect(scr,0,0,320,240,0,0,0,255);
	drawImage(scr,img.back,30,20,0,0,140,200,255);
	drawImage(scr,img.back,170,20,180,0,120,200,255);
	drawImage(scr,img.back,30,80,260,200,120,140,255);

	TextOut(scr,100,60,text[GAMETEXT+2]);
	sprintf_s(str,"%3d/%3d",gd.crops,works);
	if(phase==TODAYS_CROP){
		drawImage(scr,img.menuback,10,80,0,0,100,20,128);
		TextOut(scr,50,80,str);
	}
	else if(phase==RESULT){
		if(start>30){
			drawImage(scr,img.menuback,150-(start-30)*7,80,0,0,100,20,128);
			TextOut(scr,190-(start-30)*7,80,str);
		}else{
			drawImage(scr,img.menuback,150,80,0,0,100,20,128);
			TextOut(scr,190,80,str);
		}
	}

	if(phase==TODAYS_CROP){
		int n;
		drawImage(scr,img.menuback,50,100,0,0,260,100,128);
		for(int i=0 ; i<5 ; i++){
			n=count/10+i;
			if(n==works)break;
			if(fishbox.today[n]==EOF)break;
			if(n<0)continue;
			drawImage(scr,img.symbol,55,100+i*20,(work[fishbox.today[n]].mark%16)*17,(work[fishbox.today[n]].mark/16)*17,17,17,255);
			TextOut_lang(scr,70,100+i*20,work[fishbox.today[n]].title,60,JAPANESE);
		}
	}
	if(phase==RESULT){
		if(start<10){
			TextOut(scr,100,110,text[GAMETEXT+6]);
			drawImage(scr,img.menuback,150,130,0,0,100,20,128);
			sprintf_s(str,"%6d",gd.score);
			TextOut(scr,188,130,str);
		}else{
			TextOut(scr,100+(start-10)*8,110,text[GAMETEXT+6]);
			drawImage(scr,img.menuback,150+(start-10)*8,130,0,0,100,20,128);
			sprintf_s(str,"%6d",gd.score);
			TextOut(scr,188+(start-10)*8,130,str);
		}
		TextOut(scr,100+start*8,160,text[GAMETEXT+5]);
		drawImage(scr,img.menuback,150+start*8,180,0,0,100,20,128);
		sprintf_s(str,"%3d%c",100*gd.crops/works,37);
		TextOut(scr,203+start*8,180,str);
	}
}

void drawGetHazia(SDL_Surface *scr){
	if(gd.scene_count==0 && start>0){
		drawImage(scr,img.back,150-start/10,20,140,0,160,200,255);
		drawImage(scr,img.back,130-start/5,80,300+((count/5)%2)*80,0,80,140,255);
		drawImage(scr,img.back,10+start/10,20,0,0,140,200,255);
		drawImage(scr,img.back,20+start/20,80,260,200,120,140,255);
		fillRect(scr,0,0,320,20,0,0,0,255);
		fillRect(scr,0,220,320,20,0,0,0,255);
		fillRect(scr,0,0,30,240,0,0,0,255);
		fillRect(scr,290,0,30,240,0,0,0,255);

		if(start==1){
			int a;
			for(int i=0 ; i<100 ; i++)for(int j=0 ; j<100 ; j++){
				a=255-(int)(255.0*(sqrt((50.0-i)*(50-i)+(50-j)*(50-j))/50));
				if(a>0)fillRect(img.back,620+i,j,1,1,a,a,0,255);
			}
		}
	}
	else if(gd.scene_count==0 || gd.scene_count>=2){
		int a=0;
		if(gd.scene_count==3)a=count;
		drawImage(scr,img.back,90,20,60,200,140,160,255);
		drawImage(scr,img.back,320-((count+70)%120)*6,120,300,140,160,60,255);
		drawImage(scr,img.back,((count+20)%120)*6-160,140,460,160,160,40,255);

		int X,Y,X2,Y2,W,H;

		for(int k=0 ; k<2 ; k++){
			if(k==0){X=((count+20)%120)*6-50;Y=120;}
			if(k==1){X=270-((count+70)%120)*6;Y=140;}
			X2=0;Y2=0;W=100;H=100;

			drawLight(scr,img.back,X,Y,620+X2,Y2,W,H,255);
		}


		drawImage(scr,img.back,120-a,60,300+((count/5)%2)*80,0,80,140,255);

		Y=60;W=40;H=140;
		X2=0;Y2=0;
		int b=0;
		if(count%120<40){
			X=120-a;
			b=(20-abs(20-count%120))*13;
		}
		if((count+50)%120<40){
			X=160-a;X2=40;
			b=(20-abs(20-(count+50)%120))*13;
		}
		if(b!=0){
			drawLight(scr,img.back,X,Y,640+X2,100+Y2,W,H,b);
		}

		drawImage(scr,img.back,30,20,0,200,60,160,255);
		drawImage(scr,img.back,230,20,200,200,60,160,255);
		if((count+70)%120<40)fillRect(scr,30,20,60,160,255,255,255,(20-abs(20-(count+70)%120))*13);
		if((count+20)%120<40)fillRect(scr,230,20,60,160,255,255,255,(20-abs(20-(count+20)%120))*13);
		fillRect(scr,0,0,320,20,0,0,0,255);
		fillRect(scr,0,180,320,60,0,0,0,255);
		fillRect(scr,0,0,30,240,0,0,0,255);
		fillRect(scr,290,0,30,240,0,0,0,255);
		drawTalking(scr);
	}
	else if(gd.scene_count==1){
		int inc=gd.score-indexName[dataNo-1].hiscore;
		if(inc<0)inc=0;
		drawImage(scr,img.back,0,20,0,360,320,200,255);

		int x1=0,y1=60,x2=140,y2=-20,x3=120,y3=100,x4=320,y4=0;
		if(start<260){x1=-(260-start)*2;y1=60+(260-start)*2;}
		if(start<280 && start>=180){x2=140+(280-start)*2;y2=-20-(280-start)*2;}
		if(start<180 && start>=100){x2=300-(180-start)*2;y2=-180+(180-start)*2;}
		if(start<280){x3=120+(280-start)*2;y3=100-(280-start)*2;}
		if(start<180 && start>=100){x4=300-(180-start)*2;y4=-60+(180-start)*2;}
		if(start<100){x4=140;y4=100;}
		drawImage(scr,img.back,x1,y1,320,360,160,160,255);
		drawImage(scr,img.back,x2,y2,480,360,160,160,255);
		drawImage(scr,img.back,x3,y3,460,60,60,60,255);
		drawImage(scr,img.back,x4,y4,520,60,100,100,255);

		fillRect(scr,0,0,320,20,0,0,0,255);
		fillRect(scr,0,220,320,20,0,0,0,255);
		TextOut(scr,120,20,text[EPILOGUE+2]);

		sprintf_s(str,"%10d",indexName[dataNo-1].hiscore);
		if(start>250){
			TextOut(scr,100+(start-250)*5,40,text[GAMETEXT+7]);
			drawImage(scr,img.menuback,170+(start-250)*5,60,0,0,100,20,128);
			TextOut(scr,170+(start-250)*5,60,str);
		}else{
			TextOut(scr,100,40,text[GAMETEXT+7]);
			drawImage(scr,img.menuback,170,60,0,0,100,20,128);
			TextOut(scr,170,60,str);
		}
		sprintf_s(str,"%10d",gd.score);
		if(start>200){
			TextOut(scr,100+(start-200)*5,90,text[GAMETEXT+6]);
			drawImage(scr,img.menuback,170+(start-200)*5,110,0,0,100,20,128);
			TextOut(scr,170+(start-200)*5,110,str);
		}else{
			TextOut(scr,100,90,text[GAMETEXT+6]);
			drawImage(scr,img.menuback,170,110,0,0,100,20,128);
			TextOut(scr,170,110,str);
		}
		TextOut(scr,100,155,text[MIYAZAKITEXT+6]);
		int a=1;
		for(int i=0 ; i<10 ; i++){
			if(gd.hazia/a==0 && gd.hazia>0 || gd.hazia==0 && i>0)continue;
			drawImage(scr,img.chr,260-i*15,155,((gd.hazia/a)%10)*10,260,10,20,255);
			a*=10;
		}

		if(start==100){
			sprintf_s(str,"+%d",inc);
			TextOut(img.back,480,20,str);
		}
		if(start<100 && start>=50){
			int a=50-abs((start-50)-25)*2;
			drawImage(scr,img.back,0,20,((count/2)%2)*320,560,320,200,255);
			drawImage_x2(scr,img.back,110-a/2,90-a/2,(100.0+a)/100.0,460,0,100,60);
		}
		if(start<50 && start>0){
			drawImage(scr,img.back,210-start*2,165-(int)(start*1.5),460,0,100,60,255);
		}
		if(start==0){
			sprintf_s(str,"(+%d)",gd.hazia2);
			TextOut(scr,230,180,str);
		}
	}else{
		if(gd.scene_count==2)drawTalking(scr);
	}
}

void drawPlayer(SDL_Surface* scr){
	if(MAP3D)return;
	if(key.x && !key.up && !key.down && !key.left && !key.right){
		if(gd.doze)drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-13,(int)(gd.y*MAGNIFY)-gd.scrY-19,((count/10)%2)*30+30,30,30,24,255);
		else drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-13,(int)(gd.y*MAGNIFY)-gd.scrY-19,0,30,30,24,255);
	}else{
		int a=0;
		if(map.rural[(int)gd.x/map.rural_size][(int)gd.y/map.rural_size]==NONE)a=15;
		if(DASH_TYPE==TYPE_MEGAMAN && gd.speed>1)drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-33,(int)(gd.y*MAGNIFY)-gd.scrY-19,0,30,30,24,255);
		else drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-7,(int)(gd.y*MAGNIFY)-gd.scrY-25,gd.player_dir*15,0,15,30,255);
		if(gd.speed>1.5){
			drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-16,(int)(gd.y*MAGNIFY)-gd.scrY-20,((count/5)%2)*30,55+a*2,30,30,255);
			if(CHAR_CODE==JAPANESE)drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-16,(int)(gd.y*MAGNIFY)-gd.scrY-20,((count/5)%2)*30,115+a,30,15,255);
			else drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-16,(int)(gd.y*MAGNIFY)-gd.scrY-20,((count/5)%2)*30,145+a,30,15,255);
		}
		if(key.up||key.down||key.left||key.right){
			drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-16,(int)(gd.y*MAGNIFY)-gd.scrY,((count/5)%2)*30,a+75,30,10,255);
			if(phase==PLAYING && gd.real_speed<0.5)drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX,(int)(gd.y*MAGNIFY)-gd.scrY-35,60+((count/10)%2)*10,100,10,10,255);
		}
	}
	if(abs((int)gd.x-gd.memmaX)<20 && abs((int)gd.y-gd.memmaY)<20){
		if(gd.memma_count!=0 && gd.memma_count<20){
			if(gd.memma_type)drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-10,(int)(gd.y*MAGNIFY)-gd.scrY-15,270-(gd.memma_count/10)*20,85,20,20,255);
			else drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-10,(int)(gd.y*MAGNIFY)-gd.scrY-15,270-(gd.memma_count/10)*20,105,20,20,255);
		}
	}
}

void drawFishup(SDL_Surface* scr){
	if((phase==BS_CH||phase==BS_ATTACK) && sta[BSstation[gd.bs_ch]].ontv!=EOF){
		setTmpFish_bs();
		fishbox.drawTable(scr,tmp_fish);
	}
	else if(phase==MANEKI_FISHUP || phase==MANEKI_GRADEUP){
		fishbox.drawTable(scr,md.fish[0]);
	}
	else if(phase==BS_CH && BSstation[gd.bs_ch]!=EOF){
		drawTerop(scr,sta[ BSstation[gd.bs_ch] ].talk,100,0);
		drawImage(scr,img.fishup,0,190,50,0,50,50,255);
	}
	if(phase==BS_ATTACK){
		if(start>0)drawImage(scr,img.fishup,10,20-abs(start%10-5)*5,0,150,160,160,255);
		else drawImage(scr,img.fishup,10,20,0,150,160,160,255);
		if(!animebook[ work[sta[BSstation[gd.bs_ch]].ontv].tnum ]){
			drawImage(scr,img.fishup,150,80,260,0,120,60,255);
			sprintf_s(str,"%4d/%4d",collection+1,allworks);
			TextOut(scr,190,110,str);
		}
	}
	if(phase==CROP){
		fishbox.drawFishBox(scr);
		sprintf_s(str,"%s %d",toChar(text[GAMETEXT+13]),getMissing());
		TextOut(scr,100,0,str,(int)strlen(str));
	}
	if(phase==BS_CH){
		drawImage(scr,img.fishup,80,40,100,0,160,120,255);
		for(int i=0 ; i<12 ; i++){
			if(i==gd.bs_ch)drawImage(scr,img.fishup,90+(i%4)*36,97+(i/4)*20,130,120,30,22,192);
			else drawImage(scr,img.fishup,90+(i%4)*36,97+(i/4)*20,100,120,30,22,192);
		}
		sprintf_s(str,"%s",toChar(sta[BSstation[gd.bs_ch]].name));
		drawImage(scr,img.menuback,60,0,0,0,(int)strlen(str)*9,20,128);
		TextOut(scr,60,0,str,(int)strlen(str));
		drawImage(scr,img.chr,260,25,110,225,20,15,255);
		drawImage(scr,img.chr,280,20,(BSchannel[gd.bs_ch]/10)*10,240,10,20,255);
		drawImage(scr,img.chr,290,20,(BSchannel[gd.bs_ch]%10)*10,240,10,20,255);
	}
	if((phase==FISHUP||phase==MANEKI_FISHUP) && gd.game_mode!=BOSS){
		if(start>40)drawImage(scr,img.fishup,(start-40)*10,20,160,150,160,160,255);
		else drawImage(scr,img.fishup,0,20,160,150,160,160,255);
		int n;
		if(phase==FISHUP)n=work[sta[ant->station].ontv].tnum;
		else n=work[md.fish[0].title_num].tnum;
		if(start<70 && !animebook[n]){
			drawImage(scr,img.fishup,150,80,260,0,120,60,255);
			sprintf_s(str,"%4d/%4d",collection+1,allworks);
			TextOut(scr,190,110,str);
		}
	}
	if(phase==GRADEUP || phase==MANEKI_GRADEUP){
		int a=start-54;
		if(a<0)a=0;
		drawImage(scr,img.chr,240+a*4,100,230,30,70,50,255);
		sprintf_s(str,"+%d",gd.gradeup);
		TextOut(scr,255+a*4,120,str);
	}
}

void drawMenuBox(SDL_Surface* scr){
	if(start<25)drawImage(scr,img.chr,70,60,0,300,40,40,255);
	if(start<20)drawImage(scr,img.chr,130,60,40,300,40,40,255);
	if(start<15)drawImage(scr,img.chr,190,60,80,300,40,40,255);
	if(start<10){
		if(gd.game_mode==STORYMODE)drawImage(scr,img.chr,70,120,120,300,40,40,255);
		else if(MAP3D)drawImage(scr,img.chr,70,120,280,300,40,40,255);
		else drawImage(scr,img.chr,70,120,240,300,40,40,255);
	}
	if(start<5){
		if(gd.game_mode==STORYMODE)drawImage(scr,img.chr,130,120,160,300,40,40,255);
		else if(MAP3D)drawImage(scr,img.chr,130,120,320,300,40,40,255);
	}
	if(start==0)drawImage(scr,img.chr,190,120,200,300,40,40,255);
	drawImage(scr,img.chr,(gd.menu_selected%3)*60+70,(gd.menu_selected/3)*60+60,160,150+((count/20)%2)*40,40,40,255);

	drawImage(scr,img.menuback,60,180,0,0,200,20,128);
	if(gd.menu_selected==0)TextOut(scr,70,182,text[GAMETEXT]);
	else if(gd.menu_selected==1)TextOut(scr,70,182,text[GAMETEXT+1]);
	else if(gd.menu_selected==2)TextOut(scr,70,182,text[TITLETEXT+1]);
	else if(gd.menu_selected==3){
		if(gd.game_mode==STORYMODE)TextOut(scr,70,182,text[GAMETEXT+2]);
		else if(MAP3D){
			sprintf_s(str,"%d",gd.height);
			TextOut(scr,70,182,text[GAMETEXT+26]);
			TextOut(scr,200,182,str);
		}
		else TextOut(scr,70,182,text[GAMETEXT+28]);
	}
	else if(gd.menu_selected==4){
		if(gd.game_mode==STORYMODE)TextOut(scr,70,182,text[GAMETEXT+3]);
		else if(MAP3D){
			sprintf_s(str,"%d",gd.zoom);
			TextOut(scr,70,182,text[GAMETEXT+27]);
			TextOut(scr,200,182,str);
		}
	}
	else if(gd.menu_selected==5)TextOut(scr,70,182,text[GAMETEXT+4]);
}

void drawAntennaMenuBox(SDL_Surface* scr){
	if(start<30)drawImage(scr,img.chr,70,60,0,340,40,40,255);
	if(start<25)drawImage(scr,img.chr,130,60,40,340,40,40,255);
	if(start<20){
		if(gd.bought[0])drawImage(scr,img.chr,190,60,80,340,40,40,255);
		else drawImage(scr,img.chr,190,60,360,300,40,40,255);
	}
	if(start<15){
		if(gd.bought[1])drawImage(scr,img.chr,70,120,120,340,40,40,255);
		else drawImage(scr,img.chr,70,120,360,300,40,40,255);
	}
	if(start<10){
		if(gd.bought[2])drawImage(scr,img.chr,130,120,160,340,40,40,255);
		else drawImage(scr,img.chr,130,120,360,300,40,40,255);
	}
	if(start<5){
		if(gd.bought[3])drawImage(scr,img.chr,190,120,200,340,40,40,255);
		else drawImage(scr,img.chr,190,120,360,300,40,40,255);
	}
	if(start==0){
		if(gd.bought[8])drawImage(scr,img.chr,250,120,240,340,40,40,255);
		else drawImage(scr,img.chr,250,120,360,300,40,40,255);
	}
	if(gd.antenna_selected==6)drawImage(scr,img.chr,250,120,160,150+((count/20)%2)*40,40,40,255);
	else drawImage(scr,img.chr,(gd.antenna_selected%3)*60+70,(gd.antenna_selected/3)*60+60,160,150+((count/20)%2)*40,40,40,255);

	drawImage(scr,img.menuback,60,180,0,0,200,20,128);
	if(gd.antenna_selected==0)TextOut(scr,70,182,text[GAMETEXT+14]);
	else if(gd.antenna_selected==1)TextOut(scr,70,182,text[GAMETEXT+15]);
	else if(gd.antenna_selected==2 && gd.bought[0])TextOut(scr,70,182,text[GAMETEXT+16]);
	else if(gd.antenna_selected==3 && gd.bought[1])TextOut(scr,70,182,text[GAMETEXT+17]);
	else if(gd.antenna_selected==4 && gd.bought[2])TextOut(scr,70,182,text[GAMETEXT+18]);
	else if(gd.antenna_selected==5 && gd.bought[3])TextOut(scr,70,182,text[GAMETEXT+19]);
	else if(gd.antenna_selected==6 && gd.bought[8])TextOut(scr,70,182,text[GAMETEXT+20]);
}

void drawRural(SDL_Surface* scr){
	int a=map.rural[(int)gd.x/map.rural_size][(int)gd.y/map.rural_size];
	if(gd.game_mode!=STORYMODE && gd.game_mode!=SELECT && gd.game_mode!=BOSS)return;
	if(gd.pre_rural!=map.rural_rate[a]){
		gd.pre_rural=map.rural_rate[a];
		gd.count_rural=100;
	}
	if(a==NONE)return;
	if(gd.count_rural>0){
		drawImage(scr,img.chr,270,210,180,30,50,30,255);
		if(gd.pre_rural>=10)drawImage(scr,img.chr,291,222,(gd.pre_rural/10)*10,211,10,12,255);
		drawImage(scr,img.chr,303,222,(gd.pre_rural%10)*10,211,10,12,255);
		gd.count_rural--;
	}
}

void drawSpeedMeter(SDL_Surface* scr){
	double a=PI*gd.speed/30.0;
	drawImage(scr,img.chr,268,188,110,145,50,50,255);
	drawImage(scr,img.chr,316-(int)(cos(a)*50),236-(int)(sin(a)*50),110,195,5,5,255);
	a=PI*gd.real_speed/30.0;
	drawImage(scr,img.chr,316-(int)(cos(a)*50),236-(int)(sin(a)*50),110,195,5,5,255);
}

void drawTimeSlot(SDL_Surface* scr){
	int a=0,b=255;
	if(gd.gaze_count<20)a=320-gd.gaze_count*16;
	if(gd.gaze_count>80)b=(100-gd.gaze_count)*13;
	fillRect(scr,a,100,320,40,0,0,255,128);
	drawImage(scr,img.fishup,a,100,380,0,320,40,b);
	drawImage(scr,img.fishup,a+140,100,380,gd.gaze_num*40+40,160,40,b);
}

void drawManekiTV(SDL_Surface *scr){
	if(md.maneki_mode==NULL)drawImage(scr,img.chr,(int)(md.manekiX*MAGNIFY)-10-gd.scrX,(int)(md.manekiY*MAGNIFY)-15-gd.scrY,20+((count/20)%2)*20,180,20,20,255);
	else if(md.maneki_mode==CARRYING)drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-7-gd.scrX,(int)(gd.y*MAGNIFY)-38-gd.scrY,0,180,20,20,255);
	else if(md.maneki_mode==PLUGGED_IN)drawImage(scr,img.chr,(int)(md.manekiX*MAGNIFY)-10-gd.scrX,(int)(md.manekiY*MAGNIFY)-15-gd.scrY,0,180,20,20,255);
	for(int i=0 ; i<md.fish_num ; i++){
		if(md.maneki_count[i]!=0 && md.maneki_count[i]<40){
			int x=(int)(gd.x+(md.manekiX-gd.x)*md.maneki_count[i]/40)*MAGNIFY;
			int y=(int)(gd.y+(md.manekiY-gd.y)*md.maneki_count[i]/40)*MAGNIFY;
			if(md.fish[i].bs){
				drawImage(scr,img.chr,(int)md.manekiX-gd.scrX-10,(int)md.manekiY-gd.scrY-md.maneki_count[i]*6-30,210,125,20,40,255);
				drawImage(scr,img.chr,x-gd.scrX-10,y-gd.scrY-10,210+(((count/5))%2)*20,105,20,20,255);
			}else drawImage(scr,img.chr,x-gd.scrX-10,y-gd.scrY-10,210+(((count/5))%2)*20,85,20,20,255);
		}
	}
}

void drawGame(SDL_Surface* scr){
	if(phase==GAMESTART){
		drawWeeklyComic(scr);
		drawMap2(scr,gd.scrX,gd.scrY);
	}
	else if(phase==RESULT || phase==TODAYS_CROP)drawResult(scr);
	else if(phase==GET_HAZIA)drawGetHazia(scr);
	else if(phase==SUMMERWARS)drawSummerWars(scr);
	else if(gd.game_mode==BOSS && phase==TALKING && bd.talking==4 && gd.scene_count==1){
		fillRect(scr,0,0,320,240,0,0,0,255);
		drawImage(scr,img.back,0,20,0,0,320,200,255);
		drawImage(scr,img.back,60,60+abs(20-(count/5)%40),320,0,60,40,255);
		drawTalking(scr);
	}else{
		SDL_Color col=getSkyColor(gd.hour,gd.minute);
		fillRect(scr,0,0,320,240,col.r,col.g,col.b,255);
		drawMap(scr, gd.scrX, gd.scrY);
		if(!MAP3D && gd.y>=md.manekiY-3)drawManekiTV(scr);
		if(phase!=ANTENNA && phase!=READY && phase!=LEAVE_MAP)drawPlayer(scr);
		if(!MAP3D && gd.y<md.manekiY-3)drawManekiTV(scr);
		drawFireWork(scr);
		if(phase==CALLING)drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-10,(int)(gd.y*MAGNIFY)-gd.scrY-50,160,30,20,20,255);
		if(phase==FINISH){
			drawImage(scr,img.menuback,0,90,0,0,320,40,128);
			TextOut(scr,100,100,text[GAMETEXT+4],20);
		}
		if(!MAP3D)drawMap2(scr,gd.scrX,gd.scrY);

		if(MAP3D){
			fillRect(scr,260,180,60,60,192,192,255,255);
			drawGround(scr,(int)gd.x-30,(int)gd.y-30,260,180,60,60,1000,FALSE);
			drawTowerSpot(scr,(int)gd.x-30,(int)gd.y-30,260,180,60,60,FALSE);
			int a,b;
			for(int i=0 ; i<30 ; i++){
				a=(int)(i*cos(gd.ant_dir*PI/180));
				b=(int)(i*sin(gd.ant_dir*PI/180));
				fillRect(scr,290+a,210-b,1,1,255,255,0,255);
			}

			if(phase==ADJUST_HEIGHT){
				sprintf_s(str,"%d",gd.height);
				drawImage(scr,img.menuback,60,60,0,0,200,20,128);
				TextOut(scr,70,62,text[GAMETEXT+26]);
				TextOut(scr,200,62,str);
			}
			if(phase==ADJUST_ZOOM){
				sprintf_s(str,"%d",gd.zoom);
				drawImage(scr,img.menuback,60,60,0,0,200,20,128);
				TextOut(scr,70,62,text[GAMETEXT+26]);
				TextOut(scr,200,62,str);
			}
		}

		if(gd.game_mode==STORYMODE||gd.game_mode==SELECT){
			drawClock(scr);
			drawScore(scr);
		}
		if(gd.game_mode==BOSS)drawBossGame(scr);
		if(gd.m_wave>0)drawM(scr,gd.m_wave,rd.seeH[0],(int)(gd.x*MAGNIFY)-gd.scrX-10,(int)(gd.y*MAGNIFY)-gd.scrY+15);
		if(phase==READY){
			if(count>=100)drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX,(int)(gd.y*MAGNIFY)-gd.scrY-(125-count)*6-25,275,0,5,30,255);
			else if(count%20<10)TextOut(scr,150,100,"READY");
		}
		if(phase==LEAVE_MAP){
			if(count<50){
				drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-7,(int)(gd.y*MAGNIFY)-gd.scrY-25,0,0,15,30,255);
			}
			else if(count<120){
				drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-8,(int)(gd.y*MAGNIFY)-gd.scrY-25,160,120,15,30,255);
				if(count<80)drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-13,(int)(gd.y*MAGNIFY)-gd.scrY-30,175+((count-50)/10)*10,125,10,10,255);
			}
			else drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX,(int)(gd.y*MAGNIFY)-gd.scrY-(count-120)*6-25,275,0,5,30,255);
		}

		if(EXPLAIN)ant->drawExplain(scr);

		drawRural(scr);
		if(gd.speed>0)drawSpeedMeter(scr);
		if(phase==PREF_LIST_SMR || phase==SMR)TextOut(scr,60,220,text[GAMETEXT+21]);
		if(phase==SMR_RESULT)drawSMR(scr);
		if(phase==END_YN)TextOut(scr,0,30,text[EPILOGUE+4],50);
		if(ant->ant_mode==TROLL)ant->drawTrolling(scr);
		else ant->drawAntenna(scr);
		drawFishup(scr);
		if(gd.gaze_count!=0)drawTimeSlot(scr);

		if(phase==MENU)drawMenuBox(scr);
		if(phase==ANTENNA_MENU)drawAntennaMenuBox(scr);
		if(phase==GAME_OPTION)drawOptionMenuBox(scr);
		if(phase==GAME_OPTION_SETTING){
			if(count%40<20){
				drawImage(scr,img.chr,150,85,75,110,15,15,225);
				drawImage(scr,img.chr,150,120,75,140,15,15,225);
			}
			drawOptionSetting(scr,60,100);
		}
		if(phase==GAME_KEY_SETTING)drawKeySetting(scr);

		for(int i=0 ; i<15 ; i++)menu[i].drawMenu(scr);
		if(phase==SAVING && count>=150)TextOut(scr,80,30,text[GAMETEXT+29]);
		if(phase==SAVING_RECORD && count>=150)TextOut(scr,80,30,text[EPILOGUE+5]);
		if(phase==MANEKI)drawTalking(scr,1,text[ANTENNATEXT+17+gd.scene_count]);
		else if(phase==MANEKI_CONFIRM)drawTalking(scr,1,text[ANTENNATEXT+24+gd.scene_count]);
		else if(phase!=CALLING)drawTalking(scr);
	}
	drawGameExplain(scr);
}

void drawGameExplain(SDL_Surface* scr){
	if(EXPLAIN){
		if(phase==GAMESTART){
			drawKeyboard(scr,key.zC,0,220);
			TextOut(scr,20,220,text[TITLETEXT]);
		}
		else if(phase==PLAYING){
			if(count%1000<200){
				drawKeyboard(scr,key.zC,0,220);
				TextOut(scr,20,220,text[GAMETEXT+8]);
			}
			else if(count%1000<400){
				if(gd.game_mode==BOSS){
					drawKeyboard(scr,key.xC,0,220);
					TextOut(scr,20,220,text[GAMETEXT+11]);
				}else{
					drawKeyboard(scr,key.xC,0,220);
					TextOut(scr,20,220,text[GAMETEXT+10]);
				}
			}
			else if(count%1000<600){
				drawKeyboard(scr,key.cC,0,220);
				TextOut(scr,20,220,text[GAMETEXT+9]);
			}
			else if(count%1000<800){
				drawKeyboard(scr,key.leftC,0,220);
				drawKeyboard(scr,key.rightC,20,220);
				drawKeyboard(scr,key.upC,40,220);
				drawKeyboard(scr,key.downC,60,220);
				TextOut(scr,80,220,text[GAMETEXT+12]);
			}else{
				if(DASH_TYPE==TYPE_MARIO || DASH_TYPE==TYPE_MEGAMAN){
					drawKeyboard(scr,key.xC,0,220);
					TextOut(scr,20,220,text[GAMETEXT+24]);
				}
				if(DASH_TYPE==TYPE_KIRBY){
					drawKeyboard(scr,key.leftC,0,220);
					drawKeyboard(scr,key.rightC,20,220);
					drawKeyboard(scr,key.upC,40,220);
					drawKeyboard(scr,key.downC,60,220);
					TextOut(scr,80,220,text[GAMETEXT+25]);
				}
			}
		}
		else if(phase==SAVEMENU || phase==SMR || phase==MANEKI&&gd.scene_count==3){
			if(count%600<200){
				drawKeyboard(scr,key.zC,80,0);
				TextOut(scr,100,0,text[OPTIONTEXT+1]);
			}
			else if(count%600<400){
				drawKeyboard(scr,key.xC,80,0);
				TextOut(scr,100,0,text[MENUTEXT+4]);
			}else{
				drawKeyboard(scr,key.upC,80,0);
				drawKeyboard(scr,key.downC,100,0);
				TextOut(scr,120,0,text[MENUTEXT+15]);
			}
		}
		else if(phase==MENU || phase==ANTENNA_MENU){
			if(count%600<200){
				drawKeyboard(scr,key.zC,80,0);
				TextOut(scr,100,0,text[OPTIONTEXT+1]);
			}
			else if(count%600<400){
				drawKeyboard(scr,key.xC,80,0);
				TextOut(scr,100,0,text[MENUTEXT+4]);
			}else{
				drawKeyboard(scr,key.upC,80,0);
				drawKeyboard(scr,key.downC,100,0);
				drawKeyboard(scr,key.leftC,120,0);
				drawKeyboard(scr,key.rightC,140,0);
				TextOut(scr,160,0,text[MENUTEXT+15]);
			}
		}
		else if(phase==ADJUST_HEIGHT || phase==ADJUST_ZOOM){
			if(count%400<200){
				drawKeyboard(scr,key.cC,80,0);
				TextOut(scr,100,0,text[MENUTEXT+4]);
			}else{
				drawKeyboard(scr,key.upC,80,0);
				drawKeyboard(scr,key.downC,100,0);
				TextOut(scr,120,0,text[MENUTEXT+15]);
			}
		}
		else if(phase==PREF_LIST_TRANS || phase==TOWN_LIST || phase==PREF_LIST_SMR){
			if(count%600<200){
				drawKeyboard(scr,key.zC,80,0);
				TextOut(scr,100,0,text[OPTIONTEXT+1]);
			}
			else if(count%600<400){
				drawKeyboard(scr,key.xC,80,0);
				TextOut(scr,100,0,text[MENUTEXT+4]);
			}else{
				drawKeyboard(scr,key.upC,80,0);
				drawKeyboard(scr,key.downC,100,0);
				drawKeyboard(scr,key.leftC,120,0);
				drawKeyboard(scr,key.rightC,140,0);
				TextOut(scr,160,0,text[MENUTEXT+15]);
			}
		}
		else if(phase==SAVING){
			if(count>=150){
				drawKeyboard(scr,key.zC,80,0);
				drawKeyboard(scr,key.xC,100,0);
				TextOut(scr,120,0,text[MENUTEXT+4]);
			}
		}
		else if(phase==BS_CH || phase==BS_ATTACK){
			if(count%400<200){
				drawKeyboard(scr,key.xC,80,20);
				TextOut(scr,100,20,text[MENUTEXT+4]);
			}else{
				drawKeyboard(scr,key.leftC,80,20);
				drawKeyboard(scr,key.rightC,100,20);
				drawKeyboard(scr,key.upC,120,20);
				drawKeyboard(scr,key.downC,140,20);
				TextOut(scr,160,20,text[MENUTEXT+15]);
			}
		}
		else if(phase==FISHUP || phase==GRADEUP){
			drawKeyboard(scr,key.zC,40,160);
			TextOut(scr,60,160,text[MENUTEXT+4]);
		}
		else if(phase==CROP){
			if(count%400<200){
				drawKeyboard(scr,key.upC,0,160);
				drawKeyboard(scr,key.downC,20,160);
				drawKeyboard(scr,key.leftC,40,160);
				drawKeyboard(scr,key.rightC,60,160);
				TextOut(scr,80,160,text[MENUTEXT+15]);
			}else{
				drawKeyboard(scr,key.xC,0,160);
				TextOut(scr,20,160,text[MENUTEXT+4]);
			}
		}
		else if(phase==GAME_OPTION){
			if(count%600<200){
				drawKeyboard(scr,key.zC,0,220);
				TextOut(scr,20,220,text[OPTIONTEXT+1]);
			}
			else if(count%600<400){
				drawKeyboard(scr,key.upC,0,220);
				drawKeyboard(scr,key.downC,20,220);
				drawKeyboard(scr,key.leftC,40,220);
				drawKeyboard(scr,key.rightC,60,220);
				TextOut(scr,80,220,text[MENUTEXT+15]);
			}
			else{
				drawKeyboard(scr,key.xC,0,220);
				TextOut(scr,20,220,text[MENUTEXT+4]);
			}
		}
		else if(phase==GAME_OPTION_SETTING){
			if(count%400<200){
				drawKeyboard(scr,key.upC,0,220);
				drawKeyboard(scr,key.downC,20,220);
				drawKeyboard(scr,key.leftC,40,220);
				drawKeyboard(scr,key.rightC,60,220);
				TextOut(scr,80,220,text[MENUTEXT+15]);
			}else{
				drawKeyboard(scr,key.zC,0,220);
				TextOut(scr,20,220,text[OPTIONTEXT+1]);
			}
		}
		else if(phase==END_YN){
			if(count%600<200){
				drawKeyboard(scr,key.zC,0,220);
				TextOut(scr,20,220,text[OPTIONTEXT+1]);
			}
			else if(count%600<400){
				drawKeyboard(scr,key.xC,0,220);
				TextOut(scr,20,220,text[MENUTEXT+4]);
			}else{
				drawKeyboard(scr,key.upC,0,220);
				drawKeyboard(scr,key.downC,20,220);
				TextOut(scr,40,220,text[MENUTEXT+15]);
			}
		}
		else if(phase==TALKING || phase==MANEKI&&gd.scene_count<3 || phase==SUMMERWARS&&count>=660){
			drawKeyboard(scr,key.zC,70,0);
			TextOut(scr,90,0,text[EPILOGUE+1]);
		}
	}
}

void timerCalling(){
	if(count==100){
		if(face[gd.face_count]==SHAKE){
			Mix_PlayChannel(0,sf.decide,0);
			gd.shake_count=50;
			gd.face_count++;
		}else{
			Mix_PlayChannel(0, sf.noize, 0);
			gd.talk_open_count=1;
		}
		gd.text_count=0;
		phase=TALKING;
	}
}

void timerTodaysCrop(){
	if(count>=0){
		if(count%10==0 && count/10<works && fishbox.today[count/10]!=EOF){
			Mix_PlayChannel(0, sf.knob, 0);
			gd.crops++;
		}
		if(count/10==works || fishbox.today[count/10]==EOF){
			Mix_PlayChannel(0, sf.coin, 0);
			phase=RESULT;
			start=50;
		}
	}
}

void timerGetHazia(){
	if(gd.scene_count==0){
		if(start==1){
			if(dataNo==1)TalkingAt(3);
			else TalkingAt(2);
			gd.talk_open_count=1;
		}
	}
	else if(gd.scene_count==1){
		if(start==100)Mix_PlayChannel(0,sf.thunder,0);
		if(start==0 && count%5==0 && gd.hazia2>0){
			int a=1;
			for(int i=0 ; i<10 ; i++){
				if(gd.hazia2/a>=10)a*=10;
			}
			gd.hazia2-=a;
			gd.hazia+=a;
			if(gd.hazia2==0)Mix_PlayChannel(0,sf.coin,0);
			else Mix_PlayChannel(0,sf.cursor_move,0);
		}
	}
	else if(gd.scene_count==3){
		if(count==100){
			gd.text_count=0;
			phase=END_YN;
			if(indexName[dataNo-1].rate<100*gd.crops/works)indexName[dataNo-1].rate=100*gd.crops/works;
			if(indexName[dataNo-1].rate>100)indexName[dataNo-1].rate=100;
			if(indexName[dataNo-1].hiscore<gd.score)indexName[dataNo-1].hiscore=gd.score;
			clear_num=1;
			for(int i=0 ; i<index_num-1 ; i++){//最終ボス面は保留
				if(indexName[i].rate<60)break;
				clear_num++;
			}
			save_index();
			menu[YNFORM].setViewMode(VISIBLE);
		}
	}
}

void timerSavingGame(){
	String s;
	for(int k=0 ; k<2 ; k++){
		sprintf_s(s.str[k],"%s (%s) %2d:",indexName[dataNo-1].name.str[k],weekChar[gd.week][k],gd.hour);
		if(gd.minute<10)sprintf_s(s.str[k],"%s0%d",s.str[k],gd.minute);
		else sprintf_s(s.str[k],"%s%2d",s.str[k],gd.minute);
	}
	putHeadMark(s);
	if(count==(int)strlen(s.str[CHAR_CODE])*5){
		menu[SAVEMENU].input(menu[SAVEMENU].selected(),s);
		count=150;
	}
	else if(count<(int)strlen(s.str[CHAR_CODE])*5){
		s.str[0][count/5+1]=0;
		s.str[1][count/5+1]=0;
		menu[SAVEMENU].input(menu[SAVEMENU].selected(),s);
	}
	if(count<150 && count%5==0){
		if(s.head[CHAR_CODE][count/5] && s.str[CHAR_CODE][count/5]!=' ')Mix_PlayChannel(0,sf.knob,0);
	}
	if(count==150)Mix_PlayChannel(0,sf.coin,0);
}

void timerSavingRecord(){
	String s;
	sprintf_s(s.str[0],"%s %10d",indexName[dataNo-1].name.str[0],gd.score);
	sprintf_s(s.str[1],"%s %10d",indexName[dataNo-1].name.str[1],gd.score);
	putHeadMark(s);
	if(count==(int)strlen(s.str[CHAR_CODE])*5){
		menu[1].input(menu[1].selected(),s);
		count=150;
	}
	else if(count<(int)strlen(s.str[CHAR_CODE])*5){
		s.str[0][count/5+1]=0;
		s.str[1][count/5+1]=0;
		menu[1].input(menu[1].selected(),s);
	}
	if(count<150 && count%5==0){
		if(s.head[CHAR_CODE][count/5] && s.str[CHAR_CODE][count/5]!=' ')Mix_PlayChannel(0,sf.knob,0);
	}
	if(count==150)Mix_PlayChannel(0,sf.coin,0);
}

void timerSummerWars(){
	if(count==0){
		gd.x=gd.summerX_start;gd.y=gd.summerY_start;
		fix_scrXY();
	}
	if(count>=200 && count<300){
		gd.x+=(gd.summerX-gd.summerX_start)/100.0;
		gd.y+=(gd.summerY-gd.summerY_start)/100.0;
	}
	if(count==400){
		Mix_HaltMusic();
		Mix_PlayChannel(0,sf.swish,0);
	}
	if(count>=420 && count<600){
		if(count%10==0)Mix_PlayChannel(0,sf.thunder,0);
	}
	if(count==500)Mix_PlayChannel(1,sf.meow,0);
}

void timerMemma(){
	if(gd.memma_count>0)gd.memma_count--;
	if(abs((int)gd.x-gd.memmaX)<20 && abs((int)gd.y-gd.memmaY)<20){
		if(gd.memma_count==20){
			gd.memma_type=rand()%2;
			if(gd.memma_type)Mix_PlayChannel(0,sf.decide,0);
			else Mix_PlayChannel(0,sf.bupyo,0);
		}
		if(gd.memma_count==0){
			srand(SDL_GetTicks());
			gd.memma_count=rand()%300;
		}
	}
}

void timerGazing(){
	if(gd.game_mode==STORYMODE){
		if(gd.gaze_count>0)gd.gaze_count++;
		if(gd.gaze_count>100)gd.gaze_count=0;
		if(gd.timeslot[gd.timeslot_count]==gd.week && gd.timeslot[gd.timeslot_count+1]==gd.hour && gd.timeslot[gd.timeslot_count+2]==gd.minute){
			Mix_PlayChannel(0, sf.gaze, 0);
			gd.gaze_num=gd.timeslot[gd.timeslot_count+3]-1;
			gd.gaze_count++;
			gd.timeslot_count+=4;
		}
	}
}

void timerCatchPhone(){
	if(face[gd.face_count]==HANGUP){
		if(gd.week==face[gd.face_count+1] && gd.hour==face[gd.face_count+2] && gd.minute==face[gd.face_count+3]){
			Mix_PlayChannel(0, sf.calling, 0);
			head_of_talking();
			gd.face_count+=4;
			phase=CALLING;
			count=2;
		}
	}
}

void timerSMRcount(){
	if(SMRcount!=0){
		SMRscanned=FALSE;
		for(int i=SMRcount ; i<320*240/SMRfast ; i++){
		startSMR(area[menu[PREF_LIST].selected()].station[menu[SMR].selected()]);
			if(SMRscanned)break;
		}
	}
}

void timerSunMovement(){
	if(gd.second==0){
		if(gd.hour==22 || gd.hour==6 || gd.hour==6&&gd.minute==0 || gd.hour==23&&gd.minute==0){
			if(MAP3D){
				make3dview(gd.x,gd.y,gd.ant_dir);
				make3dview_sky();
			}
			else{
				map.buffered=FALSE;
			}
		}
		if(gd.minute==0){
			if(gd.hour==6){
				createMap_color(1000);
			}
			if(gd.hour==23){
				createMap_color(200);
			}
		}
	}
}

void timerLeaveMap(){
	if(count==50)Mix_PlayChannel(0,sf.gaze,0);
	if(count==120)Mix_PlayChannel(0,sf.swish,0);
	if(count==150){
		endGame();
		initEnding();
	}
}

void moveClock(){
	if(gd.doze)gd.second+=100*MAGNIFY;
	else gd.second++;
	if(gd.second>=100*MAGNIFY)gd.minute++;
	if(gd.minute==60){gd.hour++;gd.minute=0;}
	if(gd.second>=100*MAGNIFY){
		gd.second=0;
		if(gd.game_mode==STORYMODE||gd.game_mode==SELECT)televise();
		if(md.maneki_mode==PLUGGED_IN)ManekiTV_throw();
	}
	if(gd.hour==28){
		if(gd.game_mode==WALKING||gd.game_mode==NO_RELAY){
			gd.hour=4;gd.minute=0;
		}else{
			Mix_PlayChannel(0, sf.decide2, 0);
			phase=FINISH;
		}
	}
	if((gd.hour==12 || gd.hour==24) && gd.minute==0 && gd.second==0)Mix_PlayChannel(3, sf.noon, 0);
	else if(gd.hour==17 && gd.minute==0 && gd.second==0)Mix_PlayChannel(3, sf.sunset, 0);
}

void timerScore(){
	if(phase!=FISHUP && phase!=GRADEUP && phase!=MANEKI_FISHUP && phase!=MANEKI_GRADEUP){
		if(gd.get_score>7){
			gd.get_score-=7;
			gd.score+=7;
		}
		else if(gd.get_score>0){
			gd.score+=gd.get_score;
			gd.get_score=0;
		}
	}
}

void timerGame(){
	if(gd.game_mode!=BOSS && (gd.game_mode==STORYMODE||gd.game_mode==SELECT)){
		if(count==1)Mix_PlayMusic(bgm,-1);
	}
	if(gd.ta_count<87 && count%2==0)gd.ta_count++;
	if(fishbox.text_count<100 && count%2==0)fishbox.text_count++;
	if(gd.m_wave>0)gd.m_wave--;
	if(ant->change_mode_count>0)ant->change_mode_count--;
	if(gd.town_count<400)gd.town_count++;
	timerSMRcount();
	if(phase==READY){
		if(count==100)Mix_PlayChannel(0,sf.swish,0);
		if(count==125)phase=PLAYING;
	}
	else if(phase==PLAYING || phase==HIT_BOSS){
		if(gd.game_mode==BOSS)timer_boss();
		else{
			if(MAP3D){
				if(talk_3dtv && count==300){
					TalkingAt(22);
					gd.talk_open_count=1;
					phase=TALKING;
					talk_3dtv=FALSE;
				}
			}
			moveClock();
			timerMemma();
			timerSunMovement();
			timerCatchPhone();
			for(int i=0 ; i<md.fish_num ; i++){
				if(md.maneki_count[i]!=0){
					if(md.maneki_count[i]==40){
						if(md.fish[i].bs)Mix_PlayChannel(0,sf.swish,0);
						else Mix_PlayChannel(0,sf.meow,0);
					}
					md.maneki_count[i]--;
					if(md.maneki_count[i]==0)ManekiTV_catch();
				}
			}
		}
	}
	else if(phase==CALLING)timerCalling();
	else if(phase==TODAYS_CROP)timerTodaysCrop();
	else if(phase==GET_HAZIA)timerGetHazia();
	else if(phase==SAVING)timerSavingGame();
	else if(phase==SAVING_RECORD)timerSavingRecord();
	else if(phase==LEAVE_MAP)timerLeaveMap();
	else if(phase==SUMMERWARS)timerSummerWars();

	if(phase==FISHUP || phase==MANEKI_FISHUP){
		if(start==149)Mix_PlayChannel(1, sf.water, 0);
		if(start==72){
			Mix_PlayChannel(0, sf.get, 0);
			fishbox.text_count=1;
		}
	}
	if(phase==TALKING || phase==GET_HAZIA && start==0 || phase==MANEKI || phase==MANEKI_CONFIRM || phase==SUMMERWARS && count>=660){
		controlTextCount(TRUE);
	}else{
		controlTextCount(FALSE);
	}
	timerGazing();
	timerScore();
}

void drawM(SDL_Surface* scr, int w, int m, int x, int y){
	int Y1=0,Y2=0,Y3=0,Y4=0;
	if(m==-1){
		drawImage(scr,img.chr,x-3,y-7,88,202,5,9,255);
		return;
	}
	if(w>34){
		Y1=mwave[4*(17-(w-35)/2)];
		Y2=mwave[4*(17-(w-35)/2)+1];
		Y3=mwave[4*(17-(w-35)/2)+2];
		Y4=mwave[4*(17-(w-35)/2)+3];
	}
	if(m>=10000)drawImage(scr,img.chr,x-48,y-Y4-9,(m/10000)*8,202,8,9,255);
	if(m>=1000)drawImage(scr,img.chr,x-40,y-Y1-9,((m/1000)%10)*8,202,8,9,255);
	if(m>=100)drawImage(scr,img.chr,x-32,y-Y2-9,((m/100)%10)*8,202,8,9,255);
	if(m>=10)drawImage(scr,img.chr,x-24,y-Y3-9,((m/10)%10)*8,202,8,9,255);
	drawImage(scr,img.chr,x-16,y-Y4-9,(m%10)*8,202,8,9,255);
	if(gd.game_mode!=BOSS)drawImage(scr,img.chr,x-8,y-9,80,202,8,9,255);
}

void drawTerop(SDL_Surface* scr, String str, int rcv, int mg_rcv){
	String s;
	for(int k=0 ; k<2 ; k++){
		for(int i=0 ; i<40 ; i++){
			s.str[k][i]=str.str[k][ ((count/9)+i) % (int)strlen(str.str[k]) ];
			s.head[k][i]=str.head[k][ ((count/9)+i) % (int)strlen(str.str[k]) ];
		}
		if(!s.head[k][0])s.str[k][0]=' ';
		s.str[k][40]=0;
	}
	drawImage(scr,img.menuback,50,220,0,0,270,20,128);
	TextOut(scr,40-(count)%9,220,jummingText(s,40,rcv,mg_rcv),36);
}

void estimate_rural(){
	int *a, *t;
	a=new int[areas];
	t=new int[areas];

	for(int i=0 ; i<areas ; i++){
		a[i]=i;
		t[i]=0;
	}
	for(int i=0 ; i<areas ; i++){
		for(int j=0 ; j<area[i].st_num ; j++){
			for(int k=0 ; k<works ; k++){
				for(int n=work[k].prg ; n<work[k].prg+work[k].num ; n++){
					if(prg[n].station==area[i].station[j]){
						t[i]++;
						break;
					}
				}
			}
		}
	}

	for(int i=0 ; i<areas-1 ; i++)for(int j=areas-2 ; j>=i ; j--){
		if(t[j]<t[j+1]){
			int A=a[j],T=t[j];
			a[j]=a[j+1];t[j]=t[j+1];
			a[j+1]=A;t[j+1]=T;
		}
	}

	int this_rate=1;
	for(int i=0 ; i<areas ; i++){
		if(i!=0 && t[i-1]!=t[i])this_rate=i+1;
		map.rural_rate[a[i]]=this_rate;
	}
	delete [] a;
	delete [] t;
}

void initManekiTV(){
	BOOL ok=FALSE;
	int X,Y;
	srand(SDL_GetTicks());
	X=rand()%(map.ruralW-2)+1;
	Y=rand()%(map.ruralH-2)+1;
	for(int j=0 ; j<map.ruralH ; j++){
		for(int i=0 ; i<map.ruralW ; i++){
			if(map.rural[X][Y]==NONE){
				ok=TRUE;
				break;
			}
			X=(X+1)%map.ruralW;
		}
		if(ok)break;
		Y=(Y+1)%map.ruralH;
	}
	md.manekiX=X*map.rural_size;
	md.manekiY=Y*map.rural_size;
	md.fish_num=0;
	md.maneki_count[0]=0;
	md.maneki_mode=NULL;
}

void head_of_talking(){
	if(gd.game_mode==BOSS)return;
	if(dataNo==1)TalkingAt(2);
	else TalkingAt(1);
	if(face[gd.face_count]==HANGUP){
		while(gd.face_count<1000){
			if(face[gd.face_count+1]<gd.week
			|| face[gd.face_count+1]==gd.week && face[gd.face_count+2]<gd.hour
			|| face[gd.face_count+1]==gd.week && face[gd.face_count+2]==gd.hour && face[gd.face_count+3]<gd.minute
			){
				gd.face_count+=4;
				while(face[gd.face_count+1]!=HANGUP && face[gd.face_count]!=EOF){
					if(face[gd.face_count]==COMMA || face[gd.face_count]==SHAKE){
						gd.face_count++;
					}else{
						gd.talk_count++;
						gd.face_count++;
					}
				}
				if(face[gd.face_count]==EOF){
					gd.face_count++;
					break;
				}
			}
			else break;
		}
	}
	else gd.face_count++;
}

void head_of_timeslot(){
	if(gd.game_mode==BOSS)return;
	gd.timeslot_count=0;
	while(gd.timeslot[gd.timeslot_count]!=EOF){
		if(gd.timeslot[gd.timeslot_count]<gd.week
		|| gd.timeslot[gd.timeslot_count]==gd.week && gd.timeslot[gd.timeslot_count+1]<gd.hour
		|| gd.timeslot[gd.timeslot_count]==gd.week && gd.timeslot[gd.timeslot_count+1]==gd.hour && gd.timeslot[gd.timeslot_count+2]<gd.minute
		){
			gd.timeslot_count+=4;
		}
		else break;
	}
}

void boss_attack(){
	for(int i=0 ; i<stas ; i++)sta[i].ontv=EOF;
	for(int i=0 ; i<towers ; i++){
		tower[i].c1=0;
		tower[i].c2=0;
	}
	map.buffered2=FALSE;
	srand(SDL_GetTicks());
	int wk=rand()%works;
	bd.num=work[wk].num;
	for(int i=0 ; i<bd.num ; i++){
		bd.atkX[i]=bd.bossX+10;
		bd.atkY[i]=bd.bossY+10;
		bd.station[i]=prg[work[wk].prg+i].station;
		bd.color[i]=rand()%2+1;
		bd.tower[i]=EOF;
		for(int j=0 ; j<areas ; j++){
			for(int k=0 ; k<area[j].st_num ; k++){
				if(area[j].station[k]==bd.station[i]){
					bd.tower[i]=area[j].tower;
					break;
				}
			}
			if(bd.tower[i]!=EOF)break;
		}
		if(bd.tower[i]==EOF){
			bd.num--;
			i--;
			break;
		}
		bd.atk_count[i]=50+i*10;
	}
	if(bd.num==0)count=-1;
}

void timer_boss(){
	bd.bossX=gd.scrX+200;
	bd.bossY=gd.scrY+60;
	if(phase==HIT_BOSS){
		if(start>100)bd.hitY--;
		else if(start==100){
			bd.hit_speedX=(bd.bossX-bd.hitX)/30.0;
			bd.hit_speedY=(bd.bossY+20-bd.hitY)/30.0;
		}
		else if(start>70){
			bd.hitX+=bd.hit_speedX;
			bd.hitY+=bd.hit_speedY;
		}
		else if(start==70){
			bd.bossHP-=bd.damage;
			if(bd.bossHP<0)bd.bossHP=0;
			if(bd.bossHP>30000)bd.bossHP=30000;
			if(bd.damage<0)Mix_PlayChannel(0,sf.meow,0);
			else Mix_PlayChannel(0,sf.decide,0);
		}
		if(start==0){
			phase=PLAYING;
			count=399;
			ant->lower();
		}
	}else{
		for(int i=0 ; i<30 ; i++){
			if(bd.pre_bossHP>bd.bossHP)bd.pre_bossHP--;
			if(bd.pre_bossHP<bd.bossHP)bd.pre_bossHP++;
		}
		if(bd.talking==0){
			TalkingAt(1);
			phase=CALLING;
			Mix_PlayChannel(0,sf.calling,0);
			count=0;
			bd.talking++;
		}
		else if(bd.pre_bossHP<24000 && bd.talking==1){
			TalkingAt(2);
			phase=TALKING;
			bd.talking++;
		}
		else if(bd.pre_bossHP<18000 && bd.talking==2){
			TalkingAt(3);
			phase=TALKING;
			bd.talking++;
		}
		else if(bd.pre_bossHP<15000 && bd.talking==3){
			TalkingAt(4);
			phase=CALLING;
			Mix_PlayChannel(0,sf.calling,0);
			count=0;
			bd.talking++;
		}
		else if(bd.pre_bossHP<12000 && bd.talking==4){
			TalkingAt(5);
			phase=TALKING;
			bd.talking++;
		}
		else if(bd.pre_bossHP<6000 && bd.talking==5){
			TalkingAt(6);
			phase=TALKING;
			bd.talking++;
		}
		else if(bd.pre_bossHP==0 && bd.talking==6){
			TalkingAt(7);
			phase=TALKING;
			bd.talking++;
		}
		if(count%400==0 && bd.bossHP!=0)boss_attack();
		for(int i=0 ; i<bd.num ; i++){
			if(bd.atk_count[i]==50){
				Mix_PlayChannel(2,sf.thunder,0);
				if(bd.tower[i]==EOF){
					bd.atx_speedX[i]=0;
					bd.atx_speedY[i]=0;
				}else{
					bd.atx_speedX[i]=(tower[bd.tower[i]].x-bd.atkX[i])/50.0;
					bd.atx_speedY[i]=(tower[bd.tower[i]].y-bd.atkY[i])/50.0;
				}
			}
			if(bd.atk_count[i]>0 && bd.atk_count[i]<=50){
				bd.atkX[i]+=bd.atx_speedX[i];
				bd.atkY[i]+=bd.atx_speedY[i];
			}
			if(bd.atk_count[i]==1){
				sta[bd.station[i]].ontv=bd.color[i];
				televise();
				map.buffered2=FALSE;
			}
			if(bd.atk_count[i]>0)bd.atk_count[i]--;
		}
	}
}

void setTmpFish_maneki(int n){
	tmp_fish.title_num=sta[md.station[n]].ontv;
	tmp_fish.x=(int)md.manekiX;
	tmp_fish.y=(int)md.manekiY;
	tmp_fish.week=gd.week;
	tmp_fish.hour=gd.hour;
	tmp_fish.minute=gd.minute;
	tmp_fish.sta=md.station[n];
	tmp_fish.bs=md.bs[n];
	tmp_fish.tower=md.tower[n];
	tmp_fish.ch=tower[md.tower[n]].ch[md.ch[n]];
	if(md.rcv[n]>100)tmp_fish.rcv=100;
	else tmp_fish.rcv=md.rcv[n];
	if(md.rcv[n]-md.mg_rcv[n]>100)tmp_fish.mg_rcv=0;
	else if(md.rcv[n]-md.mg_rcv[n]<0)tmp_fish.mg_rcv=100;
	else if(md.rcv[n]>100)tmp_fish.mg_rcv=100-(md.rcv[n]-md.mg_rcv[n]);
	else tmp_fish.mg_rcv=md.mg_rcv[n];
	if(md.bs[n])tmp_fish.score=1;
	else tmp_fish.score=getScore(sta[tmp_fish.sta].ontv,tower[tmp_fish.tower].power[md.ch[n]],(int)md.manekiX,(int)md.manekiY);
}

void setFish_maneki(Fish f){
	if(f.title_num<0 || f.title_num>=works)return;
	if(md.fish_num>=300)return;
	md.fish[md.fish_num].title_num=f.title_num;
	md.fish[md.fish_num].x=f.x;md.fish[md.fish_num].y=f.y;
	md.fish[md.fish_num].hour=f.hour;md.fish[md.fish_num].minute=f.minute;
	md.fish[md.fish_num].week=f.week;
	md.fish[md.fish_num].sta=f.sta;md.fish[md.fish_num].tower=f.tower;
	md.fish[md.fish_num].ch=f.ch;
	md.fish[md.fish_num].rcv=f.rcv;md.fish[md.fish_num].mg_rcv=f.mg_rcv;
	md.fish[md.fish_num].score=f.score;
	md.fish[md.fish_num].bs=f.bs;
	md.fish_num++;
}

void shiftFish_maneki(){
	for(int i=0 ; i<md.fish_num-1 ; i++){
		md.fish[i].title_num=md.fish[i+1].title_num;
		md.fish[i].x=md.fish[i+1].x;
		md.fish[i].y=md.fish[i+1].y;
		md.fish[i].hour=md.fish[i+1].hour;
		md.fish[i].minute=md.fish[i+1].minute;
		md.fish[i].week=md.fish[i+1].week;
		md.fish[i].sta=md.fish[i+1].sta;
		md.fish[i].tower=md.fish[i+1].tower;
		md.fish[i].ch=md.fish[i+1].ch;
		md.fish[i].rcv=md.fish[i+1].rcv;
		md.fish[i].mg_rcv=md.fish[i+1].mg_rcv;
		md.fish[i].score=md.fish[i+1].score;
		md.fish[i].bs=md.fish[i+1].bs;
		md.maneki_count[i]=md.maneki_count[i+1];
	}
	if(md.fish_num>0)md.fish_num--;
}

void setManekiData(){
	for(int i=0 ; i<stas ; i++){
		md.rcv[i]=0;md.mg_rcv[i]=0;md.station[i]=EOF;md.tower[i]=0;md.ch[i]=0;
	}
	Tower *tow=tower;
	for(int i=0 ; i<towers ; i++){
		for(int k=0 ; k<area[tow->area].st_num ; k++){
			int a=area[tow->area].station[k];
			if(tow->ch[k]!=0 && tow->ch[k]!=CHANNELS+1 && tow->rcv[k]>=RCV_LEVEL){

				Uint32 dir2=0;
				int mr2=0;
				mr2=receive_mg(i,tow->ch[k],(int)tow->dir);

				if(tow->rcv[k]-mr2<md.rcv[a]-md.mg_rcv[a])continue;
				md.rcv[a]=tow->rcv[k];
				md.station[a]=a;
				md.tower[a]=i;
				md.ch[a]=k;tower[i].ch[k];
				md.mg_rcv[a]=mr2;
			}
		}
		tow++;
	}

	for(int i=0 ; i<stas-1 ; i++)for(int j=stas-2 ; j>=i ; j--){
		if(md.rcv[j]-md.mg_rcv[j]<md.rcv[j+1]-md.mg_rcv[j+1]){
			int RC=md.rcv[j],ST=md.station[j],TW=md.tower[j],CH=md.ch[j],MR=md.mg_rcv[j];
			md.rcv[j]=md.rcv[j+1];md.station[j]=md.station[j+1];md.tower[j]=md.tower[j+1];md.ch[j]=md.ch[j+1];md.mg_rcv[j]=md.mg_rcv[j+1];
			md.rcv[j+1]=RC;md.station[j+1]=ST;md.tower[j+1]=TW;md.ch[j+1]=CH;md.mg_rcv[j+1]=MR;
		}
	}
	md.st_num=0;
	for(int i=0 ; i<stas ; i++){
		if(md.rcv[i]-md.mg_rcv[i]<RCV_LEVEL)break;
		md.bs[md.st_num]=FALSE;
		md.st_num++;
	}
	for(int i=0 ; i<12 ; i++)if(BSstation[i]!=EOF){
		md.station[md.st_num]=BSstation[i];
		md.rcv[md.st_num]=100;
		md.mg_rcv[md.st_num]=0;
		md.ch[md.st_num]=i+1;
		md.bs[md.st_num]=TRUE;
		md.st_num++;
	}
}

BOOL checkManekiTV(){
	int a=map.rural[(int)md.manekiX/map.rural_size][(int)md.manekiY/map.rural_size];
	BOOL hit;
	for(int i=0 ; i<area[a].st_num ; i++){
		hit=FALSE;
		for(int j=0 ; j<md.st_num ; j++){
			if(area[a].station[i]==md.station[j]){
				hit=TRUE;break;
			}
		}
		if(!hit)return FALSE;
	}
	return TRUE;
}

void ManekiTV_throw(){
	if(md.fish_num>=300)return;
	BOOL skip;
	for(int i=0 ; i<md.st_num ; i++){
		if(sta[md.station[i]].ontv!=EOF){
			setTmpFish_maneki(i);
			skip=FALSE;
			for(int j=0 ; j<md.fish_num ; j++){
				if(md.fish[j].title_num==tmp_fish.title_num && md.fish[j].sta==tmp_fish.sta && md.fish[j].tower==tmp_fish.tower){
					skip=TRUE;
					continue;
				}
			}
			if(skip)continue;
			if(fishbox.getSC(tmp_fish.title_num)<tmp_fish.score || fishbox.getSC(tmp_fish.title_num)==tmp_fish.score && fishbox.getData(tmp_fish.title_num,9)<tmp_fish.rcv-tmp_fish.mg_rcv){
				if(md.fish_num==0)md.maneki_count[0]=40;
				else{
					if(md.maneki_count[md.fish_num-1]>30)md.maneki_count[md.fish_num]=md.maneki_count[md.fish_num-1]+10;
					else md.maneki_count[md.fish_num]=40;
				}
				setFish_maneki(tmp_fish);
			}
		}
	}
}

void ManekiTV_catch(){
	start=72;
	if(fishbox.getSC(md.fish[0].title_num)==0){
		phase=MANEKI_FISHUP;
		gd.get_score+=md.fish[0].score;
		for(int i=0 ; i<works ; i++)if(fishbox.today[i]==EOF){
			fishbox.today[i]=md.fish[0].title_num;
			break;
		}
	}else{
		phase=MANEKI_GRADEUP;
		fishbox.text_count=(int)strlen(work[md.fish[0].title_num].title.str[0]);
		gd.gradeup = md.fish[0].score-fishbox.getSC(md.fish[0].title_num);
		gd.get_score+=gd.gradeup;
		Mix_PlayChannel(1, sf.decide, 0);
	}
	fishbox.setFish(md.fish[0]);
	fishbox.panelColor(md.fish[0].title_num);
}

void makeColorLight(){
	Uint32 *rgb=img.colorlight->RGB;
	for(int m=0 ; m<3 ; m++){
		for(int k=0 ; k<8 ; k++){
			for(int i=0 ; i<80 ; i++)for(int j=0 ; j<80 ; j++){
				double a=15.97*(sqrt((k*5.0-i)*(k*5-i)+(k*5-j)*(k*5-j))/(k*5));
				int b=(int)(a*a);
				int c;
				if(b==0)c=1;
				else if(b>255)c=0;
				else{
					if(AIR_IMG==TYPE1_EDGE||AIR_IMG==TYPE1_BARE)c=b;
					else c=256-b;
				}
				if(m==0)*(rgb+(j)*640+(i+k*80))=setRGB(0,0,c);
				if(m==1)*(rgb+(j+80)*640+(i+k*80))=setRGB(c,0,0);
				if(m==2)*(rgb+(j+160)*640+(i+k*80))=setRGB(c,c,0);
			}
		}
	}

	for(int i=0 ; i<10 ; i++)for(int j=0 ; j<10 ; j++){
		int a=255-(int)(255.0*(sqrt((5.0-i)*(5-i)+(5-j)*(5-j))/5));
		if(a<0)*(rgb+(j*640)+i)=0;
		else *(rgb+(j*640)+i)=setRGB(a,0,0);
	}
}
