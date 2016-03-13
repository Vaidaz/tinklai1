#include <stdio.h>
#include "../common/word.h"

#define DB_FILE "database.csv"

void db_connect(FILE **file);
int db_add(FILE *file, Word word);
void db_close(FILE *file);
