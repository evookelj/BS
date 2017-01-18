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

int is_not_dup(int sel[17], int num_cards, int input) {
  int i;
  for (i=0; i<num_cards; i++) {
    if (sel[i] == input) {
      return 0;
    }
  }
  return 1;
}

int run_BS(player* this_player, int curr_val) {
  printf("To pick cards to put down, enter the index as listed in your printed deck (from 1 to %d) and press enter. Enter 'S/s' to stop after selecting at least one card.\n", this_player->num_cards-1);
  int cont = 1;
  int sel[this_player->num_cards];
  int count = 0;
  char input[20];
  while (cont) {
    fgets(input, sizeof(input), stdin);
    int ind = (int)strtol(input, (char **)NULL, 10);
    printf("ind: %d\n", ind);
    if (ind > 0 && ind < this_player->num_cards) {
      if (is_not_dup(sel, this_player->num_cards, ind)) {
	sel[count] = ind;
	count++;
      } else {
	printf("This index has already been selected. Try again.\n");
      }
    } else if (ind==0) {
      printf("this: %s\n", input);
      if (ind==0 && toupper(input[0]) == 'S') {
	if (count < 1) {
	  printf("You must select at least one card before stopping.\n");
	} else {
	  printf("Great! You have selected to put down the following cards and claim them as %d's.\n", curr_val);
	  int i;
	  for (i=0; i<count; i++) {
	    printf("%d of %s\n", this_player->hand[sel[i]].value, this_player->hand[sel[i]].type);
	  }
	  cont = 0;
	}
      } else {
	printf("Invalid input. Try again (from 1 to %d) or enter S/s to stop.\n", this_player->num_cards-1);
      }
    }
  }
  return count;
}

card* get_fitting(player* this_player, int curr_val) {
  card* ret = calloc(sizeof(card), this_player->num_cards);
  int i;
  int count = 0;
  for (i=0; i<this_player->num_cards; i++) {
    if (this_player->hand[i].value == curr_val) {
      printf("index %d: %d of %s\n", count+1, this_player->hand[i].value, this_player->hand[i].type);
      ret[count] = this_player->hand[i];
      count++;
    }
  }
  if (count==0) {
    printf("None\n");
  }
  return ret;
}

int run_truth_turn(player* this_player, int count, int curr_val) {
  char input[30];
  if (count==1) {
    printf("You only have one card of value %d, so you must play that. Press enter to continue.\n", curr_val);
    fgets(input, sizeof(input), stdin);
  } else {
    printf("else\n");
  }
  return 0;
}

//return 0 for BS'ing, 1 for truth'ing
int run_human_turn(player* this_player, int curr_val) {
  printf("The current value in play is %d. The cards you have that fit this are: \n", curr_val);
  //int count = get_fitting_cards(this_player, curr_val);
  int count = 0;
  card* fitting = get_fitting(this_player, curr_val);
  if (fitting[0].value != 0) {
    count = 2;
  }
  int play_count;
  if (count==0) {
    printf("You have no choice but to BS, as you have no cards of value %d.\n", curr_val);
    play_count = run_BS(this_player, curr_val);
  } else {
    printf("\nPress enter to see your whole deck.\n");
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
	play_count = run_BS(this_player, curr_val);
	return play_count;
      } else if(toupper(input[0]) == 'N') {
	invalidInput = 0;
	return run_truth_turn(this_player, count, curr_val);
      }
      else {
	printf("Invalid input. Please try again (Y/y/N/n).\n");
      }
    }
  }
  return 0;
}
/*
int main() {
  player* emma = malloc(sizeof(player));
  emma->name = "emma";
  emma->num_cards = 0;
  int i;
  for (i=1; i<4; i++) {
    card* add = malloc(sizeof(card));
    add->type = "diamond";
    add->value = i;
    add_card(emma, add);
  }
  for (i=1; i<4; i++) {
    card* add = malloc(sizeof(card));
    add->type = "heart";
    add->value = i;
    add_card(emma, add);
  }
  for (i=3; i<7; i++) {
    card* add = malloc(sizeof(card));
    add->type = "spade";
    add->value = i;
    add_card(emma, add);
  }
  run_human_turn(emma, 4);
  return 0;
}
*/
