#include <string.h>
#include <stdio.h>

#define CREATE  1
#define SEARCH  2
#define INDEX   3
#define EXIT    4
#define SEARCH_FOR_EN_TEXT  5
#define SEARCH_FOR_LT_TEXT  6

#define COMMANDS_COUNT    6

#define clear() printf("\033[H\033[J")

typedef struct {
  int id;
  char translation[30];
} Command;

Command client_commands[COMMANDS_COUNT];

void init_commands();
int select_command(int *arr, int length);
Command find_by_id(int id);