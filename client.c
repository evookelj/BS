#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"
#include "player.h"

void run_human_turn_client(int curr_val, int sd);

char ** split(char * str, char * delim, int * size) {
  //effectively split str by delim
  printf("start split\n");
  char ** ret = (char **) malloc(1000);
  char *t = str;

  int i = 0;
  int first = 1;
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

  printf("Finish splitting\n");
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
  printf("\nThe current value in play is %d. The cards you have that fit this are: \n", curr_val);

  int size = 17*200;
  char buffer[size];
  while(1) {
    read(sd, buffer, size);
    if (buffer[0] == 'd') { //used so prog knows cards sending
      printf("Getting cards\n");
      char* msg;
      msg = buffer;
      printf("Recieved: %s\n", msg);
      int size;
      char** hand = split(msg, ",", &size);
      int i;
      printf("size: %d\n", size);
      for (i=0; i<size; i++) {
	printf("hand[%d]: %s\n", i, hand[i]);
      }
      free(hand);
      break;
    }
  }
}

/*
card hand(int sd) {
  char buffer[MESSAGE_BUFFER_SIZE];
  while(read( sd, buffer, sizeof(buffer) ){
      return buffer;
*/
