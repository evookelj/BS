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
  int fd = open(loc, O_WRONLY | O_CREAT | O_EXCL, 0644);
  if (fd<0) {
    return "Username already exists OR you don't have .profiles.";
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
  free(loc);
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
  free(loc);
  return this;
}

int update_bs_ratio(char* name, int plusLies, int plusTotal) {
  profile* this = get_profile(name);
  this->lies += plusLies;
  this->total_claims += plusTotal;
  char* loc = file_location(name);

  int fd = open(loc, O_WRONLY, 0644);
  if (fd<0) {
    printf("Error opening profile of %s\n",name);
    return -1;
  }
  int wr = write(fd, this, sizeof(profile));
  if (wr<0) {
    printf("Error writing to profile of %s\n", name);
    return -1;
  }
  printf("Successfully updated BS ratio!\n");
  return 0;
}

int update_wl_ratio(char* name, int isWin) {
  profile* this = get_profile(name);
  if (isWin) {
    this->wins += 1;
  } else {
    this->losses += 1;
  }

  char* loc = file_location(name);
  int fd = open(loc, O_WRONLY, 0644);
  if (fd<0) {
    printf("Error opening profile of %s\n",name);
    return -1;
  }
  int wr = write(fd, this, sizeof(profile));
  if (wr<0) {
    printf("Error writing to profile of %s\n", name);
    return -1;
  }
  printf("Successfully updated win/loss ratio!\n");
  free(loc);
  return 0;
}

char* display_profile(char* name) {
  profile* this = get_profile(name);
  if (this==NULL) {
    return "";
  }
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
  printf("%d\n", update_bs_ratio(usr, 3, 5));
  printf("%s\n",display_profile(usr));
  printf("%d\n", update_wl_ratio(usr, 0));
  printf("%s\n",display_profile(usr));
  return 0;
}
*/
