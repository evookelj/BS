#include "networking.h"
#include "player.h"
#include "profile.h"
#include "server.h"

int main( int argc, char *argv[] ) {
  char* myName = login();
  char *host;
  if (argc != 2 ) {
    printf("host not specified, conneting to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];

  //Connect to host
  int sd;
  sd = client_connect( host );

  //Print out the rules need be
  rules();

  //Send player struct to server
  player *me = malloc(sizeof(player));
  me->type = 1;//human
  me->name = myName;
  printf("Sending player struct to server: %s\n", me->name);
  write( sd, me, sizeof(player));

  /*
  char buffer[MESSAGE_BUFFER_SIZE];
  while (1) {
    printf("enter message: ");
    fgets( buffer, sizeof(buffer), stdin );
    char *p = strchr(buffer, '\n');
    *p = 0;
  
    write( sd, buffer, sizeof(buffer) );
    read( sd, buffer, sizeof(buffer) );
    printf( "received: %s\n", buffer );
  }
  */
  return 0;
}
