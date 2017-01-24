#include "player.h"

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
  //printf("My name is %s\n", this);
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

int add_card(card** hand, int * size, card* to_add) {
  hand[*(size)] = to_add;
  *(size) += 1;
  return 0;
}

void print_hand(card** hand, int size) {
  int i;
  for (i=0; i<size; i++) {
    printf("Hand at %d: %d of %s\n", i+1, hand[i]->value, hand[i]->type);
  }
}

char* str_hand(card** hand, int size) {
  int i;
  char* ret = "DECK,";
  for (i=0; i<size; i++) {
    char add[10];
    printf("hand mem: %d\n", hand[i]->value);
    printf("hand[%d]: %d %s\n", i, hand[i]->value, hand[i]->type);
    sprintf(add, "%d %s", hand[i]->value, hand[i]->type);
    strcat(ret, add);
  }
  return ret;
}

int is_not_dup(int sel[17], int num_cards, int input) {
  int i;
  for (i=0; i<num_cards; i++) {
    if (sel[i] == input) {
      //printf("repeat of %d\n", sel[i]);
      return 0;
    }
  }
  return 1;
}

char* run_BS(card** hand, int size, int curr_val) {
  printf("To pick cards to put down, enter the index as listed in your printed deck (from 1 to %d) and press enter. Enter 'S/s' to stop after selecting at least one card.\n", size);
  int cont = 1;
  int sel[size];
  int i;
  for (i=0; i<size; i++) {
    sel[i] = 18;
  }
  int count = 0;
  char input[20];
  char* ret = malloc(size*15);
  ret[0] = 'd';
  ret[1] = ',';
  while (cont) {
    fgets(input, sizeof(input), stdin);
    int ind = (int)strtol(input, (char **)NULL, 10);
    printf("ind: %d\n", ind);
    if (ind > 0 && ind < size) {
      ind -= 1;
      if (is_not_dup(sel, size, ind)) {
	sel[count] = ind;
	count++;
      } else {
	printf("This index has already been selected. Try again.\n");
      }
    } else if (ind==0) {
      if (ind==0 && toupper(input[0]) == 'S') {
	if (count < 1) {
	  printf("You must select at least one card before stopping.\n");
	} else {
	  printf("Great! You have selected to put down the following cards and claim them as %d's.\n", curr_val);
	  for (i=0; i<count; i++) {
	    printf("%d of %s\n", hand[sel[i]]->value, hand[sel[i]]->type);
	    char add[20];
	    sprintf(add, "%d %s,", hand[sel[i]]->value, hand[sel[i]]->type);
	    strcat(ret, add);
	  }
	  cont = 0;
	}
      } else {
	printf("Invalid input. Try again (from 1 to %d) or enter S/s to stop.\n", size);
      }
    }
  }
  return ret;
}

int* get_fitting(card** hand, int size, int curr_val, int* count) {
  int* ret = malloc(sizeof(int) * 4);
  int i;
  int cnt = 0;
  for (i=0; i<size; i++) {
    if (hand[i]->value == curr_val) {
      printf("index %d: %d of %s\n", cnt+1, hand[i]->value, hand[i]->type);
      ret[cnt] = i;
      cnt++;
    }
  }
  if (cnt==0) {
    printf("None\n");
  }
  *(count) = cnt;
  return ret;
}

char* run_truth_turn(card** hand, int size, int count, int curr_val, int* fitting) {
  char input[30];
  char* ret = malloc(size*15);
  ret[0] = 'd';
  ret[1] = ',';
  if (count==1) {
    printf("You only have one card of value %d, so you must play that. Press enter to continue.\n", curr_val);
    fgets(input, sizeof(input), stdin);
    char add[20];
    sprintf(add, "%d %s,", hand[fitting[0]]->value, hand[fitting[0]]->type);
    strcat(ret, add);
  } else {
    printf("To pick cards to put down, enter the index as listed in your printed deck of fitting cards (from 1 to %d) and press enter. Enter 'S/s' to stop after selecting at least one card.\n", count);
    int cont = 1;
    int sel[size];
    int cntSel = 0;
    char input[20];
    while (cont) {
      fgets(input, sizeof(input), stdin);
      int ind = (int)strtol(input, (char **)NULL, 10);
      if (ind > 0 && ind <= count) {
	ind -= 1;
	if (is_not_dup(sel, cntSel, fitting[ind])) {
	  printf("hand[%d]: %d of %s\n\n", fitting[ind]+1, hand[fitting[ind]]->value, hand[fitting[ind]]->type);
	  sel[cntSel] = fitting[ind];
	  cntSel++;
	  char add[20];
	  sprintf(add, "%d %s,", hand[fitting[ind]]->value, hand[fitting[ind]]->type);
	  strcat(ret, add);
	  if (cntSel==count) {
	    printf("You have selected all possible choices. On we go!\n");
	    cont = 0;
	  }
	} else {
	  printf("This index has already been selected. Try again.\n");
	}
      } else if (ind==0) {
	if (ind==0 && toupper(input[0]) == 'S') {
	  if (cntSel < 1) {
	    printf("You must select at least one card before stopping.\n");
	  } else {
	    printf("Great! You have selected to put down the following cards:\n");
	    int i;
	    for (i=0; i<cntSel; i++) {
	      printf("%d of %s\n", hand[sel[i]]->value, hand[sel[i]]->type);
	    }
	    cont = 0;
	  }
	}
      } else {
	printf("Invalid input. Try again (from 1 to %d) or enter S/s to stop.\n",count);
      }
    }
  }
  return ret;
}

int ask_yn() {
  char input[100];
  int invalidInput = 1;
  while(invalidInput) {
    fgets(input, sizeof(input), stdin);
    if(toupper(input[0]) == 'Y') {
      return 1;
    } else if(toupper(input[0]) == 'N') {
      return 0;
    } else {
      printf("Invalid input. Please try again (Y/y/N/n).\n");
    }
  }
  return 0;
}

//return 0 for BS'ing, 1 for truth'ing
char* run_human_turn(card** hand, int size, int curr_val, int* is_bs) {
  int ans;
  printf("\nThe current value in play is %d. The cards you have that fit this are: \n", curr_val);
  int count = 0;
  int* fitting = get_fitting(hand, size, curr_val, &count);
  if (count==0) {
    printf("You have no choice but to BS, as you have no cards of value %d.\n", curr_val);
    print_hand(hand, size);
    return run_BS(hand, size, curr_val);
  } else {
    char input[100];
    printf("\nYour whole deck: \n");
    print_hand(hand, size);
    printf("Press enter to continue.\n");
    fgets(input, sizeof(input), stdin);
    printf("Would you like to BS? (Y/y/N/n)\n");
    ans = ask_yn();
    if(ans) {
      return run_BS(hand, size, curr_val);
    } else {
      return run_truth_turn(hand, size, count, curr_val, fitting);
    }
  }
  *(is_bs) = ans;
  return 0;
}

int run_human_accuse(char* this_player, char* last_player, int num_cards_from_turn, int curr_val, int is_bs) {
  printf("Would you like to accuse %s of BS'ing their %d %d's? (Y/y/N/n)\n", last_player, num_cards_from_turn, curr_val);
  int to_accuse = ask_yn();
  if (to_accuse) {
    printf("Okay, you have accused %s of BS'ing aaaaand... (Enter to continue)\n", last_player);
    char input[2];
    int i;
    fgets(input, sizeof(input), stdin);
    if (is_bs) {
      printf("You were correct! %s now has to take the pile.\n", last_player);
      return 2;
    } else {
      printf("You were wrong :( and now have to take the pile.\n");
      return 1;
    }
  } 
  printf("Okay, moving on then...\n");
  return 0;
}
