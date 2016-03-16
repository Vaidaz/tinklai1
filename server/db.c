#include "db.h"
#include "../common/statuses.h"

void db_connect(FILE **file){
  *file = fopen(DB_FILE, "a+");
}

int db_add(FILE *file, HASH hash){
  fprintf(file, "lt:%s,en:%s\n", hash.lt, hash.en);
}

int db_find(FILE *file, HASH search_hash, HASH *translation_hash){
  if((strlen(search_hash.en) == 0) && (strlen(search_hash.lt) == 0)){
    translation_hash->status = TRANSLATION_NOT_FOUND;
    return TRANSLATION_NOT_FOUND;
  }

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, file)) != -1) {
    strtok(line, "\n");
    *translation_hash = to_hash(line);
    if((strcmp(search_hash.en, translation_hash->en) == 0) ||
        (strcmp(search_hash.lt, translation_hash->lt) == 0)){
      translation_hash->status = TRANSLATION_FOUND;
      return TRANSLATION_FOUND;
    }
  }

  translation_hash->status = TRANSLATION_NOT_FOUND;
  return TRANSLATION_NOT_FOUND;
}

void db_close(FILE *file){
  fclose(file);
}
