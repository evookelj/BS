#include "server.h"

void process( char * s );
void sub_server( int sd );

void sub_server( int sd ) {
  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {
    printf("[SERVER %d] received: %s\n", getpid(), buffer );
    //process( buffer );
    write( sd, buffer, sizeof(buffer));    
  }
}

int update_game_status(int pid) {
  if (kill(pid, 0) == 0) //doesn't actually kill
    return 1;
  else
    return 0;
}

void print_IP( unsigned int queue[], int size ) {
  printf("IP Queue: ");
  int i;
  struct in_addr add;
  for ( i = 0; i < size; i++ ) {
    add.s_addr = (uint32_t) queue[i];
    printf("%s, ", inet_ntoa(add));
    //printf("%d, ", queue[i]);
  }
  printf("\n");
}

int get_players( int sd, unsigned int *ip_queue, int *queue_size, int timeout) {
  int connection = initial_server_connect(sd, &ip_queue[*queue_size], timeout);

  if ( connection == -1 ) {// only reached if timeout == 1 aka no new players joined
    close(connection);
    return -1;
  }

  /*
  char buffer[500];
  int port_buff = 3019;
  read( connection, &buffer, sizeof(buffer) );
  if ( strcmp(buffer, "client to listener") == 0 )
    write( connection, &port_buff, sizeof(int) ); // send port to listen on to client
  */
  close(connection);

  (*queue_size)++;
  print_IP(ip_queue, *queue_size);

  return 0;
}

int transfer_IPs( unsigned int *ip_queue, int *queue_size, unsigned int *player_IPs, int num_players ) {
  int i;
  // fill player_IPs w/ zeroes
  for ( i = 0; i < 4; i++ )
    player_IPs[i] = 0;
  // fill player_IPs with IPs FROM ip_queue
  for ( i = 0; i < num_players; i++ )
    player_IPs[i] = ip_queue[i];

  // remove transferred ips from ip_queue
  int j = num_players;
  for ( i = 0; i < *queue_size - num_players; i++ ) {
    ip_queue[i] = ip_queue[i+j];
  }
  // update queue_size
  (*queue_size) = (*queue_size) - num_players;
  
  return num_players;
}

int run_game(unsigned int *player_IPs, int num_players, int sd) {
  printf("Getting to gameplay\n");
  int sds[num_players]; //array to hold sds of clients
  //Connect to client
  int i;
  /*
  for(i = num_players, i > 0, i--) {

  }
  */
  char buffer[MESSAGE_BUFFER_SIZE];
  while (read( sd, buffer, sizeof(buffer) )) {
    printf("BUFFER: %s\n", buffer);
  }
  //While loop for gameplay
  return 0;
}

int main() {

  int sd, connection;
  unsigned int ip_queue[10];
  int queue_size = 0;
  int game_running; // 0 if no game running, 1 if there is a game running
  int game_pid;
  int num_play = 3;

  sd = server_setup();

  while(1) {

    if ( queue_size >= num_play && game_running == 0 ) { // ready to start game
      unsigned int player_IPs[queue_size];
      int num_players;
      num_players = transfer_IPs(ip_queue, &queue_size, player_IPs, queue_size);

      if ( game_running == 0 ) { // no game running
	game_running = 1;
	game_pid = fork();
	signal(SIGCHLD, SIG_IGN); // circumvents waiting
	if ( game_pid == 0 ) {  // child process
	  run_game(player_IPs, num_players, sd); // start game
	  printf("hi\n");
	  exit(0);
	}
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
