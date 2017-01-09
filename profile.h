#ifndef PROFILEHEADER_H
#define PROFILEHEADER_H

struct profileStruct {
  char* name;
  int lies;
  int totalClaims;
  int wins;
  int losses;
};

typedef struct profileStruct profile;

//returns "" upon success, error msg upon failure
//creates hidden struct file in .profiles for each player profile
char* create_profile(char* name);

//updates current bs_ratio (lies/total)
int update_bs_ratio(int lies, int total);

//isWin=0 if loss, 1 if win for a game
//updates existing win/loss ratio
void update_wl_ratio(int isWin);

//returns string of profile info formatted
char* displayProfile(char* name);

#endif
