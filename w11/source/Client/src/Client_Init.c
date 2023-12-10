/**
 * Author: Hai Binh Nguyen #20020189
 * Client_Init.c: This file implements method to initialize
 * a connection to server (Client_Init).
 */

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

int Client_Init(char *serverHost, int port) {
  struct sockaddr_in serverAddress;
  int clientFd = socket(AF_INET, SOCK_STREAM, 0);

  if (clientFd == -1) {
    perror("Cannot create socket");
    exit(EXIT_FAILURE);
  }

  printf("Socket created successfully\n");

  serverAddress.sin_port = htons(port);
  serverAddress.sin_family = AF_INET;

  inet_pton(AF_INET, serverHost, &(serverAddress.sin_addr));

  if (connect(clientFd, (struct sockaddr *)&serverAddress,
              sizeof(serverAddress)) < 0) {
    perror("Cannot connect to server");
    exit(EXIT_FAILURE);
  }

  printf("Connected to server\n");

  return clientFd;
}