#ifndef GAMEHEADER_H
#define GAMEHEADER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#include "profile.h"
#include "networking.h"
#include "player.h"

//=====================STRUCTS=====================
struct card_struct {
  char* type; //"heart", "diamond", "club", or "spade"
  short value; //A=1 b/c value doesn't rlly matter for BS
};

typedef struct card_struct card;

struct player_struct {
  unsigned short type; //0=CPU, 1=HUMAN
  char* name;
  card hand[17];
  int num_cards;
};

typedef struct player_struct player;

struct game_struct {
  player* players;
  card* pile;
};

typedef struct game_struct game;
//=====================STRUCTS=====================

//overall function to run the game
//returns 0 if game completed, -1 if game is cut short
int run_game();

//Runs opeing sequence for game
void opening();

//single player starts the game
//waits for all intended players to connect
void start_game();

//deals cards at start of game
//returns number of cards per player
int deal_cards();

//gets and returns number of players
int get_players();

//get and create human players (used in getPlayers)
//returns pointer to human players created
player* get_humans();

//run this player's turn
//return 0 for success, -1 if something wrong
void run_turn(int sd);

//put down cards and make claim
//to be used by runTurn's thisPlayer
//returns 0 if BSing, 1 if truthing
int cards_and_claim(player thisPlayer);

//accuser accuses current player of BS'ing
//1 if correct, 0 if wrong
int accuse(player accuser);

//give player giveTo these cards
//used when wrong accuser acquires discard pile
void give_cards(player giveTo, card* cards);

#endif
