#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "command_select.h"
#include "information_get.h"

#define PER_PAGE 10

int main(){
  init_commands();

  int main_choice;
  int main_choices[4] = { CREATE, SEARCH, INDEX, EXIT };

  while(1){
    main_choice = select_command(main_choices, 4);

    switch (main_choice){
      case CREATE: {
        int input_options[2] = { TRANSLATION_EN, TRANSLATION_LT };
        char data[100];
        get_information(input_options, 2, data);
        printf("Į serverį bus išsiųsta: create%s \n", data);
        break;
      }
      case SEARCH:{
        int choices[2] = { SEARCH_FOR_EN_TEXT, SEARCH_FOR_LT_TEXT };
        int choice = select_command(choices, 2);
        int input_options[1];

        if(choice == SEARCH_FOR_EN_TEXT){
          input_options[0] = TRANSLATION_EN;
        } else {
          input_options[0] = TRANSLATION_LT;
        }

        char data[100];
        get_information(input_options, 1, data);
        printf("Į serverį bus išsiųsta: search%s \n", data);

        break;
      }
      case INDEX:{
        printf("Į serverį bus išsiųsta: index{}\n");
        break;
      }
      case EXIT:{
        return 0;
      }
    }
    getchar();
    getchar();
  };

  return 0;
}
