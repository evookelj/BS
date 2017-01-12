#include "human_playing.h"

int run_human_turn(player thisPlayer, short currValue) {
  char snum[3];
  sprintf(snum, "%d", currValue);
  if (!strcmp(snum, "1")) {
    strcpy(snum,"A");
  }
  printf("The current card value for paly is %s. What would you like to do?\n", snum);
  return 0;
}

int main() {
  player emma;
  run_human_turn(emma, 1);
}
