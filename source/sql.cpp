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
void getAreaSqlValue(char *c, char *tname, char *cname, int index, bool get);

void fetchSqlString(char *&s, char *value) {
	bool inner=false, start=false;
	int pointer=0;
	while(*s) {
		if(*s==' ' || *s==10 || *s==13) {
			if(inner) {
				value[pointer]=*s;
				pointer++;
			}
			else if(start) {
				value[pointer]=0;
				break;
			}
		}
		else if(*s=='`') {
			if(!start) {
				inner=true;
				start=true;
			} else {
				value[pointer]=0;
				break;
			}
		} else {
			value[pointer]=*s;
			pointer++;
			start=true;
		}
		sqlPointer++;
		s++;
	}
}

void fetchSqlString2(char *&s, char *value) {
	bool start1=false, start2=false;
	int pointer=0;
	while(*s) {
		if(start1) {
			if(*s=='\\' && *(s+1)=='\'') {
				value[pointer]='\'';
				pointer++;
				sqlPointer++;
				s++;
			}
			else if(*s=='\'') {
				value[pointer]=0;
				break;
			}else{
				value[pointer]=*s;
				pointer++;
			}
		}
		else if(start2) {
			if(*s=='\\' && *(s+1)=='"') {
				value[pointer]='"';
				pointer++;
				sqlPointer++;
				s++;
			}
			else if(*s=='"') {
				value[pointer]=0;
				break;
			}else{
				value[pointer]=*s;
				pointer++;
			}
		}
		else if(*s==' ' || *s==10 || *s==13) {
		}
		else if(*s=='\'') {
			start1=true;
		}
		else if(*s=='"') {
			start2=true;
		}
		sqlPointer++;
		s++;
	}
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
	freeSQL();
	int mode=SQL_INSERT;
	int column_point=0, record_num=0;
	bool getDataSize=false;
	size_t restartPoint=0;

	loadFile(filename);
	sqlSize=fsize;
	sql=new char[sqlSize];
	strcpy_s(sql,fstr);

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
			getAreaSqlValue(c,tableName,tableColumn[column_point],record_num,getDataSize);
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
	else if(strcmp(tname,"cartoon")==0) {
		if(allofworks_num) {
			delete [] allofworks;
		}
		allofworks_num=data_size;
		allofworks=new Work[allofworks_num];
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
		if(index_num) {
			delete [] indexName;
		}
		index_num=data_size;
		indexName=new Index[index_num];
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
		prg=new Prg[prgs];
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

void getAreaSqlValue(char *c, char *tname, char *cname, int index, bool get) {
	int intValue=0;
	double doubleValue=0;
	if(strcmp(tname,"area")==0) {
		if(strcmp(cname,"id")==0) {
			intValue=fetchInt(c);
			if(get)area[index].id=intValue;
		}
		else if(strcmp(cname,"name_jp")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(area[index].name.str[0],str);
		}
		else if(strcmp(cname,"name_en")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(area[index].name.str[1],str);
		}
		else if(strcmp(cname,"key1")==0) {
			intValue=fetchInt(c);
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[0]=intValue;
			}
		}
		else if(strcmp(cname,"key2")==0) {
			intValue=fetchInt(c);
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[1]=intValue;
			}
		}
		else if(strcmp(cname,"key3")==0) {
			intValue=fetchInt(c);
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[2]=intValue;
			}
		}
		else if(strcmp(cname,"key4")==0) {
			intValue=fetchInt(c);
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[3]=intValue;
			}
		}
		else if(strcmp(cname,"key5")==0) {
			intValue=fetchInt(c);
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[4]=intValue;
			}
		}
		else if(strcmp(cname,"key6")==0) {
			intValue=fetchInt(c);
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[5]=intValue;
			}
		}
		else if(strcmp(cname,"key7")==0) {
			intValue=fetchInt(c);
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[6]=intValue;
			}
		}
		else if(strcmp(cname,"key8")==0) {
			intValue=fetchInt(c);
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[7]=intValue;
			}
		}
		else if(strcmp(cname,"key9")==0) {
			intValue=fetchInt(c);
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[8]=intValue;
			}
		}
		else if(strcmp(cname,"key10")==0) {
			intValue=fetchInt(c);
			if(get) {
				if(intValue)area[index].st_num++;
				area[index].button[9]=intValue;
			}
		}
		else if(strcmp(cname,"tv1")==0) {
			intValue=fetchInt(c);
			if(get)area[index].station[0]=intValue-1;
		}
		else if(strcmp(cname,"tv2")==0) {
			intValue=fetchInt(c);
			if(get)area[index].station[1]=intValue-1;
		}
		else if(strcmp(cname,"tv3")==0) {
			intValue=fetchInt(c);
			if(get)area[index].station[2]=intValue-1;
		}
		else if(strcmp(cname,"tv4")==0) {
			intValue=fetchInt(c);
			if(get)area[index].station[3]=intValue-1;
		}
		else if(strcmp(cname,"tv5")==0) {
			intValue=fetchInt(c);
			if(get)area[index].station[4]=intValue-1;
		}
		else if(strcmp(cname,"tv6")==0) {
			intValue=fetchInt(c);
			if(get)area[index].station[5]=intValue-1;
		}
		else if(strcmp(cname,"tv7")==0) {
			intValue=fetchInt(c);
			if(get)area[index].station[6]=intValue-1;
		}
		else if(strcmp(cname,"tv8")==0) {
			intValue=fetchInt(c);
			if(get)area[index].station[7]=intValue-1;
		}
		else if(strcmp(cname,"tv9")==0) {
			intValue=fetchInt(c);
			if(get)area[index].station[8]=intValue-1;
		}
		else if(strcmp(cname,"tv10")==0) {
			intValue=fetchInt(c);
			if(get)area[index].station[9]=intValue-1;
		}
	}
	else if(strcmp(tname,"areacode")==0) {
		if(strcmp(cname,"id")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].id=intValue;
		}
		else if(strcmp(cname,"name_jp")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(areacode[index].name.str[0],str);
		}
		else if(strcmp(cname,"name_en")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(areacode[index].name.str[1],str);
		}
		else if(strcmp(cname,"button1")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].code[0]=intValue;
		}
		else if(strcmp(cname,"button2")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].code[1]=intValue;
		}
		else if(strcmp(cname,"button3")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].code[2]=intValue;
		}
		else if(strcmp(cname,"button4")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].code[3]=intValue;
		}
		else if(strcmp(cname,"button5")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].code[4]=intValue;
		}
		else if(strcmp(cname,"button6")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].code[5]=intValue;
		}
		else if(strcmp(cname,"button7")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].code[6]=intValue;
		}
		else if(strcmp(cname,"button8")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].code[7]=intValue;
		}
		else if(strcmp(cname,"button9")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].code[8]=intValue;
		}
		else if(strcmp(cname,"button10")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].code[9]=intValue;
		}
		else if(strcmp(cname,"button11")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].code[10]=intValue;
		}
		else if(strcmp(cname,"button12")==0) {
			intValue=fetchInt(c);
			if(get)areacode[index].code[11]=intValue;
		}
	}
	else if(strcmp(tname,"cartoon")==0) {
		if(strcmp(cname,"id")==0) {
			intValue=fetchInt(c);
			if(get)allofworks[index].cartoon_id=intValue;
		}
		else if(strcmp(cname,"name_jp")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(allofworks[index].title.str[0],str);
		}
		else if(strcmp(cname,"name_en")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(allofworks[index].title.str[1],str);
		}
		else if(strcmp(cname,"icon")==0) {
			intValue=fetchInt(c);
			if(get)allofworks[index].mark=intValue;
		}
		else if(strcmp(cname,"red")==0) {
			intValue=fetchInt(c);
			if(get)allofworks[index].r=intValue;
		}
		else if(strcmp(cname,"green")==0) {
			intValue=fetchInt(c);
			if(get)allofworks[index].g=intValue;
		}
		else if(strcmp(cname,"blue")==0) {
			intValue=fetchInt(c);
			if(get)allofworks[index].b=intValue;
		}
	}
	else if(strcmp(tname,"mountain")==0) {
		if(strcmp(cname,"id")==0) {
			intValue=fetchInt(c);
			if(get)mount[index].id=intValue;
		}
		else if(strcmp(cname,"x")==0) {
			intValue=fetchInt(c);
			if(get)mount[index].x=intValue;
		}
		else if(strcmp(cname,"y")==0) {
			intValue=fetchInt(c);
			if(get)mount[index].y=intValue;
		}
		else if(strcmp(cname,"height")==0) {
			intValue=fetchInt(c);
			if(get)mount[index].h=intValue;
		}
		else if(strcmp(cname,"slope")==0) {
			intValue=fetchInt(c);
			if(get)mount[index].slope=intValue;
		}
		else if(strcmp(cname,"range")==0) {
			intValue=fetchInt(c);
			if(get)mount[index].range=intValue;
		}
		else if(strcmp(cname,"volcano")==0) {
			intValue=fetchInt(c);
			if(get)mount[index].volcano=intValue;
		}
		else if(strcmp(cname,"building")==0) {
			intValue=fetchInt(c);
			if(get)mount[index].city=intValue;
		}
	}
	else if(strcmp(tname,"satellite")==0) {
		if(strcmp(cname,"physical")==0) {
			intValue=fetchInt(c);
			if(get)BSchannel[index]=intValue;
		}
		else if(strcmp(cname,"station_id")==0) {
			intValue=fetchInt(c);
			if(get)BSstation[index]=intValue-1;
		}
	}
	else if(strcmp(tname,"season")==0) {
		if(strcmp(cname,"id")==0) {
			intValue=fetchInt(c);
			if(get)indexName[index].id=intValue;
		}
		else if(strcmp(cname,"name_jp")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(indexName[index].name.str[0],str);
		}
		else if(strcmp(cname,"name_en")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(indexName[index].name.str[1],str);
		}
	}
	else if(strcmp(tname,"station")==0) {
		if(strcmp(cname,"id")==0) {
			intValue=fetchInt(c);
			if(get)sta[index].id=intValue;
		}
		else if(strcmp(cname,"icon")==0) {
			intValue=fetchInt(c);
			if(get)sta[index].mark=intValue;
		}
		else if(strcmp(cname,"name_jp")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(sta[index].name.str[0],str);
		}
		else if(strcmp(cname,"name_en")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(sta[index].name.str[1],str);
		}
		else if(strcmp(cname,"message_jp")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(sta[index].talk.str[0],str);
		}
		else if(strcmp(cname,"message_en")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(sta[index].talk.str[1],str);
		}
	}
	else if(strcmp(tname,"timeslot")==0) {
		if(strcmp(cname,"id")==0) {
			intValue=fetchInt(c);
			if(get)timeslot[index].id=intValue;
		}
		else if(strcmp(cname,"season_id")==0) {
			intValue=fetchInt(c);
			if(get)timeslot[index].season_id=intValue;
		}
		else if(strcmp(cname,"week")==0) {
			intValue=fetchInt(c);
			if(get)timeslot[index].week=intValue;
		}
		else if(strcmp(cname,"hour")==0) {
			intValue=fetchInt(c);
			if(get)timeslot[index].hour=intValue;
		}
		else if(strcmp(cname,"minute")==0) {
			intValue=fetchInt(c);
			if(get)timeslot[index].minute=intValue;
		}
		else if(strcmp(cname,"type")==0) {
			intValue=fetchInt(c);
			if(get)timeslot[index].type=intValue;
		}
	}
	else if(strcmp(tname,"timetable")==0) {
		if(strcmp(cname,"id")==0) {
			intValue=fetchInt(c);
			if(get)prg[index].id=intValue;
		}
		else if(strcmp(cname,"season_id")==0) {
			intValue=fetchInt(c);
			if(get)prg[index].season_id=intValue;
		}
		else if(strcmp(cname,"anime_id")==0) {
			intValue=fetchInt(c);
			if(get)prg[index].work=intValue;
		}
		else if(strcmp(cname,"station_id")==0) {
			intValue=fetchInt(c);
			if(get)prg[index].station=intValue;
		}
		else if(strcmp(cname,"week")==0) {
			intValue=fetchInt(c);
			if(get)prg[index].week=intValue;
		}
		else if(strcmp(cname,"hour")==0) {
			intValue=fetchInt(c);
			if(get)prg[index].hour=intValue;
		}
		else if(strcmp(cname,"minute")==0) {
			intValue=fetchInt(c);
			if(get)prg[index].minute=intValue;
		}
		else if(strcmp(cname,"time")==0) {
			intValue=fetchInt(c);
			if(get)prg[index].time=intValue;
		}
	}
	else if(strcmp(tname,"tower")==0) {
		if(strcmp(cname,"id")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].id=intValue;
		}
		else if(strcmp(cname,"area_id")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].area_id=intValue;
		}
		else if(strcmp(cname,"name_jp")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(tower[index].name.str[0],str);
		}
		else if(strcmp(cname,"name_en")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(tower[index].name.str[1],str);
		}
		else if(strcmp(cname,"x")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].x=intValue;
		}
		else if(strcmp(cname,"y")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].y=intValue;
		}
		else if(strcmp(cname,"height")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].h=intValue;
		}
		else if(strcmp(cname,"kw")==0) {
			doubleValue=fetchDouble(c);
			if(get)tower[index].kw=doubleValue;
		}
		else if(strcmp(cname,"erp")==0) {
			doubleValue=fetchDouble(c);
			if(get)tower[index].erp=doubleValue;
		}
		else if(strcmp(cname,"vertical")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].v=intValue;
		}
		else if(strcmp(cname,"bias")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].bias=intValue;
		}
		else if(strcmp(cname,"ch1")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].ch[0]=intValue;
		}
		else if(strcmp(cname,"ch2")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].ch[1]=intValue;
		}
		else if(strcmp(cname,"ch3")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].ch[2]=intValue;
		}
		else if(strcmp(cname,"ch4")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].ch[3]=intValue;
		}
		else if(strcmp(cname,"ch5")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].ch[4]=intValue;
		}
		else if(strcmp(cname,"ch6")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].ch[5]=intValue;
		}
		else if(strcmp(cname,"ch7")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].ch[6]=intValue;
		}
		else if(strcmp(cname,"ch8")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].ch[7]=intValue;
		}
		else if(strcmp(cname,"ch9")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].ch[8]=intValue;
		}
		else if(strcmp(cname,"ch10")==0) {
			intValue=fetchInt(c);
			if(get)tower[index].ch[9]=intValue;
		}
	}
	else if(strcmp(tname,"town")==0) {
		if(strcmp(cname,"id")==0) {
			intValue=fetchInt(c);
			if(get)town[index].id=intValue;
		}
		else if(strcmp(cname,"area_id")==0) {
			intValue=fetchInt(c);
			if(get)town[index].area_id=intValue;
		}
		else if(strcmp(cname,"name_jp")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(town[index].name.str[0],str);
		}
		else if(strcmp(cname,"name_en")==0) {
			fetchSqlString2(c,str);
			if(get)strcpy_s(town[index].name.str[1],str);
		}
		else if(strcmp(cname,"x")==0) {
			intValue=fetchInt(c);
			if(get)town[index].x=intValue;
		}
		else if(strcmp(cname,"y")==0) {
			intValue=fetchInt(c);
			if(get)town[index].y=intValue;
		}
	}
}
