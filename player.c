#include "player.h"
#include "profile.h"

void opening() {
  int validInput = 1; //1 means invalid user input, 0 means valid user input so move on
  char* s;
  char input[128];

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
      printf("%s", display_profile(input));
    }
    else if(toupper(input[0]) == 'N') {
      validInput = 0;
      s = "Please login\nUsername: ";
      printf("%s", s);
      fgets(input, sizeof(input), stdin);
      *strchr(input, '\n') = '\0';
      printf("%s", display_profile(input));
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
