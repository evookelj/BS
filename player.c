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
  this_player->hand[this_player->num_cards] = *(to_add);
  this_player->num_cards++;
  return 0;
}

void print_hand(player* this_player) {
  int i;
  for (i=1; i<this_player->num_cards; i++) {
    printf("Hand at %d: %d of %s\n", i, this_player->hand[i].value, this_player->hand[i].type);
  }
}

char* get_fitting_cards(player* this_player, int curr_val) {
  char* ret = calloc(1, 100);
  int i;
  for (i=i; i<=this_player->num_cards; i++) {
    if (this_player->hand[i].value == curr_val) {
      char desc[30];
      sprintf(desc, "%d of %s\n", this_player->hand[i].value, this_player->hand[i].type);
      strcat(ret, desc);
    }
  }
  return ret;
}

//return 0 for BS'ing, 1 for truth'ing
int run_human_turn(player* this_player, int curr_val) {
  printf("The current value in play is %d. The cards you have that fit this are: \n%s\nPress enter to see your whole deck.\n", curr_val, get_fitting_cards(this_player, curr_val));
  char input[100];
  fgets(input, sizeof(input), stdin);
  printf("Your whole deck: \n");
  print_hand(this_player);
  printf("Press enter to continue.\n");
  fgets(input, sizeof(input), stdin);
  printf("Would you like to BS? (Y/y/N/n)\n");
  int invalidInput = 1;
  while(invalidInput) {
    fgets(input, sizeof(input), stdin);
    if(toupper(input[0]) == 'Y') {
      invalidInput = 0;
      printf("Run BS\n");
      //return 0;
      return run_BS(this_player, curr_val);
    } else if(toupper(input[0]) == 'N') {
      invalidInput = 0;
      printf("Run truth\n");
      return 0;
      //return run_truth(this_player, curr_val);
    }
    else {
      printf("Invalid input. Please try again (Y/y/N/n).\n");
    }
  }
  return 0;
}

int run_BS(player* this_player, int curr_val) {
  printf("To pick cards to put down, enter the index as listed in your printed deck (from 1 to %d) and press enter. Enter 'S/s' to stop after selecting at least one card.\n", this_player->num_cards-1);
  int cont = 1;
  int sel[this_player->num_cards];
  int count = 0;
  char input[20];
  while (cont) {
    fgets(input, sizeof(input), stdin);
    /*
    if (!strcmp(toupper(input[0]), "S")) {
      if (count < 1) {
	printf("You must select at least one card before stopping.\n");
      } else {
	printf("Great! You have selected to put down the following cards and claim them as %d's\n", curr_val);
	int i;
	for (i=0; i<count; i++) {
	  printf("%d of %s", this_player->hand[sel[i]].value, this_player->hand[sel[i]].type);
	}
	cont = 0;
      }
    }
    */
    printf("b\n");
    int ind = (int)strtol(input, (char **)NULL, 10);
    printf("ind: %d\n", ind);
    if (ind > 0 && ind < this_player->num_cards) {
      sel[count] = ind;
      count++;
    } else {
      printf("Invalid number. Try again (from 1 to %d) or enter S/s to stop.\n", this_player->num_cards-1);
    }
  }
  return 0;
}

int main() {
  player* emma = malloc(sizeof(player));
  emma->name = "emma";
  emma->num_cards = 0;
  int i;
  for (i=1; i<12; i++) {
    card* add = malloc(sizeof(card));
    add->type = "diamond";
    add->value = i;
    add_card(emma, add);
  }
  run_human_turn(emma, 7);
  return 0;
}
