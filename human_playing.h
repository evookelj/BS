#ifndef HUMANHEADER_H
#define HUMANHEADER_H

#include "server.h"

//str listing of cards player has that fit current value
//so they know what options are upon turn
char* getFitting(player* thisPlayer, short currValue);

int run_human_turn(player* thisPlayer, short currValue);

int human_cards_and_claim(player* thisPlayer);

int human_accuse(player* thisPlayer);

#endif
