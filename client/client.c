#include <stdio.h>
#include <string.h>
#include <unistd.h>

// #include "command_select.h"
#include "connector.h"
#include "../common/word.h"
#include "../common/statuses.h"

#define PORT "3490"
#define HOST "127.0.0.1"

// Select options
#define CREATE  1
#define SEARCH  2
#define DELETE  3
#define EXIT    4
#define SEARCH_FOR_EN_TEXT  5
#define SEARCH_FOR_LT_TEXT  6

// Read options
#define TRANSLATION_EN 1
#define TRANSLATION_LT 2

#define RECEIVE_ERROR -1

#define clear() printf("\033[H\033[J")

void send_command(int sockfd, char *command, char *data);

int select_command(int *arr, int length);
Word get_word_from_stdin(int *options, int size);
int receive_status(int sockfd);
int print_status(int status, Word *word);
int receive_status_and_word(int sockfd, Word *word);

int main(){
  int main_choice;
  int main_choices[4] = { CREATE, SEARCH, DELETE, EXIT };

  int sockfd;
  establish_connection(HOST, PORT, &sockfd);

  int run = 1;

  while(run){
    main_choice = select_command(main_choices, 4);

    switch (main_choice){
      case CREATE: {
        int input_options[2] = { TRANSLATION_EN, TRANSLATION_LT };
        char data[100], message[100];
        Word word =  get_word_from_stdin(input_options, 2);
        word_to_string(word, data, sizeof(data));
        send_command(sockfd, "create", data);
        int status = receive_status(sockfd);
        print_status(status, NULL);
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
        Word word =  get_word_from_stdin(input_options, 2);
        word_to_string(word, data, sizeof(data));
        send_command(sockfd, "search", data);
        word = empty_word();
        int status = receive_status_and_word(sockfd, &word);
        print_status(status, &word);
        break;
      }
      case DELETE:{
        break;
      }
      case EXIT:
        run = 0;
        break;
    }
    if(run){
      getchar();
      getchar();
    }
  };

  close(sockfd);
  return 0;
}

void send_command(int sockfd, char *command, char *data){
  char message[100];
  snprintf(message, sizeof(message), "%s|%s", command, data);
  send(sockfd, message, strlen(message), 0);
}

int select_command(int *options, int length){
  int i, choice, map[length];
  char user_input[2];

  clear();
  puts("\t\tŽodynėlis");
  puts("\tPasirinkite operaciją iš sąrašo.");
  puts("");
  for(i = 0; i < length; i++){
    switch(options[i]){
      case CREATE:
        map[i] = CREATE;
        printf("%d. Įdėti naują žodį.\n", i + 1);
        break;
      case SEARCH:
        map[i] = SEARCH;
        printf("%d. Ieškoti žodžio.\n", i + 1);
        break;
      case DELETE:
        map[i] = DELETE;
        printf("%d. Trinti žodį.\n", i + 1);
        break;
      case EXIT:
        map[i] = EXIT;
        printf("%d. Išeiti.\n", i + 1);
        break;
      case SEARCH_FOR_EN_TEXT:
        map[i] = SEARCH_FOR_EN_TEXT;
        printf("%d. Ieškoti en vertimo.\n", i + 1);
        break;
      case SEARCH_FOR_LT_TEXT:
        map[i] = SEARCH_FOR_LT_TEXT;
        printf("%d. Ieškoti lt vertimo.\n", i + 1);
        break;
    }
  }
  puts("");

  do {
    printf("Operacijos numeris: ");
    scanf("%s", user_input);
    choice = atoi(user_input) - 1;
  } while( choice >= i || choice < 0 );

  return map[choice];
}

Word get_word_from_stdin(int *options, int size){
  int i;
  Word word = empty_word();

  for(i = 0; i < size; i++){
    switch(options[i]) {
      case TRANSLATION_EN:
        printf("Iveskite en vertima: ");
        scanf("%s", word.en);
        break;
      case TRANSLATION_LT:
        printf("Iveskite lt vertima: ");
        scanf("%s", word.lt);
        break;
    }
  }

  return word;
}

int receive_status(int sockfd){
  char buf[100];
  int nbytes, status = RECEIVE_ERROR;

  if ((nbytes = recv(sockfd, buf, sizeof(buf), 0)) <= 0) {
    perror("recv");
  } else {
    char *status_string = strtok(buf, "|");
    status = atoi(status_string);
  }

  return status;
}

int print_status(int status, Word *word){
  switch(status){
    case TRANSLATION_CREATED:
      puts("Sėkmingai išsaugota.");
      break;
    case TRANSLATION_EXIST:
      puts("Vertimas jau egzistuoja.");
      break;
    case TRANSLATION_FOUND:
      puts("");
      puts("Vertimas rastas.");
      printf("EN reikšmė: %s\n", word->en);
      printf("LT reikšmė: %s\n", word->lt);
      break;
    case TRANSLATION_NOT_FOUND:
      puts("Vertimas nerastas.");
      break;
    default:
      puts("Įvyko klaida.");
      break;
  }
}

int receive_status_and_word(int sockfd, Word *word){
  char buf[100] = {0};
  int nbytes, status = RECEIVE_ERROR;

  if ((nbytes = recv(sockfd, buf, sizeof(buf), 0)) <= 0) {
    perror("recv");
  } else {
    char *status_string = strtok(buf, "|");
    char *word_string = strtok(NULL, "|");

    if(status_string != NULL){
      status = atoi(status_string);
    }

    if(word_string != NULL){
      *word = string_to_word(word_string);
    }
  }

  return status;
}

