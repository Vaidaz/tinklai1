#include "db.h"

void db_connect(FILE **file){
  *file = fopen(DB_FILE, "a+");
}

int db_add(FILE *file, Word word){
  int i;
  fprintf(file, "lt:%s,en:%s\n", word.lt, word.en);
}

void db_close(FILE *file){
  fclose(file);
}
