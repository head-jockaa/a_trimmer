#ifndef FILEIO__
#define FILEIO__

#include "util.h"

extern char hex[16];

void loadEncodingFiles();
void freeEncodingFiles();
void UTF8toLatin(char *s);
void UTF8toSJIS(char *s);

void load_text();
void load_season();
void load_areas();
void load_station();
void load_animebook();
void save_animebook();
void load_entries(int n);
void escapeSearchQuery(char* utf8, char*escaped);
void load_towers();
void load_mounts();
void load_towns();
void load_rural();
void save_option();
void load_option();
void load_parameter();
void save_index();
void save_record(int n);
void load_record(int n);
void save_game(int n);
void load_game(int n);
void load_game_head_only(int n);
void save_smr(int n);
void load_smr(int n);

#endif
