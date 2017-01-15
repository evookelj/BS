#ifndef PLAYER_H
#define PLAYER_H

#include <stdio.h>
#include <ctype.h>

#include "profile.h"

//Starts game for client. Logins to their profile
profile* login();

//Prints the rules for the client need be.
void rules();

#endif
