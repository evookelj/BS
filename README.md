# BS
Systems Final Project (BS): Emma Vukelj and Grace Stempel

## List of Files in the Repo
* DESIGN.txt
* devlog.txt
* makefile
* client.c
* networking.c
* networking.h
* player.c
* player.h
* profile.c
* profile.h
* server.c
* server.h

## Rules of BS
The rules of BS are simple. The game requires at least three players and it uses a standard deck of cards with no Jokers. The deck of cards will be dealt out evenly, any remainders will be placed in the pile of cards. The first player must put down at least one Ace, or fake it by putting differnt cards. The next player puts down a Two, the third player a Three, etc. After every turn, the other players can question the validity of cards placed down. If the player is caught bluffing, they must pick up the entire discarded pile and add it to their hand. If the player was not bluffing, the player who challenged them recieves the cards in the pile. The  goal is to get rid of all your cards.


## How to Play
* Make
* Run ./server
* Run ./client on a different computer with the IP address of machine running server as the first command line argument
  * Can be run locally if no command line arguments are inputed

## Notes
* Game will only work with three players
* Not all functionality was implemented
* Full, in terminal game exists in commented out main function of player.c