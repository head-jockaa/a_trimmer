#include "option.h"

void dupcheck(SDL_Keycode k);
bool key_ok(int k);
void keyOptionMenu();
Uint8 key_setting=0;

Uint32 key_enable[82]={
	0, SDLK_a, SDLK_b, SDLK_c, SDLK_d, SDLK_e, SDLK_f, SDLK_g, SDLK_h, SDLK_i,
	SDLK_j, SDLK_k, SDLK_l, SDLK_m, SDLK_n, SDLK_o, SDLK_p, SDLK_q, SDLK_r, SDLK_s,
	SDLK_t, SDLK_u, SDLK_v, SDLK_w, SDLK_x, SDLK_y, SDLK_z, SDLK_SPACE, SDLK_UP, SDLK_DOWN,
	SDLK_RIGHT, SDLK_LEFT, SDLK_F1, SDLK_F2, SDLK_F3, SDLK_F4, SDLK_F5, SDLK_F6, SDLK_F7, SDLK_F8,
	SDLK_F9, SDLK_F10, SDLK_F11, SDLK_F12, SDLK_0, SDLK_1, SDLK_2, SDLK_3, SDLK_4, SDLK_5,
	SDLK_6, SDLK_7, SDLK_8, SDLK_9, SDLK_COMMA, SDLK_MINUS, SDLK_PERIOD, SDLK_SLASH, SDLK_RETURN, SDLK_BACKSPACE,
	SDLK_LSHIFT, SDLK_RSHIFT, SDLK_LCTRL, SDLK_RCTRL, SDLK_LALT, SDLK_RALT, SDLK_TAB, SDLK_INSERT, SDLK_DELETE, SDLK_HOME,
	SDLK_END, SDLK_PAGEUP, SDLK_PAGEDOWN, SDLK_SCROLLLOCK, SDLK_PAUSE, SDLK_PRINTSCREEN, SDLK_SYSREQ, SDLK_MODE, SDLK_HELP, SDLK_CLEAR,
	SDLK_POWER, SDLK_MENU
};

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
	if(key_setting!=0 && key.aC==key.pressed)key.aC=k;
	if(key_setting!=1 && key.zC==key.pressed)key.zC=k;
	if(key_setting!=2 && key.xC==key.pressed)key.xC=k;
	if(key_setting!=3 && key.cC==key.pressed)key.cC=k;
	if(key_setting!=4 && key.upC==key.pressed)key.upC=k;
	if(key_setting!=5 && key.downC==key.pressed)key.downC=k;
	if(key_setting!=6 && key.leftC==key.pressed)key.leftC=k;
	if(key_setting!=7 && key.rightC==key.pressed)key.rightC=k;
	key.a=0;key.z=0;key.x=0;key.c=0;key.up=0;key.down=0;key.left=0;key.right=0;
}

void keySetting(){
	if(key.count!=2)return;
	if(!key_ok(key.pressed))return;
	switch(key_setting){
		case 0:dupcheck(key.aC);key.aC=key.pressed;break;
		case 1:dupcheck(key.zC);key.zC=key.pressed;break;
		case 2:dupcheck(key.xC);key.xC=key.pressed;break;
		case 3:dupcheck(key.cC);key.cC=key.pressed;break;
		case 4:dupcheck(key.upC);key.upC=key.pressed;break;
		case 5:dupcheck(key.downC);key.downC=key.pressed;break;
		case 6:dupcheck(key.leftC);key.leftC=key.pressed;break;
		case 7:
			dupcheck(key.rightC);key.rightC=key.pressed;
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

void switchSetting(int n, bool up_key){
	if(n==1){
		if(SHOW_TOWER==true)SHOW_TOWER=false;
		else SHOW_TOWER=true;
		if(mode==GAME)map.buffered=false;
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
			map.buffered2=false;
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
				map.buffered=false;
				map.buffered2=false;
			}
			if(MAGNIFY<8)gd.location=EOF;
		}
	}
	else if(n==6){
		if(ADJ_DIR==MANUAL)ADJ_DIR=AUTO;
		else ADJ_DIR=MANUAL;
	}
	else if(n==7){
		if(EXPLAIN==true)EXPLAIN=false;
		else EXPLAIN=true;
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
		if(NHK_REMOVE==true)NHK_REMOVE=false;
		else NHK_REMOVE=true;
		if(mode==GAME && gd.game_mode!=NO_RELAY){
			bool ok;
			for(int i=0 ; i<towers ; i++){
				ok=false;
				if(NHK_REMOVE && tower[i].kw2!=0){
					for(int j=0 ; j<10 ; j++)if(tower[i].ch[j]!=0){
						if(sta[area[tower[i].area].station[j]].mark!=5 && sta[area[tower[i].area].station[j]].mark!=6){
							ok=true;break;
						}
					}
				}
				else ok=true;
				if(ok)tower[i].remove=false;
				else tower[i].remove=true;
			}
			createMap_tower();
			map.buffered=false;
			map.buffered2=false;
			rd.received=false;
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
				rd.received=false;
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
			rd.received=false;
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
		switchSetting(gd.option_selected,true);
	}

	if((key.down||key.left) && (!key_wait(key.down)||!key_wait(key.left))){
		Mix_PlayChannel(1, sf.cursor_move, 0);
		switchSetting(gd.option_selected,false);
	}

	if((key.z||key.x||key.c) && (!(key_stop(key.z))||!key_stop(key.x)||!key_stop(key.c))){
		if(mode==GAME)phase=GAME_OPTION;
		else phase=OPTION_MENU;
		Mix_PlayChannel(1, sf.decide, 0);
	}
}

void drawKeySetting(SDL_Surface* scr){
	drawImage(scr,img.menuback,0,46,0,0,400,296,128);
	fontA=255;
	TextOut2(scr,80,50,"[LANGUAGE]");
	drawKeyboard2(scr,key.aC,40,50);
	TextOut2(scr,80,86,"[Z]");
	drawKeyboard2(scr,key.zC,40,86);
	TextOut2(scr,80,122,"[X]");
	drawKeyboard2(scr,key.xC,40,122);
	TextOut2(scr,80,158,"[C]");
	drawKeyboard2(scr,key.cC,40,158);
	TextOut2(scr,80,194,"[UP]");
	drawKeyboard2(scr,key.upC,40,194);
	TextOut2(scr,80,230,"[DOWN]");
	drawKeyboard2(scr,key.downC,40,230);
	TextOut2(scr,80,266,"[LEFT]");
	drawKeyboard2(scr,key.leftC,40,266);
	TextOut2(scr,80,302,"[RIGHT]");
	drawKeyboard2(scr,key.rightC,40,302);
	drawImage(scr,img.chr,0,key_setting*36+50,280,240,40,40,255);
	TextOut(scr,0,0,text[OPTIONTEXT+25],60);
}

void drawOptionMenuBox(SDL_Surface* scr){
	if(start<60)drawImage(scr,img.chr,60,60,0,760,80,80,255);
	if(start<55)drawImage(scr,img.chr,180,60,80,760,80,80,255);
	if(start<50)drawImage(scr,img.chr,300,60,160,760,80,80,255);
	if(start<45)drawImage(scr,img.chr,420,60,240,760,80,80,255);
	if(start<40)drawImage(scr,img.chr,60,180,320,760,80,80,255);
	if(start<35)drawImage(scr,img.chr,180,180,400,760,80,80,255);
	if(start<30)drawImage(scr,img.chr,300,180,480,760,80,80,255);
	if(start<25)drawImage(scr,img.chr,420,180,560,760,80,80,255);
	if(start<20)drawImage(scr,img.chr,60,300,640,760,80,80,255);
	if(start<15)drawImage(scr,img.chr,180,300,720,760,80,80,255);
	if(start<10){
		if(gd.bought[10])drawImage(scr,img.chr,300,300,800,760,80,80,255);
		else drawImage(scr,img.chr,300,300,720,600,80,80,255);
	}
	if(start<5){
		if(gd.bought[14])drawImage(scr,img.chr,420,300,880,760,80,80,255);
		else drawImage(scr,img.chr,420,300,720,600,80,80,255);
	}
	if(start==0)drawImage(scr,img.chr,540,300,400,600,80,80,255);
	if(gd.option_selected==12)drawImage(scr,img.chr,540,300,320,300+((count/20)%2)*80,80,80,255);
	else drawImage(scr,img.chr,(gd.option_selected%4)*120+60,(gd.option_selected/4)*120+60,320,300+((count/20)%2)*80,80,80,255);
	drawOptionSetting(scr,120,400);
}

void drawOptionSetting(SDL_Surface* scr, int x, int y){
	drawImage(scr,img.menuback,x,y,0,0,400,40,128);
	if(gd.option_selected==0)TextOut2(scr,x+20,y+4,text[OPTIONTEXT]);
	else if(gd.option_selected==1){
		if(SHOW_TOWER)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+2]);
		else TextOut2(scr,x+20,y+4,text[OPTIONTEXT+3]);
	}
	else if(gd.option_selected==2){
		if(AIR_IMG==TYPE1_EDGE)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+4]);
		else if(AIR_IMG==TYPE1_BARE)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+5]);
		else if(AIR_IMG==TYPE2_EDGE)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+6]);
		else if(AIR_IMG==TYPE2_BARE)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+7]);
		else if(AIR_IMG==NULL)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+8]);
	}
	else if(gd.option_selected==3){
		if(DASH_TYPE==TYPE_MARIO)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+9]);
		else if(DASH_TYPE==TYPE_KIRBY)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+10]);
		else if(DASH_TYPE==TYPE_MEGAMAN)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+11]);
		else if(DASH_TYPE==TYPE_SONIC)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+12]);
	}
	else if(gd.option_selected==4)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+13]);
	else if(gd.option_selected==5){
		TextOut2(scr,x+20,y+4,text[OPTIONTEXT+14]);
		sprintf_s(str,"%d",MAGNIFY);
		TextOut2(scr,x+280,y+4,str);
	}
	else if(gd.option_selected==6){
		if(ADJ_DIR==MANUAL)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+15]);
		else if(ADJ_DIR==AUTO)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+16]);
	}
	else if(gd.option_selected==7){
		if(EXPLAIN==true)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+17]);
		else if(EXPLAIN==false)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+18]);
	}
	else if(gd.option_selected==8){
		TextOut2(scr,x+20,y+4,text[OPTIONTEXT+19]);
		sprintf_s(str,"%d",BGM_VOLUME);
		TextOut2(scr,x+200,y+4,str);
	}
	else if(gd.option_selected==9){
		TextOut2(scr,x+20,y+4,text[OPTIONTEXT+20]);
		sprintf_s(str,"%d",SE_VOLUME);
		TextOut2(scr,x+200,y+4,str);
	}
	else if(gd.option_selected==10){
		if(gd.bought[10]){
			if(NHK_REMOVE==false)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+21]);
			else if(NHK_REMOVE==true)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+22]);
		}
	}
	else if(gd.option_selected==11){
		if(gd.bought[14]){
			if(WALKING_TYPE==SMOOTH)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+23]);
			else if(WALKING_TYPE==BUMPY)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+24]);
		}
	}
	else if(gd.option_selected==12)TextOut2(scr,x+20,y+4,text[OPTIONTEXT+1]);
}

void drawOptionExplain(SDL_Surface* scr){
	if(EXPLAIN){
		if(phase==OPTION_MENU){
			if(count%600<200){
				drawKeyboard(scr,key.zC,0,460);
				TextOut(scr,20,460,text[OPTIONTEXT+1]);
			}
			else if(count%600<400){
				drawKeyboard(scr,key.upC,0,460);
				drawKeyboard(scr,key.downC,20,460);
				drawKeyboard(scr,key.leftC,40,460);
				drawKeyboard(scr,key.rightC,60,460);
				TextOut(scr,80,460,text[MENUTEXT+15]);
			}
			else{
				drawKeyboard(scr,key.xC,0,460);
				TextOut(scr,20,460,text[MENUTEXT+4]);
			}
		}
		else if(phase==OPTION_SETTING){
			if(count%400<200){
				drawKeyboard(scr,key.upC,0,460);
				drawKeyboard(scr,key.downC,20,460);
				drawKeyboard(scr,key.leftC,40,460);
				drawKeyboard(scr,key.rightC,60,460);
				TextOut(scr,80,460,text[MENUTEXT+15]);
			}else{
				drawKeyboard(scr,key.zC,0,460);
				TextOut(scr,20,460,text[OPTIONTEXT+1]);
			}
		}
	}
}

void drawOption(SDL_Surface* scr){
	fillRect(scr,0,0,640,480,0,0,0,255);
	drawImage(scr,img.back,64,48,0,0,512,384,255);
	if(phase==OPTION_SETTING){
		if(count%40<20){
			drawImage(scr,img.chr,300,170,150,220,30,30,225);
			drawImage(scr,img.chr,300,240,150,280,30,30,225);
		}
		drawOptionSetting(scr,120,200);
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

bool key_ok(int k){
	for(int i=0 ; i<82 ; i++){
		if(k==key_enable[i])return true;
	}
	return false;
}
