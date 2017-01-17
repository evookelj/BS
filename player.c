#include "player.h"
#include "profile.h"

/*
//returns username once valid one is gotten from player
char* login() {
  int invalidInput = 1; //1 means invalid user input, 0 means valid user input so move on
  char* s;
  char input[128];
  char* this = calloc(1,30);

  s = "Hello, and welcome to BS. Are you new to the game and need to create a profile?(Y/N): ";
  printf("%s", s);
  fgets(input, sizeof(input), stdin);
  while(invalidInput) {
    if (toupper(input[0]) == 'Y') {
      int invalidUser = 1;
      while (invalidUser) {
	s = "Username: ";
	printf("%s", s);
	fgets(input, sizeof(input), stdin);
	*strchr(input, '\n') = '\0';
	printf("Name: %s\n", input);
	char* resp = create_profile(input);
	printf("Resp: %s\n",resp);
	if (strlen(resp)) {
	  printf("%s. Try a different username.\n",resp);
	} else {
	  if (get_profile(input) != NULL) {
	    strcpy(this, input);
	    printf("%s", display_profile(input));
	    invalidInput = 0;
	    invalidUser = 0;
	  }
	  else {
	    printf("Error getting profile for %s. Try again or try different username.\n", input);
	  }
	}
      }
    }
    else if(toupper(input[0]) == 'N') {
      s = "Please login\nUsername: ";
      printf("%s", s);
      fgets(input, sizeof(input), stdin);
      *strchr(input, '\n') = '\0';
      char* display = display_profile(input);
      if (!strlen(display)) {
	printf("Username doesn't exist. Try again\n");
	fgets(input, sizeof(input), stdin);
      } else {
	printf("%s", display);
	strcpy(this, input);
	invalidInput = 0;
      }
    }
    else {
      s = "Invalid input. Please enter 'Y' for yes and 'N' for no: ";
      printf("%s", s);
      fgets(input, sizeof(input), stdin);
    }
  }
  return this;
}

void rules() {
  int validInput = 1;
  char* s;
  char input[128];

  s = "Do you need a refresher of the rules?(Y/N): ";
  printf("%s", s);
  fgets(input, sizeof(input), stdin);
  while(validInput) {
    if(toupper(input[0]) == 'Y') {
      validInput = 0;
      s = "The rules of BS are simple.\nThe game requires at least three players and it uses a standard deck of cards with no Jokers.\nThe deck of cards will be dealt out evenly, any remainders will be placed in the pile of cards.\nThe first player must put down at least one Ace, or fake it by putting differnt cards.\nThe next player puts down a Two, the third player a Three, etc.\nAfter every turn, the other players can question the validity of cards placed down.\nIf the player is caught bluffing, they must pick up the entire discarded pile and add it to their hand.\nIf the player was not bluffing, the player who challenged them recieves the cards in the pile.\nYour goal is to get rid of all your cards\n";
      printf("%s", s);
    }
    else if(toupper(input[0]) == 'N') {
      validInput = 0;
    }
    else {
      s = "Invalid input. Please enter 'Y' for yes and 'N' for no: ";
      printf("%s", s);
      fgets(input, sizeof(input), stdin);
    }
  }
}
*/

int add_card(player* this_player, card* to_add) {
  //this_player->hand = calloc(sizeof(card), 17);
  this_player->hand[this_player->num_cards] = *(to_add);
  this_player->num_cards++;
  return 0;
}

void print_hand(player* this_player) {
  int i;
  for (i=0; i<this_player->num_cards; i++) {
    printf("%s's hand at %d: %d of %s\n", this_player->name, i, this_player->hand[i].value, this_player->hand[i].type);
  }
}

int main() {
  player* emma = malloc(sizeof(player));
  emma->name = "emma";
  emma->num_cards = 0;
  card* add = malloc(sizeof(card));
  add->type = "diamond";
  add->value = 4;
  add_card(emma, add);
  print_hand(emma);
  return 0;
}
