#include <stdio.h>
#include <string.h>

typedef struct {
  char command[30];
  char lt[30];
  char en[30];
} HASH;

HASH new_hash();
void to_string(HASH hash, char *string, char length);
HASH to_hash(char *message);
