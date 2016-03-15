#include "hash.h"
#include "../common/response_codes.h"

HASH new_hash(){
  HASH hash;
  hash.status = NO_STATUS;
  memset(hash.command, 0, sizeof(hash.command));
  memset(hash.lt, 0, sizeof(hash.lt));
  memset(hash.en, 0, sizeof(hash.en));
  return hash;
}

void to_string(HASH hash, char *string, char length){
  char buffer[100] = {0};
  if(strlen(hash.command) != 0){
    sprintf(buffer, "command:%s;", hash.command);
  }
  if(strlen(hash.lt) != 0){
    sprintf(buffer + strlen(buffer), "lt:%s;", hash.lt);
  }
  if(strlen(hash.en) != 0){
    sprintf(buffer + strlen(buffer), "en:%s;", hash.en);
  }
  if(hash.status != NO_STATUS){
    sprintf(buffer + strlen(buffer), "status:%d;", hash.status);
  }
  strncpy(string, buffer, length);
}

HASH to_hash(char *message){
  HASH hash;

  memset(hash.command, 0, sizeof(hash.command));
  memset(hash.lt, 0, sizeof(hash.lt));
  memset(hash.en, 0, sizeof(hash.en));

  char message_buffer[50] = {0};
  strcpy(message_buffer, message);

  // Suskaldau eilutę į k.v. poras pagal ';'
  int max_pairs = 3, pairs_count = 0;
  char *pairs[max_pairs];
  pairs[pairs_count] = strtok(message_buffer, ";");
  while(pairs[pairs_count] != NULL && pairs_count < max_pairs){
    pairs[++pairs_count] = strtok(NULL, ";");
  }

  // Poras suskaidau pagal ':' ir gaunu k.v.
  int i;
  char *key;
  char *value;
  for(i = 0; i < pairs_count; i++){
    key = strtok(pairs[i], ":");
    value = strtok(NULL, ":");

    if( strcmp(key, "lt") == 0 ){
      strncpy(hash.lt, value, sizeof(hash.lt));
    } else if( strcmp(key, "en") == 0 ){
      strncpy(hash.en, value, sizeof(hash.en));
    } else if( strcmp(key, "command") == 0 ){
      strncpy(hash.command, value, sizeof(hash.command));
    } else if( strcmp(key, "status") == 0 ){
      hash.status = atoi(value);
    }
  }

  return hash;
}
