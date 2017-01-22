#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"
#include "server.h"

void process( char * s );
void sub_server( int sd );
void run_turn( int sd );
void run_BSing( int sd );

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
  return deck;
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

  //Insert for loop creating player and game structs
  game* curr_game = malloc(sizeof(game));
  curr_game->players = malloc(sizeof(player) * num_players);
  int num_cards = 52 / num_players;
  int num_extras = (52 % num_players);
  for (i=0; i<num_players; i++) {
    printf("\ni: %d\n", i);
    player temp;
    temp.num_cards = num_cards;
    printf("temp.num_cards: %d\n", temp.num_cards);
    printf("num_cards: %d\n", num_cards);
    int j;
    for (j=0; j<num_cards; j++) {
      temp.hand[j] = deck[i*num_cards+j];
      printf("hand[%d] = deck[%d]\n", j, i*num_cards+j);
    }
    curr_game->players[i] = temp;
  }
  
  int offset = (num_players-1)*num_cards + (num_cards);
  curr_game->pile = malloc(sizeof(card) * (52-offset));
  for (i=0; i<52-offset; i++) {
    printf("NEW i: %d\n", i+offset);
    curr_game->pile[i] = deck[i+offset];
  }

  //Play Game
  int player_count;
  while (1) {
    for(i = 0; i < 2; i++) {
      for(player_count = 0; player_count < 2; player_count++) {
	if(connections[player_count] == connections[i]) {
	  run_turn(connections[i]);
	}
	else {
	  run_BSing(connections[player_count]);
	}
      }     
    }
  }
  free(deck);
  return 0;
}


void run_turn( int sd ) {
  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));    
  }
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
