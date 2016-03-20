#include "word.h"
#include <string.h>

void word_to_string(Word word, char *string, int string_size){
  snprintf(string, string_size-1, "en:%s%slt:%s", word.en, TRANSLATIONS_SEPARATOR, word.lt);
}

Word string_to_word(char *string){
  char buffer[strlen(string)+1];
  strncpy(buffer, string, sizeof(buffer)+1);
  strtok(buffer, "\n");

  // Suskaldau eilutę į k.v. poras pagal ';'
  // Porų yra 2 nes kolkas yra tik en ir lt kalbos.
  char *translations[NUMBER_OF_TRANSLATIONS_IN_WORD];
  int translations_size = 0;
  translations[translations_size] = strtok(buffer, TRANSLATIONS_SEPARATOR);
  while(translations[translations_size] != NULL && translations_size < sizeof(translations)){
    translations[++translations_size] = strtok(NULL, TRANSLATIONS_SEPARATOR);
  }

  // Priskiriami vertimai žodžiui
  Word word = empty_word();
  char *language, *translation;
  int i;
  for(i = 0; i < translations_size; i++){
    language = strtok(translations[i], LANGUAGE_TRANSLATION_SEPARATOR);
    translation = strtok(NULL, LANGUAGE_TRANSLATION_SEPARATOR);

    if(translation == NULL || language == NULL){
      continue;
    }

    if( strcmp(language, "en") == 0 ){
      strncpy(word.en, translation, MAX_TRANSLATION_SIZE);
    } else if ( strcmp(language, "lt") == 0 ){
      strncpy(word.lt, translation, MAX_TRANSLATION_SIZE);
    }
  }

  return word;
}

Word empty_word(){
  Word word;
  memset(word.en,0,strlen(word.en));
  memset(word.lt,0,strlen(word.lt));
  return word;
}
