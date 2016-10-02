#include "radio.h"

BOOL saveSMR=FALSE, SMRscanned=FALSE;
int SMRcount=0,SMRfast=35;
Uint16 atan_table[60000], asin_table[10001], acos_table[10001];
void create_atan_table();
RadioData rd;

void estimate(){
	if(rd.estimated)return;
/*power���v�Z����*/
	double A=(DIS62CH-DIS1CH)/674.0;
	double B=DIS1CH-A*91;
	double C=CURVE_TOP,D=CURVE_RISE;
	Tower *tw=tower;
	for(int i=0 ; i<towers ; i++){
		double n=tw->kw*1000.0;
		for(int m=0 ; m<tw->kw2 ; m++)n/=10.0;
		double ERP=tw->erp;
		for(int m=0 ; m<tw->erp2 ; m++)ERP/=10.0;
		n*=ERP;
		for(int k=0 ; k<area[tw->area].st_num ; k++){
			if(tw->ch[k]==0 || tw->ch[k]>=CHANNELS+1)tw->power[k]=0;
			else{
				double E=mhz[tw->ch[k]-1].mhz*A+B;
				if(mhz[tw->ch[k]-1].mhz<C)E+=D*sin( (PI/2)*(mhz[tw->ch[k]-1].mhz-91)/(C-91) );
				else E+=D*cos( (PI/2)*(mhz[tw->ch[k]-1].mhz-C)/(759-C) );
				tw->power[k]=(float)(sqrt(n)*E);
			}
		}
/*�d�g���̍���*/
		int tX=tw->x,tY=tw->y,tH=tw->h;
		Mount *mt=mount;
		for(int k=0 ; k<mounts ; k++){
			if(mt->city){mt++;continue;}
			int mX=mt->x,mY=mt->y;
			double mount_dis=sqrt(1.0*(tX-mX)*(tX-mX)+(tY-mY)*(tY-mY));
			if(mX-mt->range-2<=tX && tX<=mX+mt->range+2 && mY-mt->range-2<=tY && tY<=mY+mt->range+2){
				int a=mt->h - (int)((mt->h-mt->slope)*mount_dis/mt->range);
				if(tw->h<tH+a)tw->h=tH+a;
			}
			mt++;
		}
		tw++;
	}
/*�r���X�̍���*/
	Mount *mt=mount;
	Mount *mt2;
	for(int i=0 ; i<mounts ; i++)if(mt->city){
		int a=0;
		mt2=mount;
		for(int j=0 ; j<mounts ; j++)if(!mt2->city){
			double X=abs(mt2->x-mt->x),Y=abs(mt2->y-mt->y);
			if(X<=mt2->range && Y<=mt2->range){
				if(a<mt2->h-(int)( (mt2->h-mt2->slope)*sqrt(1.0*X*X+Y*Y)/mt2->range ))
				a=mt2->h-(int)( (mt2->h-mt2->slope)*sqrt(1.0*X*X+Y*Y)/mt2->range );
			}
			mt2++;
		}
		mt->h+=a;
		mt++;
	}
	create_atan_table();
	rd.estimated=TRUE;
}

void shield_each_tower(double X, double Y, Tower* T){
	double dir=T->dir, dir1=0, dir2=0;
	int dis=(int)sqrt(rd.tower_dis_multi2);
	BOOL shadow=FALSE;
	rd.seeDIS[0]=0;rd.seeH[0]=gd.height;rd.seeM[0]=0;rd.seeDIR[0]=0;rd.seeCITY[0]=FALSE;rd.see_mounts=1;

	Mount *mt=mount;
	for(int m=0 ; m<mounts ; m++){
/*rd.seeH[0]�͎����̍���*/
		if(mt->dis<=mt->range*MAP_SCALE){
			if(mt->city){mt++;continue;}
			int a=mt->h - (int)((mt->h-mt->slope)*mt->dis/(mt->range*MAP_SCALE));
			if(rd.seeH[0]<a)rd.seeH[0]=a;
		}
/*dir�̕����Ɍ�����R�̔z������*/
		if(dis<=mt->dis){mt++;continue;}
		dir1=mt->dir1;dir2=mt->dir2;
		if(dir1>270 && dir2<90){
			if(dir<90)dir1-=360;
			else dir2+=360;
		}
		if(dir2>dir && dir>dir1 && (mt->x!=T->x || mt->y!=T->y)){
			rd.seeDIS[rd.see_mounts]=(int)mt->dis;
			if(mt->dir<=dir)rd.seeH[rd.see_mounts]=mt->h - (int)((mt->h-mt->slope)*(dir-mt->dir)/(dir2-mt->dir));
			else rd.seeH[rd.see_mounts]=mt->h - (int)((mt->h-mt->slope)*(mt->dir-dir)/(mt->dir-dir1));
			rd.seeCITY[rd.see_mounts]=mt->city;
			rd.seeNUM[rd.see_mounts]=m;
			rd.seeM[rd.see_mounts]=0;
			rd.see_mounts++;
		}
		mt++;
	}
/*�Ō��rd.see�͓d�g��*/
	rd.seeDIS[rd.see_mounts]=dis;
	rd.seeH[rd.see_mounts]=T->h;
	rd.seeM[rd.see_mounts]=0;
	rd.seeCITY[rd.see_mounts]=FALSE;
	rd.see_mounts++;

/*�v���C���[�ɋ߂����ɕ��ׂ�*/
	for(int m=rd.see_mounts-2 ; m>=2 ; m--)for(int n=1 ; n<m ; n++){
		if(rd.seeDIS[n]>rd.seeDIS[n+1]){
			int see2=rd.seeDIS[n],seeH2=rd.seeH[n],seeN2=rd.seeNUM[n],seeC2=rd.seeCITY[n];
			rd.seeDIS[n]=rd.seeDIS[n+1];rd.seeH[n]=rd.seeH[n+1];rd.seeNUM[n]=rd.seeNUM[n+1];rd.seeCITY[n]=rd.seeCITY[n+1];
			rd.seeDIS[n+1]=see2;rd.seeH[n+1]=seeH2;rd.seeNUM[n+1]=seeN2;rd.seeCITY[n+1]=seeC2;
		}
	}

/*�n�����ۂ����Ƃɂ�鐷��オ�蕪��������*/
	double mound=1.0*(rd.seeDIS[rd.see_mounts-1]/8)*(rd.seeDIS[rd.see_mounts-1]/8);
	double half_point=1.0*rd.seeDIS[rd.see_mounts-1]/2;
	for(int m=1 ; m<rd.see_mounts-1 ; m++){
		if(rd.seeDIS[m]==EOF)continue;
		rd.seeM[m]=(int)(mound*sqrt( 1.0-(abs(rd.seeDIS[m]-half_point)/half_point) ));
	}

/*�Rp�ƎRm�ɋ��܂��Ⴂ�Rn������*/
	for(int p=0 ; p<rd.see_mounts ; p++){//p<n<m
		if(rd.seeDIS[p]==EOF || rd.seeCITY[p])continue;
		for(int m=p+2 ; m<rd.see_mounts ; m++){
			if(rd.seeDIS[m]==EOF || rd.seeCITY[m])continue;
			for(int n=p+1 ; n<m ; n++){
				if(rd.seeDIS[n]==EOF)continue;
				double a=1.0*((rd.seeH[m]+rd.seeM[m])-(rd.seeH[p]+rd.seeM[p]))/(rd.seeDIS[m]-rd.seeDIS[p]);
				int b=(rd.seeH[p]+rd.seeM[p])+(int)(a*(rd.seeDIS[n]-rd.seeDIS[p]));
				if((rd.seeH[n]+rd.seeM[n])<b)rd.seeDIS[n]=EOF;
			}
		}
	}

/*���ʂ���������*/
	for(int i=0 ; i<10 ; i++)T->out[i]=0;
	double a;
	BOOL in=FALSE;
	for(int m=0 ; m<rd.see_mounts ; m++){
		if(rd.seeDIS[m]==EOF || rd.seeCITY[m])continue;
		for(int n=m+1 ; n<rd.see_mounts ; n++){
			if(rd.seeDIS[n]==EOF || rd.seeCITY[n])continue;
			a=4.12*(sqrt(1.0*rd.seeH[m])+sqrt(1.0*rd.seeH[n]));
			for(int i=0 ; i<10 ; i++){
				if(rd.seeDIS[n]-rd.seeDIS[m]>mhz[T->ch[i]-1].ground*a){
					T->rcv[i]=0;
					T->out[i]=2;
				}
				else if(rd.seeDIS[n]-rd.seeDIS[m]>a){
					in=TRUE;
					T->out[i]=1;
				}
				else in=TRUE;
			}
			break;
		}
	}

if(!in)return;

/*�R���̋p*/
	for(int m=rd.see_mounts-2 ; m>=0 ; m--){
		if(rd.seeDIS[m]==EOF || rd.seeCITY[m])continue;
		for(int n=m+1 ; n<rd.see_mounts ; n++){
			if(rd.seeDIS[n]==EOF)continue;
			rd.seeDIR[m]=atan( 1.0*((rd.seeH[n]+rd.seeM[n])-(rd.seeH[m]+rd.seeM[m]))/((rd.seeDIS[n]-rd.seeDIS[m])*1000) );
			rd.seeDIR[m]=rd.seeDIR[m]*180/PI;
			break;
		}
	}
}

void shield_each_ch(Tower* T, int k){
	if(T->ch[k]==0 || T->ch[k]==CHANNELS+1)return;
	if(T->rcv[k]==0)return;
/*��ܔg��������Ȃ��Rm������*/
	for(int n=0 ; n<rd.see_mounts-1 ; n++){
		if(rd.seeDIS[n]==EOF || rd.seeCITY[n])continue;
		for(int m=1 ; m<rd.see_mounts-1 ; m++){
			if(rd.seeDIS[m]==EOF || rd.seeCITY[m])continue;
			if(rd.seeDIR[n]>rd.seeDIR[m]+mhz[T->ch[k]-1].shadow)rd.seeDIS[n]=EOF;
		}
	}

	if(rd.seeDIS[0]==EOF){T->rcv[k]=0;return;}
	double rcv=T->rcv[k];

/*�R����������������������*/
	for(int n=1 ; n<rd.see_mounts-1 ; n++)if(rd.seeDIS[n]!=EOF){
		rcv*=mhz[T->ch[k]-1].dif;
		if(rd.seeCITY[n])for(int p=1 ; p<rd.seeCITY[n] ; p++)rcv*=mhz[T->ch[k]-1].dif;
	}
	T->rcv[k]=(int)rcv;
	if(phase!=SMR_RESULT){
		T->r_num=0;
		for(int n=0 ; n<rd.see_mounts ; n++){
			if(T->r_num==20)break;
			if(rd.seeDIS[n]==EOF)continue;
			if(rd.seeCITY[n]>=1)T->r_h[T->r_num]=-1;
			else T->r_h[T->r_num]=rd.seeH[n];
			T->r_dis[T->r_num]=(Uint16)(rd.seeDIS[n]/MAP_SCALE);
			T->r_m[T->r_num]=rd.seeM[n];
			T->r_num++;
		}
	}
}

/*�R�̕��p�Ƌ������v�Z����*/
void mount_dis(double X, double Y){
	Mount *mt=mount;
	for(int i=0 ; i<mounts ; i++){
		mt->dis=(float)(MAP_SCALE*sqrt(1.0*(X-mt->x)*(X-mt->x)+(Y-mt->y)*(Y-mt->y)));
		double d1=atan_q(mt->x-X,Y-mt->y);
		double d2=atan_q(mt->dis,mt->range*MAP_SCALE);
		mt->dir=(float)d1;
		mt->dir1=(float)(d1-d2);
		mt->dir2=(float)(d1+d2);
		if(mt->dir1<0)mt->dir1+=360;
		if(mt->dir2<0)mt->dir2+=360;
		if(mt->dir1>360)mt->dir1-=360;
		if(mt->dir2>360)mt->dir2-=360;
		mt++;
	}
}

void receive_each_tower(double X, double Y, Tower* T){
/*�d�g���̕��p�Ƌ������v�Z����*/
	T->dir=(float)atan_q(T->x-X,Y-T->y);

/*�w�����ɂ���M���x���̌��蕪���v�Z*/
	if(T->bias==255)return;
	rd.bias_dir=180;
	for(int k=0 ; k<8 ; k++){
		if((T->bias>>k & 1)==1){
			int dir=(45*k+180)%360;
			if(T->dir<30 && dir>330)dir=dir-360;
			if(T->dir>330 && dir<30)dir=dir+360;
			if(abs(T->dir-dir)<=13){
				rd.bias_dir=0;break;
			}else{
				int a=(int)abs(T->dir-dir);
				if( rd.bias_dir>a )rd.bias_dir=a;
			}
		}
	}
}

void receive_each_ch(double X, double Y, Tower* T, int S){
	if(T->ch[S]==0 || T->ch[S]==CHANNELS+1)return;
	if(T->out[S] == 2)return;
	if(T->x==(int)X && T->y==(int)Y){
		T->rcv[S]=20000000;
		T->dir=0;
		return;
	}
/*��M���x�����v�Z����*/
	if(phase!=SMR_RESULT){
		T->rcv[S]=(int)( 100.0 * T->power[S] / rd.tower_dis_multi2 );
	}
	if(T->out[S] == 1)T->rcv[S]=(int)(1.0*T->rcv[S]*mhz[T->ch[S]-1].dif);
	if(T->bias!=255 && rd.bias_dir!=0){
		double a=mhz[T->ch[S]-1].ori*rd.bias_dir/180.0;
		T->rcv[S]=(int)(1.0*T->rcv[S]/a);
	}
}

void receiveAll(double X, double Y){
	mount_dis(X,Y);
	Tower *tw=tower;
	for(int i=0 ; i<towers ; i++){
		if(tw->remove){
			for(int k=0 ; k<area[tw->area].st_num ; k++)tw->rcv[k]=0;
			tw++;
			continue;
		}
		rd.tower_dis_multi2=1.0*MAP_SCALE*MAP_SCALE*( 1.0*(X-tw->x)*(X-tw->x)+1.0*(Y-tw->y)*(Y-tw->y) );
		BOOL skip=TRUE;
		for(int k=0 ; k<area[tw->area].st_num ; k++){
			if(tw->power[k] >= rd.tower_dis_multi2/100)skip=FALSE;
		}
		if(skip){
			for(int k=0 ; k<area[tw->area].st_num ; k++){
				tw->rcv[k]=0;
			}
			tw++;
			continue;
		}
		receive_each_tower(X,Y,tw);
		shield_each_tower(X,Y,tw);
		for(int j=0 ; j<area[tw->area].st_num ; j++){
			receive_each_ch(X,Y,tw,j);
			shield_each_ch(tw,j);
		}
		tw++;
	}
}

int receive_mg(int aim_tower, int CH, int DIR){
	Uint32 dir2=0,mr=0;
	Tower *tw=tower;
	for(int i=0 ; i<towers ; i++){
		for(int j=0 ; j<area[tw->area].st_num ; j++){
			if(tw->ch[j]!=CH)continue;
			if(i==aim_tower)continue;
			dir2=abs((int)(tw->dir-DIR));
			if(dir2>180)dir2=360-dir2;
			Uint32 a=tw->rcv[j]/(dir2/5+1);
			if(tower[aim_tower].v!=tw->v)a/=2;
			if( mr<a )mr=a;
		}
		tw++;
	}
	return mr;
}

void startSMR(int st){
	BOOL end=FALSE,end_mount_dis=FALSE;

	for(int a=0 ; a<SMRfast ; a++){
		end=FALSE;
		end_mount_dis=FALSE;
		int ud=1;
		if((SMRcount/(320/SMRfast))%2==0)ud=-1;
		int X=gd.scrX+a+(SMRcount%(320/SMRfast))*SMRfast;
		int Y=gd.scrY+120+(SMRcount/(320/SMRfast)+1)/2*ud;
		if(map.smr[X][Y]!=0)continue;
		saveSMR=TRUE;

		for(int i=0 ; i<areas ; i++){
			int s=-1;
			for(int j=0 ; j<area[i].st_num ; j++){
				if(area[i].station[j]==st)s=j;
			}
			if(s==-1)continue;

			Tower *tw=&tower[area[i].tower];
			for(int j=0 ; j<area[i].num ; j++){
				if(tw->remove){tw++;continue;}
				if(tw->ch[s]==0 || tw->ch[s]==CHANNELS+1){tw++;continue;}

				rd.tower_dis_multi2=1.0*MAP_SCALE*MAP_SCALE*( 1.0*(X-tw->x)*(X-tw->x)+1.0*(Y-tw->y)*(Y-tw->y) );
				if(tw->power[s] < rd.tower_dis_multi2/100){tw++;continue;}
				SMRscanned=TRUE;

				tw->rcv[s]=(int)( 100.0 * tw->power[s] / rd.tower_dis_multi2 );
				if(!end_mount_dis){mount_dis(X,Y);end_mount_dis=TRUE;}
				receive_each_tower(X,Y,tw);
				shield_each_tower(X,Y,tw);
				receive_each_ch(X,Y,tw,s);
				shield_each_ch(tw,s);
				if(tw->rcv[s]>=100){
					map.smr[X][Y]=7;
					end=TRUE;
					break;
				}
				else if(tw->rcv[s]>=80 && map.smr[X][Y]<=6)map.smr[X][Y]=6;
				else if(tw->rcv[s]>=60 && map.smr[X][Y]<=5)map.smr[X][Y]=5;
				else if(tw->rcv[s]>=40 && map.smr[X][Y]<=4)map.smr[X][Y]=4;
				else if(tw->rcv[s]>=20 && map.smr[X][Y]<=3)map.smr[X][Y]=3;
				else if(tw->rcv[s]>0 && map.smr[X][Y]<=2)map.smr[X][Y]=2;
				else if(map.smr[X][Y]<=1)map.smr[X][Y]=1;
				tw++;
			}
			if(end)break;
		}

	}
	SMRcount++;
}

void create_atan_table(){
	for(int i=0 ; i<10001 ; i++){
		asin_table[i]=(int)(asin(i/10000.0)*180/PI*10.0);
		acos_table[i]=(int)(acos(i/10000.0)*180/PI*10.0);
	}
	for(int i=0 ; i<60000 ; i++){
		atan_table[i]=(int)(atan(i/1000.0)*180/PI*10.0);
	}
}

double asin_q(double D, double X, double Y){
	if(D==0)return 0.0;
	int b=(int)(10000.0*abs(Y)/abs(D));
	int a=asin_table[b];
	if(b>=9988)a=(int)abs(asin(1.0*Y/D)*180/PI*10.0);
	if(X<0 && Y>=0)a=1800-a;
	else if(X<0 && Y<0)a+=1800;
	else if(X>=0 && Y<0)a=3600-a;
	return a/10.0;
}

double acos_q(double D, double X, double Y){
	if(D==0)return 0.0;
	int b=(int)(10000.0*abs(X)/abs(D));
	int a=acos_table[b];
	if(b<=12)a=(int)abs(acos(1.0*X/D)*180/PI*10.0);
	if(X<0 && Y>=0)a=1800-a;
	else if(X<0 && Y<0)a+=1800;
	else if(X>=0 && Y<0)a=3600-a;
	return a/10.0;
}

double atan_q(double X, double Y){
	if(X==0){
		if(Y>=0)return 90.0;
		else return 270.0;
	}
	int a=(int)(1000.0*abs(Y)/abs(X)), b=0;
	if(a>=60000)b=(int)abs(atan(1.0*Y/X)*180/PI*10.0);
	else b=atan_table[a];
	if(X<0 && Y>=0)b=1800-b;
	else if(X<0 && Y<0)b+=1800;
	else if(X>=0 && Y<0)b=3600-b;
	return b/10.0;
}
