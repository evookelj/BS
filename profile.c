#include "profile.h"

char* fileLocation(char* name) {
  char* str = malloc(sizeof(char*));
  sprintf(str, ".profiles/.%s",name);
  return str;
}

char* create_profile(char* name) {
  umask(0000);
  
  if (strlen(name) > 30 || strlen(name) < 6) {
    return "Username must be from 6-30 characters";
  }

  profile* new = malloc(sizeof(profile));
  new->name = "hey";
  printf("name: %s\n", new->name);
  new->lies = 0;
  new->totalClaims = 0;
  new->wins = 0;
  new->losses = 0;

  char* loc = fileLocation(name);
  int fd = open(loc, O_WRONLY | O_CREAT | O_EXCL);
  if (fd<0) {
    return "Username already exists.";
  } else {
    int wr = write(fd, new, sizeof(profile));
    if (wr<0) {
      return "Account creation failed.";
    }
  }

  return "";
}

int main() {
  printf("%s\n", create_profile("emmavook"));
  return 0;
}
