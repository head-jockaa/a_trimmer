#include "util.h"

Map map;
void getRPGchip();
void createMap_mount();
void createMap_town();
void createMap_shore();
void drawRPGchip(Uint32* px, Uint16 scrWidth, int x, int y, int mag, int n, int p, int bright);
void fillRect_except_yellow(Image *img, int x, int y, int w, int h, int r, int g, int b, int a);

void Map::set(){
	rgb=new Image(mapW,mapH);
	shore=new Uint16[mapW*mapH];
	road=new Uint16[mapW*mapH];
	foothill=new Uint16[mapW*mapH];
	h=new Uint16*[mapW];
	type=new Uint8[mapW*mapH];
	slope=new Uint16*[mapW];
	smr=new Uint8*[mapW];
	for(int i=0 ; i<mapW ; i++){
		h[i]=new Uint16[mapH];
		slope[i]=new Uint16[mapH];
		smr[i]=new Uint8[mapH];
	}
	for(int i=0 ; i<mapW ; i++)for(int j=0 ; j<mapH ; j++){
		rgb->RGB[j*mapW+i]=0;h[i][j]=0;
		type[j*mapW+i]=0;slope[i][j]=0;smr[i][j]=0;
	}
	getImage(rpg,"file/img/rpgchip.png",BLACK);
}

void Map::addType(int i, Uint8 t){
	if(t<16){
		type[i] = (type[i]&240) + t;
	}else{
		type[i] = t + (type[i]&15);
	}
}

void Map::setRPGchip(int bright){
	freeImage(rpg);
	getImage(rpg,"file/img/rpgchip.png",BLACK);
	int a=(1000-bright)*255/1000;
	fillRect_except_yellow(rpg,0,0,64,8,0,0,0,a);
	fillRect_except_yellow(rpg,64,0,128,16,0,0,0,a);
	fillRect_except_yellow(rpg,192,0,256,32,0,0,0,a);
}

void Map::reset(){
	if(map.rural_num) {
		for(int i=0 ; i<rural_num ; i++) {
			delete [] rural_tv[i];
		}
		delete [] rural_tv;
		for(int i=0 ; i<ruralW ; i++) {
			delete [] rural[i];
		}
		delete [] rural;
		delete [] rural_rate;
		rural_num=0;
	}
	for(int i=0 ; i<mapW; i++){
		delete [] h[i];
		delete [] slope[i];
		delete [] smr[i];
	}
	freeImage(rgb);
	delete [] shore;
	delete [] road;
	delete [] h;
	delete [] type;
	delete [] slope;
	delete [] smr;
	if(volcanoNum>0){
		delete [] volcano_id;
		delete [] volcano;
		delete [] volcanoX;
		delete [] volcanoY;
	}
	freeImage(rpg);
}

void createMap(){
	if(map_loaded)return;
	map.set();
	load_mounts();
	SDL_Surface *img, *img2=NULL;
	SDL_PixelFormat *f;
	Uint8 *px;
	Uint16 pitch;
	Uint8 bypp;
	Uint32 pixel=0;
	SDL_Color col;
	for(int i=0 ; i<=map.mapW/300 ; i++)for(int j=0 ; j<=map.mapH/300 ; j++){
		if(SDL_BYTEORDER==SDL_BIG_ENDIAN)img=SDL_CreateRGBSurface(SDL_SWSURFACE, 300, 300, 32, 0xff000000,0x00ff0000,0x0000ff00,0);
		else img=SDL_CreateRGBSurface(SDL_SWSURFACE, 300, 300, 32, 0x000000ff,0x0000ff00,0x00ff0000,0);
		sprintf_s(str,"file/img/map/map%d%d.png",j,i);
		img2=IMG_Load(str);
		if(img2){
			drawSurface(img,img2,0,0,0,0,300,300,255);
			SDL_FreeSurface(img2);
			img2=NULL;
		}
		SDL_LockSurface(img);
		f = img->format;
		px = (Uint8*)img->pixels;
		pitch = img->pitch;
		bypp = f->BytesPerPixel;
		for(int a=0 ; a<300 ; a++)for(int b=0 ; b<300 ; b++){
			if(i*300+a>=map.mapW || j*300+b>=map.mapH)continue;
			pixel = *(Uint32*)(px + pitch*b + bypp*a);
			SDL_GetRGB( pixel, f, &col.r, &col.g, &col.b);
			if(col.r==0 && col.g==0 && col.b==0){
				map.rgb->RGB[(j*300+b)*map.mapW+(i*300+a)]=0;
			}
			else if(col.r==255 && col.g==255 && col.b==255){
				map.rgb->RGB[(j*300+b)*map.mapW+(i*300+a)]=setRGB(255,255,255);
				map.addType((j*300+b)*map.mapW+(i*300+a), ROAD);
			}
			else{
				map.rgb->RGB[(j*300+b)*map.mapW+(i*300+a)]=setRGB(255,255,0);
			}
		}
		SDL_UnlockSurface(img);
		SDL_FreeSurface(img);
	}
	setAlpha(map.rgb,0,0,0);

	int a;
	for(int i=0 ; i<640 ; i++)for(int j=0 ; j<480 ; j++){
		a=255-(int)(sqrt((320.0-i)*(320-i)+(240-j)*(240-j))*3);
		if(a<0)map.c[i+j*640]=0;
		else map.c[i+j*640]=a;
	}

	createMap_mount();
	createMap_shore();
	createMap_town();
	createMap_tower();
	map_loaded=true;
}

bool isSlope(Uint8 type){
	return (type&15)==SLOPE_45 || (type&15)==SLOPE_135 || (type&15)==SLOPE_225 || (type&15)==SLOPE_315 || (type&15)==SUMMIT;
}

void createMap_shore(){
	for(int a=0 ; a<map.mapW ; a++)for(int b=0 ; b<map.mapH ; b++){
		if(map.rgb->RGB[b*map.mapW+a]==0){
			map.type[b*map.mapW+a]=SEA;
		}
	}
	for(int a=0 ; a<map.mapW ; a++)for(int b=0 ; b<map.mapH ; b++){
		map.shore[b*map.mapW+a]=0;
		map.road[b*map.mapW+a]=0;
		map.foothill[b*map.mapW+a]=0;
		if(map.type[b*map.mapW+a]==SEA){
			if(a>0 && b>0 && map.type[a-1+(b-1)*map.mapW]!=SEA)map.shore[b*map.mapW+a]++;
			if(b>0 && map.type[a+(b-1)*map.mapW]!=SEA)map.shore[b*map.mapW+a]+=2;
			if(a<map.mapW-1 && b>0 && map.type[a+1+(b-1)*map.mapW]!=SEA)map.shore[b*map.mapW+a]+=4;
			if(a>0 && map.type[a-1+b*map.mapW]!=SEA)map.shore[b*map.mapW+a]+=8;
			if(a<map.mapW-1 && map.type[a+1+b*map.mapW]!=SEA)map.shore[b*map.mapW+a]+=16;
			if(a>0 && b<map.mapH-1 && map.type[a-1+(b+1)*map.mapW]!=SEA)map.shore[b*map.mapW+a]+=32;
			if(b<map.mapH-1 && map.type[a+(b+1)*map.mapW]!=SEA)map.shore[b*map.mapW+a]+=64;
			if(a<map.mapW-1 && b<map.mapH-1 && map.type[a+1+(b+1)*map.mapW]!=SEA)map.shore[b*map.mapW+a]+=128;
		}
		else {
			if(a>0 && b>0 && map.type[a-1+b*map.mapW]==SEA && map.type[a+(b-1)*map.mapW]==SEA && map.type[a-1+(b-1)*map.mapW]!=SEA)map.shore[b*map.mapW+a]++;
			if(a<map.mapW-1 && b>0 && map.type[a+(b-1)*map.mapW]==SEA && map.type[a+1+b*map.mapW]==SEA && map.type[a+1+(b-1)*map.mapW]!=SEA)map.shore[b*map.mapW+a]+=2;
			if(a>0 && b<map.mapH-1 && map.type[a-1+b*map.mapW]==SEA && map.type[a+(b+1)*map.mapW]==SEA && map.type[a-1+(b+1)*map.mapW]!=SEA)map.shore[b*map.mapW+a]+=4;
			if(a<map.mapW-1 && b<map.mapH-1 && map.type[a+(b+1)*map.mapW]==SEA && map.type[a+1+b*map.mapW]==SEA && map.type[a+1+(b+1)*map.mapW]!=SEA)map.shore[b*map.mapW+a]+=8;
			if(map.shore[b*map.mapW+a]!=0)map.shore[b*map.mapW+a]+=256;
		}

		if(isSlope(map.type[b*map.mapW+a])){
			if(a>0 && b>0 && !isSlope(map.type[a-1+(b-1)*map.mapW]))map.foothill[b*map.mapW+a]++;
			if(b>0 && !isSlope(map.type[a+(b-1)*map.mapW]))map.foothill[b*map.mapW+a]+=2;
			if(a<map.mapW-1 && b>0 && !isSlope(map.type[a+1+(b-1)*map.mapW]))map.foothill[b*map.mapW+a]+=4;
			if(a>0 && !isSlope(map.type[a-1+b*map.mapW]))map.foothill[b*map.mapW+a]+=8;
			if(a<map.mapW-1 && !isSlope(map.type[a+1+b*map.mapW]))map.foothill[b*map.mapW+a]+=16;
			if(a>0 && b<map.mapH-1 && !isSlope(map.type[a-1+(b+1)*map.mapW]))map.foothill[b*map.mapW+a]+=32;
			if(b<map.mapH-1 && !isSlope(map.type[a+(b+1)*map.mapW]))map.foothill[b*map.mapW+a]+=64;
			if(a<map.mapW-1 && b<map.mapH-1 && !isSlope(map.type[a+1+(b+1)*map.mapW]))map.foothill[b*map.mapW+a]+=128;
		}

		if(map.type[b*map.mapW+a]&ROAD){
			if(a>0 && b>0 && !(map.type[a-1+(b-1)*map.mapW]&ROAD))map.road[b*map.mapW+a]++;
			if(b>0 && !(map.type[a+(b-1)*map.mapW]&ROAD))map.road[b*map.mapW+a]+=2;
			if(a<map.mapW-1 && b>0 && !(map.type[a+1+(b-1)*map.mapW]&ROAD))map.road[b*map.mapW+a]+=4;
			if(a>0 && !(map.type[a-1+b*map.mapW]&ROAD))map.road[b*map.mapW+a]+=8;
			if(a<map.mapW-1 && !(map.type[a+1+b*map.mapW]&ROAD))map.road[b*map.mapW+a]+=16;
			if(a>0 && b<map.mapH-1 && !(map.type[a-1+(b+1)*map.mapW]&ROAD))map.road[b*map.mapW+a]+=32;
			if(b<map.mapH-1 && !(map.type[a+(b+1)*map.mapW]&ROAD))map.road[b*map.mapW+a]+=64;
			if(a<map.mapW-1 && b<map.mapH-1 && !(map.type[a+1+(b+1)*map.mapW]&ROAD))map.road[b*map.mapW+a]+=128;
		} else {
			if(a>0 && b>0 && map.type[a-1+b*map.mapW]&ROAD && map.type[a+(b-1)*map.mapW]&ROAD && !(map.type[a-1+(b-1)*map.mapW]&ROAD))map.road[b*map.mapW+a]++;
			if(a<map.mapW-1 && b>0 && map.type[a+(b-1)*map.mapW]&ROAD && map.type[a+1+b*map.mapW]&ROAD && !(map.type[a+1+(b-1)*map.mapW]&ROAD))map.road[b*map.mapW+a]+=2;
			if(a>0 && b<map.mapH-1 && map.type[a-1+b*map.mapW]&ROAD && map.type[a+(b+1)*map.mapW]&ROAD && !(map.type[a-1+(b+1)*map.mapW]&ROAD))map.road[b*map.mapW+a]+=4;
			if(a<map.mapW-1 && b<map.mapH-1 && map.type[a+(b+1)*map.mapW]&ROAD && map.type[a+1+b*map.mapW]&ROAD && !(map.type[a+1+(b+1)*map.mapW]&ROAD))map.road[b*map.mapW+a]+=8;
			if(map.road[b*map.mapW+a]!=0)map.road[b*map.mapW+a]+=256;
		}
	}
}

void createMap_color(int bright){
	SDL_Color col;
	for(int a=0 ; a<map.mapW ; a++)for(int b=0 ; b<map.mapH ; b++){
		if(map.type[b*map.mapW+a]!=SEA){
			if(map.h[a][b]<500){
				col.r=128-map.h[a][b]*128/500;
				col.g=128;
				col.b=map.h[a][b]*128/500;
			}
			else if(map.h[a][b]<1000){
				col.r=0;
				col.g=128;
				col.b=128-(map.h[a][b]-500)*128/500;
			}
			else if(map.h[a][b]<1500){
				col.r=(map.h[a][b]-1000)*129/500;
				col.g=128-(map.h[a][b]-1000)*128/500;
				col.b=0;
			}
			else if(map.h[a][b]<2000){
				col.r=129-(map.h[a][b]-1500)*128/500;
				col.g=0;col.b=0;
			}
			else if(map.h[a][b]<2500){
				col.r=1+(map.h[a][b]-2000)*128/500;
				col.g=(map.h[a][b]-2000)*128/500;
				col.b=(map.h[a][b]-2000)*255/500;
			}
			else{
				col.r=128;
				col.g=128;
				col.b=255;
			}
			map.rgb->RGB[b*map.mapW+a]=setRGB(col.r*bright/1000,col.g*bright/1000,col.b*bright/1000);
		}
	}
	map.setRPGchip(bright);
}

void createMap_mount(){
	int X=0,Y=0;
	map.volcanoNum=0;
	Mount *mt=mount;
	for(int m=0 ; m<mounts ; m++){
		X=mt->x;Y=mt->y;
		if(mt->volcano)map.volcanoNum++;
		for(int a=1-mt->range ; a<mt->range ; a++)for(int b=1-mt->range ; b<mt->range; b++){
			if(X+a<0 || X+a>=map.mapW || Y+b<0 || Y+b>=map.mapH || sqrt(1.0*a*a+b*b)>mt->range){
				continue;
			}
			if(map.type[(Y+b)*map.mapW+(X+a)]==SEA){
				continue;
			}
			if(mt->city){
				map.addType((Y+b)*map.mapW+(X+a), CITY);
			}else{
				double sl=1.0*(mt->h-mt->slope)/mt->range;
				if( map.h[X+a][Y+b]<mt->h-(int)(sl*sqrt(1.0*a*a+b*b)) ){
					map.h[X+a][Y+b]=mt->h-(int)(sl*sqrt(1.0*a*a+b*b));
					if((map.type[(Y+b)*map.mapW+(X+a)]&15)!=CITY){
						map.slope[X+a][Y+b]=(int)sl;
						if(a==0 && b==0){
							if(mt->volcano)map.addType((Y+b)*map.mapW+(X+a), VOLCANO);
							else map.addType((Y+b)*map.mapW+(X+a), SUMMIT);
						}
					    else if(mt->h == mt->slope){
							map.addType((Y+b)*map.mapW+(X+a), NULL);
						}else{
							if(a>0 && b<=0)map.addType((Y+b)*map.mapW+(X+a), SLOPE_45);
							else if(a<=0 && b<0)map.addType((Y+b)*map.mapW+(X+a), SLOPE_135);
							else if(a<0 && b>=0)map.addType((Y+b)*map.mapW+(X+a), SLOPE_225);
							else if(a>=0 && b>0)map.addType((Y+b)*map.mapW+(X+a), SLOPE_315);
						}
					}
				}
			}
		}
		mt++;
	}
	if(map.volcanoNum>0){
		map.volcano_id=new Uint16[map.volcanoNum];
		map.volcano=new Uint16[map.volcanoNum];
		map.volcanoX=new Uint16[map.volcanoNum];
		map.volcanoY=new Uint16[map.volcanoNum];
		for(int i=0 ; i<map.volcanoNum ; i++){
			map.volcano[i]=0;map.volcanoX[i]=0;map.volcanoY[i]=0;
		}
	}
}

void createMap_town(){
	load_towns();
	Area *ar=area;
	for(int i=0 ; i<areas ; i++){
		Town *tw=ar->town;
		for(int j=0 ; j<(ar->town_num) ; j++){
			if(tw->x<0 || tw->x>=map.mapW || tw->y<0 || tw->y>=map.mapH){
				tw++;
				continue;
			}
			map.type[(tw->x)+(tw->y)*map.mapW]+=TOWN;
			tw++;
		}
		ar++;
	}
}

void createMap_tower(){
	load_towers();
	Area *ar=area;
	for(int i=0 ; i<areas ; i++){
		Tower *tw=ar->tower;
		for(int j=0 ; j<(ar->tower_num) ; j++){
			if(tw->x<0 || tw->x>=map.mapW || tw->y<0 || tw->y>=map.mapH){
				tw++;
				continue;
			}
			map.type[tw->x+tw->y*map.mapW]&=15;
			tw++;
		}
		ar++;
	}

	ar=area;
	for(int i=0 ; i<areas ; i++){
		Tower *tw=ar->tower;
		for(int j=0 ; j<(ar->tower_num) ; j++){
			if(tw->x<0 || tw->x>=map.mapW || tw->y<0 || tw->y>=map.mapH){
				tw++;
				continue;
			}
			if(tw->remove){tw++;continue;}
			if((tw->kw)>=1){
				map.type[tw->x+tw->y*map.mapW] = TOWER_L + (map.type[tw->x+tw->y*map.mapW]&15);
			}
			else if((tw->kw)>=0.1 && (map.type[tw->x+tw->y*map.mapW]&240)!=TOWER_L){
				map.type[tw->x+tw->y*map.mapW] = TOWER_M + (map.type[tw->x+tw->y*map.mapW]&15);
			}
			else if((map.type[tw->x+tw->y*map.mapW]&240)!=TOWER_L && (map.type[tw->x+tw->y*map.mapW]&240)!=TOWER_M){
				map.type[tw->x+tw->y*map.mapW] = TOWER_S + (map.type[tw->x+tw->y*map.mapW]&15);
			}
			tw++;
		}
		ar++;
	}
}

int onBS(){
	if(mode!=GAME)return 0;
	int n=0;
	for(int i=0 ; i<12 ; i++){
		if(sta[BSstation[i]].ontv!=EOF)n++;
	}
	if(n>2)n=2;
	return n;
}

void drawSea(SDL_Surface* scr, int X, int Y){
	int n=onBS();
	if(n==0){
		for(int i=-2 ; i<17 ; i++)for(int j=-1 ; j<13 ; j++){
			drawImage(scr,img.chr,i*40-X%40+((Y/40+j)%2)*20+(count/12)%40,j*40-Y%40+(count/12)%40, ((count/20)%4)*40+120,120,40,40,64);
		}
	}
	else if(n==1){
		for(int i=-2 ; i<17 ; i++)for(int j=-1 ; j<13 ; j++){
			drawImage(scr,img.chr,i*40-X%40+((Y/40+j)%2)*20+(count/12)%40,j*40-Y%40+(count/12)%40, ((count/20)%4)*40+120,160,40,40,128);
		}
	}
	else if(n==2){
		for(int i=-2 ; i<17 ; i++)for(int j=-1 ; j<13 ; j++){
			drawImage(scr,img.chr,i*40-X%40+((Y/40+j)%2)*20+(count/12)%40,j*40-Y%40+(count/12)%40, ((count/20)%4)*40+120,160,40,40,255);
		}
	}
}

void drawMap(SDL_Surface* scr, int X, int Y){
	if(X<0)X=0;
	if(Y<0)Y=0;
	if(X>map.mapW*MAGNIFY-640)X=map.mapW*MAGNIFY-640;
	if(Y>map.mapH*MAGNIFY-480)Y=map.mapH*MAGNIFY-480;

	drawSea(scr,X,Y);

	int a=abs(255-(count%255)*2);

	if(MAP3D){
		drawImage(scr,map.bufferGround,0,0,0,0,640,480,255);
		if(SHOW_TOWER)drawImage(scr,map.bufferTowerSpot,0,0,0,0,640,480,a);
	}else{
		drawImage(scr,map.bufferGround,0,0,0,0,640,480,255);

		if(SHOW_TOWER){
			if(mode==GAME && map.moved){
				drawTowerSpot(scr,X,Y,0,0,640,480,false);
				drawCityLight(X,Y,0,0,640,480);
			}
			else if(!map.bufferedTowerSpotImage){
				drawTowerSpot(scr,X,Y,0,0,640,480,true);
				drawImage(scr,map.bufferTowerSpot,0,0,0,0,640,480,a);
			}else{
				drawImage(scr,map.bufferTowerSpot,0,0,0,0,640,480,a);
			}
		}

		if(MAGNIFY>=8){
			if(gd.town_count<400){
				if(gd.town_count<30 || gd.town_count>370){
					if(gd.town_count<30)a=gd.town_count;
					else a=400-gd.town_count;
					drawImage(scr,img.chr,300-(int)(a*6.6),0,220,240,20,50,255);
					for(int i=9-a/3 ; i<11+a/3 ; i++)drawImage(scr,img.chr,120+i*20,0,240,240,20,50,255);
					drawImage(scr,img.chr,320+(int)(a*6.6),0,260,240,20,50,255);
				}else{
					drawImage(scr,img.chr,100,0,220,240,20,50,255);
					for(int i=0 ; i<20 ; i++)drawImage(scr,img.chr,120+i*20,0,240,240,20,50,255);
					drawImage(scr,img.chr,520,0,260,240,20,50,255);
					if(gd.current_area!=EOF && gd.current_town!=EOF){
						drawText2(scr,320-(int)strlen(area[gd.current_area].town[gd.current_town].name.str[CHAR_CODE])*8,8,area[gd.current_area].town[gd.current_town].name);
					}
				}
			}
		}else{
			illuminateImage_quick(scr,map.bufferCity,0,0,0,0,640,480,0,255);
		}
	}
}

void drawMap2(SDL_Surface *scr, int X, int Y){
	int a=0;
	if(count%200<80)a=(int)(6.3*(40-abs(count%200-40)));
	if(phase!=GAMESTART)illuminateImage(scr,map.bufferColorLight,map.airMinX,map.airMinY,map.airMinX,map.airMinY,map.airMaxX-map.airMinX,map.airMaxY-map.airMinY,255);
	if(a!=0 && phase!=GAMESTART && SHOW_TOWER){
		if(map.moved){
			drawVolcano(scr,X,Y,0,0,640,480,false);
		}
		else if(!map.bufferedVolcanoImage){
			drawVolcano(scr,X,Y,0,0,640,480,true);
			illuminateImage(scr,map.bufferVolcano,map.volX,map.volY,map.volX,map.volY,map.volW,map.volH,a);
		}else{
			illuminateImage(scr,map.bufferVolcano,map.volX,map.volY,map.volX,map.volY,map.volW,map.volH,a);
		}
	}
	if(gd.ta_count<87){
		illuminateImage(scr,img.tv_asahi,565,0,(gd.ta_count%10)*75,(gd.ta_count/10)*50,75,50,255);
	}
}

void fillRect_except_yellow(Image *img, int x, int y, int w, int h, int r, int g, int b, int a){
	Uint8 *px=(Uint8*)img->RGB;
	Uint16 px_skip=((img->w)-w)*4;
	SDL_Color col;
	px=px+(y*(img->w)+x)*4;
	a=255-a;

	for(int j=0 ; j<h ; j++){
		for(int i=0 ; i<w ; i++){
			getRGB(*(Uint32*)px,&col.r,&col.g,&col.b);
			if(col.r!=255 || col.g!=255 || col.b!=0){
				*(Uint32*)px=setRGB(col.r*a/255,col.g*a/255,col.b*a/255);
			}
			px+=4;
		}
		px+=px_skip;
	}
}

void drawRPGchip(Uint8* px, Uint8* alpha, Uint16 scrWidth, int x, int y, int mag, Uint8 type, int s, int r, int f, int p, int bright){
	Uint8 *rpg=(Uint8*)map.rpg->RGB;
	Uint8 *pos=(Uint8*)map.rgb->RGB;
	Uint8 *grass=(Uint8*)map.rpg->RGB;
	Uint8 *slope=(Uint8*)map.rpg->RGB;
	Uint8 *shore=(Uint8*)map.rpg->RGB;
	Uint8 *road=(Uint8*)map.rpg->RGB;
	Uint8 *foothill=(Uint8*)map.rpg->RGB;
	pos+=p*4;

	int w=mag,h=mag,x2=0,y2=0;
	int grassX=0,grassY=0,slopeX=0,slopeY=0;
	int shoreX=0,shoreY=0,roadX=0,roadY=0,foothillX=0,foothillY=0;

	int n=9,n2=4;

	switch(type&15){
		case SLOPE_45: n2=0;break;
		case SLOPE_135: n2=1;break;
		case SLOPE_225: n2=2;break;
		case SLOPE_315: n2=3;break;
		case VOLCANO: n=2;break;
		case CITY: n=6;break;
		case SEA: n=8;break;
		case SUMMIT: n=1;break;
		default: break; // FLAT LAND
	}
	switch(type&112){
		case TOWN: n=7;break;
		case TOWER_S: n=3;break;
		case TOWER_M: n=4;break;
		case TOWER_L: n=5;break;
		default: break;
	}
	if(type&ROAD && (n==1||n==6||n==9))n=0;

	if(mag==8){
		x2=(n%8)*8;
		y2=(n/8)*8;
		grassX=32;grassY=280;
		slopeX=n2*8;slopeY=280;
		shoreX=(s%8)*8;shoreY=8+(s/8)*8;
		roadX=(r%8)*8;roadY=8+(r/8)*8;
		foothillX=(f%8)*8;foothillY=8+(f/8)*8;
	}
	else if(mag==16){
		x2=64+(n%8)*16;
		y2=(n/8)*16;
		grassX=128;grassY=560;
		slopeX=64+n2*16;slopeY=560;
		shoreX=64+(s%8)*16;shoreY=16+(s/8)*16;
		roadX=64+(r%8)*16;roadY=16+(r/8)*16;
		foothillX=64+(f%8)*16;foothillY=16+(f/8)*16;
	}
	else if(mag==32){
		x2=192+(n%8)*32;
		y2=(n/8)*32;
		grassX=320;grassY=1120;
		slopeX=192+n2*32;slopeY=1120;
		shoreX=192+(s%8)*32;shoreY=32+(s/8)*32;
		roadX=192+(r%8)*32;roadY=32+(r/8)*32;
		foothillX=192+(f%8)*32;foothillY=32+(f/8)*32;
	}

	if(x<0){x2-=x;grassX-=x;slopeX-=x;shoreX-=x;roadX-=x;foothillX-=x;w+=x;x=0;}
	if(y<0){y2-=y;grassY-=y;slopeY-=y;shoreY-=y;roadY-=y;foothillY-=y;h+=y;y=0;}
	if(x+w>640)w=640-x;
	if(y+h>480)h=480-y;
	px=px+(y*scrWidth+x)*4;
	Uint16 px_skip=(scrWidth-w)*4;
	alpha=alpha+(y*scrWidth+x);
	Uint16 alpha_skip=scrWidth-w;
	rpg=rpg+(y2*(map.rpg->w)+x2)*4;
	Uint16 rpg_skip=((map.rpg->w)-w)*4;
	grass=grass+(grassY*(map.rpg->w)+grassX)*4;
	slope=slope+(slopeY*(map.rpg->w)+slopeX)*4;
	shore=shore+(shoreY*(map.rpg->w)+shoreX)*4;
	road=road+(roadY*(map.rpg->w)+roadX)*4;
	foothill=foothill+(foothillY*(map.rpg->w)+foothillX)*4;

	Uint8 dark=255-255*bright/1000;
	Uint8 dark2=dark;
	if(gd.hour>=gd.sunset_hour+2)dark2=255-255*200/1000;

	Uint8 chipR,chipG,chipB;

	// Land Color by Height
	Uint8 hightColorB=*pos;
	Uint8 hightColorG=*(pos+1);
	Uint8 hightColorR=*(pos+2);

	// Land Color by Height (slight dark)
	Uint8 hightColorR2,hightColorG2,hightColorB2;
	if(gd.hour<=gd.sunrise_hour-1 || gd.hour>=gd.sunset_hour+2){
		hightColorB2=hightColorB<6 ? 0 : hightColorB-6;
		hightColorG2=hightColorG<6 ? 0 : hightColorG-6;
		hightColorR2=hightColorR<6 ? 0 : hightColorR-6;
	}else{
		hightColorB2=hightColorB<30 ? 0 : hightColorB-30;
		hightColorG2=hightColorG<30 ? 0 : hightColorG-30;
		hightColorR2=hightColorR<30 ? 0 : hightColorR-30;
	}

	Uint8 slopeChipR,slopeChipG,slopeChipB;

	Uint8 sandR=128,sandG=128,sandB=0;
	Uint8 grassR=98,grassG=98,grassB=0;
	Uint8 roadR=192,roadG=192,roadB=0;

	for(int j=0 ; j<h ; j++){
		for(int i=0 ; i<w ; i++){
			if(n==8 && *shore==0){
				*alpha=0;
			}
			else if(s>=256 && *shore==0){
				*alpha=0;
			}
			else{
				*alpha=255;
			}
			alpha++;

			if(n<=7){
				chipB=*rpg;
				chipG=*(rpg+1);
				chipR=*(rpg+2);
			}else{
				chipB=0;chipG=0;chipR=0;
			}

			slopeChipB=*slope;
			slopeChipG=*(slope+1);
			slopeChipR=*(slope+2);

			if((n==0 || (n!=0 && r!=0)) && chipR==0 && chipG==0 && chipB==0 && *road==0){
				// Road
				*px=roadB+(roadB*(-dark2)>>8);px++;
				*px=roadG+(roadG*(-dark2)>>8);px++;
				*px=roadR+(roadR*(-dark2)>>8);px++;
				px++;rpg+=4;grass+=4;slope+=4;shore+=4;road+=4;foothill+=4;
			}
			else if(n==8){
				//SEA
				if(*shore!=0){
					if(*grass==0){
						*px=sandB+(sandB*(-dark2)>>8);px++;
						*px=sandG+(sandG*(-dark2)>>8);px++;
						*px=sandR+(sandR*(-dark2)>>8);px++;
					} else {
						*px=grassB+(grassB*(-dark2)>>8);px++;
						*px=grassG+(grassG*(-dark2)>>8);px++;
						*px=grassR+(grassR*(-dark2)>>8);px++;
					}
					px++;
				}else{
					px+=4;
				}
				rpg+=4;grass+=4;slope+=4;shore+=4;road+=4;foothill+=4;
			}
			else if(n==7 && ((chipR==255 && chipG==255 && chipB<=10) || (chipR==51 && chipG==51 && chipB<=10))){
				//Window of House
				*px=0;px++;
				*px=255;px++;
				*px=255;px++;
				px++;rpg+=4;grass+=4;slope+=4;shore+=4;road+=4;foothill+=4;
			}
			else if(n==2 && chipR==0 && chipG==0 && chipB==0){
				//Volcano
				if(*grass==0){
					*px=sandB+(sandB*(-dark2)>>8);px++;
					*px=sandG+(sandG*(-dark2)>>8);px++;
					*px=sandR+(sandR*(-dark2)>>8);px++;
				} else {
					*px=grassB+(grassB*(-dark2)>>8);px++;
					*px=grassG+(grassG*(-dark2)>>8);px++;
					*px=grassR+(grassR*(-dark2)>>8);px++;
				}
				px++;rpg+=4;grass+=4;slope+=4;shore+=4;road+=4;foothill+=4;
			}
			else if(n2>=0 && n2<=3 && chipR==0 && chipG==0 && chipB==0){
				//Slope
				if(*foothill==0){
					if(slopeChipR!=0 || slopeChipG!=0 || slopeChipB!=0){
						*px=hightColorB2+(hightColorB2*(-dark)>>8);px++;
						*px=hightColorG2+(hightColorG2*(-dark)>>8);px++;
						*px=hightColorR2+(hightColorR2*(-dark)>>8);px++;
					} else {
						*px=hightColorB+(hightColorB*(-dark)>>8);px++;
						*px=hightColorG+(hightColorG*(-dark)>>8);px++;
						*px=hightColorR+(hightColorR*(-dark)>>8);px++;
					}
				}else{
					if(*grass==0){
						*px=sandB+(sandB*(-dark2)>>8);px++;
						*px=sandG+(sandG*(-dark2)>>8);px++;
						*px=sandR+(sandR*(-dark2)>>8);px++;
					} else {
						*px=grassB+(grassB*(-dark2)>>8);px++;
						*px=grassG+(grassG*(-dark2)>>8);px++;
						*px=grassR+(grassR*(-dark2)>>8);px++;
					}
				}
				px++;rpg+=4;grass+=4;slope+=4;shore+=4;road+=4;foothill+=4;
			}
			else if(n2==4 && chipR==0 && chipG==0 && chipB==0){
				if(n==1){
					// SUMMIT
					if(*grass==0){
						*px=hightColorB+(hightColorB*(-dark)>>8);px++;
						*px=hightColorG+(hightColorG*(-dark)>>8);px++;
						*px=hightColorR+(hightColorR*(-dark)>>8);px++;
					} else {
						*px=hightColorB2+(hightColorB2*(-dark)>>8);px++;
						*px=hightColorG2+(hightColorG2*(-dark)>>8);px++;
						*px=hightColorR2+(hightColorR2*(-dark)>>8);px++;
					}
				}else{
					// Flat Land
					if(*grass==0){
						*px=sandB+(sandB*(-dark2)>>8);px++;
						*px=sandG+(sandG*(-dark2)>>8);px++;
						*px=sandR+(sandR*(-dark2)>>8);px++;
					} else {
						*px=grassB+(grassB*(-dark2)>>8);px++;
						*px=grassG+(grassG*(-dark2)>>8);px++;
						*px=grassR+(grassR*(-dark2)>>8);px++;
					}
				}
				px++;rpg+=4;grass+=4;slope+=4;shore+=4;road+=4;foothill+=4;
			}
			else{
				*px=*rpg+(*rpg*(-dark)>>8);px++;rpg++;
				*px=*rpg+(*rpg*(-dark)>>8);px++;rpg++;
				*px=*rpg+(*rpg*(-dark)>>8);px++;rpg++;
				px++;rpg++;grass+=4;slope+=4;shore+=4;road+=4;foothill+=4;
			}
		}
		px+=px_skip;
		alpha+=alpha_skip;
		rpg+=rpg_skip;
		grass+=rpg_skip;
		slope+=rpg_skip;
		shore+=rpg_skip;
		road+=rpg_skip;
		foothill+=rpg_skip;
	}
}

void drawGround(int scrX, int scrY, int x2, int y2, int w, int h, int bright){
	scrX+=MAGNIFY/2;
	scrY+=MAGNIFY/2;
	if(x2<0){w+=x2;x2=0;}
	if(y2<0){h+=y2;y2=0;}
	if(x2+w>640)w=640-x2;
	if(y2+h>480)h=480-y2;

	if(MAGNIFY>=8){
		Uint8 *px, *alpha;
		Uint16 scrWidth;
		px=(Uint8*)map.bufferGround->RGB;
		alpha=(Uint8*)map.bufferGround->A;
		scrWidth=640;
		int a,X,Y;
		for(int j=0 ; j<=h/MAGNIFY+1 ; j++)for(int i=0 ; i<=w/MAGNIFY+1 ; i++){
			a=(scrY/MAGNIFY+j)*map.mapW+(scrX/MAGNIFY+i);
			X=x2+i*MAGNIFY-scrX%MAGNIFY;
			Y=y2+j*MAGNIFY-scrY%MAGNIFY;
			drawRPGchip(px,alpha,scrWidth,X,Y,MAGNIFY,map.type[a],map.shore[a],map.road[a],map.foothill[a],a,bright);
		}
	}else{
		Uint8 *px,*alpha,*rgb,*a;
		Uint8 *rgb_start,*a_start;
		Uint16 px_skip,alpha_skip,rgb_skip,a_skip;
		Uint16 rgb_back,a_back;
		Uint8 countX=0, countY=0;
		px=(Uint8*)map.bufferGround->RGB;
		alpha=(Uint8*)map.bufferGround->A;
		px_skip=(640-w)*4;
		alpha_skip=640-w;
		px+=(y2*640+x2)*4;
		alpha+=y2*640+x2;
		rgb=(Uint8*)map.rgb->RGB;
		a=map.rgb->A;
		rgb_skip=map.mapW*4;
		a_skip=map.mapW;
		rgb_back=w/MAGNIFY*4;
		a_back=w/MAGNIFY;
		rgb+=(scrY/MAGNIFY*map.mapW+scrX/MAGNIFY)*4;
		a+=scrY/MAGNIFY*map.mapW+scrX/MAGNIFY;
		rgb_start=rgb;
		a_start=a;

		countY=scrY%MAGNIFY;
		if(bright==1000){
			for(int j=0 ; j<h ; j++){
				countX=scrX%MAGNIFY;
				for(int i=0 ; i<w ; i++){
					if(*a){
						*px=*rgb;px++;rgb++;
						*px=*rgb;px++;rgb++;
						*px=*rgb;px++;rgb++;
						px++;rgb++;
					}else{
						*px=0;px++;rgb++;
						*px=0;px++;rgb++;
						*px=0;px++;rgb++;
						px++;rgb++;
					}
					*alpha=*a;alpha++;
					countX++;
					if(countX==MAGNIFY){
						a++;
						countX=0;
					}else{
						rgb-=4;
					}
				}
				px+=px_skip;
				alpha+=alpha_skip;
				countY++;
				if(countY==MAGNIFY){
					rgb_start+=rgb_skip;a_start+=a_skip;
					countY=0;
				}
				rgb=rgb_start;a=a_start;
			}
		}else{
			Uint8 d=255-255*bright/1000;
			for(int j=0 ; j<h ; j++){
				countX=scrX%MAGNIFY;
				for(int i=0 ; i<w ; i++){
					if(*a){
						*px=*rgb+((*rgb)*(-d)>>8);px++;rgb++;
						*px=*rgb+((*rgb)*(-d)>>8);px++;rgb++;
						*px=*rgb+((*rgb)*(-d)>>8);px++;rgb++;
						px++;rgb++;
					}else{
						*px=0;px++;rgb++;
						*px=0;px++;rgb++;
						*px=0;px++;rgb++;
						px++;rgb++;
					}
					*alpha=*a;alpha++;
					countX++;
					if(countX==MAGNIFY){
						a++;
						countX=0;
					}else{
						rgb-=4;
					}
				}
				px+=px_skip;
				alpha+=alpha_skip;
				countY++;
				if(countY==MAGNIFY){
					rgb_start+=rgb_skip;a_start+=a_skip;
					countY=0;
				}
				rgb=rgb_start;a=a_start;
			}
		}
	}
}

void drawCityLight(int scrX, int scrY, int x2, int y2, int w, int h){
	scrX+=MAGNIFY/2;
	scrY+=MAGNIFY/2;
	if(x2<0){w+=x2;x2=0;}
	if(y2<0){h+=y2;y2=0;}
	if(x2+w>640)w=640-x2;
	if(y2+h>480)h=480-y2;

	Uint8* px;
	Uint16 scrWidth;
	px=(Uint8*)map.bufferCity->RGB;
	scrWidth=640;

	if(MAGNIFY<=4){
		Uint8 *px,*city,*blue;
		Uint8 *city_start,*blue_start;
		Uint16 px_skip,city_skip,blue_skip;
		Uint16 city_back;
		Uint8 countX=0, countY=0;
		px=(Uint8*)map.bufferCity->RGB;
		px_skip=(640-w)*4;
		px+=(y2*640+x2)*4;
		city=map.type;
		blue=map.c;
		city_skip=map.mapW;
		blue_skip=640-w;
		city_back=w/MAGNIFY;
		city+=scrY/MAGNIFY*map.mapW+scrX/MAGNIFY;
		blue+=y2*640+x2;
		city_start=city;
		blue_start=blue;
		countY=scrY%MAGNIFY;

		for(int j=0 ; j<h ; j++){
			countX=scrX%MAGNIFY;
			for(int i=0 ; i<w ; i++){
				if((*city&15)==CITY){
					*px=*blue;
				}else{
					*px=0;
				}
				px+=4;
				blue++;
				countX++;
				if(countX==MAGNIFY){
					city++;
					countX=0;
				}
			}
			px+=px_skip;
			blue+=blue_skip;
			countY++;
			if(countY==MAGNIFY){
				city_start+=city_skip;
				countY=0;
			}
			city=city_start;
		}
	}
}

void drawTowerCircleSpot(Uint32* px, Uint16 px_down, int x, int y, int scrX, int scrY, int scrW, int scrH, bool large, Uint32 color){
	Uint32 *_px=px+px_down*y+x;
	if(large && y-2>=scrY && y-2<scrY+scrH){
		if(x-1>=scrX && x-1<scrX+scrW)*(_px-px_down*2-1)=color;
		if(x>=scrX && x<scrX+scrW)*(_px-px_down*2)=color;
		if(x+1>=scrX && x+1<scrX+scrW)*(_px-px_down*2+1)=color;
	}
	if(y-1>=scrY && y-1<scrY+scrH){
		if(large && x-2>=scrX && x-2<scrX+scrW)*(_px-px_down-2)=color;
		if(x-1>=scrX && x-1<scrX+scrW)*(_px-px_down-1)=color;
		if(x>=scrX && x<scrX+scrW)*(_px-px_down)=color;
		if(x+1>=scrX && x+1<scrX+scrW)*(_px-px_down+1)=color;
		if(large && x+2>=scrX && x+2<scrX+scrW)*(_px-px_down+2)=color;
	}
	if(y>=scrY && y<scrY+scrH){
		if(large && x-2>=scrX && x-2<scrX+scrW)*(_px-2)=color;
		if(x-1>=scrX && x-1<scrX+scrW)*(_px-1)=color;
		if(x+1>=scrX && x+1<scrX+scrW)*(_px+1)=color;
		if(large && x+2>=scrX && x+2<scrX+scrW)*(_px+2)=color;
	}
	if(y+1>=scrY && y+1<scrY+scrH){
		if(large && x-2>=scrX && x-2<scrX+scrW)*(_px+px_down-2)=color;
		if(x-1>=scrX && x-1<scrX+scrW)*(_px+px_down-1)=color;
		if(x>=scrX && x<scrX+scrW)*(_px+px_down)=color;
		if(x+1>=scrX && x+1<scrX+scrW)*(_px+px_down+1)=color;
		if(large && x+2>=scrX && x+2<scrX+scrW)*(_px+px_down+2)=color;
	}
	if(large && y+2>=scrY && y+2<scrY+scrH){
		if(x-1>=scrX && x-1<scrX+scrW)*(_px+px_down*2-1)=color;
		if(x>=scrX && x<scrX+scrW)*(_px+px_down*2)=color;
		if(x+1>=scrX && x+1<scrX+scrW)*(_px+px_down*2+1)=color;
	}
}

void drawTowerSquareSpot(Uint32* px, Uint16 px_down, int x, int y, int scrX, int scrY, int scrW, int scrH, int size, Uint32 color){
	for(int j=y ; j<y+size ; j++){
		if(j<scrY || j>=scrY+scrH)continue;
		for(int i=x ; i<x+size ; i++){
			if(i<scrX || i>=scrX+scrW)continue;
			*(px+px_down*j+i)=color;
		}
	}
}

void drawTowerSpot(SDL_Surface *scr, int x, int y, int x2, int y2, int w, int h, bool buf){
	if(!buf)SDL_LockSurface(scr);
	else fillRect(map.bufferTowerSpot,x2,y2,w,h,0,0,0,255);
	if(!SHOW_TOWER)return;
	x+=MAGNIFY/2;
	y+=MAGNIFY/2;
	if(x<0)x=0;
	if(y<0)y=0;
	if(x2<0)x2=0;
	if(y2<0)y2=0;
	if(x2+w>640)w=640-x2;
	if(y2+h>480)h=480-y2;
	int shiftX=x%MAGNIFY,shiftY=y%MAGNIFY;

	Uint32* px;
	if(buf)px=map.bufferGround->RGB;
	else px=(Uint32*)scr->pixels;
	Uint32* px2=map.bufferTowerSpot->RGB;

	Uint8* px_tp=map.type+(y/MAGNIFY)*map.mapW+(x/MAGNIFY);
	Uint16 tp_skip=map.mapW-(w/MAGNIFY);
	if(x>0){px_tp--;tp_skip-=2;}
	if(y>0){px_tp-=map.mapW;}

	Uint16 px_down;
	if(buf){
		px_down=640;
	}else{
		px_down=(scr->pitch)/4;
	}

	if(buf){
		if(MAGNIFY==1){
			for(int j=-1 ; j<h+1 ; j++){
				for(int i=-1 ; i<w+1 ; i++){
					if((*px_tp&240)==TOWER_L){
						drawTowerCircleSpot(px,px_down,x2+i,y2+j,x2,y2,w,h,false,setRGB(255,255,0));
						drawTowerCircleSpot(px2,px_down,x2+i,y2+j,x2,y2,w,h,false,setRGB(255,255,255));
					}
					else if((*px_tp&240)==TOWER_M){
						drawTowerSquareSpot(px,px_down,x2+i,y2+j,x2,y2,w,h,2,setRGB(255,0,0));
						drawTowerSquareSpot(px2,px_down,x2+i,y2+j,x2,y2,w,h,2,setRGB(255,255,255));
					}
					else if((*px_tp&240)==TOWER_S){
						drawTowerSquareSpot(px,px_down,x2+i,y2+j,x2,y2,w,h,1,setRGB(0,0,255));
						drawTowerSquareSpot(px2,px_down,x2+i,y2+j,x2,y2,w,h,1,setRGB(255,255,255));
					}
					px_tp++;
				}
				px_tp+=tp_skip;
			}
		}
		else if(MAGNIFY==2 || MAGNIFY==4){
			for(int j=-MAGNIFY-shiftY ; j<h+MAGNIFY-shiftY ; j+=MAGNIFY){
				for(int i=-MAGNIFY-shiftX ; i<w+MAGNIFY-shiftX ; i+=MAGNIFY){
					if((*px_tp&240)==TOWER_L){
						drawTowerCircleSpot(px,px_down,x2+i,y2+j,x2,y2,w,h,true,setRGB(255,255,0));
						drawTowerCircleSpot(px2,px_down,x2+i,y2+j,x2,y2,w,h,true,setRGB(255,255,255));
					}
					else if((*px_tp&240)==TOWER_M){
						drawTowerSquareSpot(px,px_down,x2+i,y2+j,x2,y2,w,h,3,setRGB(255,0,0));
						drawTowerSquareSpot(px2,px_down,x2+i,y2+j,x2,y2,w,h,3,setRGB(255,255,255));
					}
					else if((*px_tp&240)==TOWER_S){
						drawTowerSquareSpot(px,px_down,x2+i,y2+j,x2,y2,w,h,2,setRGB(0,0,255));
						drawTowerSquareSpot(px2,px_down,x2+i,y2+j,x2,y2,w,h,2,setRGB(255,255,255));
					}
					px_tp++;
				}
				px_tp+=tp_skip;
			}
		}
		else if(MAGNIFY==8){
			for(int j=-8-shiftY ; j<h+8-shiftY ; j+=8){
				for(int i=-8-shiftX ; i<w+8-shiftX ; i+=8){
					if((*px_tp&240)==TOWER_L){
						drawTowerSquareSpot(px,px_down,x2+i+3,y2+j,x2,y2,w,h,2,setRGB(255,255,0));
						drawTowerSquareSpot(px2,px_down,x2+i+3,y2+j,x2,y2,w,h,2,setRGB(255,255,255));
					}
					else if((*px_tp&240)==TOWER_M){
						drawTowerSquareSpot(px,px_down,x2+i+3,y2+j,x2,y2,w,h,1,setRGB(255,0,0));
						drawTowerSquareSpot(px2,px_down,x2+i+3,y2+j,x2,y2,w,h,1,setRGB(255,255,255));
					}
					else if((*px_tp&240)==TOWER_S){
						drawTowerSquareSpot(px,px_down,x2+i+3,y2+j,x2,y2,w,h,1,setRGB(0,0,255));
						drawTowerSquareSpot(px2,px_down,x2+i+3,y2+j,x2,y2,w,h,1,setRGB(255,255,255));
					}
					px_tp++;
				}
				px_tp+=tp_skip;
			}
		}
		else if(MAGNIFY==16){
			for(int j=-16-shiftY ; j<h+16-shiftY ; j+=16){
				for(int i=-16-shiftX ; i<w+16-shiftX ; i+=16){
					if((*px_tp&240)==TOWER_L){
						drawTowerSquareSpot(px,px_down,x2+i+6,y2+j,x2,y2,w,h,4,setRGB(255,255,0));
						drawTowerSquareSpot(px2,px_down,x2+i+6,y2+j,x2,y2,w,h,4,setRGB(255,255,255));
					}
					else if((*px_tp&240)==TOWER_M){
						drawTowerSquareSpot(px,px_down,x2+i+6,y2+j,x2,y2,w,h,2,setRGB(255,0,0));
						drawTowerSquareSpot(px2,px_down,x2+i+6,y2+j,x2,y2,w,h,2,setRGB(255,255,255));
					}
					else if((*px_tp&240)==TOWER_S){
						drawTowerSquareSpot(px,px_down,x2+i+6,y2+j,x2,y2,w,h,2,setRGB(0,0,255));
						drawTowerSquareSpot(px2,px_down,x2+i+6,y2+j,x2,y2,w,h,2,setRGB(255,255,255));
					}
					px_tp++;
				}
				px_tp+=tp_skip;
			}
		}
		else if(MAGNIFY==32){
			for(int j=-32-shiftY ; j<h+32-shiftY ; j+=32){
				for(int i=-32-shiftX ; i<w+32-shiftX ; i+=32){
					if((*px_tp&240)==TOWER_L){
						drawTowerSquareSpot(px,px_down,x2+i+12,y2+j,x2,y2,w,h,8,setRGB(255,255,0));
						drawTowerSquareSpot(px2,px_down,x2+i+12,y2+j,x2,y2,w,h,8,setRGB(255,255,255));
					}
					else if((*px_tp&240)==TOWER_M){
						drawTowerSquareSpot(px,px_down,x2+i+12,y2+j,x2,y2,w,h,4,setRGB(255,0,0));
						drawTowerSquareSpot(px2,px_down,x2+i+12,y2+j,x2,y2,w,h,4,setRGB(255,255,255));
					}
					else if((*px_tp&240)==TOWER_S){
						drawTowerSquareSpot(px,px_down,x2+i+12,y2+j,x2,y2,w,h,4,setRGB(0,0,255));
						drawTowerSquareSpot(px2,px_down,x2+i+12,y2+j,x2,y2,w,h,4,setRGB(255,255,255));
					}
					px_tp++;
				}
				px_tp+=tp_skip;
			}
		}
	}else{
		Uint8 col=abs(255-(count%255)*2);
		if(MAGNIFY==1){
			for(int j=-1 ; j<h+1 ; j++){
				for(int i=-1 ; i<w+1 ; i++){
					if((*px_tp&240)==TOWER_L){
						drawTowerCircleSpot(px,px_down,x2+i,y2+j,x2,y2,w,h,false,setRGB(255,255,col));
					}
					else if((*px_tp&240)==TOWER_M){
						drawTowerSquareSpot(px,px_down,x2+i,y2+j,x2,y2,w,h,2,setRGB(255,col,col));
					}
					else if((*px_tp&240)==TOWER_S){
						drawTowerSquareSpot(px,px_down,x2+i,y2+j,x2,y2,w,h,1,setRGB(col,col,255));
					}
					px_tp++;
				}
				px_tp+=tp_skip;
			}
		}
		else if(MAGNIFY==2 || MAGNIFY==4){
			for(int j=-MAGNIFY-shiftY ; j<h+MAGNIFY-shiftY ; j+=MAGNIFY){
				for(int i=-MAGNIFY-shiftX ; i<w+MAGNIFY-shiftX ; i+=MAGNIFY){
					if((*px_tp&240)==TOWER_L){
						drawTowerCircleSpot(px,px_down,x2+i,y2+j,x2,y2,w,h,true,setRGB(255,255,col));
					}
					else if((*px_tp&240)==TOWER_M){
						drawTowerSquareSpot(px,px_down,x2+i,y2+j,x2,y2,w,h,3,setRGB(255,col,col));
					}
					else if((*px_tp&240)==TOWER_S){
						drawTowerSquareSpot(px,px_down,x2+i,y2+j,x2,y2,w,h,2,setRGB(col,col,255));
					}
					px_tp++;
				}
				px_tp+=tp_skip;
			}
		}
		else if(MAGNIFY==8){
			for(int j=-8-shiftY ; j<h+8-shiftY ; j+=8){
				for(int i=-8-shiftX ; i<w+8-shiftX ; i+=8){
					if((*px_tp&240)==TOWER_L){
						drawTowerSquareSpot(px,px_down,x2+i+3,y2+j,x2,y2,w,h,2,setRGB(255,255,col));
					}
					else if((*px_tp&240)==TOWER_M){
						drawTowerSquareSpot(px,px_down,x2+i+3,y2+j,x2,y2,w,h,1,setRGB(255,col,col));
					}
					else if((*px_tp&240)==TOWER_S){
						drawTowerSquareSpot(px,px_down,x2+i+3,y2+j,x2,y2,w,h,1,setRGB(col,col,255));
					}
					px_tp++;
				}
				px_tp+=tp_skip;
			}
		}
		else if(MAGNIFY==16){
			for(int j=-16-shiftY ; j<h+16-shiftY ; j+=16){
				for(int i=-16-shiftX ; i<w+16-shiftX ; i+=16){
					if((*px_tp&240)==TOWER_L){
						drawTowerSquareSpot(px,px_down,x2+i+6,y2+j,x2,y2,w,h,4,setRGB(255,255,col));
					}
					else if((*px_tp&240)==TOWER_M){
						drawTowerSquareSpot(px,px_down,x2+i+6,y2+j,x2,y2,w,h,2,setRGB(255,col,col));
					}
					else if((*px_tp&240)==TOWER_S){
						drawTowerSquareSpot(px,px_down,x2+i+6,y2+j,x2,y2,w,h,2,setRGB(col,col,255));
					}
					px_tp++;
				}
				px_tp+=tp_skip;
			}
		}
		else if(MAGNIFY==32){
			for(int j=-32-shiftY ; j<h+32-shiftY ; j+=32){
				for(int i=-32-shiftX ; i<w+32-shiftX ; i+=32){
					if((*px_tp&240)==TOWER_L){
						drawTowerSquareSpot(px,px_down,x2+i+12,y2+j,x2,y2,w,h,8,setRGB(255,255,col));
					}
					else if((*px_tp&240)==TOWER_M){
						drawTowerSquareSpot(px,px_down,x2+i+12,y2+j,x2,y2,w,h,4,setRGB(255,col,col));
					}
					else if((*px_tp&240)==TOWER_S){
						drawTowerSquareSpot(px,px_down,x2+i+12,y2+j,x2,y2,w,h,4,setRGB(col,col,255));
					}
					px_tp++;
				}
				px_tp+=tp_skip;
			}
		}
	}

	if(buf){
		setAlpha(map.bufferTowerSpot,0,0,0);
		map.bufferedTowerSpotImage=true;
	}else{
		SDL_UnlockSurface(scr);
	}
}

void drawFireWork(SDL_Surface *scr){
	if(gd.firework>0){
		int v_max=0;
		for(int i=0 ; i<map.volcanoNum ; i++)if(map.volcano[i]!=0){
			int mX=map.volcanoX[i], mY=map.volcanoY[i], a=266-map.volcano[i]/8;
			if(map.volcano[i]>1600)drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX-20,mY*MAGNIFY-gd.scrY-80,200+((count/4)%2)*40,480,40,80,255);
			else if(map.volcano[i]>1200)drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX-20,mY*MAGNIFY-gd.scrY-80,280+((count/4)%2)*40,480,40,80,255);
			else if(map.volcano[i]>800)drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX-20,mY*MAGNIFY-gd.scrY-80,360+((count/4)%2)*40,480,40,80,255);
			else drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX-20,mY*MAGNIFY-gd.scrY-40,440+((count/4)%2)*40,520,40,40,255);
			if(map.volcano[i]<=800 && map.volcano[i]>400 && count%100<8)drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX-20,mY*MAGNIFY-gd.scrY-60,440+((count/4)%2)*40,480,40,40,255);
			int x1=a-80,y1=a*3/2,x2=a*3/2-80,y2=a;
			int b=a,c=a*3/2,d=128;
			if(x1<0)x1=0;if(x2<0)x2=0;
			if(b>80)b=80;if(c>80)c=80;
			if(map.volcano[i]<128)d=map.volcano[i];
			drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX+x1,mY*MAGNIFY-gd.scrY-y1,600-b,480,b,c,d);
			drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX+x2,mY*MAGNIFY-gd.scrY-y2,600-c,480,c,b,d);
			if(v_max<map.volcano[i])v_max=map.volcano[i];
			map.volcano[i]--;
			if(map.volcano[i]==0){
				map.volcanoX[i]=0;
				map.volcanoY[i]=0;
				gd.firework--;
			}
		}
		if(v_max==2000)Mix_PlayChannel(3,sf.fw_sound,-1);
		else if(v_max==1200)Mix_PlayChannel(3,sf.fw_sound,0);
	}
}

void drawColorLight(int scrX, int scrY, int x, int y, int w, int h){
	fillRect(map.bufferColorLight,x,y,w,h,0,0,0,255);
	if(AIR_IMG==0)return;
	int X,Y,X2,Y2,W,H;

	Area *ar=area;
	for(int i=0 ; i<areas ; i++){
		Tower *tw=ar->tower;
		for(int j=0 ; j<(ar->tower_num) ; j++){
			if(tw->remove){tw++;continue;}
			int onair_num=tw->onair_num, size=tw->colorlight_size;
			if(onair_num==0 || size<0 || size>=8){tw++;continue;}
			if(MAGNIFY>=2)size*=2;
			X=(tw->x)*MAGNIFY+x-scrX-size*5;
			Y=(tw->y)*MAGNIFY+y-scrY-size*5;
			X2=0;Y2=0;W=size*10+10;H=size*10+10;

			if(X>=x+w || Y>=y+h){tw++;continue;}
			if(X+W<x|| Y+H<y){tw++;continue;}
			if(X<x){X2+=x-X;W-=x-X;X=x;}
			if(Y<y){Y2+=y-Y;H-=y-Y;Y=y;}
			if(X+W>x+w)W=x+w-X;
			if(Y+H>y+h)H=y+h-Y;

			if(onair_num==1)illuminateImage_quick(map.bufferColorLight,img.colorlight,X,Y,X2+size*160,Y2,W,H,0,255);
			else if(onair_num==2)illuminateImage_quick(map.bufferColorLight,img.colorlight,X,Y,X2+size*160,Y2,W,H,1,255);
			else if(onair_num==3)illuminateImage_quick(map.bufferColorLight,img.colorlight,X,Y,X2+size*160,Y2,W,H,2,255);

			if(map.airMinX>X-1)map.airMinX=X-1;
			if(map.airMaxX<X+W+1)map.airMaxX=X+W+1;
			if(map.airMinY>Y-1)map.airMinY=Y-1;
			if(map.airMaxY<Y+H+1)map.airMaxY=Y+H+1;
			tw++;
		}
		ar++;
	}

	if(map.airMinX<0)map.airMinX=0;
	if(map.airMaxX>640)map.airMaxX=640;
	if(map.airMinY<0)map.airMinY=0;
	if(map.airMaxY>480)map.airMaxY=480;

	if(AIR_IMG==TYPE1_EDGE || AIR_IMG==TYPE2_EDGE){
		X=x-1;Y=y-1;W=w+2;H=h+2;
		if(X<0)X=0;
		if(Y<0)Y=0;
		if(X+W>640)W=640-X;
		if(Y+H>480)H=480-Y;
		Uint32* px=map.bufferColorLight->RGB;
		Uint32 white=setRGB(255,255,255);
		px=px+(Y*640+X);
		Uint16 px_skip=640-W;

		for(int j=0 ; j<H ; j++){
			for(int i=0 ; i<W ; i++){
				if(!(*px)){
					if( i!=0 && *(px-1) && *(px-1)!=white ){
						*px=white;
					}
					else if( i!=W-1 && *(px+1) && *(px+1)!=white ){
						*px=white;
					}
					else if( j!=0 && *(px-640) && *(px-640)!=white ){
						*px=white;
					}
					else if( j!=H-1 && *(px+640) && *(px+640)!=white ){
						*px=white;
					}
				}
				px++;
			}
			px+=px_skip;
		}
	}
}

void drawVolcano(SDL_Surface *scr, int scrX, int scrY, int x, int y, int w, int h, bool buf){
	if(buf){
		fillRect(map.bufferVolcano,0,0,640,480,0,0,0,255);
	}
	if(AIR_IMG==0 || mode==GAMEMENU || mode==MIYAZAKI)return;
	int minX=640,maxX=0,minY=480,maxY=0;
	int X,Y,X2,Y2,W,H;

	int a=0;
	if(count%200<80)a=(int)(6.3*(40-abs(count%200-40)));

	for(int i=0 ; i<mounts ; i++)if(mount[i].volcano){
		X=mount[i].x*MAGNIFY-scrX-5;
		Y=mount[i].y*MAGNIFY-scrY-5;
		X2=0;Y2=0;W=10;H=10;

		if(X>=x+w || Y>=y+h)continue;
		if(X+W<x|| Y+H<y)continue;
		if(X<x){X2+=x-X;W-=x-X;X=x;}
		if(Y<y){Y2+=y-Y;H-=y-Y;Y=y;}
		if(X+W>x+w)W=x+w-X;
		if(Y+H>y+h)H=y+h-Y;

		if(minX>X)minX=X;
		if(maxX<X+W)maxX=X+W;
		if(minY>Y)minY=Y;
		if(maxY<Y+H)maxY=Y+H;

		if(buf)illuminateImage_quick(map.bufferVolcano,img.colorlight,X,Y,X2,Y2+160,W,H,1,255);
		else illuminateImage_quick(scr,img.colorlight,X,Y,X2,Y2+160,W,H,1,a);
	}

	map.volX=minX;
	map.volW=maxX-minX;
	map.volY=minY;
	map.volH=maxY-minY;
	if(buf){
		map.bufferedVolcanoImage=true;
	}
}

SDL_Color getSkyColor(int h, int m){
	SDL_Color col;
	col.r=0;col.g=0;col.b=0;
	if(h<=gd.sunrise_hour-2){
		col.r=0;
		col.g=0;
		col.b=0;
	}
	else if(h==gd.sunrise_hour-1){
		col.r=0;
		col.g=0;
		col.b=m*128/60;
	}
	else if(h==gd.sunrise_hour){
		col.r=m*192/60;
		col.g=m*128/60;
		col.b=128;
	}
	else if(h<=gd.sunrise_hour+2){
		col.r=192;
		col.g=128+((h-gd.sunrise_hour-1)*60+m)*64/120;
		col.b=128+((h-gd.sunrise_hour-1)*60+m)*64/120;
	}
	else if(h<=11){
		col.r=192;
		col.g=192;
		col.b=192+((h-gd.sunrise_hour-3)*60+m)*64/((9-gd.sunrise_hour)*60);
	}
	else if(h<=gd.sunset_hour-3){
		col.r=192-((h-12)*60+m)*64/((gd.sunset_hour-14)*60);
		col.g=192-((h-12)*60+m)*64/((gd.sunset_hour-14)*60);
		col.b=255;
	}
	else if(h==gd.sunset_hour-2){
		col.r=128+m*127/60;
		col.g=128;
		col.b=255-m*255/60;
	}
	else if(h==gd.sunset_hour-1){
		if(m<30){
			col.r=255;
			col.g=128-m*64/30;
			col.b=m*128/30;
		}else{
			col.r=255-(m-30)*191/30;
			col.g=64;
			col.b=128+(m-30)*127/30;
		}
	}
	else if(h==gd.sunset_hour){
		col.r=64-m*64/60;
		col.g=64-m*64/60;
		col.b=255-m*255/60;
	}
	else if(h<=27){
		col.r=0;
		col.g=0;
		col.b=1;
	}
	return col;
}

void refreshGroundImage(){
	int blight=1000;
	if(mode==GAME){
		if(gd.hour==gd.sunset_hour+1)blight=(120000-gd.minute*1600)/120;
		else if(gd.hour==gd.sunrise_hour)blight=(24000+gd.minute*1600)/120;
	}
	drawGround(gd.scrX,gd.scrY,0,0,640,480,blight);
	drawCityLight(gd.scrX,gd.scrY,0,0,640,480);
	if(AIR_IMG!=0){
		map.airMinX=640;
		map.airMaxX=0;
		map.airMinY=480;
		map.airMaxY=0;
		drawColorLight(gd.scrX,gd.scrY,0,0,640,480);
	}
	map.bufferedTowerSpotImage=false;
	map.bufferedVolcanoImage=false;
}

void make3dview(double X, double Y, int D){
	resetZ();
	int cubeW,cubeH,half;
	int Z=gd.height;
	if(Z<map.h[(int)X][(int)Y])Z=map.h[(int)X][(int)Y];
	double xd,zd,dis;
	double span=8.0*sqrt(1.0*Z);
	int mg=(int)(span*2/3);
	span/=2;

	double aa=1;
	if(mode==GAME){
		if(gd.hour<gd.sunrise_hour || gd.hour>=gd.sunset_hour+2)aa=0.2;
		if(gd.hour==gd.sunset_hour+1)aa=(100.0-gd.minute*1.33)/100;
		else if(gd.hour==gd.sunrise_hour)aa=(20.0+gd.minute*1.33)/100;
	}

	for(int i=(int)X-mg ; i<(int)X+mg ; i++)for(int j=(int)Y-mg ; j<(int)Y+mg ; j++){
		cubeW=0;
		if(i<0 || i>=map.mapW || j<0 || j>=map.mapH)continue;
		dis=sqrt(1.0*(i-X)*(i-X)+1.0*(Y-j)*(Y-j));
		if(dis<2 || dis>=mg)continue;
		double mound=(mg/8.0)*(mg/8.0)*sqrt( 1-(std::abs(dis-span)/span) );
		xd=asin_q(dis,i-X,Y-j);
		if(xd>180 && D<90)xd-=360;
		if(xd<180 && D>270)xd+=360;
		xd=D-xd;// xd‚Í•ûŠp
		if(std::abs(xd)>gd.zoom*4)continue; //Ž‹ŠE‚ÌŠO
		zd=asin_q(dis*MAP_SCALE*1000,0,map.h[i][j]-Z+mound);
		if(zd>180)zd-=360; //zd‚Í‹ÂŠp

		Uint16 *px_z=map.z, px_skip;
		Uint32 *px_rgb=map.bufferGround->RGB;
		Uint32 col;
		Uint8 R,G,B;
		bool flat=false,road=false;

		if(map.h[i][j]==0){
			if(map.type[j*map.mapW+i]==SEA){
				col=setRGB(0,0,0);
			}
			else col=setRGB((int)(128*aa),(int)(128*aa),0);
			flat=true;
		}
		else if(map.type[j*map.mapW+i]==SEA){
			col=setRGB(0,0,0);
			flat=true;road=true;
		}
		else if(map.type[j*map.mapW+i]&ROAD){
			col=setRGB((int)(128*aa),(int)(128*aa),0);
			flat=true;road=true;
		}else{
			getRGB(map.rgb->RGB[j*map.mapW+i],&R,&G,&B);
			col=setRGB( (int)(R*aa),(int)(G*aa),(int)(B*aa) );
		}

		cubeW=(int)(240.0*asin_q(dis,0,1)/gd.zoom);
		if(cubeW==0)cubeW=1;
		cubeH=(int)(cubeW*1.5);
		half=cubeW/2;
		int cubeX=320+(int)(80.0*xd/gd.zoom)-half;
		int cubeY=240-(int)(80.0*zd/gd.zoom);
		flat=true;
//		if(road||flat)cubeY+=half;

		if(cubeX<0){cubeW+=cubeX;cubeX=0;}
		if(cubeY<0){cubeH+=cubeY;cubeY=0;}
		if(cubeX+cubeW>640)cubeW=640-cubeX;
		if(cubeY+cubeH>480)cubeH=480-cubeY;
		px_z+=cubeY*640+cubeX;
		px_rgb+=cubeY*640+cubeX;
		px_skip=640-cubeW;

		for(int a=0 ; a<cubeH ; a++){
			for(int b=0 ; b<cubeW ; b++){
				if(*px_z>(Uint16)dis){
					if(flat || a>half || (b>half-(int)(half*sin((PI/2)*a/half)) && b<half+(int)(half*sin((PI/2)*a/half)))){
						*px_rgb=col;
						*px_z=(Uint16)dis;
					}
				}
				px_rgb++;px_z++;
			}
			px_rgb+=px_skip;px_z+=px_skip;
		}
	}

	make3dview_tower(X,Y,D);
	make3dview_sky();
	setAlpha(map.bufferGround,0,0,0);
}

void make3dview_tower(double X, double Y, int D){
	fillRect(map.bufferTowerSpot,0,0,640,480,0,0,0,255);
	double xd,zd,zd2,dis;
	int Z=gd.height;
	if(Z<map.h[(int)X][(int)Y])Z=map.h[(int)X][(int)Y];
	double span=8.0*sqrt(1.0*Z);
	int mg=(int)(span*2/3);

	Area *ar=area;
	for(int i=0 ; i<areas ; i++){
		Tower *tw=ar->tower;
		for(int j=0 ; j<(ar->tower_num) ; j++){
			tw->x_3d=400;
			dis=sqrt(1.0*(tw->x-X)*(tw->x-X)+1.0*(tw->y-Y)*(tw->y-Y));
			if(dis>=mg){tw++;continue;}
			double mound=(mg/8.0)*(mg/8.0)*sqrt( 1-(std::abs(dis-span)/span) );
			xd=atan_q(tw->x-X,Y-tw->y);
			zd=atan_q(dis*MAP_SCALE*1000,tw->h+map.h[tw->x][tw->y]-Z+mound);
			zd2=atan_q(dis*MAP_SCALE*1000,map.h[tw->x][tw->y]-Z+mound);
			if(xd>180 && D<90)xd-=360;
			if(xd<180 && D>270)xd+=360;
			xd=D-xd;
			if(zd>180)zd-=360;
			if(zd2>180)zd2-=360;
			int m=0;
			if(tw->kw>10)m=2;
			else if(tw->kw>1)m=1;
			int A=320+(int)(80.0*xd/gd.zoom), B=240-(int)(80.0*zd/gd.zoom);
			tw->x_3d=A;
			tw->y_3d=B;

			if(A<0 || A>=640 || B<0 || B>=480){tw++;continue;}

			for(int a=0 ; a<=(int)std::abs(zd-zd2)*10 ; a++){
				if(B+a<0 || B+a>=480)continue;
				if(m>=2 && A-1>=0){
					if(map.z[(B+a)*640+(A-1)]>(Uint16)dis){
						map.bufferGround->RGB[(B+a)*640+A-1]=setRGB(255,255,0);
						map.bufferTowerSpot->RGB[(B+a)*640+A-1]=setRGB(255,255,255);
						map.z[(B+a)*640+(A-1)]=(Uint16)dis;
					}
				}
				if(m>=1 && A+1<640){
					if(map.z[(B+a)*640+(A+1)]>(Uint16)dis){
						if(m>=2)map.bufferGround->RGB[(B+a)*640+A+1]=setRGB(255,255,0);
						else if(m==1)map.bufferGround->RGB[(B+a)*640+A+1]=setRGB(255,0,0);
						map.bufferTowerSpot->RGB[(B+a)*640+A+1]=setRGB(255,255,255);
						map.z[(B+a)*640+(A+1)]=(Uint16)dis;
					}
				}
				if(map.z[(B+a)*640+A]>(Uint16)dis){
					if(m>=2)map.bufferGround->RGB[(B+a)*640+A]=setRGB(255,255,0);
					else if(m==1)map.bufferGround->RGB[(B+a)*640+A]=setRGB(255,0,0);
					else map.bufferGround->RGB[(B+a)*640+A]=setRGB(0,0,255);
					map.bufferTowerSpot->RGB[(B+a)*640+A]=setRGB(255,255,255);
					map.z[(B+a)*640+A]=(Uint16)dis;
				}
			}
			tw++;
		}
		ar++;
	}
	setAlpha(map.bufferTowerSpot,0,0,0);
}

void make3dview_sky(){
	SDL_Color sky=getSkyColor(gd.hour,gd.minute);
	Uint32 col=setRGB(sky.r,sky.g,sky.b);

	Uint16 *px_z=map.z;
	Uint32 *px_rgb=map.bufferGround->RGB;
	for(int i=0 ; i<640*480 ; i++){
		if(*px_z==BEYOND){
			*px_rgb=col;
		}
		px_z++;px_rgb++;
	}
}

void resetZ(){
	Uint16 *p=map.z;
	for(int i=0 ; i<640*480 ; i++){
		*p=BEYOND;
		p++;
	}
}
