#include "db.h"

// public
void db_connect(FILE **file){
  *file = fopen(DB_FILE, "a+");
}

int db_add(FILE *file, Word word){
  Word tmp_word = empty_word();
  int status = db_find(file, word, &tmp_word);
  switch(status){
    case DB_TRANSLATION_FOUND:{
      return DB_TRANSLATION_FOUND;
      break;
    }
    case DB_TRANSLATION_NOT_FOUND:{
      char buffer[100];
      word_to_string(word, buffer, sizeof(buffer));
      fprintf(file, "%s\n", buffer);
      return DB_TRANSLATION_ADDED;
      break;
    }
  }
}

int db_remove(FILE *file, Word word){
  Word tmp_word = empty_word();
  int status = db_find(file, word, &tmp_word);
  switch(status){
    case DB_TRANSLATION_NOT_FOUND:
      return DB_TRANSLATION_NOT_FOUND;
      break;
    case DB_TRANSLATION_FOUND:{
      char word_string_value[100];
      word_to_string(word, word_string_value, sizeof(word_to_string));

      fseek(file, 0, SEEK_SET);
      FILE *tmp_db = fopen("tmp_db.txt", "w+");

      char *line = NULL;
      size_t len = 0;
      ssize_t read;
      while ((read = getline(&line, &len, file)) != -1) {
        strtok(line, "\n");

        if( strcmp(line, word_string_value) != 0 ){
          fprintf(file, "%s\n", line);
        }

      }

      fclose(tmp_db);
      rename("tmp_db.txt", DB_FILE);

      return DB_TRANSLATION_ADDED;
      break;
    }
  }
}


int db_find(FILE *file, Word search_word, Word *full_word){
  fseek(file, 0, SEEK_SET);

  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  while ((read = getline(&line, &len, file)) != -1) {
    strtok(line, "\n");
    *full_word = string_to_word(line);

    if((strcmp(search_word.en, full_word->en) == 0) ||
        (strcmp(search_word.lt, full_word->lt) == 0)){
      free(line);
      return DB_TRANSLATION_FOUND;
    }
  }

  return DB_TRANSLATION_NOT_FOUND;
}

void db_close(FILE *file){
  fclose(file);
}
