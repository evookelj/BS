#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "networking.h"
#include "player.h"
#include "server.h"

void run_human_turn_client(int curr_val, int sd, int* is_bs);

int main( int argc, char *argv[] ) {

  int f = fork();if (!f) {
    execlp("mkdir", "mkdir", ".profiles", (char*)0);
  }
  
  char *myName = login();
  printf("My name: %s\n", myName);

  char *host;
  if (argc != 2 ) {
    printf("host not specified, conneting to 127.0.0.1\n");
    host = "127.0.0.1";
  }
  else
    host = argv[1];
  
  int sd;

  int is_bs;

  sd = client_connect( host );

  char buffer[MESSAGE_BUFFER_SIZE];

  //Write name to server
  while(read(sd, buffer, sizeof(buffer))) {
    if(strcmp(buffer, "name") == 0) {
      write(sd, myName, 30);
      printf("Wrote name to server\n");
      break;
    }
  }

  int placeh;
  while(1) {
    read(sd, buffer, sizeof(buffer));
    
    int val = (int)strtol(buffer, (char**)NULL, 10);
    if(strcmp(buffer, "notTurn") == 0) {
      printf("It's not your turn!\n");
    } else if (val) {
      printf("It's your turn!\n");
      run_human_turn_client(val, sd, &is_bs);
    }
    if(buffer[0] == 'b') {
      printf("Got the BS\n");
      char** args = split(buffer, ",", &placeh, 0);
      int i;
      for (i=0; i<placeh; i++) {
	printf("Recieved[%d]: %s\n",i,args[i]);
      }

      int result = run_human_accuse(args[0],args[1],(int)strtol(args[2],(char**)NULL, 10), (int)strtol(args[3],(char**)NULL, 10), is_bs);
      printf("result: %d\n", result);
      char res[2];
      sprintf(res, "%d", result);
      write(sd, res, sizeof(res));
    }
  }

  char buff3[5];
  while(1) {
    read(sd, buff3, sizeof(buff3));
  }
  
  return 0;
}

void run_human_turn_client(int curr_val, int sd, int* is_bs) {
  //Get deck of cards
  char buffer[17*200];
  int rd;
  while (1) {
    write(sd, "got", 4);
    printf("READING FOR CARDS...\n");
    rd = read(sd, buffer, 17*200);
    if (buffer[0] == 'd') {//used so prog knows cards sending
      write(sd, "gotDeck", 8);
      printf("Sent 'gotDeck' to server\n");
      char* msg;
      msg = buffer;
      printf("Recieved: %s\n", msg);
      int size;
      char** hand_str = split(msg, ",", &size, 1);
      card** hand = malloc(sizeof(card*) * 17);
      int i;
      printf("size: %d\n", size);
      int placeholder;
      char** new;
      for (i=0; i<size; i++) {
	new = split(hand_str[i], " ", &placeholder, 0);
	int ind = (int)strtol(new[0], (char**)NULL, 10);
	hand[i] = malloc(sizeof(card));
	hand[i]->value = ind;
	hand[i]->type = new[1];
      }
      char* played = run_human_turn(hand, size, curr_val, is_bs);
      write(sd, played, size*17);
      printf("Sent played cards to server\n");
      
      free(played);
      free(hand);
      break;
    }
  }
  return;
}
