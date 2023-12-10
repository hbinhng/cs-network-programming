/**
 * Author: Hai-Binh Nguyen #20020189
 * client.c: This file implements the start point of the client application (startClient()).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "common.h"
#include "client.h"

const int BUFSIZE = 1024;
const char *fail = "FAIL";
const char *ok = "OK";

int initializeClient()
{
  struct sockaddr_in serverAddress;
  int clientFd = socket(AF_INET, SOCK_STREAM, 0);

  if (clientFd == -1)
  {
    perror("Cannot create socket");
    exit(EXIT_FAILURE);
  }

  printf("Socket created successfully\n");

  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_family = AF_INET;

  inet_pton(AF_INET, "127.0.0.1", &(serverAddress.sin_addr));

  if (connect(clientFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
  {
    perror("Cannot connect to server");
    exit(EXIT_FAILURE);
  }

  printf("Connected to server\n");

  return clientFd;
}

void clean(char *buffer)
{
  memset(buffer, 0, BUFSIZE);
}

void sendMessage(int clientFd, char *content)
{
  send(clientFd, content, strlen(content), 0);
}

void assert(char *label, char *expected, char *received)
{
  printf("\n\nTesting \"%s\":\n", label);

  const char *result = strcmp(expected, received) == 0 ? ok : fail;

  printf("Expected: \"%s\" - Received: \"%s\" -> %s\n", expected, received, result);
}

void try(int clientFd, char *buffer, char *test, char *message, char *expectedResponse)
{
  clean(buffer);

  sendMessage(clientFd, message);

  read(clientFd, buffer, BUFSIZE);

  assert(test, expectedResponse, buffer);
}

void startClient()
{
  int clientFd = initializeClient();

  char buffer[BUFSIZE];

  try(clientFd, buffer, "Hello Server", "HELLO server", "200 Hello Client");
  try(clientFd, buffer, "Init username session", "USER NAME", "210 User name OK");
  try(clientFd, buffer, "Invalid username", "abc123", "410 User name error");
  try(clientFd, buffer, "Init username session", "USER NAME", "210 User name OK");
  try(clientFd, buffer, "Valid username", "Hoang", "211 User name Hoang OK");
  try(clientFd, buffer, "Init user age session", "USER AGE", "220 User age OK");
  try(clientFd, buffer, "Invalid user age", "abc123", "420 User age error");
  try(clientFd, buffer, "Init user age session", "USER AGE", "220 User age OK");
  try(clientFd, buffer, "Valid user age", "20", "221 User age 20 OK");
  try(clientFd, buffer, "Quit", "QUIT", "500 bye");
}