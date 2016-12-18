#ifndef SQL__
#define SQL__

#include "util.h"

int getSeasonById(int id);
int getStationById(int id);
int getCartoonById(int id, Entry *wk, int wk_num);
void readSQL(const char *filename);

#endif
