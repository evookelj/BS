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

int main() {
  int sd, connection;
  sd = server_setup(); 
  //opening();
  int connects = 0;
  while (1) {
    char input[128];
    char* s;
    connection = server_connect( sd );
    connects++;
    if (connects>=3) {
      printf("Is this everyone?\n");
      int invalidInput = 1;
      while(invalidInput) {
	if (toupper(input[0]) == 'Y') {
	  invalidInput = 0;
	}
	else if(toupper(input[0]) == 'N') {
	  invalidInput = 0;
	}
	else {
	  s = "Invalid input. Please enter 'Y' for yes and 'N' for no: ";
	  printf("%s", s);
	  fgets(input, sizeof(input), stdin);
	}
      }
    }
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
