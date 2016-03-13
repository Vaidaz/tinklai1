#include "information_get.h"


HASH get_information(int *options, int size){
  int i;
  HASH hash = new_hash();

  for(i = 0; i < size; i++){
    switch(options[i]) {
      case TRANSLATION_EN:
        printf("Iveskite en vertima: ");
        scanf("%s", hash.en);
        break;
      case TRANSLATION_LT:
        printf("Iveskite lt vertima: ");
        scanf("%s", hash.lt);
        break;
    }
  }

  return hash;
}
