#include "game.h"

int run_game() {
  opening();
  return -1;
}

void opening() {
  int validInput = 1; //1 means invalid user input, 0 means valid user input so move on
  char* s;
  char input[128];

  s = "Hello, and welcome to BS. Are you new to the game and need to create a profile?(Y/N): ";
  printf("%s", s);
  fgets(input, sizeof(input), stdin);
  while(validInput) {
    if (strncmp(input, "Y", 1) == 0) {
      validInput = 0;
      s = "Username: ";
      printf("%s", s);
      fgets(input, sizeof(input), stdin);
      //create_profile(input);
      s = "Password: ";
      printf("%s", s);
      fgets(input, sizeof(input), stdin);
    }
    else if(strncmp(input, "N", 1) == 0) {
      validInput = 0;
      s = "Please login\nUsername: ";
      printf("%s", s);
      fgets(input, sizeof(input), stdin);
      s = "Password: ";
      printf("%s", s);
      fgets(input, sizeof(input), stdin);
    }
    else {
      s = "Invalid input. Please enter 'Y' for yes and 'N' for no: ";
      printf("%s", s);
      fgets(input, sizeof(input), stdin);
    }
  }

  s = "Do you need a refresher of the rules?(Y/N): ";
  //start_game();
}

void start_game() {

}

int deal_cards() {
  return -1;
}

int get_players() {
  return -1;
}

player* get_humans() {
  player* new = malloc(sizeof(player));
  return new;
}

int run_turn(player thisPlayer) {
  return -1;
}

int cars_and_claim(player thisPlayer) {
  return -1;
}

int accuse(player accuser) {
  return -1;
}

void give_cards(player giveTo, card* cards) {
}

int main() {
  run_game();
  return 1;
}
