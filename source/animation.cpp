#include "animation.h"

// opening.png
void drawOpeningAnim(SDL_Surface* scr, int cn){
	int a;
	if(cn>=1120 && cn<1350)fillRect(scr,0,40,640,400,0,0,0,255);
	else{
		for(int i=0 ; i<400 ; i++)fillRect(scr,0,40+i,640,1,0,0,i/4,255);
	}
	if(cn<70){
		a=cn;
		drawImage_x(scr,img.back,140,286+a/5,(160.0+a/4)/160.0,2120,400,320,200,255);
		drawImage_x(scr,img.back,80-a,120,(50.0+a/3)/50.0,2120,0,100,400,255);
		drawImage_x(scr,img.back,480+a,120,(50.0+a/3)/50.0,2220,0,100,400,255);
		drawImage_x(scr,img.back,-a*2,40-a*2,(50.0+a/2)/50.0,1920,0,100,600,255);
		drawImage_x(scr,img.back,540+a*2,40-a*2,(50.0+a/2)/50.0,2020,0,100,600,255);
		drawImage_x(scr,img.back,140-a,200-a,(160.0-a/3)/160.0,2320,200+((a/2)%2)*100,320,100,255);
		drawImage_x(scr,img.back,140-a,200-a,(160.0-a/3)/160.0,2320,0,320,200,255);
	}
	else if(cn<120){
		a=cn-70;
		drawImage_x(scr,img.back,140,300-a*2,(160.0+(a+70)/4)/160.0,2120,400,320,200,255);
		drawImage_x(scr,img.back,80-(a+70),120-a*2,(50.0+(a+70)/3)/50.0,2120,0,100,400,255);
		drawImage_x(scr,img.back,480+(a+70),120-a*2,(50.0+(a+70)/3)/50.0,2220,0,100,400,255);
		drawImage_x(scr,img.back,-(a+70)*2,-100,(50.0+(a+70)/2)/50.0,1920,0,100,600,255);
		drawImage_x(scr,img.back,540+(a*2+140),26,(50.0+(a+70)/2)/50.0,2020,0,100,600,255);
		drawImage_x(scr,img.back,70+a*2,130+a*4,(137.0-a/2)/160.0,2320,200+(((a+70)/2)%2)*100,320,100,255);
		drawImage_x(scr,img.back,70+a*2,130+a*4,(137.0-a/2)/160.0,2320,0,320,200,255);
	}
	else if(cn<190){
		a=cn-120;
		drawImage(scr,img.back,0,40,0,0,640,400,255);
		if(cn%2==0)drawImage(scr,img.back,330,a*2+60,1920,600,320,40,255);
		drawImage(scr,img.back,280,a*2+100,1920,640,320,160,255);
		if(a>=30 && a<50){
			drawImage(scr,img.back,360-(a-30)*4,360-(a-30),2240,640,200,80,128-(a-30)*6);
			drawImage(scr,img.back,360+(a-30)*4,360-(a-30),2240,720,200,80,128-(a-30)*6);
		}
	}
	else if(cn<270){
		a=cn-190;
		if(a<40)drawImage(scr,img.back,0,40,0,400,640,400,255);
		else drawImage(scr,img.back,0,40,0,800,640,400,255);
		fillRect(scr,0,40,640,400,0,0,0,abs(40-(cn%8)*10));
	}
	else if(cn<330){
		int a=cn-270;
		for(int i=0 ; i<3 ; i++){
			drawImage(scr,img.back,-a*4+160*i,a*12+40-400*i,0,1200,640,400,255);
		}
		drawImage(scr,img.back,320,240,1920+abs(2-((a/8)%4))*320,1100,320,200,255);
		drawImage(scr,img.back,0,140,1920+abs(2-((a/8)%4))*320,800,320,300,255);
	}
	else if(cn<400){
		int a=cn-330;
		drawImage(scr,img.back,0,40,0,1600,640,400,255);
		if(a<25){
			drawImage(scr,img.back,260,80,2120,1300,120,200,255);
			drawImage(scr,img.back,240,180,2240,1300,200,100,255);
			drawImage(scr,img.back,230-a*4,80,1920,1300,100,200,255);
			drawImage(scr,img.back,330+a*4,80,2020,1300,100,200,255);
		}else{
			drawImage(scr,img.back,130,80,1920,1300,100,200,255);
			drawImage(scr,img.back,430,80,2020,1300,100,200,255);
			drawImage(scr,img.back,260-(a-25)*2,80+(a-25)*4-(5-abs(5-(a/2)%10))*12,2120,1300,120,200,255);
			drawImage(scr,img.back,260+(a-25)*2,180+(a-25)*4,2240,1300,200,100,255);
		}
	}
	else if(cn<500){
		a=cn-400;
		drawImage(scr,img.back,0,240,0,2200,640,200,255);
		if(a<80)drawImage(scr,img.back,a,240-a*2,1920,1500,320,200,255);
		else drawImage(scr,img.back,80,240,1920,1660,240,40,255);
		drawImage(scr,img.back,0,40,0,2000,640,200,255);
		drawImage(scr,img.back,320,240,320,2200,120,40,255);
		if(a<70)drawImage(scr,img.back,320,320,2440,1300,160,100,255);
		else drawImage(scr,img.back,320+(a-70)*2,320-(a-70)*4,2440,1300,160,100,255);
		if(a<30)drawImage(scr,img.back,0,240,2240+(a/10)*200,1500,200,200,255);
		else if(a<70)drawImage(scr,img.back,(a-30)*4,240,2640,1500,200,200,255);
		else drawImage(scr,img.back,160,240,2640-((a-70)/10)*200,1500,200,200,255);
	}
	else if(cn<520){
		a=cn-500;
		drawImage(scr,img.back,0,40,0,2400,640,400,255);
		drawImage(scr,img.back,280+a*20,60-a*8,2560,1700,200,100,255);
	}
	else if(cn<540){
		a=cn-520;
		drawImage(scr,img.back,a*48-160,140-a*8,2560,1700,200,100,255);
		drawImage(scr,img.back,200,40,1920,1700,240,400,255);
	}
	else if(cn<570){
		a=cn-540;
		drawImage(scr,img.back,120,40,1920,2100,400,400,255);
		if(a<10)drawImage(scr,img.back,a*20-160,240-a*8,2560,1700,200,100,255);
		else if(a<20)drawImage(scr,img.back,160,160,2940+((a/2)%2)*120,2500,120,120,255);
	}
	else if(cn<600){
		a=cn-570;
		drawImage(scr,img.back,260,40,2160,1700,120,400,255);
		if(a>=10)drawImage(scr,img.back,180,240,2280,1700+((a/4)%2)*200,280,200,255);
	}
	else if(cn<680){
		a=cn-600;
		if(a>=40)fillRect(scr,160,80,320,200,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,0,40,0,2800,640,400,255);
		if(a<40)drawImage(scr,img.back,160,80,1920,2500,320,200,255);
		if(a<40){
			drawImage(scr,img.back,220+(5-abs(5-(a%10)))*2,360,2560,1820,70,100,255);
			drawImage(scr,img.back,290-(5-abs(5-((a/3)%10)))*2,360,2630,1820,60,100,255);
			drawImage(scr,img.back,350,360+(5-abs(5-(a%10)))*2,2690,1820,70,100,255);
		}
		else if(a<50){
			drawImage(scr,img.back,220,360-(5-abs(5-(a-40)))*4,2560,1820,70,100,255);
			drawImage(scr,img.back,290,360-(5-abs(5-(a-40)))*4,2630,1820,60,100,255);
			drawImage(scr,img.back,350,360-(5-abs(5-(a-40)))*4,2690,1820,70,100,255);
		}else{
			drawImage(scr,img.back,220,360,2560,1820,70,100,255);
			drawImage(scr,img.back,290,360,2630,1820,60,100,255);
			drawImage(scr,img.back,350,360,2690,1820,70,100,255);
		}
		if(a<40)drawImage(scr,img.back,380+a*2,240,2320,2100,200,200,255);
		else drawImage(scr,img.back,440,240,2320,2300,200,200,255);
		if(a<50)drawImage(scr,img.back,0,240,2520,2100,200,200,255);
		else drawImage(scr,img.back,0,240,2520,2300,200,200,255);
	}
	else if(cn<760){
		a=cn-680;
		if(a<15)drawImage(scr,img.back,a*52-640,40,640,400,640,400,255);
		else drawImage(scr,img.back,140,40,640,400,640,400,255);
		if(a>=25){
			fillRect(scr,490,280,120,80,0,0,64,255);
			fillRect(scr,490,280,120,80,255,255,0,128+abs(5-(a%10))*25);
		}
		if(a>=35)drawImage(scr,img.back,450,240,2400,2500,160,160,255);
		else if(a>=25)drawImage(scr,img.back,450,240,2240,2500,160,160,255);
		drawImage(scr,img.back,0,40,640,0,640,400,255);
		if(a>=40)drawImage(scr,img.back,0,140,2240,2700,320,300,255);
		else drawImage(scr,img.back,0,140,1920,2700,320,300,255);
	}
	else if(cn<800){
		a=cn-760;
		drawImage(scr,img.back,0,40,640,800,640,400,255);
		if(a>=15){
			drawImage(scr,img.back,200,140,2560,2700,320,300,255);
			drawImage(scr,img.back,200,200,1920,3000,320,120,abs(5-(a%10))*25);
		}
	}
	else if(cn<900){
		a=cn-800;
		if(a>=40)fillRect(scr,0,40,520,400,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,0,40,640,1200,640,400,255);
		if(a<40)drawImage(scr,img.back,0,40,2640,0,520,400,255);
		if(a>=50)drawImage(scr,img.back,210,180,2360,3000,120,120,255);
		else if(a>=40)drawImage(scr,img.back,210,180,2240,3000,120,120,255);
		if(a>=65)drawImage(scr,img.back,380,280,2600,3000,120,160,255);
		else if(a>=55)drawImage(scr,img.back,380,280,2480,3000,120,140,255);
		if(a>=60)fillRect(scr,0,40,640,400,255,255,0,(a-60)*6);
	}
	else if(cn<1000){
		a=cn-900;
		if(a>=40)fillRect(scr,0,40,320,280,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,0,40,640,1600,640,400,255);
		if(a<60)drawImage(scr,img.back,400,40,2880,1040,120,160,255);
		else if(a<65)drawImage(scr,img.back,400,40,3000,1040,120,160,255);
		else if(a<80)drawImage(scr,img.back,400,40,3120,1040,120,160,255);
		else drawImage(scr,img.back,400+(a-80)*16,40-(a-80)*6,3120,1040,120,160,255);
		if(a<40){
			drawImage(scr,img.back,100+a*2,300-a*4,2880+abs(2-((a/8)%4))*120,800,120,140,255);
			drawImage(scr,img.back,320-a*2,200+a*4,2880+abs(2-((a/8)%4))*120,940,120,100,255);
		}else{
			drawImage(scr,img.back,140+(a-40)*20,220-(a-40)*4,3240,800,120,140,255);
			drawImage(scr,img.back,280+(a-40)*20,280-(a-40)*4,3240,940,120,100,255);
		}
	}
	else if(cn<1100){
		a=cn-1000;
		fillRect(scr,0,40,640,400,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,0,40,640,2000,640,400,255);
		drawImage_x(scr,img.back,40-a,120+a/2,45/60.0,2760,400,120,400,355);
		drawImage_x(scr,img.back,40-a*2,140-(int)(sin(a/30.0)*80),120/160.0,2440,400,320,400,255);
		drawImage_x(scr,img.back,130-a,120+a/2,75/100.0,2880,400,200,400,255);
		drawImage(scr,img.back,320-(int)(a*1.4),80+a/2,2760,400,120,400,255);
		drawImage(scr,img.back,320-a*4,120-(int)(sin(a/30.0)*80),2440,400,320,400,255);
		drawImage(scr,img.back,440-(int)(a*1.4),80+a/2,2880,400,200,400,255);
		if((a>=30 && a<40 || a>=80) && a%10>=5)fillRect(scr,0,40,640,400,255,255,0,(5-a%5)*30);
	}
	else if(cn<1120){
		a=cn-1100;
		drawImage(scr,img.back,0,340,2840,1600,440,100,255);
		drawImage(scr,img.back,360,40,2880,1200,280,400,255);
		drawImage_x(scr,img.back,160+a*8,360-a*8,(a*4.0)/70.0,2600,1300,140,140,255);
	}
	else if(cn<1170){
		a=cn-1120;
		for(int i=3 ; i>=0 ; i--){
			drawImage_x(scr,img.back,300-(a-i*10)*24,240-(a-i*10)*10,(a-i*10)*10/200.0,2760,1700,400,320,255);
			drawImage_x(scr,img.back,300+(a-i*10-5)*4,240-(a-i*10-5)*10,(a-i*10-5)*10/200.0,3160,1700,400,320,255);
		}
		if(a<30)drawImage_x(scr,img.back,250,-100+a*6,1,2600+((a/5)%2)*140,1300,140,140,255);
		else drawImage_x(scr,img.back,250+(a-30)*6,40+(a-30)*20,(70.0-(a-30)*7)/70.0,2600+((a/5)%2)*140,1300,140,140,255);
	}
	else if(cn<1200){
		a=cn-1170;
		drawImage(scr,img.back,0,40,640,2400,640,400,255);
		drawImage(scr,img.back,350-a*12,286-a*12,3590-a*12,3046-a*12,a*24,a*24,255);
	}
	else if(cn<1250){
		a=cn-1200;
		if(loadtime<1200){
			drawImage(scr,img.back,0,40,640,2400,640,400,255);
			drawImage(scr,img.back,0,40,3240,2800,640,400,255);
			drawImage(img.scr2,scr,0,0,0,40,640,400,255);
			loadtime=cn;
		}
		drawImage_x(scr,img.scr2,-a*16,40-a*8,(320.0+a*40)/320.0,0,0,640,400,255);
	}
	else if(cn<1350){
		a=cn-1250;
		int dx=(int)(cos(a*0.3)*(100-a)/5.0);
		int dy=(int)(sin(a*0.3)*(100-a)/5.0);
		if(a<20){
			drawImage_x(scr,img.back,240+dx,380-a*2+dy,0.5,2920,2020,320,240,255);
			drawImage_x(scr,img.back,280+dx,360-a*2+dy,40/320.0,640,2800,640,400,255);
			drawImage(scr,img.back,180-a*2+dx,260+a*4+dy,2720,2020,200,240,255);
			drawImage(scr,img.back,300+a*2+dx,260+a*4+dy,2720,2020,200,240,255);
			drawImage(scr,img.back,40-a*2+dx,240+a*2+dy,2720,2020,200,240,255);
			drawImage(scr,img.back,440+a*2+dx,240+a*2+dy,2720,2020,200,240,255);
		}
		else if(a<40){
			drawImage_x(scr,img.back,240+dx,340+dy,0.5,2920,2020,320,240,255);
			drawImage_x(scr,img.back,280+dx,320+dy,40/320.0,640,2800,640,400,255);
			drawImage(scr,img.back,140+dx,340+dy,2720,2020,200,240,255);
			drawImage(scr,img.back,340+dx,340+dy,2720,2020,200,240,255);
			drawImage(scr,img.back,0+dx,280+dy,2720,2020,200,240,255);
			drawImage(scr,img.back,480+dx,280+dy,2720,2020,200,240,255);
		}else{
			drawImage_x(scr,img.back,240-(a-40)*24+dx,340-(a-40)*6+dy,(80.0+(a-40)*24)/160.0,2920,2020,320,240,255);
			drawImage_x(scr,img.back,280-(a-40)*12+dx,320-(a-40)*10+dy,(40.0+(a-40)*12)/320.0,640,2800,640,400,255);
			drawImage_x(scr,img.back,140-(a-40)*6+dx,340+(a-40)*4+dy,(100.0+(a-40))/100.0,2720,2020,200,240,255);
			drawImage_x(scr,img.back,340+(a-40)*4+dx,340+(a-40)*4+dy,(100.0+(a-40))/100.0,2720,2020,200,240,255);
			drawImage_x(scr,img.back,-(a-40)*14+dx,280-(a-40)*6+dy,(100.0+(a-40)*2)/100.0,2720,2020,200,240,255);
			drawImage_x(scr,img.back,480+(a-40)*10+dx,280-(a-40)*6+dy,(100.0+(a-40)*2)/100.0,2720,2020,200,240,255);
		}
	}
	else if(cn<1450){
		a=cn-1350;
		drawImage(scr,img.back,0,220,2720,2280,640,220,255);
		if(a<50){
			drawImage(scr,img.back,100,300,2640,2500,100,100,255);
			drawImage(scr,img.back,320,180,2640,2600,100,100,255);
			drawImage(scr,img.back,480,340,2640,2600,100,100,255);
			drawImage(scr,img.back,(a-40)*26,(a-40)*16,2600,1300,140,140,255);
		}else{
			drawImage(scr,img.back,100-(a-50)*10,300-(a-50)*6,2740+((a/3)%2)*100,2500,100,100,255);
			drawImage(scr,img.back,320+(a-50)*10,180-(a-50)*6,2740+((a/3)%2)*100,2600,100,100,255);
			drawImage(scr,img.back,480+(a-50)*10,340-(a-50)*6,2740+((a/3)%2)*100,2600,100,100,255);
			drawImage(scr,img.back,280,180,2280,1700+((a/3)%2)*200,280,200,255);
		}
	}
	else if(cn<1500){
		a=cn-1450;
		fillRect(scr,120,140,400,240,255,255,0,128+abs(5-(a%10))*25);
		fillRect(scr,130,230,70,70,0,0,0,255);
		drawImage(scr,img.back,0,40,1280,0,640,400,255);
		drawImage(scr,img.back,420,60,2940+((a/3)%2)*120,2500,120,120,255);
	}
	else if(cn<1560){
		a=cn-1500;
		drawImage(scr,img.back,0,40,1280,400,640,400,255);
		if(a<15)drawImage(scr,img.back,520,a*6,2940+((a/3)%2)*120,2500,120,120,255);
		if(a>=30 && a<36)drawImage(scr,img.back,140-(a-30)*10,120+(a-30)*10,2940+((a/3)%2)*120,2500,120,120,255);
		if(a>=50)drawImage(scr,img.back,0,200,2940+((a/3)%2)*120,2500,120,120,255);
	}
	else if(cn<1640){
		a=cn-1560;
		drawImage(scr,img.back,0,40,1280,800,640,250,255);
		for(int i=0 ; i<3 ; i++)drawImage(scr,img.back,60-a*4+i*160,-10-a*6+i*300,2880,2700,200,300,128);
		drawImage(scr,img.back,0,290,1280,1050,640,150,255);
		if(a%20<10)drawImage(scr,img.back,440,300,3080,2620,120,100,255);
		if(a>=20 && a<40)drawImage(scr,img.back,460,180,2940+((a/3)%2)*120,2500,120,120,255);
	}
	else if(cn<1700){
		a=cn-1640;
		fillRect(scr,120,140,400,240,255,255,0,128+abs(5-(a%10))*25);
		if(a<30)fillRect(scr,130,230,70,70,0,0,0,255);
		drawImage(scr,img.back,0,40,1280,0,640,400,255);
		drawImage(scr,img.back,420,60,2940+((a/3)%2)*120,2500,120,120,255);
	}
	else if(cn<1760){
		a=cn-1700;
		fillRect(scr,160,120,200,120,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,0,40,1280,1200,640,400,255);
		if(a%6<3)drawImage(scr,img.back,380,200,3080,2720,140,240,255);
	}
	else if(cn<1860){
		a=cn-1760;
		if(a<50)drawImage(scr,img.back,240,180,3040,3000,160,80,55+abs(5-(a%10))*40);
		else drawImage(scr,img.back,480,240,3040,3000,160,100,55+abs(5-(a%10))*40);
		drawImage(scr,img.back,300,200,2720,3000,320,160,255);
		drawImage(scr,img.back,100,160,2720,3000,320,160,255);
		for(int i=0 ; i<4 ; i++){
			drawImage_x(scr,img.back,80-i*160+a*4,360-i*80+a,(200.0+i*20-a)/140.0,3160,0,280,200,255);
		}
		if(a<50)drawImage(scr,img.back,440,260+abs(10-a%20)*2,3160,200,200,200,255);
		else if(a<70)drawImage(scr,img.back,440,260+abs(10-a%20)*2,3360,200,200,200,255);
		else drawImage(scr,img.back,440,260+abs(10-a%20)*2,3560,200,200,200,255);
	}
	else if(cn<1940){
		a=cn-1860;
		drawImage(scr,img.back,0,40,1280,1600,640,400,255);
		if(a%8<4)drawImage(scr,img.back,160,40,3080,400,320,400,255);
		if(a>=30)drawImage(scr,img.back,320,40,3440,0,320,200,255);
	}
	else if(cn<2000 || cn>=2080 && cn<2180){
		a=cn-1940;
		drawImage(scr,img.back,300-a,80,3760,0,140,300,255);
		drawImage(scr,img.back,310-a,100,2940+((a/3)%2)*120,2500,120,120,255);
		for(int i=0 ; i<5 ; i++){
			drawImage(scr,img.back,i*240-a*2,280,3760,300,240,100,255);
		}
		//ビル１
		fillRect(scr,200-a*4,140,320,300,0,0,64,255);
		fillRect(scr,200-a*4,150,320,290,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,200-a*4,40,3400,400,320,400,255);
		//ビル２
		fillRect(scr,700-a*4,340,320,100,0,0,64,255);
		fillRect(scr,700-a*4,340,320,100,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,700-a*4,340,3400,700,320,100,255);
		fillRect(scr,700-a*4,140,200,200,0,0,64,255);
		fillRect(scr,700-a*4,150,200,190,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,700-a*4,40,3400,400,200,300,255);
		if(a<190){
			fillRect(scr,900-a*4,140,120,200,0,0,64,255);
			fillRect(scr,900-a*4,150,120,190,255,255,0,128+abs(5-(a%10))*25);
			drawImage(scr,img.back,940-a*4,240,3360+abs(2-(a/10)%4)*40,1100,40,60,255);
			drawImage(scr,img.back,900-a*4,40,3600,400,120,300,255);
		}else{
			drawImage(scr,img.back,940-a*4,240,3360+abs(2-(a/10)%4)*40,1100,40,60,255);
			drawImage(scr,img.back,900-a*4,140,3720,500,120,200,255);
			drawImage(scr,img.back,960-a*4+(a-190)*2,180+(a-190)*10,3720,700,80,80,255);
			drawImage(scr,img.back,960-a*4+(a-190)*6,180+(a-190)*(a-190)*2/5,3720,700,80,80,255);
			drawImage(scr,img.back,960-a*4+(a-190)*6,120+(a-200)*(a-200)*2/5,3720,700,80,80,255);
		}
		for(int i=0 ; i<4 ; i++){
			drawImage(scr,img.back,600-a*8+i*160,40-a*6+i*300,2880,2700,200,300,128);
		}
		//ビル３
		fillRect(scr,1200-a*4,140,320,300,0,0,64,255);
		fillRect(scr,1200-a*4,150,320,290,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,1200-a*4,40,3400,400,320,400,255);
		if(a<30)drawImage(scr,img.back,420-a*4,244,3720,400,80,60,255);
		else{
			drawImage(scr,img.back,420-a*4,244,3800,400,80,60,255);
			drawImage(scr,img.back,420-a*4+(a-30)*6,250+(a-30)*10,3880,400,120,120,255);
			drawImage(scr,img.back,420-a*4+(a-30)*4,250+(a-30)*4,3880,400,120,120,255);
			drawImage(scr,img.back,420-a*4+(a-30)*8,250+(a-30)*6,3880,400,120,120,255);
		}
	}
	else if(cn<2080){
		a=cn-2000;
		fillRect(scr,0,40,640,400,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,140,80,3360+abs(2-(a/10)%4)*160,800,160,300,255);
		drawImage(scr,img.back,0,40,1280,2000,640,400,255);
		if(a<30)drawImage(scr,img.back,480,220,3840,800,60,120,255);
		else if(a<40)drawImage(scr,img.back,480,220,3900,800,60,120,255);
		else drawImage(scr,img.back,480,220+(a-40)*8,3840,920,60,160,255);
		if(a<20 && a%10<5)fillRect(scr,0,40,640,400,255,255,0,128-(a%5)*25);
		if(a>=55)fillRect(scr,0,40,640,400,255,255,0,(a%5)*25);
	}
	else if(cn<2380){
		a=cn-2180;
		for(int i=0 ; i<400 ; i++){
			drawImage(scr,img.back,(int)(sin((a+i)/5.0)*8),i+40,1280,2400+i,640,1,255);
		}
		if(a>=170)drawImage(scr,img.back,300,40,3280+((a-170)/10)*220,1480,220,220,255);
		for(int i=0 ; i<5 ; i++){
			drawImage(scr,img.back,440+i*160-a*4,i*300-a*6,2880,2700,200,300,128);
			drawImage(scr,img.back,480+i*160-(a+50)*6,i*300-(a+75)*6,2880,2700,200,300,128);
		}
		drawImage_x(scr,img.back,20+abs(40-a%80)/2,240,(100.0+abs(40-a%80)/2)/140.0,3160+abs(2-(a/15)%4)*280,1200,280,280,255);
		drawImage_x(scr,img.back,400+abs(40-(a+10)%80)/2,200,(120.0+abs(40-(a+10)%80)/2)/140.0,3160+abs(2-(a/15)%4)*280,1200,280,280,255);
		drawImage_x(scr,img.back,140+abs(40-(a+30)%80)/2,160,(140.0+abs(40-(a+30)%80)/2)/140.0,3160+abs(2-(a/15)%4)*280,1200,280,280,255);
		if(a<120){
			fillRect(scr,0,40,640,400,0,0,0,(120-a)*2);
			drawImage(scr,img.back,240-a*2,200,750,520,240,240,(120-a)*2);
			if(a%6<3)drawImage(scr,img.back,240-a*2,200,3460,2290,220,240,(120-a));
			drawImage(scr,img.back,420-a*2,240,3360,2290,100,200,(120-a)*2);
		}
	}
	else if(cn<2600){
		a=cn-2380;
		drawImage(scr,img.back,200,60+a/6,3680,1880,240,240,(int)(a*1.4));
		drawImage(scr,img.back,80-a/6,120-a/3,3560,1880,120,240,(int)(a*1.4));
		drawImage(scr,img.back,440+a/6,120-a/3,3920,1880,120,240,(int)(a*1.4));
		drawImage(scr,img.back,250,140,3760,0,140,300,255);
		drawImage(scr,img.back,100,260-a/4,3680,2300+((count/10)%2)*100,220,100,255);
		drawImage(scr,img.back,320,260-a/4,3900,2300+((count/10)%2)*100,220,100,255);
		for(int i=0 ; i<3 ; i++)drawImage(scr,img.back,i*240,340,3760,300,240,100,255);
		if(a>=100)drawImage(scr,img.back,0,40,3240,2120,640,170,255);
		else if(a>=40)drawImage(scr,img.back,0,40,3240,2120,640,170,(a-40)*4);
		int x[30]={500,40,220,360,560,0,400,540,140,80,520,320,440,520,140,280,20,340,300,0,500,60,220,520,140,280,560,0,380,340};
		int y[30]={360,400,320,380,280,340,280,400,380,320,260,340,400,320,280,380,300,260,280,400,280,260,240,340,320,380,260,260,340,280};
		for(int i=0 ; i<4 ; i++){
			if(a/7-i<0 || a/7-i>=30)continue;
			drawImage(scr,img.back,x[a/7-i],y[a/7-i],3800-i*80,1800,80,80,255);
		}
	}
	else if(cn<2740){
		a=cn-2600;
		fillRect(scr,0,40,640,60,0,0,0,255);
		fillRect(scr,0,380,640,60,0,0,0,255);
		drawImage(scr,img.back,0,160,3180,2540,640,80,255);
		drawImage(scr,img.back,0,60-a*2/7,1280,2800,640,400,255);
		if(a>=30)drawImage(scr,img.back,140,240,3240,2620,360,180,255);
	}else{
		a=cn-2740;
		drawImage(scr,img.back,140,40,640,400,640,400,255);
		fillRect(scr,490,280,120,80,255,255,0,255);
		drawImage(scr,img.back,450,240,2400,2500,160,160,255);
		drawImage(scr,img.back,0,40,640,0,640,400,255);
		drawImage(scr,img.back,0,140,2240,2700,320,300,255);
		fillRect(scr,0,40,640,400,0,0,255,128);
		drawImage(scr,img.back,160,380,1920,3120,440,60,255);
		if(a<450){
			drawImage(scr,img.back,220,60,3820,2500,220,80,255);
			if(a<220)drawImage(scr,img.back,100,240,3600,2620,440,50,255);
			else{
				drawImage(scr,img.back,100,180,3600,2670,440,50,255);
				drawImage(scr,img.back,100,280,3600,2720,440,50,255);
			}
		}
	}
	if(cn>2020 && cn<2240)drawImage(scr,img.back,300,320,3560,1700,400,100,255);

	if(cn<300)drawImage(scr,img.back,540,60,2240,600,80,40,255);

	if(cn>50 && cn<200)drawImage(scr,img.back,320,100,3880,2800,240,50,255);
	if(cn>250 && cn<400)drawImage(scr,img.back,60,360,3880,2850,240,50,255);
	if(cn>750 && cn<900)drawImage(scr,img.back,60,100,3880,2900,240,50,255);
	if(cn>1050 && cn<1200)drawImage(scr,img.back,60,360,3880,2950,240,50,255);
	if(cn>1300 && cn<1450)drawImage(scr,img.back,60,100,3880,3000,240,50,255);
	if(cn>1650 && cn<1800)drawImage(scr,img.back,60,100,3880,3050,240,50,255);
	if(cn>2250 && cn<2400)drawImage(scr,img.back,320,100,3880,3100,240,50,255);

	fillRect(scr,0,0,640,40,0,0,0,255);
	fillRect(scr,0,440,640,40,0,0,0,255);
//	sprintf_s(str,"%d",cn);
//	TextOut(scr,0,0,str,5);
}

// scene0.png - scene6.png
void drawWeeklyComic(SDL_Surface* scr){
	fillRect(scr,0,0,640,480,0,0,0,255);
	if(gd.week==0){
		int a,b,c;
		if(count<200)a=count*2-60;
		else if(count<340)a=340;
		else a=count*2-340;
		if(count<200)b=((count/10)%2)*120;
		else if(count<220)b=240;
		else if(count<320)b=360;
		else if(count<340)b=240;
		else b=((count/5)%2)*120;
		if(count<200)c=150;
		else if(count<300)c=(int)(1.5*(300-count));
		else c=0;
		fillRect(scr,0,90,640,300,128,128,255,255);
		drawImage(scr,img.back,400,100,600,340,30,30,255);
		drawImage(scr,img.back,260-count/10,90,240,340,200,100,255);
		drawImage(scr,img.back,0,90,0,40,640,300,255);
		drawImage(scr,img.back,a,280,((count/10)%2)*120,340,120,120,255);
		drawImage(scr,img.back,a,230,b,460,120,120,255);
		if(count>200&&count%20<10 || count>300)drawImage(scr,img.back,240,130,480,420,160,160,255);
		fillRect(scr,0,90,640,300,0,0,0,c);
	}
	else if(gd.week==1){
		fillRect(scr,0,90,640,300,192,192,192,255);
		drawImage(scr,img.back,0,90,((count/2)%160)*4,340+((count/2)%160)*2,640,300,64);
		drawImage(scr,img.back,0,90,((count/3)%160)*4,340+((count/3)%160)*2,640,300,64);
		drawImage(scr,img.back,0,90,0,40,640,300,255);
		if(count>=140 && count<200)drawImage(scr,img.back,188,250,640,150,104,140,255);
		if(count<100)drawImage(scr,img.back,60,220,880,230,50,80,255);
		else if(count<130)drawImage(scr,img.back,60+(count-100)*2,220+(count-100)*4,880,230,50,80,255);
		else if(count<200)drawImage(scr,img.back,120+(count-130)*2,340,880,230,50,80,255);
		if(count>=150 && count<200)drawImage(scr,img.back,292,250,744,150,36,140,255);
		drawImage(scr,img.back,0,240,640,0,640,150,255);
		if(count>=260 && count<300)drawImage(scr,img.back,0,260,1000,230,240,120,255);
		if(count>=300 && count<340)drawImage(scr,img.back,0,260,1000,350,240,120,255);
		if(count>500)drawImage(scr,img.back,0,260,1000,230+((count/10)%2)*120,240,120,255);
		if(count<340){
			if(count%40<20)drawImage(scr,img.back,490,290,780,150,100,80,255);
		}
		else if(count<440){
			if(count%40<20)drawImage(scr,img.back,490,290,880,150,100,80,255);
			else drawImage(scr,img.back,490,290,980,150,100,80,255);
			drawImage(scr,img.back,520,240,780,240,60,60,255);
		}else{
			if(count%40<20)drawImage(scr,img.back,490,290,1080,150,100,80,255);
			else drawImage(scr,img.back,490,290,1180,150,100,80,255);
			drawImage(scr,img.back,570,290,840,240,40,40,255);
		}
	}
	else if(gd.week==2){
		int a=0,b=240,sx,sy=130,cx,cy=190;
		int fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;

		cx=520-count/5;
		if(count>=100 && count<400)cy+=(count-100)/3;
		else if(count>=400 && count<500)cy+=(500-count)-abs(4-count%10)*6;
		if(count>=400 && count<450){a=240;cx-=(count-400)*2;}
		else if(count>=450 && count<500){a=360;cx+=(count-450)*2;}
		else a=((count/10)%2)*120;

		if(count>=200)sy+=(count-200)/3;
		if(count>=500)sy+=(count-500);
		if(sy<310)sx=80+count/5;
		else sx=160+count/5;

		if(count<100){
			fx1=640-count*4;
			fy1=320-count*count/50;
		}
		else if(count<180){
			fx1=240;
			fy1=120+(count-100)/2;
			b=400;
		}else{
			fx1=240-(count-180)*4;
			fy1=200-(count-180)*(count-180)/50;
		}
		fx2=800-count*4;
		fy2=300-count+abs(count%40-20)*2;
		fx3=920-count*4;
		fy3=540-count*2+abs((count+10)%40-20)*2;
		fx4=1400-count*4;
		fy4=800-count*2;
		test=fx2*10000+fy2;
		drawImage(scr,img.back,0,90,0,40,640,300,255);
		if(sy<310)drawImage(scr,img.back,sx,sy,0,560,160,80,255);
		else drawImage(scr,img.back,sx,sy,160,560,80,80,255);
		drawImage(scr,img.back,count/5-200,180,0,340,640,100,255);
		drawImage(scr,img.back,fx1,fy1,b+((count/10)%2)*80,560,80,80,255);
		drawImage(scr,img.back,fx2,fy2,480,440,60,60,255);
		drawImage(scr,img.back,fx3,fy3,480,440,60,60,255);
		drawImage(scr,img.back,fx4,fy4,480+((count/10)%2)*60,500,60,60,255);
		drawImage(scr,img.back,cx,cy,a,440,120,120,255);
		drawImage(scr,img.back,440-count/5,280,0,340,640,100,255);
	}
	else if(gd.week==3){
		int a=(count/10)%3;
		drawImage(scr,img.back,0,90,0,40,640,300,255);
		drawImage(scr,img.back,120,280,460,340+abs(2-(count/10)%4)*90,180,90,255);
		if(count<270)drawImage(scr,img.back,440,count*2/3,0,520+a*40,120,40,255);
		else if(count<350)drawImage(scr,img.back,440,180,0,520+a*40,120,40,255);
		if(count>=300 && count<350)drawImage(scr,img.back,446,194,0,340,100,180,255);
		else if(count>=350 && count<500){
			if(count%50<25){
				drawImage(scr,img.back,420,230,120+((count-325)/50)*80,520+a*80,80,80,255);
				drawImage(scr,img.back,460,250,200,340,100,120,255);
			}else{
				drawImage(scr,img.back,460,220,120+((count-325)/50)*80,520+a*80,80,80,255);
				drawImage(scr,img.back,460,250,100,340,100,120,255);
				if(count%50<40)drawImage(scr,img.back,460,220,380,520+((count%50-25)/5)*80,80,80,255);
			}
		}else{
			drawImage(scr,img.back,500,330,300,340+abs(2-(count/20)%4)*60,120,60,255);
		}
		if(count>=400){
			if(count<600)fillRect(scr,0,90,640,300,255,0,0,(int)((count-400)*0.48));
			else fillRect(scr,0,90,640,300,255,0,0,96);
		}
	}
	else if(gd.week==4){
		int a=0,b=0,c=0,d=0,e=0;
		if(count<300){a=540-count/5;b=90+count/5;}
		else if(count<380){a=480;b=150;}
		else if(count<400){a=580;b=150-(10-abs(count-390))*4;}
		else{
			a=500+(count-400)*2;b=150-(count-400)*2;
			c=50;
			if(count%100<20){d=((count-400)/100)*40+(count%20)*2;e=((count/2)%2)*80;}
			else d=((count-400)/100)*40+40;
		}
		drawImage(scr,img.back,0,90,0,40+((count/10)%2)*300,640,300,255);
		drawImage(scr,img.back,a,b,c,640,50,50,255);
		if(count>=300)drawImage(scr,img.back,120,150,((count/10)%3)*160,720,160,160,255);
		if(count>=380)drawImage(scr,img.back,320,250,180,640,80,80,255);
		else if(count>=340)drawImage(scr,img.back,320,250,100,640,80,80,255);
		if(count>=420)drawImage(scr,img.back,380,90,340,640,80,80,255);
		else if(count>=380)drawImage(scr,img.back,380,90,260,640,80,80,255);
		if(count>=400)drawImage(scr,img.back,380+d,110+d,160+e,880,80,80,255);
		if(count>=360)drawImage(scr,img.back,340+d,200-d,e,880,80,80,255);
		drawImage(scr,img.back,520,230,520,640+((count/10)%2)*160,120,160,255);
		if(count>=320 && count<360)drawImage(scr,img.back,440,120,420,640,40,40,255);
		if(count>=380 && count<420)drawImage(scr,img.back,440,120,460,640,40,40,255);
	}
	else if(gd.week==5){
		int a=144;
		if(count>=350)a-=(count-350)/5;
		for(int i=0 ; i<200 ; i++)fillRect(scr,0,i+90,640,1,i,0,64,255);
		drawImage(scr,img.back,300,a,740,120,40,180,255);
		if(count>=300){
			drawImage_x(scr,img.back,280-(count-300)*2/7,240-(count-300)*2/9,2+(count-300)/1000.0,840,120,80,80,255);
			drawImage_x(scr,img.back,280-(count-300)/2,240-(count-300)*2/5,2+(count-300)/500.0,840,120,80,80,255);
			drawImage_x(scr,img.back,280-(count-300)*2/3,240-(count-300)*2/3,2+(count-300)/200.0,840,120,80,80,255);
			drawImage_x(scr,img.back,280+(count-300)*2/7,240-(count-300)*2/9,2+(count-300)/1000.0,840,120,80,80,255);
			drawImage_x(scr,img.back,280+(count-300)/2,240-(count-300)*2/5,2+(count-300)/500.0,840,120,80,80,255);
			drawImage_x(scr,img.back,280+(count-300)*2/3,240-(count-300)*2/3,2+(count-300)/200.0,840,120,80,80,255);
		}
		drawImage(scr,img.back,0,90,0,40,640,300,255);
		if(count>230 && count<260 && count%10<5)drawImage(scr,img.back,360,90,680,40,120,60,255);
		if(count>280 && count<300 && count%10<5)drawImage(scr,img.back,320,90,640,40,40,80,255);
		if(count%100>=50)drawImage(scr,img.back,180,290,640,120,100,100,255);
		if(count>=450)drawImage(scr,img.back,180,230,780,120,60,60,255);
		if(count<200 || count>700)drawImage(scr,img.back,480,90,abs(2-(count/10)%4)*160,340,160,300,255);
		else if(count<230)drawImage(scr,img.back,480,90,320+((count-200)/10)*160,340,160,300,255);
		else if(count<270)drawImage(scr,img.back,480,90,800+(((count-230)/10)%2)*160,340,160,300,255);
		else drawImage(scr,img.back,480,90,800,340,160,300,255);
	}
	else if(gd.week==6){
		if(count>=300 && count<400){
			drawImage_x(scr,img.back,(int)((count-300)/1.25)-80,(count-300)/2+40,1.25-(count-300)/400.0,0,340,640,300,255);
			drawImage(scr,img.back,320+(count-300),120+(count-300),((count/5)%2)*200,640,200,200,255);
			if(CHAR_CODE==JAPANESE)drawImage(scr,img.back,320+(count-300),120+(count-300),((count/5)%2)*200,840,200,100,255);
			else drawImage(scr,img.back,320+(count-300),120+(count-300),((count/5)%2)*200,940,200,100,255);
		}else{
			int a=0,b=0,c=320;
			if(count<200)a=count;
			else if(count>=200 && count<400)a=200;
			else if(count>=400 && count<450)a=200+(count-400)*2;
			else a=300+(count-450)*6;
			if(count<300)b=count-16;
			else if(count<425)b=284+(count-400);
			else b=308+(count-425)*4;
			if(count>=250 && count<260 || count>=450)c-=abs(count%10-4)*6;
			drawImage(scr,img.back,0,90,0,40,640,300,255);
			if(count>=250 && count<300)drawImage(scr,img.back,a,c,400,640,60,60,255);
			else if(count>=450)drawImage(scr,img.back,a,c,460,640,60,60,255);
			else drawImage(scr,img.chr,a,c,30,0,30,60,255);
			drawImage(scr,img.chr,b,330,((count/5)%2)*60,110,60,60,255);
			if(CHAR_CODE==JAPANESE)drawImage(scr,img.chr,b,330,((count/5)%2)*60,230,60,30,255);
			else drawImage(scr,img.chr,b,165,((count/5)%2)*30,145,30,15,255);
		}
	}
	fillRect(scr, 0,0,640,start*2+90, 0,0,0,255);
	fillRect(scr, 0,390-start*2,640,start*2+90, 0,0,0,255);
	drawImage(scr,img.back,340,410,0,0,300,38,255);
}


// ending.png
void drawEndingAnim(SDL_Surface *scr, int cn){
	int a,b;
	if(cn<250){
		a=cn;
		fillRect(scr,0,0,640,480,0,0,0,255);
		drawImage(scr,img.back,120,40,240,1520,400,400,255);
		drawImage(scr,img.back,180,170-a/5,240,0,240,320,255);
		drawImage(scr,img.back,360+a/5,120+a/10,480,0,240,320,255);
		drawImage(scr,img.back,40-a/5,120+a/15,0,0,240,320,255);
	}
	else if(cn<550){
		a=cn-250;
		fillRect(scr,0,40,640,400,192,192,255,255);
		drawImage(scr,img.back,380+a/8,140-a+a/4,0,320,240,120,128);
		drawImage(scr,img.back,220+a/8,320-a+a/4,240,320,240,120,128);
		drawImage(scr,img.back,280+a/8,520-a+a/4,480,320,240,120,128);
		drawImage(scr,img.back,360,120-a,0,320,240,120,255);
		drawImage(scr,img.back,200,300-a,240,320,240,120,255);
		drawImage(scr,img.back,260,500-a,480,320,240,120,255);
		drawImage(scr,img.back,60,-560+a*2,0,440,160,280,255);
		drawImage(scr,img.back,160,-240+a*2,160,440,160,280,255);
		drawImage(scr,img.back,100,80+a*2,320,440,160,280,255);
		if(a>20 && a<230)drawImage(scr,img.back,360,200,1600,0,240,120,255);
	}
	else if(cn<1050){
		a=cn-550;
		fillRect(scr,0,40,640,400,0,0,0,255);
		for(int i=0 ; i<3 ; i++){
			drawImage(scr,img.back,-a+i*640,220,0,760,640,200,255);
		}
		if(a%250<20)b=(20-a%250)*32;
		else if(a%250<230)b=0;
		else b=-(a%250-230)*32;
		drawImage(scr,img.back,b+40,80,1600,(a/250)*240+240,240,120,255);
		drawImage(scr,img.back,b+360,80,1600,(a/250)*240+360,240,120,255);
	}
	else if(cn<1550){
		a=cn-1050;
		fillRect(scr,0,40,640,400,0,0,0,255);
		drawImage(scr,img.back,60,120,0,960,300,240,255);
		if(a%100>=50 && a%100!=80 && a%100!=90)drawImage(scr,img.back,138,120,300,960,90,240,255);
		drawImage(scr,img.back,200,120+abs(10-(a/3)%20)*2,400,1120,56,50,255);
		drawImage(scr,img.back,60,270,520,960+((a/10)%2)*50,100,90,255);
		drawImage(scr,img.back,260,270,520,1050-((a/10)%2)*90,100,90,255);
		drawImage(scr,img.back,170+abs(10-(a/20)%20)*2,200+abs(20-a%40)*2,400,960,120,160,255);
		fillRect(scr,170,360,140,40,0,0,0,255);
		if(a%250<20)b=(20-a%250)*20;
		else if(a%250<230)b=0;
		else b=-(a%250-230)*20;
		drawImage(scr,img.back,380,b+80,1600,(a/250)*240+720,240,120,255);
		drawImage(scr,img.back,380,b+280,1600,(a/250)*240+840,240,120,255);
	}
	else if(cn<2050){
		a=cn-1550;
		int b=80,c=0;
		if(a%100<8 || a%100>=10 && a%100<18)b+=8-abs(4-a%8)*2;
		if(a>=100 && a<200)c=(a-100)*2/5;
		if(a>=200 && a<250)c=40;
		if(a>=250 && a<350)c=40-(a-250)*2/5;
		fillRect(scr,0,80,640,180,192,192,192,255);
		fillRect(scr,0,240+c,640,160,32,32,255,255);
		for(int i=0 ; i<4 ; i++)drawImage(scr,img.back,i*240-(a%60)*4,290,0,1666,240,40,255);
		for(int i=0 ; i<4 ; i++)drawImage(scr,img.back,i*240-(a%30)*8,320-c/2,0,1608,240,60,255);
		for(int i=0 ; i<4 ; i++)drawImage(scr,img.back,i*240-(a%20)*12,340-c,0,1520,240,88,255);
		drawImage(scr,img.back,-a/5,218+c,0,1708,240,30,255);
		drawImage(scr,img.back,0,b,0,1200,640,320,255);
		if(a>=100 && a<120 || a>=140 && a<160 || a>=400 && a<410 || a>=420 && a<430)drawImage(scr,img.back,492,100+b,14,1740,14,20,255);
		else if(a>=120 && a<140 || a>=410 && a<420)drawImage(scr,img.back,492,100+b,28,1740,14,20,255);
		else drawImage(scr,img.back,492,100+b,0,1740,14,20,255);
		drawImage(scr,img.back,640-a*4,120,1600,120,240,120,255);
		drawImage(scr,img.back,1040-a*4,120,1600,1200,240,120,255);
		drawImage(scr,img.back,1440-a*4,120,1600,1320,240,120,255);
		drawImage(scr,img.back,1840-a*4,120,1600,1440,240,120,255);
		fillRect(scr,0,40,640,48,0,0,0,255);
		fillRect(scr,0,400,640,40,0,0,0,255);
	}
	else if(cn<2550){
		a=cn-2050;
		fillRect(scr,0,40,640,400,0,0,0,255);
		drawImage(scr,img.back,360,24+a/20,1400,1200,200,420,255);
		drawImage(scr,img.back,400,320+a/20,480,440+((a/8)%3)*80,120,80,255);
		drawImage(scr,img.back,390,448-a/3,1400,1620,60,160,255);
		drawImage(scr,img.back,374,504-a/3,1460+((a/2)%2)*70,1710,70,60,255);
		drawImage(scr,img.back,360,440-a/3,1460,1620,100,90,255);
		drawImage(scr,img.back,360,422-a/6,1400,1780,200,100,255);
		if(a%250<20)b=(20-a%250)*20;
		else if(a%250<230)b=0;
		else b=-(a%250-230)*20;
		drawImage(scr,img.back,40,b+80,1600,(a/250)*240+1560,240,120,255);
		drawImage(scr,img.back,40,b+280,1600,(a/250)*240+1680,240,120,255);
	}
	else if(cn<2850){
		a=cn-2550;
		for(int i=0 ; i<400 ; i++)fillRect(scr,0,i+40,640,1,i/6+(300-a)/6,i/6+(300-a)/6,255,255);
		drawImage(scr,img.back,-300,a*2/3,0,1880,640,120,128);
		drawImage(scr,img.back,200,a,0,1880,640,120,128);
		drawImage(scr,img.back,-200,a,0,1880,640,120,128);
		drawImage(scr,img.back,200,a+130,820,440,140,320,255);
		drawImage(scr,img.back,360,a+250,820,760,140,200,255);
		drawImage(scr,img.back,280,a-290,640,440,180,720,255);
		drawImage(scr,img.back,-200,a-200,0,1880,640,120,128);
		drawImage(scr,img.back,0,a*4-600,0,1880,640,120,128);
		drawImage(scr,img.back,100,a*6-1000,0,1880,640,120,128);
		drawImage(scr,img.back,-100,a*8-1000,0,1880,640,120,128);
	}
	else if(cn<3150){
		a=cn-2850;
		for(int i=0 ; i<400 ; i++)fillRect(scr,0,i+40,640,1,96+i/4,96+i/4,96+i/4,255);
		if(a<150){
			fillRect(scr,0,260+a/5,640,180,0,64,64,255);
			drawImage_x(scr,img.back,300,160+a/5,20/60.0,640,1400,120,360,255);
			drawImage_x(scr,img.back,180,210+a/8,60/140.0,640,1200,280,200,255);
			drawImage_x(scr,img.back,340,210+a/8,60/140.0,920,1200,280,200,255);
			drawImage_x(scr,img.back,70,220-a/6,100/140.0,640,1200,280,200,255);
			drawImage_x(scr,img.back,370,220-a/6,100/140.0,920,1200,280,200,255);
			drawImage(scr,img.back,-40,240-a/2,640,1200,280,200,255);
			drawImage(scr,img.back,400,240-a/2,920,1200,280,200,255);
			drawImage_x(scr,img.back,320,280+a/25,20/40.0,840,1400,80,60,255);
			drawImage_x(scr,img.back,260,320-a/3,40/40.0,760,1400,80,60,255);
		}else{
			fillRect(scr,0,290+(a-150)*2,640,150,0,64,64,255);
			if(a>=225)drawImage_x(scr,img.back,300-(a-150),-34+(a-225)*4,(20.0+(a-150))/60.0,640,1400,120,360,255);
			else drawImage_x(scr,img.back,300-(a-150),190-(a-150)*3,(20.0+(a-150))/60.0,640,1400,120,360,255);
			drawImage_x(scr,img.back,180-(a-150)*5,228+(a-150)*2,(60.0+(a-150)*1.5)/140.0,640,1200,280,200,255);
			drawImage_x(scr,img.back,340+(a-150)*2,228+(a-150)*2,(60.0+(a-150)*1.5)/140.0,920,1200,280,200,255);
			drawImage_x(scr,img.back,70-(a-150)*8,196+(a-150),(100.0+(a-150)*2)/140.0,640,1200,280,200,255);
			drawImage_x(scr,img.back,370+(a-150)*4,196+(a-150),(100.0+(a-150)*2)/140.0,920,1200,280,200,255);
			drawImage_x(scr,img.back,-40-(a-150)*10,166,(140.0+(a-150)*3)/140.0,640,1200,280,200,255);
			drawImage_x(scr,img.back,400+(a-150)*6,166,(140.0+(a-150)*3)/140.0,920,1200,280,200,255);
			drawImage_x(scr,img.back,320,290+(a-150),(20.0+(a-150)*2)/40.0,840,1400,80,60,255);
			drawImage_x(scr,img.back,260-(a-150)*5,290-(a-150),(40.0+(a-150)*4)/40.0,760,1400,80,60,255);
		}
		if(a>=250)fillRect(scr,0,0,640,480,255,255,255,(a-250)*5);
	}
	else if(cn<3750){
		a=cn-3150;
		int b=0;
		if(a>=500)b=255;
		else if(a>=245)b=(a-245);
		if(a>=255 && a<260 || a>=285 && a<290)b=0;
		for(int i=0 ; i<400 ; i++)fillRect(img.scr2,0,i+40,640,1,i/2,i/2,192,255);
		drawImage(img.scr2,img.back,a*2/5,a*2/5,960,800,640,400,255);
		drawImage(img.scr2,img.back,a*2/5,-a*2/5,960,400,640,400,255);
		drawImage(img.scr2,img.back,-a*2/5,0,960,0,640,400,255);
		drawImage(img.scr2,img.back,280,80,1000,1400,160,320,b);
		drawImage(img.scr2,img.back,120+a/10,160-a/10,1200,1200+((a/5)%2)*300,200,300,255);
		drawImage(img.scr2,img.back,268,90+a/10,640,1400,104,350,255);
		drawImage(img.scr2,img.back,-190+a/3,250-a/6,760,1700,240,240,255);
		drawImage(img.scr2,img.back,580-a/3,250-a/6,760,1460,240,240,255);
		if(a<300)drawImage_x(scr,img.scr2,-(300-a)*320/300,40-(300-a)*200/300,1+(300-a)/300.0,0,0,640,400,255);
		else drawImage(scr,img.scr2,0,40,0,0,640,400,255);
		if(a>=550)fillRect(scr,0,40,640,400,0,0,0,255);
		else if(a>=500)fillRect(scr,0,40,640,400,0,0,0,(a-500)*5);
		if(a<255)fillRect(scr,0,40,640,400,255,255,255,255-a);
		if(a>=250)drawImage(scr,img.back,220,200,1000,1720,200,80,255);
	}else{
		a=cn-3750;
		if(loadtime<3750){
			freeImage(img.back);
			getImage(img.back,"file/img/prologue.png",0,0,255);
			loadtime=cn;
		}
		drawImage(scr,img.back,0,60,0,1080,640,360,255);
		drawImage(scr,img.back,40,120+abs(10-count%20)*2,1440,600,300,300,255);
		drawImage(scr,img.back,240,200,1280,920,400,220,255);
		fillRect(scr,0,40,640,400,0,0,255,128);
		fillRect(scr,0,40,640,20,0,0,0,255);
		fillRect(scr,0,420,640,20,0,0,0,255);
		if(a<450){
			drawImage(scr,img.back,220,60,1280,1200,220,80,255);
			if(a<220)drawImage(scr,img.back,100,240,1280,1280,440,50,255);
			else{
				drawImage(scr,img.back,100,180,1280,1330,440,50,255);
				drawImage(scr,img.back,100,280,1280,1380,440,50,255);
			}
		}
	}
	if(cn>=2600 && cn<3000)drawImage(scr,img.back,300,320,1000,1800,340,100,255);

	fillRect(scr,0,0,640,40,0,0,0,255);
	fillRect(scr,0,440,640,40,0,0,0,255);
}

// ending2.png
// このバージョンではまだ使わない
void drawLastEndingAnim(SDL_Surface* scr, int cn){
	if(cn>=1300 && cn<2800)fillRect(scr,0,20,320,120,255,192,192,255);
	if(cn>=2800 && cn<3350 || cn>=3750)fillRect(scr,0,20,320,200,255,255,255,255);
	if(cn>=3350 && cn<3750)fillRect(scr,0,20,320,200,0,0,0,255);
	if(cn>=1300 && cn<2800){
		fillRect(scr,0,140,320,80,128,128,255,255);
		if(cn%230>=30)drawImage(scr,img.back,60,160,1280,(cn/230)*60,200,60,255);
	}
	int a;
	if(cn<50){
		drawImage(scr,img.back,0,20,0,0,320,120,255);
		if(cn<40)drawImage(scr,img.back,150+cn,10+cn*cn/20,0,120,40,40,255);
		else drawImage(scr,img.back,150+cn,90,0,120,40,40,255);
		if(cn>=10)drawImage(scr,img.back,80+(cn-10),10+(cn-10)*(cn-10)/20,120,120,40,40,255);
	}
	else if(cn<1030){
		a=cn-50;
		for(int i=0 ; i<4 ; i++)drawImage(scr,img.back,320*i-a,20,0,0,320,120,255);
		drawImage(scr,img.back,200,90,abs(2-(a/10)%4)*40,120,40,40,255);
		drawImage(scr,img.back,120,90,120+abs(2-(a/10)%4)*40,120,40,40,255);
	}
	else if(cn<1100){
		a=cn-1030;
		drawImage(scr,img.back,0,20,0,600,320,120,255);
		drawImage(scr,img.back,-70+a*6,170-a*4,abs(2-(a/10)%4)*100,1800,100,100,255);
		drawImage(scr,img.back,-200+a*6,250-a*4,300+abs(2-(a/10)%4)*100,1800,100,100,255);
		drawImage(scr,img.back,220,20,460,480,120,120,255);
	}
	else if(cn<1170){
		a=cn-1100;
		drawImage(scr,img.back,0,20,320,600,270,120,255);
		if(a<20)drawImage(scr,img.back,150-(int)(a*4.5),20-(int)(a*0.75),810,360,120,120,255);
		else{
			drawImage(scr,img.back,60,5,810,360,120,120,255);
			drawImage(scr,img.back,270-(a-20)*6,-10+(a-20)*2,abs(2-(a/10)%4)*100,1900,100,100,255);
			drawImage(scr,img.back,370-(a-20)*6,-30+(a-20)*2,300+abs(2-(a/10)%4)*100,1900,100,100,255);
		}
		drawImage(scr,img.back,270,20,590,600,50,120,255);
	}
	else if(cn<1230){
		a=cn-1170;
		drawImage(scr,img.back,0,20,640,600,320,120,255);
		drawImage(scr,img.back,320-a*7,40-(45-(a-30)*(a-30)/20),abs(2-(a/10)%4)*100,1900,100,100,255);
		drawImage(scr,img.back,420-a*7,40-(45-(a-30)*(a-30)/20),300+abs(2-(a/10)%4)*100,1900,100,100,255);
	}
	else if(cn<1300){
		a=cn-1230;
		drawImage(scr,img.back,0,20,960,600,320,120,255);
		drawImage(scr,img.back,320-a*7,40,abs(2-(a/10)%4)*100,1900,100,100,255);
		drawImage(scr,img.back,420-a*7,40,300+abs(2-(a/10)%4)*100,1900,100,100,255);
	}
	else if(cn<1800){
		a=cn-1300;
		if(a<60){
			drawImage(scr,img.back,0,20,320,0,320,120,255);
			drawImage(scr,img.back,0,140,0,160,320,15,255);
		}
		else if(a<180){
			drawImage(scr,img.back,0,20,320,120,320,120,255);
			if(a<80)fillRect(scr,0,20,90,120,255,192,192,255);
			if(a<120)fillRect(scr,200,20,30,60,255,192,192,255);
			drawImage(scr,img.back,0,140,0,175,320,30,255);
		}
		else if(a<300){
			drawImage(scr,img.back,0,20,320,240,320,120,255);
			if(a<195)fillRect(scr,0,20,220,30,255,192,192,255);
			if(a<210)fillRect(scr,0,50,220,30,255,192,192,255);
			if(a<225)fillRect(scr,0,80,220,30,255,192,192,255);
			if(a<240)fillRect(scr,240,20,80,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,205,320,30,255);
		}
		else if(a<420){
			drawImage(scr,img.back,0,20,320,360,320,120,255);
			if(a<320)fillRect(scr,240,20,80,120,255,192,192,255);
			if(a<360)fillRect(scr,0,20,110,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,235,320,30,255);
		}else{
			drawImage(scr,img.back,0,20,320,120,320,120,255);
			fillRect(scr,0,20,90,120,255,192,192,255);
			fillRect(scr,200,20,30,60,255,192,192,255);
			if(a>=440)drawImage(scr,img.back,0,20,320,480,110,120,255);
			if(a>=460)drawImage(scr,img.back,200,20,430,480,30,60,255);
			drawImage(scr,img.back,0,140,0,265,320,30,255);
		}
	}
	else if(cn<2300){
		a=cn-1800;
		if(a<60){
			drawImage(scr,img.back,0,20,640,0,320,120,255);
			drawImage(scr,img.back,0,140,0,295,320,15,255);
		}
		else if(a<160){
			drawImage(scr,img.back,0,20,640,120,320,120,255);
			if(a<80)fillRect(scr,220,20,60,80,255,192,192,255);
			if(a<120)fillRect(scr,0,20,110,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,310,320,30,255);
		}
		else if(a<280){
			if(a<210)drawImage(scr,img.back,80,20,640,240,160,120,255);
			else drawImage(scr,img.back,80,20,800,240,160,120,255);
		}
		else if(a<380){
			drawImage(scr,img.back,0,20,640,120,320,120,255);
			fillRect(scr,220,20,60,80,255,192,192,255);
			fillRect(scr,0,20,110,120,255,192,192,255);
			if(a>=300)drawImage(scr,img.back,220,20,720,360,50,70,255);
			if(a>=340)drawImage(scr,img.back,0,20,640,360,80,120,255);
			drawImage(scr,img.back,0,140,0,340,320,15,255);
		}else{
			drawImage(scr,img.back,0,20,640,480,320,80,255);
			drawImage(scr,img.back,110+abs(5-a%10),50+abs(5-a%10),770,360,40,50,255);
			drawImage(scr,img.back,0,100,640,560,320,40,255);
			if(a<400)fillRect(scr,230,20,90,120,255,192,192,255);
			if(a<440)fillRect(scr,0,20,90,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,355,320,30,255);
		}
	}
	else if(cn<2800){
		a=cn-2300;
		if(a<60){
			drawImage(scr,img.back,0,20,960,0,320,120,255);
			drawImage(scr,img.back,0,140,0,385,320,30,255);
		}
		else if(a<160){
			drawImage(scr,img.back,0,20,960,120,320,120,255);
			if(a<80)fillRect(scr,270,20,50,120,255,192,192,255);
			if(a<120)fillRect(scr,0,20,70,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,415,320,30,255);
		}
		else if(a<260){
			drawImage(scr,img.back,0,20,960,240,320,120,255);
			if(a<180)fillRect(scr,0,20,100,120,255,192,192,255);
			if(a<220)fillRect(scr,240,20,80,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,445,320,30,255);
		}
		else if(a<380){
			drawImage(scr,img.back,0,60,960,400,270,80,255);
			if(a>=280)drawImage(scr,img.back,270,20,1230,360,50,120,255);
			if(a>=310){
				drawImage(scr,img.back,110,100,960,360+((a/2)%2)*20,60,20,255);
				drawImage(scr,img.back,170,100,1020,360+((a/2)%2)*20,60,20,255);
			}
			drawImage(scr,img.back,0,140,0,475,320,15,255);
		}else{
			drawImage(scr,img.back,0,20,960,480,320,120,255);
			if(a<400)fillRect(scr,60,20,30,120,255,192,192,255);
			if(a<420)fillRect(scr,30,20,30,120,255,192,192,255);
			if(a<440)fillRect(scr,0,20,30,120,255,192,192,255);
			drawImage(scr,img.back,0,140,0,490,320,30,255);
		}
	}
	else if(cn<3000){
		a=cn-2800;
		int b=0;
		if(a>=60)b=a-60;
		drawImage_x(scr,img.back,130+a+b*4,120-(int)(b*0.8),(100.0+b*2)/100.0,0,1560,100,100,255);
		if(a<40)drawImage_x(scr,img.back,240-(int)(a*0.5),200-a-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1600,40,40,255);
		else if(a<135)drawImage_x(scr,img.back,240-(int)(a*0.5),160-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1600,40,40,255);
		else drawImage_x(scr,img.back,240-(int)(a*0.5),160-(int)(b*0.5)-(a-135),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1600,40,40,255);
		if(a<20)drawImage_x(scr,img.back,280-(int)(a*0.5),200-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1560,40,40,255);
		else if(a<60)drawImage_x(scr,img.back,280-(int)(a*0.5),200-(a-20)-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1560,40,40,255);
		else if(a<135)drawImage_x(scr,img.back,280-(int)(a*0.5),160-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1560,40,40,255);
		else drawImage_x(scr,img.back,280-(int)(a*0.5),160-(int)(b*0.5)-(a-135),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1560,40,40,255);
		drawImage_x(scr,img.back,120+a/3+(int)(b*3.5),140-(int)(b*1.5),(20.0+b)/80.0,abs(2-(a/10)%4)*80,720,80,160,255);
		drawImage_x(scr,img.back,-100+a,140-b,(40.0+(b*1.5))/100.0,720,720,100,100,255);
		fillRect(scr,0,180+(int)(b*0.5),130+a+b*4,40,0,0,0,255);
	}
	else if(cn<3050){
		a=cn-3000;
		int b=a*2,c;
		if(a<10)c=1;
		else if(a<20)c=2;
		else if(a<30)c=0;
		else if(a<40)c=3;
		else c=0;
		drawImage_x(scr,img.back,100-b/2,b/2,(250.0-b)/100.0,720+c*100,720,100,100,255);
		drawImage_x(scr,img.back,320-(int)(a*2.5)-(int)(b*0.5),60+(int)(b*0.5),(120.0-(b*0.5))/120.0,360,720,120,160,255);
		fillRect(scr,0,250-(int)(b*0.5),320,40,0,0,0,255);
	}
	else if(cn<3100){
		a=cn-3050;
		int b=a;
		drawImage_x(scr,img.back,50+b/2,50-(int)(b*0.8),(150.0+(b*1.6))/100.0,720,720,100,100,255);
		drawImage_x(scr,img.back,145+b*3,110-b*2,(70.0+b*3)/120.0,360,720,120,160,255);
		fillRect(scr,0,170+b*2,320,50,0,0,0,255);
	}
	else if(cn<3250){
		a=cn-3100;
		double b;
		int c,d;
		if(a<50){b=1-a/166.6;c=50-a;d=a;}
		else if(a<100){b=0.70+(a-50)/166.6;c=a-50;d=50-(a-50);}
		else{b=1+(a-100)/100.0;c=50+(a-100)*2;d=(a-100)*7;}
		drawImage_x(scr,img.back,c,40-d+(int)(50*b),b,0,880,100,100,255);
		drawImage_x(scr,img.back,c+(int)(50*b),40-d+(int)(95*b),b,0,980,100,150,255);
		drawImage_x(scr,img.back,c+(int)(110*b),40-d+(int)(75*b),b,500,880,170,180,255);
		drawImage_x(scr,img.back,c+(int)(80*b),40-d,b,1010,880,90,90,255);
	}
	else if(cn<3350){
		a=cn-3250;
		int b,c=0;
		if(a<85)b=a/3;
		else c=(a-85)*4;
		fillRect(scr,0,320-a*4,320,300,0,0,0,255);
		if(a<50)drawImage_x(scr,img.back,250-a,-40+(int)(a*2.5),(60.0-b)/60.0,((a/5)%6)*60,1680,60,60,255);
		else if(a<85)drawImage_x(scr,img.back,250-a,-40+(int)(a*2.5),(60.0-b)/60.0,((a/5)%6)*60,1740,60,60,255);
		else drawImage_x(scr,img.back,165+(int)((a-85)*2.4),172-(int)((a-85)*0.7),(32.0-c/8)/60.0,0,1740,60,60,255);
		drawImage_x(scr,img.back,-570+a*6,-60+a,(250.0-c)/250.0,0,1130,250,180,255);
	}
	else if(cn<3400){
		a=cn-3350;
		drawImage_x(scr,img.back,30,40,(190.0+a*1.2)/250.0,0,1130,250,180,255);
	}
	else if(cn<3550){
		a=cn-3400;
		double b=1+a/75.0;
		drawImage_x(scr,img.back,(int)(95*b)-a*4,20-a,b,0,1310,100,100,255);
		drawImage_x(scr,img.back,(int)(105*b)-a*4,20+(int)(65*b)-a,b,570,1440,180,120,255);
		drawImage_x(scr,img.back,-a*4,20+(int)(30*b)-a,b,300,1310,130,120,255);
		drawImage_x(scr,img.back,(int)(70*b)-a*4,20+(int)(110*b)-a,b,0,1430,190,130,255);
	}
	else if(cn<3750){
		a=cn-3550;
		drawImage_x(scr,img.back,80,110-(int)(a*0.25),(100.0+a/5)/120.0,600,1680,120,60,255);
		drawImage_x(scr,img.back,80,60-(int)(a*0.25),(100.0+a/5)/120.0,360,1680,120,60,255);
		drawImage_x(scr,img.back,80,110+a/10,(100.0+a/5)/120.0,360,1740,120,60,255);
	}
	else if(cn<3900){
		a=cn-3750;
		drawImage_x(scr,img.back,80,70,140/120.0,600,1740,120,60,255);
		drawImage_x(scr,img.back,80,10,140/120.0,480,1680,120,60,255);
		drawImage_x(scr,img.back,80,130,140/120.0,480,1740,120,60,255);
	}
	else if(cn<3950){
		a=cn-3900;
		drawImage_x(scr,img.back,80+(int)(a*1.4),70+(int)(a*0.7),(140.0-a*2.8)/120.0,600,1740,120,60,255);
		drawImage_x(scr,img.back,80+(int)(a*1.4),10+(int)(a*2.0),(140.0-a*2.8)/120.0,480,1680,120,60,255);
		drawImage_x(scr,img.back,80+(int)(a*1.4),130-(int)(a*0.5),(140.0-a*2.8)/120.0,480,1740,120,60,255);
	}else{
		a=cn-3950;
		drawImage(scr,img.back,0,20,960,1680,320,200,255);
		if(a>=100 && a<650){
			drawImage(scr,img.back,110,30,720,1800,110,40,255);
			drawImage(scr,img.back,50,90,720,1840,220,90,255);
		}
	}
	fillRect(scr,0,0,320,20,0,0,0,255);
	fillRect(scr,0,220,320,20,0,0,0,255);
	if(cn<1300){
		fillRect(scr,0,140,320,80,128,128,255,255);
		if(cn%230>=30)drawImage(scr,img.back,60,160,1280,(cn/230)*60,200,60,255);
	}
}

// epilogue.png
void drawSummerWars(SDL_Surface* scr){
	if(count<200){
		fillRect(scr,0,0,320,240,0,0,0,255);
		drawImage(scr,img.back,0,20,0,0,320,200,255);
		drawImage(scr,img.back,60+count/2,60+count/4,320,40+(count%2)*40,60,40,255);
	}
	else if(count<300){
		if(count==200){
			map.buffered=false;
		}
		else{
			walking();
		}
		SDL_Color col=getSkyColor(gd.hour,gd.minute);
		fillRect(scr,0,0,320,240,col.r,col.g,col.b,255);
		drawMap(scr,gd.scrX,gd.scrY);
		if(count==299){
			drawImage(img.scr2,scr,0,0,0,0,320,240,255);
		}
	}
	else if(count<400){
		drawImage_x(scr,img.scr2,0,0,(320.0+(count-300)*4)/320.0,count-300,(int)((count-300)/1.2),320,240,255);
	}
	else if(count<500){
		int a=count-400;
		int dx=0,dy=0;
		if(a>=20){
			dx=(int)(cos(a*0.7)*5);
			dy=(int)(sin(a*0.7)*5);
		}
		drawImage(scr,img.back,-5+dx,15+dy,0,350,330,160,255);
		if(a<20){
			drawImage(scr,img.back,160-a*3,-100+a*12,320,120,40,120,255);
			drawImage(scr,img.back,140-a*3,a*12,360,120,40,40,128);
			drawImage(scr,img.back,50-a*a,80-a*a,360,180,60,60,a*13);
			drawImage(scr,img.back,80-a*a/2,70-a*a,360,180,60,60,a*13);
			drawImage(scr,img.back,120,60-a*a,360,180,60,60,a*13);
			drawImage(scr,img.back,150-a*a/2,80-a*a,360,180,60,60,a*13);
			drawImage(scr,img.back,170+a*a,100-a*a/2,360,180,60,60,a*13);
		}else{
			drawImage(scr,img.back,80+dx-a,140+dy-a/2,400,0,120,120,255);
			drawImage(scr,img.back,80+dx-a/2,160+dy-a,520,0,120,120,255);
			drawImage(scr,img.back,80+dx,220+dy-a*2,640,0,120,120,255);
			drawImage(scr,img.back,80+dx+a/2,170+dy-a,760,0,120,120,255);
			drawImage(scr,img.back,80+dx+a,140+dy-a/2,880,0,120,120,255);
			drawImage(scr,img.back,80+dx,300+dy-a*2,400,120,120,60,255);
			drawImage(scr,img.back,60+dx-a/2,190+dy-a,420,180,60,60,255);
			drawImage(scr,img.back,120+dx-a,170+dy-a,420,180,60,60,255);
			drawImage(scr,img.back,90+dx,240+dy-a*2,420,180,60,60,255);
			drawImage(scr,img.back,60+dx+a,160+dy-a,420,180,60,60,255);
			drawImage(scr,img.back,130+dx+a/2,180+dy-a,420,180,60,60,255);
			drawImage(scr,img.back,160+dx-a,220+dy-a,420,180,60,60,255);
			drawImage(scr,img.back,20+dx+a,200+dy-a,420,180,60,60,255);
			drawImage(scr,img.back,60+dx-a/2,200+dy-a*2,540,120,80,160,a*2);
		}
		drawImage(scr,img.back,-5+dx,75+dy,0,200,330,150,255);
		fillRect(scr,0,0,320,20,0,0,0,255);
		fillRect(scr,0,220,320,20,0,0,0,255);
	}
	else if(count<600){
		int a=count-500;
		int dx=0,dy=0;
		dx=(int)(cos(a*0.7)*5);
		dy=(int)(sin(a*0.7)*5);
		drawImage(scr,img.back,-5+dx,15+dy,0,510,330,210,255);
		if(a<30)drawImage(scr,img.back,50+dx+a/3,30+dy-a/3,620,120,240,100,255);
		else drawImage(scr,img.back,60+dx+(a-30)/2,20+dy-(a-30)/2,620,220,240,100,255);
		//砂嵐
		drawImage(scr,img.back,-250+a*3+dx,240-a*6+dy,580,320,200,400,255);
		drawImage(scr,img.back,-130+a*3+dx,280-a*6+dy,580,320,200,400,255);
		drawImage(scr,img.back,-10+a*3+dx,310-a*6+dy,580,320,200,400,255);
		//じゃり
		drawImage(scr,img.back,20-a/4+dx,140-a+dy,480,180,60,60,255);
		drawImage(scr,img.back,90+dx,160-a*2+dy,480,180,60,60,255);
		drawImage(scr,img.back,130+a/4+dx,140-a*2+dy,480,180,60,60,255);
		drawImage(scr,img.back,160+a/2+dx,110-a+dy,480,180,60,60,255);
		drawImage(scr,img.back,200+a+dx,110-a/2+dy,480,180,60,60,255);
		//岩
		drawImage(scr,img.back,dx,190-a*2+dy,420,240,80,80,255);
		drawImage(scr,img.back,100+a/4+dx,260-a*4+dy,420,240,80,80,255);
		drawImage(scr,img.back,140+a/2+dx,230-a*3+dy,420,240,80,80,255);
		drawImage(scr,img.back,230+a+dx,210-a*2+dy,420,240,80,80,255);
		//大岩
		drawImage(scr,img.back,-120+a*2+dx,360-a*4+dy,330,520,180,100,255);
		drawImage(scr,img.back,40+a*2+dx,380-a*4+dy,330,520,180,100,255);
		//砂嵐
		drawImage(scr,img.back,-390+a*4+dx,620-a*8+dy,580,320,200,400,255);
		drawImage(scr,img.back,-270+a*4+dx,560-a*8+dy,580,320,200,400,255);
		drawImage(scr,img.back,-150+a*4+dx,600-a*8+dy,580,320,200,400,255);
		//大砂嵐
		drawImage(scr,img.back,-560+a*5+dx,930-a*10+dy,780,320,200,400,255);
		drawImage(scr,img.back,-440+a*5+dx,900-a*10+dy,780,320,200,400,255);
		drawImage(scr,img.back,-320+a*5+dx,870-a*10+dy,780,320,200,400,255);
		//黒岩
		drawImage(scr,img.back,-600+a*6+dx,1260-a*12+dy,420,320,100,100,255);
		drawImage(scr,img.back,-470+a*6+dx,1200-a*12+dy,420,320,100,100,255);
		drawImage(scr,img.back,-340+a*6+dx,1220-a*12+dy,420,320,100,100,255);
		//風
		drawImage(scr,img.back,(a*2)%25+dx,200-(a%25)*16+dy,330,240,80,200,128);
		drawImage(scr,img.back,70+a%50+dx,190-(a%50)*8+dy,330,240,80,200,128);
		drawImage(scr,img.back,150+a%50+dx,210-(a%50)*8+dy,330,240,80,200,128);
		drawImage(scr,img.back,260+(a*2)%25+dx,220-(a%25)*16+dy,330,240,80,200,128);
		drawImage(scr,img.back,-200+(a%50)*12+dx,150-a%50+dy,330,440,250,80,128);
		fillRect(scr,0,0,320,20,0,0,0,255);
		fillRect(scr,0,220,320,20,0,0,0,255);
	}else{
		if(count==600){
			gd.scrX=gd.summerX-160;
			gd.scrY=gd.summerY-120;
			map.buffered=false;
		}
		SDL_Color col=getSkyColor(gd.hour,gd.minute);
		fillRect(scr,0,0,320,240,col.r,col.g,col.b,255);
		drawMap(scr,gd.scrX,gd.scrY);
		if(count<660)drawImage(scr,img.back,150,110,380,((count-600)/20)*20,20,20,255);
		else drawTalking(scr);
	}
}

// prologue.png
void drawPrologue(SDL_Surface* scr){
	if(gd.scene_count==0){
		if(start>0){
			drawImage(scr,img.back,0,0,0,0,640,360,255);
		}else{
			drawImage(scr,img.back,0,0,0,360,640,360,255);
			drawImage(scr,img.back,160,120+2*abs(20-(count/2)%40),1280,0,160,240,255);
			drawImage(scr,img.back,320,120+2*abs(20-((count/2)+20)%40),1440,0,160,240,255);
		}
	}
	if(gd.scene_count==1){
		drawImage(scr,img.back,0,0,0,720,640,360,255);
		if(start>50){
			drawImage(scr,img.back,200,100,1280,600,160,160,255);
			drawImage(scr,img.back,220,220,1280,600,160,160,255);
		}
		if(start>75)drawImage(scr,img.back,200,0,1280,240,200,360,255);
		else if(start>50)drawImage(scr,img.back,200,0,1480,240,200,360,255);
		else drawImage(scr,img.back,40,0,1760,240,160,360,255);
		if(start<=50 && start>25){
			drawImage(scr,img.back,200+2*abs(5-(count/3)%10),100,1280,600,160,160,255);
			drawImage(scr,img.back,220+2*abs(5-((count+5)/3)%10),220,1280,600,160,160,255);
		}
		if(start<=25){
			drawImage(scr,img.back,350-((count+5)%20)*16,-10+((count+5)%20)*16,1280,600,160,160,255);
		}
		drawImage(scr,img.back,160,200,1600,0,160,240,255);
		if(start<=25){
			drawImage(scr,img.back,360-(count%20)*16,20+(count%20)*16,1280,600,160,160,255);
			drawImage(scr,img.back,380-((count+10)%20)*16,40+((count+10)%20)*16,1280,600,160,160,255);
			drawImage(scr,img.back,320,0,320,720,320,60,255);
			drawImage(scr,img.back,400,0,400,720,240,360,255);
		}
	}
	if(gd.scene_count==2){
		drawImage(scr,img.back,0,0,0,1080,640,360,255);
		drawImage(scr,img.back,40,60+abs(10-count%20)*4,1440,600,300,300,255);
		drawImage(scr,img.back,240,140,1280,920,400,220,255);
	}
	if(gd.scene_count==3 || gd.scene_count==5){
		drawImage(scr,img.back,0,0,640,0,640,360,255);
		drawImage(scr,img.back,360+2*abs(20-count%40),40+2*abs(20-(count/2)%40),1740,600,180,320,255);
	}
	if(gd.scene_count==4){
		drawImage(scr,img.back,0,0,0,720,640,360,255);
		drawImage(scr,img.back,40,0,1760,240,160,360,255);
		drawImage(scr,img.back,80,200+abs(10-count%20)*4,1760,0,160,240,255);
	}
	if(gd.scene_count==6){
		drawImage(scr,img.back,0,0,640,360,640,360,255);
		drawImage(scr,img.back,200,120+2*abs(6-(count/2)%12),1640+abs(2-(count/10)%4)*40,1140,40,60,255);
		drawImage(scr,img.back,280,120+2*abs(6-((count/2)+6)%12),1760,1140,40,60,255);
		if(count%150<50)drawImage(scr,img.back,180+(count%150)*4,40+(count%150-25)*(count%150-25)/5,1800,1140,60,60,255);
		drawImage(scr,img.back,150,160,1280,1140,360,60,255);
	}
	if(gd.scene_count==7){
		drawImage(scr,img.back,0,0,640,720,640,360,255);
		drawImage(scr,img.back,220+2*abs(20-count%40),200,1280,760,160,160,255);
	}
	if(gd.scene_count==8){
		drawImage(scr,img.back,0,0,640,1080,640,360,255);
		fillRect(scr,2*(count%50),0,6,30,0,0,0,255);
		if(start>50)drawImage(scr,img.back,280-(start-50)*4,40-(start-50)*8,1280,600,160,160,255);
		else if(start>0)drawImage(scr,img.back,280,40,1280,600,160,160,255);
	}
	fillRect(scr,0,360,640,120,0,0,0,255);
	if(start==0)drawTalking(scr);
}

// komoro.png komoro/*.jpg
void drawKomoro(SDL_Surface* scr){
	int x2=0,y2=0;
	double w=0;
	if(gd.scene_count==2 || gd.scene_count==6 || gd.scene_count==9){
		if(count<260)y2=260-count;
	}
	else if(gd.scene_count==8){
		if(count<160){
			x2=count*2;
			y2=120;
		}
		else if(count<200){
			w=1-0.0125*(count-160);
			x2=(200-count)*8;
			y2=(200-count)*3;
		}
		else if(count==200){
			drawImage_x(img.scr2,img.photo[8],0,0,0.5,0,0,640,480,255);
		}
	}
	else if(gd.scene_count==12){
		if(count<200)y2=count;
		else y2=200;
	}
	else if(gd.scene_count==13){
		if(start==0){
			if(count==2){
				drawImage_x(img.scr2,img.photo[13],0,0,0.5,0,0,640,480,255);
			}
		}
		else if(start<100){
			w=0.5+0.005*(100-start);
			y2=(100-start)*2;
		}
	}

    fillRect(scr,0,180,320,60,0,0,0,255);
	if(w==0){
		if(gd.scene_count==8&&count>=200 || gd.scene_count==13)drawImage(scr,img.scr2,0,0,x2,y2,320,240,255);
		else drawImage(scr,img.photo[gd.scene_count],0,0,x2,y2,320,240,255);
	}
	else drawImage_x(scr,img.photo[gd.scene_count],0,0,w,x2,y2,(int)(320/w),(int)(240/w),255);

	if(gd.scene_count==0){
		drawImage(scr,img.back,220+start,100,320+80*((count/5)%2),0,80,140,255);
	}
	else if(gd.scene_count==1){
		drawImage(scr,img.back,abs(20-count%40),100,320+80*((count/5)%2),0,80,140,255);
		drawImage(scr,img.back,180+abs(40-(count+40)%80),120+abs(10-(count/2)%20),80,0,80,120,255);
	}
	else if(gd.scene_count==4){
		drawImage(scr,img.back,-start*4,120,160,0,80,120,255);
	}
	else if(gd.scene_count==5){
		drawImage(scr,img.back,40+abs(80-(count)%160),120+abs(10-(count/2)%20),0,0,80,120,255);
		drawImage(scr,img.back,160+abs(80-(count+40)%160),120+abs(10-(count/2+10)%20),80,0,80,120,255);
	}
	else if(gd.scene_count==6){
		if(count>180 && count<200)drawImage(scr,img.back,50+(200-count)*14,100-(200-count)*2,160,120,20,20,255);
		if(count>200 && count<220)drawImage(scr,img.back,50+(count-200)*14,100+(count-200)*8,160,120,20,20,255);
	}
	else if(gd.scene_count==8){
		if(count>200)drawImage(scr,img.back,240,120+abs(20-count%40),240,0,80,120,255);
	}
	else if(gd.scene_count==11){
		drawImage(scr,img.back,count*4-80,100,320+80*((count/5)%2),0,80,140,255);
	}
	else if(gd.scene_count==12){
		drawImage(scr,img.back,0,320-y2,0,0,80,120,255);
		drawImage(scr,img.back,240,320-y2,80,0,80,120,255);
	}
	else if(gd.scene_count==13){
		if(face[gd.face_count]==EOF){
			if(start>100){
				drawImage(scr,img.back,(200-start)*5+20,80+(200-start)/3+abs(10-(count/2)%20),0,0,80,140,255);
				drawImage(scr,img.back,(200-start)*4+40,90+(200-start)/2+abs(10-(count/2)%20),80,0,80,140,255);
				drawImage(scr,img.back,(200-start)*3,100+(200-start),320+80*((count/5)%2),0,80,140,255);
			}
			if(start<30)fillRect(scr,0,0,320,240,0,0,0,(30-start)*8);
		}else{
			drawImage(scr,img.back,20,80,0,0,80,140,255);
			drawImage(scr,img.back,40,90,80,0,80,140,255);
			drawImage(scr,img.back,0,100,320+80*((count/5)%2),0,80,140,255);
		}
	}

	if(start==0)drawTalking(scr);
}
