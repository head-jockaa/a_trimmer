#include "option.h"

void dupcheck(SDL_Keycode k);
BOOL key_ok(int k);
void keyOptionMenu();
Uint8 key_setting=0;

void initOption(){
	mode=OPTION;
	phase=OPTION_MENU;
	start=65;
	count=-1;
	getImage(img.back,"file/img/setting.png",0,0,0);
}

void keyOption(){
	switch(phase){
		case OPTION_MENU: keyOptionMenu();break;
		case OPTION_SETTING: keyOptionSetting();break;
		case OPTION_KEY_SETTING: keySetting();break;
		default: break;
	}
}

void dupcheck(int k){
	if(key_setting!=0 && key.aC==ev.key.keysym.sym)key.aC=k;
	if(key_setting!=1 && key.zC==ev.key.keysym.sym)key.zC=k;
	if(key_setting!=2 && key.xC==ev.key.keysym.sym)key.xC=k;
	if(key_setting!=3 && key.cC==ev.key.keysym.sym)key.cC=k;
	if(key_setting!=4 && key.upC==ev.key.keysym.sym)key.upC=k;
	if(key_setting!=5 && key.downC==ev.key.keysym.sym)key.downC=k;
	if(key_setting!=6 && key.leftC==ev.key.keysym.sym)key.leftC=k;
	if(key_setting!=7 && key.rightC==ev.key.keysym.sym)key.rightC=k;
	key.a=0;key.z=0;key.x=0;key.c=0;key.up=0;key.down=0;key.left=0;key.right=0;
}

void keySetting(){
	if(key.count!=2)return;
	if(!key_ok(ev.key.keysym.sym))return;
	switch(key_setting){
		case 0:dupcheck(key.aC);key.aC=ev.key.keysym.sym;break;
		case 1:dupcheck(key.zC);key.zC=ev.key.keysym.sym;break;
		case 2:dupcheck(key.xC);key.xC=ev.key.keysym.sym;break;
		case 3:dupcheck(key.cC);key.cC=ev.key.keysym.sym;break;
		case 4:dupcheck(key.upC);key.upC=ev.key.keysym.sym;break;
		case 5:dupcheck(key.downC);key.downC=ev.key.keysym.sym;break;
		case 6:dupcheck(key.leftC);key.leftC=ev.key.keysym.sym;break;
		case 7:
			dupcheck(key.rightC);key.rightC=ev.key.keysym.sym;
			if(mode==GAME)phase=GAME_OPTION;
			else phase=OPTION_MENU;
			key.right=0;
			Mix_PlayChannel(1, sf.decide, 0);
			break;
		default:break;
	}
	key_setting++;
	if(phase!=OPTION_MENU && phase!=GAME_OPTION)Mix_PlayChannel(1, sf.decide2, 0);
}

void switchSetting(int n, BOOL up_key){
	if(n==1){
		if(SHOW_TOWER==TRUE)SHOW_TOWER=FALSE;
		else SHOW_TOWER=TRUE;
		if(mode==GAME)map.buffered=FALSE;
	}
	else if(n==2){
		if(up_key){
			if(AIR_IMG==TYPE1_EDGE)AIR_IMG=TYPE1_BARE;
			else if(AIR_IMG==TYPE1_BARE)AIR_IMG=TYPE2_EDGE;
			else if(AIR_IMG==TYPE2_EDGE)AIR_IMG=TYPE2_BARE;
			else if(AIR_IMG==TYPE2_BARE)AIR_IMG=NULL;
			else if(AIR_IMG==NULL)AIR_IMG=TYPE1_EDGE;
		}else{
			if(AIR_IMG==NULL)AIR_IMG=TYPE2_BARE;
			else if(AIR_IMG==TYPE2_BARE)AIR_IMG=TYPE2_EDGE;
			else if(AIR_IMG==TYPE2_EDGE)AIR_IMG=TYPE1_BARE;
			else if(AIR_IMG==TYPE1_BARE)AIR_IMG=TYPE1_EDGE;
			else if(AIR_IMG==TYPE1_EDGE)AIR_IMG=NULL;
		}
		if(mode==GAME){
			makeColorLight();
			map.buffered2=FALSE;
		}
	}
	else if(n==3){
		if(up_key){
			if(DASH_TYPE==TYPE_MARIO)DASH_TYPE=TYPE_SONIC;
			else if(DASH_TYPE==TYPE_SONIC){
				if(gd.bought[13])DASH_TYPE=TYPE_KIRBY;
				else DASH_TYPE=TYPE_MARIO;
			}
			else if(DASH_TYPE==TYPE_KIRBY)DASH_TYPE=TYPE_MEGAMAN;
			else if(DASH_TYPE==TYPE_MEGAMAN)DASH_TYPE=TYPE_MARIO;
		}else{
			if(DASH_TYPE==TYPE_MEGAMAN)DASH_TYPE=TYPE_KIRBY;
			else if(DASH_TYPE==TYPE_KIRBY)DASH_TYPE=TYPE_SONIC;
			else if(DASH_TYPE==TYPE_SONIC)DASH_TYPE=TYPE_MARIO;
			else if(DASH_TYPE==TYPE_MARIO){
				if(gd.bought[13])DASH_TYPE=TYPE_MEGAMAN;
				else DASH_TYPE=TYPE_SONIC;
			}
		}
	}
	else if(n==4){
		if(CHAR_CODE==JAPANESE)CHAR_CODE=EUROPEAN;
		else CHAR_CODE=JAPANESE;
	}
	else if(n==5){
		if(mode!=GAME || gd.game_mode!=BOSS){
			if(mode==GAME)gd.second/=MAGNIFY;
			if(up_key){
				MAGNIFY*=2;
				if(!gd.bought[4] && MAGNIFY==8)MAGNIFY=1;
				else if(MAGNIFY==64)MAGNIFY=1;
			}else{
				MAGNIFY/=2;
				if(!gd.bought[4] && MAGNIFY==0)MAGNIFY=4;
				else if(MAGNIFY==0)MAGNIFY=32;
			}
			if(mode==GAME){
				gd.second*=MAGNIFY;
				fix_XY();
				fix_scrXY();
				map.buffered=FALSE;
				map.buffered2=FALSE;
			}
			if(MAGNIFY<8)gd.location=EOF;
		}
	}
	else if(n==6){
		if(ADJ_DIR==MANUAL)ADJ_DIR=AUTO;
		else ADJ_DIR=MANUAL;
	}
	else if(n==7){
		if(EXPLAIN==TRUE)EXPLAIN=FALSE;
		else EXPLAIN=TRUE;
	}
	else if(gd.option_selected==8){
		if(up_key && BGM_VOLUME<128)BGM_VOLUME++;
		else if(!up_key && BGM_VOLUME>0)BGM_VOLUME--;
		Mix_VolumeMusic(BGM_VOLUME);
	}
	else if(n==9){
		if(up_key && SE_VOLUME<128)SE_VOLUME++;
		else if(!up_key && SE_VOLUME>0)SE_VOLUME--;
		for(int i=0 ; i<4 ; i++)Mix_Volume(i,SE_VOLUME);
	}
	else if(n==10){
		if(NHK_REMOVE==TRUE)NHK_REMOVE=FALSE;
		else NHK_REMOVE=TRUE;
		if(mode==GAME && gd.game_mode!=NO_RELAY){
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
			createMap_tower();
			map.buffered=FALSE;
			map.buffered2=FALSE;
			rd.received=FALSE;
		}
	}
	else if(gd.option_selected==11){
		if(WALKING_TYPE==SMOOTH)WALKING_TYPE=BUMPY;
		else WALKING_TYPE=SMOOTH;
	}
}

void keyOptionMenu(){
	if(key.z && !key_stop(key.z)){
		if(gd.option_selected==12){
			gd.option_selected=0;
			save_option();
			if(mode==GAME){
				phase=MENU;
				delete ant;
				setAntenna();
				rd.received=FALSE;
				Mix_PlayChannel(1, sf.decide, 0);
			}else{
				endOption();
				initTitle2();
				start=21;
			}
		}
		else if(gd.option_selected==0){
			if(mode==GAME)phase=GAME_KEY_SETTING;
			else phase=OPTION_KEY_SETTING;
			key.count=60;
			key_setting=0;
			Mix_PlayChannel(1, sf.decide, 0);
		}
		else if(!(!gd.bought[10]&&gd.option_selected==10) && !(!gd.bought[14]&&gd.option_selected==11)){
			if(mode==GAME)phase=GAME_OPTION_SETTING;
			else phase=OPTION_SETTING;
			Mix_PlayChannel(1, sf.decide2, 0);
		}
	}
	if(key.x && !key_stop(key.x)){
		save_option();
		if(mode==GAME){
			phase=MENU;
			delete ant;
			setAntenna();
			rd.received=FALSE;
			Mix_PlayChannel(1, sf.decide, 0);
		}else{
			endOption();
			initTitle2();
			start=21;
		}
	}

	if(key.up && !key_wait(key.up)){
		if(gd.option_selected!=12){
			if(gd.option_selected/4==0)gd.option_selected+=8;
			else gd.option_selected-=4;
		}
		Mix_PlayChannel(0,sf.cursor_move,0);
	}
	if(key.down && !key_wait(key.down)){
		if(gd.option_selected!=12){
			if(gd.option_selected/4==2)gd.option_selected-=8;
			else gd.option_selected+=4;
		}
		Mix_PlayChannel(0,sf.cursor_move,0);
	}
	if(key.left && !key_wait(key.left)){
		if(gd.option_selected==8)gd.option_selected=12;
		else if(gd.option_selected!=12 && gd.option_selected%4==0)gd.option_selected+=3;
		else gd.option_selected--;
		Mix_PlayChannel(0,sf.cursor_move,0);
	}
	if(key.right && !key_wait(key.right)){
		if(gd.option_selected==12)gd.option_selected=8;
		else if(gd.option_selected%4==3)gd.option_selected=12;
		else gd.option_selected++;
		Mix_PlayChannel(0,sf.cursor_move,0);
	}
}

void keyOptionSetting(){
	if((key.up||key.right) && (!key_wait(key.up)||!key_wait(key.right))){
		Mix_PlayChannel(1, sf.cursor_move, 0);
		switchSetting(gd.option_selected,TRUE);
	}

	if((key.down||key.left) && (!key_wait(key.down)||!key_wait(key.left))){
		Mix_PlayChannel(1, sf.cursor_move, 0);
		switchSetting(gd.option_selected,FALSE);
	}

	if((key.z||key.x||key.c) && (!(key_stop(key.z))||!key_stop(key.x)||!key_stop(key.c))){
		if(mode==GAME)phase=GAME_OPTION;
		else phase=OPTION_MENU;
		Mix_PlayChannel(1, sf.decide, 0);
	}
}

void drawKeySetting(SDL_Surface* scr){
	drawImage(scr,img.menuback,0,23,0,0,200,148,128);
	fontA=255;
	TextOut(scr,40,25,"[LANGUAGE]");
	drawKeyboard(scr,key.aC,20,25);
	TextOut(scr,40,43,"[Z]");
	drawKeyboard(scr,key.zC,20,43);
	TextOut(scr,40,61,"[X]");
	drawKeyboard(scr,key.xC,20,61);
	TextOut(scr,40,79,"[C]");
	drawKeyboard(scr,key.cC,20,79);
	TextOut(scr,40,97,"[UP]");
	drawKeyboard(scr,key.upC,20,97);
	TextOut(scr,40,115,"[DOWN]");
	drawKeyboard(scr,key.downC,20,115);
	TextOut(scr,40,133,"[LEFT]");
	drawKeyboard(scr,key.leftC,20,133);
	TextOut(scr,40,151,"[RIGHT]");
	drawKeyboard(scr,key.rightC,20,151);
	drawImage(scr,img.chr,0,key_setting*18+25,140,120,20,20,255);
	TextOut(scr,0,0,text[OPTIONTEXT+25],60);
}

void drawOptionMenuBox(SDL_Surface* scr){
	if(start<60)drawImage(scr,img.chr,30,30,0,380,40,40,255);
	if(start<55)drawImage(scr,img.chr,90,30,40,380,40,40,255);
	if(start<50)drawImage(scr,img.chr,150,30,80,380,40,40,255);
	if(start<45)drawImage(scr,img.chr,210,30,120,380,40,40,255);
	if(start<40)drawImage(scr,img.chr,30,90,160,380,40,40,255);
	if(start<35)drawImage(scr,img.chr,90,90,200,380,40,40,255);
	if(start<30)drawImage(scr,img.chr,150,90,240,380,40,40,255);
	if(start<25)drawImage(scr,img.chr,210,90,280,380,40,40,255);
	if(start<20)drawImage(scr,img.chr,30,150,320,380,40,40,255);
	if(start<15)drawImage(scr,img.chr,90,150,360,380,40,40,255);
	if(start<10){
		if(gd.bought[10])drawImage(scr,img.chr,150,150,400,380,40,40,255);
		else drawImage(scr,img.chr,150,150,360,300,40,40,255);
	}
	if(start<5){
		if(gd.bought[14])drawImage(scr,img.chr,210,150,440,380,40,40,255);
		else drawImage(scr,img.chr,210,150,360,300,40,40,255);
	}
	if(start==0)drawImage(scr,img.chr,270,150,200,300,40,40,255);
	if(gd.option_selected==12)drawImage(scr,img.chr,270,150,160,150+((count/20)%2)*40,40,40,255);
	else drawImage(scr,img.chr,(gd.option_selected%4)*60+30,(gd.option_selected/4)*60+30,160,150+((count/20)%2)*40,40,40,255);
	drawOptionSetting(scr,60,200);
}

void drawOptionSetting(SDL_Surface* scr, int x, int y){
	drawImage(scr,img.menuback,x,y,0,0,200,20,128);
	if(gd.option_selected==0)TextOut(scr,x+10,y+2,text[OPTIONTEXT]);
	else if(gd.option_selected==1){
		if(SHOW_TOWER)TextOut(scr,x+10,y+2,text[OPTIONTEXT+2]);
		else TextOut(scr,x+10,y+2,text[OPTIONTEXT+3]);
	}
	else if(gd.option_selected==2){
		if(AIR_IMG==TYPE1_EDGE)TextOut(scr,x+10,y+2,text[OPTIONTEXT+4]);
		else if(AIR_IMG==TYPE1_BARE)TextOut(scr,x+10,y+2,text[OPTIONTEXT+5]);
		else if(AIR_IMG==TYPE2_EDGE)TextOut(scr,x+10,y+2,text[OPTIONTEXT+6]);
		else if(AIR_IMG==TYPE2_BARE)TextOut(scr,x+10,y+2,text[OPTIONTEXT+7]);
		else if(AIR_IMG==NULL)TextOut(scr,x+10,y+2,text[OPTIONTEXT+8]);
	}
	else if(gd.option_selected==3){
		if(DASH_TYPE==TYPE_MARIO)TextOut(scr,x+10,y+2,text[OPTIONTEXT+9]);
		else if(DASH_TYPE==TYPE_KIRBY)TextOut(scr,x+10,y+2,text[OPTIONTEXT+10]);
		else if(DASH_TYPE==TYPE_MEGAMAN)TextOut(scr,x+10,y+2,text[OPTIONTEXT+11]);
		else if(DASH_TYPE==TYPE_SONIC)TextOut(scr,x+10,y+2,text[OPTIONTEXT+12]);
	}
	else if(gd.option_selected==4)TextOut(scr,x+10,y+2,text[OPTIONTEXT+13]);
	else if(gd.option_selected==5){
		TextOut(scr,x+10,y+2,text[OPTIONTEXT+14]);
		sprintf_s(str,"%d",MAGNIFY);
		TextOut(scr,x+140,y+2,str);
	}
	else if(gd.option_selected==6){
		if(ADJ_DIR==MANUAL)TextOut(scr,x+10,y+2,text[OPTIONTEXT+15]);
		else if(ADJ_DIR==AUTO)TextOut(scr,x+10,y+2,text[OPTIONTEXT+16]);
	}
	else if(gd.option_selected==7){
		if(EXPLAIN==TRUE)TextOut(scr,x+10,y+2,text[OPTIONTEXT+17]);
		else if(EXPLAIN==FALSE)TextOut(scr,x+10,y+2,text[OPTIONTEXT+18]);
	}
	else if(gd.option_selected==8){
		TextOut(scr,x+10,y+2,text[OPTIONTEXT+19]);
		sprintf_s(str,"%d",BGM_VOLUME);
		TextOut(scr,x+100,y+2,str);
	}
	else if(gd.option_selected==9){
		TextOut(scr,x+10,y+2,text[OPTIONTEXT+20]);
		sprintf_s(str,"%d",SE_VOLUME);
		TextOut(scr,x+100,y+2,str);
	}
	else if(gd.option_selected==10){
		if(gd.bought[10]){
			if(NHK_REMOVE==FALSE)TextOut(scr,x+10,y+2,text[OPTIONTEXT+21]);
			else if(NHK_REMOVE==TRUE)TextOut(scr,x+10,y+2,text[OPTIONTEXT+22]);
		}
	}
	else if(gd.option_selected==11){
		if(gd.bought[14]){
			if(WALKING_TYPE==SMOOTH)TextOut(scr,x+10,y+2,text[OPTIONTEXT+23]);
			else if(WALKING_TYPE==BUMPY)TextOut(scr,x+10,y+2,text[OPTIONTEXT+24]);
		}
	}
	else if(gd.option_selected==12)TextOut(scr,x+10,y+2,text[OPTIONTEXT+1]);
}

void drawOptionExplain(SDL_Surface* scr){
	if(EXPLAIN){
		if(phase==OPTION_MENU){
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
		else if(phase==OPTION_SETTING){
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
	}
}

void drawOption(SDL_Surface* scr){
	fillRect(scr,0,0,320,240,0,0,0,255);
	drawImage(scr,img.back,32,24,0,0,256,192,255);
	if(phase==OPTION_SETTING){
		if(count%40<20){
			drawImage(scr,img.chr,150,85,75,110,15,15,225);
			drawImage(scr,img.chr,150,120,75,140,15,15,225);
		}
		drawOptionSetting(scr,60,100);
	}
	else drawOptionMenuBox(scr);
	if(phase==OPTION_KEY_SETTING)drawKeySetting(scr);
	drawOptionExplain(scr);
}

void endOption(){
	freeImage(img.back);
	kick_count=1;
}

void timerOption(){
}

BOOL key_ok(int k){
	BOOL b=FALSE;
	int a[]={8,9,12,13,19,32,44,45,46,47,48,49,50,51,52,53,54,55,56,57,
			 97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,
			 113,114,115,116,117,118,119,120,121,122,127,
			 273,274,275,276,277,278,279,280,281,288,289,290,291,292,293,
			 302,303,304,305,306,307,308,313,315,316,317,318,319,320};
	for(int i=0 ; i<76 ; i++){
		if(k==a[i]){b=TRUE;break;}
	}
	return b;
}
