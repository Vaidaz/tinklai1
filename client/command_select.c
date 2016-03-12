#include "command_select.h"

void init_commands(){
  client_commands[0].id = CREATE;
  strcpy(client_commands[0].translation, "Įdėti naują žodį.");

  client_commands[1].id = SEARCH;
  strcpy(client_commands[1].translation, "Ieškoti žodžio.");

  client_commands[2].id = INDEX;
  strcpy(client_commands[2].translation, "Rodyti žodžius.");

  client_commands[3].id = EXIT;
  strcpy(client_commands[3].translation, "Išeiti.");

  client_commands[4].id = SEARCH_FOR_EN_TEXT;
  strcpy(client_commands[4].translation, "Ieškoti žodžio angliškai.");

  client_commands[5].id = SEARCH_FOR_LT_TEXT;
  strcpy(client_commands[5].translation, "Ieškoti žodžio lietuviškai.");
}

int select_command(int *arr, int length){
  int i, choice;
  char user_input[2];
  Command command;

  clear();
  puts("\t\tŽodynėlis");
  puts("\tPasirinkite operaciją iš sąrašo.");
  puts("");
  for(i = 0; i < length; i++){
    command = find_by_id(arr[i]);
    printf("%d. %s\n", i + 1, command.translation);
  }
  puts("");

  do {
    printf("Operacijos numeris: ");
    scanf("%s", user_input);
    choice = atoi(user_input) - 1;
  } while( choice >= i || choice < 0 );

  command = find_by_id(arr[choice]);
  return command.id;
}

Command find_by_id(int id){
  int i;
  for(i = 0; i < COMMANDS_COUNT; i++){
    if(client_commands[i].id == id){
      return client_commands[i];
    }
  }
}