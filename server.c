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
void run_BSing(int sd, game* curr_game, int accuser, int accused);
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
  for (i=0; i<this_player.num_cards; i++) {
    char add[20];
    char* type = this_player.hand[i].type;
    int val = this_player.hand[i].value;
    sprintf(add, "%d %s,", val, type);
    strcat(ret, add);
  }
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
    player temp;
    temp.num_cards = num_cards;
    int j;
    for (j=0; j<num_cards; j++) {
      temp.hand[j] = deck[i*num_cards+j];
    }
    curr_game->players[i] = temp;
  }
  
  int offset = (num_players-1)*num_cards + (num_cards);
  curr_game->pile = malloc(sizeof(card) * (52-offset));
  for (i=0; i<52-offset; i++) {
    printf("NEW i: %d\n", i+offset);
    curr_game->pile[i] = deck[i+offset];
  }
  curr_game->pile_size = 52-offset;

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
  char b[100];
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
      while(1) {
	write(connections[i], msg, 8);
	printf("Sent client [%d] '%s' \n", i, msg);
	read(connections[i], b, 4);
	if(b[0] == 'g') {
	  break;
	}
      }
      curr_val++;
      if (curr_val == 14) { curr_val = 1; }
      printf("Running turn of player %d\n", i);
      run_turn(i, curr_game, connections[i]);
      printf("Finished turn of player %d\n", i);

      for (p=0; p<num_players; p++) {
	printf("p: %d\n", p);
	if (connections[p] != connections[i]) {
	  run_BSing(connections[p], curr_game, p, i);
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

void remove_card(game* curr_game, int i, int newVal, char* newType) {
  int j;
  int found = 0;
  if (curr_game->players[i].num_cards==1 &&
      curr_game->players[i].hand[i].value == newVal &&
      !strcmp(curr_game->players[i].hand[i].type, newType)) {
    curr_game->players[i].hand->type = "";
    curr_game->players[i].hand->value = 0;
    return;
  }
  for (j=0; j<curr_game->players[i].num_cards; j++) {
    if (curr_game->players[i].hand[j].value == newVal && //same val and type
	!strcmp(curr_game->players[i].hand[j].type, newType) &&
	!found) { //hasn't already been found
      found = 1;
    }
    if (found && j<(curr_game->players[i].num_cards)-1) { //found + valid range
      //shift all values one to the right (starting at the one u found bc that needs to be overwritten
      curr_game->players[i].hand[j].value = curr_game->players[i].hand[j+1].value;
      curr_game->players[i].hand[j].type = curr_game->players[i].hand[j+1].type;
    }
  }
  curr_game->players[i].num_cards -= 1; //lower num_cards
  printf("Removing %d of %s\n", newVal, newType);
}

void after_turn(game* curr_game, int player_num, char** cards_played, int num_cards_played) {
  printf("Starting after turn!\n");
  card** played;
  int i;
  int placehold;
  for (i=0; i<num_cards_played; i++) { //for each card they've played

    char** split_card = split(cards_played[i], " ", &placehold, 0);

    //add card to pile
    int val = (int)strtol(split_card[0], (char**)NULL, 10);
    curr_game->pile[curr_game->pile_size].value = val;
    curr_game->pile[curr_game->pile_size].type = split_card[1];
    curr_game->pile_size++;
    curr_game->players[player_num].num_cards -= 1;

    //remove card from player's hand
    remove_card(curr_game, player_num, val, split_card[1]);
  }
}

void run_turn( int i, game* curr_game, int sd) {
  //Write deck to client
  printf("RUNNING TURN\n");
  int size;
  char* joined = str_hand(curr_game->players[i]);
  char buffer[8];
  int count;
  while (1) {
    write(sd, joined, (curr_game->players[i].num_cards)*20);
    printf("Trying to send deck...\n");
    read(sd, buffer, sizeof(buffer));
    if(buffer[0] == 'g') {
      printf("Recieved gotDeck\n");
      break;
    }
  }
  char** cards_played;
  char buff2[25*17];
  while (1) {
    printf("Reading for player's move...\n");
    read(sd, buff2, sizeof(buff2));
    if(buff2[0] == 'd') {
      int num_played = 0;
      cards_played = split(buff2, ",", &num_played, 1);
      int j;
      for (j=0; j<num_played; j++) {
	printf("played[%d]: %s\n", j, cards_played[j]);
      }
      after_turn(curr_game, i, cards_played, num_played);
      for (j=0; j<curr_game->pile_size; j++) {
	printf("pile[%d]: %d of %s\n", j, curr_game->pile[j].value, curr_game->pile[j].type);
      }
      break;
    }
  }
  free(cards_played);
  free(joined);
  
  printf("END TURN\n");
}

void add_card_server(game* curr_game, int player_num, char* newType, int newVal) {
  int sz = curr_game->players[player_num].num_cards;
  curr_game->players[player_num].hand[sz].value = newVal;
  strcpy(curr_game->players[player_num].hand[sz].type, newType);
}

void run_BSing(int sd, game* curr_game, int accuser, int accused) {
  char buffer[10];
  int i;
  
  while (1) {
    printf("Reading for BS result...\n");
    read(sd, buffer, sizeof(buffer)); //buffer = sign + num cards involved
    if(buffer[0]=='1') { //player accused correct, player who BSed takes pile
      for (i=0; i<curr_game->pile_size; i++) {
	add_card_server(curr_game, accused, curr_game->pile[i].type, curr_game->pile[i].value);
      }
    }
    if(buffer[0]=='0') { //player accused wrong, player takes pile
      for (i=0; i<curr_game->pile_size; i++) {
	add_card_server(curr_game, accuser, curr_game->pile[i].type, curr_game->pile[i].value);
      }
    }
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
