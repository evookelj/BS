#include "profile.h"

char* file_location(char* name) {
  char* str = malloc(sizeof(char*));
  sprintf(str, ".profiles/.%s",name);
  return str;
}

char* create_profile(char* name) {
  umask(0000);
  
  if (strlen(name) > 30 || strlen(name) < 6) {
    return "Username must be from 6-30 characters";
  }

  profile new;
  new.lies = 0;
  new.total_claims = 0;
  new.wins = 0;
  new.losses = 0;

  char* loc = file_location(name);
  int fd = open(loc, O_WRONLY | O_CREAT | O_EXCL | O_APPEND, 0644);
  if (fd<0) {
    return "Username already exists.";
  } else {
    int wr = write(fd, &(new.lies), sizeof(int));
    if (wr<0) {
      return "Account creation failed. Try again";
    }
    wr = write(fd, &(new.total_claims), sizeof(int));
    if (wr<0) {
      return "Account creation failed. Try again";
    }
    wr = write(fd, &(new.wins), sizeof(int));
    if (wr<0) {
      return "Account creation failed. Try again";
    }
    wr = write(fd, &(new.losses), sizeof(int));
    if (wr<0) {
      return "Account creation failed. Try again";
    }
  }

  close(fd);

  return "";
}

profile* get_profile(char* name) {
  char* loc = file_location(name);
  profile* this = malloc(sizeof(profile));
  int fd = open(loc, O_RDONLY, 0644);
  if (fd<0) {
    return NULL;
  }
  int rd;
  rd = read(fd, &(this->lies), sizeof(int));
  if (rd<0) {
    return NULL;
  }
  rd = read(fd, &(this->total_claims), sizeof(int));
  if (rd<0) {
    return NULL;
  }
  rd = read(fd, &(this->wins), sizeof(int));
  if (rd<0) {
    return NULL;
  }
  rd = read(fd, &(this->losses), sizeof(int));
  if (rd<0) {
    return NULL;
  }
  close(fd);
  return this;
}

char* display_profile(char* name) {
  profile* this = get_profile(name);
  char* ret = malloc(sizeof(1, 10000));
  sprintf(ret, "Lies: %d\nTotal Claims: %d\nWins: %d\nLosses: %d\n\n",
	  this->lies, this->total_claims, this->wins, this->losses);
  free(this);
  return ret;
}

/*
int main() {
  char* usr = "emmavook";
  printf("%s\n", create_profile(usr));
  printf("%s\n",display_profile(usr));
  
  return 0;
}
*/
nt main( int argc, char *argv[] ) {

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
