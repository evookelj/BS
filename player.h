#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <ctype.h>

#include "profile.h"
#include "server.h"

struct fitting_cards_struct {
  //card** fitting;
  int count;
};

typedef struct fitting_cards_struct fitting_cards;

//Starts game for client. Logins to their profile
char* login();

//Prints the rules for the client need be.
void rules();

#endif
