#include <stdio.h>
#include "../common/hash.h"

#define DB_FILE "database.csv"

void db_connect(FILE **file);
int db_add(FILE *file, HASH hash);
int db_find(FILE *file, HASH search_hash, HASH *translation_hash);
void db_close(FILE *file);

