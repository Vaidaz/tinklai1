#include "db.h"

void db_connect(FILE **file){
  *file = fopen(DB_FILE, "a+");
}

int db_add(FILE *file, HASH hash){
  fprintf(file, "lt:%s,en:%s\n", hash.lt, hash.en);
}

void db_close(FILE *file){
  fclose(file);
}
