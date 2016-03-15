#include <stdio.h>
#include "../common/hash.h"

#define DB_FILE "database.csv"

void db_connect(FILE **file);
int db_add(FILE *file, HASH hash);
void db_close(FILE *file);

