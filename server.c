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

player* get_player( int sd ) {
  player* this = malloc(sizeof(player));
  char buffer[MESSAGE_BUFFER_SIZE];
  while(read( sd, buffer, sizeof(buffer) )) {
    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    this = &buffer;
    printf("%s joined game by [SERVER %d]\n", this->name, getpid() );
    break;
  }
  return this;
}

int run_game() {
  game G;
  int sd, connection;
  sd = server_setup(); 
  int connects = 0;
  int ready_to_play = 0;
  while (1) {
    char input[128];
    char* s;
    connection = server_connect( sd );
    connects++;
    int f = fork();
    if (f == 0) {
      close(sd);
      G.players[connects] = (player*)get_player( connection );
      sub_server( connection );
      exit(0);
    }
    else{
    close( connection );
    }
  }
}


int main() {
  return run_game();
}
