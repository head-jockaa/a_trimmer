#include "file_io.h"

char hex[16]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
char *E28XXX,*E38XXX,*E48XXX,*E58XXX,*E68XXX,*E78XXX,*E88XXX,*E98XXX,*EE8XXX,*EF8XXX,*CXXX;

void loadEncodingFiles(){
	loadFile("file/data/utf8_to_sjis/E28XXX-E2BXXX.txt");
	E28XXX=new char[fsize];
	strcpy_s(E28XXX,fstr);
	loadFile("file/data/utf8_to_sjis/E38XXX-E3BXXX.txt");
	E38XXX=new char[fsize];
	strcpy_s(E38XXX,fstr);
	loadFile("file/data/utf8_to_sjis/E48XXX-E4BXXX.txt");
	E48XXX=new char[fsize];
	strcpy_s(E48XXX,fstr);
	loadFile("file/data/utf8_to_sjis/E58XXX-E5BXXX.txt");
	E58XXX=new char[fsize];
	strcpy_s(E58XXX,fstr);
	loadFile("file/data/utf8_to_sjis/E68XXX-E6BXXX.txt");
	E68XXX=new char[fsize];
	strcpy_s(E68XXX,fstr);
	loadFile("file/data/utf8_to_sjis/E78XXX-E7BXXX.txt");
	E78XXX=new char[fsize];
	strcpy_s(E78XXX,fstr);
	loadFile("file/data/utf8_to_sjis/E88XXX-E8BXXX.txt");
	E88XXX=new char[fsize];
	strcpy_s(E88XXX,fstr);
	loadFile("file/data/utf8_to_sjis/E98XXX-E9BXXX.txt");
	E98XXX=new char[fsize];
	strcpy_s(E98XXX,fstr);
	loadFile("file/data/utf8_to_sjis/EE8XXX-EEBXXX.txt");
	EE8XXX=new char[fsize];
	strcpy_s(EE8XXX,fstr);
	loadFile("file/data/utf8_to_sjis/EF8XXX-EFBXXX.txt");
	EF8XXX=new char[fsize];
	strcpy_s(EF8XXX,fstr);
	loadFile("file/data/utf8_to_sjis/CXXX-DXXX.txt");
	CXXX=new char[fsize];
	strcpy_s(CXXX,fstr);
}

void freeEncodingFiles(){
	if(E28XXX){
		delete [] E28XXX;
		E28XXX=NULL;
	}
	if(E38XXX){
		delete [] E38XXX;
		E38XXX=NULL;
	}
	if(E48XXX){
		delete [] E48XXX;
		E48XXX=NULL;
	}
	if(E58XXX){
		delete [] E58XXX;
		E58XXX=NULL;
	}
	if(E68XXX){
		delete [] E68XXX;
		E68XXX=NULL;
	}
	if(E78XXX){
		delete [] E78XXX;
		E78XXX=NULL;
	}
	if(E88XXX){
		delete [] E88XXX;
		E88XXX=NULL;
	}
	if(E98XXX){
		delete [] E98XXX;
		E98XXX=NULL;
	}
	if(EE8XXX){
		delete [] EE8XXX;
		EE8XXX=NULL;
	}
	if(EF8XXX){
		delete [] EF8XXX;
		EF8XXX=NULL;
	}
	if(CXXX){
		delete [] CXXX;
		CXXX=NULL;
	}
}

void UTF8toLatin(char *s){
	int pointer=0;
	for(int i=0 ; i<200 ; i++){
		if(pointer>=200 || s[pointer]==0){
			s[i]=0;
			break;
		}
		if(s[pointer]==-61){
			pointer++;
			s[i]=s[pointer]+64;
		}else{
			s[i]=s[pointer];
		}
		pointer++;
	}
}

void UTF8toSJIS(char *s){
	int pointer=0;
	for(int i=0 ; i<200 ; i++){
		if(pointer>=200 || s[pointer]==0){
			s[i]=0;
			break;
		}
		if(i<199 && pointer<199 && s[pointer]>=-64 && s[pointer]<=-33){
			int n=(64+s[pointer])*64+(128+s[pointer+1]);
			s[i]=CXXX[n*2];
			s[i+1]=CXXX[n*2+1];
			i++;
			pointer+=2;
		}
		else if(i<199 && pointer<198 && s[pointer]==-30){
			int n=(128+s[pointer+1])*64+(128+s[pointer+2]);
			s[i]=E28XXX[n*2];
			s[i+1]=E28XXX[n*2+1];
			i++;
			pointer+=3;
		}
		else if(i<199 && pointer<198 && s[pointer]==-29){
			int n=(128+s[pointer+1])*64+(128+s[pointer+2]);
			s[i]=E38XXX[n*2];
			s[i+1]=E38XXX[n*2+1];
			i++;
			pointer+=3;
		}
		else if(i<199 && pointer<198 && s[pointer]==-28){
			int n=(128+s[pointer+1])*64+(128+s[pointer+2]);
			s[i]=E48XXX[n*2];
			s[i+1]=E48XXX[n*2+1];
			i++;
			pointer+=3;
		}
		else if(i<199 && pointer<198 && s[pointer]==-27){
			int n=(128+s[pointer+1])*64+(128+s[pointer+2]);
			s[i]=E58XXX[n*2];
			s[i+1]=E58XXX[n*2+1];
			i++;
			pointer+=3;
		}
		else if(i<199 && pointer<198 && s[pointer]==-26){
			int n=(128+s[pointer+1])*64+(128+s[pointer+2]);
			s[i]=E68XXX[n*2];
			s[i+1]=E68XXX[n*2+1];
			i++;
			pointer+=3;
		}
		else if(i<199 && pointer<198 && s[pointer]==-25){
			int n=(128+s[pointer+1])*64+(128+s[pointer+2]);
			s[i]=E78XXX[n*2];
			s[i+1]=E78XXX[n*2+1];
			i++;
			pointer+=3;
		}
		else if(i<199 && pointer<198 && s[pointer]==-24){
			int n=(128+s[pointer+1])*64+(128+s[pointer+2]);
			s[i]=E88XXX[n*2];
			s[i+1]=E88XXX[n*2+1];
			i++;
			pointer+=3;
		}
		else if(i<199 && pointer<198 && s[pointer]==-23){
			int n=(128+s[pointer+1])*64+(128+s[pointer+2]);
			s[i]=E98XXX[n*2];
			s[i+1]=E98XXX[n*2+1];
			i++;
			pointer+=3;
		}
		else if(i<199 && pointer<198 && s[pointer]==-18){
			int n=(128+s[pointer+1])*64+(128+s[pointer+2]);
			s[i]=EE8XXX[n*2];
			s[i+1]=EE8XXX[n*2+1];
			i++;
			pointer+=3;
		}
		else if(i<199 && pointer<198 && s[pointer]==-17){
			if(s[pointer+1]==-67){
				s[i]=s[pointer+2];
				pointer+=3;
			}
			else if(s[pointer+1]==-66){
				s[i]=s[pointer+2]+64;
				pointer+=3;
			}else{
				int n=(128+s[pointer+1])*64+(128+s[pointer+2]);
				s[i]=EF8XXX[n*2];
				s[i+1]=EF8XXX[n*2+1];
				i++;
				pointer+=3;
			}
		} else {
			s[i]=s[pointer];
			pointer++;
		}
	}
}

void load_text(){
	size_t fc=0;

	for(int i=0 ; i<7 ; i++)for(int j=0 ; j<2 ; j++)for(int k=0 ; k<4 ; k++){
		weekChar[i][j][k]=0;
	}
	for(int i=0 ; i<1000 ; i++)for(int j=0 ; j<61 ; j++)for(int k=0 ; k<2 ; k++){
		text[i].str[k][j]=0;
	}

	for(int k=0 ; k<2 ; k++){
		fc=0;
		if(k==0)loadFile("file/data/text_jp.txt");
		else loadFile("file/data/text_en.txt");
		for(int i=0 ; i<7 ; i++){
			for(int j=0 ; j<200 ; j++){
				if(fc<fsize-1 && fstr[fc]==13 && fstr[fc+1]==10){
					fc+=2;
					break;
				}
				else if(fstr[fc]==13 || fstr[fc]==10){
					fc++;
					break;
				}
				weekChar[i][k][j]=fstr[fc];
				fc++;
			}
		}
		for(int i=0 ; i<1000 ; i++){
			if(fc>=fsize)break;
			for(int j=0 ; j<200 ; j++){
				if(fc<fsize-1 && fstr[fc]==13 && fstr[fc+1]==10){
					fc+=2;
					break;
				}
				else if(fstr[fc]==13 || fstr[fc]==10){
					fc++;
					break;
				}
				text[i].str[k][j]=fstr[fc];
				fc++;
			}
		}
	}
	for(int i=0 ; i<7 ; i++){
		UTF8toSJIS(weekChar[i][0]);
		UTF8toLatin(weekChar[i][1]);
	}
	for(int i=0 ; i<1000 ; i++){
		UTF8toSJIS(text[i].str[0]);
		UTF8toLatin(weekChar[i][1]);
	}
}

void load_season(){
	if(season_num)return;
	size_t fc=0;
	gd.hazia=0;
	readSQL("file/data/sql/season.sql");
	if(!loadFile("save/clear.dat")){
		for(int i=0 ; i<SHOP_ITEMS ; i++)gd.bought[i]=false;
		for(int i=0 ; i<season_num ; i++){
			season[i].rate=0;
			season[i].hiscore=0;
		}
		clear_num=1;
	}else{
		for(int i=0 ; i<SHOP_ITEMS ; i++){
			gd.bought[i]=toBool(fstr[fc]);fc++;
			if(gd.bought[i])gd.hazia-=price[i];
		}
		for(int i=0 ; i<season_num ; i++){
			season[i].rate=to8int(fstr[fc]);fc++;
			season[i].hiscore=to32int(fstr[fc],fstr[fc+1],fstr[fc+2],fstr[fc+3]);fc+=4;
			gd.hazia+=season[i].hiscore;
		}
		clear_num=1;
		for(int i=0 ; i<season_num-1 ; i++){//ÅIƒ{ƒX–Ê‚Í•Û—¯
			if(season[i].rate<60)break;
			clear_num++;
		}
	}
}

void load_areas(){
	if(areas)return;
	load_station();
}

void load_station(){
	if(stas)return;
	readSQL("file/data/sql/station.sql");
	readSQL("file/data/sql/area.sql");
	readSQL("file/data/sql/satellite.sql");
	for(int i=0 ; i<stas ; i++)putHeadMark(sta[i].talk);
}

void load_animebook(){
	if(animedex_num)return;
	if(!allofworks_num){
		readSQL("file/data/sql/cartoon.sql");
	}
	animedex_num=allofworks_num;
	collection=0;
	animebook=new bool[animedex_num];
	if(!loadFile("save/animebook.dat")){
		for(int i=0 ; i<animedex_num ; i++)animebook[i]=false;
	}else{
		for(int i=0 ; i<animedex_num ; i++){
			animebook[i]=(fstr[i/8]>>i%8 & 1);
			if(animebook[i])collection++;
		}
	}
}

void save_animebook(){
	if(!animedex_num)load_animebook();
	if(fsize)delete [] fstr;
	fsize=animedex_num/8+1;
	fstr=new char[fsize];
	FILE* hFile;
	int a=0;

	for(int i=0 ; i<(int)fsize ; i++)fstr[i]=0;
	for(int i=0 ; i<animedex_num ; i++){
		if(i%8==0)a+=animebook[i];
		else if(i%8==1)a+=animebook[i]*2;
		else if(i%8==2)a+=animebook[i]*4;
		else if(i%8==3)a+=animebook[i]*8;
		else if(i%8==4)a+=animebook[i]*16;
		else if(i%8==5)a+=animebook[i]*32;
		else if(i%8==6)a+=animebook[i]*64;
		else if(i%8==7)a+=animebook[i]*128;
		if(i%8==7 || i==animedex_num-1){
			fstr[i/8]=a;
			a=0;
		}
	}

	fopen_s(&hFile, "save/animebook.dat", "wb");
	fwrite(fstr, sizeof(fstr[0]), fsize/sizeof(fstr[0]), hFile);
	fclose(hFile);
}

void load_entries(int n){
	load_station();
	load_animebook();
	load_season();

	readSQL("file/data/sql/cartoon.sql");
	sprintf_s(str,"file/data/sql/timetable%d.sql",n);
	readSQL(str);
	for(int i=0 ; i<prgs ; i++){
		allofworks[prg[i].cartoon_index].exist=true;
	}
	if(entries){
		for(int i=0 ; i<entries ; i++){
			if(entry[i].prg_num){
				delete [] entry[i].prg;
			}
		}
		delete [] entry;
	}
	entries=0;
	for(int i=0 ; i<allofworks_num ; i++){
		if(allofworks[i].exist){
			entries++;
		}
	}
	entry=new Entry[entries];
	entries=0;
	for(int i=0 ; i<allofworks_num ; i++){
		if(allofworks[i].exist){
			strcpy_s(entry[entries].title.str[0],allofworks[i].title.str[0]);
			strcpy_s(entry[entries].title.str[1],allofworks[i].title.str[1]);
			entry[entries].mark=allofworks[i].mark;
			entry[entries].r=allofworks[i].r;
			entry[entries].g=allofworks[i].g;
			entry[entries].b=allofworks[i].b;
			entry[entries].exist=true;
			entry[entries].prg_num=0;
			entry[entries].cartoon_id=allofworks[i].cartoon_id;
			entry[entries].cartoon_index=allofworks[i].cartoon_index;
			for(int j=0 ; j<600 ; j++){
				entry[entries].query[j]=allofworks[i].query[j];
			}
			entries++;
		}
	}
	int *prg_count=new int[allofworks_num];
	for(int i=0 ; i<allofworks_num ; i++){
		prg_count[i]=0;
	}
	for(int i=0 ; i<prgs ; i++){
		prg_count[prg[i].cartoon_index]++;
	}
	for(int i=0 ; i<entries ; i++){
		if(prg_count[entry[i].cartoon_index]){
			entry[i].prg = new Program[prg_count[entry[i].cartoon_index]];
		}
	}
	for(int i=0 ; i<entries ; i++){
		entry[i].prg_num=0;
		for(int j=0 ; j<prgs ; j++){
			if(prg[j].cartoon_index==entry[i].cartoon_index){
				int n2=entry[i].prg_num;
				entry[i].prg[n2].week=prg[j].week;
				entry[i].prg[n2].hour=prg[j].hour;
				entry[i].prg[n2].minute=prg[j].minute;
				entry[i].prg[n2].time=prg[j].time;
				entry[i].prg[n2].station_index=prg[j].station_index;
				entry[i].prg_num++;
			}
		}
	}
	delete [] prg;
	delete [] allofworks;
	prgs=0;
	allofworks=0;

	readSQL("file/data/sql/timeslot.sql");
}

void escapeSearchQuery(char* utf8, char*escaped){
	int value=0, pointer=0;
	for(int i=0 ; i<600 ; i++){
		if(utf8[i] == 0){
			escaped[pointer] = 0;
			break;
		}
		else if(utf8[i] == 32){
			escaped[pointer] = '+';pointer++;
		}
		else if(utf8[i]<=-33 && utf8[i]>=-62){
			escaped[pointer] = '%';pointer++;
			value = utf8[i];
			if(value < 0)value += 256;
			escaped[pointer] = hex[value/16];pointer++;
			escaped[pointer] = hex[value%16];pointer++;
			i++;
			escaped[pointer] = '%';pointer++;
			value = utf8[i];
			if(value < 0)value += 256;
			escaped[pointer] = hex[value/16];pointer++;
			escaped[pointer] = hex[value%16];pointer++;
		}
		else if(utf8[i]<=-17 && utf8[i]>=-32){
			escaped[pointer] = '%';pointer++;
			value = utf8[i];
			if(value < 0)value += 256;
			escaped[pointer] = hex[value/16];pointer++;
			escaped[pointer] = hex[value%16];pointer++;
			i++;
			escaped[pointer] = '%';pointer++;
			value = utf8[i];
			if(value < 0)value += 256;
			escaped[pointer] = hex[value/16];pointer++;
			escaped[pointer] = hex[value%16];pointer++;
			i++;
			escaped[pointer] = '%';pointer++;
			value = utf8[i];
			if(value < 0)value += 256;
			escaped[pointer] = hex[value/16];pointer++;
			escaped[pointer] = hex[value%16];pointer++;
		}
		else{
			escaped[pointer] = utf8[i];pointer++;
		}
	}
}

void load_towers(){
	if(towers)return;
	load_areas();
	readSQL("file/data/sql/tower.sql");
	for(int i=0 ; i<towers ; i++){
		area[tower[i].area_index].tower_num++;
	}
	for(int i=0 ; i<areas ; i++){
		area[i].tower=new Tower[area[i].tower_num];
		area[i].tower_num=0;
		for(int j=0 ; j<towers ; j++){
			if(tower[j].area_index==i){
				int n=area[i].tower_num;
				strcpy_s(area[i].tower[n].name.str[0], tower[j].name.str[0]);
				strcpy_s(area[i].tower[n].name.str[1], tower[j].name.str[1]);
				area[i].tower[n].x=tower[j].x;
				area[i].tower[n].y=tower[j].y;
				area[i].tower[n].h=tower[j].h;
				area[i].tower[n].kw=tower[j].kw;
				area[i].tower[n].erp=tower[j].erp;
				area[i].tower[n].v=tower[j].v;
				area[i].tower[n].bias=tower[j].bias;
				for(int k=0 ; k<10 ; k++){
					area[i].tower[n].ch[k]=tower[j].ch[k];
				}
				area[i].tower_num++;
			}
		}
	}
}

void load_mounts(){
	if(mounts)return;
	readSQL("file/data/sql/mountain.sql");
}

void load_towns(){
	if(towns)return;
	load_areas();
	readSQL("file/data/sql/town.sql");
	for(int i=0 ; i<towns ; i++){
		area[town[i].area_index].town_num++;
	}
	for(int i=0 ; i<areas ; i++){
		area[i].town=new Town[area[i].town_num];
		area[i].town_num=0;
		for(int j=0 ; j<towns ; j++){
			if(town[j].area_index==i){
				int n=area[i].town_num;
				area[i].town[n].x=town[j].x;
				area[i].town[n].y=town[j].y;
				strcpy_s(area[i].town[n].name.str[0], town[j].name.str[0]);
				strcpy_s(area[i].town[n].name.str[1], town[j].name.str[1]);
				area[i].town_num++;
			}
		}
	}
}

void load_rural(){
	if(map.rural_num)return;
	load_station();
	readSQL("file/data/sql/rural.sql");
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
	fstr[count]=key.aC&255;count++;
	fstr[count]=key.aC>>8&255;count++;
	fstr[count]=key.aC>>16&255;count++;
	fstr[count]=key.aC>>24&255;count++;
	fstr[count]=key.zC&255;count++;
	fstr[count]=key.zC>>8&255;count++;
	fstr[count]=key.zC>>16&255;count++;
	fstr[count]=key.zC>>24&255;count++;
	fstr[count]=key.xC&255;count++;
	fstr[count]=key.xC>>8&255;count++;
	fstr[count]=key.xC>>16&255;count++;
	fstr[count]=key.xC>>24&255;count++;
	fstr[count]=key.cC&255;count++;
	fstr[count]=key.cC>>8&255;count++;
	fstr[count]=key.cC>>16&255;count++;
	fstr[count]=key.cC>>24&255;count++;
	fstr[count]=key.upC&255;count++;
	fstr[count]=key.upC>>8&255;count++;
	fstr[count]=key.upC>>16&255;count++;
	fstr[count]=key.upC>>24&255;count++;
	fstr[count]=key.downC&255;count++;
	fstr[count]=key.downC>>8&255;count++;
	fstr[count]=key.downC>>16&255;count++;
	fstr[count]=key.downC>>24&255;count++;
	fstr[count]=key.leftC&255;count++;
	fstr[count]=key.leftC>>8&255;count++;
	fstr[count]=key.leftC>>16&255;count++;
	fstr[count]=key.leftC>>24&255;count++;
	fstr[count]=key.rightC&255;count++;
	fstr[count]=key.rightC>>8&255;count++;
	fstr[count]=key.rightC>>16&255;count++;
	fstr[count]=key.rightC>>24&255;count++;

	fopen_s(&hFile, "save/option.dat", "wb");
	fwrite(fstr, sizeof(fstr[0]), count/sizeof(fstr[0]), hFile);
	fclose(hFile);
}

int fetchNameString(char *s, char *value) {
	int pointer=0, length=0;
	while(*s) {
		if(*s==' ' || *s==10 || *s==13){
		}
		else if(*s=='='){
			break;
		}
		else if(*s>='A' && *s<='Z'){
			value[pointer]=(*s)+32;
			pointer++;
		}else{
			value[pointer]=*s;
			pointer++;
		}
		length++;
		s++;
	}
	value[pointer]=0;
	length++;
	return length;
}

void load_parameter(){
	loadFile("file/data/parameter.ini");
	int fc=0;
	char name[100];
	double doubleValue=0;
	double SHADOW_1CH=0,SHADOW_62CH=0,SIGHT_1CH=0,SIGHT_62CH=0,DIFFRACT_1CH=0,DIFFRACT_62CH=0,DIRECTIVITY_1CH=0,DIRECTIVITY_62CH=0;
	while(fc<fsize){
		fc+=fetchNameString(&fstr[fc], name);
		if(name[0]==0)break;
		fc+=fetchDouble(&fstr[fc], &doubleValue);
		if(strcmp(name,"channels")==0){
			CHANNELS=doubleValue;
			mhz=new Mhz[CHANNELS];
			break;
		}
	}
	fc=0;
	while(fc<fsize){
		fc+=fetchNameString(&fstr[fc], name);
		if(name[0]==0)break;
		fc+=fetchDouble(&fstr[fc], &doubleValue);
		if(strcmp(name,"map_width")==0)map.mapW=doubleValue;
		else if(strcmp(name,"map_height")==0)map.mapH=doubleValue;
		else if(strcmp(name,"start_point_x")==0)gd.startX=doubleValue;
		else if(strcmp(name,"start_point_y")==0)gd.startY=doubleValue;
		else if(strcmp(name,"memma_point_x")==0)gd.memmaX=doubleValue;
		else if(strcmp(name,"memma_point_y")==0)gd.memmaY=doubleValue;
		else if(strcmp(name,"distance_1ch")==0)DISTANCE_1CH=doubleValue;
		else if(strcmp(name,"distance_62ch")==0)DISTANCE_62CH=doubleValue;
		else if(strcmp(name,"x_of_graph_vertex")==0)X_OF_GRAPH_VERTEX=doubleValue;
		else if(strcmp(name,"fill_graph_height")==0)FILL_GRAPH_HEIGHT=doubleValue;
		else if(strcmp(name,"diffract_1ch")==0)DIFFRACT_1CH=doubleValue;
		else if(strcmp(name,"diffract_62ch")==0)DIFFRACT_62CH=doubleValue;
		else if(strcmp(name,"shadow_length_400m_1ch")==0)SHADOW_1CH=doubleValue;
		else if(strcmp(name,"shadow_length_400m_62ch")==0)SHADOW_62CH=doubleValue;
		else if(strcmp(name,"magnify_sight_distance_1ch")==0)SIGHT_1CH=doubleValue;
		else if(strcmp(name,"magnify_sight_distance_62ch")==0)SIGHT_62CH=doubleValue;
		else if(strcmp(name,"directivity_1ch")==0)DIRECTIVITY_1CH=doubleValue;
		else if(strcmp(name,"directivity_62ch")==0)DIRECTIVITY_62CH=doubleValue;
		else if(strcmp(name,"receiption_level")==0)RECEIPTION_LEVEL=doubleValue;
		else if(strcmp(name,"map_scale_km")==0)MAP_SCALE=doubleValue;
		for(int i=0 ; i<18 ; i++){
			sprintf_s(str,"item_icon_%d",i+1);
			if(strcmp(name,str)==0)shop_icon[i]=doubleValue;
			sprintf_s(str,"item_price_%d",i+1);
			if(strcmp(name,str)==0)price[i]=doubleValue;
		}
		for(int i=0 ; i<CHANNELS ; i++){
			sprintf_s(str,"frequency_%dch",i+1);
			if(strcmp(name,str)==0)mhz[i].mhz=doubleValue;
		}
	}
	double Y_RANGE=mhz[CHANNELS-1].mhz-mhz[0].mhz;
	double SLOPE_SHADOW = (SHADOW_62CH*SHADOW_62CH - SHADOW_1CH*SHADOW_1CH)/Y_RANGE;
	double Y_INTERCEPT_SHADOW = SHADOW_1CH*SHADOW_1CH - SLOPE_SHADOW*mhz[0].mhz;
	double SLOPE_DIFFRACT = (DIFFRACT_62CH*DIFFRACT_62CH - DIFFRACT_1CH*DIFFRACT_1CH)/Y_RANGE;
	double Y_INTERCEPT_DIFFRACT = DIFFRACT_1CH*DIFFRACT_1CH - SLOPE_DIFFRACT*mhz[0].mhz;
	double SLOPE_DIRECTIVITY = (DIRECTIVITY_62CH*DIRECTIVITY_62CH - DIRECTIVITY_1CH*DIRECTIVITY_1CH)/Y_RANGE;
	double Y_INTERCEPT_DIRECTIVITY = DIRECTIVITY_1CH*DIRECTIVITY_1CH - SLOPE_DIRECTIVITY*mhz[0].mhz;
	double SLOPE_SIGHT = (SIGHT_62CH*SIGHT_62CH - SIGHT_1CH*SIGHT_1CH)/Y_RANGE;
	double Y_INTERCEPT_SIGHT = SIGHT_1CH*SIGHT_1CH - SLOPE_SIGHT*mhz[0].mhz;
	for(int i=0 ; i<CHANNELS ; i++){
		mhz[i].shadow_length=sqrt(mhz[i].mhz * SLOPE_SHADOW + Y_INTERCEPT_SHADOW);
		mhz[i].diffract=sqrt(mhz[i].mhz * SLOPE_DIFFRACT + Y_INTERCEPT_DIFFRACT);
		mhz[i].directivity=sqrt(mhz[i].mhz * SLOPE_DIRECTIVITY + Y_INTERCEPT_DIRECTIVITY);
		mhz[i].magnify_sight_distance=sqrt(mhz[i].mhz * SLOPE_SIGHT + Y_INTERCEPT_SIGHT);
	}
}

void load_option(){
	int fc=0;
	if(!loadFile("save/option.dat")){
		SHOW_TOWER=true;
		AIR_IMG=TYPE1_EDGE;
		DASH_TYPE=TYPE_MARIO;
		CHAR_CODE=JAPANESE;
		MAGNIFY=2;
		pre_magnify=2;
		ADJ_DIR=AUTO;
		EXPLAIN=true;
		BGM_VOLUME=64;
		SE_VOLUME=64;
		NHK_REMOVE=false;
		WALKING_TYPE=SMOOTH;
		ROD_TYPE=CONVENIENTROD;
		reset_key();
	}else{
		SHOW_TOWER=toBool(fstr[fc]);fc++;
		AIR_IMG=fstr[fc];fc++;
		DASH_TYPE=fstr[fc];fc++;
		CHAR_CODE=fstr[fc];fc++;
		MAGNIFY=fstr[fc];fc++;
		pre_magnify=MAGNIFY;
		ADJ_DIR=fstr[fc];fc++;
		EXPLAIN=toBool(fstr[fc]);fc++;
		BGM_VOLUME=fstr[fc];fc++;
		SE_VOLUME=fstr[fc];fc++;
		NHK_REMOVE=toBool(fstr[fc]);fc++;
		WALKING_TYPE=fstr[fc];fc++;
		ROD_TYPE=fstr[fc];fc++;
		key.aC=to32int(fstr[fc],fstr[fc+1],fstr[fc+2],fstr[fc+3]);fc+=4;
		key.zC=to32int(fstr[fc],fstr[fc+1],fstr[fc+2],fstr[fc+3]);fc+=4;
		key.xC=to32int(fstr[fc],fstr[fc+1],fstr[fc+2],fstr[fc+3]);fc+=4;
		key.cC=to32int(fstr[fc],fstr[fc+1],fstr[fc+2],fstr[fc+3]);fc+=4;
		key.upC=to32int(fstr[fc],fstr[fc+1],fstr[fc+2],fstr[fc+3]);fc+=4;
		key.downC=to32int(fstr[fc],fstr[fc+1],fstr[fc+2],fstr[fc+3]);fc+=4;
		key.leftC=to32int(fstr[fc],fstr[fc+1],fstr[fc+2],fstr[fc+3]);fc+=4;
		key.rightC=to32int(fstr[fc],fstr[fc+1],fstr[fc+2],fstr[fc+3]);
	}
}

void save_index(){
	if(fsize)delete [] fstr;
	fsize=season_num*5;
	fstr=new char[fsize+SHOP_ITEMS];
	FILE* hFile;
	size_t fc=0;

	for(int i=0 ; i<SHOP_ITEMS ; i++){
		fstr[fc]=gd.bought[i];fc++;
	}
	for(int i=0 ; i<season_num ; i++){
		fstr[fc]=season[i].rate;fc++;
		fstr[fc]=season[i].hiscore%256;fc++;
		fstr[fc]=(season[i].hiscore/256)%256;fc++;
		fstr[fc]=(season[i].hiscore/65536)%256;fc++;
		fstr[fc]=season[i].hiscore/16777216;fc++;
	}

	sprintf_s(str,"save/clear.dat");
	fopen_s(&hFile, str, "wb");
	fwrite(fstr, sizeof(fstr[0]), fc/sizeof(fstr[0]), hFile);
	fclose(hFile);
}

void save_record(int n){
	if(fsize)delete [] fstr;
	fsize=entries*19+1;
	fstr=new char[fsize];
	FILE* hFile;
	size_t fc=0;

	fstr[fc]=which_season;fc++;
	for(int i=0 ; i<entries ; i++){
		fstr[fc]=fishbox.getData(i,0)%256;fc++;
		fstr[fc]=fishbox.getData(i,0)/256;fc++;
		fstr[fc]=fishbox.getData(i,1)%256;fc++;
		fstr[fc]=fishbox.getData(i,1)/256;fc++;
		fstr[fc]=fishbox.getData(i,2)%256;fc++;
		fstr[fc]=fishbox.getData(i,2)/256;fc++;
		fstr[fc]=fishbox.getData(i,3);fc++;
		fstr[fc]=fishbox.getData(i,4)%256;fc++;
		fstr[fc]=fishbox.getData(i,4)/256;fc++;
		fstr[fc]=fishbox.getData(i,5)%256;fc++;
		fstr[fc]=fishbox.getData(i,5)/256;fc++;
		fstr[fc]=fishbox.getData(i,6);fc++;
		fstr[fc]=fishbox.getData(i,7);fc++;
		fstr[fc]=fishbox.getData(i,8);fc++;
		fstr[fc]=fishbox.getData(i,9);fc++;
		fstr[fc]=fishbox.getData(i,10);fc++;
		fstr[fc]=fishbox.getData(i,11);fc++;
		fstr[fc]=fishbox.getData(i,12)%256;fc++;
		fstr[fc]=fishbox.getData(i,12)/256;fc++;
	}

	sprintf_s(str,"save/record%d.dat",n);
	fopen_s(&hFile, str, "wb");
	fwrite(fstr, sizeof(fstr[0]), fc/sizeof(fstr[0]), hFile);
	fclose(hFile);
}

void load_record(int n){
	size_t fc=0;
	sprintf_s(str,"save/record%d.dat",n);
	loadFile(str);
	which_season=fstr[fc];fc++;
	load_entries(which_season);
	fishbox.initFishBox(entries);
	sprintf_s(str,"save/record%d.dat",n);
	loadFile(str);
	gd.score=0;
	fc=1;
	for(int i=0 ; i<entries ; i++){
		if(fc>=fsize)break;
		fishbox.setData(i,0,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,1,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,2,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,3,fstr[fc]);fc++;
		fishbox.setData(i,4,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,5,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,6,fstr[fc]);fc++;
		fishbox.setData(i,7,fstr[fc]);fc++;
		fishbox.setData(i,8,fstr[fc]);fc++;
		fishbox.setData(i,9,fstr[fc]);fc++;
		fishbox.setData(i,10,fstr[fc]);fc++;
		fishbox.setData(i,11,fstr[fc]);fc++;
		fishbox.setData(i,12,to16int(fstr[fc],fstr[fc+1]));
		gd.score+=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	}
	load_towers();
	load_station();
	createMap();
}

void save_game(int n){
	if(fsize)delete [] fstr;
	fsize=entries*43+19;
	fstr=new char[fsize];
	FILE* hFile;
	size_t fc=0;
	int today_num=0;

	fstr[fc]=which_season;fc++;
	fstr[fc]=entries%256;fc++;
	fstr[fc]=entries/256;fc++;
	fstr[fc]=gd.week;fc++;
	fstr[fc]=gd.hour;fc++;
	fstr[fc]=gd.minute;fc++;
	for(int i=0 ; i<entries ; i++){
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
	for(int i=0 ; i<entries ; i++){
		fstr[fc]=fishbox.getData(i,0)%256;fc++;
		fstr[fc]=fishbox.getData(i,0)/256;fc++;
		fstr[fc]=fishbox.getData(i,1)%256;fc++;
		fstr[fc]=fishbox.getData(i,1)/256;fc++;
		fstr[fc]=fishbox.getData(i,2)%256;fc++;
		fstr[fc]=fishbox.getData(i,2)/256;fc++;
		fstr[fc]=fishbox.getData(i,3);fc++;
		fstr[fc]=fishbox.getData(i,4)%256;fc++;
		fstr[fc]=fishbox.getData(i,4)/256;fc++;
		fstr[fc]=fishbox.getData(i,5)%256;fc++;
		fstr[fc]=fishbox.getData(i,5)/256;fc++;
		fstr[fc]=fishbox.getData(i,6);fc++;
		fstr[fc]=fishbox.getData(i,7);fc++;
		fstr[fc]=fishbox.getData(i,8);fc++;
		fstr[fc]=fishbox.getData(i,9);fc++;
		fstr[fc]=fishbox.getData(i,10);fc++;
		fstr[fc]=fishbox.getData(i,11);fc++;
		fstr[fc]=fishbox.getData(i,12)%256;fc++;
		fstr[fc]=fishbox.getData(i,12)/256;fc++;
	}
	for(int i=0 ; i<today_num ; i++){
		fstr[fc]=fishbox.today[i]%256;fc++;
		fstr[fc]=fishbox.today[i]/256;fc++;
	}
	for(int i=0 ; i<md.fish_num ; i++){
		fstr[fc]=md.maneki_count[i];fc++;
		fstr[fc]=md.fish[i].which_work%256;fc++;
		fstr[fc]=md.fish[i].which_work/256;fc++;
		fstr[fc]=md.fish[i].x%256;fc++;
		fstr[fc]=md.fish[i].x/256;fc++;
		fstr[fc]=md.fish[i].y%256;fc++;
		fstr[fc]=md.fish[i].y/256;fc++;
		fstr[fc]=md.fish[i].sta%256;fc++;
		fstr[fc]=md.fish[i].sta/256;fc++;
		fstr[fc]=md.fish[i].bs;fc++;
		fstr[fc]=md.fish[i].area%256;fc++;
		fstr[fc]=md.fish[i].area/256;fc++;
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
	which_season=fstr[fc];fc++;
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
		fishbox.setData(i,5,to16int(fstr[fc],fstr[fc+1]));fc+=2;
		fishbox.setData(i,6,fstr[fc]);fc++;
		fishbox.setData(i,7,fstr[fc]);fc++;
		fishbox.setData(i,8,fstr[fc]);fc++;
		fishbox.setData(i,9,fstr[fc]);fc++;
		fishbox.setData(i,10,fstr[fc]);fc++;
		fishbox.setData(i,11,fstr[fc]);fc++;
		fishbox.setData(i,12,to16int(fstr[fc],fstr[fc+1]));fc+=2;
	}
	for(int i=0 ; i<today_num ; i++){
		fishbox.today[i]=to16int(fstr[fc],fstr[fc+1]);fc+=2;
	}
	for(int i=0 ; i<md.fish_num ; i++){
		md.maneki_count[i]=fstr[fc];fc++;
		md.fish[i].which_work=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		md.fish[i].x=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		md.fish[i].y=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		md.fish[i].sta=to16int(fstr[fc],fstr[fc+1]);fc+=2;
		md.fish[i].bs=toBool(fstr[fc]);fc++;
		md.fish[i].area=to16int(fstr[fc],fstr[fc+1]);fc+=2;
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
	sprintf_s(str,"save/smr/smr%d.dat",n);
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
				fstr[fc]=a%256;fc++;
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

	sprintf_s(str,"save/smr/smr%d.dat",n);
	fopen_s(&hFile, str, "wb");
	fwrite(fstr, sizeof(fstr[0]), fc/sizeof(fstr[0]), hFile);
	fclose(hFile);
}
