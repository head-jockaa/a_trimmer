#include "util.h"

int price[18], shop_icon[18];

void makeShoppingList();

void initHaziaShop(){
	gd.x=80;
	gd.y=340;
	gd.talk_count=EOF;
	mode=HAZIASHOP;
	phase=SHOP_FLOOR;
	menu[YNFORM].setMenu(320,300,12,3,3);
	menu[YNFORM].stack("");
	menu[YNFORM].stack("");
	menu[YNFORM].stack("");
	menu[SHOP_CONFIRM].setMenu(320,300,18,3,3);
	menu[SHOP_CONFIRM].stack(text[MIYAZAKITEXT+7]);
	menu[SHOP_CONFIRM].stack(text[MIYAZAKITEXT+13]);
	menu[SHOP_CONFIRM].stack(text[MIYAZAKITEXT+8]);
	menu[OHANA_MENU].setMenu(320,200,10,2,2);
	menu[OHANA_MENU].stack(text[MIYAZAKITEXT+31]);
	menu[OHANA_MENU].stack(text[EPILOGUE+10]);
	load_story(0);
	getImage(img.back,"file/img/haziashop.png",0,0,255);
	sf.coin=Mix_LoadWAV("file/se/17.wav");
	sf.swish=Mix_LoadWAV("file/se/25.wav");
	sf.water=Mix_LoadWAV("file/se/9.wav");
	sf.grumble=Mix_LoadWAV("file/se/19.wav");
	bgm=Mix_LoadMUS("file/bgm/4.ogg");
	Mix_PlayMusic(bgm,-1);
}

void endHaziaShop(){
	kick_count++;
	freeImage(img.back);
	Mix_FreeChunk(sf.coin);
	Mix_FreeChunk(sf.swish);
	Mix_FreeChunk(sf.water);
	Mix_FreeChunk(sf.grumble);
	freeMusic();
	for(int i=0 ; i<15 ; i++)menu[i].reset();
}

void keyShopFloor(){
	if(key.z && !key_stop(key.z)){
		if(gd.y==140 && gd.x>120 && gd.x<240){
			phase=SHOP_TALK;
			TalkingAt(29);
			gd.talk_open_count=1;
		}
		if(gd.y==140 && gd.x>300 && gd.x<400){
			phase=NAGANO_APPLE;
			TalkingAt(35);
			gd.talk_open_count=1;
		}
		if(gd.y>240 && gd.y<340 && gd.x>400 && gd.x<500){
			phase=ARANCIA;
			TalkingAt(36);
			gd.talk_open_count=1;
		}
		if(gd.y==40 && gd.x>=400){
			phase=MEMMA_TALK;
			TalkingAt(37);
			gd.talk_open_count=1;
			count=0;
		}
		if(gd.x==560 && gd.y>=120 && gd.y<=200){
			phase=OHANA_TALK;
			TalkingAt(38);
			gd.talk_open_count=1;
			count=0;
		}
	}
	if(key.up && key.left)gd.player_dir=6;
	else if(key.up && key.right)gd.player_dir=7;
	else if(key.down && key.left)gd.player_dir=4;
	else if(key.down && key.right)gd.player_dir=5;
	else if(key.up)gd.player_dir=2;
	else if(key.down)gd.player_dir=0;
	else if(key.left)gd.player_dir=3;
	else if(key.right)gd.player_dir=1;
	if(key.up)gd.y-=4;
	if(key.down)gd.y+=4;
	if(key.left)gd.x-=4;
	if(key.right)gd.x+=4;
	if(gd.x<40)gd.x=40;
	if(gd.x>560)gd.x=560;
	if(gd.y<40)gd.y=40;
	if(gd.y>380)gd.y=380;
	if(key.left && gd.x>380 && gd.x<400 && gd.y<140)gd.x=400;
	if(key.up && gd.x<400 && gd.y>120 && gd.y<140)gd.y=140;
	if(gd.y==380 && gd.x>160 && gd.x<200){
		endHaziaShop();
		initMiyazaki();
		phase=MIYAZAKI_MUSEUM;
		gd.x=200;
	}
}

void keyShopping(){
	if(key.z && !key_stop(key.z)){
		if(gd.bought[menu[SHOPPING].selected()]){
			menu[YNFORM].input(0,text[MIYAZAKITEXT+9]);
			menu[YNFORM].input(1,text[MIYAZAKITEXT+10]);
			menu[YNFORM].input(2,text[MIYAZAKITEXT+11]);
			phase=SELLING;
		}else{
			menu[YNFORM].input(0,text[MIYAZAKITEXT]);
			menu[YNFORM].input(1,text[MIYAZAKITEXT+7]);
			menu[YNFORM].input(2,text[MIYAZAKITEXT+8]);
			phase=BUYING;
		}
		menu[YNFORM].setViewMode(VISIBLE);
		gd.hazia2=0;
		if(gd.talk_count!=EOF)gd.talk_open_count=1;
		gd.talk_count=EOF;
	}
	if(key.x && !key_stop(key.x)){
		gd.text_count=0;
		bool ok=true;
		for(int i=0 ; i<18 ; i++)if(!gd.bought[i]){
			ok=false;break;
		}
		menu[YNFORM].setViewMode(HIDE);
		menu[SHOPPING].setViewMode(HIDE);
		gd.talk_open_count=1;
		phase=SHOP_LASTTALK;
		if(ok){
			TalkingAt(34);
		}else{
			TalkingAt(33);
		}
	}
	if(key.up && !key_wait(key.up)){
		menu[SHOPPING].cursorUp();
	}
	if(key.down && !key_wait(key.down)){
		menu[SHOPPING].cursorDown();
	}
}

void keySelling(){
	if(gd.hazia2>0)return;
	if(key.z && !key_stop(key.z)){
		if(menu[YNFORM].selected()==0){
			TalkingAt(60+menu[SHOPPING].selected());
			gd.talk_open_count=1;
			gd.text_count=0;
		}
		if(menu[YNFORM].selected()==1){
			String s;
			sprintf_s(s.str[0],"%10d%s",price[menu[SHOPPING].selected()],text[MIYAZAKITEXT+6].str[0]);
			sprintf_s(s.str[1],"%10d%s",price[menu[SHOPPING].selected()],text[MIYAZAKITEXT+6].str[1]);
			menu[SHOPPING].input(menu[SHOPPING].selected()*2+1,s);
			menu[SHOPPING].gray[menu[SHOPPING].selected()*2+1]=false;
			gd.bought[menu[SHOPPING].selected()]=false;
			gd.hazia2=price[menu[SHOPPING].selected()];
			if(gd.talk_count!=EOF)gd.talk_open_count=1;
			gd.talk_count=EOF;
			save_index();
			if(menu[SHOPPING].selected()==0 && ROD_TYPE==UVROD)ROD_TYPE=SIMPLEROD;
			else if(menu[SHOPPING].selected()==1 && ROD_TYPE==MHZROD)ROD_TYPE=SIMPLEROD;
			else if(menu[SHOPPING].selected()==2 && ROD_TYPE==BUTTONROD)ROD_TYPE=SIMPLEROD;
			else if(menu[SHOPPING].selected()==3 && ROD_TYPE==SUPERHANDYROD)ROD_TYPE=SIMPLEROD;
			else if(menu[SHOPPING].selected()==4 && MAGNIFY>=8)MAGNIFY=1;
			else if(menu[SHOPPING].selected()==10)NHK_REMOVE=false;
			else if(menu[SHOPPING].selected()==13 && (DASH_TYPE==TYPE_KIRBY||DASH_TYPE==TYPE_MEGAMAN))DASH_TYPE=TYPE_MARIO;
			else if(menu[SHOPPING].selected()==14)WALKING_TYPE=SMOOTH;
			else if(menu[SHOPPING].selected()==15)scr_design=NULL;
			else if(menu[SHOPPING].selected()==16)scr_design=NULL;
			else if(menu[SHOPPING].selected()==17)CHOSEON=false;
		}
		if(menu[YNFORM].selected()==2){
			menu[YNFORM].setViewMode(HIDE);
			if(gd.talk_count!=EOF)gd.talk_open_count=1;
			gd.talk_count=EOF;
			phase=SHOPPING;
		}
	}
	if(key.x && !key_stop(key.x)){
		menu[YNFORM].setViewMode(HIDE);
		if(gd.talk_count!=EOF)gd.talk_open_count=1;
		gd.talk_count=EOF;
		phase=SHOPPING;
	}
	if(key.up && !key_wait(key.up))menu[YNFORM].cursorUp();
	if(key.down && !key_wait(key.down))menu[YNFORM].cursorDown();
}

void keyBuying(){
	if(gd.hazia2>0)return;
	if(key.z && !key_stop(key.z)){
		if(menu[YNFORM].selected()==0){
			TalkingAt(42+menu[SHOPPING].selected());
			gd.talk_open_count=1;
			gd.text_count=0;
		}
		if(menu[YNFORM].selected()==1){
			if(gd.hazia>price[menu[SHOPPING].selected()]){
				if(menu[SHOPPING].selected()>=13){
					TalkingAt(30);
					phase=SHOP_CONFIRM_TALK;
					menu[YNFORM].setViewMode(GRAY);
					gd.talk_open_count=1;
					gd.text_count=0;
					gd.scene_count=0;
				}else{
					menu[SHOPPING].input(menu[SHOPPING].selected()*2+1,text[MIYAZAKITEXT+12]);
					menu[SHOPPING].gray[menu[SHOPPING].selected()*2+1]=true;
					gd.bought[menu[SHOPPING].selected()]=true;
					gd.hazia2=price[menu[SHOPPING].selected()];
					if(gd.talk_count!=EOF)gd.talk_open_count=1;
					gd.talk_count=EOF;
					save_index();
				}
			}
		}
		if(menu[YNFORM].selected()==2){
			menu[YNFORM].setViewMode(HIDE);
			if(gd.talk_count!=EOF)gd.talk_open_count=1;
			gd.talk_count=EOF;
			phase=SHOPPING;
		}
	}
	if(key.x && !key_stop(key.x)){
		menu[YNFORM].setViewMode(HIDE);
		if(gd.talk_count!=EOF)gd.talk_open_count=1;
		gd.talk_count=EOF;
		phase=SHOPPING;
	}
	if(key.up && !key_wait(key.up))menu[YNFORM].cursorUp();
	if(key.down && !key_wait(key.down))menu[YNFORM].cursorDown();
}

void keyHaziaShopTalk(){
	if(key.z && !key_stop(key.z)){
		int a=controlTalking();
		if(a==EOF){
			if(phase==SHOP_TALK){
				makeShoppingList();
				menu[SHOPPING].setViewMode(VISIBLE);
				phase=SHOPPING;
			}
			else if(phase==SHOP_CONFIRM_TALK){
				if(gd.scene_count==0){
					menu[SHOP_CONFIRM].setViewMode(VISIBLE);
					menu[YNFORM].setViewMode(HIDE);
					phase=SHOP_CONFIRM;
				}
				else if(gd.scene_count==1){
					menu[SHOP_CONFIRM].setViewMode(HIDE);
					phase=SHOPPING;
				}
				else if(gd.scene_count==2){
					phase=BUYING;
					menu[SHOP_CONFIRM].setViewMode(HIDE);
					menu[SHOPPING].input(menu[SHOPPING].selected()*2+1,text[MIYAZAKITEXT+12]);
					menu[SHOPPING].gray[menu[SHOPPING].selected()*2+1]=true;
					gd.bought[menu[SHOPPING].selected()]=true;
					gd.hazia2=price[menu[SHOPPING].selected()];
					if(gd.talk_count!=EOF)gd.talk_open_count=1;
					gd.talk_count=EOF;
					save_index();
				}
			}
			else if(phase==MEMMA_TALK){
				phase=THROW_MEMMA;
				count=0;
			}
			else if(phase==OHANA_TALK){
				if(gd.scene_count==0){
					menu[OHANA_MENU].setViewMode(VISIBLE);
					phase=OHANA_MENU;
				}
				else if(gd.scene_count==1){
					phase=STAY_IN;
					count=0;
				}
				else phase=SHOP_FLOOR;
			}
			else{
				phase=SHOP_FLOOR;
			}
		}
	}
}

void keyShopConfirm(){
	if(key.z && !key_wait(key.z)){
		if(menu[SHOP_CONFIRM].selected()==0){
			TalkingAt(31);
			gd.scene_count=1;
			gd.text_count=0;
			gd.talk_open_count=1;
			menu[SHOP_CONFIRM].setViewMode(GRAY);
			phase=SHOP_CONFIRM_TALK;
		}
		else if(menu[SHOP_CONFIRM].selected()==1){
			TalkingAt(32);
			gd.scene_count=2;
			gd.text_count=0;
			gd.talk_open_count=1;
			phase=SHOP_CONFIRM_TALK;
		}else{
			menu[SHOP_CONFIRM].setViewMode(HIDE);
			phase=SHOPPING;
		}
	}
	if(key.x && !key_wait(key.x)){
		menu[SHOP_CONFIRM].setViewMode(HIDE);
		phase=SHOPPING;
	}
	if(key.up && !key_wait(key.up))menu[SHOP_CONFIRM].cursorUp();
	if(key.down && !key_wait(key.down))menu[SHOP_CONFIRM].cursorDown();
}

void keyOhanaMenu(){
	if(key.z && !key_wait(key.z)){
		if(menu[OHANA_MENU].selected()==0){
			TalkingAt(39);
			gd.scene_count=1;
		}
		else if(menu[OHANA_MENU].selected()==1){
			TalkingAt(41);
			gd.scene_count=3;
		}
		gd.text_count=0;
		gd.talk_open_count=1;
		menu[OHANA_MENU].setViewMode(HIDE);
		phase=OHANA_TALK;
	}
	if(key.x && !key_wait(key.x)){
		menu[OHANA_MENU].setViewMode(HIDE);
		phase=SHOP_FLOOR;
	}
	if(key.up && !key_wait(key.up))menu[OHANA_MENU].cursorUp();
	if(key.down && !key_wait(key.down))menu[OHANA_MENU].cursorDown();
}

void keyHaziaShop(){
	switch(phase){
		case MEMMA_TALK:
		case OHANA_TALK:
		case ARANCIA:
		case NAGANO_APPLE:
		case SHOP_TALK:
		case SHOP_CONFIRM_TALK:
		case SHOP_LASTTALK:keyHaziaShopTalk();break;
		case SHOP_FLOOR:keyShopFloor();break;
		case SHOPPING:keyShopping();break;
		case SHOP_CONFIRM:keyShopConfirm();break;
		case SELLING:keySelling();break;
		case BUYING:keyBuying();break;
		case OHANA_MENU:keyOhanaMenu();break;
		default: break;
	}
}

void timerHaziaShop(){
	if(phase==BUYING || phase==SELLING){
		if(count%5==0 && gd.hazia2>0){
			int a=1;
			for(int i=0 ; i<10 ; i++){
				if(gd.hazia2/a>=10)a*=10;
			}
			gd.hazia2-=a;
			if(phase==SELLING)gd.hazia+=a;
			else gd.hazia-=a;
			if(gd.hazia2==0){
				if(phase==BUYING){
					menu[YNFORM].input(0,text[MIYAZAKITEXT+9]);
					menu[YNFORM].input(1,text[MIYAZAKITEXT+10]);
					menu[YNFORM].input(2,text[MIYAZAKITEXT+11]);
					TalkingAt(60+menu[SHOPPING].selected());
					gd.talk_open_count=1;
					gd.text_count=0;
				}
				else if(phase==SELLING){
					menu[YNFORM].input(0,text[MIYAZAKITEXT]);
					menu[YNFORM].input(1,text[MIYAZAKITEXT+7]);
					menu[YNFORM].input(2,text[MIYAZAKITEXT+8]);
				}
				Mix_PlayChannel(0,sf.coin,0);
				for(int i=0 ; i<2 ; i++)menu[YNFORM].cursorUp();
				menu[YNFORM].setViewMode(HIDE);
				phase=SHOPPING;
			}
			else Mix_PlayChannel(0,sf.cursor_move,0);
		}
	}
	if(phase==THROW_MEMMA){
		if(count==1)Mix_PlayChannel(0,sf.swish,0);
		if(count==30){
			Mix_PlayChannel(0,sf.water,0);
			phase=SHOP_FLOOR;
		}
	}
	if(phase==STAY_IN){
		if(count==0)Mix_FadeOutMusic(1000);
		if(count==100)Mix_PlayChannel(0,sf.grumble,0);
		if(count==300)Mix_PlayMusic(bgm,-1);
		if(count==400){
			TalkingAt(40);
			gd.scene_count=2;
			gd.text_count=0;
			gd.talk_open_count=1;
			menu[OHANA_MENU].setViewMode(HIDE);
			phase=OHANA_TALK;
		}
	}
	if(phase==SHOP_TALK || phase==SHOPPING || phase==BUYING || phase==SELLING || phase==SHOP_LASTTALK || phase==MEMMA_TALK || phase==SHOP_CONFIRM_TALK || phase==ARANCIA || phase==NAGANO_APPLE || phase==OHANA_TALK){
		controlTextCount(true);
	}else{
		controlTextCount(false);
	}
}

void drawHaziaShopExplain(SDL_Surface* scr){
	if(EXPLAIN){
		if(phase==SHOP_FLOOR){
			drawKeyboard(scr,key.upC,160,460);
			drawKeyboard(scr,key.downC,180,460);
			drawKeyboard(scr,key.leftC,200,460);
			drawKeyboard(scr,key.rightC,220,460);
			TextOut(scr,480,440,text[GAMETEXT+12]);
			if((gd.y==140 && gd.x<400) || (gd.y==40 && gd.x>=400)){
				drawKeyboard(scr,key.zC,0,0);
				TextOut(scr,20,0,text[EPILOGUE+10]);
			}
			if((gd.y==140 && gd.x>300 && gd.x<400)||(gd.y>240 && gd.y<340 && gd.x>400 && gd.x<500)||(gd.x==560 && gd.y>=120 && gd.y<=200)){
				drawKeyboard(scr,key.zC,0,0);
				TextOut(scr,20,0,text[MIYAZAKITEXT+30]);
			}
		}
		else if(phase==SHOP_TALK || phase==SHOP_LASTTALK || phase==MEMMA_TALK || phase==SHOP_CONFIRM_TALK || phase==OHANA_TALK){
			drawKeyboard(scr,key.zC,0,0);
			TextOut(scr,20,0,text[EPILOGUE+1]);
		}
		else if(phase!=STAY_IN){
			if(count%600<200){
				drawKeyboard(scr,key.upC,0,0);
				drawKeyboard(scr,key.downC,20,0);
				TextOut(scr,40,0,text[MENUTEXT+15]);
			}
			else if(count%600<400){
				drawKeyboard(scr,key.zC,0,0);
				TextOut(scr,20,0,text[OPTIONTEXT+1]);
			}else{
				drawKeyboard(scr,key.xC,0,0);
				TextOut(scr,20,0,text[MENUTEXT+4]);
			}
		}
	}
}

void drawHaziaShop(SDL_Surface* scr){
	if(phase==NAGANO_APPLE){
		drawImage(scr,img.back,0,0,0,960,640,360,255);
		fillRect(scr,0,360,640,120,0,0,0,255);
	}
	else if(phase==ARANCIA){
		drawImage(scr,img.back,0,0,0,1320,640,360,255);
		fillRect(scr,0,360,640,120,0,0,0,255);
	}
	else if(phase==SHOP_FLOOR || phase==OHANA_TALK || phase==OHANA_MENU || phase==MEMMA_TALK || phase==THROW_MEMMA || (phase==STAY_IN && count<30)){
		drawImage(scr,img.back,0,0,0,0,640,480,255);
		drawImage(scr,img.back,160,80,640+((count/5)%2)*50,0,50,40,255);
		if(phase==MEMMA_TALK && count<20)drawImage(scr,img.back,480,0,700,40,60,60,255);
		else if(phase==MEMMA_TALK)drawImage(scr,img.back,480,0,760,40,60,60,255);
		else drawImage(scr,img.back,480,0,640,40,60,60,255);
		drawImage(scr,img.chr,(int)gd.x,(int)gd.y,gd.player_dir*30,0,30,60,255);
		if(key.up||key.down||key.left||key.right){
			drawImage(scr,img.chr,(int)gd.x-20,(int)gd.y+20,((count/5)%2)*60,110,60,60,255);
		}
		if((gd.y==140 && gd.x>120 && gd.x<240)
		   ||(gd.y==140 && gd.x>300 && gd.x<400)
		   ||(gd.y==40 && gd.x>=400)
		   ||(gd.y>240 && gd.y<340 && gd.x>400 && gd.x<500)
		   ||(gd.x==560 && gd.y>=120 && gd.y<=200))
		{
			drawImage(scr,img.back,(int)gd.x-40,(int)gd.y-40,640,820,40,40,255);
		}
		if(phase==THROW_MEMMA){
			drawImage(scr,img.back,180+count*10,80-count*2,790,0,30,30,255);
		}
		drawImage(scr,img.back,480-(count%100),count%100,740,0,50,40,500-5*(count%100));
	}else{
		drawImage(scr,img.back,0,0,0,480,640,480,255);
		if(CHAR_CODE==JAPANESE)drawImage(scr,img.back,80,170,640,660,160,80,255);
		else drawImage(scr,img.back,80,170,640,740,160,80,255);
		drawImage(scr,img.back,360,80,640,100+((count/5)%2)*280,160,280,255);
		if(phase==SHOPPING || phase==BUYING || phase==SELLING){
			TextOut(scr,230,400,text[MIYAZAKITEXT+6]);
			int a=1;
			for(int i=0 ; i<10 ; i++){
				if((gd.hazia/a==0 && gd.hazia>0) || (gd.hazia==0 && i>0))continue;
				drawImage(scr,img.chr,1200-i*30,400,((gd.hazia/a)%10)*20,520,20,40,255);
				a*=10;
			}
		}
	}
	if(phase==STAY_IN){
		if(count<30)fillRect(scr,0,0,640,480,0,0,0,255-(30-count)*8);
		else if(count<300){
			fillRect(scr,0,0,640,480,0,0,0,255);
			if(CHAR_CODE==JAPANESE){
				if(count>100)drawImage(scr,img.back,80,120,0,1680,480,100,255);
				if(count>200)drawImage(scr,img.back,80,260,0,1780,480,60,(count-200)*6);
			}else{
				if(count>100)drawImage(scr,img.back,60,120,0,1840,520,150,255);
				if(count>200)drawImage(scr,img.back,30,300,0,1990,580,50,(count-200)*6);
			}
		}
	}
	drawHaziaShopExplain(scr);
	for(int i=0 ; i<15 ; i++)menu[i].drawMenu(scr);
	drawTalking(scr);
}

void makeShoppingList(){
	menu[SHOPPING].setMenu(0,40,40,8,36);
	menu[SHOPPING].setCombo(2);
	menu[SHOPPING].setBG(192);
	for(int i=0 ; i<18 ; i++){
		menu[SHOPPING].stack(text[MIYAZAKITEXT+34+i]);
		if(gd.bought[i]){
			menu[SHOPPING].stack(text[MIYAZAKITEXT+12]);
			menu[SHOPPING].gray[i*2+1]=true;
		}else{
			String s;
			sprintf_s(s.str[0],"%10d%s",price[i],text[MIYAZAKITEXT+6].str[0]);
			sprintf_s(s.str[1],"%10d%s",price[i],text[MIYAZAKITEXT+6].str[1]);
			menu[SHOPPING].stack(s);
		}
		menu[SHOPPING].inputMark(i*2,shop_icon[i]);
	}
}
