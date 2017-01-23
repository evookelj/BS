#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"
#include "player.h"

void run_human_turn_client(int curr_val, int sd);

char ** split(char * str, char * delim, int * size, int skip_first) {
  //effectively split str by delim
  char ** ret = (char **) malloc(1000);
  char *t = str;

  int i = 0;
  int first = skip_first;
  while ( t != NULL ) {
    if (first) {
      strsep(&t, delim);
      first = 0;
    } else {
      ret[i] = strsep(&t, delim);
      i++;
    }
  }
  *(size) = i-1;
  ret[i] = 0; //null term for both exec and cmd parsing
  return ret;
}

int main( int argc, char *argv[] ) {
  char *myName = login();
  printf("My name: %s\n", myName);

  char *host;
  if (argc != 2 ) {
    printf("host not specified, conneting to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];
  
  int sd;

  sd = client_connect( host );

  char buffer[MESSAGE_BUFFER_SIZE];

  //Write name to server
  while(read(sd, buffer, sizeof(buffer))) {
    if(strcmp(buffer, "name") == 0) {
      write(sd, myName, 30);
      printf("Wrote name to server\n");
      break;
    }
  }

  while(1) {
    read(sd, buffer, sizeof(buffer));

    int val = (int)strtol(buffer, (char**)NULL, 10);
    if(strcmp(buffer, "notTurn") == 0) {
      printf("It's not your turn!\n");
    } else if (val) {
      printf("It's your turn!\n");
      run_human_turn_client(val, sd);
    }
  }
  
  while (1) {
    printf("enter message: ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;
  
    write( sd, buffer, sizeof(buffer) );
    read( sd, buffer, sizeof(buffer) );
    printf( "received: %s\n", buffer );
  }
  
  return 0;
}

void run_human_turn_client(int curr_val, int sd) {
  //Get deck of cards
  char buffer[17*200];
  int rd = read(sd, buffer, 17*200);
  if (buffer[0] == 'd') {//used so prog knows cards sending
    write(sd, "gotDeck", 8);
    printf("Sent 'gotDeck' to server\n");
    char* msg;
    msg = buffer;
    printf("Recieved: %s\n", msg);
    int size;
    char** hand_str = split(msg, ",", &size, 1);
    card** hand = malloc(sizeof(card*) * 17);
    int i;
    printf("size: %d\n", size);
    int placeholder;
    char** new;
    for (i=0; i<size; i++) {
      new = split(hand_str[i], " ", &placeholder, 0);
      int ind = (int)strtol(new[0], (char**)NULL, 10);
      hand[i] = malloc(sizeof(card));
      hand[i]->value = ind;
      hand[i]->type = new[1];
    }
    char* played = run_human_turn(hand, size, curr_val);
    printf("played: %s\n", played);
    free(played);
    free(hand);
  }
  else {
    printf("Trying to get deck...\n");
    run_human_turn_client(curr_val, sd);
  }
}
