#include "util.h"

void checkEndian();
void createAlphaKey();
void drawZoom(SDL_Surface* scr);
void initAll();
void endAll();
bool pause;
SDL_Window* window;
SDL_Renderer *render;

void draw(SDL_Surface* scr){
	setKick(scr);
	if(mode==OPENING)drawOpening(scr);
	else if(mode==TITLE)drawTitle(scr);
	else if(mode==GAMEMENU)drawGameMenu(scr);
	else if(mode==GAME)drawGame(scr);
	else if(mode==ENDING)drawEnding(scr);
	else if(mode==OPTION)drawOption(scr);
	else if(mode==MIYAZAKI)drawMiyazaki(scr);
	else if(mode==HAZIASHOP)drawHaziaShop(scr);
	if(CHOSEON){
		drawImage(scr,img.chr,30,30,460,20,44,34,255);
	}else{
		if(gd.ta_count==0 || gd.ta_count==87)drawImage(scr,img.chr,560,10,456,0,70,16,64);
	}
	drawKick(scr);
	if(scr_design!=NULL)changeScreenColor(scr);
//	if(mode==GAME){
//		sprintf_s(str,"%.12g",test);
//		sprintf_s(str,"%10d",(Uint32)test);
//		TextOut(scr,0,0,str);
//	}
//	TextOut(scr,0,0,SDL_VideoDriverName(str,1000));
}

void timer(){
	if(kick_count!=0)return;
	if(mode==OPENING)timerOpening();
	else if(mode==TITLE)timerTitle();
	else if(mode==GAMEMENU)timerGameMenu();
	else if(mode==GAME)timerGame();
	else if(mode==OPTION)timerOption();
	else if(mode==ENDING)timerEnding();
	else if(mode==MIYAZAKI)timerMiyazaki();
	else if(mode==HAZIASHOP)timerHaziaShop();
	if(start>0)start--;
	if(start<0)start++;
	count++;bg_count++;
	for(int i=0 ; i<20 ; i++)menu[i].animation();
}

void pushKey(){
	if(mode==OPENING)keyOpening();
	else if(mode==TITLE)keyTitle();
	else if(mode==GAMEMENU)keyGameMenu();
	else if(mode==GAME)keyGame();
	else if(mode==ENDING)keyEnding();
	else if(mode==OPTION)keyOption();
	else if(mode==MIYAZAKI)keyMiyazaki();
	else if(mode==HAZIASHOP)keyHaziaShop();
}
void end(){
	if(mode==OPENING)endOpening();
	else if(mode==TITLE)endTitle();
	else if(mode==GAMEMENU)endGameMenu();
	else if(mode==GAME)endGame();
	else if(mode==ENDING)endEnding();
	else if(mode==OPTION)endOption();
	else if(mode==MIYAZAKI)endMiyazaki();
	else if(mode==HAZIASHOP)endHaziaShop();
}

void key_counter(){
	if(key.up>0 && key.up<60)key.up++;
	if(key.down>0 && key.down<60)key.down++;
	if(key.left>0 && key.left<60)key.left++;
	if(key.right>0 && key.right<60)key.right++;
	if(key.z>0 && key.z<60)key.z++;
	if(key.x>0 && key.x<60)key.x++;
	if(key.c>0 && key.c<60)key.c++;
	if(key.a>0 && key.a<60)key.a++;
	if(key.F1>0 && key.F1<60)key.F1++;
	if(key.F4>0 && key.F4<60)key.F4++;
	if(key.F10>0 && key.F10<60)key.F10++;
	if(key.count>0 && key.count<60)key.count++;
}

void pauseMovie(){
	if(mode==OPENING&&phase==2 || mode==ENDING&&(phase==ENDING_ANIME||phase==LAST_ENDING)){
		pausetime=SDL_GetTicks();
		Mix_HaltMusic();
//		Mix_PauseMusic();
	}
}
void resumeMovie(){
	if(mode==OPENING&&phase==2 || mode==ENDING&&(phase==ENDING_ANIME||phase==LAST_ENDING)){
		int n=SDL_GetTicks();
		timestamp+=n-pausetime;
//		Mix_ResumeMusic();
		Mix_PlayMusic(bgm,-1);
		if(mode==ENDING && phase==LAST_ENDING)Mix_SetMusicPosition((n-timestamp-833)/1000.0);
		else Mix_SetMusicPosition((n-timestamp)/1000.0);
	}
}

int main(int argc, char *argv[]) {
	key.up=0;key.down=0;key.left=0;key.right=0;
	key.z=0;key.x=0;key.c=0;key.a=0;
	key.F1=0;key.F4=0;key.F10=0;
	int time=SDL_GetTicks();
	double delay=16;
	pause=false;
	scr.w=640;scr.h=480;
	run=true;

//	putenv("SDL_VIDEODRIVER=windib");	
//	putenv("SDL_AUDIODRIVER=waveout");
    if( SDL_Init(SDL_INIT_VIDEO|SDL_INIT_AUDIO) < 0 )exit(0);
	if( Mix_OpenAudio(22050, AUDIO_S16SYS, 2, 1024) < 0 )exit(0);
	Mix_AllocateChannels(4);
	img.iwa=IMG_Load("file/img/crag.png");
	window = SDL_CreateWindow(text[0].str[0], SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
	img.screen = SDL_GetWindowSurface(window);
	if ( img.screen == NULL )exit(0);
	load_option();
	Mix_VolumeMusic(BGM_VOLUME);
	for(int i=0 ; i<4 ; i++)Mix_Volume(i,SE_VOLUME);
	createAlphaKey();
	initFont();
	initAll();
	SDL_Surface *icon;
	icon = IMG_Load("crag.ico");
	SDL_SetWindowIcon(window, icon);

	while(run){
		SDL_Delay((int)delay);
		delay*=16.0/(SDL_GetTicks()-time);
		time=SDL_GetTicks();
		if(delay<1)delay=16;
		if(!pause){
			timer();
			pushKey();
		}
		key_counter();
		SDL_PollEvent(&ev);
		switch(ev.type){
			case SDL_KEYDOWN:
				key.pressed = ev.key.keysym.sym;
//				test=ev.key.keysym.scancode;
				if(key.count==0)key.count=1;
				if(key.pressed==key.upC && key.up==0)key.up=1;
				if(key.pressed==key.downC && key.down==0)key.down=1;
				if(key.pressed==key.leftC && key.left==0)key.left=1;
				if(key.pressed==key.rightC && key.right==0)key.right=1;
				if(key.pressed==key.zC && key.z==0)key.z=1;
				if(key.pressed==key.xC && key.x==0)key.x=1;
				if(key.pressed==key.cC && key.c==0)key.c=1;
				if(key.pressed==key.aC && key.a==0)key.a=1;
				if(key.pressed==SDLK_F1 && key.F1==0)key.F1=1;
				if(key.pressed==SDLK_F4 && key.F4==0)key.F4=1;
				if(key.pressed==SDLK_F10 && key.F10==0)key.F10=1;
				if(key.pressed==SDLK_ESCAPE)run=false;
				if(key.pressed==SDLK_a && !key_stop(key.a)){
					if(!(mode==OPTION&&phase==OPTION_KEY_SETTING)){
						if(CHAR_CODE==JAPANESE)CHAR_CODE=EUROPEAN;
						else CHAR_CODE=JAPANESE;
						gd.text_count=0;
					}
				}
				if(key.pressed==SDLK_F1 && !key_stop(key.F1)){
					if(pause){
						pause=false;
						resumeMovie();
					}else{
						pause=true;
						pauseMovie();
					}
				}
				if(key.pressed==SDLK_F4 && !key_stop(key.F4)){
					if(FULLSCR){
						window = SDL_CreateWindow(text[0].str[0], SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, 0);
						FULLSCR=false;
					}else{
						window = SDL_CreateWindow(text[0].str[0], SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 640, 480, SDL_WINDOW_FULLSCREEN);
						FULLSCR=true;
					}
					img.screen = SDL_GetWindowSurface(window);
				}
				if(key.pressed==SDLK_F10 && !key_stop(key.F10)){
					if(gd.bought[17]){
						if(CHOSEON)CHOSEON=false;
						else CHOSEON=true;
					}
				}
				if(key.pressed==SDLK_F11)reset_key();
				if(key.pressed==SDLK_F12){
					movie_test=false;
					end();
					initOpening();
				}
				break;
			case SDL_KEYUP:
				key.count=0;
				if(ev.key.keysym.sym==key.upC)key.up=0;
				if(ev.key.keysym.sym==key.downC)key.down=0;
				if(ev.key.keysym.sym==key.leftC)key.left=0;
				if(ev.key.keysym.sym==key.rightC)key.right=0;
				if(ev.key.keysym.sym==key.zC)key.z=0;
				if(ev.key.keysym.sym==key.xC)key.x=0;
				if(ev.key.keysym.sym==key.cC)key.c=0;
				if(ev.key.keysym.sym==key.aC)key.a=0;
				if(ev.key.keysym.sym==SDLK_F1)key.F1=0;
				if(ev.key.keysym.sym==SDLK_F4)key.F4=0;
				if(ev.key.keysym.sym==SDLK_F10)key.F10=0;
				break;
			case SDL_QUIT:
				run=false;
				break;
			default: break;
		}
		if(!run)break;

		draw(img.screen);
		SDL_UpdateWindowSurface(window);
	}

	if(tm.running){
		tm.halt = THREAD_END;
	}
	while(tm.running){}

	end();
	endAll();
	endFont();
	SDL_FreeSurface(img.iwa);
	SDL_FreeSurface(img.screen);
	Mix_CloseAudio();
	SDL_Quit();
	return 1;
}

void initAll(){
	img.pre_scr=new Image(640,480);
	img.scr2=new Image(640,480);
	for(int i=0 ; i<4 ; i++)img.buffer[i]=new Image(640,480);
	checkEndian();
	sf.decide=Mix_LoadWAV("file/se/3.wav");
	sf.decide2=Mix_LoadWAV("file/se/2.wav");
	sf.cursor_move=Mix_LoadWAV("file/se/1.wav");
	getImage(img.chr, "file/img/chr.png", 0,0,255);
	getImage(img.keyboard,"file/img/keyboard.gif",0,0,255);
	getImage(img.menuback,"file/img/menuback.png",0,0,0);
	getImage(img.facechip,"file/img/face.png",0,0,255);
	getSymbolImage();
	gd.timeslot=NULL;
	map.rural_loaded=false;
	fishbox.initAll();
	load_parameter();
	load_text();
	load_index();
	initOpening();
	gd.option_selected=0;
	gd.antenna_selected=0;
	gd.menu_selected=0;
	gd.mainmenu_selected=0;
}

void endAll(){
	for(int i=0 ; i<4 ; i++)freeImage(img.buffer[i]);
	if(fsize)delete [] fstr;
	if(index_num)delete [] indexName;
	if(stas)delete [] sta;
	if(allworks)delete [] animebook;
	if(works){delete [] prg;delete [] work;delete [] gd.timeslot;}
	if(towers){delete [] area;delete [] tower;}
	if(mounts)delete [] mount;
	if(towns)delete [] town;
	if(CHANNELS)delete [] mhz;
	fishbox.endAll();
	if(map_loaded)map.reset();
	for(int i=0 ; i<20 ; i++)menu[i].reset();
	freeImage(img.scr2);
	freeImage(img.chr);
	freeImage(img.symbol);
	freeImage(img.keyboard);
	freeImage(img.pre_scr);
	freeImage(img.menuback);
	freeImage(img.facechip);
	Mix_FreeChunk(sf.decide);
	Mix_FreeChunk(sf.decide2);
	Mix_FreeChunk(sf.cursor_move);
}

void checkEndian(){
	fillRect(img.screen,0,0,1,1,1,0,0,255);
	SDL_PixelFormat *f = img.screen->format;
	Uint8 bpp = f->BytesPerPixel;
	Uint8 *px = (Uint8*)img.screen->pixels;
	Uint16 pitch = img.screen->pitch;
	Uint8 bypp = f->BytesPerPixel;

	SDL_LockSurface(img.screen);
	if(*(Uint32*)px%256==1)ABGR=true;
	else ABGR=false;
	SDL_UnlockSurface(img.screen);
}

void createAlphaKey(){
	SDL_Surface *img1=NULL, *img2;
	img1=IMG_Load("file/img/alphakey.png");
	if(img1==NULL){
		return;
	}

	if(SDL_BYTEORDER==SDL_BIG_ENDIAN)img2=SDL_CreateRGBSurface(SDL_SWSURFACE, 1, 1, 32, 0xff000000,0x00ff0000,0x0000ff00,0);
	else img2=SDL_CreateRGBSurface(SDL_SWSURFACE, 1, 1, 32, 0x000000ff,0x0000ff00,0x00ff0000,0);
	drawSurface(img2,img1,0,0,0,0,1,1,255);
	SDL_FreeSurface(img1);

	SDL_LockSurface(img2);
	SDL_PixelFormat *f = img2->format;
	Uint32 *px = (Uint32*)img2->pixels;
	SDL_GetRGB(*px,f,&img.alphaR,&img.alphaG,&img.alphaB);
	SDL_UnlockSurface(img2);
	SDL_FreeSurface(img2);
}
