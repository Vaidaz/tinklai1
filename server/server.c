#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "db.h"
#include "../common/hash.h"
#include "../common/statuses.h"

#define PORT "3490"
#define BACKLOG 3

void *get_in_addr(struct sockaddr *sa);
int initialize_server(char *port);
void handle_incomming_connection(int listener, int *fdmax, fd_set *main_set);
void handle_incomming_data(int sockfd, fd_set *main_set);
void respond_with_hash(int sockfd, HASH hash);

int main(void){
  int listener = initialize_server("3490");

  fd_set main_set; // pagrindinis, kuriame viskas saugoma
  fd_set read_set; // selektui, kuris bus iškraipomas
  int fdmax;

  FD_SET(listener, &main_set);
  fdmax = listener;
  int i;

  for(;;) {
    read_set = main_set;
    if (select(fdmax+1, &read_set, NULL, NULL, NULL) == -1) {
      perror("select");
      exit(4);
    }

    // pereinam per egzistuojančius prisijungimus ir tikrinam
    for(i = 0; i <= fdmax; i++) {
      if (FD_ISSET(i, &read_set)) { // nauji duomenys
        if (i == listener) {
          handle_incomming_connection(listener, &fdmax, &main_set);
        } else {
          handle_incomming_data(i, &main_set);
        }
      }
    }
  }

  // TODO: uždaryti socketus
  return 0;
}

int initialize_server(char *port){
  int status, yes = 1;
  struct addrinfo hints, *servinfo;
  int sockfd;

  memset(&hints, 0, sizeof(hints));   // užtikrinam, kad struktūra yra švari
  hints.ai_family = AF_UNSPEC;        // IPv4 arba IPv6, (AF_INET, AF_INET6, AF_UNSPEC)
  hints.ai_socktype = SOCK_STREAM;    // stream socket o ne dgram
  hints.ai_flags = AI_PASSIVE;        // nurodo mano IP

  // getaddrinfo();
  if ((status = getaddrinfo(NULL, port, &hints, &servinfo))) {    // pirmu argumentu siunèiamas DNS arba IP
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    return -1;
  }

  // socket();
  if((sockfd = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1){
    printf("Could not create socket");
  }

  // perpanaudojam adresą, jei jis jau panaudotas
  if(-1 == setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))){
    return -1;
  }

  // bind();
  if(bind(sockfd, servinfo->ai_addr, servinfo->ai_addrlen) == -1){
    perror("bind");
    close( sockfd );
    return -1;
  }

  freeaddrinfo(servinfo);

  // listen();
  if(listen(sockfd, BACKLOG) == -1){
    printf("listen failed\n");
    close( sockfd );
    return -1;
  }

  return sockfd;
}

void handle_incomming_connection(int listener, int *fdmax, fd_set *main_set){
  socklen_t addrlen;
  struct sockaddr_storage remoteaddr; // kliento adresas
  int newfd;
  char remoteIP[INET6_ADDRSTRLEN];

  addrlen = sizeof remoteaddr;
  newfd = accept(listener, (struct sockaddr *)&remoteaddr, &addrlen);

  if (newfd == -1) {
    perror("accept");
  } else {
    FD_SET(newfd, main_set); // įdedam į pagrindinį sąrašą
    if (newfd > *fdmax) {    // saugom didžiausią deskriptorių
      *fdmax = newfd;
    }
    printf("selectserver: new connection from %s on "
      "socket %d\n",
      inet_ntop(remoteaddr.ss_family,
        get_in_addr((struct sockaddr*)&remoteaddr),
        remoteIP, INET6_ADDRSTRLEN),
      newfd);
  }
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa){
  if (sa->sa_family == AF_INET) {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

void handle_incomming_data(int sockfd, fd_set *main_set){
  char buf[100] = {0};
  int nbytes;
  HASH hash;
  FILE *db;
  int status_code;

  if ((nbytes = recv(sockfd, buf, sizeof buf, 0)) <= 0) {
    if (nbytes == 0) {
      // uždarytas prisijungimas
      printf("selectserver: socket %d hung up\n", sockfd);
    } else {
      // klaida
      perror("recv");
    }
    close(sockfd);
    FD_CLR(sockfd, main_set); // paralinam į pagrindinio sąrašo
  } else {
    hash = to_hash(buf);
    db_connect(&db);

    if (strcmp(hash.command, "create") == 0){
      db_add(db, hash);
      HASH hash = new_hash();
      hash.status = TRANSLATION_CREATED;
      respond_with_hash(sockfd, hash);
    } else if (strcmp(hash.command, "search") == 0){
      // TODO paieska
      HASH translation_hash;
      db_find(db, hash, &translation_hash);
      respond_with_hash(sockfd, translation_hash);
    } else if (strcmp(hash.command, "index") == 0){
      // TODO sarasas
      puts("sarasas");
    } else {
      printf("Neatpazinta komanda: %s\n", hash.command);
    }

    db_close(db);
  }

}

void respond_with_hash(int sockfd, HASH hash){
  char data[100];
  to_string(hash, data, sizeof(data));
  send(sockfd, data, strlen(data), 0);
}
