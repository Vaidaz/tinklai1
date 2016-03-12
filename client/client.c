#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "command_select.h"
#include "information_get.h"
#include "connector.h"

#define PORT "3490"
#define HOST "127.0.0.1"

void send_command(int sockfd, char *command, char *data){
  char message[100];
  strcpy(message, command);
  strcat(message, data);
  send(sockfd, message, strlen(message), 0);
}

int main(){
  init_commands();

  int main_choice;
  int main_choices[4] = { CREATE, SEARCH, INDEX, EXIT };

  // int sockfd = connect_to_server();
  int sockfd;
  establish_connection(HOST, PORT, &sockfd);

  int run = 1;

  while(run){
    main_choice = select_command(main_choices, 4);

    switch (main_choice){
      case CREATE: {
        int input_options[2] = { TRANSLATION_EN, TRANSLATION_LT };
        char data[100], message[100];
        get_information(input_options, 2, data);
        send_command(sockfd, "create", data);
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
        send_command(sockfd, "search", data);
        break;
      }
      case INDEX:{
        send_command(sockfd, "index", "{}");
        break;
      }
      case EXIT:
        run = 0;
        break;
    }
    getchar();
    getchar();
  };

  close(sockfd);
  return 0;
}

