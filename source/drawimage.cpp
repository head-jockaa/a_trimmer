#include "drawimage.h"

int imgPointX[640],imgPointY[480],horizontalX[640],horizontalY[640],verticalX[480],verticalY[480];


void clipping(int *x, int *y, int *scr_w, int *scr_h, int *x2, int *y2, int *w2, int *h2, int *ima_w, int *ima_h, int *a){
	if(*a<0)*a=0;
	if(*a>255)*a=255;
	//‚Í‚Ý‚¾‚·•”•ª‚Í•`‰æ‚µ‚È‚¢
	if(*x2<0){*w2+=*x2;*x2=0;}
	if(*y2<0){*h2+=*y2;*y2=0;}
	if(*x2+*w2>*ima_w)*w2=*ima_w-*x2;
	if(*y2+*h2>*ima_h)*h2=*ima_h-*y2;
	if(*x<0){*x2-=*x;*w2+=*x;*x=0;}
	if(*y<0){*y2-=*y;*h2+=*y;*y=0;}
	if(*x+*w2>*scr_w)*w2=*scr_w-*x;
	if(*y+*h2>*scr_h)*h2=*scr_h-*y;
}

void clipping_r(int *x, int *y, int *scr_w, int *scr_h, int *x2, int *y2, int *w2, int *h2, int *ima_w, int *ima_h, int *a){
	if(*a<0)*a=0;
	if(*a>255)*a=255;
	//‚Í‚Ý‚¾‚·•”•ª‚Í•`‰æ‚µ‚È‚¢
	if(*x2<0){*w2+=*x2;*x2=0;}
	if(*y2<0){*h2+=*y2;*y2=0;}
	if(*x2+*w2>*ima_w)*w2=*ima_w-*x2;
	if(*y2+*h2>*ima_h)*h2=*ima_h-*y2;
	if(*x<0){*w2+=*x;*x=0;}
	if(*y<0){*h2+=*y;*y=0;}
	if(*x+*w2>*scr_w){*x2+=*x+*w2-*scr_w;*w2=*scr_w-*x;}
	if(*y+*h2>*scr_h){*y2+=*y+*h2-*scr_h;*h2=*scr_h-*y;}
}

void __drawImage(Uint8* px, int pxwidth, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int a){
	Uint8 *rgb=(Uint8*)ima->RGB;
	Uint8 *alpha=(Uint8*)ima->A;
	Uint16 px_skip, rgb_skip, alpha_skip;

	px=px+(pxwidth*y+x)*4;
	px_skip=(pxwidth-w2)*4;
	rgb=rgb+(y2*(ima->w)+x2)*4;
	rgb_skip=((ima->w)-w2)*4;
	alpha=alpha+y2*(ima->w)+x2;
	alpha_skip=(ima->w)-w2;

	if(a==255){
		for(int j=0 ; j<h2 ; j++){
			for(int i=0 ; i<w2 ; i++){
				if(*(Uint8 *)alpha){
					*(Uint32 *)px = *(Uint32 *)rgb;
				}
				px+=4;
				rgb+=4;
				alpha++;
			}
			px+=px_skip;
			rgb+=rgb_skip;
			alpha+=alpha_skip;
		}
	}else{
		for(int j=0 ; j<h2 ; j++){
			for(int i=0 ; i<w2 ; i++){
				if(*(Uint8 *)alpha){
						*px+=( (a*(*rgb-*px)) >> 8 );
						*(px+1)+=( (a*(*(rgb+1)-*(px+1))) >> 8 );
						*(px+2)+=( (a*(*(rgb+2)-*(px+2))) >> 8 );
				}
				px+=4;
				rgb+=4;
				alpha++;
			}
			px+=px_skip;
			rgb+=rgb_skip;
			alpha+=alpha_skip;
		}
	}
}

void __drawImage(Uint8* px, int pxwidth, SDL_Surface* ima, int x, int y, int x2, int y2, int w2, int h2, int a){
	Uint8 *rgb=(Uint8*)ima->pixels;
	Uint16 px_skip, rgb_skip;

	px=px+(pxwidth*y+x)*4;
	px_skip=(pxwidth-w2)*4;
	rgb=rgb+(y2*(ima->w)+x2)*4;
	rgb_skip=((ima->w)-w2)*4;

	if(a==255){
		for(int j=0 ; j<h2 ; j++){
			for(int i=0 ; i<w2 ; i++){
				*(Uint32 *)px = *(Uint32 *)rgb;
				px+=4;
				rgb+=4;
			}
			px+=px_skip;
			rgb+=rgb_skip;
		}
	}else{
		for(int j=0 ; j<h2 ; j++){
			for(int i=0 ; i<w2 ; i++){
				*px+=( (a*(*rgb-*px)) >> 8 );
				*(px+1)+=( (a*(*(rgb+1)-*(px+1))) >> 8 );
				*(px+2)+=( (a*(*(rgb+2)-*(px+2))) >> 8 );
				px+=4;
				rgb+=4;
			}
			px+=px_skip;
			rgb+=rgb_skip;
		}
	}
}

void drawImage(SDL_Surface* scr, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
	SDL_LockSurface(scr);
	__drawImage((Uint8*)scr->pixels, scr->w, ima, x, y, x2, y2, w2, h2, a);
	SDL_UnlockSurface(scr);
}

void drawImage(Image* scr, SDL_Surface* ima, int x, int y, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
	SDL_LockSurface(ima);
	__drawImage((Uint8*)scr->RGB, scr->w, ima, x, y, x2, y2, w2, h2, a);
	SDL_UnlockSurface(ima);
}

void drawImage(Image* scr, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
	__drawImage((Uint8*)scr->RGB, scr->w, ima, x, y, x2, y2, w2, h2, a);
}

void __drawImage_r(Uint8* px, int pxwidth, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int a){
	Uint8 *rgb=(Uint8*)ima->RGB;
	Uint8 *alpha=(Uint8*)ima->A;
	Uint16 px_skip, rgb_skip, alpha_skip;

	px=px+(pxwidth*y+x+w2)*4;
	px_skip=(pxwidth+w2)*4;
	rgb=rgb+(y2*(ima->w)+x2)*4;
	rgb_skip=((ima->w)-w2)*4;
	alpha=alpha+y2*(ima->w)+x2;
	alpha_skip=(ima->w)-w2;

	if(a==255){
		for(int j=0 ; j<h2 ; j++){
			for(int i=0 ; i<w2 ; i++){
				px-=4;
				if(*(Uint8 *)alpha){
					*(Uint32 *)px = *(Uint32 *)rgb;
				}
				rgb+=4;
				alpha++;
			}
			px+=px_skip;
			rgb+=rgb_skip;
			alpha+=alpha_skip;
		}
	}else{
		for(int j=0 ; j<h2 ; j++){
			for(int i=0 ; i<w2 ; i++){
				px-=4;
				if(*(Uint8 *)alpha){
						*px+=( (a*(*rgb-*px)) >> 8 );
						*(px+1)+=( (a*(*(rgb+1)-*(px+1))) >> 8 );
						*(px+2)+=( (a*(*(rgb+2)-*(px+2))) >> 8 );
				}
				rgb+=4;
				alpha++;
			}
			px+=px_skip;
			rgb+=rgb_skip;
			alpha+=alpha_skip;
		}
	}
}

void drawImage_r(SDL_Surface* scr, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	clipping_r(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
	SDL_LockSurface(scr);
	__drawImage_r((Uint8*)scr->pixels, scr->w, ima, x, y, x2, y2, w2, h2, a);
	SDL_UnlockSurface(scr);
}

void drawImage_r(Image* scr, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	clipping_r(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
	__drawImage_r((Uint8*)scr->RGB, scr->w, ima, x, y, x2, y2, w2, h2, a);
}


// Šg‘åk¬•`‰æ
void __drawImage_x(Uint8* px, int pxwidth, int pxheight, Image* ima, int x, int y, double mag, int x2, int y2, int w2, int h2, int a){
	Uint8 *ima_a2,*ima_rgb2;
	Uint8 *ima_a = (Uint8*)ima->A;
	Uint8 *ima_rgb = (Uint8*)ima->RGB;
	Uint16 px_skip, rgb_skip, a_skip;
	double x_pitch, y_pitch;
	double x3=x2, y3=y2, w3=w2, h3=h2; // double‚É‚·‚é

	if(a<0)a=0;
	if(a>255)a=255;

	//‚Í‚Ý‚¾‚·•”•ª‚Í•`‰æ‚µ‚È‚¢
	if(x3<0){w3+=x3;x3=0;}
	if(y3<0){h3+=y3;y3=0;}
	if(x3+w3>(ima->w))w3=(ima->w)-x3;
	if(y3+h3>(ima->h))h3=(ima->h)-y3;

	int w=(int)(w3*mag);
	int h=(int)(h3*mag);

	//‚Í‚Ý‚¾‚·•”•ª‚Í•`‰æ‚µ‚È‚¢
	if(x<0){x3-=x/mag;w+=x;x=0;}
	if(y<0){y3-=y/mag;h+=y;y=0;}
	if(x+w>pxwidth)w=pxwidth-x;
	if(y+h>pxheight)h=pxheight-y;


	px=px+(pxwidth*y+x)*4;
	px_skip=(pxwidth-w)*4;

	rgb_skip=((ima->w)-(int)w3)*4;
	a_skip=(ima->w)-(int)w3;

	double grope2=1.0/mag;
	y_pitch=y3;

	if(a==255){
		for(int j=0 ; j<h ; j++){
			x_pitch=x3;
			for(int i=0 ; i<w ; i++){
				ima_rgb2=ima_rgb+((int)y_pitch*(ima->w)+(int)x_pitch)*4;
				ima_a2=ima_a+((int)y_pitch*(ima->w)+(int)x_pitch);
				if(*ima_a2){
					*(Uint32*)px=*(Uint32*)ima_rgb2;
				}
				x_pitch+=grope2;
				px+=4;
			}
			y_pitch+=grope2;
			px+=px_skip;
		}
	}else{
		for(int j=0 ; j<h ; j++){
			x_pitch=x3;
			for(int i=0 ; i<w ; i++){
				ima_rgb2=ima_rgb+((int)y_pitch*(ima->w)+(int)x_pitch)*4;
				ima_a2=ima_a+((int)y_pitch*(ima->w)+(int)x_pitch);
				if(*ima_a2){
					*px+=( (a*(*ima_rgb2-*px)) >> 8 );
					*(px+1)+=( (a*(*(ima_rgb2+1)-*(px+1))) >> 8 );
					*(px+2)+=( (a*(*(ima_rgb2+2)-*(px+2))) >> 8 );
				}
				x_pitch+=grope2;
				px+=4;
			}
			y_pitch+=grope2;
			px+=px_skip;
		}
	}
}

void drawImage_x(SDL_Surface* scr, Image* ima, int x, int y, double mag, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	SDL_LockSurface(scr);
	if(mag==1){
		clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
		__drawImage((Uint8*)scr->pixels, scr->w, ima, x, y, x2, y2, w2, h2, a);
	}else{
		__drawImage_x((Uint8*)scr->pixels, scr->w, scr->h, ima, x, y, mag, x2, y2, w2, h2, a);
	}
	SDL_UnlockSurface(scr);
}

void drawImage_x(Image* scr, Image* ima, int x, int y, double mag, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	if(mag==1){
		clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
		__drawImage((Uint8*)scr->RGB, scr->w, ima, x, y, x2, y2, w2, h2, a);
	}else{
		__drawImage_x((Uint8*)scr->RGB, scr->w, scr->h, ima, x, y, mag, x2, y2, w2, h2, a);
	}
}

// Šg‘åk¬•`‰æ‚Æ¶‰E‹t“]
void __drawImage_xr(Uint8* px, int pxwidth, int pxheight, Image* ima, int x, int y, double mag, int x2, int y2, int w2, int h2, int a){
	Uint8 *ima_a2,*ima_rgb2;
	Uint8 *ima_a = (Uint8*)ima->A;
	Uint8 *ima_rgb = (Uint8*)ima->RGB;
	Uint16 px_skip, rgb_skip, a_skip;
	double x_pitch, y_pitch;
	double x3=x2, y3=y2, w3=w2, h3=h2;

	if(a<0)a=0;
	if(a>255)a=255;

	//‚Í‚Ý‚¾‚·•”•ª‚Í•`‰æ‚µ‚È‚¢
	if(x3<0){w3+=x3;x3=0;}
	if(y3<0){h3+=y3;y3=0;}
	if(x3+w3>(ima->w))w3=(ima->w)-x3;
	if(y3+h3>(ima->h))h3=(ima->h)-y3;

	int w=(int)(w3*mag);
	int h=(int)(h3*mag);

	//‚Í‚Ý‚¾‚·•”•ª‚Í•`‰æ‚µ‚È‚¢
	if(x<0){x3-=x/mag;w+=x;x=0;}
	if(y<0){y3-=y/mag;h+=y;y=0;}
	if(x+w>pxwidth)w=pxwidth-x;
	if(y+h>pxheight)h=pxheight-y;

	px=px+(pxwidth*y+x+w)*4;
	px_skip=(pxwidth+w)*4;

	rgb_skip=((ima->w)-(int)w3)*4;
	a_skip=(ima->w)-(int)w3;

	double grope2=1.0/mag;
	y_pitch=y3;

	if(a==255){
		for(int j=0 ; j<h ; j++){
			x_pitch=x3;
			for(int i=0 ; i<w ; i++){
				px-=4;
				ima_rgb2=ima_rgb+((int)y_pitch*(ima->w)+(int)x_pitch)*4;
				ima_a2=ima_a+((int)y_pitch*(ima->w)+(int)x_pitch);
				if(*ima_a2){
					*(Uint32*)px=*(Uint32*)ima_rgb2;
				}
				x_pitch+=grope2;
			}
			y_pitch+=grope2;
			px+=px_skip;
		}
	}else{
		for(int j=0 ; j<h ; j++){
			x_pitch=x3;
			for(int i=0 ; i<w ; i++){
				px-=4;
				ima_rgb2=ima_rgb+((int)y_pitch*(ima->w)+(int)x_pitch)*4;
				ima_a2=ima_a+((int)y_pitch*(ima->w)+(int)x_pitch);
				if(*ima_a2){
					*px+=( (a*(*ima_rgb2-*px)) >> 8 );
					*(px+1)+=( (a*(*(ima_rgb2+1)-*(px+1))) >> 8 );
					*(px+2)+=( (a*(*(ima_rgb2+2)-*(px+2))) >> 8 );
				}
				x_pitch+=grope2;
			}
			y_pitch+=grope2;
			px+=px_skip;
		}
	}
}

void drawImage_xr(SDL_Surface* scr, Image* ima, int x, int y, double mag, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	if(mag==1){
		clipping_r(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
		drawImage_r(scr, ima, x, y, x2, y2, w2, h2, a);
	}else{
		SDL_LockSurface(scr);
		__drawImage_xr((Uint8*)scr->pixels, scr->w, scr->h, ima, x, y, mag, x2, y2, w2, h2, a);
		SDL_UnlockSurface(scr);
	}
}

void drawImage_xr(Image* scr, Image* ima, int x, int y, double mag, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	if(mag==1){
		clipping_r(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
		drawImage_r(scr, ima, x, y, x2, y2, w2, h2, a);
	}else{
		__drawImage_xr((Uint8*)scr->RGB, scr->w, scr->h, ima, x, y, mag, x2, y2, w2, h2, a);
	}
}

void __rotateImage(Uint8* px, int pxwidth, int pxheight, Image* ima, int x, int y, double rot, int x2, int y2, int cx, int cy, int w2, int h2, int a){
//Šg‘åk¬•`‰æ‚Æ‰ñ“]
	Uint8 *ima_a = ima->A;
	Uint8 *ima_rgb = (Uint8*)ima->RGB;
	int px_skip;

	if(a<0)a=0;
	if(a>255)a=255;

	double r1=sqrt(1.0*cx*cx+cy*cy);
	double r2=sqrt(1.0*(w2-cx)*(w2-cx)+cy*cy);
	double r3=sqrt(1.0*cx*cx+(h2-cy)*(h2-cy));
	double r4=sqrt(1.0*(w2-cx)*(w2-cx)+(h2-cy)*(h2-cy));
	int max_radius=(int)r1;
	if(max_radius<r2)max_radius=(int)r2;
	if(max_radius<r3)max_radius=(int)r3;
	if(max_radius<r4)max_radius=(int)r4;

	int w=max_radius*2, h=max_radius*2;
	x-=max_radius;y-=max_radius;

	int radius=w/2;
	int offsetX=-radius, offsetY=-radius;

	//‚Í‚Ý‚¾‚·•”•ª‚Í•`‰æ‚µ‚È‚¢
	if(x<0){offsetX-=x;w+=x;x=0;}
	if(y<0){offsetY-=y;h+=y;y=0;}
	if(x+w>pxwidth)w=pxwidth-x;
	if(y+h>pxheight)h=pxheight-y;

	px=px+(pxwidth*y+x)*4;
	px_skip=(pxwidth-w)*4;

	double rotateX, rotateY, backX, backY, rcos, rsin;
	int img_offset;
	rcos=cos(rot);
	rsin=sin(rot);
	backX=rcos*w;
	backY=rsin*w;

	rotateX=x2+cx+offsetX*rcos-offsetY*rsin;
	rotateY=y2+cy+offsetX*rsin+offsetY*rcos;
	for(int j=0 ; j<h ; j++){
		for(int i=0 ; i<w ; i++){
			if(rotateX<x2 || rotateX>=x2+w2 || rotateY<y2 || rotateY>=y2+h2){
				px+=4;rotateX+=rcos;rotateY+=rsin;continue;
			}
			img_offset=(int)rotateY*(ima->w)+(int)rotateX;
			if(*(ima_a+img_offset)){
				*(Uint32*)px=*(Uint32*)(ima_rgb+img_offset*4);
			}
			px+=4;
			rotateX+=rcos;
			rotateY+=rsin;
		}
		px+=px_skip;
		rotateX-=rsin;
		rotateY+=rcos;
		rotateX-=backX;
		rotateY-=backY;
	}
}

void rotateImage(SDL_Surface* scr, Image* ima, int x, int y, double rot, int x2, int y2, int cx, int cy, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	SDL_LockSurface(scr);
	__rotateImage((Uint8*)scr->pixels, scr->w, scr->h, ima, x, y, rot, x2, y2, cx, cy, w2, h2, a);
	SDL_UnlockSurface(scr);
}

void rotateImage(Image* scr, Image* ima, int x, int y, double rot, int x2, int y2, int cx, int cy, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	__rotateImage((Uint8*)scr->RGB, scr->w, scr->h, ima, x, y, rot, x2, y2, cx, cy, w2, h2, a);
}

void __rotateImage_x(Uint32* px, int pxwidth, int pxheight, Image* ima, int x, int y, float rot, float mag, int x2, int y2, int cx, int cy, int w2, int h2, int a){
//Šg‘åk¬•`‰æ‚Æ‰ñ“]
	Uint8 *ima_a = ima->A;
	Uint32 *ima_rgb = ima->RGB;
	int px_skip, rgb_skip, a_skip;

	if(a<0)a=0;
	if(a>255)a=255;

	int cx2=cx,cy2=cy;
	cx2=(int)(cx*mag);
	cy2=(int)(cy*mag);

	float r1=(float)sqrt(1.0*cx*cx+cy*cy);
	float r2=(float)sqrt(1.0*(w2-cx)*(w2-cx)+cy*cy);
	float r3=(float)sqrt(1.0*cx*cx+(h2-cy)*(h2-cy));
	float r4=(float)sqrt(1.0*(w2-cx)*(w2-cx)+(h2-cy)*(h2-cy));
	int max_radius=(int)r1;
	if(max_radius<r2)max_radius=(int)r2;
	if(max_radius<r3)max_radius=(int)r3;
	if(max_radius<r4)max_radius=(int)r4;
	max_radius=(int)(max_radius*mag);

	int w=max_radius*2, h=max_radius*2;
	x-=max_radius;y-=max_radius;


	float grope=(float)(1.0/mag);
	float radius=grope*w/2;
	float offsetX=-radius, offsetY=-radius;

	//‚Í‚Ý‚¾‚·•”•ª‚Í•`‰æ‚µ‚È‚¢
	if(x<0){offsetX-=x*grope;w+=x;x=0;}
	if(y<0){offsetY-=y*grope;h+=y;y=0;}
	if(x+w>pxwidth)w=pxwidth-x;
	if(y+h>pxheight)h=pxheight-y;

	px=px+pxwidth*y+x;
	px_skip=pxwidth-w;
	rgb_skip=(ima->w)-w2;
	a_skip=(ima->w)-w2;

	float rcos, rsin, rcosGrope, rsinGrope;
	rcos=cos(rot);
	rsin=sin(rot);
	rcosGrope=rcos*grope;
	rsinGrope=rsin*grope;


	int imgX=(int)(x2+cx+offsetX*rcos-offsetY*rsin);
	int imgY=(int)(y2+cy+offsetX*rsin+offsetY*rcos);
	int imgX2=imgX, imgY2=imgY;

	int *imgPointX2, *imgPointY2;
	int *horizontalX2, *horizontalY2, *verticalX2, *verticalY2;
	Uint32 *ima_rgb2 = ima_rgb+(int)imgY*(ima->w)+(int)imgX;
	Uint8 *ima_a2 = ima_a+(int)imgY*(ima->w)+(int)imgX;
	Uint32 *ima_rgb3=ima_rgb2;
	Uint8 *ima_a3=ima_a2;

	double imgX3=(int)imgX, imgY3=(int)imgY;
	imgPointX2=imgPointX;
	horizontalX2=horizontalX;
	horizontalY2=horizontalY;
	for(int i=0 ; i<w ; i++){
		*imgPointX2=(int)imgX3+(int)imgY3*(ima->w);
		*horizontalX2=(int)imgX3;
		*horizontalY2=(int)imgY3;
		imgX3+=rcosGrope;
		imgY3+=rsinGrope;
		imgPointX2++;horizontalX2++;horizontalY2++;
	}
	imgPointX2=&imgPointX[w-1];
	horizontalX2=&horizontalX[w-1];
	horizontalY2=&horizontalY[w-1];
	for(int i=w-1 ; i>0 ; i--){
		*imgPointX2-=*(imgPointX2-1);
		*horizontalX2-=*(horizontalX2-1);
		*horizontalY2-=*(horizontalY2-1);
		imgPointX2--;horizontalX2--;horizontalY2--;
	}
	imgPointX[0]=0;horizontalX[0]=0;horizontalY[0]=0;

	imgX3=(int)imgX, imgY3=(int)imgY;
	imgPointY2=imgPointY;
	verticalX2=verticalX;
	verticalY2=verticalY;
	for(int i=0 ; i<h ; i++){
		*imgPointY2=(int)imgX3+(int)imgY3*(ima->w);
		*verticalX2=(int)imgX3;
		*verticalY2=(int)imgY3;
		imgX3-=rsinGrope;
		imgY3+=rcosGrope;
		imgPointY2++;verticalX2++;verticalY2++;
	}
	imgPointY2=&imgPointY[h-1];
	verticalX2=&verticalX[h-1];
	verticalY2=&verticalY[h-1];
	for(int i=h-1 ; i>0 ; i--){
		*imgPointY2-=*(imgPointY2-1);
		*verticalX2-=*(verticalX2-1);
		*verticalY2-=*(verticalY2-1);
		imgPointY2--;verticalX2--;verticalY2--;
	}
	imgPointY[0]=0;verticalX[0]=0;verticalY[0]=0;

	imgPointX2=imgPointX;
	imgPointY2=imgPointY;
	horizontalX2=horizontalX;
	horizontalY2=horizontalY;
	verticalX2=verticalX;
	verticalY2=verticalY;
	int i;
	for(int j=0 ; j<h ; j++){
		ima_rgb3+=*imgPointY2;
		ima_a3+=*imgPointY2;
		ima_rgb2=ima_rgb3;
		ima_a2=ima_a3;
		imgPointX2=imgPointX;
		horizontalX2=horizontalX;
		horizontalY2=horizontalY;
		imgX2+=*verticalX2;
		imgY2+=*verticalY2;
		imgX=imgX2;
		imgY=imgY2;

		i=0;
		while((imgX<x2 || imgY<y2 || imgX>=x2+w2 || imgY>=y2+h2) && i<w){
			imgX+=*horizontalX2;
			imgY+=*horizontalY2;
			ima_rgb2+=*imgPointX2;
			ima_a2+=*imgPointX2;
			imgPointX2++;
			horizontalX2++;
			horizontalY2++;
			px++;
			i++;
		}
		while(imgX>=x2 && imgY>=y2 && imgX<x2+w2 && imgY<y2+h2 && i<w){
			if(*ima_a2){
				*px=*ima_rgb2;
			}
			imgX+=*horizontalX2;
			imgY+=*horizontalY2;
			ima_rgb2+=*imgPointX2;
			ima_a2+=*imgPointX2;
			imgPointX2++;
			horizontalX2++;
			horizontalY2++;
			px++;
			i++;
		}
		px+=w-i;

		imgPointY2++;
		verticalX2++;
		verticalY2++;
		px+=px_skip;
	}

}

void rotateImage_x(SDL_Surface* scr, Image* ima, int x, int y, double rot, double mag, int x2, int y2, int cx, int cy, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	SDL_LockSurface(scr);
	if(mag==1){
		rotateImage(scr, ima, x, y, rot, x2, y2, cx, cy, w2, h2, a);
	}else{
		__rotateImage_x((Uint32*)scr->pixels, scr->w, scr->h, ima, x, y, (float)rot, (float)mag, x2, y2, cx, cy, w2, h2, a);
		SDL_UnlockSurface(scr);
	}
}

void rotateImage_x(Image* scr, Image* ima, int x, int y, double rot, double mag, int x2, int y2, int cx, int cy, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	if(mag==1){
		rotateImage(scr, ima, x, y, rot, x2, y2, cx, cy, w2, h2, a);
	}else{
		__rotateImage_x(scr->RGB, scr->w, scr->h, ima, x, y, (float)rot, (float)mag, x2, y2, cx, cy, w2, h2, a);
	}
}

void __fillRect(Uint8* px, int pxwidth, int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B, int a){
	if(a==0)return;

	Uint16 px_skip;
	px=px+(pxwidth*y+x)*4;
	px_skip=(pxwidth-w)*4;

	Uint8 R2,B2;
	if(ABGR){
		R2=R;B2=B;
	}else{
		R2=B;B2=R;
	}

	if(a==255){
		Uint32 color=setRGB(R,G,B);
		for(int j=0 ; j<h ; j++){
			for(int i=0 ; i<w ; i++){
				*(Uint32 *)px=color;
				px+=4;
			}
			px+=px_skip;
		}
	}
	else if(a==128){
		for(int j=0 ; j<h ; j++){
			for(int i=0 ; i<w ; i++){
				*px = (R2+*px) >> 2;
				px++;
				*px = (G+*px) >> 2;
				px++;
				*px = (B2+*px) >> 2;
				px++;px++;
			}
			px+=px_skip;
		}
	}else{
		for(int j=0 ; j<h ; j++){
			for(int i=0 ; i<w ; i++){
				*px+= a*(R2-*px) >> 8;
				px++;
				*px+= a*(G-*px) >> 8;
				px++;
				*px+= a*(B2-*px) >> 8;
				px++;px++;
			}
			px+=px_skip;
		}
	}
}

void fillRect(SDL_Surface* scr, int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B, int a){
	if(scr==NULL)return;
	int x2=0,y2=0;
	clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w,&h,&w,&h,&a);
	SDL_LockSurface(scr);
	__fillRect((Uint8*)scr->pixels, scr->w, x, y, w, h, R, G, B, a);
	SDL_UnlockSurface(scr);
}

void fillRect(Image* scr, int x, int y, int w, int h, Uint8 R, Uint8 G, Uint8 B, int a){
	if(scr==NULL)return;
	int x2=0,y2=0;
	clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w,&h,&w,&h,&a);
	__fillRect((Uint8*)scr->RGB, scr->w, x, y, w, h, R, G, B, a);
}

void __illuminateImage(Uint8* px, int pxwidth, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int a){
	if(a<0)return;
	if(a>255)a=255;

	Uint8 *rgb=(Uint8*)ima->RGB;
	Uint8 *alpha=(Uint8*)ima->A;
	Uint16 px_skip, rgb_skip, alpha_skip;

	px=px+(pxwidth*y+x)*4;
	px_skip=(pxwidth-w2)*4;
	rgb=rgb+(y2*(ima->w)+x2)*4;
	rgb_skip=((ima->w)-w2)*4;
	alpha=alpha+y2*(ima->w)+x2;
	alpha_skip=(ima->w)-w2;

	if(a==255){
		for(int j=0 ; j<h2 ; j++){
			for(int i=0 ; i<w2 ; i++){
				if(*(Uint8 *)alpha){
					if(*rgb+*px<256)*px+=*rgb;
					else *px=255;
					px++;rgb++;
					if(*rgb+*px<256)*px+=*rgb;
					else *px=255;
					px++;rgb++;
					if(*rgb+*px<256)*px+=*rgb;
					else *px=255;
					px++;px++;rgb++;rgb++;
				}else{
					px+=4;	
					rgb+=4;
				}
				alpha++;
			}
			px+=px_skip;
			rgb+=rgb_skip;
			alpha+=alpha_skip;
		}
	}else{
		Uint8 col;
		for(int j=0 ; j<h2 ; j++){
			for(int i=0 ; i<w2 ; i++){
				if(*(Uint8 *)alpha){
					col=(*rgb*a)>>8;
					if(col+*px<256)*px+=col;
					else *px=255;
					px++;rgb++;
					col=(*rgb*a)>>8;
					if(col+*px<256)*px+=col;
					else *px=255;
					px++;rgb++;
					col=(*rgb*a)>>8;
					if(col+*px<256)*px+=col;
					else *px=255;
					px++;px++;rgb++;rgb++;
				}else{
					px+=4;	
					rgb+=4;
				}
				alpha++;
			}
			px+=px_skip;
			rgb+=rgb_skip;
			alpha+=alpha_skip;
		}
	}
}

void illuminateImage(SDL_Surface* scr, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
	SDL_LockSurface(scr);
	__illuminateImage((Uint8*)scr->pixels, scr->w, ima, x, y, x2, y2, w2, h2, a);
	SDL_UnlockSurface(scr);
}

void illuminateImage(Image* scr, Image* ima, int x, int y, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
	__illuminateImage((Uint8*)scr->RGB, scr->w, ima, x, y, x2, y2, w2, h2, a);
}

void __illuminateImage_x(Uint8* px, int pxwidth, int pxheight, Image* ima, int x, int y, double mag, int x2, int y2, int w2, int h2, int a){
	if(a<0)a=0;
	if(a>255)a=255;

	Uint8 *ima_a2,*ima_rgb2;
	Uint8 *ima_a = (Uint8*)ima->A;
	Uint8 *ima_rgb = (Uint8*)ima->RGB;
	Uint16 px_skip, rgb_skip, a_skip;
	short R,G,B;
	double x_pitch, y_pitch;
	double x3=x2, y3=y2, w3=w2, h3=h2; // double‚É‚·‚é

	if(a<0)a=0;
	if(a>255)a=255;

	//‚Í‚Ý‚¾‚·•”•ª‚Í•`‰æ‚µ‚È‚¢
	if(x3<0){w3+=x3;x3=0;}
	if(y3<0){h3+=y3;y3=0;}
	if(x3+w3>(ima->w))w3=(ima->w)-x3;
	if(y3+h3>(ima->h))h3=(ima->h)-y3;

	int w=(int)(w3*mag);
	int h=(int)(h3*mag);

	//‚Í‚Ý‚¾‚·•”•ª‚Í•`‰æ‚µ‚È‚¢
	if(x<0){x3-=x/mag;w+=x;x=0;}
	if(y<0){y3-=y/mag;h+=y;y=0;}
	if(x+w>pxwidth)w=pxwidth-x;
	if(y+h>pxheight)h=pxheight-y;


	px=px+(pxwidth*y+x)*4;
	px_skip=(pxwidth-w)*4;

	rgb_skip=((ima->w)-(int)w3)*4;
	a_skip=(ima->w)-(int)w3;

	double grope2=1.0/mag;
	y_pitch=y3;

	for(int j=0 ; j<h ; j++){
		x_pitch=x3;
		for(int i=0 ; i<w ; i++){
			ima_rgb2=ima_rgb+((int)y_pitch*(ima->w)+(int)x_pitch)*4;
			ima_a2=ima_a+((int)y_pitch*(ima->w)+(int)x_pitch);
			if(*ima_a2){
				R=( ((*ima_rgb2)*a) >> 8 ) + *px;
				G=( ((*(ima_rgb2+1)*a)) >> 8 ) + *(px+1);
				B=( ((*(ima_rgb2+2)*a)) >> 8 ) + *(px+2);
				if(R>255)*px=255;
				else *px=(Uint8)R;
				if(G>255)*(px+1)=255;
				else *(px+1)=(Uint8)G;
				if(B>255)*(px+2)=255;
				else *(px+2)=(Uint8)B;
			}
			x_pitch+=grope2;
			px+=4;
		}
		y_pitch+=grope2;
		px+=px_skip;
	}
}

void illuminateImage_x(SDL_Surface* scr, Image* ima, int x, int y, double mag, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	if(mag==1){
		clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
		illuminateImage(scr, ima, x, y, x2, y2, w2, h2, a);
	}else{
		SDL_LockSurface(scr);
		__illuminateImage_x((Uint8*)scr->pixels, scr->w, scr->h, ima, x, y, mag, x2, y2, w2, h2, a);
		SDL_UnlockSurface(scr);
	}
}

void illuminateImage_x(Image* scr, Image* ima, int x, int y, double mag, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || ima==NULL)return;
	if(mag==1){
		clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w2,&h2,&ima->w,&ima->h,&a);
		illuminateImage(scr, ima, x, y, x2, y2, w2, h2, a);
	}else{
		__illuminateImage_x((Uint8*)scr->RGB, scr->w, scr->h, ima, x, y, mag, x2, y2, w2, h2, a);
	}
}

void __drawLightBall(Uint8* px, int pxwidth, int x, int y, int cx, int r, int cy, int w, int h, Uint8 R, Uint8 G, Uint8 B){
	Uint16 px_skip;
	px=px+(pxwidth*y+x)*4;
	px_skip=(pxwidth-w)*4;
	cx-=x;
	cy-=y;

	double a;
	for(int j=0 ; j<h ; j++){
		for(int i=0 ; i<w ; i++){
			a=1.0-sqrt((cx-i)*(cx-i)+(cy-j)*(cy-j))/r;
			if(a>0){
				*(Uint32 *)px=setRGB((int)(R*a),(int)(G*a),(int)(B*a));
			}
			px+=4;
		}
		px+=px_skip;
	}
}

void drawLightBall(SDL_Surface* scr, int x, int y, int w, int R, int G, int B) {
	if(scr==NULL)return;
	int x2=0,y2=0,h=w,cx=x+w/2,cy=y+h/2,r=w/2,a=0;
	clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w,&h,&w,&h,&a);
	SDL_LockSurface(scr);
	__drawLightBall((Uint8*)scr->pixels, scr->w, x, y, cx, cy, r, w, h, R, G, B);
	SDL_UnlockSurface(scr);
}

void drawLightBall(Image* scr, int x, int y, int w, int R, int G, int B) {
	if(scr==NULL)return;
	int x2=0,y2=0,h=w,cx=x+w/2,cy=y+h/2,r=w/2,a=0;
	clipping(&x,&y,&scr->w,&scr->h,&x2,&y2,&w,&h,&w,&h,&a);
	__drawLightBall((Uint8*)scr->RGB, scr->w, x, y, cx, cy, r, w, h, R, G, B);
}
