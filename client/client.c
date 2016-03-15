#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "command_select.h"
#include "information_get.h"
#include "connector.h"
#include "../common/response_codes.h"

#define PORT "3490"
#define HOST "127.0.0.1"

void send_command(int sockfd, char *command, char *data);
HASH receive_hash(int sockfd);

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
        HASH hash = get_information(input_options, 2);
        strcpy(hash.command, "create");
        to_string(hash, data, sizeof(data));
        send(sockfd, data, strlen(data), 0);
        hash = receive_hash(sockfd);
        if(hash.status == SUCCESS){
          puts("Sėkmingai išsaugota.");
        } else {
          puts("Įvyko klaida.");
        }
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
        HASH hash = get_information(input_options, 1);
        strcpy(hash.command, "search");
        to_string(hash, data, sizeof(data));
        send(sockfd, data, strlen(data), 0);

        break;
      }
      case INDEX:{
        char data[100];
        HASH hash = new_hash();
        strcpy(hash.command, "index");
        to_string(hash, data, sizeof(data));
        send(sockfd, data, strlen(data), 0);
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

void send_command(int sockfd, char *command, char *data){
  char message[100];
  strcpy(message, command);
  strcat(message, data);
  send(sockfd, message, strlen(message), 0);
}

HASH receive_hash(int sockfd){
  char buf[100];
  int nbytes;
  HASH hash;

  if ((nbytes = recv(sockfd, buf, sizeof buf, 0)) <= 0) {
    perror("recv");
  } else {
    hash = to_hash(buf);
  }

  return hash;
}
