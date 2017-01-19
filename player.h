#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <ctype.h>

#include "profile.h"
#include "server.h"

//Starts game for client. Logins to their profile
char* login();

//Prints the rules for the client need be.
void rules();

//used to get answer via stdin to y/n question
//returns 1 if yes, 0 if no
int ask_yn();

#endif
