#include "animation.h"

// opening.png
void drawOpeningAnim(SDL_Surface* scr, int cn){
	int a;
	if(cn>=1120 && cn<1350)fillRect(scr,0,20,320,200,0,0,0,255);
	else{
		for(int i=0 ; i<200 ; i++)fillRect(scr,0,20+i,320,1,0,0,i/2,255);
	}
	if(cn<70){
		a=cn;
		drawImage_x2(scr,img.back,80,143+a/10,(160.0+a/4)/160.0,1060,200,160,100);
		drawImage_x2(scr,img.back,40-a/2,60,(50.0+a/3)/50.0,1060,0,50,200);
		drawImage_x2(scr,img.back,240+a/2,60,(50.0+a/3)/50.0,1110,0,50,200);
		drawImage_x2(scr,img.back,-a,20-a,(50.0+a/2)/50.0,960,0,50,300);
		drawImage_x2(scr,img.back,270+a,20-a,(50.0+a/2)/50.0,1010,0,50,300);
		drawImage_x2(scr,img.back,70-a/2,100-a/2,(160.0-a/3)/160.0,1160,100+((a/2)%2)*50,160,50);
		drawImage_x2(scr,img.back,70-a/2,100-a/2,(160.0-a/3)/160.0,1160,0,160,100);
	}
	else if(cn<120){
		a=cn-70;
		drawImage_x2(scr,img.back,80,150-a,(160.0+(a+70)/4)/160.0,1060,200,160,100);
		drawImage_x2(scr,img.back,40-(a+70)/2,60-a,(50.0+(a+70)/3)/50.0,1060,0,50,200);
		drawImage_x2(scr,img.back,240+(a+70)/2,60-a,(50.0+(a+70)/3)/50.0,1110,0,50,200);
		drawImage_x2(scr,img.back,-(a+70),-50,(50.0+(a+70)/2)/50.0,960,0,50,300);
		drawImage_x2(scr,img.back,270+(a+70),13,(50.0+(a+70)/2)/50.0,1010,0,50,300);
		drawImage_x2(scr,img.back,35+a,65+a*2,(137.0-a/2)/160.0,1160,100+(((a+70)/2)%2)*50,160,50);
		drawImage_x2(scr,img.back,35+a,65+a*2,(137.0-a/2)/160.0,1160,0,160,100);
	}
	else if(cn<190){
		a=cn-120;
		drawImage(scr,img.back,0,20,0,0,320,200,255);
		if(cn%2==0)drawImage(scr,img.back,165,a+30,960,300,160,20,255);
		drawImage(scr,img.back,140,a+50,960,320,160,80,255);
		if(a>=30 && a<50){
			drawImage(scr,img.back,180-(a-30)*2,180-(a-30)/2,1120,320,100,40,128-(a-30)*6);
			drawImage(scr,img.back,180+(a-30)*2,180-(a-30)/2,1120,360,100,40,128-(a-30)*6);
		}
	}
	else if(cn<270){
		a=cn-190;
		if(a<40)drawImage(scr,img.back,0,20,0,200,320,200,255);
		else drawImage(scr,img.back,0,20,0,400,320,200,255);
		fillRect(scr,0,20,320,200,0,0,0,abs(40-(cn%8)*10));
	}
	else if(cn<330){
		int a=cn-270;
		for(int i=0 ; i<3 ; i++){
			drawImage(scr,img.back,-a*2+80*i,a*6+20-200*i,0,600,320,200,255);
		}
		drawImage(scr,img.back,160,120,960+abs(2-((a/8)%4))*160,550,160,100,255);
		drawImage(scr,img.back,0,70,960+abs(2-((a/8)%4))*160,400,160,150,255);
	}
	else if(cn<400){
		int a=cn-330;
		drawImage(scr,img.back,0,20,0,800,320,200,255);
		if(a<25){
			drawImage(scr,img.back,130,40,1060,650,60,100,255);
			drawImage(scr,img.back,130,90,1120,650,100,50,255);
			drawImage(scr,img.back,115-a*2,40,960,650,50,100,255);
			drawImage(scr,img.back,165+a*2,40,1010,650,50,100,255);
		}else{
			drawImage(scr,img.back,65,40,960,650,50,100,255);
			drawImage(scr,img.back,215,40,1010,650,50,100,255);
			drawImage(scr,img.back,130-(a-25),40+(a-25)*2-(5-abs(5-(a/2)%10))*6,1060,650,60,100,255);
			drawImage(scr,img.back,130+(a-25),90+(a-25)*2,1120,650,100,50,255);
		}
	}
	else if(cn<500){
		a=cn-400;
		drawImage(scr,img.back,0,120,0,1100,320,100,255);
		if(a<80)drawImage(scr,img.back,a/2,120-a,960,750,160,100,255);
		else drawImage(scr,img.back,40,120,960,830,120,20,255);
		drawImage(scr,img.back,0,20,0,1000,320,100,255);
		drawImage(scr,img.back,160,120,160,1100,60,20,255);
		if(a<70)drawImage(scr,img.back,160,160,1220,650,80,50,255);
		else drawImage(scr,img.back,160+(a-70),160-(a-70)*2,1220,650,80,50,255);
		if(a<30)drawImage(scr,img.back,0,120,1120+(a/10)*100,750,100,100,255);
		else if(a<70)drawImage(scr,img.back,(a-30)*2,120,1320,750,100,100,255);
		else drawImage(scr,img.back,80,120,1320-((a-70)/10)*100,750,100,100,255);
	}
	else if(cn<520){
		a=cn-500;
		drawImage(scr,img.back,0,20,0,1200,320,200,255);
		drawImage(scr,img.back,140+a*10,30-a*4,1280,850,100,50,255);
	}
	else if(cn<540){
		a=cn-520;
		drawImage(scr,img.back,a*24-80,70-a*4,1280,850,100,50,255);
		drawImage(scr,img.back,100,20,960,850,120,200,255);
	}
	else if(cn<570){
		a=cn-540;
		drawImage(scr,img.back,60,20,960,1050,200,200,255);
		if(a<10)drawImage(scr,img.back,a*10-80,120-a*4,1280,850,100,50,255);
		else if(a<20)drawImage(scr,img.back,80,80,1470+((a/2)%2)*60,1250,60,60,255);
	}
	else if(cn<600){
		a=cn-570;
		drawImage(scr,img.back,130,20,1080,850,60,200,255);
		if(a>=10)drawImage(scr,img.back,90,120,1140,850+((a/4)%2)*100,140,100,255);
	}
	else if(cn<680){
		a=cn-600;
		if(a>=40)fillRect(scr,80,40,160,100,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,0,20,0,1400,320,200,255);
		if(a<40)drawImage(scr,img.back,80,40,960,1250,160,100,255);
		if(a<40){
			drawImage(scr,img.back,110+(5-abs(5-(a%10))),180,1280,910,35,50,255);
			drawImage(scr,img.back,145-(5-abs(5-((a/3)%10))),180,1315,910,30,50,255);
			drawImage(scr,img.back,175,180+(5-abs(5-(a%10))),1345,910,35,50,255);
		}
		else if(a<50){
			drawImage(scr,img.back,110,180-(5-abs(5-(a-40)))*2,1280,910,35,50,255);
			drawImage(scr,img.back,145,180-(5-abs(5-(a-40)))*2,1315,910,30,50,255);
			drawImage(scr,img.back,175,180-(5-abs(5-(a-40)))*2,1345,910,35,50,255);
		}else{
			drawImage(scr,img.back,110,180,1280,910,35,50,255);
			drawImage(scr,img.back,145,180,1315,910,30,50,255);
			drawImage(scr,img.back,175,180,1345,910,35,50,255);
		}
		if(a<40)drawImage(scr,img.back,190+a,120,1160,1050,100,100,255);
		else drawImage(scr,img.back,220,120,1160,1150,100,100,255);
		if(a<50)drawImage(scr,img.back,0,120,1260,1050,100,100,255);
		else drawImage(scr,img.back,0,120,1260,1150,100,100,255);
	}
	else if(cn<760){
		a=cn-680;
		if(a<15)drawImage(scr,img.back,a*26-320,20,320,200,320,200,255);
		else drawImage(scr,img.back,70,20,320,200,320,200,255);
		if(a>=25){
			fillRect(scr,245,140,60,40,0,0,64,255);
			fillRect(scr,245,140,60,40,255,255,0,128+abs(5-(a%10))*25);
		}
		if(a>=35)drawImage(scr,img.back,225,120,1200,1250,80,80,255);
		else if(a>=25)drawImage(scr,img.back,225,120,1120,1250,80,80,255);
		drawImage(scr,img.back,0,20,320,0,320,200,255);
		if(a>=40)drawImage(scr,img.back,0,70,1120,1350,160,150,255);
		else drawImage(scr,img.back,0,70,960,1350,160,150,255);
	}
	else if(cn<800){
		a=cn-760;
		drawImage(scr,img.back,0,20,320,400,320,200,255);
		if(a>=15){
			drawImage(scr,img.back,100,70,1280,1350,160,150,255);
			drawImage(scr,img.back,100,100,960,1500,160,60,abs(5-(a%10))*25);
		}
	}
	else if(cn<900){
		a=cn-800;
		if(a>=40)fillRect(scr,0,20,260,200,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,0,20,320,600,320,200,255);
		if(a<40)drawImage(scr,img.back,0,20,1320,0,260,200,255);
		if(a>=50)drawImage(scr,img.back,105,90,1180,1500,60,60,255);
		else if(a>=40)drawImage(scr,img.back,105,90,1120,1500,60,60,255);
		if(a>=65)drawImage(scr,img.back,190,140,1300,1500,60,80,255);
		else if(a>=55)drawImage(scr,img.back,190,140,1240,1500,60,80,255);
		if(a>=60)fillRect(scr,0,20,320,200,255,255,0,(a-60)*6);
	}
	else if(cn<1000){
		a=cn-900;
		if(a>=40)fillRect(scr,0,20,160,140,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,0,20,320,800,320,200,255);
		if(a<60)drawImage(scr,img.back,200,20,1440,520,60,80,255);
		else if(a<65)drawImage(scr,img.back,200,20,1500,520,60,80,255);
		else if(a<80)drawImage(scr,img.back,200,20,1560,520,60,80,255);
		else drawImage(scr,img.back,200+(a-80)*8,20-(a-80)*3,1560,520,60,80,255);
		if(a<40){
			drawImage(scr,img.back,50+a,150-a*2,1440+abs(2-((a/8)%4))*60,400,60,70,255);
			drawImage(scr,img.back,160-a,100+a*2,1440+abs(2-((a/8)%4))*60,470,60,50,255);
		}else{
			drawImage(scr,img.back,70+(a-40)*10,110-(a-40)*2,1620,400,60,70,255);
			drawImage(scr,img.back,140+(a-40)*10,140-(a-40)*2,1620,470,60,50,255);
		}
	}
	else if(cn<1100){
		a=cn-1000;
		fillRect(scr,0,20,320,200,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,0,20,320,1000,320,200,255);
		drawImage_x2(scr,img.back,20-a/2,60+a/4,45/60.0,1380,200,60,200);
		drawImage_x2(scr,img.back,20-a,70-(int)(sin(a/30.0)*40),120/160.0,1220,200,160,200);
		drawImage_x2(scr,img.back,65-a/2,60+a/4,75/100.0,1440,200,100,200);
		drawImage(scr,img.back,160-(int)(a*0.7),40+a/4,1380,200,60,200,255);
		drawImage(scr,img.back,160-a*2,60-(int)(sin(a/30.0)*40),1220,200,160,200,255);
		drawImage(scr,img.back,220-(int)(a*0.7),40+a/4,1440,200,100,200,255);
		if((a>=30 && a<40 || a>=80) && a%10>=5)fillRect(scr,0,20,320,200,255,255,0,(5-a%5)*30);
	}
	else if(cn<1120){
		a=cn-1100;
		drawImage(scr,img.back,0,170,1420,800,220,50,255);
		drawImage(scr,img.back,180,20,1440,600,140,200,255);
		drawImage_x2(scr,img.back,80+a*4,180-a*4,(a*4.0)/70.0,1300,650,70,70);
	}
	else if(cn<1170){
		a=cn-1120;
		for(int i=3 ; i>=0 ; i--){
			drawImage_x2(scr,img.back,150-(a-i*10)*12,120-(a-i*10)*5,(a-i*10)*10/200.0,1380,850,200,160);
			drawImage_x2(scr,img.back,150+(a-i*10-5)*2,120-(a-i*10-5)*5,(a-i*10-5)*10/200.0,1580,850,200,160);
		}
		if(a<30)drawImage_x2(scr,img.back,125,-50+a*3,1,1300+((a/5)%2)*70,650,70,70);
		else drawImage_x2(scr,img.back,125+(a-30)*3,20+(a-30)*10,(70.0-(a-30)*7)/70.0,1300+((a/5)%2)*70,650,70,70);
	}
	else if(cn<1200){
		a=cn-1170;
		drawImage(scr,img.back,0,20,320,1200,320,200,255);
		drawImage(scr,img.back,175-a*6,143-a*6,1795-a*6,1523-a*6,a*12,a*12,255);
	}
	else if(cn<1250){
		a=cn-1200;
		if(loadtime<1200){
			drawImage(scr,img.back,0,20,320,1200,320,200,255);
			drawImage(scr,img.back,0,20,1620,1400,320,200,255);
			drawImage(img.scr2,scr,0,0,0,20,320,200,255);
			loadtime=cn;
		}
		drawImage_x2(scr,img.scr2,-a*8,20-a*4,(320.0+a*40)/320.0,0,0,320,200);
	}
	else if(cn<1350){
		a=cn-1250;
		int dx=(int)(cos(a*0.3)*(100-a)/10.0);
		int dy=(int)(sin(a*0.3)*(100-a)/10.0);
		if(a<20){
			drawImage_x2(scr,img.back,120+dx,190-a+dy,0.5,1460,1010,160,120);
			drawImage_x2(scr,img.back,140+dx,180-a+dy,40/320.0,320,1400,320,200);
			drawImage(scr,img.back,90-a+dx,130+a*2+dy,1360,1010,100,120,255);
			drawImage(scr,img.back,150+a+dx,130+a*2+dy,1360,1010,100,120,255);
			drawImage(scr,img.back,20-a+dx,120+a+dy,1360,1010,100,120,255);
			drawImage(scr,img.back,220+a+dx,120+a+dy,1360,1010,100,120,255);
		}
		else if(a<40){
			drawImage_x2(scr,img.back,120+dx,170+dy,0.5,1460,1010,160,120);
			drawImage_x2(scr,img.back,140+dx,160+dy,40/320.0,320,1400,320,200);
			drawImage(scr,img.back,70+dx,170+dy,1360,1010,100,120,255);
			drawImage(scr,img.back,170+dx,170+dy,1360,1010,100,120,255);
			drawImage(scr,img.back,0+dx,140+dy,1360,1010,100,120,255);
			drawImage(scr,img.back,240+dx,140+dy,1360,1010,100,120,255);
		}else{
			drawImage_x2(scr,img.back,120-(a-40)*12+dx,170-(a-40)*3+dy,(80.0+(a-40)*24)/160.0,1460,1010,160,120);
			drawImage_x2(scr,img.back,140-(a-40)*6+dx,160-(a-40)*5+dy,(40.0+(a-40)*12)/320.0,320,1400,320,200);
			drawImage_x2(scr,img.back,70-(a-40)*3+dx,170+(a-40)*2+dy,(100.0+(a-40))/100.0,1360,1010,100,120);
			drawImage_x2(scr,img.back,170+(a-40)*2+dx,170+(a-40)*2+dy,(100.0+(a-40))/100.0,1360,1010,100,120);
			drawImage_x2(scr,img.back,-(a-40)*7+dx,140-(a-40)*3+dy,(100.0+(a-40)*2)/100.0,1360,1010,100,120);
			drawImage_x2(scr,img.back,240+(a-40)*5+dx,140-(a-40)*3+dy,(100.0+(a-40)*2)/100.0,1360,1010,100,120);
		}
	}
	else if(cn<1450){
		a=cn-1350;
		drawImage(scr,img.back,0,120,1360,1140,320,110,255);
		if(a<50){
			drawImage(scr,img.back,50,150,1320,1250,50,50,255);
			drawImage(scr,img.back,160,90,1320,1300,50,50,255);
			drawImage(scr,img.back,240,170,1320,1300,50,50,255);
			drawImage(scr,img.back,(a-40)*13,(a-40)*8,1300,650,70,70,255);
		}else{
			drawImage(scr,img.back,50-(a-50)*5,150-(a-50)*3,1370+((a/3)%2)*50,1250,50,50,255);
			drawImage(scr,img.back,160+(a-50)*5,90-(a-50)*3,1370+((a/3)%2)*50,1300,50,50,255);
			drawImage(scr,img.back,240+(a-50)*5,170-(a-50)*3,1370+((a/3)%2)*50,1300,50,50,255);
			drawImage(scr,img.back,140,90,1140,850+((a/3)%2)*100,140,100,255);
		}
	}
	else if(cn<1500){
		a=cn-1450;
		fillRect(scr,60,70,200,120,255,255,0,128+abs(5-(a%10))*25);
		fillRect(scr,65,115,35,35,0,0,0,255);
		drawImage(scr,img.back,0,20,640,0,320,200,255);
		drawImage(scr,img.back,210,30,1470+((a/3)%2)*60,1250,60,60,255);
	}
	else if(cn<1560){
		a=cn-1500;
		drawImage(scr,img.back,0,20,640,200,320,200,255);
		if(a<15)drawImage(scr,img.back,260,a*3,1470+((a/3)%2)*60,1250,60,60,255);
		if(a>=30 && a<36)drawImage(scr,img.back,70-(a-30)*5,60+(a-30)*5,1470+((a/3)%2)*60,1250,60,60,255);
		if(a>=50)drawImage(scr,img.back,0,100,1470+((a/3)%2)*60,1250,60,60,255);
	}
	else if(cn<1640){
		a=cn-1560;
		drawImage(scr,img.back,0,20,640,400,320,125,255);
		for(int i=0 ; i<3 ; i++)drawImage(scr,img.back,30-a*2+i*80,-5-a*3+i*150,1440,1350,100,150,128);
		drawImage(scr,img.back,0,145,640,525,320,75,255);
		if(a%20<10)drawImage(scr,img.back,220,150,1540,1310,60,50,255);
		if(a>=20 && a<40)drawImage(scr,img.back,230,90,1470+((a/3)%2)*60,1250,60,60,255);
	}
	else if(cn<1700){
		a=cn-1640;
		fillRect(scr,60,70,200,120,255,255,0,128+abs(5-(a%10))*25);
		if(a<30)fillRect(scr,65,115,35,35,0,0,0,255);
		drawImage(scr,img.back,0,20,640,0,320,200,255);
		drawImage(scr,img.back,210,30,1470+((a/3)%2)*60,1250,60,60,255);
	}
	else if(cn<1760){
		a=cn-1700;
		fillRect(scr,80,60,100,60,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,0,20,640,600,320,200,255);
		if(a%6<3)drawImage(scr,img.back,190,100,1540,1360,70,120,255);
	}
	else if(cn<1860){
		a=cn-1760;
		if(a<50)drawImage(scr,img.back,120,90,1520,1500,80,40,55+abs(5-(a%10))*40);
		else drawImage(scr,img.back,240,120,1520,1500,80,50,55+abs(5-(a%10))*40);
		drawImage(scr,img.back,150,100,1360,1500,160,80,255);
		drawImage(scr,img.back,50,80,1360,1500,160,80,255);
		for(int i=0 ; i<4 ; i++){
			drawImage_x2(scr,img.back,40-i*80+a*2,180-i*40+a/2,(200.0+i*20-a)/140.0,1580,0,140,100);
		}
		if(a<50)drawImage(scr,img.back,220,130+abs(10-a%20),1580,100,100,100,255);
		else if(a<70)drawImage(scr,img.back,220,130+abs(10-a%20),1680,100,100,100,255);
		else drawImage(scr,img.back,220,130+abs(10-a%20),1780,100,100,100,255);
	}
	else if(cn<1940){
		a=cn-1860;
		drawImage(scr,img.back,0,20,640,800,320,200,255);
		if(a%8<4)drawImage(scr,img.back,80,20,1540,200,160,200,255);
		if(a>=30)drawImage(scr,img.back,160,20,1720,0,160,100,255);
	}
	else if(cn<2000 || cn>=2080 && cn<2180){
		a=cn-1940;
		drawImage(scr,img.back,150-a/2,40,1880,0,70,150,255);
		drawImage(scr,img.back,155-a/2,50,1470+((a/3)%2)*60,1250,60,60,255);
		for(int i=0 ; i<5 ; i++){
			drawImage(scr,img.back,i*120-a,140,1880,150,120,50,255);
		}
		//ビル１
		fillRect(scr,100-a*2,70,160,150,0,0,64,255);
		fillRect(scr,100-a*2,75,160,145,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,100-a*2,20,1700,200,160,200,255);
		//ビル２
		fillRect(scr,350-a*2,170,160,50,0,0,64,255);
		fillRect(scr,350-a*2,170,160,50,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,350-a*2,170,1700,350,160,50,255);
		fillRect(scr,350-a*2,70,100,100,0,0,64,255);
		fillRect(scr,350-a*2,75,100,95,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,350-a*2,20,1700,200,100,150,255);
		if(a<190){
			fillRect(scr,450-a*2,70,60,100,0,0,64,255);
			fillRect(scr,450-a*2,75,60,95,255,255,0,128+abs(5-(a%10))*25);
			drawImage(scr,img.back,470-a*2,120,1680+abs(2-(a/10)%4)*20,550,20,30,255);
			drawImage(scr,img.back,450-a*2,20,1800,200,60,150,255);
		}else{
			drawImage(scr,img.back,470-a*2,120,1680+abs(2-(a/10)%4)*20,550,20,30,255);
			drawImage(scr,img.back,450-a*2,70,1860,250,60,100,255);
			drawImage(scr,img.back,480-a*2+(a-190),90+(a-190)*5,1860,350,40,40,255);
			drawImage(scr,img.back,480-a*2+(a-190)*3,90+(a-190)*(a-190)/5,1860,350,40,40,255);
			drawImage(scr,img.back,480-a*2+(a-190)*3,60+(a-200)*(a-200)/5,1860,350,40,40,255);
		}
		for(int i=0 ; i<4 ; i++){
			drawImage(scr,img.back,300-a*4+i*80,20-a*3+i*150,1440,1350,100,150,128);
		}
		//ビル３
		fillRect(scr,600-a*2,70,160,150,0,0,64,255);
		fillRect(scr,600-a*2,75,160,145,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,600-a*2,20,1700,200,160,200,255);
		if(a<30)drawImage(scr,img.back,210-a*2,122,1860,200,40,30,255);
		else{
			drawImage(scr,img.back,210-a*2,122,1900,200,40,30,255);
			drawImage(scr,img.back,210-a*2+(a-30)*3,125+(a-30)*5,1940,200,60,60,255);
			drawImage(scr,img.back,210-a*2+(a-30)*2,125+(a-30)*2,1940,200,60,60,255);
			drawImage(scr,img.back,210-a*2+(a-30)*4,125+(a-30)*3,1940,200,60,60,255);
		}
	}
	else if(cn<2080){
		a=cn-2000;
		fillRect(scr,0,20,320,200,255,255,0,128+abs(5-(a%10))*25);
		drawImage(scr,img.back,70,40,1680+abs(2-(a/10)%4)*80,400,80,150,255);
		drawImage(scr,img.back,0,20,640,1000,320,200,255);
		if(a<30)drawImage(scr,img.back,240,110,1920,400,30,60,255);
		else if(a<40)drawImage(scr,img.back,240,110,1950,400,30,60,255);
		else drawImage(scr,img.back,240,110+(a-40)*4,1920,460,30,80,255);
		if(a<20 && a%10<5)fillRect(scr,0,20,320,200,255,255,0,128-(a%5)*25);
		if(a>=55)fillRect(scr,0,20,320,200,255,255,0,(a%5)*25);
	}
	else if(cn<2380){
		a=cn-2180;
		for(int i=0 ; i<200 ; i++){
			drawImage(scr,img.back,(int)(sin((a+i)/5.0)*4),i+20,640,1200+i,320,1,255);
		}
		if(a>=170)drawImage(scr,img.back,150,20,1640+((a-170)/10)*110,740,110,110,255);
		for(int i=0 ; i<5 ; i++){
			drawImage(scr,img.back,220+i*80-a*2,i*150-a*3,1440,1350,100,150,128);
			drawImage(scr,img.back,240+i*80-(a+50)*3,i*150-(a+75)*3,1440,1350,100,150,128);
		}
		drawImage_x2(scr,img.back,10+abs(40-a%80)/4,120,(100.0+abs(40-a%80)/2)/140.0,1580+abs(2-(a/15)%4)*140,600,140,140);
		drawImage_x2(scr,img.back,200+abs(40-(a+10)%80)/4,100,(120.0+abs(40-(a+10)%80)/2)/140.0,1580+abs(2-(a/15)%4)*140,600,140,140);
		drawImage_x2(scr,img.back,70+abs(40-(a+30)%80)/4,80,(140.0+abs(40-(a+30)%80)/2)/140.0,1580+abs(2-(a/15)%4)*140,600,140,140);
		if(a<120){
			fillRect(scr,0,20,320,200,0,0,0,(120-a)*2);
			drawImage(scr,img.back,120-a,100,375,260,120,120,(120-a)*2);
			if(a%6<3)drawImage(scr,img.back,120-a,100,1730,1145,110,120,(120-a));
			drawImage(scr,img.back,210-a,120,1680,1145,50,100,(120-a)*2);
		}
	}
	else if(cn<2600){
		a=cn-2380;
		drawImage(scr,img.back,100,30+a/12,1840,940,120,120,(int)(a*1.4));
		drawImage(scr,img.back,40-a/12,60-a/6,1780,940,60,120,(int)(a*1.4));
		drawImage(scr,img.back,220+a/12,60-a/6,1960,940,60,120,(int)(a*1.4));
		drawImage(scr,img.back,125,70,1880,0,70,150,255);
		drawImage(scr,img.back,50,130-a/8,1840,1150+((count/10)%2)*50,110,50,255);
		drawImage(scr,img.back,160,130-a/8,1950,1150+((count/10)%2)*50,110,50,255);
		for(int i=0 ; i<3 ; i++)drawImage(scr,img.back,i*120,170,1880,150,120,50,255);
		if(a>=100)drawImage(scr,img.back,0,20,1620,1060,320,85,255);
		else if(a>=40)drawImage(scr,img.back,0,20,1620,1060,320,85,(a-40)*4);
		int x[30]={250,20,110,180,280,0,200,270,70,40,260,160,220,260,70,140,10,170,150,0,250,30,110,260,70,140,280,0,190,170};
		int y[30]={180,200,160,190,140,170,140,200,190,160,130,170,200,160,140,190,150,130,140,200,140,130,120,170,160,190,130,130,170,140};
		for(int i=0 ; i<4 ; i++){
			if(a/7-i<0 || a/7-i>=30)continue;
			drawImage(scr,img.back,x[a/7-i],y[a/7-i],1900-i*40,900,40,40,255);
		}
	}
	else if(cn<2740){
		a=cn-2600;
		fillRect(scr,0,20,320,30,0,0,0,255);
		fillRect(scr,0,190,320,30,0,0,0,255);
		drawImage(scr,img.back,0,80,1590,1270,320,40,255);
		drawImage(scr,img.back,0,30-a/7,640,1400,320,200,255);
		if(a>=30)drawImage(scr,img.back,70,120,1620,1310,180,90,255);
	}else{
		a=cn-2740;
		drawImage(scr,img.back,70,20,320,200,320,200,255);
		fillRect(scr,245,140,60,40,255,255,0,255);
		drawImage(scr,img.back,225,120,1200,1250,80,80,255);
		drawImage(scr,img.back,0,20,320,0,320,200,255);
		drawImage(scr,img.back,0,70,1120,1350,160,150,255);
		fillRect(scr,0,20,320,200,0,0,255,128);
		drawImage(scr,img.back,80,190,960,1560,220,30,255);
		if(a<450){
			drawImage(scr,img.back,110,30,1910,1250,110,40,255);
			if(a<220)drawImage(scr,img.back,50,120,1800,1310,220,25,255);
			else{
				drawImage(scr,img.back,50,90,1800,1335,220,25,255);
				drawImage(scr,img.back,50,140,1800,1360,220,25,255);
			}
		}
	}
	if(cn>2020 && cn<2240)drawImage(scr,img.back,150,160,1780,850,200,50,255);

	if(cn<300)drawImage(scr,img.back,270,30,1120,300,40,20,255);

	if(cn>50 && cn<200)drawImage(scr,img.back,160,50,1940,1400,120,25,255);
	if(cn>250 && cn<400)drawImage(scr,img.back,30,180,1940,1425,120,25,255);
	if(cn>750 && cn<900)drawImage(scr,img.back,30,50,1940,1450,120,25,255);
	if(cn>1050 && cn<1200)drawImage(scr,img.back,30,180,1940,1475,120,25,255);
	if(cn>1300 && cn<1450)drawImage(scr,img.back,30,50,1940,1500,120,25,255);
	if(cn>1650 && cn<1800)drawImage(scr,img.back,30,50,1940,1525,120,25,255);
	if(cn>2250 && cn<2400)drawImage(scr,img.back,160,50,1940,1550,120,25,255);

	fillRect(scr,0,0,320,20,0,0,0,255);
	fillRect(scr,0,220,320,20,0,0,0,255);
//	sprintf_s(str,"%d",cn);
//	TextOut(scr,0,0,str,5);
}

// scene0.png - scene6.png
void drawWeeklyComic(SDL_Surface* scr){
	fillRect(scr,0,0,320,240,0,0,0,255);
	if(gd.week==0){
		int a,b,c;
		if(count<200)a=count-30;
		else if(count<340)a=170;
		else a=count-170;
		if(count<200)b=((count/10)%2)*60;
		else if(count<220)b=120;
		else if(count<320)b=180;
		else if(count<340)b=120;
		else b=((count/5)%2)*60;
		if(count<200)c=150;
		else if(count<300)c=(int)(1.5*(300-count));
		else c=0;
		fillRect(scr,0,45,320,150,128,128,255,255);
		drawImage(scr,img.back,200,50,300,170,15,15,255);
		drawImage(scr,img.back,130-count/20,45,120,170,100,50,255);
		drawImage(scr,img.back,0,45,0,20,320,150,255);
		drawImage(scr,img.back,a,140,((count/10)%2)*60,170,60,60,255);
		drawImage(scr,img.back,a,115,b,230,60,60,255);
		if(count>200&&count%20<10 || count>300)drawImage(scr,img.back,120,65,240,210,80,80,255);
		fillRect(scr,0,45,320,150,0,0,0,c);
	}
	else if(gd.week==1){
		fillRect(scr,0,45,320,150,192,192,192,255);
		drawImage(scr,img.back,0,45,((count/2)%160)*2,170+(count/2)%160,320,150,64);
		drawImage(scr,img.back,0,45,((count/3)%160)*2,170+(count/3)%160,320,150,64);
		drawImage(scr,img.back,0,45,0,20,320,150,255);
		if(count>=140 && count<200)drawImage(scr,img.back,94,125,320,75,52,70,255);
		if(count<100)drawImage(scr,img.back,30,110,440,115,25,40,255);
		else if(count<130)drawImage(scr,img.back,30+(count-100),110+(count-100)*2,440,115,25,40,255);
		else if(count<200)drawImage(scr,img.back,60+(count-130),170,440,115,25,40,255);
		if(count>=150 && count<200)drawImage(scr,img.back,146,125,372,75,18,70,255);
		drawImage(scr,img.back,0,120,320,0,320,75,255);
		if(count>=260 && count<300)drawImage(scr,img.back,0,130,500,115,120,60,255);
		if(count>=300 && count<340)drawImage(scr,img.back,0,130,500,175,120,60,255);
		if(count>500)drawImage(scr,img.back,0,130,500,115+((count/10)%2)*60,120,60,255);
		if(count<340){
			if(count%40<20)drawImage(scr,img.back,245,145,390,75,50,40,255);
		}
		else if(count<440){
			if(count%40<20)drawImage(scr,img.back,245,145,440,75,50,40,255);
			else drawImage(scr,img.back,245,145,490,75,50,40,255);
			drawImage(scr,img.back,260,120,390,120,30,30,255);
		}else{
			if(count%40<20)drawImage(scr,img.back,245,145,540,75,50,40,255);
			else drawImage(scr,img.back,245,145,590,75,50,40,255);
			drawImage(scr,img.back,285,145,420,120,20,20,255);
		}
	}
	else if(gd.week==2){
		int a=0,b=120,sx,sy=65,cx,cy=95;
		int fx1,fy1,fx2,fy2,fx3,fy3,fx4,fy4;

		cx=260-count/10;
		if(count>=100 && count<400)cy+=(count-100)/6;
		else if(count>=400 && count<500)cy+=(500-count)/2-abs(4-count%10)*3;
		if(count>=400 && count<450){a=120;cx-=(count-400);}
		else if(count>=450 && count<500){a=180;cx+=(count-450);}
		else a=((count/10)%2)*60;

		if(count>=200)sy+=(count-200)/6;
		if(count>=500)sy+=(count-500)/2;
		if(sy<155)sx=40+count/10;
		else sx=80+count/10;

		if(count<100){
			fx1=320-count*2;
			fy1=160-count*count/100;
		}
		else if(count<180){
			fx1=120;
			fy1=60+(count-100)/4;
			b=200;
		}else{
			fx1=120-(count-180)*2;
			fy1=100-(count-180)*(count-180)/100;
		}
		fx2=400-count*2;
		fy2=300-count+abs(count%40-20);
		fx3=460-count*2;
		fy3=270-count+abs((count+10)%40-20);
		fx4=700-count*2;
		fy4=400-count;

		drawImage(scr,img.back,0,45,0,20,320,150,255);
		if(sy<155)drawImage(scr,img.back,sx,sy,0,280,80,40,255);
		else drawImage(scr,img.back,sx,sy,80,280,40,40,255);
		drawImage(scr,img.back,count/10-100,90,0,170,320,50,255);
		drawImage(scr,img.back,fx1,fy1,b+((count/10)%2)*40,280,40,40,255);
		drawImage(scr,img.back,fx2,fy2,240,220,30,30,255);
		drawImage(scr,img.back,fx3,fy3,240,220,30,30,255);
		drawImage(scr,img.back,fx4,fy4,240+((count/10)%2)*30,250,30,30,255);
		drawImage(scr,img.back,cx,cy,a,220,60,60,255);
		drawImage(scr,img.back,220-count/10,140,0,170,320,50,255);
	}
	else if(gd.week==3){
		int a=(count/10)%3;
		drawImage(scr,img.back,0,45,0,20,320,150,255);
		drawImage(scr,img.back,60,140,230,170+abs(2-(count/10)%4)*45,90,45,255);
		if(count<270)drawImage(scr,img.back,220,count/3,0,260+a*20,60,20,255);
		else if(count<350)drawImage(scr,img.back,220,90,0,260+a*20,60,20,255);
		if(count>=300 && count<350)drawImage(scr,img.back,223,97,0,170,50,90,255);
		else if(count>=350 && count<500){
			if(count%50<25){
				drawImage(scr,img.back,210,115,60+((count-325)/50)*40,260+a*40,40,40,255);
				drawImage(scr,img.back,230,125,100,170,50,60,255);
			}else{
				drawImage(scr,img.back,230,110,60+((count-325)/50)*40,260+a*40,40,40,255);
				drawImage(scr,img.back,230,125,50,170,50,60,255);
				if(count%50<40)drawImage(scr,img.back,230,110,190,260+((count%50-25)/5)*40,40,40,255);
			}
		}else{
			drawImage(scr,img.back,250,165,150,170+abs(2-(count/20)%4)*30,60,30,255);
		}
		if(count>=400){
			if(count<600)fillRect(scr,0,45,320,150,255,0,0,(int)((count-400)*0.48));
			else fillRect(scr,0,45,320,150,255,0,0,96);
		}
	}
	else if(gd.week==4){
		int a=0,b=0,c=0,d=0,e=0;
		if(count<300){a=270-count/10;b=45+count/10;}
		else if(count<380){a=240;b=75;}
		else if(count<400){a=240;b=75-(10-abs(count-390))*2;}
		else{
			a=250+(count-400);b=75-(count-400);
			c=25;
			if(count%100<20){d=(count-400)/100*20+count%20;e=((count/2)%2)*40;}
			else d=(count-400)/100*20+20;
		}
		drawImage(scr,img.back,0,45,0,20+((count/10)%2)*150,320,150,255);
		drawImage(scr,img.back,a,b,c,320,25,25,255);
		if(count>=300)drawImage(scr,img.back,60,75,((count/10)%3)*80,360,80,80,255);
		if(count>=380)drawImage(scr,img.back,160,125,90,320,40,40,255);
		else if(count>=340)drawImage(scr,img.back,160,125,50,320,40,40,255);
		if(count>=420)drawImage(scr,img.back,190,45,170,320,40,40,255);
		else if(count>=380)drawImage(scr,img.back,190,45,130,320,40,40,255);
		if(count>=400)drawImage(scr,img.back,190+d,55+d,80+e,440,40,40,255);
		if(count>=360)drawImage(scr,img.back,170+d,100-d,e,440,40,40,255);
		drawImage(scr,img.back,260,115,260,320+((count/10)%2)*80,60,80,255);
		if(count>=320 && count<360)drawImage(scr,img.back,220,60,210,320,20,20,255);
		if(count>=380 && count<420)drawImage(scr,img.back,220,60,230,320,20,20,255);
	}
	else if(gd.week==5){
		int a=72;
		if(count>=350)a-=(count-350)/10;
		for(int i=0 ; i<100 ; i++)fillRect(scr,0,i+45,320,1,i,0,64,255);
		drawImage(scr,img.back,150,a,370,60,20,90,255);
		if(count>=300){
			drawImage_x2(scr,img.back,140-(count-300)/7,120-(count-300)/9,1+(count-300)/1000.0,420,60,40,40);
			drawImage_x2(scr,img.back,140-(count-300)/4,120-(count-300)/5,1+(count-300)/500.0,420,60,40,40);
			drawImage_x2(scr,img.back,140-(count-300)/3,120-(count-300)/3,1+(count-300)/200.0,420,60,40,40);
			drawImage_x2(scr,img.back,140+(count-300)/7,120-(count-300)/9,1+(count-300)/1000.0,420,60,40,40);
			drawImage_x2(scr,img.back,140+(count-300)/4,120-(count-300)/5,1+(count-300)/500.0,420,60,40,40);
			drawImage_x2(scr,img.back,140+(count-300)/3,120-(count-300)/3,1+(count-300)/200.0,420,60,40,40);
		}
		drawImage(scr,img.back,0,45,0,20,320,150,255);
		if(count>230 && count<260 && count%10<5)drawImage(scr,img.back,180,45,340,20,60,30,255);
		if(count>280 && count<300 && count%10<5)drawImage(scr,img.back,160,45,320,20,20,40,255);
		if(count%100>=50)drawImage(scr,img.back,90,145,320,60,50,50,255);
		if(count>=450)drawImage(scr,img.back,90,115,390,60,30,30,255);
		if(count<200 || count>700)drawImage(scr,img.back,240,45,abs(2-(count/10)%4)*80,170,80,150,255);
		else if(count<230)drawImage(scr,img.back,240,45,160+((count-200)/10)*80,170,80,150,255);
		else if(count<270)drawImage(scr,img.back,240,45,400+(((count-230)/10)%2)*80,170,80,150,255);
		else drawImage(scr,img.back,240,45,400,170,80,150,255);
	}
	else if(gd.week==6){
		if(count>=300 && count<400){
			drawImage_x2(scr,img.back,(int)((count-300)/2.5)-40,(count-300)/4+20,1.25-(count-300)/400.0,0,170,320,150);
			drawImage(scr,img.back,160+(count-300)/2,60+(count-300)/2,((count/5)%2)*100,320,100,100,255);
			if(CHAR_CODE==JAPANESE)drawImage(scr,img.back,160+(count-300)/2,60+(count-300)/2,((count/5)%2)*100,420,100,50,255);
			else drawImage(scr,img.back,160+(count-300)/2,60+(count-300)/2,((count/5)%2)*100,470,100,50,255);
		}else{
			int a=0,b=0,c=160;
			if(count<200)a=count/2;
			else if(count>=200 && count<400)a=100;
			else if(count>=400 && count<450)a=100+(count-400);
			else a=150+(count-450)*3;
			if(count<300)b=count/2-8;
			else if(count<425)b=142+(count-400)/2;
			else b=154+(count-425)*2;
			if(count>=250 && count<260 || count>=450)c-=abs(count%10-4)*3;
			drawImage(scr,img.back,0,45,0,20,320,150,255);
			if(count>=250 && count<300)drawImage(scr,img.back,a,c,200,320,30,30,255);
			else if(count>=450)drawImage(scr,img.back,a,c,230,320,30,30,255);
			else drawImage(scr,img.chr,a,c,15,0,15,30,255);
			drawImage(scr,img.chr,b,165,((count/5)%2)*30,55,30,30,255);
			if(CHAR_CODE==JAPANESE)drawImage(scr,img.chr,b,165,((count/5)%2)*30,115,30,15,255);
			else drawImage(scr,img.chr,b,165,((count/5)%2)*30,145,30,15,255);
		}
	}
	fillRect(scr, 0,0,320,start+45, 0,0,0,255);
	fillRect(scr, 0,195-start,320,start+45, 0,0,0,255);
	drawImage(scr,img.back,170,205,0,0,150,19,255);
}


// ending.png
void drawEndingAnim(SDL_Surface *scr, int cn){
	int a,b;
	if(cn<250){
		a=cn;
		fillRect(scr,0,0,320,240,0,0,0,255);
		drawImage(scr,img.back,60,20,120,760,200,200,255);
		drawImage(scr,img.back,90,85-a/10,120,0,120,160,255);
		drawImage(scr,img.back,180+a/10,60+a/20,240,0,120,160,255);
		drawImage(scr,img.back,20-a/10,60+a/30,0,0,120,160,255);
	}
	else if(cn<550){
		a=cn-250;
		fillRect(scr,0,20,320,200,192,192,255,255);
		drawImage(scr,img.back,190+a/16,70-a/2+a/8,0,160,120,60,128);
		drawImage(scr,img.back,110+a/16,160-a/2+a/8,120,160,120,60,128);
		drawImage(scr,img.back,140+a/16,260-a/2+a/8,240,160,120,60,128);
		drawImage(scr,img.back,180,60-a/2,0,160,120,60,255);
		drawImage(scr,img.back,100,150-a/2,120,160,120,60,255);
		drawImage(scr,img.back,130,250-a/2,240,160,120,60,255);
		drawImage(scr,img.back,30,-280+a,0,220,80,140,255);
		drawImage(scr,img.back,80,-120+a,80,220,80,140,255);
		drawImage(scr,img.back,50,40+a,160,220,80,140,255);
		if(a>20 && a<230)drawImage(scr,img.back,180,100,800,0,120,60,255);
	}
	else if(cn<1050){
		a=cn-550;
		fillRect(scr,0,20,320,200,0,0,0,255);
		for(int i=0 ; i<3 ; i++){
			drawImage(scr,img.back,-a+i*320,110,0,380,320,100,255);
		}
		if(a%250<20)b=(20-a%250)*16;
		else if(a%250<230)b=0;
		else b=-(a%250-230)*16;
		drawImage(scr,img.back,b+20,40,800,(a/250)*120+120,120,60,255);
		drawImage(scr,img.back,b+180,40,800,(a/250)*120+180,120,60,255);
	}
	else if(cn<1550){
		a=cn-1050;
		fillRect(scr,0,20,320,200,0,0,0,255);
		drawImage(scr,img.back,30,60,0,480,150,120,255);
		if(a%100>=50 && a%100!=80 && a%100!=90)drawImage(scr,img.back,69,60,150,480,45,120,255);
		drawImage(scr,img.back,100,60+abs(10-(a/3)%20),200,560,28,25,255);
		drawImage(scr,img.back,30,135,260,480+((a/10)%2)*45,50,45,255);
		drawImage(scr,img.back,130,135,260,525-((a/10)%2)*45,50,45,255);
		drawImage(scr,img.back,85+abs(10-(a/20)%20),100+abs(20-a%40),200,480,60,80,255);
		fillRect(scr,85,180,70,20,0,0,0,255);
		if(a%250<20)b=(20-a%250)*10;
		else if(a%250<230)b=0;
		else b=-(a%250-230)*10;
		drawImage(scr,img.back,190,b+40,800,(a/250)*120+360,120,60,255);
		drawImage(scr,img.back,190,b+140,800,(a/250)*120+420,120,60,255);
	}
	else if(cn<2050){
		a=cn-1550;
		int b=40,c=0;
		if(a%100<8 || a%100>=10 && a%100<18)b+=4-abs(4-a%8);
		if(a>=100 && a<200)c=(a-100)/5;
		if(a>=200 && a<250)c=20;
		if(a>=250 && a<350)c=20-(a-250)/5;
		fillRect(scr,0,40,320,90,192,192,192,255);
		fillRect(scr,0,120+c/2,320,80,32,32,255,255);
		for(int i=0 ; i<4 ; i++)drawImage(scr,img.back,i*120-(a%60)*2,145,0,833,120,20,255);
		for(int i=0 ; i<4 ; i++)drawImage(scr,img.back,i*120-(a%30)*4,160-c/2,0,804,120,30,255);
		for(int i=0 ; i<4 ; i++)drawImage(scr,img.back,i*120-(a%20)*6,170-c,0,760,120,44,255);
		drawImage(scr,img.back,-a/10,109+c/2,0,854,120,15,255);
		drawImage(scr,img.back,0,b,0,600,320,160,255);
		if(a>=100 && a<120 || a>=140 && a<160 || a>=400 && a<410 || a>=420 && a<430)drawImage(scr,img.back,246,50+b,7,870,7,10,255);
		else if(a>=120 && a<140 || a>=410 && a<420)drawImage(scr,img.back,246,50+b,14,870,7,10,255);
		else drawImage(scr,img.back,246,50+b,0,870,7,10,255);
		drawImage(scr,img.back,320-a*2,60,800,60,120,60,255);
		drawImage(scr,img.back,520-a*2,60,800,600,120,60,255);
		drawImage(scr,img.back,720-a*2,60,800,660,120,60,255);
		drawImage(scr,img.back,920-a*2,60,800,720,120,60,255);
		fillRect(scr,0,20,320,24,0,0,0,255);
		fillRect(scr,0,200,320,20,0,0,0,255);
	}
	else if(cn<2550){
		a=cn-2050;
		fillRect(scr,0,20,320,200,0,0,0,255);
		drawImage(scr,img.back,180,12+a/40,700,600,100,210,255);
		drawImage(scr,img.back,200,160+a/40,240,220+((a/8)%3)*40,60,40,255);
		drawImage(scr,img.back,195,224-a/6,700,810,30,80,255);
		drawImage(scr,img.back,187,252-a/6,730+((a/2)%2)*35,855,35,30,255);
		drawImage(scr,img.back,180,220-a/6,730,810,50,45,255);
		drawImage(scr,img.back,180,211-a/12,700,890,100,50,255);
		if(a%250<20)b=(20-a%250)*10;
		else if(a%250<230)b=0;
		else b=-(a%250-230)*10;
		drawImage(scr,img.back,20,b+40,800,(a/250)*120+780,120,60,255);
		drawImage(scr,img.back,20,b+140,800,(a/250)*120+840,120,60,255);
	}
	else if(cn<2850){
		a=cn-2550;
		for(int i=0 ; i<200 ; i++)fillRect(scr,0,i+20,320,1,i/3+(300-a)/3,i/3+(300-a)/3,255,255);
		drawImage(scr,img.back,-150,a/3,0,940,320,60,128);
		drawImage(scr,img.back,100,a/2,0,940,320,60,128);
		drawImage(scr,img.back,-100,a,0,940,320,60,128);
		drawImage(scr,img.back,100,a/2+65,410,220,70,160,255);
		drawImage(scr,img.back,180,a/2+125,410,380,70,100,255);
		drawImage(scr,img.back,140,a-145,320,220,90,360,255);
		drawImage(scr,img.back,-100,a-100,0,940,320,60,128);
		drawImage(scr,img.back,0,a*2-300,0,940,320,60,128);
		drawImage(scr,img.back,50,a*3-500,0,940,320,60,128);
		drawImage(scr,img.back,-50,a*4-500,0,940,320,60,128);
	}
	else if(cn<3150){
		a=cn-2850;
		for(int i=0 ; i<200 ; i++)fillRect(scr,0,i+20,320,1,96+i/2,96+i/2,96+i/2,255);
		if(a<150){
			fillRect(scr,0,130+a/10,320,90,0,64,64,255);
			drawImage_x2(scr,img.back,150,80+a/10,20/60.0,320,700,60,180);
			drawImage_x2(scr,img.back,90,105+a/16,60/140.0,320,600,140,100);
			drawImage_x2(scr,img.back,170,105+a/16,60/140.0,460,600,140,100);
			drawImage_x2(scr,img.back,35,110-a/12,100/140.0,320,600,140,100);
			drawImage_x2(scr,img.back,185,110-a/12,100/140.0,460,600,140,100);
			drawImage(scr,img.back,-20,120-a/4,320,600,140,100,255);
			drawImage(scr,img.back,200,120-a/4,460,600,140,100,255);
			drawImage_x2(scr,img.back,160,140+a/50,20/40.0,420,700,40,30);
			drawImage_x2(scr,img.back,130,170-a/6,40/40.0,380,700,40,30);
		}else{
			fillRect(scr,0,145+(a-150),320,75,0,64,64,255);
			if(a>=225)drawImage_x2(scr,img.back,150-(a-150)/2,-17+(a-225)*2,(20.0+(a-150))/60.0,320,700,60,180);
			else drawImage_x2(scr,img.back,150-(a-150)/2,95-(int)((a-150)*1.5),(20.0+(a-150))/60.0,320,700,60,180);
			drawImage_x2(scr,img.back,90-(int)((a-150)*2.5),114+(a-150),(60.0+(a-150)*1.5)/140.0,320,600,140,100);
			drawImage_x2(scr,img.back,170+(a-150),114+(a-150),(60.0+(a-150)*1.5)/140.0,460,600,140,100);
			drawImage_x2(scr,img.back,35-(a-150)*4,98+(a-150)/2,(100.0+(a-150)*2)/140.0,320,600,140,100);
			drawImage_x2(scr,img.back,185+(a-150)*2,98+(a-150)/2,(100.0+(a-150)*2)/140.0,460,600,140,100);
			drawImage_x2(scr,img.back,-20-(a-150)*5,83,(140.0+(a-150)*3)/140.0,320,600,140,100);
			drawImage_x2(scr,img.back,200+(a-150)*3,83,(140.0+(a-150)*3)/140.0,460,600,140,100);
			drawImage_x2(scr,img.back,160,145+(a-150)/2,(20.0+(a-150)*2)/40.0,420,700,40,30);
			drawImage_x2(scr,img.back,130-(int)((a-150)*2.5),145-(a-150)/2,(40.0+(a-150)*4)/40.0,380,700,40,30);
		}
		if(a>=250)fillRect(scr,0,0,320,240,255,255,255,(a-250)*5);
	}
	else if(cn<3750){
		a=cn-3150;
		int b=0;
		if(a>=500)b=255;
		else if(a>=245)b=(a-245);
		if(a>=255 && a<260 || a>=285 && a<290)b=0;
		for(int i=0 ; i<200 ; i++)fillRect(img.scr2,0,i+20,320,1,i/2,i/2,192,255);
		drawImage(img.scr2,img.back,a/5,a/5,480,400,320,200,255);
		drawImage(img.scr2,img.back,a/5,-a/5,480,200,320,200,255);
		drawImage(img.scr2,img.back,-a/5,0,480,0,320,200,255);
		drawImage(img.scr2,img.back,140,40,500,700,80,160,b);
		drawImage(img.scr2,img.back,60+a/20,80-a/20,600,600+((a/5)%2)*150,100,150,255);
		drawImage(img.scr2,img.back,134,45+a/20,320,700,52,175,255);
		drawImage(img.scr2,img.back,-95+a/6,125-a/12,380,850,120,120,255);
		drawImage(img.scr2,img.back,290-a/6,125-a/12,380,730,120,120,255);
		if(a<300)drawImage_x2(scr,img.scr2,-(300-a)*180/300,20-(300-a)*100/300,1+(300-a)/300.0,0,0,320,200);
		else drawImage(scr,img.scr2,0,20,0,0,320,200,255);
		if(a>=550)fillRect(scr,0,20,320,200,0,0,0,255);
		else if(a>=500)fillRect(scr,0,20,320,200,0,0,0,(a-500)*5);
		if(a<255)fillRect(scr,0,20,320,200,255,255,255,255-a);
		if(a>=250)drawImage(scr,img.back,110,100,500,860,100,40,255);
	}else{
		a=cn-3750;
		if(loadtime<3750){
			freeImage(img.back);
			getImage(img.back,"file/img/prologue.png",0,0,255);
			loadtime=cn;
		}
		drawImage(scr,img.back,0,30,0,540,320,180,255);
		drawImage(scr,img.back,20,60+abs(10-count%20),720,300,150,150,255);
		drawImage(scr,img.back,120,100,640,460,200,110,255);
		fillRect(scr,0,20,320,200,0,0,255,128);
		fillRect(scr,0,20,320,10,0,0,0,255);
		fillRect(scr,0,210,320,10,0,0,0,255);
		if(a<450){
			drawImage(scr,img.back,110,30,640,600,110,40,255);
			if(a<220)drawImage(scr,img.back,50,120,640,640,220,25,255);
			else{
				drawImage(scr,img.back,50,90,640,665,220,25,255);
				drawImage(scr,img.back,50,140,640,690,220,25,255);
			}
		}
	}
	if(cn>=2600 && cn<3000)drawImage(scr,img.back,150,160,500,900,170,50,255);

	fillRect(scr,0,0,320,20,0,0,0,255);
	fillRect(scr,0,220,320,20,0,0,0,255);
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
		drawImage_x2(scr,img.back,130+a+b*4,120-(int)(b*0.8),(100.0+b*2)/100.0,0,1560,100,100);
		if(a<40)drawImage_x2(scr,img.back,240-(int)(a*0.5),200-a-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1600,40,40);
		else if(a<135)drawImage_x2(scr,img.back,240-(int)(a*0.5),160-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1600,40,40);
		else drawImage_x2(scr,img.back,240-(int)(a*0.5),160-(int)(b*0.5)-(a-135),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1600,40,40);
		if(a<20)drawImage_x2(scr,img.back,280-(int)(a*0.5),200-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1560,40,40);
		else if(a<60)drawImage_x2(scr,img.back,280-(int)(a*0.5),200-(a-20)-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1560,40,40);
		else if(a<135)drawImage_x2(scr,img.back,280-(int)(a*0.5),160-(int)(b*0.5),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1560,40,40);
		else drawImage_x2(scr,img.back,280-(int)(a*0.5),160-(int)(b*0.5)-(a-135),(20.0+b)/40.0,100+abs(2-(a/10)%4)*40,1560,40,40);
		drawImage_x2(scr,img.back,120+a/3+(int)(b*3.5),140-(int)(b*1.5),(20.0+b)/80.0,abs(2-(a/10)%4)*80,720,80,160);
		drawImage_x2(scr,img.back,-100+a,140-b,(40.0+(b*1.5))/100.0,720,720,100,100);
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
		drawImage_x2(scr,img.back,100-b/2,b/2,(250.0-b)/100.0,720+c*100,720,100,100);
		drawImage_x2(scr,img.back,320-(int)(a*2.5)-(int)(b*0.5),60+(int)(b*0.5),(120.0-(b*0.5))/120.0,360,720,120,160);
		fillRect(scr,0,250-(int)(b*0.5),320,40,0,0,0,255);
	}
	else if(cn<3100){
		a=cn-3050;
		int b=a;
		drawImage_x2(scr,img.back,50+b/2,50-(int)(b*0.8),(150.0+(b*1.6))/100.0,720,720,100,100);
		drawImage_x2(scr,img.back,145+b*3,110-b*2,(70.0+b*3)/120.0,360,720,120,160);
		fillRect(scr,0,170+b*2,320,50,0,0,0,255);
	}
	else if(cn<3250){
		a=cn-3100;
		double b;
		int c,d;
		if(a<50){b=1-a/166.6;c=50-a;d=a;}
		else if(a<100){b=0.70+(a-50)/166.6;c=a-50;d=50-(a-50);}
		else{b=1+(a-100)/100.0;c=50+(a-100)*2;d=(a-100)*7;}
		drawImage_x2(scr,img.back,c,40-d+(int)(50*b),b,0,880,100,100);
		drawImage_x2(scr,img.back,c+(int)(50*b),40-d+(int)(95*b),b,0,980,100,150);
		drawImage_x2(scr,img.back,c+(int)(110*b),40-d+(int)(75*b),b,500,880,170,180);
		drawImage_x2(scr,img.back,c+(int)(80*b),40-d,b,1010,880,90,90);
	}
	else if(cn<3350){
		a=cn-3250;
		int b,c=0;
		if(a<85)b=a/3;
		else c=(a-85)*4;
		fillRect(scr,0,320-a*4,320,300,0,0,0,255);
		if(a<50)drawImage_x2(scr,img.back,250-a,-40+(int)(a*2.5),(60.0-b)/60.0,((a/5)%6)*60,1680,60,60);
		else if(a<85)drawImage_x2(scr,img.back,250-a,-40+(int)(a*2.5),(60.0-b)/60.0,((a/5)%6)*60,1740,60,60);
		else drawImage_x2(scr,img.back,165+(int)((a-85)*2.4),172-(int)((a-85)*0.7),(32.0-c/8)/60.0,0,1740,60,60);
		drawImage_x2(scr,img.back,-570+a*6,-60+a,(250.0-c)/250.0,0,1130,250,180);
	}
	else if(cn<3400){
		a=cn-3350;
		drawImage_x2(scr,img.back,30,40,(190.0+a*1.2)/250.0,0,1130,250,180);
	}
	else if(cn<3550){
		a=cn-3400;
		double b=1+a/75.0;
		drawImage_x2(scr,img.back,(int)(95*b)-a*4,20-a,b,0,1310,100,100);
		drawImage_x2(scr,img.back,(int)(105*b)-a*4,20+(int)(65*b)-a,b,570,1440,180,120);
		drawImage_x2(scr,img.back,-a*4,20+(int)(30*b)-a,b,300,1310,130,120);
		drawImage_x2(scr,img.back,(int)(70*b)-a*4,20+(int)(110*b)-a,b,0,1430,190,130);
	}
	else if(cn<3750){
		a=cn-3550;
		drawImage_x2(scr,img.back,80,110-(int)(a*0.25),(100.0+a/5)/120.0,600,1680,120,60);
		drawImage_x2(scr,img.back,80,60-(int)(a*0.25),(100.0+a/5)/120.0,360,1680,120,60);
		drawImage_x2(scr,img.back,80,110+a/10,(100.0+a/5)/120.0,360,1740,120,60);
	}
	else if(cn<3900){
		a=cn-3750;
		drawImage_x2(scr,img.back,80,70,140/120.0,600,1740,120,60);
		drawImage_x2(scr,img.back,80,10,140/120.0,480,1680,120,60);
		drawImage_x2(scr,img.back,80,130,140/120.0,480,1740,120,60);
	}
	else if(cn<3950){
		a=cn-3900;
		drawImage_x2(scr,img.back,80+(int)(a*1.4),70+(int)(a*0.7),(140.0-a*2.8)/120.0,600,1740,120,60);
		drawImage_x2(scr,img.back,80+(int)(a*1.4),10+(int)(a*2.0),(140.0-a*2.8)/120.0,480,1680,120,60);
		drawImage_x2(scr,img.back,80+(int)(a*1.4),130-(int)(a*0.5),(140.0-a*2.8)/120.0,480,1740,120,60);
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
			map.buffered=FALSE;
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
		drawImage_x2(scr,img.scr2,0,0,(320.0+(count-300)*4)/320.0,count-300,(int)((count-300)/1.2),320,240);
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
			map.buffered=FALSE;
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
			drawImage(scr,img.back,0,0,0,0,320,180,255);
		}else{
			drawImage(scr,img.back,0,0,0,180,320,180,255);
			drawImage(scr,img.back,80,60+abs(20-(count/2)%40),640,0,80,120,255);
			drawImage(scr,img.back,160,60+abs(20-((count/2)+20)%40),720,0,80,120,255);
		}
	}
	if(gd.scene_count==1){
		drawImage(scr,img.back,0,0,0,360,320,180,255);
		if(start>50){
			drawImage(scr,img.back,100,50,640,300,80,80,255);
			drawImage(scr,img.back,110,110,640,300,80,80,255);
		}
		if(start>75)drawImage(scr,img.back,100,0,640,120,100,180,255);
		else if(start>50)drawImage(scr,img.back,100,0,740,120,100,180,255);
		else drawImage(scr,img.back,20,0,880,120,80,180,255);
		if(start<=50 && start>25){
			drawImage(scr,img.back,100+abs(5-(count/3)%10),50,640,300,80,80,255);
			drawImage(scr,img.back,110+abs(5-((count+5)/3)%10),110,640,300,80,80,255);
		}
		if(start<=25){
			drawImage(scr,img.back,175-((count+5)%20)*8,-5+((count+5)%20)*8,640,300,80,80,255);
		}
		drawImage(scr,img.back,80,100,800,0,80,120,255);
		if(start<=25){
			drawImage(scr,img.back,180-(count%20)*8,10+(count%20)*8,640,300,80,80,255);
			drawImage(scr,img.back,190-((count+10)%20)*8,20+((count+10)%20)*8,640,300,80,80,255);
			drawImage(scr,img.back,160,0,160,360,160,30,255);
			drawImage(scr,img.back,200,0,200,360,120,180,255);
		}
	}
	if(gd.scene_count==2){
		drawImage(scr,img.back,0,0,0,540,320,180,255);
		drawImage(scr,img.back,20,30+abs(10-count%20)*2,720,300,150,150,255);
		drawImage(scr,img.back,120,70,640,460,200,110,255);
	}
	if(gd.scene_count==3 || gd.scene_count==5){
		drawImage(scr,img.back,0,0,320,0,320,180,255);
		drawImage(scr,img.back,180+abs(20-count%40),20+abs(20-(count/2)%40),870,300,90,160,255);
	}
	if(gd.scene_count==4){
		drawImage(scr,img.back,0,0,0,360,320,180,255);
		drawImage(scr,img.back,20,0,880,120,80,180,255);
		drawImage(scr,img.back,40,100+abs(10-count%20)*2,880,0,80,120,255);
	}
	if(gd.scene_count==6){
		drawImage(scr,img.back,0,0,320,180,320,180,255);
		drawImage(scr,img.back,100,60+abs(6-(count/2)%12),820+abs(2-(count/10)%4)*20,570,20,30,255);
		drawImage(scr,img.back,140,60+abs(6-((count/2)+6)%12),880,570,20,30,255);
		if(count%150<50)drawImage(scr,img.back,90+(count%150)*2,20+(count%150-25)*(count%150-25)/10,900,570,30,30,255);
		drawImage(scr,img.back,75,80,640,570,180,30,255);
	}
	if(gd.scene_count==7){
		drawImage(scr,img.back,0,0,320,360,320,180,255);
		drawImage(scr,img.back,110+abs(20-count%40),100,640,380,80,80,255);
	}
	if(gd.scene_count==8){
		drawImage(scr,img.back,0,0,320,540,320,180,255);
		fillRect(scr,count%50,0,3,15,0,0,0,255);
		if(start>50)drawImage(scr,img.back,140-(start-50)*2,20-(start-50)*4,640,300,80,80,255);
		else if(start>0)drawImage(scr,img.back,140,20,640,300,80,80,255);
	}
	fillRect(scr,0,180,320,60,0,0,0,255);
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
			drawImage_x2(img.scr2,img.photo[8],0,0,0.5,0,0,640,480);
		}
	}
	else if(gd.scene_count==12){
		if(count<200)y2=count;
		else y2=200;
	}
	else if(gd.scene_count==13){
		if(start==0){
			if(count==2){
				drawImage_x2(img.scr2,img.photo[13],0,0,0.5,0,0,640,480);
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
	else drawImage_x2(scr,img.photo[gd.scene_count],0,0,w,x2,y2,(int)(320/w),(int)(240/w));

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
