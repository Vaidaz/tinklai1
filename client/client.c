#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "command_select.h"
#include "information_get.h"

int main(){
  init_commands();

  int choice;
  int main_choices[4] = { CREATE, SEARCH, INDEX, EXIT };

  while(1){
    choice = select_command(main_choices, 4);

    switch (choice){
      case CREATE: {
        int input_options[2] = { TRANSLATION_EN, TRANSLATION_LT };
        char data[100];
        get_information(input_options, 2, data);
        printf("Į serverį bus išsiųsta: create%s \n", data);
        break;
      }
      case SEARCH:{
        puts("Ieškoti");
        break;
      }
      case INDEX:{
        puts("Rodyti sąrašą");
        break;
      }
      case EXIT:{
        puts("Išeiti");
        return 0;
      }
    }
    getchar();
    getchar();
  };

  return 0;
}
