#include "util.h"

Key key;
Index *indexName;
Station *sta;
Prg *prg;
Work *work;
Area *area;
Tower *tower;
Mount *mount;
Town *town;
Menu menu[20];
FishBox fishbox;
Fish tmp_fish;
Image *font[47];
Mix_Music *bgm=NULL;
ImageFile img;
SoundFile sf;
SDL_Event ev;
SDL_Rect scr;
char weekChar[7][2][4],str[1000],str3[60],*fstr;
size_t fsize=0;
String text[1000],talk[1000];
bool run,setSMR,map_loaded=false, *animebook, ABGR;
double test=0;
int stas=0,works=0,prgs=0,allworks=0,collection=0,areas=0,towers=0,mounts=0,towns=0,index_num=0,clear_num=0;
int count=0,bg_count=0,face[1000],start=0;
Uint8 mode=0,phase=0,dataNo=1,fontA=255,kick_count=0,pauseGame;
bool SHOW_TOWER, EXPLAIN, NHK_REMOVE;
Uint8 CHAR_CODE,AIR_IMG,WALKING_TYPE,ROD_TYPE,ADJ_DIR,MAP3D;
Uint8 CHANNELS,FULLSCR,SCRSIZE=1,BGM_VOLUME,SE_VOLUME;
Uint8 CHOSEON=0,CURVE_TOP,MAGNIFY,DASH_TYPE;
double DIS1CH,DIS62CH,RCV_LEVEL,MAP_SCALE,CURVE_RISE;
int BSchannel[12],BSstation[12];
Mhz *mhz;

void Menu::setMenu(int X, int Y, int w, int r, int n){
	reset();
	if(n==0)return;
	x=X;y=Y;width=w;raw=r;lim=n;combo=1;lang=EOF;
	st=new String[lim];
	gray=new bool[lim];
	mark=new Uint16[lim];
	R=new Uint8[lim];
	G=new Uint8[lim];
	B=new Uint8[lim];
	for(int i=0 ; i<lim ; i++){
		gray[i]=false;
		mark[i]=0;
		R[i]=0;G[i]=0;B[i]=0;
		st[i].str[0][0]=0;
		st[i].str[1][0]=0;
	}
}
void Menu::setBG(int a){
	bg=a;
}
void Menu::setViewMode(int a){
	if(v==HIDE && (a==GRAY||a==VISIBLE))open_count=1;
	if((v==GRAY||v==VISIBLE) && a==HIDE)open_count=1;
	v=a;
}
void Menu::setCombo(int a){
	combo=a;
}
void Menu::animation(){
	if(open_count==0)return;
	if(open_count<20)open_count++;
	else open_count=0;
}
void Menu::drawMenuText(SDL_Surface* scr){
	int a=255;
	if(v==GRAY)a=128;
	drawImage(scr,img.chr, x,y+show*40, 280,240,40,40, a);
	for(int i=pageOff ; i<pageOff+raw ; i++){
		if(i==lim)break;
		if(v==GRAY || gray[i])a=128;
		else a=255;
		fontA=a;
		if(mark[i]!=0){
			drawImage(scr,img.symbol,x+40,y+(i-pageOff)*40,(mark[i]%16)*34,(mark[i]/16)*34,34,34,a);
			if(lang==JAPANESE)drawText2_lang(scr, x+80, y+(i-pageOff)*40, st[i], width, JAPANESE);
			else if(lang==EUROPEAN)drawText2_lang(scr, x+80, y+(i-pageOff)*40, st[i], width, EUROPEAN);
			else drawText2(scr, x+80, y+(i-pageOff)*40, st[i], width);
		}else{
			if(lang==JAPANESE)drawText2_lang(scr, x+40, y+(i-pageOff)*40, st[i], width, JAPANESE);
			else if(lang==EUROPEAN)drawText2_lang(scr, x+40, y+(i-pageOff)*40, st[i], width, EUROPEAN);
			else drawText2(scr, x+40, y+(i-pageOff)*40, st[i], width);
		}
	}
	fontA=255;
}
void Menu::drawMenu(SDL_Surface* scr){
	if(open_count!=0){drawAnimation(scr);return;}
	if(v==HIDE)return;
	drawImage(scr,img.menuback,x,y,0,0,(width+2)*20,raw*40,bg);
	drawMenuText(scr);
}
void Menu::drawColorMenu(SDL_Surface* scr){
	if(open_count!=0){drawAnimation(scr);return;}
	if(v==HIDE)return;
	int w=(width+2)*10;
	for(int i=0 ; i<raw ; i++){
		if(pageOff+i>=lim)break;
		for(int j=0 ; j<w ; j++){
			fillRect(scr,x+j,y+i*40,1,40,R[pageOff+i],G[pageOff+i],B[pageOff+i],(w-j)*255/w);
		}
	}
	drawMenuText(scr);
}
void Menu::drawAnimation(SDL_Surface* scr){
	int w=(width+2)*20*open_count/20;
	int h=raw*40*open_count/20;
	if(v==HIDE){
		w=(width+2)*20-w;
		h=raw*40-h;
	}
	drawRect(scr,x,y,w,h,128,128,255,255);
}
void Menu::stack(const char *c){
	if(head>=lim)return;
	for(int i=0 ; i<60 ; i++){
		st[head].str[0][i]=c[i];
		st[head].str[1][i]=c[i];
		if(c[i]==0){
			if(width<i-1)width=i-1;
			break;
		}
	}
	head++;
}
void Menu::input(int n, char *c){
	if(n<0 || n>=lim)return;
	for(int i=0 ; i<60 ; i++){
		st[n].str[0][i]=c[i];
		st[n].str[1][i]=c[i];
		if(c[i]==0){
			if(width<i-1)width=i-1;
			break;
		}
	}
}
void Menu::insert(int n, char *c){
	if(n<0 || n>=lim || head>=lim)return;
	for(int i=head ; i>n ; i--)for(int j=0 ; j<60 ; j++){
		st[i].str[0][j]=st[i-1].str[0][j];
		st[i].str[1][j]=st[i-1].str[1][j];
	}
	for(int i=head ; i>n ; i--)mark[i]=mark[i-1];
	input(n,c);
	head++;
}
void Menu::stack(String s){
	if(head>=lim)return;
	for(int i=0 ; i<60 ; i++)for(int k=0 ; k<2 ; k++){
		st[head].str[k][i]=s.str[k][i];
	}
	head++;
}
void Menu::input(int n, String s){
	if(n<0 || n>=lim)return;
	for(int i=0 ; i<60 ; i++)for(int k=0 ; k<2 ; k++){
		st[n].str[k][i]=s.str[k][i];
	}
}
void Menu::insert(int n, String s){
	if(n<0 || n>=lim || head>=lim)return;
	for(int i=head ; i>n ; i--)for(int j=0 ; j<60 ; j++)for(int k=0 ; k<2 ; k++){
		st[i].str[k][j]=st[i-1].str[k][j];
	}
	for(int i=head ; i>n ; i--)mark[i]=mark[i-1];
	input(n,s);
	head++;
}
void Menu::delet(int n){
	if(head==0 || n<0 || n>=lim)return;
	for(int i=n ; i<head ; i++)for(int j=0 ; j<60 ; j++)for(int k=0 ; k<2 ; k++){
		st[i].str[k][j]=st[i+1].str[k][j];
	}
	for(int i=n ; i<head ; i++)mark[i]=mark[i+1];
	head--;
	if(n==head)cursorUp();
}
void Menu::swap(int n1, int n2){
	if(n1<0 || n2<0 || n1>=lim || n2>=lim)return;
	String s;
	int m=mark[n1];
	for(int i=0 ; i<60 ; i++)for(int k=0 ; k<2 ; k++){
		s.str[k][i]=st[n1].str[k][i];
		st[n1].str[k][i]=st[n2].str[k][i];
		st[n2].str[k][i]=s.str[k][i];
	}
	mark[n1]=mark[n2];
	mark[n2]=m;
}
void Menu::inputMark(int n, int m){
	if(n<0 || n>=lim)return;
	mark[n]=m;
}
void Menu::inputColor(int n, Uint8 r, Uint8 g, Uint8 b){
	if(n<0 || n>=lim)return;
	R[n]=r;G[n]=g;B[n]=b;
}
Uint8 Menu::getR(int n){
	if(n<0 || n>=lim)return 0;
	return R[n];
}
Uint8 Menu::getG(int n){
	if(n<0 || n>=lim)return 0;
	return G[n];
}
Uint8 Menu::getB(int n){
	if(n<0 || n>=lim)return 0;
	return B[n];
}
void Menu::cursorUp(){
	if(show+pageOff!=0)Mix_PlayChannel(2, sf.cursor_move, 0);
	if(show>0)show-=combo;
	else if(show==0 && pageOff>0)pageOff-=combo;
}
void Menu::cursorDown(){
	if(show+pageOff!=head-combo)Mix_PlayChannel(2, sf.cursor_move, 0);
	if(show<raw-combo && show+pageOff<head-combo)show+=combo;
	else if(show==raw-combo && pageOff<head-raw)pageOff+=combo;
}
void Menu::pageUp(){
	if(show+pageOff!=0)Mix_PlayChannel(2, sf.cursor_move, 0);
	if(pageOff>=raw)pageOff-=raw;
	else if(pageOff>0){show+=pageOff;pageOff=0;}
	else show=0;
	if(show>=raw)show=0;
}
void Menu::pageDown(){
	if(show+pageOff!=head-1)Mix_PlayChannel(2, sf.cursor_move, 0);
	if(pageOff<head-raw)pageOff+=raw;
	else show=head-pageOff-combo;
	if(show+pageOff>=head)show=head+pageOff-combo;
}
int Menu::selected(){
	if(combo==0)return 0;
	return (show+pageOff)/combo;
}
int Menu::count(){
	return head;
}
void Menu::reset(){
	show=0;pageOff=0;head=0;bg=128;combo=1;
	v=HIDE;
	if(lim==0)return;
	lim=0;
	delete [] st;
	delete [] gray;
	delete [] mark;
	delete [] R;
	delete [] G;
	delete [] B;
}

void FishBox::initAll(){
	max=0;
	getImage(panel,"file/img/colorpanel.png",0,0,0);
	getImage(mold,"file/img/colorpanel.png",0,0,0);
}

void FishBox::endAll(){
	freeImage(panel);
	freeImage(mold);
}

void FishBox::initFishBox(int a){
	if(max!=0)endFishBox();
	fish=new Fish[a];
	today=new int[a];
	for(int i=0 ; i<a ; i++){
		fish[i].title_num=i;
		fish[i].x=0;fish[i].y=0;
		fish[i].sta=0;fish[i].tower=0;fish[i].ch=0;
		fish[i].hour=0;fish[i].minute=0;fish[i].week=0;
		fish[i].rcv=0;fish[i].mg_rcv=0;fish[i].score=0;
		fish[i].bs=false;
		today[i]=EOF;
	}
	n=0;cx=0;cy=0;offset=0;max=a;text_count=60;
}

void FishBox::endFishBox(){
	if(max==0)return;
	delete [] fish;
	delete [] today;
	max=0;
}
void FishBox::setFish(Fish f){
	if(f.title_num<0 || f.title_num>=max)return;
	fish[f.title_num].title_num=f.title_num;
	fish[f.title_num].x=f.x;fish[f.title_num].y=f.y;
	fish[f.title_num].hour=f.hour;fish[f.title_num].minute=f.minute;
	fish[f.title_num].week=f.week;
	fish[f.title_num].sta=f.sta;fish[f.title_num].tower=f.tower;
	fish[f.title_num].ch=f.ch;
	fish[f.title_num].rcv=f.rcv;fish[f.title_num].mg_rcv=f.mg_rcv;
	fish[f.title_num].score=f.score;
	fish[f.title_num].bs=f.bs;
}
void FishBox::drawFishBox(SDL_Surface* scr){
	if(max==0)return;
	panelColor(n);
	drawImage(scr,img.menuback,40,60,0,0,560,240,128);
	for(int i=0 ; i<14 ; i++)for(int j=0 ; j<6 ; j++){
		if((j+offset)*14+i>=max || work[(j+offset)*14+i].notExist)drawImage(scr,img.symbol,i*40+40,j*40+60,476,0,34,34,255);
		else if(fish[(j+offset)*14+i].score!=0)drawImage(scr,img.symbol,i*40+40,j*40+60,(work[(j+offset)*14+i].mark%16)*34,(work[(j+offset)*14+i].mark/16)*34,34,34,255);
	}
	if(count%40<20)drawImage(scr,img.chr,cx*40+40,cy*40+60,566,0,40,40,255);
	if(fish[n].score!=0)drawTable(scr,fish[n]);
}
void FishBox::drawTable(SDL_Surface* scr, Fish f){
	if(max==0)return;
	drawImage(scr,panel,20,300,0,400,600,180,255);
	drawImage(scr,img.symbol,30,362,(work[f.title_num].mark%16)*34,(work[f.title_num].mark/16)*34,34,34,255);
	drawText2_lang(scr, 64, 362, jummingText(work[f.title_num].title,60,f.rcv,f.mg_rcv), text_count, JAPANESE);
	int tc=text_count-(int)strlen(work[f.title_num].title.str[0]);
	if(tc>=0){
		fontA=192;
		drawImage(scr,img.symbol,30,402,(sta[f.sta].mark%16)*34,(sta[f.sta].mark/16)*34,34,34,255);
		if(f.bs)sprintf_s(str,"%s",toChar(sta[f.sta].name));
		else{
			sprintf_s(str,"%s",toChar(sta[f.sta].name));
			sprintf_s(str,"%s %s Ch%2d",str,toChar(tower[f.tower].name),f.ch);
		}
		drawText(scr, 64, 402, str, (int)strlen(str));
		sprintf_s(str,"%2d:%2d(%s)",f.hour,f.minute,weekChar[f.week][CHAR_CODE]);
		drawText(scr, 50, 442, str, (int)strlen(str));
		fontA=255;
		tc*=4;
		if(tc>40)tc=40;
		drawImage(scr,panel,20,402+tc,0,502+tc,600,40-tc,255);
		drawImage(scr,panel,20,442+tc,0,542+tc,600,40-tc,255);
	}
	drawImage(scr,img.chr,370,310,280,120,100,50,255);
	int N=16,Y1=0,Y2=0,Y3=0,Y4=0;
	if(start>=34){
		N=16-(start-34)/2;
		Y1=mwave[N*4]*3;
		Y2=mwave[N*4+1]*3;
		Y3=mwave[N*4+2]*3;
		Y4=mwave[N*4+3]*3;
	}
	if(f.score>=10000)drawImage(scr,img.chr,484,320-Y4,(f.score%100000)/10000*20,520,20,40,255);
	if(f.score>=1000)drawImage(scr,img.chr,508,320-Y1,(f.score%10000)/1000*20,520,20,40,255);
	if(f.score>=100)drawImage(scr,img.chr,532,320-Y2,(f.score%1000)/100*20,520,20,40,255);
	if(f.score>=10)drawImage(scr,img.chr,556,320-Y3,(f.score%100)/10*20,520,20,40,255);
	drawImage(scr,img.chr,580,320-Y4,f.score%10*20,520,20,40,255);
}
void FishBox::panelColor(int r, int g, int b){
	if(preR==r && preG==g && preB==b)return;
	preR=r;preG=g;preB=b;
	double rate=0;
	int size=(mold->w)*(mold->h);
	Uint8 *px=(Uint8*)mold->RGB;
	Uint8 *px2=(Uint8*)panel->RGB;
	for(int i=0 ; i<size ; i++){
		if(*px!=0){
			rate=*px/255.0;
			*(Uint32*)px2=setRGB( (int)(r*rate),(int)(g*rate),(int)(b*rate) );
		}
		px+=4;px2+=4;
	}
}
void FishBox::panelColor(int N){
	if(max==0)return;
	panelColor(work[N].r,work[N].g,work[N].b);
}
void FishBox::cursorUp(){
	if(cy+offset!=0)Mix_PlayChannel(2, sf.cursor_move, 0);
	if(cy>0)cy--;
	else if(cy==0 && offset>0)offset--;
	n=(cy+offset)*14+cx;
	panelColor(n);
}
void FishBox::cursorDown(){
	if(cy+offset!=(max-1)/14)Mix_PlayChannel(2, sf.cursor_move, 0);
	if(cy<5 && cy+offset<(max-1)/14)cy++;
	else if(cy==5 && offset<(max-1)/14-5)offset++;
	n=(cy+offset)*14+cx;
	if(n>=max){cx=(max-1)%14;n=max-1;}
	panelColor(n);
}
void FishBox::cursorLeft(){
	if(cx>0)Mix_PlayChannel(2, sf.cursor_move, 0);
	if(cx>0)cx--;
	n=(cy+offset)*14+cx;
	panelColor(n);
}
void FishBox::cursorRight(){
	if(cx<13 && (cy+offset!=(max-1)/14 || cx!=(max-1)%14))Mix_PlayChannel(2, sf.cursor_move, 0);
	if(cx<13)cx++;
	if((cy+offset)*14+cx>=max)cx--;
	n=(cy+offset)*14+cx;
	panelColor(n);
}

int FishBox::getNum(){
	return n;
}
int FishBox::getX(){
	if(max==0)return 0;
	return fish[n].x;
}
int FishBox::getY(){
	if(max==0)return 0;
	return fish[n].y;
}
int FishBox::getH(){
	if(max==0)return 0;
	return fish[n].hour;
}
int FishBox::getM(){
	if(max==0)return 0;
	return fish[n].minute;
}
int FishBox::getSC(int i){
	if(i<0 || i>=max)return 0;
	return fish[i].score;
}
bool FishBox::loaded(){
	if(max==0)return false;
	else return true;
}
int FishBox::getData(int i, int k){
	if(i<0 || i>=max)return 0;
	if(k==0)return fish[i].x;
	if(k==1)return fish[i].y;
	if(k==2)return fish[i].sta;
	if(k==3)return fish[i].bs;
	if(k==4)return fish[i].tower;
	if(k==5)return fish[i].ch;
	if(k==6)return fish[i].hour;
	if(k==7)return fish[i].minute;
	if(k==8)return fish[i].week;
	if(k==9)return fish[i].rcv;
	if(k==10)return fish[i].mg_rcv;
	if(k==11)return fish[i].score;
	return 0;
}
void FishBox::setData(int i, int k, int data){
	if(i<0 || i>=max)return;
	if(k==0)fish[i].x=data;
	if(k==1)fish[i].y=data;
	if(k==2)fish[i].sta=data;
	if(k==3)fish[i].bs=toBool(data);
	if(k==4)fish[i].tower=data;
	if(k==5)fish[i].ch=data;
	if(k==6)fish[i].hour=data;
	if(k==7)fish[i].minute=data;
	if(k==8)fish[i].week=data;
	if(k==9)fish[i].rcv=data;
	if(k==10)fish[i].mg_rcv=data;
	if(k==11)fish[i].score=data;
}

int to8int(char c){
	int a=c;
	if(a<0)a+=256;
	return a;
}
int to16int(char c1, char c2){
	int a=c1,b=c2;
	if(a<0)a+=256;
	if(b<0)b+=256;
	return a+b*256;
}
int to16int_signed(char c1, char c2){
	int a=c1,b=c2;
	if(a<0)a+=256;
	if(b<0)b+=256;
	if(b>=128){
		return -32768+a+(b-128)*256;
	}
	return a+b*256;
}
Uint32 to32int(char c1, char c2, char c3, char c4){
	int a=c1,b=c2, c=c3, d=c4;
	if(a<0)a+=256;
	if(b<0)b+=256;
	if(c<0)c+=256;
	if(d<0)d+=256;
	return a+b*256+c*65536+d*16777216;
}
bool toBool(char c){
	if(c)return true;
	else return false;
}
char* toChar(String s){
	if(CHAR_CODE==JAPANESE)sprintf_s(str3,s.str[0]);
	else sprintf_s(str3,s.str[1]);
	return str3;
}

bool key_wait(int a){
	if(a==1 || a>=30)return false;
	else return true;
}
bool key_stop(int a){
	if(a==1)return false;
	else return true;
}

void drawText(SDL_Surface* scr, int X, int Y, const char* st){
	drawText_lang(scr,X,Y,st,0,CHAR_CODE);
}
void drawText(SDL_Surface* scr, int X, int Y, String st){
	if(CHAR_CODE==JAPANESE)drawText_lang(scr,X,Y,st.str[0],0,JAPANESE);
	else drawText_lang(scr,X,Y,st.str[1],0,EUROPEAN);
}
void drawText(SDL_Surface* scr, int X, int Y, String st, int l){
	if(CHAR_CODE==JAPANESE)drawText_lang(scr,X,Y,st.str[0],l,JAPANESE);
	else drawText_lang(scr,X,Y,st.str[1],l,EUROPEAN);
}
void drawText(SDL_Surface* scr, int x, int y, const char* st, int l){
	drawText_lang(scr,x,y,st,l,CHAR_CODE);
}
void drawText(Image* img, int X, int Y, const char* st){
	drawText_lang(img,X,Y,st,0,CHAR_CODE);
}
void drawText(Image* img, int X, int Y, String st){
	if(CHAR_CODE==JAPANESE)drawText_lang(img,X,Y,st.str[0],0,JAPANESE);
	else drawText_lang(img,X,Y,st.str[1],0,EUROPEAN);
}
void drawText(Image* img, int X, int Y, String st, int l){
	if(CHAR_CODE==JAPANESE)drawText_lang(img,X,Y,st.str[0],l,JAPANESE);
	else drawText_lang(img,X,Y,st.str[1],l,EUROPEAN);
}
void drawText(Image* img, int x, int y, char* st, int l){
	drawText_lang(img,x,y,st,l,CHAR_CODE);
}
void drawText_lang(SDL_Surface* scr, int x, int y, String st, int l, int lang){
	if(lang==JAPANESE)drawText_lang(scr,x,y,st.str[0],l,JAPANESE);
	else drawText_lang(scr,x,y,st.str[1],l,EUROPEAN);
}
void drawText_lang(Image* img, int x, int y, String st, int l, int lang){
	if(lang==JAPANESE)drawText_lang(img,x,y,st.str[0],l,JAPANESE);
	else drawText_lang(img,x,y,st.str[1],l,EUROPEAN);
}

void drawText_lang(SDL_Surface* scr, int x, int y, const char* st, int l, int lang){
	int i=0,s=0,s2=0,p=0;
	if(l<0)l=0;
	while(i<l || l==0){
		if(st[i]==0)break;
		s=st[i];
		if(s<0)s+=256;
		s2=st[i+1];
		if(s2<0)s2+=256;

		if(lang==JAPANESE){
			if(((s>=129 && s<=159) || s>=224) && s2!=0){
				if(s>=224)s-=64;
				s=(s-129)*192;
				s+=s2-64;
				drawImage(scr, font[s/256+1], x+p,y, ((s%256)%16)*17,((s%256)/16)*17,17,17, fontA);
				p+=16;i++;
			}else{
				drawImage(scr, font[0], x+p,y, (s%32)*9,(s/32)*17,9,17, fontA);
				p+=8;
			}
		}else{
			drawImage(scr, font[46], x+p,y, (s%32)*9,(s/32)*17,9,17, fontA);
			p+=8;
		}
		i++;
	}
}

void drawText_lang(Image* img, int x, int y, const char* st, int l, int lang){
	int i=0,s=0,s2=0,p=0;
	if(l<0)l=0;
	while(i<l || l==0){
		if(st[i]==0)break;
		s=st[i];
		if(s<0)s+=256;
		s2=st[i+1];
		if(s2<0)s2+=256;

		if(lang==JAPANESE){
			if(((s>=129 && s<=159) || s>=224) && s2!=0){
				if(s>=224)s-=64;
				s=(s-129)*192;
				s+=s2-64;
				drawImage(img, font[s/256+1], x+p,y, ((s%256)%16)*17,((s%256)/16)*17,17,17, fontA);
				p+=16;i++;
			}else{
				drawImage(img, font[0], x+p,y, (s%32)*9,(s/32)*17,9,17, fontA);
				p+=8;
			}
		}else{
			drawImage(img, font[46], x+p,y, (s%32)*9,(s/32)*17,9,17, fontA);
			p+=8;
		}
		i++;
	}
}

void drawText2(SDL_Surface* scr, int X, int Y, const char* st){
	drawText2_lang(scr,X,Y,st,0,CHAR_CODE);
}
void drawText2(SDL_Surface* scr, int X, int Y, String st){
	if(CHAR_CODE==JAPANESE)drawText2_lang(scr,X,Y,st.str[0],0,JAPANESE);
	else drawText2_lang(scr,X,Y,st.str[1],0,EUROPEAN);
}
void drawText2(SDL_Surface* scr, int X, int Y, String st, int l){
	if(CHAR_CODE==JAPANESE)drawText2_lang(scr,X,Y,st.str[0],l,JAPANESE);
	else drawText2_lang(scr,X,Y,st.str[1],l,EUROPEAN);
}
void drawText2(SDL_Surface* scr, int x, int y, const char* st, int l){
	drawText2_lang(scr,x,y,st,l,CHAR_CODE);
}
void drawText2(Image* img, int X, int Y, const char* st){
	drawText2_lang(img,X,Y,st,0,CHAR_CODE);
}
void drawText2(Image* img, int X, int Y, String st){
	if(CHAR_CODE==JAPANESE)drawText2_lang(img,X,Y,st.str[0],0,JAPANESE);
	else drawText2_lang(img,X,Y,st.str[1],0,EUROPEAN);
}
void drawText2(Image* img, int X, int Y, String st, int l){
	if(CHAR_CODE==JAPANESE)drawText2_lang(img,X,Y,st.str[0],l,JAPANESE);
	else drawText2_lang(img,X,Y,st.str[1],l,EUROPEAN);
}
void drawText2(Image* img, int x, int y, const char* st, int l){
	drawText2_lang(img,x,y,st,l,CHAR_CODE);
}
void drawText2_lang(SDL_Surface* scr, int x, int y, String st, int l, int lang){
	if(lang==JAPANESE)drawText2_lang(scr,x,y,st.str[0],l,JAPANESE);
	else drawText2_lang(scr,x,y,st.str[1],l,EUROPEAN);
}
void drawText2_lang(Image* img, int x, int y, String st, int l, int lang){
	if(lang==JAPANESE)drawText2_lang(img,x,y,st.str[0],l,JAPANESE);
	else drawText2_lang(img,x,y,st.str[1],l,EUROPEAN);
}

void drawText2_lang(SDL_Surface* scr, int x, int y, const char* st, int l, int lang){
	int i=0,s=0,s2=0,p=0;
	if(l<0)l=0;
	while(i<l || l==0){
		if(st[i]==0)break;
		s=st[i];
		if(s<0)s+=256;
		s2=st[i+1];
		if(s2<0)s2+=256;

		if(lang==JAPANESE){
			if(((s>=129 && s<=159) || s>=224) && s2!=0){
				if(s>=224)s-=64;
				s=(s-129)*192;
				s+=s2-64;
				drawImage_x(scr, font[s/256+1], x+p,y, 2, ((s%256)%16)*17,((s%256)/16)*17,17,17, fontA);
				p+=32;i++;
			}else{
				drawImage_x(scr, font[0], x+p,y, 2, (s%32)*9,(s/32)*17,9,17, fontA);
				p+=16;
			}
		}else{
			drawImage_x(scr, font[46], x+p,y, 2, (s%32)*9,(s/32)*17,9,17, fontA);
			p+=16;
		}
		i++;
	}
}

void drawText2_lang(Image* img, int x, int y, const char* st, int l, int lang){
	int i=0,s=0,s2=0,p=0;
	if(l<0)l=0;
	while(i<l || l==0){
		if(st[i]==0)break;
		s=st[i];
		if(s<0)s+=256;
		s2=st[i+1];
		if(s2<0)s2+=256;

		if(lang==JAPANESE){
			if(((s>=129 && s<=159) || s>=224) && s2!=0){
				if(s>=224)s-=64;
				s=(s-129)*192;
				s+=s2-64;
				drawImage_x(img, font[s/256+1], x+p,y, 2, ((s%256)%16)*17,((s%256)/16)*17,17,17, fontA);
				p+=32;i++;
			}else{
				drawImage_x(img, font[0], x+p,y, 2, (s%32)*9,(s/32)*17,9,17, fontA);
				p+=16;
			}
		}else{
			drawImage_x(img, font[46], x+p,y, 2, (s%32)*9,(s/32)*17,9,17, fontA);
			p+=16;
		}
		i++;
	}
}

void putHeadMark(String &s){
	//２バイト文字コードの頭にマークをつける
	int a;
	for(int k=0 ; k<2 ; k++){
		for(int j=0 ; j<100 ; j++)s.head[k][j]=false;
		for(int j=0 ; j<100 ; j++){
			a=s.str[k][j];
			if(a==0)break;
			if(a<0)a+=256;
			if(k==0 && ((a>=129 && a<=159) || a>=224)){//Japanese
				s.head[0][j]=true;
				s.head[0][j+1]=false;
				j++;
			}
			else s.head[k][j]=true;
		}
	}
}

void getImage(Image*& im, const char* st, int r, int g, int b){
	//実際のimgにアクセスするために&を付けている(参照型の&)
	SDL_Surface *img2=NULL, *img3;
	img2=IMG_Load(st);
	if(img2==NULL){
		im=NULL;
		return;
	}
	if(SDL_BYTEORDER==SDL_BIG_ENDIAN)img3=SDL_CreateRGBSurface(SDL_SWSURFACE, img2->w, img2->h, 32, 0xff000000,0x00ff0000,0x0000ff00,0);
	else img3=SDL_CreateRGBSurface(SDL_SWSURFACE, img2->w, img2->h, 32, 0x000000ff,0x0000ff00,0x00ff0000,0);
	drawSurface(img3,img2,0,0,0,0,img2->w,img2->h,255);
	SDL_FreeSurface(img2);

	im=new Image(img3->w,img3->h);

	SDL_LockSurface(img3);
	SDL_PixelFormat *f = img3->format;
	Uint8 *px = (Uint8*)img3->pixels;
	Uint8 bypp = f->BytesPerPixel;
	Uint8 *rgb = (Uint8*)im->RGB;
	Uint8 R,G,B;

	for(int j=0 ; j<(im->h) ; j++)for(int i=0 ; i<(im->w) ; i++){
		SDL_GetRGB(*(Uint32*)px,f,&R,&G,&B);
		*(Uint32*)rgb = setRGB(R,G,B);
		rgb+=4;px+=bypp;
	}

	SDL_UnlockSurface(img3);
	SDL_FreeSurface(img3);
	setAlpha(im,r,g,b);
}

void getImage(Image*& im, char* st){
	SDL_Surface *img2=NULL, *img3;
	img2=IMG_Load(st);
	if(img2==NULL){
		im=NULL;
		return;
	}

	if(SDL_BYTEORDER==SDL_BIG_ENDIAN)img3=SDL_CreateRGBSurface(SDL_SWSURFACE, img2->w, img2->h, 32, 0xff000000,0x00ff0000,0x0000ff00,0);
	else img3=SDL_CreateRGBSurface(SDL_SWSURFACE, img2->w, img2->h, 32, 0x000000ff,0x0000ff00,0x00ff0000,0);
	drawSurface(img3,img2,0,0,0,0,img2->w,img2->h,255);
	SDL_FreeSurface(img2);

	im=new Image(img3->w,img3->h);

	SDL_LockSurface(img3);
	SDL_PixelFormat *f = img3->format;
	Uint8 *px = (Uint8*)img3->pixels;
	Uint8 bypp = f->BytesPerPixel;
	Uint8 *rgb = (Uint8*)im->RGB;
	Uint8 R,G,B;

	for(int j=0 ; j<(im->h) ; j++){
		for(int i=0 ; i<(im->w) ; i++){
			SDL_GetRGB(*(Uint32*)px,f,&R,&G,&B);
			*(Uint32*)rgb = setRGB(R,G,B);
			rgb+=4;px+=bypp;
		}
	}

	SDL_UnlockSurface(img3);
	SDL_FreeSurface(img3);
	resetAlpha(im);
}

void freeImage(Image*& im){
	if(!im){im=NULL;return;}
	delete [] im->RGB;
	delete [] im->A;
	delete im;
	
	im=NULL;
}

void freeMusic(){
	if(bgm!=NULL){
		Mix_FreeMusic(bgm);
		bgm=NULL;
	}
}

void getSymbolImage(){
	Image *img2;
	int a=0;
	while(true){
		sprintf_s(str,"file/img/symbol%d.png",a);
		if(!loadFile(str))break;
		a++;
	}
	img.symbol=new Image(544,a*544);
	fillRect(img.symbol,0,0,544,a*544,0,0,254,255);
	for(int i=0 ; i<a ; i++){
		sprintf_s(str,"file/img/symbol%d.png",i);
		getImage(img2,str,0,0,254);
		drawImage(img.symbol,img2,0,i*544,0,0,544,544,255);
		freeImage(img2);
	}
	setAlpha(img.symbol,0,0,254);
}

void drawLight(SDL_Surface* scr, Image* img, int x, int y, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || img==NULL)return;
	if(a<0)a=0;
	if(a>255)a=255;
	SDL_LockSurface(scr);
	SDL_PixelFormat *f = scr->format;
	Uint8 *px = (Uint8*)scr->pixels;
	Uint16 pitch = scr->pitch;
	Uint8 bypp = f->BytesPerPixel;
	Uint8 *rgb=(Uint8*)img->RGB;
	Uint16 px_skip,rgb_skip;

	if(x2<0){w2+=x2;x2=0;}
	if(y2<0){h2+=y2;y2=0;}
	if(x2+w2>(img->w))w2=(img->w)-x2;
	if(y2+h2>(img->h))h2=(img->h)-y2;
	if(x<0){x2-=x;w2+=x;x=0;}
	if(y<0){y2-=y;h2+=y;y=0;}
	if(x+w2>(scr->w))w2=(scr->w)-x;
	if(y+h2>(scr->h))h2=(scr->h)-y;
	px=px+pitch*y+bypp*x;
	px_skip=pitch-bypp*w2;
	rgb=rgb+(y2*(img->w)+x2)*4;
	rgb_skip=((img->w)-w2)*4;

	if(a==255){
		for(int k=0 ; k<h2 ; k++){
			for(int j=0 ; j<w2 ; j++){
				if(*px+*rgb>255)*px=255;
				else *px+=*rgb;
				px++;rgb++;
				if(*px+*rgb>255)*px=255;
				else *px+=*rgb;
				px++;rgb++;
				if(*px+*rgb>255)*px=255;
				else *px+=*rgb;
				px++;rgb++;
				px++;rgb++;
			}
			px+=px_skip;
			rgb+=rgb_skip;
		}
	}else{
		for(int k=0 ; k<h2 ; k++){
			for(int j=0 ; j<w2 ; j++){
				if(*px+(*rgb)*a/255>255)*px=255;
				else *px+=(*rgb)*a/255;
				px++;rgb++;
				if(*px+(*rgb)*a/255>255)*px=255;
				else *px+=(*rgb)*a/255;
				px++;rgb++;
				if(*px+(*rgb)*a/255>255)*px=255;
				else *px+=(*rgb)*a/255;
				px++;rgb++;
				px++;rgb++;
			}
			px+=px_skip;
			rgb+=rgb_skip;
		}
	}

	SDL_UnlockSurface(scr);
}

void drawLight(Image* scr, Image* img, int x, int y, int x2, int y2, int w2, int h2, int a){
	if(scr==NULL || img==NULL)return;
	if(a<0)a=0;
	if(a>255)a=255;
	Uint8 *px=(Uint8*)scr->RGB;
	Uint8 *rgb=(Uint8*)img->RGB;
	Uint16 px_skip,rgb_skip;

	if(x2<0){w2+=x2;x2=0;}
	if(y2<0){h2+=y2;y2=0;}
	if(x2+w2>(img->w))w2=(img->w)-x2;
	if(y2+h2>(img->h))h2=(img->h)-y2;
	if(x<0){x2-=x;w2+=x;x=0;}
	if(y<0){y2-=y;h2+=y;y=0;}
	if(x+w2>(scr->w))w2=(scr->w)-x;
	if(y+h2>(scr->h))h2=(scr->h)-y;
	px=px+(y*(scr->w)+x)*4;
	px_skip=((scr->w)-w2)*4;
	rgb=rgb+(y2*(img->w)+x2)*4;
	rgb_skip=((img->w)-w2)*4;

	if(a==255){
		for(int k=0 ; k<h2 ; k++){
			for(int j=0 ; j<w2 ; j++){
				if(*px+*rgb>255)*px=255;
				else *px+=*rgb;
				px++;rgb++;
				if(*px+*rgb>255)*px=255;
				else *px+=*rgb;
				px++;rgb++;
				if(*px+*rgb>255)*px=255;
				else *px+=*rgb;
				px++;rgb++;
				px++;rgb++;
			}
			px+=px_skip;
			rgb+=rgb_skip;
		}
	}else{
		for(int k=0 ; k<h2 ; k++){
			for(int j=0 ; j<w2 ; j++){
				if(*px+(*rgb)*a/255>255)*px=255;
				else *px+=(*rgb)*a/255;
				px++;rgb++;
				if(*px+(*rgb)*a/255>255)*px=255;
				else *px+=(*rgb)*a/255;
				px++;rgb++;
				if(*px+(*rgb)*a/255>255)*px=255;
				else *px+=(*rgb)*a/255;
				px++;rgb++;
				px++;rgb++;
			}
			px+=px_skip;
			rgb+=rgb_skip;
		}
	}
}

void drawSurface(SDL_Surface* sdl, SDL_Surface* img, int x, int y, int x2, int y2, int w2, int h2, int a){
	if(img==NULL)return;
	if(a<0)a=0;
	if(a>255)a=255;
	SDL_Rect rect,rect2;
	SDL_SetSurfaceAlphaMod(img, a);
	rect.x=x;rect.y=y;rect.w=0;rect.h=0;
	rect2.x=x2;rect2.y=y2;rect2.w=w2;rect2.h=h2;
	SDL_BlitSurface(img, &rect2, sdl, &rect);
}

void drawKeyboard(SDL_Surface* scr, int k, int X, int Y){
	int a=0;
	for(int i=0 ; i<82 ; i++){
		if(k==key_enable[i]){
			a=i;
			break;
		}
	}
	drawImage(scr,img.keyboard,X,Y,(a%16)*17,(a/16)*17,17,17,255);
}

void drawKeyboard2(SDL_Surface* scr, int k, int X, int Y){
	int a=0;
	for(int i=0 ; i<82 ; i++){
		if(k==key_enable[i]){
			a=i;
			break;
		}
	}
	drawImage_x(scr,img.keyboard,X,Y,2,(a%16)*17,(a/16)*17,17,17,255);
}

void fix_scrXY(){
	gd.scrX=(int)(gd.x*MAGNIFY)-320;
	gd.scrY=(int)(gd.y*MAGNIFY)-240;
	if(gd.scrX<0)gd.scrX=0;
	if(gd.scrX>map.mapW*MAGNIFY-640)gd.scrX=map.mapW*MAGNIFY-640;
	if(gd.scrY<0)gd.scrY=0;
	if(gd.scrY>map.mapH*MAGNIFY-480)gd.scrY=map.mapH*MAGNIFY-480;
}

void fix_XY(){
	if(gd.x<0)gd.x=0;
	if(gd.x>=map.mapW)gd.x=map.mapW-1;
	if(gd.y<0)gd.y=0;
	if(gd.y>=map.mapH)gd.y=map.mapH-1;
	if(!MAP3D){
		int X=(int)(gd.x*100),Y=(int)(gd.y*100);
		X/=100/MAGNIFY;
		X*=100/MAGNIFY;
		Y/=100/MAGNIFY;
		Y*=100/MAGNIFY;
		gd.dotX=X/100.0;gd.dotY=Y/100.0;
	}
}

void TalkingAt(int n){
	gd.talk_count=0;
	gd.face_count=0;
	gd.scene_count=0;
	for(int i=0 ; i<n ; i++){
		while(face[gd.face_count]!=EOF && gd.face_count<1000){
			if(face[gd.face_count]==HANGUP){
				gd.face_count+=4;
				continue;
			}
			if(face[gd.face_count]!=COMMA && face[gd.face_count]!=SHAKE){
				gd.talk_count++;
			}
			gd.face_count++;
		}
		gd.face_count++;
	}
}

void drawTalking(SDL_Surface* scr){
	drawTalking(scr,face[gd.face_count],talk[gd.talk_count]);
}

void drawTalking(SDL_Surface* scr, int fc, String st){
	if(gd.talk_open_count!=0){
		int w=600*gd.talk_open_count/20;
		int h=120*gd.talk_open_count/20;
		if(gd.talk_count==EOF){
			w=600-w;
			h=120-h;
		}
		drawRect(scr,20,360,w,h,128,128,255,255);
		return;
	}
	if(gd.talk_count==EOF)return;
	int a=0,b=0,c=0,d=0;
	if(gd.text_count<30)a=gd.text_count;
	else if(gd.text_count<60){a=30;b=gd.text_count-30;}
	else if(gd.text_count<90){a=30;b=30;c=gd.text_count-60;}
	else{a=30;b=30;c=30;}
	if(gd.shake_count>0)d=(3-abs(6-gd.shake_count%12))*gd.shake_count/6;
	drawImage(scr,img.menuback,20+d,360,0,0,600,120,192);
	drawImage(scr,img.facechip,20+d,370,((fc-1)%5)*100,((fc-1)/5)*100,100,100,255);
	if(a!=0)drawText2(scr,120+d,360,&(st.str[CHAR_CODE][0]),a);
	if(b!=0)drawText2(scr,120+d,400,&(st.str[CHAR_CODE][30]),b);
	if(c!=0)drawText2(scr,120+d,440,&(st.str[CHAR_CODE][60]),c);
}

void fillRect(SDL_Surface* scr, int x, int y, int w, int h, int R,int G,int B, int a){
	SDL_LockSurface(scr);
	SDL_PixelFormat *f = scr->format;
	Uint16 pitch = scr->pitch;
	Uint8 bypp = f->BytesPerPixel;
	Uint16 px_skip;

	if(a<0)a=0;
	if(a>255)a=255;
	if(x<0){w+=x;x=0;}
	if(y<0){h+=y;y=0;}
	if(x+w>(scr->w))w=(scr->w)-x;
	if(y+h>(scr->h))h=(scr->h)-y;

	if(a==255){
		Uint32 *px = (Uint32*)scr->pixels;
		px=px + ((scr->pitch)/4)*y + x;
		px_skip=(scr->pitch)/4-w;
		Uint32 color=setRGB(R,G,B);
		for(int j=0 ; j<h ; j++){
			for(int i=0 ; i<w ; i++){
				*px = color;
				px++;
			}
			px+=px_skip;
		}
	}else{
		Uint8 *px = (Uint8*)scr->pixels;
		px=px + pitch*y + bypp*x;
		px_skip=pitch-bypp*w;
		Uint8 R2,B2;
		if(ABGR){
			R2=R;B2=B;
		}else{
			R2=B;B2=R;
		}
		for(int j=0 ; j<h ; j++){
			for(int i=0 ; i<w ; i++){
				*px+=( (a*(R2-*px)) >> 8 );
				px++;
				*px+=( a*(G-*px ) >> 8 );
				px++;
				*px+=( a*(B2-*px ) >> 8 );
				px++;px++;
			}
			px+=px_skip;
		}
	}
	SDL_UnlockSurface(scr);
}

void drawRect(Image* scr, int x, int y, int w, int h, int R, int G, int B, int a){
	fillRect(scr,x,y,w,1,R,G,B,a);
	fillRect(scr,x,y+h-1,w,1,R,G,B,a);
	fillRect(scr,x,y,1,h,R,G,B,a);
	fillRect(scr,x+w-1,y,1,h,R,G,B,a);
}

void drawRect(SDL_Surface* scr, int x, int y, int w, int h, int R, int G, int B, int a){
	fillRect(scr,x,y,w,1,R,G,B,a);
	fillRect(scr,x,y+h-1,w,1,R,G,B,a);
	fillRect(scr,x,y,1,h,R,G,B,a);
	fillRect(scr,x+w-1,y,1,h,R,G,B,a);
}

void fillRect(Image* scr, int x, int y, int w, int h, int R,int G,int B, int a){
	Uint16 px_skip;
	if(a<0)a=0;
	if(a>255)a=255;
	if(x<0){w+=x;x=0;}
	if(y<0){h+=y;y=0;}
	if(x+w>(scr->w))w=(scr->w)-x;
	if(y+h>(scr->h))h=(scr->h)-y;

	if(a==255){
		Uint32 *px=scr->RGB;
		Uint32 color=setRGB(R,G,B);
		px=px+y*(scr->w)+x;
		px_skip=(scr->w)-w;
		for(int j=0 ; j<h ; j++){
			for(int i=0 ; i<w ; i++){
				*px = color;
				px++;
			}
			px+=px_skip;
		}
	}else{
		Uint8 *px = (Uint8*)scr->RGB;
		Uint8 R2,B2;
		px=px+(y*(scr->w)+x)*4;
		px_skip=((scr->w)-w)*4;
		if(ABGR){
			R2=R;B2=B;
		}else{
			R2=B;B2=R;
		}
		for(int j=0 ; j<h ; j++){
			for(int i=0 ; i<w ; i++){
				*px+=( a*(R2-*px) >> 8 );
				px++;
				*px+=( a*(G-*px ) >> 8 );
				px++;
				*px+=( a*(B2-*px ) >> 8 );
				px++;px++;
			}
			px+=px_skip;
		}
	}
}

void setAlpha(Image* scr, int R,int G,int B){
	Uint8 *px = (Uint8*)scr->RGB;
	Uint8 *alpha = (Uint8*)scr->A;
	Uint32 color;
	if(R==0 && G==0 && B==0){
		color=0;
	}else{
		color=setRGB(img.alphaR,img.alphaG,img.alphaB);
	}
	for(int j=0 ; j<scr->h ; j++){
		for(int i=0 ; i<scr->w ; i++){
			if( *(Uint32 *)px == color)*alpha=0;
			else *alpha=255;
			px+=4;
			alpha++;
		}
	}
}

void resetAlpha(Image* scr){
	Uint8 *alpha = (Uint8*)scr->A;
	for(int j=0 ; j<scr->h ; j++){
		for(int i=0 ; i<scr->w ; i++){
			*alpha=255;
			alpha++;
		}
	}
}

void initFont(){
	SDL_Surface *img2;
	Image *font2;
	SDL_Color col;
	for(int k=0 ; k<47 ; k++){
		img2=NULL;
		sprintf_s(str,"file/font/font%d.gif",k);
		img2=IMG_Load(str);
		if(img2!=NULL){
			SDL_FreeSurface(img2);
			getImage(font2,str,0,0,0);
			font[k]=new Image(font2->w,font2->h);
			fillRect(font[k],0,0,font2->w,font2->h,img.alphaR,img.alphaG,img.alphaB,255);
			drawImage(font[k],font2,0,0,0,0,font2->w,font2->h,255);
			freeImage(font2);
			for(int j=0 ; j<font[k]->h ; j++)for(int i=0 ; i<font[k]->w ; i++){
				getRGB( font[k]->RGB[j*(font[k]->w)+i], &col.r, &col.g, &col.b);
				//青は背景、黒は縁取り
				if(col.r==0 && col.g==0 && col.b==0)continue;
				if(col.r==img.alphaR && col.g==img.alphaG && col.b==img.alphaB)continue;
				for(int b=-1 ; b<=1 ; b++)for(int a=-1 ; a<=1 ; a++){
					if((a==0 && b==0) || i+a<0 || i+a>=font[k]->w || j+b<0 || j+b>=font[k]->h)continue;
					getRGB( font[k]->RGB[(j+b)*(font[k]->w)+(i+a)], &col.r, &col.g, &col.b);
					//黒の縁取りをほどこす
					if(col.r==img.alphaR && col.g==img.alphaG && col.b==img.alphaB){
						font[k]->RGB[(j+b)*(font[k]->w)+(i+a)] = setRGB(0,0,0);
					}
				}
			}
			setAlpha(font[k],img.alphaR,img.alphaG,img.alphaB);
		}
	}
}

void endFont(){
	for(int i=0 ; i<47 ; i++){
		if(font[i]!=NULL)freeImage(font[i]);
	}
}

bool loadFile(const char* fn){
	fpos_t a=0,b=0;
	FILE* hFile;

	if(fsize)delete [] fstr;
	fsize=0;
	fopen_s(&hFile,fn,"rb");
	if(hFile==NULL)return false;

	b=fseek(hFile,0,SEEK_END);
	fgetpos(hFile,&a);
	fsize=(size_t)a;
	fseek(hFile,(long)b,SEEK_SET);

	fstr=new char[fsize];
	fread(fstr, sizeof(fstr[0]), fsize/sizeof(fstr[0]), hFile);
	fclose(hFile);
	return true;
}

void setKick(SDL_Surface* scr){
	if(kick_count==1){
		drawImage(img.pre_scr,scr,0,0,0,0,640,480,255);
		Mix_PlayChannel(0,sf.decide,0);
	}
}

void drawKick(SDL_Surface* scr){
	if(kick_count>=1){
		int X=kick_count*20,Y=(10-kick_count)*(10-kick_count)-200;
		int X2=0,Y2=380;
		if(kick_count<15){X2=kick_count*4-60;Y2=440-kick_count*4;}
		drawImage(scr,img.pre_scr,X,Y,0,0,640,480,255);
		drawImage(scr,img.facechip,X2,Y2,200,0,100,100,255);
		kick_count++;
		if(kick_count==30)kick_count=0;
	}
}

void reset_key(){
	key.upC=SDLK_UP;
	key.downC=SDLK_DOWN;
	key.leftC=SDLK_LEFT;
	key.rightC=SDLK_RIGHT;
	key.aC=SDLK_a;
	key.zC=SDLK_z;
	key.xC=SDLK_x;
	key.cC=SDLK_c;
}

String jummingText(String s, int array_num, int rcv, int mg_rcv){
	//noizeの数だけ*印が入る
	//slashの数だけ/印が入る
	int noize=100-(rcv-mg_rcv);
	int slash=0,offset;
	if(noize<=0)noize=0;
	else if(noize<16)noize=1;
	else noize=noize*noize*array_num/10000;
	if(rcv-mg_rcv<100){
		if(rcv+mg_rcv>100)slash=100-(rcv-mg_rcv);
		else slash=mg_rcv;
	}
	if(slash>0 && slash<3)slash=1;
	else slash=slash*array_num/100;

	for(int i=0 ; i<array_num ; i++)str[i]=false;
	for(int k=0 ; k<2 ; k++){
		for(int i=0 ; i<noize ; i++){
			char chr='*';
			if(i<slash)chr='/';
			int a=rand()%array_num;
			for(int j=0 ; j<array_num ; j++){
				offset=(a+j)%array_num;
				if(s.str[k][offset]==0){
					if(str[offset])continue;
					str[offset]=true;
					break;
				}
				if(!s.head[k][offset])continue;
				if( offset<array_num-1 && s.head[k][offset] && !s.head[k][offset+1] ){
					if(str[offset])continue;
					str[offset]=true;
					str[offset+1]=true;
					s.str[k][offset]=chr;
					s.str[k][offset+1]=chr;
					s.head[k][offset+1]=true;
					i++;
					break;
				}else{
					if(str[offset])continue;
					str[offset]=true;
					s.str[k][offset]=chr;
					break;
				}
			}
		}
	}
	return s;
}

void padSpace(int n, int k, int x){
	int a;
	a=0;
	for(int i=0 ; i<100 ; i++){
		if(talk[n].str[k][i]==0)break;
		else if(talk[n].str[k][i]==10){
			if(a==0){
				for(int ii=i ; ii<99 ; ii++){
					talk[n].str[k][ii]=talk[n].str[k][ii+1];
					talk[n].head[k][ii]=talk[n].head[k][ii+1];
				}
				i--;
			}else{
				int sp=x-a;
				for(int ii=99-sp+1 ; ii>=i ; ii--){
					if(ii-i!=0){
						talk[n].str[k][ii+sp-1]=talk[n].str[k][ii];
						talk[n].head[k][ii+sp-1]=talk[n].head[k][ii];
					}
					if(ii-i<sp){
						talk[n].str[k][ii]=' ';
						talk[n].head[k][ii]=true;
					}
				}
				i+=sp-1;
				a=0;
			}
		}else{
			a++;
			if(a>=x)a=0;
		}
	}
}

inline void getRGB(Uint32 px, Uint8 *r, Uint8 *g, Uint8 *b){
	if(ABGR){
		*r=(px & 255);
		*g=(px>>8 & 255);
		*b=(px>>16 & 255);
	}else{
		*b=(px & 255);
		*g=(px>>8 & 255);
		*r=(px>>16 & 255);
	}
}

inline Uint32 setRGB(Uint8 r, Uint8 g, Uint8 b){
	if(ABGR){
		return ( b<<16 | g<<8 | r );
	}else{
		return ( r<<16 | g<<8 | b );
	}
}

Image::Image(int W, int H){
	w=W;h=H;
	RGB=new Uint32[W*H];
	A=new Uint8[W*H];
	for(int i=0 ; i<W*H ; i++){
		RGB[i]=0;A[i]=255;
	}
}

int controlTalking(){
	if(gd.text_count<(int)strlen(talk[gd.talk_count].str[CHAR_CODE])){
		gd.text_count=90;
	}else{
		gd.text_count=0;
		gd.talk_count++;
		gd.face_count++;
		if(face[gd.face_count]==SHAKE){
			gd.shake_count=50;
			gd.face_count++;
		}
		if(face[gd.face_count]==COMMA){
			gd.scene_count++;
			gd.face_count++;
			if(face[gd.face_count]==SHAKE){
				gd.shake_count=50;
				gd.face_count++;
			}
			return COMMA;
		}
		if(face[gd.face_count]==HANGUP){
			gd.face_count++;
			gd.shake_count=0;
			gd.talk_open_count=1;
			gd.talk_count=EOF;
			return HANGUP;
		}
		if(face[gd.face_count]==EOF){
			gd.shake_count=0;
			gd.talk_open_count=1;
			gd.talk_count=EOF;
			return EOF;
		}
	}
	return 0;
}

void controlTextCount(bool ok){
	if(gd.talk_open_count>0){
		gd.talk_open_count++;
		if(gd.talk_open_count==20)gd.talk_open_count=0;
	}
	if(gd.talk_open_count==0 && ok){
		if(gd.shake_count==49)Mix_PlayChannel(0,sf.decide,0);
		if(gd.shake_count>0)gd.shake_count--;
		while(talk[gd.talk_count].str[CHAR_CODE][gd.text_count]==' ' && talk[gd.talk_count].str[CHAR_CODE][gd.text_count+1]==' '){
			gd.text_count++;
		}
		gd.text_count++;
	}
}

#ifdef __APPLE__
void sprintf_s(char *s, const char *c, ...){
	va_list c2;
	va_start(c2, c);
	vsprintf(s,c,c2);
}
void sprintf_s(char *s, long n, const char *c, ...){
	va_list c2;
	va_start(c2, c);
	vsprintf(s,c,c2);
}
void fopen_s(FILE **f, const char* c1, const char* c2){
    *f = fopen(c1, c2);
}
void strcpy_s(char *s1, char *s2){
	strcpy(s1, s2);
}
void strcpy_s(char *s1, long n, char *s2){
	strcpy(s1, s2);
}
#endif
