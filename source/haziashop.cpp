#include "util.h"

int price[SHOP_ITEMS], shop_icon[SHOP_ITEMS];

void makeShoppingList();

void initHaziaShop(){
	gd.x=80;
	gd.y=340;
	mode=HAZIASHOP;
	phase=SHOP_FLOOR;
	menu[YNFORM].setMenu(320,200,12,3,3);
	menu[YNFORM].setBG(255);
	menu[YNFORM].stack("");
	menu[YNFORM].stack("");
	menu[YNFORM].stack("");
	menu[SHOP_CONFIRM].setMenu(320,200,18,3,3);
	menu[SHOP_CONFIRM].setBG(255);
	menu[SHOP_CONFIRM].stack(text[MIYAZAKITEXT+7]);
	menu[SHOP_CONFIRM].stack(text[MIYAZAKITEXT+13]);
	menu[SHOP_CONFIRM].stack(text[MIYAZAKITEXT+8]);
	menu[OHANA_MENU].setMenu(320,200,10,2,2);
	menu[OHANA_MENU].stack(text[MIYAZAKITEXT+31]);
	menu[OHANA_MENU].stack(text[EPILOGUE+10]);
	getImage(img.back,"file/img/haziashop.png",0,0,255);
	sf.coin=Mix_LoadWAV("file/se/17.wav");
	loadCartoon(&cartoonJson, "file/data/cartoon/haziashop.json");
	readCartoon(&cartoonJson,0);
	readCartoon(&cartoonJson,1);
}

void endHaziaShop(){
	kick_count++;
	freeImage(img.back);
	freeSound(sf.coin);
	freeMusic();
	freeCartoon(&cartoonJson);
	for(int i=0 ; i<15 ; i++)menu[i].reset();
}

void keyShopFloor(){
	if(key.z && !key_stop(key.z)){
		if(gd.y==140 && gd.x>120 && gd.x<240){
			phase=SHOP_TALK;
			readCartoon(&cartoonJson,2);
		}
		if(gd.y==140 && gd.x>300 && gd.x<400){
			phase=SHOP_FLOOR_TALK;
			readCartoon(&cartoonJson,8);
		}
		if(gd.y>240 && gd.y<340 && gd.x>400 && gd.x<500){
			phase=SHOP_FLOOR_TALK;
			readCartoon(&cartoonJson,9);
		}
		if(gd.y==40 && gd.x>=400){
			phase=SHOP_FLOOR_TALK;
			readCartoon(&cartoonJson,10);
			count=0;
		}
		if(gd.x==560 && gd.y>=120 && gd.y<=200){
			phase=OHANA_TALK;
			readCartoon(&cartoonJson,11);
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
		backToMiyazaki();
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
	}
	if(key.x && !key_stop(key.x)){
		bool ok=true;
		for(int i=0 ; i<SHOP_ITEMS ; i++)if(!gd.bought[i]){
			ok=false;break;
		}
		menu[YNFORM].setViewMode(HIDE);
		menu[SHOPPING].setViewMode(HIDE);
		phase=SHOP_LASTTALK;
		if(ok){
			readCartoon(&cartoonJson,7);
		}else{
			readCartoon(&cartoonJson,6);
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
			readCartoon(&cartoonJson,33+menu[SHOPPING].selected());
		}
		if(menu[YNFORM].selected()==1){
			String s;
			sprintf_s(s.str[0],"%10d%s",price[menu[SHOPPING].selected()],text[MIYAZAKITEXT+6].str[0]);
			sprintf_s(s.str[1],"%10d%s",price[menu[SHOPPING].selected()],text[MIYAZAKITEXT+6].str[1]);
			menu[SHOPPING].input(menu[SHOPPING].selected()*2+1,s);
			menu[SHOPPING].gray[menu[SHOPPING].selected()*2+1]=false;
			gd.bought[menu[SHOPPING].selected()]=false;
			gd.hazia2=price[menu[SHOPPING].selected()];
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
			phase=SHOPPING;
		}
	}
	if(key.x && !key_stop(key.x)){
		menu[YNFORM].setViewMode(HIDE);
		phase=SHOPPING;
	}
	if(key.up && !key_wait(key.up))menu[YNFORM].cursorUp();
	if(key.down && !key_wait(key.down))menu[YNFORM].cursorDown();
}

void keyBuying(){
	if(gd.hazia2>0)return;
	if(key.z && !key_stop(key.z)){
		if(menu[YNFORM].selected()==0){
			readCartoon(&cartoonJson,15+menu[SHOPPING].selected());
		}
		if(menu[YNFORM].selected()==1){
			if(gd.hazia>price[menu[SHOPPING].selected()]){
				if(menu[SHOPPING].selected()>=13){
					readCartoon(&cartoonJson,3);
					phase=TRYING_TO_PAY_MUCH;
					menu[YNFORM].setViewMode(GRAY);
				}else{
					menu[SHOPPING].input(menu[SHOPPING].selected()*2+1,text[MIYAZAKITEXT+12]);
					menu[SHOPPING].gray[menu[SHOPPING].selected()*2+1]=true;
					gd.bought[menu[SHOPPING].selected()]=true;
					gd.hazia2=price[menu[SHOPPING].selected()];
					save_index();
				}
			}
		}
		if(menu[YNFORM].selected()==2){
			menu[YNFORM].setViewMode(HIDE);
			phase=SHOPPING;
		}
	}
	if(key.x && !key_stop(key.x)){
		menu[YNFORM].setViewMode(HIDE);
		phase=SHOPPING;
	}
	if(key.up && !key_wait(key.up))menu[YNFORM].cursorUp();
	if(key.down && !key_wait(key.down))menu[YNFORM].cursorDown();
}

void keyHaziaShopTalk(){
	if(key.z && !key_stop(key.z)){
		if(nextTalk(&cartoonJson)){
			if(phase==SHOP_TALK){
				makeShoppingList();
				menu[SHOPPING].setViewMode(VISIBLE);
				phase=SHOPPING;
			}
			else if(phase==TRYING_TO_PAY_MUCH){
				menu[SHOP_CONFIRM].setViewMode(VISIBLE);
				menu[YNFORM].setViewMode(HIDE);
				phase=SHOP_CONFIRM;
			}
			else if(phase==MERELY_YES){
				menu[SHOP_CONFIRM].setViewMode(HIDE);
				phase=SHOPPING;
			}
			else if(phase==NOD_AND_BUY){
				phase=BUYING;
				menu[SHOP_CONFIRM].setViewMode(HIDE);
				menu[SHOPPING].input(menu[SHOPPING].selected()*2+1,text[MIYAZAKITEXT+12]);
				menu[SHOPPING].gray[menu[SHOPPING].selected()*2+1]=true;
				gd.bought[menu[SHOPPING].selected()]=true;
				gd.hazia2=price[menu[SHOPPING].selected()];
				save_index();
			}
			else if(phase==OHANA_TALK){
					menu[OHANA_MENU].setViewMode(VISIBLE);
					phase=OHANA_MENU;
			}
			else{
				readCartoon(&cartoonJson,1);
				phase=SHOP_FLOOR;
			}
		}
	}
}

void keyShopConfirm(){
	if(key.z && !key_wait(key.z)){
		if(menu[SHOP_CONFIRM].selected()==0){
			readCartoon(&cartoonJson,4);
			phase=MERELY_YES;
			menu[SHOP_CONFIRM].setViewMode(GRAY);
		}
		else if(menu[SHOP_CONFIRM].selected()==1){
			readCartoon(&cartoonJson,5);
			phase=NOD_AND_BUY;
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
			readCartoon(&cartoonJson,13);
		}
		else if(menu[OHANA_MENU].selected()==1){
			readCartoon(&cartoonJson,14);
		}
		menu[OHANA_MENU].setViewMode(HIDE);
		phase=SHOP_FLOOR_TALK;
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
		case SHOP_FLOOR_TALK:
		case OHANA_TALK:
		case SHOP_TALK:
		case SHOP_LASTTALK:
		case TRYING_TO_PAY_MUCH:
		case MERELY_YES:
		case NOD_AND_BUY:
			keyHaziaShopTalk();break;
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
	if(nextCut(&cartoonJson)){
		if(phase==SHOP_FLOOR_TALK){
			readCartoon(&cartoonJson,1);
			phase=SHOP_FLOOR;
		}
	}
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
					readCartoon(&cartoonJson,33+menu[SHOPPING].selected());
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
}

void drawHaziaShopExplain(SDL_Surface* scr){
	if(EXPLAIN){
		if(phase==SHOP_FLOOR){
			drawKeyboard(scr,key.upC,400,440);
			drawKeyboard(scr,key.downC,420,440);
			drawKeyboard(scr,key.leftC,440,440);
			drawKeyboard(scr,key.rightC,460,440);
			drawText(scr,480,440,text[GAMETEXT+12]);
			if((gd.y==140 && gd.x>120 && gd.x<240) || (gd.y==40 && gd.x>=400)){
				drawKeyboard(scr,key.zC,20,60);
				drawText(scr,40,60,text[EPILOGUE+10]);
			}
			if((gd.y==140 && gd.x>300 && gd.x<400)||(gd.y>240 && gd.y<340 && gd.x>400 && gd.x<500)||(gd.x==560 && gd.y>=120 && gd.y<=200)){
				drawKeyboard(scr,key.zC,20,0);
				drawText(scr,40,440,text[MIYAZAKITEXT+30]);
			}
		}
		else if(phase==SHOP_TALK || phase==SHOP_LASTTALK || phase==SHOP_FLOOR_TALK || phase==TRYING_TO_PAY_MUCH || phase==MERELY_YES || phase==NOD_AND_BUY || phase==OHANA_TALK){
			drawKeyboard(scr,key.zC,0,0);
			drawText(scr,20,0,text[EPILOGUE+1]);
		}else{
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

void drawPlayerInHaziaShop(SDL_Surface* scr){
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
}

void drawHaziasInYourPocket(SDL_Surface* scr){
	if(phase==SHOPPING || phase==BUYING || phase==SELLING){
		drawText2(scr,230,400,text[MIYAZAKITEXT+6]);
		int a=1;
		for(int i=0 ; i<10 ; i++){
			if((gd.hazia/a==0 && gd.hazia>0) || (gd.hazia==0 && i>0))continue;
			drawImage(scr,img.chr,600-i*30,400,((gd.hazia/a)%10)*20,520,20,40,255);
			a*=10;
		}
	}
}

void drawHaziaShop(SDL_Surface* scr){
	drawAnimationCutBeforeDivision(&cartoonJson,scr);
	drawPlayerInHaziaShop(scr);
	drawAnimationCutAfterDivision(&cartoonJson,scr);
	drawHaziasInYourPocket(scr);
	for(int i=0 ; i<15 ; i++)menu[i].drawMenu(scr);
	drawHaziaShopExplain(scr);
}

void makeShoppingList(){
	menu[SHOPPING].setMenu(0,40,40,8,36);
	menu[SHOPPING].setCombo(2);
	menu[SHOPPING].setBG(128);
	for(int i=0 ; i<SHOP_ITEMS ; i++){
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
