#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"

void process( char * s );
void sub_server( int sd );
void run_turn( int sd );
void run_BSing( int sd );


int main() {

  int sd, connection;

  sd = server_setup();

  //INSERT ASKING FOR NUMBER OF PLAYERS
  int expect_players = 2;
  int connections[expect_players];
  int num_players = 0;

  //Get players
  while (1) {

    connection = server_connect( sd );
    connections[num_players] = connection;
    num_players ++;

    /*
    int f = fork();
    if ( f == 0 ) {

      close(sd);
      sub_server( connection );

      exit(0);
    }
    else {
      close( connection );
    }
    */
    //close(sd);
    //close(connection);
    if (num_players == 2) {
      printf("GOT ALL PLAYERS\n");
      break;
    }
  }

  //Insert for loop creating player and game structs

  //Play Game
  int i;
  int player_count;
  while (1) {
    for(i = 0; i < 2; i++) {
      for(player_count = 0; player_count < 2; player_count++) {
	if(connections[player_count] == connections[i]) {
	  run_turn(connections[i]);
	}
	else {
	  run_BSing(connections[player_count]);
	}
      }     
    }
  }
  return 0;
}


void run_turn( int sd ) {
  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));    
  }
}

void run_BSing( int sd ) {
  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {

    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    process( buffer );
    write( sd, buffer, sizeof(buffer));    
  }
}

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
