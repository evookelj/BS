#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"
#include "server.h"
#include "player.h"

void process( char * s );
void sub_server( int sd );
//void run_turn( int sd );
void run_BSing( int sd );
char* get_names( int sd );
//int run_human_turn_server(player* this_player, int curr_val);
char* get_hand(player *this_player);

int shuffleDeck(card* deck) {
  srand(time(NULL));
  int count = 52;
  int i;
  for (i=0; i<52; i++) {
    int r = rand() % count;
    card curr = deck[i];
    deck[i] = deck[r];
    deck[r] = curr;
  }
  return 0;
}

int createDeck(card* deck) {

  char *suits[] = {
    "heart",
    "diamond",
    "spade",
    "club"
  };
  
  int i, j;
  for (i=0; i<4; i++) {
    for (j=1; j<14; j++) {
      card temp;
      temp.type = suits[i];
      temp.value = (j%14);
      deck[i*13+j-1] = temp;
    }
  }
  shuffleDeck(deck);
  return 0;
}

char* str_hand(player this_player) {
  int i;
  char* ret = malloc(this_player.num_cards * 20);
  strcpy(ret, "d,");
  printf("DECK MADE\n");
  for (i=0; i<this_player.num_cards; i++) {
    char add[20];
    char* type = this_player.hand[i].type;
    int val = this_player.hand[i].value;
    sprintf(add, "%d %s,", val, type);
    printf("adding\n");
    strcat(ret, add);
    printf("added\n");
  }
  printf("Done joining\n");
  //printf("Hand: %s", ret);
  return ret;
}

int main() {

  int sd, connection;

  sd = server_setup();

  //INSERT ASKING FOR NUMBER OF PLAYERS
  int expect_players = 3;
  int connections[expect_players];
  int num_players = 0;

  int i;
  card* deck = malloc(sizeof(card) * 52);
  printf("before\n");
  createDeck(deck);

  //Get players
  while (1) {

    connection = server_connect( sd );
    connections[num_players] = connection;
    num_players ++;

    if (num_players == expect_players) {
      close(sd);
      printf("GOT ALL PLAYERS\n");
      break;
    }
  }

  //Create game struct and player structs
  //Deals out cards to players
  game* curr_game = malloc(sizeof(game));
  curr_game->players = malloc(sizeof(player) * num_players);
  int num_cards = 52 / num_players;
  int num_extras = (52 % num_players);
  for (i=0; i<num_players; i++) {
    //printf("\ni: %d\n", i);
    player temp;
    temp.num_cards = num_cards;
    //printf("temp.num_cards: %d\n", temp.num_cards);
    //printf("num_cards: %d\n", num_cards);
    int j;
    for (j=0; j<num_cards; j++) {
      temp.hand[j] = deck[i*num_cards+j];
      //printf("hand[%d] = deck[%d]\n", j, i*num_cards+j);
    }
    curr_game->players[i] = temp;
  }
  
  int offset = (num_players-1)*num_cards + (num_cards);
  curr_game->pile = malloc(sizeof(card) * (52-offset));
  for (i=0; i<52-offset; i++) {
    printf("NEW i: %d\n", i+offset);
    curr_game->pile[i] = deck[i+offset];
  }

  //Get names of players from clients
  for(i = 0; i<num_players; i++) {
    printf("Getting name for player [%d]\n", i);
    write(connections[i], "name", 5);
    char* n = get_names(connections[i]);
    printf("Name: %s\n", n);
    curr_game->players[i].name = n;
    printf("Player [%d]: %s\n", i, curr_game->players[i].name);
    free(n);
  }

  //Play Game
  int player_count;
  int p;
  int curr_val = 1;
  while (1) {
    for(i = 0; i < num_players; i++) {
      for (p=0; p<num_players; p++) {
	if(connections[p] != connections[i]) {
	  //Send notification to clients that it is not their turn
	  write(connections[p], "notTurn", 8);
	  printf("Sent client [%d] 'notTurn'\n", p);
	}
      }

      //if(connections[p] == connections[i]) {
      char curr[3];
      sprintf(curr, "%d", curr_val);
      char* msg = curr;
      write(connections[i], msg, 8);
      printf("Sent client [%d] '%s' \n", i, msg);
      curr_val++;
      if (curr_val == 14) { curr_val = 1; }
      run_turn(i, curr_game, connections[i]);

      for (p=0; p<num_players; p++) {
	if (connections[p] != connections[i]) {
	  run_BSing(connections[p]);
	}
      }
    }
  }
  free(deck);
  free(curr_game->players);
  free(curr_game->pile);
  free(curr_game);
  return 0;
}

char* get_names( int sd ) {
  char *buffer = malloc(125);
  while (read( sd, buffer, sizeof(buffer) )) {
    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    return buffer;
  }
  return "";
}

void run_turn( int i, game* curr_game, int sd) {

  int size;
  char* joined = str_hand(curr_game->players[i]);
  write(sd, joined, (curr_game->players[i].num_cards)*200);
  printf("Send cards to client\n");
  //free(joined);
}

void run_BSing( int sd ) {

  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));    
  }
}

void sub_server( int sd ) {

  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));    
  }
  
}
void process( char * s ) {

  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}
/*
int run_human_turn_server(player* this_player, int curr_val, int sd) {

  write(sd, get_hand(this_player), 128);

}


card* get_hand(player *this_player) {
  int i;
  card *hand = malloc(sizeof(card) * 52);
  hand = this_player->hand;
  return hand;
}


int run_human_turn_server(player* this_player, int curr_val) {
  printf("\nThe current value in play is %d. The cards you have that fit this are: \n", curr_val);
  //int count = get_fitting_cards(this_player, curr_val);
  int count = 0;
  int* fitting = get_fitting(this_player, curr_val, &count);
  int play_count;
  if (count==0) {
    printf("You have no choice but to BS, as you have no cards of value %d.\n", curr_val);
    play_count = run_BS(this_player, curr_val);
  } else {
    char input[100];
    printf("\nYour whole deck: \n");
    print_hand(this_player);
    printf("Press enter to continue.\n");
    fgets(input, sizeof(input), stdin);
    printf("Would you like to BS? (Y/y/N/n)\n");
    int ans = ask_yn();
    if(ans) {
      play_count = run_BS(this_player, curr_val);
      return play_count;
    } else {
      return run_truth_turn(this_player, count, curr_val, fitting);
    }
  }
  return 0;
}
*/
