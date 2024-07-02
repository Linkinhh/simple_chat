#ifndef SOCKET_UTIL
#define SOCKET_UTIL

#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <malloc.h>

struct sockaddr_in* enderecoIP(const char *ip, int port);

int socketTCP();

#endif