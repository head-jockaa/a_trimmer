#include "file_io.h"

char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

void extra_station();
void extra_work(int n, int mark, String title);
void extra_tower();

void load_text(){
	size_t fc=0;

	for(int i=0 ; i<1000 ; i++)for(int j=0 ; j<61 ; j++)for(int k=0 ; k<2 ; k++){
		text[i].str[k][j]=0;
	}

	for(int k=0 ; k<2 ; k++){
		fc=0;
		if(k==0)loadFile("file/data/text_jp.dat");
		else loadFile("file/data/text_en.dat");
		for(int i=0 ; i<7 ; i++){
			for(int j=0 ; j<4 ; j++){
				weekChar[i][k][j]=fstr[fc];fc++;
				if(weekChar[i][k][j]==0)break;
			}
		}
		for(int i=0 ; i<1000 ; i++){
			if(fc>=fsize)break;
			for(int j=0 ; j<200 ; j++){
				text[i].str[k][j]=fstr[fc];
				fc++;
				if(fstr[fc-1]==0)break;
			}
		}
	}
}

void load_haziashop(){
	loadFile("file/data/haziashop.dat");
	int fc=0;
	for(int i=0 ; i<18 ; i++){
		shop_icon[i]=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	}
	for(int i=0 ; i<18 ; i++){
		price[i]=to32int(fstr[fc],fstr[fc+1],fstr[fc+2],fstr[fc+3]);fc+=4;
	}
}

void load_index(){
	load_haziashop();
	char fn[50];
	size_t fc=0;
	if(index_num)delete [] indexName;
	index_num=0;
	gd.hazia=0;
	while(true){
		sprintf_s(fn,"file/data/work/work%d.dat",index_num+1);
		if(!loadFile(fn))break;
		index_num++;
	}
	indexName=new Index[index_num];

	if(!loadFile("save/clear.dat")){
		for(int i=0 ; i<18 ; i++)gd.bought[i]=false;
		for(int i=0 ; i<index_num ; i++){
			indexName[i].rate=0;
			indexName[i].hiscore=0;
		}
		clear_num=1;
	}else{
		for(int i=0 ; i<18 ; i++){
			gd.bought[i]=fstr[fc];fc++;
			if(gd.bought[i])gd.hazia-=price[i];
		}
		for(int i=0 ; i<index_num ; i++){
			indexName[i].rate=to8int(fstr[fc]);fc++;
			indexName[i].hiscore=to32int(fstr[fc],fstr[fc+1],fstr[fc+2],fstr[fc+3]);fc+=4;
			gd.hazia+=indexName[i].hiscore;
		}
		clear_num=1;
		for(int i=0 ; i<index_num-1 ; i++){//最終ボス面は保留
			if(indexName[i].rate<60)break;
			clear_num++;
		}
	}

	for(int k=0 ; k<2 ; k++){
		fc=0;
		if(k==0)loadFile("file/data/index_jp.dat");
		else loadFile("file/data/index_en.dat");
		for(int i=0 ; i<index_num ; i++){
			for(int j=0 ; j<60 ; j++){
				indexName[i].name.str[k][j]=fstr[fc];
				fc++;
				if(fstr[fc-1]==0)break;
			}
		}
	}
}

void load_station(){
	if(stas)return;
	int fc=0;
	stas=0;

	loadFile("file/data/station.dat");
	while(fstr[fc]!=EOF || fstr[fc+1]!=EOF){
		stas++;
		fc+=2;
	}
	sta=new Station[stas+1];
	for(int k=0 ; k<2 ; k++){
		fc=0;
		if(k==0)loadFile("file/data/station_name_jp.dat");
		else loadFile("file/data/station_name_en.dat");
		for(int i=0 ; i<stas ; i++){
			for(int j=0 ; j<60 ; j++){
				sta[i].name.str[k][j]=fstr[fc];
				fc++;
				if(fstr[fc-1]==0)break;
			}
		}
	}
	for(int k=0 ; k<2 ; k++){
		fc=0;
		if(k==0)loadFile("file/data/station_talk_jp.dat");
		else loadFile("file/data/station_talk_en.dat");
		for(int i=0 ; i<stas ; i++){
			for(int j=0 ; j<200 ; j++){
				if(fstr[fc]==0){
					sta[i].talk.str[k][j]=' ';
					sta[i].talk.str[k][j+1]=0;
				}
				else sta[i].talk.str[k][j]=fstr[fc];
				fc++;
				if(fstr[fc-1]==0)break;
			}
		}
	}
	for(int i=0 ; i<stas ; i++)putHeadMark(sta[i].talk);
	fc=0;
	loadFile("file/data/station.dat");
	for(int i=0 ; i<stas ; i++){
		sta[i].mark=to16int(fstr[fc],fstr[fc+1]);
		fc+=2;
	}
	fc+=2;

	for(int i=0 ; i<12 ; i++){
		if(fstr[fc]==0)BSstation[i]=EOF;
		else{
			BSchannel[i]=to8int(fstr[fc]);
			BSstation[i]=to16int(fstr[fc+1],fstr[fc+2]);
		}
		fc+=3;
	}
	extra_station();
}

void extra_station(){
	for(int k=0 ; k<2 ; k++){
		sprintf_s(sta[stas].name.str[k],text[ANTENNATEXT+14].str[k]);
		sprintf_s(sta[stas].talk.str[k],text[ANTENNATEXT+15].str[k]);
	}
	stas++;

}

void load_story(int n){
	char fn[50];
	size_t fc=0;

	for(int i=0 ; i<1000 ; i++)for(int j=0 ; j<2 ; j++)for(int k=0 ; k<200 ; k++){
		talk[i].str[j][k]=0;
	}

	sprintf_s(fn,"file/data/story/face%d.dat",n);
	loadFile(fn);
	for(size_t i=0 ; i<1000 ; i++){
		if(i>=fsize)break;
		face[i]=fstr[i];
	}

	for(int k=0 ; k<2 ; k++){
		fc=0;
		if(k==0)sprintf_s(fn,"file/data/story/talk%d_jp.dat",n);
		else sprintf_s(fn,"file/data/story/talk%d_en.dat",n);
		loadFile(fn);
		for(int i=0 ; i<1000 ; i++){
			if(fc>=fsize)break;
			for(int j=0 ; j<200 ; j++){
				talk[i].str[k][j]=fstr[fc];fc++;
				if(talk[i].str[k][j]==0)break;
			}
		}
	}

	for(int k=0 ; k<2 ; k++)for(int i=0 ; i<1000 ; i++){
		padSpace(i,k,30);
	}
}

void load_animebook(){
	if(allworks)return;
	int fc=0;
	allworks=0;collection=0;
	loadFile("file/data/work/works.dat");
	allworks=(int)(fsize/5);
	animebook=new bool[allworks];
	if(!loadFile("save/animebook.dat")){
		for(int i=0 ; i<allworks ; i++)animebook[i]=false;
	}else{
		for(int i=0 ; i<allworks ; i++){
			animebook[i]=(fstr[i/8]>>i%8 & 1);
			if(animebook[i])collection++;
		}
	}
}

void save_animebook(){
	if(fsize)delete [] fstr;
	fsize=allworks/8+1;
	fstr=new char[fsize];
	FILE* hFile;
	int a=0;

	for(int i=0 ; i<(int)fsize ; i++)fstr[i]=0;
	for(int i=0 ; i<allworks ; i++){
		if(i%8==0)a+=animebook[i];
		else if(i%8==1)a+=animebook[i]*2;
		else if(i%8==2)a+=animebook[i]*4;
		else if(i%8==3)a+=animebook[i]*8;
		else if(i%8==4)a+=animebook[i]*16;
		else if(i%8==5)a+=animebook[i]*32;
		else if(i%8==6)a+=animebook[i]*64;
		else if(i%8==7)a+=animebook[i]*128;
		if(i%8==7 || i==allworks-1){
			fstr[i/8]=a;
			a=0;
		}
	}

	fopen_s(&hFile, "save/animebook.dat", "wb");
	fwrite(fstr, sizeof(fstr[0]), fsize/sizeof(fstr[0]), hFile);
	fclose(hFile);
}

void load_works(int n){
	load_station();
	load_animebook();
	if(works){
		delete [] prg;
		delete [] work;
	}
	char fn[50];
	size_t fc=0;
	prgs=0;
	works=0;
	sprintf_s(fn,"file/data/work/work%d.dat",n);
	loadFile(fn);

	while(fc<fsize){
		fc+=2;
		while(fstr[fc]!=EOF || fstr[fc+1]!=EOF){
			prgs++;
			fc+=6;
		}
		works++;
		fc+=2;
	}
	prg=new Prg[prgs];
	work=new Work[works];
	prgs=0;
	fc=0;

	for(int i=0 ; i<works ; i++){
		work[i].num=0;
		work[i].notExist=false;
		work[i].prg=prgs;
		work[i].tnum=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		while(fstr[fc]!=EOF || fstr[fc+1]!=EOF){
			prg[prgs].work=i;
			prg[prgs].station=to16int(fstr[fc],fstr[fc+1]);
			fc+=2;
			prg[prgs].week=fstr[fc];
			fc++;
			prg[prgs].hour=fstr[fc];
			fc++;
			prg[prgs].minute=fstr[fc];
			fc++;
			prg[prgs].time=to8int(fstr[fc]);
			fc++;
			work[i].num++;
			prgs++;
		}
		fc+=2;
	}

	String *title;
	int *mark;
	SDL_Color *col;
	fc=0;
	loadFile("file/data/work/works.dat");
	title=new String[allworks];
	mark=new int[allworks];
	col=new SDL_Color[allworks];
	fc=0;
	for(int i=0 ; i<allworks ; i++){
		mark[i]=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		col[i].r=to8int(fstr[fc]);fc++;
		col[i].g=to8int(fstr[fc]);fc++;
		col[i].b=to8int(fstr[fc]);fc++;
	}

	fc=0;
	loadFile("file/data/work/work_name_jp.dat");
	for(int i=0 ; i<allworks ; i++){
		for(int j=0 ; j<200 ; j++){
			title[i].str[0][j]=fstr[fc];
			fc++;
			if(fstr[fc-1]==0)break;
		}
		if(fstr[fc]==EOF)break;
	}

	for(int i=0 ; i<works ; i++){
		work[i].mark=mark[work[i].tnum];
		work[i].r=col[work[i].tnum].r;
		work[i].g=col[work[i].tnum].g;
		work[i].b=col[work[i].tnum].b;
		for(int k=0 ; k<2 ; k++)for(int j=0 ; j<200 ; j++){
			work[i].title.str[k][j]=0;
		}
		sprintf_s(work[i].title.str[0],"%s",title[work[i].tnum].str[0]);
		sprintf_s(work[i].title.str[1],"%s",title[work[i].tnum].str[1]);
	}

	for(int i=0 ; i<works ; i++)putHeadMark(work[i].title);

	delete [] title;
	delete [] mark;
	delete [] col;

	if(gd.timeslot!=NULL)delete [] gd.timeslot;
	sprintf_s(str,"file/data/work/timeslot%d.dat",n);
	loadFile(str);
	gd.timeslot = new Uint8[fsize+1];
	for(size_t i=0 ; i<fsize ; i++){
		gd.timeslot[i]=fstr[i];
	}
	gd.timeslot[fsize]=EOF;
	load_searchQueries();
}

void load_searchQueries(){
	loadFile("file/data/work/search_query.dat");
	int num = 0;
	int j = 0;
	String *query;
	query=new String[allworks];

	for(size_t i=0 ; i<fsize ; i++){
		if(j<200){
			query[num].str[0][j] = fstr[i];
		}
		j++;

		if(fstr[i]==0){
			j = 0;
			num++;
		}
	}

	int value, w;
	for(int i=0 ; i<works ; i++){
		j = 0;
		w = work[i].tnum;
		for(int k=0 ; k<200 ; k++){
			if(query[w].str[0][k] == 0){
				work[i].query[j] = 0;
				break;
			}
			else if(query[w].str[0][k] == 32){
				work[i].query[j] = '+';j++;
			}
			else if(query[w].str[0][k]<=-33 && query[w].str[0][k]>=-62){
				work[i].query[j] = '%';j++;
				value = query[i].str[0][k];
				if(value < 0)value += 256;
				work[i].query[j] = hex[value/16];j++;
				work[i].query[j] = hex[value%16];j++;
				k++;
				work[i].query[j] = '%';j++;
				value = query[i].str[0][k];
				if(value < 0)value += 256;
				work[i].query[j] = hex[value/16];j++;
				work[i].query[j] = hex[value%16];j++;
			}
			else if(query[w].str[0][k]<=-17 && query[w].str[0][k]>=-32){
				work[i].query[j] = '%';j++;
				value = query[w].str[0][k];
				if(value < 0)value += 256;
				work[i].query[j] = hex[value/16];j++;
				work[i].query[j] = hex[value%16];j++;
				k++;
				work[i].query[j] = '%';j++;
				value = query[w].str[0][k];
				if(value < 0)value += 256;
				work[i].query[j] = hex[value/16];j++;
				work[i].query[j] = hex[value%16];j++;
				k++;
				work[i].query[j] = '%';j++;
				value = query[w].str[0][k];
				if(value < 0)value += 256;
				work[i].query[j] = hex[value/16];j++;
				work[i].query[j] = hex[value%16];j++;
			}
			else{
				work[i].query[j] = query[w].str[0][k];j++;
			}
		}
	}

	delete query;
}

void load_towers(){
	if(towers)return;
	size_t fc=0;
	areas=0;
	towers=0;

	char fn[50];
	while(true){
		sprintf_s(fn,"file/data/tower/tower%d.dat",areas);
		if(!loadFile(fn))break;
		areas++;
	}

	for(int i=0 ; i<areas ; i++){
		fc=0;
		sprintf_s(fn,"file/data/tower/tower%d.dat",i);
		loadFile(fn);
		int st_num=fstr[fc];
		fc+=st_num*3+1;
		while(fc<fsize){
			fc+=st_num+9;
			towers++;
		}
	}
	area=new Area[areas+1];
	tower=new Tower[towers+1];
	Tower *tw=tower;
	towers=0;

	for(int i=0 ; i<areas ; i++){
		fc=0;
		sprintf_s(fn,"file/data/tower/tower%d.dat",i);
		loadFile(fn);
		area[i].num=0;
		area[i].tower=towers;
		area[i].st_num=fstr[fc];fc++;
		for(int j=0 ; j<area[i].st_num ; j++){
			area[i].station[j]=to16int(fstr[fc],fstr[fc+1]);
			fc+=2;
			area[i].button[j]=fstr[fc];fc++;
		}
		while(fc<fsize){
			tw->area=i;
			tw->x=to16int(fstr[fc],fstr[fc+1]);
			fc+=2;
			tw->y=to16int(fstr[fc],fstr[fc+1]);
			fc+=2;
			tw->h=to8int(fstr[fc]);fc++;
			tw->kw=fstr[fc];fc++;
			tw->kw2=fstr[fc];fc++;
			tw->erp=fstr[fc];fc++;
			tw->erp2=fstr[fc];fc++;
			tw->v=fstr[fc];fc++;
			tw->bias=to8int(fstr[fc]);fc++;
			for(int j=0 ; j<area[i].st_num ; j++){
				tw->ch[j]=fstr[fc];fc++;
			}
			area[i].num++;
			tw++;towers++;
		}
	}

	for(int j=0 ; j<2 ; j++){
		fc=0;
		if(j==0)loadFile("file/data/tower/area_name_jp.dat");
		else loadFile("file/data/tower/area_name_en.dat");
		for (int i=0 ; i<areas ; i++){
			for(int k=0 ; k<60 ; k++){
				area[i].name.str[j][k]=fstr[fc];
				fc++;
				if(fstr[fc-1]==0)break;
			}
		}
	}

	int n=0;
	for(int j=0 ; j<2 ; j++){
		n=0;
		for(int i=0 ; i<areas ; i++){
			fc=0;
			if(j==0)sprintf_s(fn,"file/data/tower/tower%d_name_jp.dat",i);
			else sprintf_s(fn,"file/data/tower/tower%d_name_en.dat",i);
			loadFile(fn);
			while(fc<fsize && n<towers){
				for(int k=0 ; k<60 ; k++){
					tower[n].name.str[j][k]=fstr[fc];
					fc++;
					if(fstr[fc-1]==0)break;
				}
				n++;
			}
		}
	}
	extra_tower();
}

void extra_tower(){
	area[areas].button[0]=1;
	area[areas].name.str[0][0]=0;
	area[areas].name.str[1][0]=0;
	area[areas].num=1;
	area[areas].st_num=1;
	area[areas].station[0]=stas-1;
	area[areas].tower=towers;
	tower[towers].area=areas;
	tower[towers].bias=255;
	tower[towers].ch[0]=CHANNELS;
	tower[towers].h=1;
	tower[towers].kw=5;
	tower[towers].kw2=5;
	for(int k=0 ; k<2 ; k++){
		sprintf_s(tower[towers].name.str[k],text[ANTENNATEXT+16].str[k]);
	}
	tower[towers].x=1121;
	tower[towers].y=2585;
	areas++;towers++;
}

void load_mounts(){
	if(mounts)return;
	size_t fc=0;
	mounts=0;

	loadFile("file/data/mount.dat");

	mounts=(int)(fsize/8);
	mount=new Mount[mounts];
	fc=0;

	Mount *mt=mount;
	for(int i=0 ; i<mounts ; i++){
		mt->x=to16int(fstr[fc],fstr[fc+1]);
		fc+=2;
		mt->y=to16int(fstr[fc],fstr[fc+1]);
		fc+=2;
		mt->h=to16int(fstr[fc],fstr[fc+1]);
		fc+=2;
		mt->slope=fstr[fc];
		mt->slope=mt->h*mt->slope/100;
		fc++;
		mt->range=to8int(fstr[fc])%64;
		mt->city=0;
		mt->volcano=false;
		if(to8int(fstr[fc])>=192){
			mt->volcano=true;
		}
		else if(to8int(fstr[fc])>=128){
			mt->slope=mt->h;
			mt->city=fstr[fc-1];
		}
		mt++;fc++;
	}

}

void load_towns(){
	if(towns)return;
	load_towers();
	size_t fc=0;
	towns=0;

	loadFile("file/data/town.dat");

	towns=(int)((fsize-(areas-1)*2)/4);
	town=new Town[towns];
	fc=0;

	int a=0;
	for(int i=0 ; i<areas-1 ; i++){
		area[i].town=a;
		area[i].town_num=to16int(fstr[fc],fstr[fc+1]);
		a+=area[i].town_num;
		fc+=2;
	}

	for(int i=0 ; i<towns ; i++){
		town[i].x=to16int(fstr[fc],fstr[fc+1]);
		fc+=2;
		town[i].y=to16int(fstr[fc],fstr[fc+1]);
		fc+=2;
	}

	int n;
	for(int j=0 ; j<2 ; j++){
		n=0;
		fc=0;
		if(j==0)loadFile("file/data/town_name_jp.dat");
		else loadFile("file/data/town_name_en.dat");
		while(fc<fsize && n<towns){
			for(int k=0 ; k<60 ; k++){
				town[n].name.str[j][k]=fstr[fc];
				fc++;
				if(fstr[fc-1]==0)break;
			}
			n++;
		}
	}
}

void load_rural(){
	if(map.rural_loaded)return;
	load_towers();
	size_t fc=0;
	int a=0,b=0;

	loadFile("file/data/rural.dat");
	map.rural_size=fstr[fc];fc++;
	map.ruralW=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	map.ruralH=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	map.set2();
	for(int j=0 ; j<map.ruralH ; j++)for(int i=0 ; i<map.ruralW ; i++){
		if(fc>=fsize)break;
		map.rural[i][j]=fstr[fc];fc++;
	}
}

void save_option(){
	char fstr[100];
	FILE* hFile;
	int count=0;

	fstr[count]=SHOW_TOWER;count++;
	fstr[count]=AIR_IMG;count++;
	fstr[count]=DASH_TYPE;count++;
	fstr[count]=CHAR_CODE;count++;
	fstr[count]=MAGNIFY;count++;
	fstr[count]=ADJ_DIR;count++;
	fstr[count]=EXPLAIN;count++;
	fstr[count]=BGM_VOLUME;count++;
	fstr[count]=SE_VOLUME;count++;
	fstr[count]=NHK_REMOVE;count++;
	fstr[count]=WALKING_TYPE;count++;
	fstr[count]=ROD_TYPE;count++;
	fstr[count]=key.aC%256;count++;
	fstr[count]=key.aC/256;count++;
	fstr[count]=key.zC%256;count++;
	fstr[count]=key.zC/256;count++;
	fstr[count]=key.xC%256;count++;
	fstr[count]=key.xC/256;count++;
	fstr[count]=key.cC%256;count++;
	fstr[count]=key.cC/256;count++;
	fstr[count]=key.upC%256;count++;
	fstr[count]=key.upC/256;count++;
	fstr[count]=key.downC%256;count++;
	fstr[count]=key.downC/256;count++;
	fstr[count]=key.leftC%256;count++;
	fstr[count]=key.leftC/256;count++;
	fstr[count]=key.rightC%256;count++;
	fstr[count]=key.rightC/256;count++;

	fopen_s(&hFile, "save/option.dat", "wb");
	fwrite(fstr, sizeof(fstr[0]), count/sizeof(fstr[0]), hFile);
	fclose(hFile);
}

void load_parameter(){
	int fc=0;
	double SDW400H=0,SDW62CH=0,GROUND1CH=0,GROUND62CH=0,DIF1CH=0,DIF62CH=0,ORI1CH=0,ORI62CH=0;

//	if(loadFile("file/data/parameter.dat")){
		loadFile("file/data/parameter.dat");
		map.mapW=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		map.mapH=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		gd.startX=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		gd.startY=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		gd.summerX_start=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		gd.summerY_start=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		gd.summerX=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		gd.summerY=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		gd.memmaX=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		gd.memmaY=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		DIS1CH=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		for(int i=0 ; i<fstr[fc] ; i++)DIS1CH/=10.0;
		fc++;
		DIS62CH=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		for(int i=0 ; i<fstr[fc] ; i++)DIS62CH/=10.0;
		fc++;
		CURVE_TOP=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		CURVE_RISE=to8int(fstr[fc]);fc++;
		for(int i=0 ; i<fstr[fc] ; i++)CURVE_RISE/=10.0;
		fc++;
		DIF1CH=to8int(fstr[fc]);fc++;
		for(int i=0 ; i<fstr[fc] ; i++)DIF1CH/=10.0;
		fc++;
		DIF62CH=to8int(fstr[fc]);fc++;
		for(int i=0 ; i<fstr[fc] ; i++)DIF62CH/=10.0;
		fc++;
		SDW400H=to8int(fstr[fc]);fc++;
		for(int i=0 ; i<fstr[fc] ; i++)SDW400H/=10.0;
		fc++;
		SDW62CH=to8int(fstr[fc]);fc++;
		for(int i=0 ; i<fstr[fc] ; i++)SDW62CH/=10.0;
		fc++;
		GROUND1CH=to8int(fstr[fc]);fc++;
		for(int i=0 ; i<fstr[fc] ; i++)GROUND1CH/=10.0;
		fc++;
		GROUND62CH=to8int(fstr[fc]);fc++;
		for(int i=0 ; i<fstr[fc] ; i++)GROUND62CH/=10.0;
		fc++;
		ORI1CH=to8int(fstr[fc]);fc++;
		ORI62CH=to8int(fstr[fc]);fc++;
		RCV_LEVEL=to8int(fstr[fc]);fc++;
		for(int i=0 ; i<fstr[fc] ; i++)RCV_LEVEL/=10.0;
		fc++;
		CHANNELS=to8int(fstr[fc]);fc++;
		MAP_SCALE=to8int(fstr[fc]);fc++;
		for(int i=0 ; i<fstr[fc] ; i++)MAP_SCALE/=10.0;
		fc++;
		mhz=new Mhz[CHANNELS];
		for(int i=0 ; i<CHANNELS ; i++){
			mhz[i].mhz=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		}
/*	}else{
		map.mapW=1920;map.mapH=2600;
		gd.startX=1100;gd.startY=1046;
		DIS1CH=78.7;
		DIS62CH=22.1;
		CURVE_TOP=214;
		CURVE_RISE=4.7;
		DIF1CH=0.7;
		DIF62CH=0.3;
		SDW400H=3.1;
		SDW62CH=0.1;
		GROUND1CH=1.3;
		GROUND62CH=1.0;
		ORI1CH=16.0;
		ORI62CH=20.0;
		RCV_LEVEL=20.0;
		CHANNELS=62;
		MAP_SCALE=1.3;
		mhz=new Mhz[CHANNELS];
		for(int i=1 ; i<=CHANNELS ; i++){
			int a=87+i*6;
			if(i>=4)a+=62;
			if(i>=8)a-=2;
			if(i>=13)a+=248;
			if(i>=63)a+=11321;
			mhz[i-1].mhz=a;
		}
		fstr=new char[33+CHANNELS*2];
		fstr[0]=0;fstr[1]=0;
		fstr[2]=map.mapW%256;fstr[3]=map.mapW/256;
		fstr[4]=map.mapH%256;fstr[5]=map.mapH/256;
		fstr[6]=gd.startX%256;fstr[7]=gd.startX/256;
		fstr[8]=gd.startY%256;fstr[9]=gd.startY/256;
		fstr[10]=-7;fstr[11]=0;fstr[12]=0;
		fstr[13]=70;fstr[14]=0;
		fstr[15]=-42;fstr[16]=0;
		fstr[17]=15;
		fstr[18]=7;fstr[19]=1;
		fstr[20]=3;fstr[21]=1;
		fstr[22]=31;fstr[23]=0;
		fstr[24]=1;fstr[25]=0;
		fstr[26]=13;fstr[27]=1;
		fstr[28]=1;fstr[29]=0;
		fstr[30]=18;
		fstr[31]=22;
		fstr[32]=20;fstr[33]=0;
		fstr[34]=62;
		fstr[35]=13;fstr[36]=1;
		fc=37;
		for(int i=0 ; i<CHANNELS ; i++){
			fstr[fc]=mhz[i].mhz%256;fc++;
			fstr[fc]=mhz[i].mhz/256;fc++;
		}
		FILE* hFile;
		fopen_s(&hFile,"file/data/parameter.dat","wb");
		fwrite(fstr, sizeof(fstr[0]), fc/sizeof(fstr[0]), hFile);
		fclose(hFile);
		delete [] fstr;
		fsize=0;
	}
*/

//	DIS1CH=249.0;DIS62CH=70.0;
//	CURVE_TOP=214;CURVE_RISE=15;
//	SDW400H=31.0;SDW62CH=1.0;
//	DIF1CH=0.7;DIF62CH=0.3;
//	CHANNELS=62;MAP_SCALE=1.3;
//	ORI1CH=18;ORI62CH=22;
	double SDWFUNC_A=(SDW62CH*SDW62CH-SDW400H*SDW400H)/674.0;
	double SDWFUNC_B=SDW400H*SDW400H-SDWFUNC_A*93;
	double DIFFUNC_A=(DIF62CH*DIF62CH-DIF1CH*DIF1CH)/674.0;
	double DIFFUNC_B=DIF1CH*DIF1CH-DIFFUNC_A*93;
	double ORIFUNC_A=(ORI62CH*ORI62CH-ORI1CH*ORI1CH)/674.0;
	double ORIFUNC_B=ORI1CH*ORI1CH-ORIFUNC_A*93;
	double GROFUNC_A=(GROUND62CH*GROUND62CH-GROUND1CH*GROUND1CH)/674.0;
	double GROFUNC_B=GROUND1CH*GROUND1CH-GROFUNC_A*93;

	//カーブを与えるためにsqrtを使う
	for(int i=0 ; i<CHANNELS ; i++){
		mhz[i].shadow=sqrt(mhz[i].mhz*SDWFUNC_A+SDWFUNC_B);
		mhz[i].dif=sqrt(mhz[i].mhz*DIFFUNC_A+DIFFUNC_B);
		mhz[i].ori=sqrt(mhz[i].mhz*ORIFUNC_A+ORIFUNC_B);
		mhz[i].ground=sqrt(mhz[i].mhz*GROFUNC_A+GROFUNC_B);
	}
}

void load_option(){
	int fc=0;
	if(!loadFile("save/option.dat")){
		SHOW_TOWER=true;
		AIR_IMG=TYPE1_EDGE;
		DASH_TYPE=TYPE_MARIO;
		CHAR_CODE=JAPANESE;
		MAGNIFY=1;
		ADJ_DIR=AUTO;
		EXPLAIN=true;
		BGM_VOLUME=64;
		SE_VOLUME=64;
		NHK_REMOVE=false;
		WALKING_TYPE=SMOOTH;
		ROD_TYPE=CONVENIENTROD;
		reset_key();
	}else{
		SHOW_TOWER=fstr[fc];fc++;
		AIR_IMG=fstr[fc];fc++;
		DASH_TYPE=fstr[fc];fc++;
		CHAR_CODE=fstr[fc];fc++;
		MAGNIFY=fstr[fc];fc++;
		ADJ_DIR=fstr[fc];fc++;
		EXPLAIN=fstr[fc];fc++;
		BGM_VOLUME=fstr[fc];fc++;
		SE_VOLUME=fstr[fc];fc++;
		NHK_REMOVE=fstr[fc];fc++;
		WALKING_TYPE=fstr[fc];fc++;
		ROD_TYPE=fstr[fc];fc++;
		key.aC=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		key.zC=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		key.xC=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		key.cC=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		key.upC=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		key.downC=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		key.leftC=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		key.rightC=to16int(fstr[fc],fstr[fc+1]);
	}
}

void save_index(){
	if(fsize)delete [] fstr;
	fsize=index_num*5;
	fstr=new char[fsize+18];
	FILE* hFile;
	size_t fc=0;

	for(int i=0 ; i<18 ; i++){
		fstr[fc]=gd.bought[i];fc++;
	}
	for(int i=0 ; i<index_num ; i++){
		fstr[fc]=indexName[i].rate;fc++;
		fstr[fc]=indexName[i].hiscore%256;fc++;
		fstr[fc]=(indexName[i].hiscore/256)%256;fc++;
		fstr[fc]=(indexName[i].hiscore/65536)%256;fc++;
		fstr[fc]=indexName[i].hiscore/16777216;fc++;
	}

	sprintf_s(str,"save/clear.dat");
	fopen_s(&hFile, str, "wb");
	fwrite(fstr, sizeof(fstr[0]), fc/sizeof(fstr[0]), hFile);
	fclose(hFile);
}

void save_record(int n){
	if(fsize)delete [] fstr;
	fsize=works*18+1;
	fstr=new char[fsize];
	FILE* hFile;
	size_t fc=0;

	fstr[fc]=dataNo-1;fc++;
	for(int i=0 ; i<works ; i++){
		fstr[fc]=fishbox.getData(i,0)%256;fc++;
		fstr[fc]=fishbox.getData(i,0)/256;fc++;
		fstr[fc]=fishbox.getData(i,1)%256;fc++;
		fstr[fc]=fishbox.getData(i,1)/256;fc++;
		fstr[fc]=fishbox.getData(i,2)%256;fc++;
		fstr[fc]=fishbox.getData(i,2)/256;fc++;
		fstr[fc]=fishbox.getData(i,3)%256;fc++;
		fstr[fc]=fishbox.getData(i,3)/256;fc++;
		fstr[fc]=fishbox.getData(i,4)%256;fc++;
		fstr[fc]=fishbox.getData(i,4)/256;fc++;
		fstr[fc]=fishbox.getData(i,5);fc++;
		fstr[fc]=fishbox.getData(i,6);fc++;
		fstr[fc]=fishbox.getData(i,7);fc++;
		fstr[fc]=fishbox.getData(i,8);fc++;
		fstr[fc]=fishbox.getData(i,9);fc++;
		fstr[fc]=fishbox.getData(i,10);fc++;
		fstr[fc]=fishbox.getData(i,11)%256;fc++;
		fstr[fc]=fishbox.getData(i,11)/256;fc++;
	}

	sprintf_s(str,"save/record%d.dat",n);
	fopen_s(&hFile, str, "wb");
	fwrite(fstr, sizeof(fstr[0]), fc/sizeof(fstr[0]), hFile);
	fclose(hFile);
}

void load_record(int n){
	int a=0;
	size_t fc=0;
	sprintf_s(str,"save/record%d.dat",n);
	loadFile(str);
	dataNo=fstr[fc]+1;fc++;
	load_works(dataNo);
	fishbox.initFishBox(works);
	sprintf_s(str,"save/record%d.dat",n);
	loadFile(str);
	gd.score=0;
	fc=1;
	for(int i=0 ; i<works ; i++){
		if(fc>=fsize)break;
		fishbox.setData(i,0,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,1,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,2,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,3,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,4,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,5,fstr[fc]);fc++;
		fishbox.setData(i,6,fstr[fc]);fc++;
		fishbox.setData(i,7,fstr[fc]);fc++;
		fishbox.setData(i,8,fstr[fc]);fc++;
		fishbox.setData(i,9,fstr[fc]);fc++;
		fishbox.setData(i,10,fstr[fc]);fc++;
		fishbox.setData(i,11,to16int(fstr[fc],fstr[fc+1]));
		gd.score+=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	}
	load_towers();
	load_station();
	createMap();
}

void save_game(int n){
	if(fsize)delete [] fstr;
	fsize=works*39+19;
	fstr=new char[fsize];
	FILE* hFile;
	size_t fc=0;
	int today_num=0;

	fstr[fc]=dataNo-1;fc++;
	fstr[fc]=works%256;fc++;
	fstr[fc]=works/256;fc++;
	fstr[fc]=gd.week;fc++;
	fstr[fc]=gd.hour;fc++;
	fstr[fc]=gd.minute;fc++;
	for(int i=0 ; i<works ; i++){
		if(fishbox.today[i]==EOF)break;
		today_num++;
	}
	fstr[fc]=today_num%256;fc++;
	fstr[fc]=today_num/256;fc++;
	fstr[fc]=md.maneki_mode;fc++;
	fstr[fc]=md.fish_num%256;fc++;
	fstr[fc]=md.fish_num/256;fc++;
	fstr[fc]=(int)gd.x%256;fc++;
	fstr[fc]=(int)gd.x/256;fc++;
	fstr[fc]=(int)gd.y%256;fc++;
	fstr[fc]=(int)gd.y/256;fc++;
	fstr[fc]=(int)md.manekiX%256;fc++;
	fstr[fc]=(int)md.manekiX/256;fc++;
	fstr[fc]=(int)md.manekiY%256;fc++;
	fstr[fc]=(int)md.manekiY/256;fc++;
	for(int i=0 ; i<works ; i++){
		fstr[fc]=fishbox.getData(i,0)%256;fc++;
		fstr[fc]=fishbox.getData(i,0)/256;fc++;
		fstr[fc]=fishbox.getData(i,1)%256;fc++;
		fstr[fc]=fishbox.getData(i,1)/256;fc++;
		fstr[fc]=fishbox.getData(i,2)%256;fc++;
		fstr[fc]=fishbox.getData(i,2)/256;fc++;
		fstr[fc]=fishbox.getData(i,3);fc++;
		fstr[fc]=fishbox.getData(i,4)%256;fc++;
		fstr[fc]=fishbox.getData(i,4)/256;fc++;
		fstr[fc]=fishbox.getData(i,5);fc++;
		fstr[fc]=fishbox.getData(i,6);fc++;
		fstr[fc]=fishbox.getData(i,7);fc++;
		fstr[fc]=fishbox.getData(i,8);fc++;
		fstr[fc]=fishbox.getData(i,9);fc++;
		fstr[fc]=fishbox.getData(i,10);fc++;
		fstr[fc]=fishbox.getData(i,11)%256;fc++;
		fstr[fc]=fishbox.getData(i,11)/256;fc++;
	}
	for(int i=0 ; i<today_num ; i++){
		fstr[fc]=fishbox.today[i]%256;fc++;
		fstr[fc]=fishbox.today[i]/256;fc++;
	}
	for(int i=0 ; i<md.fish_num ; i++){
		fstr[fc]=md.maneki_count[i];fc++;
		fstr[fc]=md.fish[i].title_num%256;fc++;
		fstr[fc]=md.fish[i].title_num/256;fc++;
		fstr[fc]=md.fish[i].x%256;fc++;
		fstr[fc]=md.fish[i].x/256;fc++;
		fstr[fc]=md.fish[i].y%256;fc++;
		fstr[fc]=md.fish[i].y/256;fc++;
		fstr[fc]=md.fish[i].sta%256;fc++;
		fstr[fc]=md.fish[i].sta/256;fc++;
		fstr[fc]=md.fish[i].bs;fc++;
		fstr[fc]=md.fish[i].tower%256;fc++;
		fstr[fc]=md.fish[i].tower/256;fc++;
		fstr[fc]=md.fish[i].ch;fc++;
		fstr[fc]=md.fish[i].hour;fc++;
		fstr[fc]=md.fish[i].minute;fc++;
		fstr[fc]=md.fish[i].week;fc++;
		fstr[fc]=md.fish[i].rcv;fc++;
		fstr[fc]=md.fish[i].mg_rcv;fc++;
		fstr[fc]=md.fish[i].score%256;fc++;
		fstr[fc]=md.fish[i].score/256;fc++;
	}
	fsize=fc;

	sprintf_s(str,"save/save%d.dat",n);
	fopen_s(&hFile, str, "wb");
	fwrite(fstr, sizeof(fstr[0]), fc/sizeof(fstr[0]), hFile);
	fclose(hFile);
	save_animebook();
}

void load_game(int n){
	size_t fc=0;
	int a=0, today_num=0;

	sprintf_s(str,"save/save%d.dat",n);
	if(!loadFile(str))return;
	dataNo=fstr[fc]+1;fc++;
	a=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	gd.week=fstr[fc];fc++;
	gd.hour=fstr[fc];fc++;
	gd.minute=fstr[fc];fc++;
	today_num=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	md.maneki_mode=fstr[fc];fc++;
	md.fish_num=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	gd.x=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	gd.y=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	md.manekiX=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	md.manekiY=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	fishbox.initFishBox(a);
	for(int i=0 ; i<a ; i++){
		fishbox.setData(i,0,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,1,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,2,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,3,fstr[fc]);fc++;
		fishbox.setData(i,4,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,5,fstr[fc]);fc++;
		fishbox.setData(i,6,fstr[fc]);fc++;
		fishbox.setData(i,7,fstr[fc]);fc++;
		fishbox.setData(i,8,fstr[fc]);fc++;
		fishbox.setData(i,9,fstr[fc]);fc++;
		fishbox.setData(i,10,fstr[fc]);fc++;
		fishbox.setData(i,11,to16int(fstr[fc],fstr[fc+1]));fc+=2;
	}
	for(int i=0 ; i<today_num ; i++){
		fishbox.today[i]=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	}
	for(int i=0 ; i<md.fish_num ; i++){
		md.maneki_count[i]=fstr[fc];fc++;
		md.fish[i].title_num=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		md.fish[i].x=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		md.fish[i].y=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		md.fish[i].sta=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		md.fish[i].bs=fstr[fc];fc++;
		md.fish[i].tower=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		md.fish[i].ch=fstr[fc];fc++;
		md.fish[i].hour=fstr[fc];fc++;
		md.fish[i].minute=fstr[fc];fc++;
		md.fish[i].week=fstr[fc];fc++;
		md.fish[i].rcv=fstr[fc];fc++;
		md.fish[i].mg_rcv=fstr[fc];fc++;
		md.fish[i].score=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	}
	fix_scrXY();
}

void load_smr(int n){
	sprintf_s(str,"file/smr/smr%d.dat",n);
	if(!loadFile(str))return;
	int fc=0,a=0,b=0;
	for(int j=0 ; j<map.mapH ; j++){
		for(int i=0 ; i<map.mapW ; i++){
			if(b==0){
				a=to8int(fstr[fc])%8;
				b=to8int(fstr[fc])/8+1;
				if(b==30){b=to8int(fstr[fc+1]);fc+=2;}
				else if(b==31){b=to16int(fstr[fc+1],fstr[fc+2]);fc+=3;}
				else if(b==32){b=to32int(fstr[fc+1],fstr[fc+2],fstr[fc+3],0);fc+=4;}
				else fc++;
			}
			map.smr[i][j]=a;
			b--;
		}
	}
}

void save_smr(int n){
	if(!saveSMR)return;
	if(fsize)delete [] fstr;
	fsize=map.mapW*map.mapH;
	fstr=new char[map.mapW*map.mapH];
	FILE* hFile;
	int fc=0, a=0;

	for(int i=0 ; i<map.mapW*map.mapH ; i++){
		if(i!=0)if(map.smr[(i-1)%map.mapW][(i-1)/map.mapW]!=map.smr[i%map.mapW][i/map.mapW] || i==map.mapW*map.mapH-1 || a==16777215){
			fstr[fc]=map.smr[(i-1)%map.mapW][(i-1)/map.mapW];fc++;
			if(a<=29){
				fstr[fc-1]+=(a-1)*8;
			}
			if(a>=30){
				fstr[fc-1]+=232;
				fstr[fc]+=a%256;fc++;
			}
			if(a>=256){
				fstr[fc-2]+=8;
				fstr[fc]=(a/256)%65536;fc++;
			}
			if(a>=65536){
				fstr[fc-3]+=8;
				fstr[fc]=(a/65536)%16777216;fc++;
			}
			a=0;
		}
		a++;
	}

	fstr[fc]=EOF;fc++;
	fsize=fc;

	sprintf_s(str,"file/smr/smr%d.dat",n);
	fopen_s(&hFile, str, "wb");
	fwrite(fstr, sizeof(fstr[0]), fc/sizeof(fstr[0]), hFile);
	fclose(hFile);
}
