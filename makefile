all: server client

server: server.o networking.o
	gcc -o server server.o networking.o

client: client.o networking.o player.o profile.o
	gcc -o client client.o networking.o player.o profile.o

server.o: server.c server.h networking.h
	gcc -c server.c

client.o: client.c networking.h
	gcc -c client.c

player.o: player.c player.h
	gcc -c player.c

profile.o: profile.c profile.h
	gcc -c profile.c

networking.o: networking.c networking.h
	gcc -c networking.c

clean:
	rm *.o
	rm *~
