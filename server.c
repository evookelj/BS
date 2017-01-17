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

int update_game_status(int pid) {
  if (kill(pid, 0) == 0) //doesn't actually kill
    return 1;
  else
    return 0;
}

int get_players( int sd, unsigned int *ip_queue, int *queue_size) {
  int connection = server_connect(sd, ip_queue);
  return connection;
}

/*
player* get_player( int sd ) {
  player* this = malloc(sizeof(player));
  //char buffer[MESSAGE_BUFFER_SIZE];
  player* buffer;
  while(read( sd, buffer, sizeof(player) )) {
    printf("[SERVER %d] received: %d\n", getpid(), sizeof(*(buffer)));
    //printf("user: %s\n", buffer->name);
    //printf("%s joined game by [SERVER %d]\n", buffer->name, getpid() );
    *(this) = *(buffer);
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
      G.players[connects] = *((player*)get_player( connection ));
      sub_server( connection );
      exit(0);
    }
    else{
    close( connection );
    }
  }
}
*/
int main() {

  int sd, connection;
  unsigned int ip_queue[10];
  int queue_size = 0;
  int game_running; // 0 if no game running, 1 if there is a game running
  int game_pid;

  sd = server_setup();

  while(1) {

    if (queue_size >= 3 && game_running == 0) {
      //insert asking if 3 players in enough
      game_running = 1;
      game_pid = fork();
      signal(SIGCHLD, SIG_IGN); // doesn't wait
      if (game_pid == 0) { // child process
	//run_game(ip_queue, queue_size);
	exit(0);
      }
    }

    game_running = update_game_status(game_pid);
    if(queue_size < 3) { // not enough players to play game
      get_players(sd, ip_queue, &queue_size, 0); // listens without time outs
    }
    else if(game_running == 1) { // at least 3 players have already joined
      get_players(sd, ip_queue, &queue_size, 1); // listens with time outs
    }
    game_running = update_game_status(game_pid);

  }

  return 0;
}
