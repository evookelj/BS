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
  strncpy(new.name,name, sizeof(new.name)-1);
  new.lies = 0;
  new.total_claims = 0;
  new.wins = 0;
  new.losses = 0;

  char* loc = file_location(name);
  int fd = open(loc, O_WRONLY | O_CREAT | O_EXCL | O_APPEND, 0644);
  if (fd<0) {
    return "Username already exists.";
  } else {
    int wr = write(fd, &(new.name), 30);
    if (wr<0) {
      return "Account creation failed. Try again";
    }
    wr = write(fd, &(new.lies), sizeof(int));
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
  rd = read(fd, &(this->name), 30);
  if (rd<0) {
    return NULL;
  }
  this->name[30] = '\0';
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
  //below causes error when account wasn't just
  //created for some reason
  printf("name: %d\nn", this->name);
  close(fd);
  return this;
}

char* display_profile(char* name) {
  profile* this = get_profile(name);
  char* ret = malloc(sizeof(1, 10000));
  sprintf(ret, "Name: %d\nLies: %d\nTotal Claims: %d\nWins: %d\nLosses: %d\n\n",
	  this->name, this->lies, this->total_claims, this->wins, this->losses);
  free(this);
  return ret;
}

int main() {
  char* usr = "emmavook";
  printf("%s\n", create_profile(usr));
  printf("%s\n",display_profile(usr));
  
  return 0;
}
