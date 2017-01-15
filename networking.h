#ifndef NETWORKING_H
#define NETWORKING_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MESSAGE_BUFFER_SIZE 1000

void error_check( int i, char *s );

int server_setup();

int server_connect(int sd);

int client_connect( char *host );

#endif
