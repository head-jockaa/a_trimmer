#include "sql.h"

#define SQL_INSERT 0
#define SQL_INTO 1
#define SQL_TABLE_NAME 2
#define SQL_START_COLUMN 3
#define SQL_GET_COLUMN 4
#define SQL_COMMA 5
#define SQL_VALUES 6
#define SQL_START_VALUES 7
#define SQL_GET_VALUES 8
#define SQL_COMMA2 9
#define SQL_NEXT_VALUE 10

size_t sqlPointer, sqlSize;
char *sql, tableName[100], tableColumn[100][1000];
int column_num;

void initVariableArray(char *tname, int data_size);
void getSqlValue(char *c, char *tname, char *cname, int index, bool get);

int getAreaById(int id){
	for(int i=0 ; i<areas ; i++) {
		if(area[i].id==id)return i;
	}
	return 0;
}

int getStationById(int id){
	for(int i=0 ; i<stas ; i++) {
		if(sta[i].id==id)return i;
	}
	return -1;
}

int getSeasonById(int id){
	for(int i=0 ; i<season_num ; i++) {
		if(season[i].id==id)return i;
	}
	return 0;
}

int getCartoonById(int id, Entry *wk, int wk_num){
	for(int i=0 ; i<wk_num ; i++) {
		if(wk[i].cartoon_id==id)return i;
	}
	return 0;
}

int fetchSqlString(char *&s, char *value) {
	int n=0;
	n=fetchString(s,'\'',value);
	if(n==0)n=fetchString(s,'"',value);
	if(n==0)n=fetchString(s,'`',value);
	return n;
}

void freeSQL(){
	column_num=0;
	sqlPointer=0;
	if(sqlSize){
		delete [] sql;
		sqlSize=0;
	}
}

void readSQL(const char *filename) {
	int mode=SQL_INSERT;
	int column_point=0, record_num=0;
	bool getDataSize=false;
	size_t restartPoint=0;

	loadFile(filename);
	sqlSize=fsize;
	sql=new char[sqlSize];
	strcpy_no_const(sql,fstr,sqlSize);

	while(sqlPointer < sqlSize) {
		char *c = &sql[sqlPointer];
		if(*c==' ' || *c==10 || *c==13) {
			sqlPointer++;
			continue;
		}
		if(mode==SQL_INSERT) {
			if(startsWith(c,"INSERT")) {
				mode=SQL_INTO;
				sqlPointer+=5;
			}
		}
		else if(mode==SQL_INTO) {
			if(startsWith(c,"INTO")) {
				mode=SQL_TABLE_NAME;
				sqlPointer+=3;
			}
		}
		else if(mode==SQL_TABLE_NAME) {
			fetchSqlString(c,tableName);
			mode=SQL_START_COLUMN;
		}
		else if(mode==SQL_START_COLUMN) {
			if(*c=='(') {
				mode=SQL_GET_COLUMN;
			}
		}
		else if(mode==SQL_GET_COLUMN) {
			fetchSqlString(c,tableColumn[column_num]);
			column_num++;
			mode=SQL_COMMA;
		}
		else if(mode==SQL_COMMA) {
			if(*c==',') {
				mode=SQL_GET_COLUMN;
			}
			else if(*c==')') {
				mode=SQL_VALUES;
			}
		}
		else if(mode==SQL_VALUES) {
			if(startsWith(c,"VALUES")) {
				mode=SQL_START_VALUES;
				sqlPointer+=5;
				restartPoint=sqlPointer;
			}
		}
		else if(mode==SQL_START_VALUES) {
			if(*c=='(') {
				mode=SQL_GET_VALUES;
			}
		}
		else if(mode==SQL_GET_VALUES) {
			getSqlValue(c,tableName,tableColumn[column_point],record_num,getDataSize);
			mode=SQL_COMMA2;
			column_point++;
		}
		else if(mode==SQL_COMMA2) {
			if(*c==',') {
				mode=SQL_GET_VALUES;
			}
			else if(*c==')') {
				mode=SQL_NEXT_VALUE;
				record_num++;
				column_point=0;
			}
		}
		else if(mode==SQL_NEXT_VALUE) {
			if(*c==',') {
				mode=SQL_START_VALUES;
			}
			else if(*c==';') {
				if(getDataSize) {
					break;
				} else {
					mode=SQL_START_VALUES;
					sqlPointer=restartPoint;
					initVariableArray(tableName, record_num);
					record_num=0;
					getDataSize=true;
				}
			}
		}
		sqlPointer++;
	}
	freeSQL();
}

void initVariableArray(char *tname, int data_size) {
	if(strcmp(tname,"area")==0) {
		if(areas) {
			for(int i=0 ; i<areas ; i++){
				if(area[i].tower_num)delete [] area[i].tower;
				if(area[i].town_num)delete [] area[i].town;
			}
			delete [] area;
		}
		areas=data_size;
		area=new Area[areas];
		for(int i=0 ; i<areas ; i++) {
			area[i].st_num=0;
			area[i].tower_num=0;
			area[i].town_num=0;
		}
	}
	else if(strcmp(tname,"areacode")==0) {
		if(areacode_num) {
			delete [] areacode;
		}
		areacode_num=data_size;
		areacode=new Areacode[areacode_num];
	}
	else if(strcmp(tname,"rural")==0) {
		if(map.rural_num) {
			for(int i=0 ; i<map.rural_num ; i++) {
				delete [] map.rural_tv[i];
			}
			delete [] map.rural_tv;
			for(int i=0 ; i<map.ruralW ; i++) {
				delete [] map.rural[i];
			}
			delete [] map.rural;
			delete [] map.rural_rate;
		}
		map.rural_num=data_size;
		map.rural_tv=new int*[map.rural_num];
		for(int i=0 ; i<map.rural_num ; i++) {
			map.rural_tv[i]=new int[10];
		}
		map.ruralW=map.mapW/10;
		map.ruralH=map.mapH/10;
		map.rural=new Uint8*[map.ruralW];
		for(int i=0 ; i<map.ruralW ; i++) {
			map.rural[i]=new Uint8[map.ruralH];
		}
		for(int j=0 ; j<map.ruralH ; j++) {
			for(int i=0 ; i<map.ruralW ; i++) {
				map.rural[i][j]=-1;
			}
		}
		map.rural_rate=new Uint16[map.rural_num];
		map.rural_size=10;
	}
	else if(strcmp(tname,"cartoon")==0) {
		if(allofworks_num) {
			delete [] allofworks;
		}
		allofworks_num=data_size;
		allofworks=new Entry[allofworks_num];
		for(int i=0 ; i<allofworks_num ; i++) {
			allofworks[i].prg_num=0;
			allofworks[i].exist=false;
		}
	}
	else if(strcmp(tname,"mountain")==0) {
		if(mounts) {
			delete [] mount;
		}
		mounts=data_size;
		mount=new Mount[mounts];
	}
	else if(strcmp(tname,"season")==0) {
		if(season_num) {
			delete [] season;
		}
		season_num=data_size;
		season=new Season[season_num];
	}
	else if(strcmp(tname,"station")==0) {
		if(stas) {
			delete [] sta;
		}
		stas=data_size;
		sta=new Station[stas];
	}
	else if(strcmp(tname,"timeslot")==0) {
		if(timeslot_num) {
			delete [] timeslot;
		}
		timeslot_num=data_size;
		timeslot=new Timeslot[timeslot_num];
	}
	else if(strcmp(tname,"timetable")==0) {
		if(prgs) {
			delete [] prg;
		}
		prgs=data_size;
		prg=new Program[prgs];
	}
	else if(strcmp(tname,"tower")==0) {
		if(towers) {
			delete [] tower;
		}
		towers=data_size;
		tower=new Tower[towers];
	}
	else if(strcmp(tname,"town")==0) {
		if(towns) {
			delete [] town;
		}
		towns=data_size;
		town=new Town[towns];
	}
}

void getSqlValue(char *c, char *tname, char *cname, int index, bool get) {
	int intValue=0;
	double doubleValue=0;
	if(strcmp(tname,"area")==0) {
		if(strcmp(cname,"id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)area[index].id=intValue;
		}
		else if(strcmp(cname,"name_jp")==0) {
			sqlPointer+=fetchSqlString(c,str)-1;
			if(get){
				strcpy_s(area[index].name.str[0],str);
				UTF8toSJIS(area[index].name.str[0]);
			}
		}
		else if(strcmp(cname,"name_en")==0) {
			sqlPointer+=fetchSqlString(c,str)-1;
			if(get)strcpy_s(area[index].name.str[1],str);
		}
		else if(strcmp(cname,"key1")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[0]=intValue;
			}
		}
		else if(strcmp(cname,"key2")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[1]=intValue;
			}
		}
		else if(strcmp(cname,"key3")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[2]=intValue;
			}
		}
		else if(strcmp(cname,"key4")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[3]=intValue;
			}
		}
		else if(strcmp(cname,"key5")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[4]=intValue;
			}
		}
		else if(strcmp(cname,"key6")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[5]=intValue;
			}
		}
		else if(strcmp(cname,"key7")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[6]=intValue;
			}
		}
		else if(strcmp(cname,"key8")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[7]=intValue;
			}
		}
		else if(strcmp(cname,"key9")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[8]=intValue;
			}
		}
		else if(strcmp(cname,"key10")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[9]=intValue;
			}
		}
		else if(strcmp(cname,"tv1")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)area[index].station[0]=getStationById(intValue);
		}
		else if(strcmp(cname,"tv2")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)area[index].station[1]=getStationById(intValue);
		}
		else if(strcmp(cname,"tv3")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)area[index].station[2]=getStationById(intValue);
		}
		else if(strcmp(cname,"tv4")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)area[index].station[3]=getStationById(intValue);
		}
		else if(strcmp(cname,"tv5")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)area[index].station[4]=getStationById(intValue);
		}
		else if(strcmp(cname,"tv6")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)area[index].station[5]=getStationById(intValue);
		}
		else if(strcmp(cname,"tv7")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)area[index].station[6]=getStationById(intValue);
		}
		else if(strcmp(cname,"tv8")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)area[index].station[7]=getStationById(intValue);
		}
		else if(strcmp(cname,"tv9")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)area[index].station[8]=getStationById(intValue);
		}
		else if(strcmp(cname,"tv10")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)area[index].station[9]=getStationById(intValue);
		}
	}
	else if(strcmp(tname,"rural")==0) {
		if(strcmp(cname,"filepath")==0) {
			sqlPointer+=fetchSqlString(c,str)-1;
			if(get) {
				Image *ruralmap;
				getImage(ruralmap,str,BLACK);
				for(int j=0 ; j<map.ruralH ; j++) {
					if(j==ruralmap->h)break;
					for(int i=0 ; i<map.ruralW ; i++) {
						if(i==ruralmap->w)break;
						// extract BLUE component
						if(ABGR) {
							if(ruralmap->RGB[j*(ruralmap->w)+i] & (128<<16)) {
								map.rural[i][j]=index;
							}
						} else {
							if(ruralmap->RGB[j*(ruralmap->w)+i] & 128) {
								map.rural[i][j]=index;
							}
						}
					}
				}
				freeImage(ruralmap);
			}
		}
		else if(strcmp(cname,"tv1")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				map.rural_tv[index][0]=getStationById(intValue);
			}
		}
		else if(strcmp(cname,"tv2")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				map.rural_tv[index][1]=getStationById(intValue);
			}
		}
		else if(strcmp(cname,"tv3")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				map.rural_tv[index][2]=getStationById(intValue);
			}
		}
		else if(strcmp(cname,"tv4")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				map.rural_tv[index][3]=getStationById(intValue);
			}
		}
		else if(strcmp(cname,"tv5")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				map.rural_tv[index][4]=getStationById(intValue);
			}
		}
		else if(strcmp(cname,"tv6")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				map.rural_tv[index][5]=getStationById(intValue);
			}
		}
		else if(strcmp(cname,"tv7")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				map.rural_tv[index][6]=getStationById(intValue);
			}
		}
		else if(strcmp(cname,"tv8")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				map.rural_tv[index][7]=getStationById(intValue);
			}
		}
		else if(strcmp(cname,"tv9")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				map.rural_tv[index][8]=getStationById(intValue);
			}
		}
		else if(strcmp(cname,"tv10")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get) {
				map.rural_tv[index][9]=getStationById(intValue);
			}
		}
	}
	else if(strcmp(tname,"areacode")==0) {
		if(strcmp(cname,"id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].id=intValue;
		}
		else if(strcmp(cname,"name_jp")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get){
				strcpy_s(areacode[index].name.str[0],str);
				UTF8toSJIS(areacode[index].name.str[0]);
			}
		}
		else if(strcmp(cname,"name_en")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get)strcpy_s(areacode[index].name.str[1],str);
		}
		else if(strcmp(cname,"button1")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].code[0]=intValue;
		}
		else if(strcmp(cname,"button2")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].code[1]=intValue;
		}
		else if(strcmp(cname,"button3")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].code[2]=intValue;
		}
		else if(strcmp(cname,"button4")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].code[3]=intValue;
		}
		else if(strcmp(cname,"button5")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].code[4]=intValue;
		}
		else if(strcmp(cname,"button6")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].code[5]=intValue;
		}
		else if(strcmp(cname,"button7")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].code[6]=intValue;
		}
		else if(strcmp(cname,"button8")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].code[7]=intValue;
		}
		else if(strcmp(cname,"button9")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].code[8]=intValue;
		}
		else if(strcmp(cname,"button10")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].code[9]=intValue;
		}
		else if(strcmp(cname,"button11")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].code[10]=intValue;
		}
		else if(strcmp(cname,"button12")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)areacode[index].code[11]=intValue;
		}
	}
	else if(strcmp(tname,"cartoon")==0) {
		if(strcmp(cname,"id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get){
				allofworks[index].cartoon_id=intValue;
				allofworks[index].cartoon_index=intValue-1;
			}
		}
		else if(strcmp(cname,"name_jp")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get){
				strcpy_s(allofworks[index].title.str[0],str);
				UTF8toSJIS(allofworks[index].title.str[0]);
			}
		}
		else if(strcmp(cname,"name_en")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get){
				strcpy_s(allofworks[index].title.str[1],str);
				UTF8toLatin(allofworks[index].title.str[1]);
			}
		}
		else if(strcmp(cname,"search_query")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get)escapeSearchQuery(str,allofworks[index].query);
		}
		else if(strcmp(cname,"icon")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)allofworks[index].mark=intValue;
		}
		else if(strcmp(cname,"red")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)allofworks[index].r=intValue;
		}
		else if(strcmp(cname,"green")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)allofworks[index].g=intValue;
		}
		else if(strcmp(cname,"blue")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)allofworks[index].b=intValue;
		}
	}
	else if(strcmp(tname,"mountain")==0) {
		if(strcmp(cname,"id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)mount[index].id=intValue;
		}
		else if(strcmp(cname,"x")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)mount[index].x=intValue;
		}
		else if(strcmp(cname,"y")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)mount[index].y=intValue;
		}
		else if(strcmp(cname,"height")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)mount[index].h=intValue;
		}
		else if(strcmp(cname,"slope")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)mount[index].slope=intValue;
		}
		else if(strcmp(cname,"range")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)mount[index].range=intValue;
		}
		else if(strcmp(cname,"volcano")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)mount[index].volcano=intValue;
		}
		else if(strcmp(cname,"building")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)mount[index].city=intValue;
		}
		else if(strcmp(cname,"note")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get)strcpy_s(mount[index].note.str[0],str);
		}
	}
	else if(strcmp(tname,"satellite")==0) {
		if(strcmp(cname,"physical")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)BSchannel[index]=intValue;
		}
		else if(strcmp(cname,"station_id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)BSstation[index]=getStationById(intValue);
		}
	}
	else if(strcmp(tname,"season")==0) {
		if(strcmp(cname,"id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)season[index].id=intValue;
		}
		else if(strcmp(cname,"name_jp")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get){
				strcpy_s(season[index].name.str[0],str);
				UTF8toSJIS(season[index].name.str[0]);
			}
		}
		else if(strcmp(cname,"name_en")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get)strcpy_s(season[index].name.str[1],str);
		}
	}
	else if(strcmp(tname,"station")==0) {
		if(strcmp(cname,"id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)sta[index].id=intValue;
		}
		else if(strcmp(cname,"icon")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)sta[index].mark=intValue;
		}
		else if(strcmp(cname,"name_jp")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get){
				strcpy_s(sta[index].name.str[0],str);
				UTF8toSJIS(sta[index].name.str[0]);
			}
		}
		else if(strcmp(cname,"name_en")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get)strcpy_s(sta[index].name.str[1],str);
		}
		else if(strcmp(cname,"message_jp")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get){
				strcpy_s(sta[index].talk.str[0],str);
				UTF8toSJIS(sta[index].talk.str[0]);
			}
		}
		else if(strcmp(cname,"message_en")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get)strcpy_s(sta[index].talk.str[1],str);
		}
		else if(strcmp(cname,"abbr")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get)strcpy_s(sta[index].abbr.str[0],str);
			if(get)strcpy_s(sta[index].abbr.str[1],str);
		}
	}
	else if(strcmp(tname,"timeslot")==0) {
		if(strcmp(cname,"id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)timeslot[index].id=intValue;
		}
		else if(strcmp(cname,"season_id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)timeslot[index].season_index=getSeasonById(intValue);
		}
		else if(strcmp(cname,"week")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)timeslot[index].week=intValue;
		}
		else if(strcmp(cname,"hour")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)timeslot[index].hour=intValue;
		}
		else if(strcmp(cname,"minute")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)timeslot[index].minute=intValue;
		}
		else if(strcmp(cname,"type")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)timeslot[index].type=intValue;
		}
	}
	else if(strcmp(tname,"timetable")==0) {
		if(strcmp(cname,"id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)prg[index].id=intValue;
		}
		else if(strcmp(cname,"season_id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)prg[index].season_index=getSeasonById(intValue);
		}
		else if(strcmp(cname,"anime_id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)prg[index].cartoon_index=getCartoonById(intValue,allofworks,allofworks_num);
		}
		else if(strcmp(cname,"station_id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)prg[index].station_index=getStationById(intValue);
		}
		else if(strcmp(cname,"week")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)prg[index].week=intValue;
		}
		else if(strcmp(cname,"hour")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)prg[index].hour=intValue;
		}
		else if(strcmp(cname,"minute")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)prg[index].minute=intValue;
		}
		else if(strcmp(cname,"time")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)prg[index].time=intValue;
		}
	}
	else if(strcmp(tname,"tower")==0) {
		if(strcmp(cname,"id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].id=intValue;
		}
		else if(strcmp(cname,"area_id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].area_index=getAreaById(intValue);
		}
		else if(strcmp(cname,"name_jp")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get){
				strcpy_s(tower[index].name.str[0],str);
				UTF8toSJIS(tower[index].name.str[0]);
			}
		}
		else if(strcmp(cname,"name_en")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get)strcpy_s(tower[index].name.str[1],str);
		}
		else if(strcmp(cname,"x")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].x=intValue;
		}
		else if(strcmp(cname,"y")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].y=intValue;
		}
		else if(strcmp(cname,"height")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].h=intValue;
		}
		else if(strcmp(cname,"kw")==0) {
			sqlPointer+=fetchDouble(c,&doubleValue)-1;
			if(get)tower[index].kw=doubleValue;
		}
		else if(strcmp(cname,"erp")==0) {
			sqlPointer+=fetchDouble(c,&doubleValue)-1;
			if(get)tower[index].erp=doubleValue;
		}
		else if(strcmp(cname,"vertical")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].v=intValue;
		}
		else if(strcmp(cname,"bias")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].bias=intValue;
		}
		else if(strcmp(cname,"ch1")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].ch[0]=intValue;
		}
		else if(strcmp(cname,"ch2")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].ch[1]=intValue;
		}
		else if(strcmp(cname,"ch3")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].ch[2]=intValue;
		}
		else if(strcmp(cname,"ch4")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].ch[3]=intValue;
		}
		else if(strcmp(cname,"ch5")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].ch[4]=intValue;
		}
		else if(strcmp(cname,"ch6")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].ch[5]=intValue;
		}
		else if(strcmp(cname,"ch7")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].ch[6]=intValue;
		}
		else if(strcmp(cname,"ch8")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].ch[7]=intValue;
		}
		else if(strcmp(cname,"ch9")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].ch[8]=intValue;
		}
		else if(strcmp(cname,"ch10")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)tower[index].ch[9]=intValue;
		}
	}
	else if(strcmp(tname,"town")==0) {
		if(strcmp(cname,"id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)town[index].id=intValue;
		}
		else if(strcmp(cname,"area_id")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)town[index].area_index=getAreaById(intValue);
		}
		else if(strcmp(cname,"name_jp")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get){
				strcpy_s(town[index].name.str[0],str);
				UTF8toSJIS(town[index].name.str[0]);
			}
		}
		else if(strcmp(cname,"name_en")==0) {
			sqlPointer+=fetchSqlString(c,str);
			if(get)strcpy_s(town[index].name.str[1],str);
		}
		else if(strcmp(cname,"x")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)town[index].x=intValue;
		}
		else if(strcmp(cname,"y")==0) {
			sqlPointer+=fetchInt(c,&intValue)-1;
			if(get)town[index].y=intValue;
		}
	}
}
