#include "player.h"
#include "profile.h"

profile* login() {
  int validInput = 1; //1 means invalid user input, 0 means valid user input so move on
  char* s;
  char input[128];
  profile* this;

  s = "Hello, and welcome to BS. Are you new to the game and need to create a profile?(Y/N): ";
  printf("%s", s);
  fgets(input, sizeof(input), stdin);
  while(validInput) {
    if (toupper(input[0]) == 'Y') {
      validInput = 0;
      s = "Username: ";
      printf("%s", s);
      fgets(input, sizeof(input), stdin);
      *strchr(input, '\n') = '\0';
      printf("Name: %s\n", input);
      create_profile(input);
      this = get_profile(input);
      this->name = input;
      printf("%s", display_profile(input));
    }
    else if(toupper(input[0]) == 'N') {
      validInput = 0;
      s = "Please login\nUsername: ";
      printf("%s", s);
      fgets(input, sizeof(input), stdin);
      *strchr(input, '\n') = '\0';
      printf("%s", display_profile(input));
      this  = get_profile(input);
      this->name = input;
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
