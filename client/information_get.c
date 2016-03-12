#include "information_get.h"


void get_information(int *options, int size, char *result){
  int i;
  int answers_counter = 0;
  char answers[2][ANSWERS_SIZE];
  char user_input[ANSWERS_SIZE];

  for(i = 0; i < size; i++){
    switch(options[i]) {
      case TRANSLATION_EN:
        printf("Iveskite en vertima: ");
        scanf("%s", user_input);
        sprintf(answers[answers_counter], "%s:%s", "translation_en", user_input);
        answers_counter++;
        break;
      case TRANSLATION_LT:
        printf("Iveskite lt vertima: ");
        scanf("%s", user_input);
        sprintf(answers[answers_counter], "%s:%s", "translation_lt", user_input);
        answers_counter++;
        break;
    }
  }

  strcpy(result, "{");
  for(i = 0; i < answers_counter; i++){
    if(i != 0){
      strcat(result, ",");
    }
    strcat(result, answers[i]);
  }
  strcat(result, "}");
}
