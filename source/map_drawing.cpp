#include "util.h"

Map map;
void getRPGchip();
void createMap_mount();
void createMap_town();
void createMap_shore();
void drawRPGchip(Uint32* px, Uint16 scrWidth, int x, int y, int mag, int n, int p, int bright);
void fillRect_except_yellow(Image *img, int x, int y, int w, int h, int r, int g, int b, int a);

void Map::set(){
	RGB=new Uint32[mapW*mapH];
	shore=new Uint8[mapW*mapH];
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
		RGB[j*mapW+i]=0;h[i][j]=0;
		type[j*mapW+i]=0;slope[i][j]=0;smr[i][j]=0;
	}
	getImage(rpg,"file/img/rpgchip.png",0,0,0);
}

void Map::set2(){
	rural=new Uint8*[ruralW];
	for(int i=0 ; i<ruralW ; i++)rural[i]=new Uint8[ruralH];
	for(int i=0 ; i<ruralW ; i++)for(int j=0 ; j<ruralH ; j++)rural[i][j]=0;
	rural_rate=new Uint16[areas];
	rural_loaded=TRUE;
}

void Map::setRPGchip(int bright){
	freeImage(rpg);
	getImage(rpg,"file/img/rpgchip.png",0,0,0);
	int a=(1000-bright)*255/1000;
	fillRect_except_yellow(rpg,0,0,64,8,0,0,0,a);
	fillRect_except_yellow(rpg,64,0,128,16,0,0,0,a);
	fillRect_except_yellow(rpg,192,0,256,32,0,0,0,a);
}

void Map::reset(){
	for(int i=0 ; i<ruralW ; i++)delete [] rural[i];
	for(int i=0 ; i<mapW; i++){
		delete [] h[i];
		delete [] slope[i];
		delete [] smr[i];
	}
	delete [] rural;
	delete [] rural_rate;
	delete [] RGB;
	delete [] shore;
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
	rural_loaded=FALSE;
}

void createMap(){
	if(map_loaded)return;
	map.set();
	load_mounts();
	SDL_Surface *img, *img2;
	SDL_PixelFormat *f;
	Uint8 *px;
	Uint16 pitch;
	Uint8 bypp;
	Uint32 pixel=0;
	SDL_Color col;
	for(int i=0 ; i<=map.mapW/300 ; i++)for(int j=0 ; j<=map.mapH/300 ; j++){
		if(SDL_BYTEORDER==SDL_BIG_ENDIAN)img=SDL_CreateRGBSurface(SDL_SWSURFACE, 300, 300, 32, 0xff000000,0x00ff0000,0x0000ff00,0);
		else img=SDL_CreateRGBSurface(SDL_SWSURFACE, 300, 300, 32, 0x000000ff,0x0000ff00,0x00ff0000,0);
		sprintf_s(str,"file/img/map/map%d%d.gif",j,i);
		img2=IMG_Load(str);
		drawSurface(img,img2,0,0,0,0,300,300,255);
		SDL_FreeSurface(img2);
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
				map.RGB[(j*300+b)*map.mapW+(i*300+a)]=0;
			}
			else if(col.r==255 && col.g==255 && col.b==255){
				map.RGB[(j*300+b)*map.mapW+(i*300+a)]=setRGB(255,255,255);
				map.type[(j*300+b)*map.mapW+(i*300+a)]=ROAD;
			}
			else{
				map.RGB[(j*300+b)*map.mapW+(i*300+a)]=setRGB(255,255,0);
			}
		}
		SDL_UnlockSurface(img);
		SDL_FreeSurface(img);
	}

	int a;
	for(int i=0 ; i<320 ; i++)for(int j=0 ; j<240 ; j++){
		a=255-(int)(sqrt((160.0-i)*(160-i)+(120-j)*(120-j))*3);
		if(a<0)map.c[i+j*320]=0;
		else map.c[i+j*320]=a;
	}

	createMap_shore();
	createMap_mount();
	createMap_town();
	createMap_tower();
	map_loaded=TRUE;
}

void createMap_shore(){
	for(int a=0 ; a<map.mapW ; a++)for(int b=0 ; b<map.mapH ; b++){
		if(map.RGB[b*map.mapW+a]==0){
			map.type[b*map.mapW+a]=SEA;
		}
	}
	for(int a=0 ; a<map.mapW ; a++)for(int b=0 ; b<map.mapH ; b++){
		if(map.type[b*map.mapW+a]==SEA){
			map.shore[b*map.mapW+a]=0;
			if(a>0 && b>0 && map.type[a-1+(b-1)*map.mapW]!=SEA && map.type[a-1+(b-1)*map.mapW]!=SHORE)map.shore[b*map.mapW+a]++;
			if(b>0 && map.type[a+(b-1)*map.mapW]!=SEA && map.type[a+(b-1)*map.mapW]!=SHORE)map.shore[b*map.mapW+a]+=2;
			if(a<map.mapW-1 && b>0 && map.type[a+1+(b-1)*map.mapW]!=SEA && map.type[a+1+(b-1)*map.mapW]!=SHORE)map.shore[b*map.mapW+a]+=4;
			if(a>0 && map.type[a-1+b*map.mapW]!=SEA && map.type[a-1+b*map.mapW]!=SHORE)map.shore[b*map.mapW+a]+=8;
			if(a<map.mapW-1 && map.type[a+1+b*map.mapW]!=SEA && map.type[a+1+b*map.mapW]!=SHORE)map.shore[b*map.mapW+a]+=16;
			if(a>0 && b<map.mapH-1 && map.type[a-1+(b+1)*map.mapW]!=SEA && map.type[a-1+(b+1)*map.mapW]!=SHORE)map.shore[b*map.mapW+a]+=32;
			if(b<map.mapH-1 && map.type[a+(b+1)*map.mapW]!=SEA && map.type[a+(b+1)*map.mapW]!=SHORE)map.shore[b*map.mapW+a]+=64;
			if(a<map.mapW-1 && b<map.mapH-1 && map.type[a+1+(b+1)*map.mapW]!=SEA && map.type[a+1+(b+1)*map.mapW]!=SHORE)map.shore[b*map.mapW+a]+=128;
			if(map.shore[b*map.mapW+a]!=0)map.type[b*map.mapW+a]=SHORE;
		}
	}
}

void createMap_color(int bright){
	SDL_Color col;
	for(int a=0 ; a<map.mapW ; a++)for(int b=0 ; b<map.mapH ; b++){
		if(map.type[b*map.mapW+a]!=SEA && map.type[b*map.mapW+a]!=SHORE){
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
			map.RGB[b*map.mapW+a]=setRGB(col.r*bright/1000,col.g*bright/1000,col.b*bright/1000);
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
		for(int a=-mt->range ; a<mt->range ; a++)for(int b=-mt->range ; b<mt->range; b++){
			if(X+a<0 || X+a>=map.mapW || Y+b<0 || Y+b>=map.mapH || sqrt(1.0*a*a+b*b)>mt->range){
				continue;
			}
			if(map.type[(Y+b)*map.mapW+(X+a)]==SEA || map.type[(Y+b)*map.mapW+(X+a)]==SHORE){
				continue;
			}
			if(mt->city){
				map.type[(Y+b)*map.mapW+(X+a)]=CITY;
			}else{
				double sl=1.0*(mt->h-mt->slope)/mt->range;
				if( map.h[X+a][Y+b]<mt->h-(int)(sl*sqrt(1.0*a*a+b*b)) ){
					map.h[X+a][Y+b]=mt->h-(int)(sl*sqrt(1.0*a*a+b*b));
					if(map.type[(Y+b)*map.mapW+(X+a)]!=ROAD && map.type[(Y+b)*map.mapW+(X+a)]!=CITY){
						map.slope[X+a][Y+b]=(int)sl;
						if(a==0 && b==0){
							if(mt->volcano)map.type[(Y+b)*map.mapW+(X+a)]=VOLCANO;
							else map.type[(Y+b)*map.mapW+(X+a)]=SUMMIT;
						}
						else if((int)sl<10){
							map.type[(Y+b)*map.mapW+(X+a)]=NULL;
						}else{
							if(a>0 && b<=0)map.type[(Y+b)*map.mapW+(X+a)]=SLOPE_45;
							else if(a<=0 && b<0)map.type[(Y+b)*map.mapW+(X+a)]=SLOPE_135;
							else if(a<0 && b>=0)map.type[(Y+b)*map.mapW+(X+a)]=SLOPE_225;
							else if(a>=0 && b>0)map.type[(Y+b)*map.mapW+(X+a)]=SLOPE_315;
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
	Town *tw=town;
	for(int i=0 ; i<towns ; i++){
		if(tw->x<0 || tw->x>=map.mapW || tw->y<0 || tw->y>=map.mapH){
			tw++;
			continue;
		}
		map.type[tw->x+tw->y*map.mapW]=TOWN;
		tw++;
	}
}

void createMap_tower(){
	load_towers();
	Tower *tw=tower;
	for(int i=0 ; i<towers ; i++){
		if(tw->x<0 || tw->x>=map.mapW || tw->y<0 || tw->y>=map.mapH){
			tw++;
			continue;
		}
		map.type[tw->x+tw->y*map.mapW]=NULL;
		tw++;
	}

	tw=tower;
	for(int i=0 ; i<towers ; i++){
		if(tw->x<0 || tw->x>=map.mapW || tw->y<0 || tw->y>=map.mapH){
			tw++;
			continue;
		}
		if(tw->remove){tw++;continue;}
		int m=1;
		if(tw->kw>0)m++;
		if(tw->kw>10)m++;
		m-=tw->kw2;
		if(m>=2)map.type[tw->x+tw->y*map.mapW]=TOWER_L;
		else if(m==1 && map.type[tw->x+tw->y*map.mapW]!=TOWER_L)map.type[tw->x+tw->y*map.mapW]=TOWER_M;
		else if(m<=0 && map.type[tw->x+tw->y*map.mapW]!=TOWER_L && map.type[tw->x+tw->y*map.mapW]!=TOWER_M)map.type[tw->x+tw->y*map.mapW]=TOWER_S;
		tw++;
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

void drawMap(SDL_Surface* scr, int X, int Y){
	if(X<0)X=0;
	if(Y<0)Y=0;
	if(X>map.mapW*MAGNIFY-320)X=map.mapW*MAGNIFY-320;
	if(Y>map.mapH*MAGNIFY-240)Y=map.mapH*MAGNIFY-240;
	int n=onBS();
	if(n==0){
		for(int i=-2 ; i<17 ; i++)for(int j=-1 ; j<13 ; j++){
			drawImage(scr,img.chr,i*20-X%20+((Y/20+j)%2)*10+(count/12)%20,j*20-Y%20+(count/12)%20, ((count/20)%4)*20+60,60,20,20,64);
		}
	}
	else if(n==1){
		for(int i=-2 ; i<17 ; i++)for(int j=-1 ; j<13 ; j++){
			drawImage(scr,img.chr,i*20-X%20+((Y/20+j)%2)*10+(count/12)%20,j*20-Y%20+(count/12)%20, ((count/20)%4)*20+60,80,20,20,128);
		}
	}
	else if(n==2){
		for(int i=-2 ; i<17 ; i++)for(int j=-1 ; j<13 ; j++){
			drawImage(scr,img.chr,i*20-X%20+((Y/20+j)%2)*10+(count/12)%20,j*20-Y%20+(count/12)%20, ((count/20)%4)*20+60,80,20,20,255);
		}
	}

	int a=abs(255-(count%255)*2);
	int b=1000;
	if(mode==GAME){
		if(gd.hour==22)b=(120000-gd.minute*1600)/120;
		else if(gd.hour==6)b=(24000+gd.minute*1600)/120;
	}

	if(MAP3D){
		drawImage(scr,img.buffer[0],0,0,0,0,320,240,255);
		if(SHOW_TOWER)drawImage(scr,img.buffer[1],0,0,0,0,320,240,a);
	}else{
	if(map.moved){
		drawGround(scr,X,Y,0,0,320,240,b,FALSE);
		drawTowerSpot(scr,X,Y,0,0,320,240,FALSE);
		drawVolcano(X,Y,320,240,FALSE);
	}else{
		if(!map.buffered){
			drawGround(scr,X,Y,0,0,320,240,b,TRUE);
			drawTowerSpot(scr,X,Y,0,0,320,240,TRUE);
			drawVolcano(X,Y,320,240,TRUE);
		}
		drawImage(scr,img.buffer[0],0,0,0,0,320,240,255);
		if(SHOW_TOWER)drawImage(scr,img.buffer[1],0,0,0,0,320,240,a);
	}

	if(MAGNIFY>=8){
		if(gd.town_count<400){
			if(gd.town_count<30 || gd.town_count>370){
				if(gd.town_count<30)a=gd.town_count;
				else a=400-gd.town_count;
				drawImage(scr,img.chr,150-(int)(a*3.3),0,110,120,10,25,255);
				for(int i=9-a/3 ; i<11+a/3 ; i++)drawImage(scr,img.chr,60+i*10,0,120,120,10,25,255);
				drawImage(scr,img.chr,160+(int)(a*3.3),0,130,120,10,25,255);
			}else{
				drawImage(scr,img.chr,50,0,110,120,10,25,255);
				for(int i=0 ; i<20 ; i++)drawImage(scr,img.chr,60+i*10,0,120,120,10,25,255);
				drawImage(scr,img.chr,260,0,130,120,10,25,255);
				if(gd.location!=EOF)TextOut(scr,160-(int)strlen(town[gd.location].name.str[CHAR_CODE])*4,4,town[gd.location].name);
			}
		}
	}
	}
}

void drawMap2(SDL_Surface *scr, int X, int Y){
	if(AIR_IMG==NULL)return;
	if(map.moved && gd.ta_count>=87)drawColorLight(X,Y,320,240,FALSE);
	else if(!map.buffered2)drawColorLight(X,Y,320,240,TRUE);
	int a=0;
	if(count%200<80)a=(int)(6.3*(40-abs(count%200-40)));
	if(phase!=GAMESTART)drawLight(scr,img.buffer[2],map.airX,map.airY,map.airX,map.airY,map.airW,map.airH,255);
	if(a!=0 && phase!=GAMESTART)drawLight(scr,img.buffer[3],map.volX,map.volY,map.volX,map.volY,map.volW,map.volH,a);
	if(gd.ta_count<87){
		drawLight(scr,img.tv_asahi,245,0,(gd.ta_count%10)*75,(gd.ta_count/10)*50,75,50,255);
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

void drawRPGchip(Uint32* px, Uint16 scrWidth, int x, int y, int mag, int n, int p, int bright){
	Uint32 *rgb=map.rpg->RGB;
	int w=mag,h=mag,x2,y2;

	if(mag==8){
		x2=(n%8)*8;
		y2=(n/8)*8;
	}
	else if(mag==16){
		x2=64+(n%8)*16;
		y2=(n/8)*16;
	}
	else if(mag==32){
		x2=192+(n%8)*32;
		y2=(n/8)*32;
	}

	if(x<0){x2-=x;w+=x;x=0;}
	if(y<0){y2-=y;h+=y;y=0;}
	if(x+w>320)w=320-x;
	if(y+h>240)h=240-y;
	px=px+y*scrWidth+x;
	Uint16 px_skip=scrWidth-w;
	rgb=rgb+y2*(map.rpg->w)+x2;
	Uint16 rgb_skip=(map.rpg->w)-w;
	SDL_Color col;

	int shore_bright=1000;
	if(gd.hour<=5 || gd.hour>=23)shore_bright=200;
	else if(gd.hour==6 || gd.hour==22)shore_bright=bright;

	for(int j=0 ; j<h ; j++){
		for(int i=0 ; i<w ; i++){
			getRGB(*rgb,&col.r,&col.g,&col.b);
			if(n>=8 && col.r==255 && col.g==255 && col.b==255){
				*px = setRGB(128*shore_bright/1000,128*shore_bright/1000,0);
			}
			else if(n==7 && col.r==255 && col.g==255 && col.b==0){
				*px = setRGB(255,255,0);
			}
			else if(*rgb!=0){
				*px = setRGB(col.r*bright/1000,col.g*bright/1000,col.b*bright/1000);
			}
			else if(n==1){
				getRGB(map.RGB[p],&col.r,&col.g,&col.b);
				*px = setRGB(col.r*bright/1000,col.g*bright/1000,col.b*bright/1000);
			}
			px++;
			rgb++;
		}
		px+=px_skip;
		rgb+=rgb_skip;
	}
}

void drawGround(SDL_Surface *scr, int x, int y, int x2, int y2, int w, int h, int bright, BOOL buf){
	SDL_LockSurface(scr);
		
	x+=MAGNIFY/2;
	y+=MAGNIFY/2;
	if(x2<0){w+=x2;x2=0;}
	if(y2<0){h+=y2;y2=0;}
	if(x2+w>320)w=320-x2;
	if(y2+h>240)h=240-y2;
	int countX=x%MAGNIFY+1,countY=y%MAGNIFY+1;

	Uint32* px;
	Uint16 px_skip,scrWidth;
	if(buf){
		px=img.buffer[0]->RGB;
		px=px+y2*320+x2;
		scrWidth=320;
		px_skip=320-w;
	}else{
		px=(Uint32*)scr->pixels;
		px=px+y2*((scr->pitch)/4)+x2;
		scrWidth=(scr->pitch)/4;
		px_skip=((scr->pitch)/4)-w;
	}
	Uint8 px_r,px_g,px_b;
	Uint32* px_RGB=map.RGB+(y/MAGNIFY)*map.mapW+(x/MAGNIFY);
	Uint8* px_city=map.type+(y/MAGNIFY)*map.mapW+(x/MAGNIFY);
	Uint8* px_c=map.c+y2*map.mapW+x2;
	Uint16 rgb_skip=map.mapW-w/MAGNIFY, c_skip=320-w;
	Uint16 rgb_back=w/MAGNIFY;

	if(buf)fillRect(img.buffer[0],x2,y2,w,h,0,0,0,255);

	if(MAGNIFY>=8){
		int a,X,Y;
		for(int j=0 ; j<h/MAGNIFY+2 ; j++)for(int i=0 ; i<w/MAGNIFY+1 ; i++){
			a=(y/MAGNIFY+j)*map.mapW+(x/MAGNIFY+i);
			X=i*MAGNIFY-x%MAGNIFY;
			Y=j*MAGNIFY-y%MAGNIFY;
			switch(map.type[a]){
				case NULL:
				case ROAD: drawRPGchip(px,scrWidth,X,Y,MAGNIFY,0,a,bright);break;
				case VOLCANO: drawRPGchip(px,scrWidth,X,Y,MAGNIFY,2,a,bright);break;
				case TOWER_S: drawRPGchip(px,scrWidth,X,Y,MAGNIFY,3,a,bright);break;
				case TOWER_M: drawRPGchip(px,scrWidth,X,Y,MAGNIFY,4,a,bright);break;
				case TOWER_L: drawRPGchip(px,scrWidth,X,Y,MAGNIFY,5,a,bright);break;
				case CITY: drawRPGchip(px,scrWidth,X,Y,MAGNIFY,6,a,bright);break;
				case TOWN: drawRPGchip(px,scrWidth,X,Y,MAGNIFY,7,a,bright);break;
				case SHORE: drawRPGchip(px,scrWidth,X,Y,MAGNIFY,8+map.shore[a],a,bright);break;
				case SEA: break;
				default: drawRPGchip(px,scrWidth,X,Y,MAGNIFY,1,a,bright);break;
			}
		}
	}else{
		if(bright==1000){
			for(int j=0 ; j<h ; j++){
				for(int i=0 ; i<w ; i++){
					if(*px_RGB){
						if(*px_city==CITY){
							getRGB(*px_RGB,&px_r,&px_g,&px_b);
							if(px_b+*px_c>255)*px=setRGB(px_r,px_g,255);
							else *px=setRGB(px_r,px_g,px_b+*px_c);
						}else{
							*px=*px_RGB;
						}
					}
					px++;px_c++;
					if(countX==MAGNIFY){
						px_RGB++;px_city++;
						countX=1;
					}
					else countX++;
				}
				px+=px_skip;px_c+=c_skip;
				if(countY==MAGNIFY){
					px_RGB+=rgb_skip;
					px_city+=rgb_skip;
					countY=1;
				}else{
					px_RGB-=rgb_back;
					px_city-=rgb_back;
					countY++;
				}
			}
		}else{
			for(int j=0 ; j<h ; j++){
				for(int i=0 ; i<w ; i++){
					if(*px_RGB){
						getRGB(*px_RGB,&px_r,&px_g,&px_b);
						if(*px_city==CITY){
							if(px_b*bright/1000+*px_c>255)*px=setRGB(px_r*bright/1000,px_g*bright/1000,255);
							else *px=setRGB(px_r*bright/1000,px_g*bright/1000,px_b*bright/1000+*px_c);
						}else{
							*px=setRGB(px_r*bright/1000,px_g*bright/1000,px_b*bright/1000);
						}
					}
					px++;px_c++;
					if(countX==MAGNIFY){
						px_RGB++;px_city++;
						countX=1;
					}
					else countX++;
				}
				px+=px_skip;px_c+=c_skip;
				if(countY==MAGNIFY){
					px_RGB+=rgb_skip;
					px_city+=rgb_skip;
					countY=1;
				}else{
					px_RGB-=rgb_back;
					px_city-=rgb_back;
					countY++;
				}
			}
		}
	}

	if(buf){
		setColorKey(img.buffer[0],0,0,0);
		map.buffered=TRUE;
	}

	SDL_UnlockSurface(scr);
}

void drawTowerSpot(SDL_Surface *scr, int x, int y, int x2, int y2, int w, int h, BOOL buf){
	SDL_LockSurface(scr);
	if(SHOW_TOWER==NULL)return;
	x+=MAGNIFY/2;
	y+=MAGNIFY/2;
	if(x<0)x=0;
	if(y<0)y=0;
	if(x2<0)x2=0;
	if(y2<0)y2=0;
	if(x2+w>320)w=320-x2;
	if(y2+h>240)h=240-y2;
	int countX=x%MAGNIFY,countY=y%MAGNIFY;

	Uint32* px;
	Uint16 px_skip, px_down;
	if(buf)px=img.buffer[0]->RGB;
	else px=(Uint32*)scr->pixels;
	Uint32* px2=img.buffer[1]->RGB;
	Uint8* px_tp=map.type+(y/MAGNIFY)*map.mapW+(x/MAGNIFY);
	Uint16 tp_skip=map.mapW-(w/MAGNIFY);
	px=px+y2*(scr->pitch)/4+x2;
	if(countY!=0)px_tp=px_tp+w/MAGNIFY;

	if(buf){
		fillRect(img.buffer[1],x2,y2,w,h,0,0,0,255);
		px_skip=320-w;
		px_down=320;
	}else{
		px_skip=(scr->pitch)/4-w;
		px_down=(scr->pitch)/4;
	}

	if(buf){
		if(MAGNIFY<=4){
			for(int j=0 ; j<h ; j++){
				if(countY==MAGNIFY){countY=0;px_tp+=tp_skip;}
				countY++;
				if(countY!=1){
					px+=px_down;px2+=px_down;
					continue;
				}
				for(int i=0 ; i<w ; i++){
					if(countX==MAGNIFY){countX=0;px_tp++;}
					countX++;
					if(countX!=1){
						px++;px2++;
						continue;
					}
					if(*px_tp==TOWER_L){
						if(j>0){
							if(i>0){
								*(px-px_down-1)=setRGB(255,255,0);
								*(px2-px_down-1)=setRGB(255,255,255);
							}
							*(px-px_down)=setRGB(255,255,0);
							*(px2-px_down)=setRGB(255,255,255);
							if(i<w-1){
								*(px-px_down+1)=setRGB(255,255,0);
								*(px2-px_down+1)=setRGB(255,255,255);
							}
						}
						if(i>0){
							*(px-1)=setRGB(255,255,0);
							*(px2-1)=setRGB(255,255,255);
						}
						if(i<w-1){
							*(px+1)=setRGB(255,255,0);
							*(px2+1)=setRGB(255,255,255);
						}
						if(j<h-1){
							if(i>0){
								*(px+px_down-1)=setRGB(255,255,0);
								*(px2+px_down-1)=setRGB(255,255,255);
							}
							*(px+px_down)=setRGB(255,255,0);
							*(px2+px_down)=setRGB(255,255,255);
							if(i<w-1){
								*(px+px_down+1)=setRGB(255,255,0);
								*(px2+px_down+1)=setRGB(255,255,255);
							}
						}
					}
					else if(*px_tp==TOWER_M){
						*px=setRGB(255,0,0);
						*px2=setRGB(255,255,255);
						if(i<w-1){
							*(px+1)=setRGB(255,0,0);
							*(px2+1)=setRGB(255,255,255);
						}
						if(j<h-1){
							*(px+px_down)=setRGB(255,0,0);
							*(px2+px_down)=setRGB(255,255,255);
							if(i<w-1){
								*(px+px_down+1)=setRGB(255,0,0);
								*(px2+px_down+1)=setRGB(255,255,255);
							}
						}
					}
					else if(*px_tp==TOWER_S){
						*px=setRGB(0,0,255);
						*px2=setRGB(255,255,255);
					}
					px++;px2++;
				}
				px+=px_skip;px2+=px_skip;
			}
		}
		else if(MAGNIFY==8){
			for(int j=0 ; j<h ; j++){
				if(countY==MAGNIFY){countY=0;px_tp+=tp_skip;}
				countY++;
				if(countY!=1){
					px+=px_down;px2+=px_down;
					continue;
				}
				for(int i=0 ; i<w ; i++){
					if(countX==MAGNIFY){countX=0;px_tp++;}
					countX++;
					if(countX!=1){
						px++;px2++;
						continue;
					}
					if(*px_tp==TOWER_L){
						if(i<w-4){
							*(px+3)=setRGB(255,255,0);
							*(px2+3)=setRGB(255,255,255);
						}
						if(i<w-5){
							*(px+4)=setRGB(255,255,0);
							*(px2+4)=setRGB(255,255,255);
						}
						if(j<h-1){
							if(i<w-4){
								*(px+px_down+3)=setRGB(255,255,0);
								*(px2+px_down+3)=setRGB(255,255,255);
							}
							if(i<w-5){
								*(px+px_down+4)=setRGB(255,255,0);
								*(px2+px_down+4)=setRGB(255,255,255);
							}
						}
					}
					else if(*px_tp==TOWER_M){
						if(i<w-4){
							*(px+3)=setRGB(255,0,0);
							*(px2+3)=setRGB(255,255,255);
						}
					}
					else if(*px_tp==TOWER_S){
						if(i<w-4){
							*(px+3)=setRGB(0,0,255);
							*(px2+3)=setRGB(255,255,255);
						}
					}
					px++;px2++;
				}
				px+=px_skip;px2+=px_skip;
			}
		}
		else if(MAGNIFY==16){
			for(int j=0 ; j<h ; j++){
				if(countY==MAGNIFY){countY=0;px_tp+=tp_skip;}
				countY++;
				if(countY!=1){
					px+=px_down;px2+=px_down;
					continue;
				}
				for(int i=0 ; i<w ; i++){
					if(countX==MAGNIFY){countX=0;px_tp++;}
					countX++;
					if(countX!=1){
						px++;px2++;
						continue;
					}
					if(*px_tp==TOWER_L){
						for(int a=0 ; a<4 ; a++){
							if(j>=h-a)break;
							for(int b=6 ; b<10 ; b++){
								if(i>=w-b)break;
								*(px+px_down*a+b)=setRGB(255,255,0);
								*(px2+px_down*a+b)=setRGB(255,255,255);
							}
						}
					}
					else if(*px_tp==TOWER_M){
						if(i<w-4){
							*(px+6)=setRGB(255,0,0);
							*(px2+6)=setRGB(255,255,255);
						}
						if(i<w-5){
							*(px+7)=setRGB(255,0,0);
							*(px2+7)=setRGB(255,255,255);
						}
						if(j<h-1){
							if(i<w-4){
								*(px+px_down+6)=setRGB(255,0,0);
								*(px2+px_down+6)=setRGB(255,255,255);
							}
							if(i<w-5){
								*(px+px_down+7)=setRGB(255,0,0);
								*(px2+px_down+7)=setRGB(255,255,255);
							}
						}
					}
					else if(*px_tp==TOWER_S){
						if(i<w-4){
							*(px+6)=setRGB(0,0,255);
							*(px2+6)=setRGB(255,255,255);
						}
						if(i<w-5){
							*(px+7)=setRGB(0,0,255);
							*(px2+7)=setRGB(255,255,255);
						}
						if(j<h-1){
							if(i<w-4){
								*(px+px_down+6)=setRGB(0,0,255);
								*(px2+px_down+6)=setRGB(255,255,255);
							}
							if(i<w-5){
								*(px+px_down+7)=setRGB(0,0,255);
								*(px2+px_down+7)=setRGB(255,255,255);
							}
						}
					}
					px++;px2++;
				}
				px+=px_skip;px2+=px_skip;
			}
		}
		else if(MAGNIFY==32){
			for(int j=0 ; j<h ; j++){
				if(countY==MAGNIFY){countY=0;px_tp+=tp_skip;}
				countY++;
				if(countY!=1){
					px+=px_down;px2+=px_down;
					continue;
				}
				for(int i=0 ; i<w ; i++){
					if(countX==MAGNIFY){countX=0;px_tp++;}
					countX++;
					if(countX!=1){
						px++;px2++;
						continue;
					}
					if(*px_tp==TOWER_L){
						for(int a=0 ; a<8 ; a++){
							if(j>=h-a)break;
							for(int b=12 ; b<20 ; b++){
								if(i>=w-b)break;
								*(px+px_down*a+b)=setRGB(255,255,0);
								*(px2+px_down*a+b)=setRGB(255,255,255);
							}
						}
					}
					else if(*px_tp==TOWER_M){
						for(int a=0 ; a<4 ; a++){
							if(j>=h-a)break;
							for(int b=12 ; b<16 ; b++){
								if(i>=w-b)break;
								*(px+px_down*a+b)=setRGB(255,0,0);
								*(px2+px_down*a+b)=setRGB(255,255,255);
							}
						}
					}
					else if(*px_tp==TOWER_S){
						for(int a=0 ; a<4 ; a++){
							if(j>=h-a)break;
							for(int b=12 ; b<16 ; b++){
								if(i>=w-b)break;
								*(px+px_down*a+b)=setRGB(0,0,255);
								*(px2+px_down*a+b)=setRGB(255,255,255);
							}
						}
					}
					px++;px2++;
				}
				px+=px_skip;px2+=px_skip;
			}
		}
	}else{
		Uint8 col=abs(255-(count%255)*2);
		if(MAGNIFY<=4){
			for(int j=0 ; j<h ; j++){
				if(countY==MAGNIFY){countY=0;px_tp+=tp_skip;}
				countY++;
				if(countY!=1){
					px+=px_down;
					continue;
				}
				for(int i=0 ; i<w ; i++){
					if(countX==MAGNIFY){countX=0;px_tp++;}
					countX++;
					if(countX!=1){
						px++;
						continue;
					}
					if(*px_tp==TOWER_L){
						if(j>0){
							if(i>0)*(px-px_down-1)=setRGB(255,255,col);
							*(px-px_down)=setRGB(255,255,col);
							if(i<w-1)*(px-px_down+1)=setRGB(255,255,col);
						}
						if(i>0)*(px-1)=setRGB(255,255,col);
						if(i<w-1)*(px+1)=setRGB(255,255,col);
						if(j<h-1){
							if(i>0)*(px+px_down-1)=setRGB(255,255,col);
							*(px+px_down)=setRGB(255,255,col);
							if(i<w-1)*(px+px_down+1)=setRGB(255,255,col);
						}
					}
					else if(*px_tp==TOWER_M){
						*px=setRGB(255,col,col);
						if(i<w-1)*(px+1)=setRGB(255,col,col);
						if(j<h-1){
							*(px+px_down)=setRGB(255,col,col);
							if(i<w-1)*(px+px_down+1)=setRGB(255,col,col);
						}
					}
					else if(*px_tp==TOWER_S){
						*px=setRGB(col,col,255);
					}
					px++;
				}
				px+=px_skip;
			}
		}
		else if(MAGNIFY==8){
			for(int j=0 ; j<h ; j++){
				if(countY==MAGNIFY){countY=0;px_tp+=tp_skip;}
				countY++;
				if(countY!=1){
					px+=px_down;
					continue;
				}
				for(int i=0 ; i<w ; i++){
					if(countX==MAGNIFY){countX=0;px_tp++;}
					countX++;
					if(countX!=1){
						px++;
						continue;
					}
					if(*px_tp==TOWER_L){
						if(i<w-4)*(px+3)=setRGB(255,255,col);
						if(i<w-5)*(px+4)=setRGB(255,255,col);
						if(j<h-1){
							if(i<w-4)*(px+px_down+3)=setRGB(255,255,col);
							if(i<w-5)*(px+px_down+4)=setRGB(255,255,col);
						}
					}
					else if(*px_tp==TOWER_M){
						if(i<w-4)*(px+3)=setRGB(255,col,col);
					}
					else if(*px_tp==TOWER_S){
						if(i<w-4)*(px+4)=setRGB(col,col,255);
					}
					px++;
				}
				px+=px_skip;
			}
		}
		else if(MAGNIFY==16){
			for(int j=0 ; j<h ; j++){
				if(countY==MAGNIFY){countY=0;px_tp+=tp_skip;}
				countY++;
				if(countY!=1){
					px+=px_down;
					continue;
				}
				for(int i=0 ; i<w ; i++){
					if(countX==MAGNIFY){countX=0;px_tp++;}
					countX++;
					if(countX!=1){
						px++;
						continue;
					}
					if(*px_tp==TOWER_L){
						for(int a=0 ; a<4 ; a++){
							if(j>=h-a)break;
							for(int b=6 ; b<10 ; b++){
								if(i>=w-b)break;
								*(px+px_down*a+b)=setRGB(255,255,col);
							}
						}
					}
					else if(*px_tp==TOWER_M){
						if(i<w-4)*(px+6)=setRGB(255,col,col);
						if(i<w-5)*(px+7)=setRGB(255,col,col);
						if(j<h-1){
							if(i<w-4)*(px+px_down+6)=setRGB(255,col,col);
							if(i<w-5)*(px+px_down+7)=setRGB(255,col,col);
						}
					}
					else if(*px_tp==TOWER_S){
						if(i<w-4)*(px+6)=setRGB(col,col,255);
						if(i<w-5)*(px+7)=setRGB(col,col,255);
						if(j<h-1){
							if(i<w-4)*(px+px_down+6)=setRGB(col,col,255);
							if(i<w-5)*(px+px_down+7)=setRGB(col,col,255);
						}
					}
					px++;
				}
				px+=px_skip;
			}
		}
		else if(MAGNIFY==32){
			for(int j=0 ; j<h ; j++){
				if(countY==MAGNIFY){countY=0;px_tp+=tp_skip;}
				countY++;
				if(countY!=1){
					px+=px_down;
					continue;
				}
				for(int i=0 ; i<w ; i++){
					if(countX==MAGNIFY){countX=0;px_tp++;}
					countX++;
					if(countX!=1){
						px++;
						continue;
					}
					if(*px_tp==TOWER_L){
						for(int a=0 ; a<8 ; a++){
							if(j>=h-a)break;
							for(int b=12 ; b<20 ; b++){
								if(i>=w-b)break;
								*(px+px_down*a+b)=setRGB(255,255,col);
							}
						}
					}
					else if(*px_tp==TOWER_M){
						for(int a=0 ; a<4 ; a++){
							if(j>=h-a)break;
							for(int b=12 ; b<16 ; b++){
								if(i>=w-b)break;
								*(px+px_down*a+b)=setRGB(255,col,col);
							}
						}
					}
					else if(*px_tp==TOWER_S){
						for(int a=0 ; a<4 ; a++){
							if(j>=h-a)break;
							for(int b=12 ; b<16 ; b++){
								if(i>=w-b)break;
								*(px+px_down*a+b)=setRGB(col,col,255);
							}
						}
					}
					px++;
				}
				px+=px_skip;
			}
		}
	}

	if(buf){
		setColorKey(img.buffer[0],0,0,0);
		setColorKey(img.buffer[1],0,0,0);
		map.buffered=TRUE;
	}

	SDL_UnlockSurface(scr);
}

void drawFireWork(SDL_Surface *scr){
	if(gd.firework>0){
		int v_max=0;
		for(int i=0 ; i<map.volcanoNum ; i++)if(map.volcano[i]!=0){
			int mX=map.volcanoX[i], mY=map.volcanoY[i], a=133-map.volcano[i]/15;
			if(map.volcano[i]>1600)drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX-10,mY*MAGNIFY-gd.scrY-40,100+((count/4)%2)*20,240,20,40,255);
			else if(map.volcano[i]>1200)drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX-10,mY*MAGNIFY-gd.scrY-40,140+((count/4)%2)*20,240,20,40,255);
			else if(map.volcano[i]>800)drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX-10,mY*MAGNIFY-gd.scrY-40,180+((count/4)%2)*20,240,20,40,255);
			else drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX-10,mY*MAGNIFY-gd.scrY-20,220+((count/4)%2)*20,260,20,20,255);
			if(map.volcano[i]<=800 && map.volcano[i]>400 && count%100<8)drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX-10,mY*MAGNIFY-gd.scrY-30,220+((count/4)%2)*20,240,20,20,255);
			int x1=a-40,y1=a*3/2,x2=a*3/2-40,y2=a;
			int b=a,c=a*3/2,d=128;
			if(x1<0)x1=0;if(x2<0)x2=0;
			if(b>40)b=40;if(c>40)c=40;
			if(map.volcano[i]<128)d=map.volcano[i];
			drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX+x1,mY*MAGNIFY-gd.scrY-y1,300-b,240,b,c,d);
			drawImage(scr,img.chr,mX*MAGNIFY-gd.scrX+x2,mY*MAGNIFY-gd.scrY-y2,300-c,240,c,b,d);
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

void drawColorLight(int x, int y, int w, int h, BOOL buf){
	if(AIR_IMG==NULL)return;
	x-=gd.scrX;
	y-=gd.scrY;

	int minX=320,maxX=0,minY=240,maxY=0;
	int X,Y,X2,Y2,W,H;

	fillRect(img.buffer[2],0,0,320,240,0,0,0,255);

	for(int i=0 ; i<towers ; i++){
		if(tower[i].remove)continue;
		int n=tower[i].c1, m=tower[i].c2-1;
		if(n==0 || m<0 || m>=8)continue;
		X=tower[i].x*MAGNIFY-gd.scrX-m*5;
		Y=tower[i].y*MAGNIFY-gd.scrY-m*5;
		X2=0;Y2=0;W=m*10+10;H=m*10+10;


		if(X>=x+w || Y>=y+h)continue;
		if(X+W<x|| Y+H<y)continue;
		if(X<x){X2+=x-X;W-=x-X;X=x;}
		if(Y<y){Y2+=y-Y;H-=y-Y;Y=y;}
		if(X+W>x+w)W=x+w-X;
		if(Y+H>y+h)H=y+h-Y;

		drawLight(img.buffer[2],img.colorlight,X,Y,X2+m*80,Y2+(n-1)*80,W,H,255);

		if(minX>X-1)minX=X-1;
		if(maxX<X+W)maxX=X+W;
		if(minY>Y-1)minY=Y-1;
		if(maxY<Y+H)maxY=Y+H;
	}

	map.airX=minX+1;
	map.airW=maxX-minX-1;
	map.airY=minY+1;
	map.airH=maxY-minY-1;

	if(AIR_IMG==TYPE1_EDGE || AIR_IMG==TYPE2_EDGE){
		if(minX<x+1)minX=x+1;
		if(maxX>x+w-1)maxX=x+w-1;
		if(minY<y+1)minY=y+1;
		if(maxY>y+h-1)maxY=y+h-1;
		W=maxX-minX;
		H=maxY-minY;

		Uint32* px=img.buffer[2]->RGB;
		Uint32 white=setRGB(255,255,255);
		px=px+(minY*320+minX);
		Uint16 px_skip=320-W;

		for(int j=0 ; j<H ; j++){
			for(int i=0 ; i<W ; i++){
				if(!(*px)){
					if( *(px-1) && *(px-1)!=white ){
						*px=white;
					}
					else if( *(px+1) && *(px+1)!=white ){
						*px=white;
					}
					else if( *(px-320) && *(px-320)!=white ){
						*px=white;
					}
					else if( *(px+320) && *(px+320)!=white ){
						*px=white;
					}
				}
				px++;
			}
			px+=px_skip;
		}
	}

	if(buf)map.buffered2=TRUE;
}

void drawVolcano(int x, int y, int w, int h, BOOL buf){
	if(AIR_IMG==NULL || mode==GAMEMENU || mode==MIYAZAKI)return;
	x-=gd.scrX;
	y-=gd.scrY;
	int minX=320,maxX=0,minY=240,maxY=0;
	int X,Y,X2,Y2,W,H;

	fillRect(img.buffer[3],0,0,320,240,0,0,0,255);
	for(int i=0 ; i<mounts ; i++)if(mount[i].volcano){
		X=mount[i].x*MAGNIFY-gd.scrX-5;
		Y=mount[i].y*MAGNIFY-gd.scrY-5;
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

		drawLight(img.buffer[3],img.colorlight,X,Y,X2,Y2,W,H,255);
	}

	map.volX=minX;
	map.volW=maxX-minX;
	map.volY=minY;
	map.volH=maxY-minY;
}

SDL_Color getSkyColor(int h, int m){
	SDL_Color col;
	col.r=0;col.g=0;col.b=0;
	int a=h*60+m;
	if(h<=gd.sunrise_hour-2){
		col.r=0;
		col.g=0;
		col.b=0;
	}
	else if(h==gd.sunrise_hour-1){
		col.r=0;
		col.g=0;
		col.b=(a-(gd.sunrise_hour-1)*60)*2;
	}
	else if(h==gd.sunrise_hour){
		col.r=(int)((a-gd.sunrise_hour*60)*3.2);
		col.g=(a-gd.sunrise_hour*60)*2;
		col.b=128;
	}
	else if(h<=8){
		col.r=192;
		col.g=128+(a-(gd.sunrise_hour+1)*60)/(8-gd.sunrise_hour);
		col.b=128+(a-(gd.sunrise_hour+1)*60)/(8-gd.sunrise_hour);
	}
	else if(h<=11){
		col.r=192;
		col.g=192;
		col.b=192+(int)((a-540)/2.8);
	}
	else if(h<=gd.sunset_hour-3){
		col.r=192-(int)(((a-720)/2.8)/(gd.sunset_hour-14));
		col.g=192-(int)(((a-720)/2.8)/(gd.sunset_hour-14));
		col.b=255;
	}
	else if(h<=gd.sunset_hour-2){
		col.r=128+((a-(gd.sunset_hour-2)*60)*2);
		col.g=128;
		col.b=255-(int)((a-(gd.sunset_hour-2)*60)*4.2);
	}
	else if(h==gd.sunset_hour-1){
		if(m<30){
			col.r=255;
			col.g=128-(a-(gd.sunset_hour-1)*60)*2;
			col.b=(int)((a-(gd.sunset_hour-1)*60)*8.5);
		}else{
			col.r=255-(int)((a-(gd.sunset_hour-1)*60-30)*6.4);
			col.g=64;
			col.b=128+(a-(gd.sunset_hour-1)*60-30)*4;
		}
	}
	else if(h==gd.sunset_hour){
		col.r=64-(a-(gd.sunset_hour*60));
		col.g=64-(a-(gd.sunset_hour*60));
		col.b=255-(int)((a-gd.sunset_hour*60)*4.2);
	}
	else if(h<=27){
		col.r=0;
		col.g=0;
		col.b=1;
	}
	return col;
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
		if(gd.hour<=6 || gd.hour>=23)aa=0.2;
		if(gd.hour==22)aa=(120.0-gd.minute*1.3)/120;
		else if(gd.hour==7)aa=(20.0+gd.minute*1.3)/120;
	}

	for(int i=(int)X-mg ; i<(int)X+mg ; i++)for(int j=(int)Y-mg ; j<(int)Y+mg ; j++){
		cubeW=0;
		if(i<0 || i>=map.mapW || j<0 || j>=map.mapH)continue;
		dis=sqrt(1.0*(i-X)*(i-X)+1.0*(Y-j)*(Y-j));
		if(dis<2 || dis>=mg)continue;
		double mound=(mg/8.0)*(mg/8.0)*sqrt( 1-(abs(dis-span)/span) );
		xd=asin_q(dis,i-X,Y-j);
		if(xd>180 && D<90)xd-=360;
		if(xd<180 && D>270)xd+=360;
		xd=D-xd;// xd‚Í•ûŠp
		if(abs(xd)>gd.zoom*3)continue; //Ž‹ŠE‚ÌŠO
		zd=asin_q(dis*MAP_SCALE*1000,0,map.h[i][j]-Z+mound);
		if(zd>180)zd-=360; //zd‚Í‹ÂŠp

		Uint16 *px_z=map.z, px_skip;
		Uint32 *px_rgb=img.buffer[0]->RGB;
		Uint32 col;
		Uint8 R,G,B;
		BOOL flat=FALSE,road=FALSE;

		if(map.h[i][j]==0){
			if(map.type[j*map.mapW+i]==SEA || map.type[j*map.mapW+i]==SHORE){
				col=setRGB(0,0,0);
			}
			else col=setRGB((int)(128*aa),(int)(128*aa),0);
			flat=TRUE;
		}
		else if(map.type[j*map.mapW+i]==SEA || map.type[j*map.mapW+i]==SHORE){
			col=setRGB(0,0,0);
			flat=TRUE;road=TRUE;
		}
		else if(map.type[j*map.mapW+i]==ROAD){
			col=setRGB((int)(128*aa),(int)(128*aa),0);
			flat=TRUE;road=TRUE;
		}else{
			getRGB(map.RGB[j*map.mapW+i],&R,&G,&B);
			col=setRGB( (int)(R*aa),(int)(G*aa),(int)(B*aa) );
		}

		cubeW=(int)(120.0*asin_q(dis,0,1)/gd.zoom);
		if(cubeW==0)cubeW=1;
		cubeH=(int)(cubeW*1.5);
		half=cubeW/2;
		int cubeX=160+(int)(80.0*xd/gd.zoom)-half;
		int cubeY=120-(int)(80.0*zd/gd.zoom);
		flat=TRUE;
//		if(road||flat)cubeY+=half;

		if(cubeX<0){cubeW+=cubeX;cubeX=0;}
		if(cubeY<0){cubeH+=cubeY;cubeY=0;}
		if(cubeX+cubeW>320)cubeW=320-cubeX;
		if(cubeY+cubeH>240)cubeH=240-cubeY;
		px_z+=cubeY*320+cubeX;
		px_rgb+=cubeY*320+cubeX;
		px_skip=320-cubeW;

		for(int a=0 ; a<cubeH ; a++){
			for(int b=0 ; b<cubeW ; b++){
				if(*px_z>(Uint16)dis){
					if(flat || a>half || b>half-(int)(half*sin((PI/2)*a/half)) && b<half+(int)(half*sin((PI/2)*a/half))){
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
	setAlpha(img.buffer[0],0,0,0);
}

void make3dview_tower(double X, double Y, int D){
	fillRect(img.buffer[1],0,0,320,240,0,0,0,255);
	double xd,zd,zd2,dis;
	int Z=gd.height;
	if(Z<map.h[(int)X][(int)Y])Z=map.h[(int)X][(int)Y];
	double span=8.0*sqrt(1.0*Z);
	int mg=(int)(span*2/3);

	Tower *tw=tower;
	for(int i=0 ; i<towers ; i++){
		tw->x_3d=400;
		dis=sqrt(1.0*(tw->x-X)*(tw->x-X)+1.0*(tw->y-Y)*(tw->y-Y));
		if(dis>=mg){tw++;continue;}
		double mound=(mg/8.0)*(mg/8.0)*sqrt( 1-(abs(dis-span)/span) );
		xd=atan_q(tw->x-X,Y-tw->y);
		zd=atan_q(dis*MAP_SCALE*1000,tw->h+map.h[tw->x][tw->y]-Z+mound);
		zd2=atan_q(dis*MAP_SCALE*1000,map.h[tw->x][tw->y]-Z+mound);
		if(xd>180 && D<90)xd-=360;
		if(xd<180 && D>270)xd+=360;
		xd=D-xd;
		if(zd>180)zd-=360;
		if(zd2>180)zd2-=360;
		int m=1;
		if(tw->kw>0)m++;
		if(tw->kw>10)m++;
		m-=tw->kw2;
		int A=160+(int)(80.0*xd/gd.zoom), B=120-(int)(80.0*zd/gd.zoom);
		tw->x_3d=A;
		tw->y_3d=B;

		if(A<0 || A>=320 || B<0 || B>=240){tw++;continue;}

		for(int a=0 ; a<=(int)abs(zd-zd2)*10 ; a++){
			if(B+a<0 || B+a>=240)continue;
			if(m>=2 && A-1>=0){
				if(map.z[(B+a)*320+(A-1)]>(Uint16)dis){
					img.buffer[0]->RGB[(B+a)*320+A-1]=setRGB(255,255,0);
					img.buffer[1]->RGB[(B+a)*320+A-1]=setRGB(255,255,255);
					map.z[(B+a)*320+(A-1)]=(Uint16)dis;
				}
			}
			if(m>=1 && A+1<320){
				if(map.z[(B+a)*320+(A+1)]>(Uint16)dis){
					if(m>=2)img.buffer[0]->RGB[(B+a)*320+A+1]=setRGB(255,255,0);
					else if(m==1)img.buffer[0]->RGB[(B+a)*320+A+1]=setRGB(255,0,0);
					img.buffer[1]->RGB[(B+a)*320+A+1]=setRGB(255,255,255);
					map.z[(B+a)*320+(A+1)]=(Uint16)dis;
				}
			}
			if(map.z[(B+a)*320+A]>(Uint16)dis){
				if(m>=2)img.buffer[0]->RGB[(B+a)*320+A]=setRGB(255,255,0);
				else if(m==1)img.buffer[0]->RGB[(B+a)*320+A]=setRGB(255,0,0);
				else img.buffer[0]->RGB[(B+a)*320+A]=setRGB(0,0,255);
				img.buffer[1]->RGB[(B+a)*320+A]=setRGB(255,255,255);
				map.z[(B+a)*320+A]=(Uint16)dis;
			}
		}
		tw++;
	}
	setAlpha(img.buffer[1],0,0,0);
}

void make3dview_sky(){
	SDL_Color sky=getSkyColor(gd.hour,gd.minute);
	Uint32 col=setRGB(sky.r,sky.g,sky.b);

	Uint16 *px_z=map.z;
	Uint32 *px_rgb=img.buffer[0]->RGB;
	for(int i=0 ; i<76800 ; i++){
		if(*px_z==BEYOND){
			*px_rgb=col;
		}
		px_z++;px_rgb++;
	}
}

void resetZ(){
	Uint16 *p=map.z;
	for(int i=0 ; i<76800 ; i++){
		*p=BEYOND;
		p++;
	}
}
