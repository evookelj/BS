#include "human_playing.h"

char* getFitting(player* thisPlayer, short currValue) {
  int i;
  while (thisPlayer->hand[i]) {
    card* this = thisPlayer->hand[i];
    char sval[3];
    sprintf(sval, "%d of %s", this->value, this->type);
    printf("%s\n", sval);
    i++;
  }
  return ":O";
}

int run_human_turn(player* thisPlayer, short currValue) {
  char snum[3];
  sprintf(snum, "%d", currValue);
  if (!strcmp(snum, "1")) {
    strcpy(snum,"A");
  }
  char input[4];
  printf("The current card value for play is %s and the cards you have that fit this are %s. What would you like to do: lie (L/l) or BS(B/b)?\n", snum, getFitting(thisPlayer, currValue));
  fgets(input, sizeof(input), stdin);
  
  return 0;
}

int main() {
  //ISSUE WITH CARD ADDING THAT I DONT HAVE
  //TIME TO FIX
  player* emma;
  int i;
  for (i=0; i<5; i++) {
    strcpy( emma->hand[i]->type, "heart");
    emma->hand[i]->value = i;
  }
  for (i=5; i++; i<17) {
    emma->hand[i] = NULL;
  }
  run_human_turn(emma, 1);
  return 0;
}
