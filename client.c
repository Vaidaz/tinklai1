#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "command_select.h"
#include "information_get.h"

int main(){
  init_commands();

  int choice;
  int main_choices[] = { CREATE, SEARCH, INDEX, EXIT };

  int c[2] = {TRANSLATION_EN, TRANSLATION_LT};
  char inf[100];
  get_information(c, 2, inf);
  puts(inf);

  // while(1){
  while(0){
    choice = select_command(main_choices, 4);

    switch (choice){
      case CREATE:
        puts("Kurti");
        break;
      case SEARCH:
        puts("Ieškoti");
        break;
      case INDEX:
        puts("Rodyti sąrašą");
        break;
      case EXIT:
        puts("Išeiti");
        return 0;
    }
    getchar();
    getchar();
  };

  return 0;
}
