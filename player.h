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

#endif
