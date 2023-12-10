/**
 * Author: Hai Binh Nguyen #20020189
 * Server_Init.c: This file implements Server structure
 * initalizer (Server_Init), include binding and listening.
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

#include "Server.h"

Server *Server_Init(char *host, int port, int capacity) {
  Server *server = (Server *)malloc(sizeof(Server));

  server->Users = (User **)malloc(capacity * sizeof(User *));
  server->ConnectionThreads =
      (pthread_t **)malloc(capacity * sizeof(pthread_t *));
  server->Capacity = capacity;
  server->Allocated = 0;

  if (inet_pton(AF_INET, host, &(server->Address.sin_addr)) == 0) {
    perror("Invalid address\n");
    exit(EXIT_FAILURE);
  }

  server->SockFd = socket(AF_INET, SOCK_STREAM, 0);

  if (server->SockFd == 0) {
    perror("Cannot create socket\n");
    exit(EXIT_FAILURE);
  }

  printf("Socket created successfully\n");

  int opt = 1;

  if (setsockopt(server->SockFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
    perror("Cannot set socket options failed\n");
    exit(EXIT_FAILURE);
  }

  printf("Socket options set successfully\n");

  server->Address.sin_port = htons(port);
  server->Address.sin_family = AF_INET;

  if (bind(server->SockFd, (struct sockaddr *)&server->Address,
           sizeof(server->Address)) < 0) {
    perror("Cannot bind to specified address\n");
    exit(EXIT_FAILURE);
  }

  printf("Server is bound to address successfully\n");

  if (listen(server->SockFd, 3) < 0) {
    perror("Cannot listen on specified address and port\n");
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on %s:%d\n", host, port);

  return server;
}