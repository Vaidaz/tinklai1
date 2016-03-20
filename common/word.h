#ifndef _word_h_included_
#define _word_h_included_

#include <stdio.h>
#include <string.h>

#define NUMBER_OF_TRANSLATIONS_IN_WORD 2
#define TRANSLATIONS_SEPARATOR ";"
#define LANGUAGE_TRANSLATION_SEPARATOR ":"

#define MAX_TRANSLATION_SIZE 30

typedef struct {
  char lt[MAX_TRANSLATION_SIZE];
  char en[MAX_TRANSLATION_SIZE];
} Word;

void word_to_string(Word translation, char *string, int string_size);
Word string_to_word(char *string);
Word empty_word();

#endif