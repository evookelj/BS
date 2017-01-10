#ifndef PROFILEHEADER_H
#define PROFILEHEADER_H

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

struct profileStruct {
  char* name;
  int lies;
  int total_claims;
  int wins;
  int losses;
};

typedef struct profileStruct profile;

//helper function to take username and sprintf it
//into correct format to find hidden file holding struct
char* file_location(char name[30]);

//returns "" upon success, error msg upon failure
//creates hidden struct file in .profiles for each player profile
char* create_profile(char* name);

//returns profile of player name
profile* get_profile(char name[30]);

//updates current bs_ratio (lies/total)
int update_bs_ratio(char name[30], int lies, int total);

//isWin=0 if loss, 1 if win for a game
//updates existing win/loss ratio
void update_wl_ratio(char name[30], int isWin);

//returns string of profile info formatted
char* display_profile(char name[30]);

#endif
