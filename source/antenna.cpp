#include "antenna.h"

Antenna *ant;

void Antenna::setTmpFish(){
	tmp_fish.which_work=sta[station].ontv;
	tmp_fish.x=(int)gd.x;
	tmp_fish.y=(int)gd.y;
	tmp_fish.week=gd.week;
	tmp_fish.hour=gd.hour;
	tmp_fish.minute=gd.minute;
	tmp_fish.sta=station;
	tmp_fish.bs=false;
	tmp_fish.area=A;
	tmp_fish.tower=T;
	tmp_fish.ch=ch;
	if(rcv>100)tmp_fish.rcv=100;
	else tmp_fish.rcv=rcv;
	if(rcv-mg_rcv>100)tmp_fish.mg_rcv=0;
	else if(rcv-mg_rcv<0)tmp_fish.mg_rcv=100;
	else if(rcv>100)tmp_fish.mg_rcv=100-(rcv-mg_rcv);
	else tmp_fish.mg_rcv=mg_rcv;
	tmp_fish.score=getScore(sta[station].ontv,area[A].tower[T].power[C],(int)gd.x,(int)gd.y);
}

void Antenna::receive(){
	station=EOF;rcv=0;A=0;T=0;C=0;m_wave2=70;
	mg_rcv=0;
	if(ADJ_DIR==AUTO && ant_mode==TUNE){
		Area *ar=area;
		for(int i=0 ; i<areas ; i++){
			Tower *tw=ar->tower;
			for(int j=0 ; j<(ar->tower_num) ; j++){
				for(int k=0 ; k<10 ; k++){
					if(tw->ch[k]!=ch)continue;
					if( rcv<tw->rcv[k] ){
						mg_rcv=0;
						rcv=tw->rcv[k];
						A=i;T=j;C=k;
					}
				}
				tw++;
			}
			ar++;
		}
		gd.ant_dir=(int)area[A].tower[T].dir;
		mg_rcv=receive_mg(A,T,ch,gd.ant_dir);
	}else{
		Uint32 dir2=0;
		Area *ar=area;
		for(int i=0 ; i<areas ; i++){
			Tower *tw=ar->tower;
			for(int j=0 ; j<(ar->tower_num) ; j++){
				for(int k=0 ; k<10 ; k++){
					if(tw->ch[k]!=ch)continue;
					dir2=abs((int)(tw->dir-gd.ant_dir));
					if(dir2>180)dir2=360-dir2;
					if( rcv<tw->rcv[k]/(int)(dir2/5+1) ){
						mg_rcv=0;
						rcv=(int)( tw->rcv[k]/(dir2/5+1) );
						A=i;T=j;C=k;
					}
				}
				tw++;
			}
			ar++;
		}
		mg_rcv=receive_mg(A,T,ch,gd.ant_dir);
	}
	station=area[A].station[C];
}

void Antenna::catching(){
	Uint32 dir2=abs((int)(area[A].tower[T].dir-gd.ant_dir));
	if(dir2>180)dir2=360-dir2;
	if((ant_mode==TURN || (ROD_TYPE!=CONVENIENTROD && ROD_TYPE!=SUPERHANDYROD)) && rcv-mg_rcv>=RECEIPTION_LEVEL && dir2<10)Mix_PlayChannel(1,sf.noize,0);

	if(gd.game_mode==STORYMODE){
		if(rcv-mg_rcv>=RECEIPTION_LEVEL && sta[station].ontv!=EOF){
			setTmpFish();
			if(fishbox.getSC(tmp_fish.which_work)<tmp_fish.score || (fishbox.getSC(tmp_fish.which_work)==tmp_fish.score && fishbox.getRCV(tmp_fish.which_work)<tmp_fish.rcv-tmp_fish.mg_rcv)){
				if(fishbox.getSC(tmp_fish.which_work)==0){
					phase=FISHUP;
					start=150;
					gd.get_score+=tmp_fish.score;
					for(int i=0 ; i<entries ; i++)if(fishbox.today[i]==EOF){
						fishbox.today[i]=tmp_fish.which_work;
						break;
					}
				}else{
					phase=GRADEUP;
					start=67;
					fishbox.text_count=(int)strlen(entry[tmp_fish.which_work].title.str[0]);
					gd.gradeup = tmp_fish.score-fishbox.getSC(tmp_fish.which_work);
					gd.get_score+=gd.gradeup;
					Mix_PlayChannel(1, sf.decide, 0);
				}
				setTmpFish();
				fishbox.setFish(tmp_fish);
			}
			fishbox.panelColor(tmp_fish.which_work);
		}
	}

	if(MAP3D)make3dview(gd.x,gd.y,gd.ant_dir);
}
void Antenna::turnL(){
	gd.ant_dir+=3;
	if(gd.ant_dir>=360)gd.ant_dir%=360;
	receive();
	catching();
}
void Antenna::turnR(){
	gd.ant_dir-=3;
	if(gd.ant_dir<=-1)gd.ant_dir=(gd.ant_dir+360)%(360);
	receive();
	catching();
}
void Antenna::drawAntennaMode(SDL_Surface* scr){
	if(ant_mode==TURN){
		if(rcv-mg_rcv>=RECEIPTION_LEVEL)drawImage(scr,img.fishup,-change_mode_count*6,380,100,100,100,100,255);
		else drawImage(scr,img.fishup,0,380,0,100,100,100,255);
	}
	else if(ant_mode==TUNE){
		if(rcv-mg_rcv>=RECEIPTION_LEVEL)drawImage(scr,img.fishup,-change_mode_count*6,380,100,0,100,100,255);
		else drawImage(scr,img.fishup,0,380,0,0,100,100,255);
	}
	else if(ant_mode==TROLL){
		drawImage(scr,img.fishup,-change_mode_count*6,380,((count/100)%2)*100,200,100,100,255);
	}
}
void Antenna::drawBuoy(SDL_Surface* scr){
	Uint32 dir2=abs((int)(area[A].tower[T].dir-gd.ant_dir));
	if(dir2>180)dir2=360-dir2;
	if(MAP3D){
		if(dir2<10 && rcv>0){ //電波塔の方を向いているとき
			if(area[A].tower[T].v)drawImage(scr,img.chr, area[A].tower[T].x_3d-16, area[A].tower[T].y_3d-60, 280,60,40,60,255);
			else drawImage(scr,img.chr, area[A].tower[T].x_3d-16, area[A].tower[T].y_3d-60, 240,60,40,60,255);
		}
	}else{
		if(dir2<10 && rcv>0){ //電波塔の方を向いているとき
			if(area[A].tower[T].v)drawImage(scr,img.chr, area[A].tower[T].x*MAGNIFY-gd.scrX-16, area[A].tower[T].y*MAGNIFY-gd.scrY-60, 280,60,40,60,255);
			else drawImage(scr,img.chr, area[A].tower[T].x*MAGNIFY-gd.scrX-16, area[A].tower[T].y*MAGNIFY-gd.scrY-60, 240,60,40,60,255);
			if(ant_mode==TURN && start>0 && start%40<20){
				drawImage(scr,img.chr, area[A].tower[T].x*MAGNIFY-gd.scrX-44, area[A].tower[T].y*MAGNIFY-gd.scrY-88, 120,220,90,90,255);
			}
		}else{
			if(area[A].tower[T].v)drawImage(scr,img.chr, (int)(gd.x*MAGNIFY)-gd.scrX-16+(int)(100*cos(gd.ant_dir*PI/180)), (int)(gd.y*MAGNIFY)-gd.scrY-60-(int)(100*sin(gd.ant_dir*PI/180)), 280,60,40,60,255);
			else drawImage(scr,img.chr, (int)(gd.x*MAGNIFY)-gd.scrX-16+(int)(100*cos(gd.ant_dir*PI/180)), (int)(gd.y*MAGNIFY)-gd.scrY-60-(int)(100*sin(gd.ant_dir*PI/180)), 240,60,40,60,255);
			if(ant_mode==TURN && start>0 && start%40<20){
				drawImage(scr,img.chr, (int)(gd.x*MAGNIFY)-gd.scrX-44+(int)(100*cos(gd.ant_dir*PI/180)), (int)(gd.y*MAGNIFY)-gd.scrY-88-(int)(100*sin(gd.ant_dir*PI/180)), 120,220,90,90,255);
			}
		}
	}
}
void Antenna::drawWaterBall(SDL_Surface* scr){
	if(start>120)drawImage(scr,img.chr,area[A].tower[T].x*MAGNIFY-gd.scrX-20,area[A].tower[T].y*MAGNIFY-gd.scrY-140+(start-120)*4,420+(((count/5))%2)*40,170,40,40,255);
	else if(start>90)drawImage(scr,img.chr,area[A].tower[T].x*MAGNIFY-gd.scrX-20,area[A].tower[T].y*MAGNIFY-gd.scrY-140,420+(((count/5))%2)*40,170,40,40,255);
	else{
		int X=(int)(gd.x*MAGNIFY+(area[A].tower[T].x-gd.x)*MAGNIFY*(start-70)/20);
		int Y=(int)(gd.y*MAGNIFY+(area[A].tower[T].y-gd.y)*MAGNIFY*(start-70)/20);
		drawImage(scr,img.chr,X-gd.scrX-20,Y-gd.scrY-140+(90-start)*4,420+(((count/5))%2)*40,170,40,40,255);
	}
}
void Antenna::drawRadioWaveMeter(SDL_Surface* scr){
	if(rcv-mg_rcv>100)drawImage(scr,img.chr,100,420,0,580,200,20,255);
	else drawImage(scr,img.chr,100,420,0,580,(rcv-mg_rcv)*2,20,255);
	if(rcv-mg_rcv<100){
		if(rcv>100)drawImage(scr,img.chr,100+(rcv-mg_rcv)*2,420,0,560,200-(rcv-mg_rcv)*2,20,255);
		else drawImage(scr,img.chr,100+(rcv-mg_rcv)*2,420,0,560,mg_rcv*2,20,255);
	}
}
void Antenna::drawDirectionLine(SDL_Surface* scr){
	int a,b;
	int length=400;
	if(ant_mode==TUNE){
		length=area[A].tower[T].r_dis[area[A].tower[T].r_num-1]*MAGNIFY+MAGNIFY/2;
		if(length>400)length=400;
	}
	for(int i=0 ; i<length ; i++){
		a=(int)(i*cos(gd.ant_dir*PI/180));
		b=(int)(i*sin(gd.ant_dir*PI/180));
		fillRect(scr,320+a,240-b,1,1,255,255,0,255);
	}
}
void Antenna::drawMountainHeight(SDL_Surface* scr){
	if(MAP3D)return;
	if(m_wave2>0){
		for(int i=1 ; i<area[A].tower[T].r_num ; i++){
			int X=(int)(gd.x*MAGNIFY)-gd.scrX;
			int Y=(int)(gd.y*MAGNIFY)-gd.scrY;
			X+=(int)(area[A].tower[T].r_dis[i]*cos(area[A].tower[T].dir*PI/180))*MAGNIFY;
			Y-=(int)(area[A].tower[T].r_dis[i]*sin(area[A].tower[T].dir*PI/180))*MAGNIFY;
			drawM(scr,m_wave2,area[A].tower[T].r_h[i],X,Y);
		}
		m_wave2--;
	}
}
void Antenna::drawRader(SDL_Surface* scr){
	if(!MAP3D)drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-30,(int)(gd.y*MAGNIFY)-gd.scrY-30,180,60,60,60,255);
	drawBuoy(scr);
	if(rcv-mg_rcv>=RECEIPTION_LEVEL && sta[station].ontv!=EOF){
		if(phase==FISHUP && start>67){
			drawWaterBall(scr);
		}else{
			setTmpFish();
			fishbox.drawTable(scr,tmp_fish);
		}
	}else{
		if(station!=EOF)drawTerop(scr,sta[station].talk,rcv,mg_rcv);
		drawAntennaMode(scr);
		drawRadioWaveMeter(scr);
	}
	if(rcv>0){
		drawMountainHeight(scr);
		sprintf_s(str,"%s",toChar(area[A].tower[T].name));
		drawImage(scr,img.menuback,120,0,0,0,(int)strlen(str)*18,40,128);
		drawText2(scr,120,0,str,(int)strlen(str));
	}
	if(MAGNIFY>=8)drawDirectionLine(scr);
}
void Antenna::drawChannel(SDL_Surface* scr){
	drawImage(scr,img.chr,560,40,(ch/10)*20,480,20,40,255);
	drawImage(scr,img.chr,580,40,(ch%10)*20,480,20,40,255);
}
void Antenna::drawExplain2(SDL_Surface* scr){
	if(ant_mode==TURN){
		if(count%800<200){
			drawKeyboard(scr,key.cC,100,380);
			if(gd.bought[5])drawText(scr,120,380,text[ANTENNATEXT+13]);
			else drawText(scr,120,380,text[ANTENNATEXT]);
		}
		else if(count%800<400){
			drawKeyboard(scr,key.leftC,100,380);
			drawKeyboard(scr,key.upC,120,380);
			drawText(scr,140,380,text[ANTENNATEXT+1]);
		}
		else if(count%800<600){
			drawKeyboard(scr,key.rightC,100,380);
			drawKeyboard(scr,key.downC,120,380);
			drawText(scr,140,380,text[ANTENNATEXT+2]);
		}else{
			drawKeyboard(scr,key.xC,100,380);
			drawText(scr,120,380,text[MENUTEXT+4]);
		}
	}
	else if(ant_mode==TROLL){
		if(count%600<200){
			drawKeyboard(scr,key.upC,120,460);
			drawKeyboard(scr,key.downC,140,460);
			drawKeyboard(scr,key.leftC,160,460);
			drawKeyboard(scr,key.rightC,180,460);
			drawText(scr,200,460,text[GAMETEXT+12]);
		}
		else if(count%600<400){
			drawKeyboard(scr,key.cC,120,460);
			drawText(scr,140,460,text[ANTENNATEXT]);
		}else{
			drawKeyboard(scr,key.xC,120,460);
			drawText(scr,140,460,text[MENUTEXT+4]);
		}
	}
}

SimpleRod::SimpleRod(){
	ch=1;station=EOF;rcv=0;v=HIDE;ant_mode=TUNE;
	rc=new int[CHANNELS];
}
SimpleRod::~SimpleRod(){
	delete [] rc;
}

void SimpleRod::hoist(){
	receive();
	catching();
	change_mode_count=20;
	v=VISIBLE;
}
void SimpleRod::lower(){
	v=HIDE;
}
void SimpleRod::pushD(){
	pushR();
}
void SimpleRod::pushU(){
	pushL();
}
void SimpleRod::pushL(){
	if(ant_mode==TURN){
		turnL();
	}else{
		if(!key_wait(key.left) || !key_wait(key.up)){
			if(ch>1){
				ch--;
				receive();
				catching();
			}
		}
	}
}
void SimpleRod::pushR(){
	if(ant_mode==TURN){
		turnR();
	}else{
		if(!key_wait(key.right) || !key_wait(key.down)){
			if(ch<CHANNELS){
				ch++;
				receive();
				catching();
			}
		}
	}
}
void SimpleRod::pushButton(){
	receive();
	catching();
}
void SimpleRod::drawExplain(SDL_Surface* scr){
	if(v==HIDE)return;
	if(ant_mode!=TUNE){drawExplain2(scr);return;}
	if(count%800<200){
		drawKeyboard(scr,key.cC,100,380);
		drawText(scr,120,380,text[ANTENNATEXT+3]);
	}
	else if(count%800<400){
		drawKeyboard(scr,key.leftC,100,380);
		drawKeyboard(scr,key.upC,120,380);
		drawText(scr,140,380,text[ANTENNATEXT+4]);
	}
	else if(count%800<600){
		drawKeyboard(scr,key.rightC,100,380);
		drawKeyboard(scr,key.downC,120,380);
		drawText(scr,140,380,text[ANTENNATEXT+5]);
	}else{
		drawKeyboard(scr,key.xC,100,380);
		drawText(scr,120,380,text[MENUTEXT+4]);
	}
}
void SimpleRod::drawAntenna(SDL_Surface* scr){
	if(v==HIDE)return;
	drawRader(scr);
	drawChannel(scr);
	if(ant_mode==TURN)return;
	drawImage(scr,img.rod,550,30,0,0,60,60,255);
	if(count%40<20){
		drawImage(scr,img.rod,566,20,60,0,26,18,255);
		drawImage(scr,img.rod,566,80,60,18,26,18,255);
	}
}
void SimpleRod::trolling(){
	for(int i=0 ; i<62 ; i++)rc[i]=0;
	Area *ar=area;
	for(int i=0 ; i<areas ; i++){
		Tower *tw=ar->tower;
		for(int j=0 ; j<(ar->tower_num) ; j++){
			for(int k=0 ; k<10 ; k++){
				int this_ch=tw->ch[k];
				if(this_ch>=1 && this_ch<=CHANNELS){
					if(rc[this_ch-1]<tw->rcv[k])rc[this_ch-1]=tw->rcv[k];
				}
			}
			tw++;
		}
		ar++;
	}
	for(int i=0 ; i<62 ; i++){
		if(rc[i]>100)rc[i]=100;
	}
}
void SimpleRod::drawTrolling(SDL_Surface* scr){
	if(v==HIDE)return;
	drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-30,(int)(gd.y*MAGNIFY)-gd.scrY-30,180,60,60,60,255);
	drawAntennaMode(scr);
	for(int i=0 ; i<62 ; i++){
		sprintf_s(str,"%2d",i+1);
		drawText2(scr,(i/13)*120,(i%13)*34,str);
		drawImage(scr,img.chr,(i/13)*120+40,(i%13)*34,0,580,rc[i]*80/100,20,255);
	}
}

void ButtonRod::set_button(){
	Uint8 bt[62];
	for(int i=0 ; i<12 ; i++)button2[i]=0;
	for(int i=0 ; i<CHANNELS ; i++){
		rc[i]=0;st[i]=EOF;bt[i]=0;
	}
	Area *ar=area;
	for(int i=0 ; i<areas ; i++){
		Tower *tw=ar->tower;
		for(int j=0 ; j<(ar->tower_num) ; j++){
			for(int k=0 ; k<10 ; k++){
				if(tw->ch[k]>=1 && tw->ch[k]<=CHANNELS && rc[ tw->ch[k]-1 ]<tw->rcv[k]){
					rc[ tw->ch[k]-1 ]=tw->rcv[k];
					st[ tw->ch[k]-1 ]=ar->station[k];
					bt[ tw->ch[k]-1 ]=ar->button[k];
				}
			}
			tw++;
		}
		ar++;
	}

	for(int i=0 ; i<CHANNELS ; i++){
		if(rc[i]<100)bt[i]=0;
	}
	for(int i=0 ; i<CHANNELS ; i++){
		if(bt[i]==0)continue;
		for(int j=0 ; j<CHANNELS ; j++){
			if(i==j || bt[j]==0)continue;
			if(st[i]==st[j] && rc[i]>=rc[j])bt[j]=0;
		}
	}
	int n=0;
	for(int i=0 ; i<CHANNELS ; i++)if(bt[i]!=0){
		if(i<12)button2[i]=i+1;
		else{
			if(button2[ bt[i]-1 ]==0)button2[ bt[i]-1 ]=i+1;
			else{
				for(int j=n ; j<12 ; j++){
					if(button2[j]==0)break;
					else n++;
				}
				if(n<12)button2[n]=i+1;
			}
		}
	}
}

void ButtonRod::auto_set(){
	if(set==5){
		set_button();
		for(int i=0 ; i<12 ; i++)button[i]=0;
	}

	if(count%30==0){
		int i=set-5;
		if(i==CHANNELS){
			b_num=11;
			ch_up();
			receive();
			set=3;
			return;
		}
		ch=i+1;
		receive();

		for(int j=0 ; j<12 ; j++){
			if(button2[j]==i+1)button[j]=i+1;
		}
		set++;
	}
}

void ButtonRod::code_set(){
	for(int i=0 ; i<12 ; i++)button[i]=areacode[code].code[i];
	b_num=11;
	ch_up();
	receive();
	set=1;
}

void ButtonRod::ch_up(){
	for(int i=0 ; i<12 ; i++){
		b_num++;
		if(b_num==12)b_num=0;
		if(button[b_num]!=0){
			ch=button[b_num];
			break;
		}
	}
}
void ButtonRod::ch_down(){
	for(int i=0 ; i<12 ; i++){
		b_num--;
		if(b_num==0)b_num=11;
		if(button[b_num]!=0){
			ch=button[b_num];
			break;
		}
	}
}

ButtonRod::ButtonRod(){
	ch=1;station=EOF;rcv=0;push=0;b_num=0;set=0;code=0;v=HIDE;ant_mode=TUNE;
	rc=new int[CHANNELS];
	st=new int[CHANNELS];
	for(int i=0 ; i<12 ; i++)button[i]=0;
	readSQL("file/data/sql/areacode.sql");
}

ButtonRod::~ButtonRod(){
	delete [] areacode;
	delete [] rc;
	delete [] st;
	areacode=NULL;
}

void ButtonRod::hoist(){
	set=0;
	receive();
	catching();
	change_mode_count=20;
	v=VISIBLE;
}
void ButtonRod::lower(){
	v=HIDE;
}
void ButtonRod::pushD(){
	if(ant_mode==TURN)turnR();
	else if(!key_wait(key.down)){
		if(set==1 || set==2)set++;
		else if(set==0 && push<12){
			push+=3;
		}
	}
}
void ButtonRod::pushU(){
	if(ant_mode==TURN)turnL();
	else if(!key_wait(key.up)){
		if(set==2 || set==3)set--;
		else if(set==0 && push>2){
			push-=3;
		}
	}
}
void ButtonRod::pushL(){
	if(ant_mode==TURN)turnL();
	else if(!key_wait(key.left)){
		if(set==4){
			if(code==0)code=areacode_num-1;
			else code--;
		}
		else if(set==0 && push%3>0)push--;
	}
}
void ButtonRod::pushR(){
	if(ant_mode==TURN)turnR();
	else if(!key_wait(key.right)){
		if(set==4){
			if(code==areacode_num-1)code=0;
			else code++;
		}
		else if(set==0 && push%3<2)push++;
	}
}
void ButtonRod::pushButton(){
	if(ant_mode==TURN || key_wait(key.z))return;
	if(set==0){
		switch(push){
			case 12:set=1;break;
			case 13:ch_down();break;
			case 14:ch_up();break;
			default:{ch=button[push];b_num=push;}break;
		}
	}
	else if(set==1)set=4;
	else if(set==2)set=5;
	else if(set==4)code_set();
	else set=0;
	Mix_PlayChannel(2, sf.bupyo, 0);
	receive();
	catching();
}
void ButtonRod::drawExplain(SDL_Surface* scr){
	if(v==HIDE)return;
	if(ant_mode!=TUNE){drawExplain2(scr);return;}
	if(count%800<200){
		drawKeyboard(scr,key.zC,100,380);
		drawText(scr,120,380,text[ANTENNATEXT+9]);
	}
	else if(count%800<400){
		drawKeyboard(scr,key.cC,100,380);
		drawText(scr,120,380,text[ANTENNATEXT+3]);
	}
	else if(count%800<600){
		drawKeyboard(scr,key.leftC,100,380);
		drawKeyboard(scr,key.upC,120,380);
		drawKeyboard(scr,key.rightC,140,380);
		drawKeyboard(scr,key.downC,160,380);
		drawText(scr,180,380,text[ANTENNATEXT+10]);
	}else{
		drawKeyboard(scr,key.xC,100,380);
		drawText(scr,120,380,text[MENUTEXT+4]);
	}
}
void ButtonRod::drawAntenna(SDL_Surface* scr){
	if(v==HIDE)return;
	drawRader(scr);
	drawChannel(scr);
	if(ant_mode==TURN)return;
	drawImage(scr,img.rod,520,120,0,0,120,200,255);
	for(int i=0 ; i<12 ; i++){
		if(button[i]!=0)drawImage(scr,img.rod, 520+(i%3)*40,(i/3)*40+120, 0,200,40,40, 255);
	}
	if(set==0)drawImage(scr,img.chr, 500+(push%3)*40,(push/3)*40+120, 280,240,40,40, 255 );
	else if(set>=1 && set<=3){
		drawImage(scr,img.menuback,60,0,0,0,300,120,192);
		drawImage(scr,img.chr, 60,set*40-40, 280,240,40,40, 255);
		drawText2(scr,100,0,toChar(text[ANTENNATEXT+11]),30);
		drawText2(scr,100,40,toChar(text[ANTENNATEXT+12]),30);
		drawText2(scr,100,80,toChar(text[MENUTEXT+4]),30);
	}
	else if(set==4){
		sprintf_s(str,"[%3d]%s",code,areacode[code].name.str[CHAR_CODE]);
		drawText(scr,180,40,str,(int)strlen(str));
	}
	else if(set>=5)auto_set();

	if(set!=0){
		drawImage(scr,img.menuback,120,120,0,0,340,240,192);
		for(int i=0 ; i<12 ; i++){
			drawImage(scr,img.rod,(i/6)*200+120,(i%6)*40+120,(i%3)*40+2,(i/3)*40+4,36,32,255);
			if(button[i]==0){
				drawText2(scr,(i/6)*200+200,(i%6)*40+120,"-",2,128);
			}else{
				sprintf_s(str,"%2d",button[i]);
				drawText2(scr,(i/6)*200+200,(i%6)*40+120,str,2);
			}
		}
	}
}
void ButtonRod::trolling(){
	set_button();
}
void ButtonRod::drawTrolling(SDL_Surface* scr){
	if(v==HIDE)return;
	drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-30,(int)(gd.y*MAGNIFY)-gd.scrY-30,180,60,60,60,255);
	drawImage(scr,img.menuback,120,120,0,0,340,240,128);
	for(int i=0 ; i<12 ; i++){
		sprintf_s(str,"%2d",i+1);
		drawText2(scr,(i/6)*200+120,(i%6)*40+120,str,2);
		if(button2[i]==0)sprintf_s(str,"-");
		else sprintf_s(str,"%2d",button2[i]);
		drawText2(scr,(i/6)*200+200,(i%6)*40+120,str,2);
	}
	drawAntennaMode(scr);
}

void UVRod::setCH(){
	if(vhf==13)ch=uhf;
	else ch=vhf;
	receive();
	catching();
}
UVRod::UVRod(){
	ch=1;station=EOF;rcv=0;vhf=1;uhf=13;part=0;v=HIDE;ant_mode=TUNE;
	st=new Uint8[stas];
}
UVRod::~UVRod(){
	delete [] st;
}
void UVRod::hoist(){
	receive();
	catching();
	change_mode_count=20;
	v=VISIBLE;
}
void UVRod::lower(){
	v=HIDE;
}
void UVRod::pushR(){
	if(ant_mode==TURN)turnR();
	else if(!key_wait(key.right)){
		if(part==1){
			if(uhf<CHANNELS)uhf++;
			setCH();
		}
		if(part==0){
			vhf++;
			if(vhf==14)vhf=1;
			setCH();
			Mix_PlayChannel(2,sf.knob,0);
		}
	}
}
void UVRod::pushL(){
	if(ant_mode==TURN)turnL();
	else if(!key_wait(key.left)){
		if(part==1 && uhf>13){uhf--;setCH();}
		if(part==0){
			vhf--;
			if(vhf==0)vhf=13;
			setCH();
			Mix_PlayChannel(2,sf.knob,0);
		}
	}
}
void UVRod::pushU(){
	if(ant_mode==TURN)turnL();
	else if(!key_wait(key.up)){
		if(part==1)part=0;
	}
}
void UVRod::pushD(){
	if(ant_mode==TURN)turnR();
	else if(!key_wait(key.down)){
		if(part==0)part=1;
	}
}
void UVRod::pushButton(){
	receive();
	catching();
}
void UVRod::drawExplain(SDL_Surface* scr){
	if(v==HIDE)return;
	if(ant_mode!=TUNE){drawExplain2(scr);return;}
	if(count%800<200){
		drawKeyboard(scr,key.cC,100,380);
		drawText(scr,120,380,text[ANTENNATEXT+3]);
	}
	else if(count%800<400){
		drawKeyboard(scr,key.leftC,100,380);
		drawKeyboard(scr,key.rightC,120,380);
		drawText(scr,140,380,text[ANTENNATEXT+7]);
	}
	else if(count%800<600){
		drawKeyboard(scr,key.upC,100,380);
		drawKeyboard(scr,key.downC,120,380);
		drawText(scr,140,380,text[ANTENNATEXT+8]);
	}else{
		drawKeyboard(scr,key.xC,100,380);
		drawText(scr,120,380,text[MENUTEXT+4]);
	}
}
void UVRod::drawAntenna(SDL_Surface* scr){
	if(v==HIDE)return;
	drawRader(scr);
	drawChannel(scr);
	if(ant_mode==TURN)return;
	drawImage(scr,img.rod, 560,120, 0,0,80,160, 255);
	drawImage(scr,img.rod, 560,120, (vhf-1)%7*80+80,(vhf-1)/7*80,80,80, 255);
	drawImage(scr,img.rod, 596+(int)(20*cos(3.0+(uhf-13)*0.05)),230-(int)(20*sin(3.0+(uhf-13)*0.05)), 560,80,10,10, 255);
	drawImage(scr,img.chr,520,part*80+140,280,240,40,40,255);
}
void UVRod::trolling(){
	for(int i=0 ; i<10 ; i++){net[i]=0;net2[i]=0;}
	for(int i=0 ; i<stas ; i++)st[i]=0;
	Area *ar=area;
	for(int i=0 ; i<areas ; i++){
		Tower *tw=ar->tower;
		for(int j=0 ; j<(ar->tower_num) ; j++){
			for(int k=0 ; k<10 ; k++){
				if(tw->ch[k]>=1 && tw->ch[k]<=CHANNELS){
					if(tw->rcv[k]>=100){
						st[ar->station[k]]=2;
					}
					else if(tw->rcv[k]>=RECEIPTION_LEVEL){
						if(st[ar->station[k]]!=2)st[ar->station[k]]=1;
					}
				}
			}
			tw++;
		}
		ar++;
	}
	for(int i=0 ; i<stas ; i++){
		if(st[i]==2)net[sta[i].mark-5]++;
		if(st[i]==1)net2[sta[i].mark-5]++;
	}
}
void UVRod::drawTrolling(SDL_Surface* scr){
	if(v==HIDE)return;
	drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-30,(int)(gd.y*MAGNIFY)-gd.scrY-30,180,60,60,60,255);
	drawAntennaMode(scr);
	int a;
	for(int i=0 ; i<10 ; i++){
		a=0;
		for(int j=0 ; j<net[i] ; j++){
			drawImage(scr,img.symbol,i*40+100,a*40+200,i*34+170,0,34,34,255);
			a++;
		}
		for(int j=0 ; j<net2[i] ; j++){
			drawImage(scr,img.symbol,i*40+100,a*40+200,i*34+170,0,34,34,64);
			a++;
		}
	}
}

MHzRod::MHzRod(){
	mh=mhz[0].mhz;ch=1;station=EOF;rcv=0;v=HIDE;ant_mode=TUNE;
	rc=new int[CHANNELS];
}
MHzRod::~MHzRod(){
	delete [] rc;
}
void MHzRod::hoist(){
	receive();
	change_mode_count=20;
	v=VISIBLE;
}
void MHzRod::lower(){
	v=HIDE;
}
void MHzRod::pushD(){
	if(ant_mode==TURN)turnR();
	else if(mh<mhz[CHANNELS-1].mhz && (!key_wait(key.down) || !key_wait(key.right))){
		mh++;
		ch=0;
		for(int i=0 ; i<CHANNELS ; i++){
			if(mhz[i].mhz==mh)ch=i+1;
		}
		receive();
		catching();
	}
}
void MHzRod::pushU(){
	if(ant_mode==TURN)turnL();
	else if(mh>1 && (!key_wait(key.up) || !key_wait(key.left))){
		mh--;
		ch=0;
		for(int i=0 ; i<CHANNELS ; i++){
			if(mhz[i].mhz==mh)ch=i+1;
		}
		receive();
		catching();
	}
}
void MHzRod::pushL(){
	pushU();
}
void MHzRod::pushR(){
	pushD();
}
void MHzRod::pushButton(){
	receive();
	catching();
}
void MHzRod::drawExplain(SDL_Surface* scr){
	if(v==HIDE)return;
	if(ant_mode!=TUNE){drawExplain2(scr);return;}
	if(count%800<200){
		drawKeyboard(scr,key.cC,100,380);
		drawText(scr,120,380,text[ANTENNATEXT+3]);
	}
	else if(count%800<400){
		drawKeyboard(scr,key.leftC,100,380);
		drawKeyboard(scr,key.upC,120,380);
		drawText(scr,140,380,text[ANTENNATEXT+4]);
	}
	else if(count%800<600){
		drawKeyboard(scr,key.rightC,100,380);
		drawKeyboard(scr,key.downC,120,380);
		drawText(scr,140,380,text[ANTENNATEXT+5]);
	}else{
		drawKeyboard(scr,key.xC,100,380);
		drawText(scr,120,380,text[MENUTEXT+4]);
	}
}
void MHzRod::drawAntenna(SDL_Surface* scr){
	if(v==HIDE)return;
	drawRader(scr);
	if(ant_mode==TURN)return;
	drawImage(scr,img.rod,20,40,0,0,600,40,255);
	drawImage(scr,img.rod,mh*2/3+20,40,600,0,20,40,255);
	drawImage(scr,img.menuback,440,0,0,0,160,40,128);
	sprintf_s(str,"%5d Mhz",mh);
	drawText2(scr, 440, 0, str, 9);
}
void MHzRod::trolling(){
	for(int i=0 ; i<62 ; i++)rc[i]=0;
	Area *ar=area;
	for(int i=0 ; i<areas ; i++){
		Tower *tw=ar->tower;
		for(int j=0 ; j<(ar->tower_num) ; j++){
			for(int k=0 ; k<10 ; k++){
				int this_ch=tw->ch[k];
				if(this_ch>=1 && this_ch<=CHANNELS){
					if(rc[this_ch-1]<tw->rcv[k])rc[this_ch-1]=tw->rcv[k];
				}
			}
			tw++;
		}
		ar++;
	}
}
void MHzRod::drawTrolling(SDL_Surface* scr){
	if(v==HIDE)return;
	drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-30,(int)(gd.y*MAGNIFY)-gd.scrY-30,180,60,60,60,255);
	drawAntennaMode(scr);
	drawImage(scr,img.rod,20,0,0,0,600,40,255);
	for(int i=0 ; i<62 ; i++){
		fillRect(scr,mhz[i].mhz*2/3+20,40,2,rc[i]/5,255,255,0,255);
	}
}

void ConvenientRod::makeList(){
	for(int i=0 ; i<stas ; i++){
		rc[i]=0;mr[i]=0;st[i]=EOF;ar[i]=0;tw[i]=0;chn[i]=0;
	}
	Area *are=area;
	for(int i=0 ; i<areas ; i++){
		Tower *tow=are->tower;
		for(int j=0 ; j<(are->tower_num) ; j++){
			for(int k=0 ; k<10 ; k++){
				int this_ch=are->station[k];
				if(tow->ch[k]>=1 && tow->ch[k]<=CHANNELS && tow->rcv[k]>=RECEIPTION_LEVEL){
					int mr2=0;
					mr2=receive_mg(i,j,tow->ch[k],(int)tow->dir);
					if(tow->rcv[k]-mr2<rc[this_ch]-mr[this_ch])continue;
					rc[this_ch]=tow->rcv[k];
					st[this_ch]=this_ch;
					ar[this_ch]=i;
					tw[this_ch]=j;
					chn[this_ch]=k;
					mr[this_ch]=mr2;
				}
			}
			tow++;
		}
		are++;
	}

	for(int i=0 ; i<stas-1 ; i++)for(int j=stas-2 ; j>=i ; j--){
		if(rc[j]-mr[j]<rc[j+1]-mr[j+1]){
			int RC=rc[j],ST=st[j],AR=ar[j],TW=tw[j],CH=chn[j],MR=mr[j];
			rc[j]=rc[j+1];st[j]=st[j+1];ar[j]=ar[j+1];tw[j]=tw[j+1];chn[j]=chn[j+1];mr[j]=mr[j+1];
			rc[j+1]=RC;st[j+1]=ST;ar[j+1]=AR;tw[j+1]=TW;chn[j+1]=CH;mr[j+1]=MR;
		}
	}
	st_num=0;
	for(int i=0 ; i<stas ; i++){
		if(rc[i]-mr[i]<RECEIPTION_LEVEL)break;
		st_num++;
	}
}
void ConvenientRod::make_st_list(){
	st_list.setMenu(0,30,20,8,st_num);
	for(int i=0 ; i<st_num ; i++){
		st_list.stack(sta[st[i]].name);
		st_list.inputMark(i,sta[st[i]].mark);
		if(rc[i]-mr[i]<100)st_list.gray[i]=true;
	}
	st_list.setViewMode(VISIBLE);
}

ConvenientRod::ConvenientRod(){
	load_station();
	rd.received=false;
	ch=1;A=0;T=0;ant_mode=TUNE;v=HIDE;st_num=0;
	rc=new int[stas];
	st=new int[stas];
	ar=new int[stas];
	tw=new int[stas];
	chn=new int[stas];
	mr=new int[stas];
}
ConvenientRod::~ConvenientRod(){
	st_list.reset();
	delete [] rc;
	delete [] st;
	delete [] ar;
	delete [] tw;
	delete [] chn;
	delete [] mr;
}
void ConvenientRod::hoist(){
	if(!rd.received){
		makeList();
		make_st_list();
	}
	change_mode_count=20;
	v=VISIBLE;
	st_list.setViewMode(VISIBLE);
	setTable();
}
void ConvenientRod::lower(){
	v=HIDE;
	st_list.setViewMode(HIDE);
}
void ConvenientRod::pushU(){
	if(ant_mode==TURN)turnL();
	else if(!key_wait(key.up))st_list.cursorUp();
}
void ConvenientRod::pushD(){
	if(ant_mode==TURN)turnR();
	else if(!key_wait(key.down))st_list.cursorDown();
}
void ConvenientRod::pushL(){
	if(ant_mode==TURN)turnL();
	else if(!key_wait(key.left)){
		st_list.cursorUp();
		pushButton();
	}
}
void ConvenientRod::pushR(){
	if(ant_mode==TURN)turnR();
	else if(!key_wait(key.right)){
		st_list.cursorDown();
		pushButton();
	}
}
void ConvenientRod::pushButton(){
	Mix_PlayChannel(0,sf.bupyo,0);
	setTable();
}
void ConvenientRod::setTable(){
	m_wave2=70;
	rcv=rc[st_list.selected()];
	station=st[st_list.selected()];
	A=ar[st_list.selected()];
	T=tw[st_list.selected()];
	C=chn[st_list.selected()];
	ch=area[A].tower[T].ch[C];
	gd.ant_dir=(int)area[A].tower[T].dir;
	mg_rcv=mr[st_list.selected()];
	catching();
}
void ConvenientRod::drawExplain(SDL_Surface* scr){
	if(v==HIDE)return;
	drawAntennaMode(scr);
	if(ant_mode!=TUNE){drawExplain2(scr);return;}
	if(count%1000<200){
		drawKeyboard(scr,key.zC,100,380);
		drawText(scr,120,380,text[OPTIONTEXT+1]);
	}
	else if(count%1000<400){
		drawKeyboard(scr,key.cC,100,380);
		drawText(scr,120,380,text[ANTENNATEXT+3]);
	}
	else if(count%1000<600){
		drawKeyboard(scr,key.upC,100,380);
		drawKeyboard(scr,key.downC,120,380);
		drawText(scr,140,380,text[MENUTEXT+15]);
	}
	else if(count%1000<800){
		drawKeyboard(scr,key.leftC,100,380);
		drawKeyboard(scr,key.rightC,120,380);
		drawText(scr,140,380,text[ANTENNATEXT+6]);
	}else{
		drawKeyboard(scr,key.xC,100,380);
		drawText(scr,120,380,text[MENUTEXT+4]);
	}
}
void ConvenientRod::drawAntenna(SDL_Surface* scr){
	st_list.animation();
	if(v==HIDE)return;
	drawRader(scr);
	drawChannel(scr);
	if(ant_mode==TUNE && phase!=FISHUP)st_list.drawMenu(scr);
}
void ConvenientRod::trolling(){
	makeList();
	make_st_list();
}
void ConvenientRod::drawTrolling(SDL_Surface* scr){
	if(v==HIDE)return;
	drawAntennaMode(scr);
	drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-30,(int)(gd.y*MAGNIFY)-gd.scrY-30,180,60,60,60,255);
	for(int i=0 ; i<18 ; i++){
		if(i==st_num)break;
		int a=255;
		if(rc[i]-mr[i]<100)a=128;
		drawText2(scr,(i/9)*320+40,(i%9)*40,sta[st[i]].name,60,a);
		drawImage(scr,img.symbol,(i/9)*320,(i%9)*40,(sta[st[i]].mark%16)*34,(sta[st[i]].mark/16)*34,34,34,a);
	}
}

void SuperHandyRod::makeList2(){
	st_num=0;
	int tower_num=0;
	Area *are=area;
	for(int i=0 ; i<areas ; i++){
		tower_num+=are->tower_num;
		are++;
	}
	for(int i=0 ; i<tower_num*10 ; i++){
		rc[i]=0;mr[i]=0;st[i]=EOF;ar[i]=0;tw[i]=0;chn[i]=0;
	}
	for(int i=0 ; i<64 ; i++){rc2[i]=-1;ar2[i]=0;tw2[i]=0;}
	are=area;
	for(int i=0 ; i<areas ; i++){
		Tower *tow=are->tower;
		for(int j=0 ; j<(are->tower_num) ; j++){
			for(int k=0 ; k<10 ; k++){
				if(tow->ch[k]>=1 && tow->ch[k]<=CHANNELS && tow->rcv[k]>=RECEIPTION_LEVEL){
					int mr2=0;
					mr2=receive_mg(i,j,tow->ch[k],(int)tow->dir);
					if(tow->rcv[k]-mr2<RECEIPTION_LEVEL)continue;
					rc[st_num]=tow->rcv[k];
					st[st_num]=are->station[k];
					ar[st_num]=i;
					tw[st_num]=j;
					chn[st_num]=k;
					mr[st_num]=mr2;
					st_num++;
				}
			}
			tow++;
		}
		are++;
	}
	for(int i=0 ; i<st_num-1 ; i++)for(int j=st_num-2 ; j>=i ; j--){
		if(rc[j]-mr[j]<rc[j+1]-mr[j+1]){
			int RC=rc[j],ST=st[j],AR=ar[j],TW=tw[j],CH=chn[j],MR=mr[j];
			rc[j]=rc[j+1];st[j]=st[j+1];ar[j]=ar[j+1];tw[j]=tw[j+1];chn[j]=chn[j+1];mr[j]=mr[j+1];
			rc[j+1]=RC;st[j+1]=ST;ar[j+1]=AR;tw[j+1]=TW;chn[j+1]=CH;mr[j+1]=MR;
		}
	}
	for(int i=0 ; i<st_num-1 ; i++)for(int j=st_num-2 ; j>=i ; j--){
		if(st[j]>st[j+1]){
			int RC=rc[j],ST=st[j],AR=ar[j],TW=tw[j],CH=chn[j],MR=mr[j];
			rc[j]=rc[j+1];st[j]=st[j+1];ar[j]=ar[j+1];tw[j]=tw[j+1];chn[j]=chn[j+1];mr[j]=mr[j+1];
			rc[j+1]=RC;st[j+1]=ST;ar[j+1]=AR;tw[j+1]=TW;chn[j+1]=CH;mr[j+1]=MR;
		}
	}
	for(int i=0 ; i<st_num ; i++){
		for(int j=0 ; j<64 ; j++){
			if(rc2[j]!=-1 && ar2[j]==ar[i] && tw2[j]==tw[i]){
				if(rc2[j]>rc[i]-mr[i])rc2[j]=rc[i]-mr[i];
				break;
			}
			if(rc2[j]==-1){
				rc2[j]=rc[i]-mr[i];ar2[j]=ar[i];tw2[j]=tw[i];
				break;
			}
		}
	}
}
void SuperHandyRod::make_st_list2(){
	st_list.setMenu(0,30,30,8,st_num);
	String s;
	for(int i=0 ; i<st_num ; i++){
		sprintf_s(s.str[0],"%s",sta[st[i]].abbr.str[0]);
		sprintf_s(s.str[0],"%s:%d:%s",s.str[0],(int)area[ar[i]].tower[tw[i]].rcv[chn[i]],area[ar[i]].tower[tw[i]].name.str[0]);
		sprintf_s(s.str[1],"%s",sta[st[i]].abbr.str[1]);
		sprintf_s(s.str[1],"%s:%d:%s",s.str[1],area[ar[i]].tower[tw[i]].rcv[chn[i]],area[ar[i]].tower[tw[i]].name.str[1]);
		st_list.stack(s);
		st_list.inputMark(i,sta[st[i]].mark);
		if(rc[i]-mr[i]<100)st_list.gray[i]=true;
    }
	st_list.setViewMode(VISIBLE);
}

SuperHandyRod::SuperHandyRod(){
	ch=1;A=0;T=0;ant_mode=TUNE;v=HIDE;st_num=0;
	rd.received=false;
	load_towers();
	int tower_num=0;
	Area *are=area;
	for(int i=0 ; i<areas ; i++){
		tower_num+=are->tower_num;
		are++;
	}
	rc=new int[tower_num*10];
	st=new int[tower_num*10];
	ar=new int[tower_num*10];
	tw=new int[tower_num*10];
	chn=new int[tower_num*10];
	mr=new int[tower_num*10];
}

void SuperHandyRod::hoist(){
	if(!rd.received){
		makeList2();
		make_st_list2();
	}
	change_mode_count=20;
	v=VISIBLE;
	st_list.setViewMode(VISIBLE);
	setTable();
}
void SuperHandyRod::drawAntenna(SDL_Surface* scr){
	st_list.animation();
	if(v==HIDE)return;
	drawRader(scr);
	drawChannel(scr);
	if(ant_mode==TUNE && phase!=FISHUP)st_list.drawMenu(scr);
}
void SuperHandyRod::trolling(){
	makeList2();
	make_st_list2();
}
void SuperHandyRod::drawTrolling(SDL_Surface* scr){
	if(v==HIDE)return;
	drawAntennaMode(scr);
	drawImage(scr,img.chr,(int)(gd.x*MAGNIFY)-gd.scrX-30,(int)(gd.y*MAGNIFY)-gd.scrY-30,180,60,60,60,255);
	for(int i=0 ; i<64 ; i++){
		if(rc2[i]<RECEIPTION_LEVEL)break;
		int a=255;
		if(rc2[i]<100)a=128;
		drawText(scr,(i/16)*160+20,(i%16)*20+20,area[ar2[i]].tower[tw2[i]].name,60,a);
	}
}
