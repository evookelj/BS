#include "server.h"

void process( char * s );
void sub_server( int sd );

void sub_server( int sd ) {
  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {
    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));    
  }
}

void process( char * s ) {
  while ( *s ) {
    *s = (*s - 'a' + 13) % 26 + 'a';
    s++;
  }
}



int run_game() {
  //opening(); commenting out so i can just test game play
  
  return -1;
}
/*
void start_game() {
  //create struct
  //add players
  //deal hands
}

int run_turn(player thisPlayer) {
  //send msg to corr player that its their turn
  //update msgs to other players
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

int run_turn(player* thisPlayer, short currValue) {
  int ret;
  if (thisPlayer->type) {
    ret = run_human_turn(thisPlayer, currValue);
  } else {
    ret = run_cpu_turn(thisPlayer, currValue);
  }
  return ret;
}

int cards_and_claim(player* thisPlayer) {
  int ret;
  if (thisPlayer->type) {
    ret = human_cards_and_claim(thisPlayer);
  } else {
    ret = cpu_cards_and_claim(thisPlayer);
  }
  return ret;
}

int accuse(player* accuser) {
  int ret;
  if (accuser->type) {
    ret = human_accuse(accuser);
  } else {
    ret = cpu_accuse(accuser);
  }
  return ret;
}

void give_cards(player* giveTo, card* cards) {
}
*/
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
      //create_profile(input);
      s = "Password: ";
      printf("%s", s);
      fgets(input, sizeof(input), stdin);
    }
    else if(toupper(input[0]) == 'N') {
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

int main() {
  int sd, connection;
  sd = server_setup(); 
  opening();
  while (1) {
    connection = server_connect( sd );
    int f = fork();
    if ( f == 0 ) {
      close(sd);
      sub_server( connection );
      exit(0);
    } else {
      close( connection );
    }
  }
  return 0;
}
