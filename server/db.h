#include <stdio.h>
#include <stdlib.h>
#include "../common/word.h"

#define DB_FILE "database.txt"

// Status codes
#define DB_TRANSLATION_FOUND 0
#define DB_TRANSLATION_NOT_FOUND 1
#define DB_TRANSLATION_ADDED 2
#define DB_TRANSLATION_REMOVED 3

void db_connect(FILE **file);
int db_add(FILE *file, Word word);
int db_remove(FILE *file, Word word);
int db_find(FILE *file, Word search_word, Word *full_word);
void db_close(FILE *file);
