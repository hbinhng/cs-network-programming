/**
 * Author: Hai-Binh Nguyen #20020189
 * server.c: This file implements the start point of the server application (startServer()).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "server.h"
#include "common.h"

int initializeServer()
{
  struct sockaddr_in serverAddress;

  int serverFd = 0;
  int opt = 1;

  if ((serverFd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
  {
    perror("Cannot create socket");
    exit(EXIT_FAILURE);
  }

  printf("Socket created successfully\n");

  if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
  {
    perror("Cannot set socket options failed");
    exit(EXIT_FAILURE);
  }

  printf("Socket options set successfully\n");

  serverAddress.sin_port = htons(PORT);
  serverAddress.sin_family = AF_INET;
  serverAddress.sin_addr.s_addr = INADDR_ANY;

  if (bind(serverFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
  {
    perror("Cannot bind to specified address");
    exit(EXIT_FAILURE);
  }

  printf("Server is bound to address successfully\n");

  if (listen(serverFd, 3) < 0)
  {
    perror("Cannot listen on specified address and port");
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on :%d\n", PORT);

  return serverFd;
}

int waitForClient(int serverFd)
{
  struct sockaddr_in clientAddress;
  int clientFd;
  int addressLength = sizeof(clientAddress);

  if ((clientFd = accept(serverFd, (struct sockaddr *)&clientAddress, (socklen_t *)&addressLength)) < 0)
  {
    perror("Cannot accept new client");
    exit(EXIT_FAILURE);
  }

  return clientFd;
}

int validName(char *content)
{
  int contentLength = strlen(content);

  for (int i = 0; i < contentLength; i++)
  {
    char c = content[i];

    if (c < 'A' || (c > 'Z' && c < 'a') || c > 'z')
      return 0;
  }

  return 1;
}

int validAge(char *content)
{
  int contentLength = strlen(content);

  for (int i = 0; i < contentLength; i++)
  {
    char c = content[i];

    if (c < '0' || c > '9')
      return 0;
  }

  return 1;
}

void startServer()
{
  int serverFd = initializeServer();
  int clientFd = 0;

  const int BUFSIZE = 1024;
  char inBuffer[BUFSIZE];
  char outBuffer[BUFSIZE];

  int sessionType = 0;
  int willQuit = 0;

  while (1)
  {
    if (clientFd == 0)
      clientFd = waitForClient(serverFd);

    memset(inBuffer, 0, BUFSIZE);
    memset(outBuffer, 0, BUFSIZE);

    read(clientFd, inBuffer, BUFSIZE);

    if (inBuffer[0] == 0)
    {
      printf("Client unexpectedly closed connection\n");
      clientFd = 0;
      continue;
    }

    char *content = trim(inBuffer);

    printf("Incoming message: \"%s\"\n", content);

    if (sessionType == 0 && !strcmp(content, "HELLO server"))
    {
      strcpy(outBuffer, "200 Hello Client");
    }
    else if (sessionType == 0 && !strcmp(content, "USER NAME"))
    {
      strcpy(outBuffer, "210 User name OK");
      sessionType = 1;
      printf("Entering username session\n");
    }
    else if (sessionType == 0 && !strcmp(content, "USER AGE"))
    {
      strcpy(outBuffer, "220 User age OK");
      sessionType = 2;
      printf("Entering user age session\n");
    }
    else if (sessionType == 0 && !strcmp(content, "QUIT"))
    {
      strcpy(outBuffer, "500 bye");
      willQuit = 1;
    }
    else if (sessionType == 1)
    {
      if (validName(content))
        sprintf(outBuffer, "211 User name %s OK", content);
      else
        sprintf(outBuffer, "410 User name error");

      sessionType = 0;
      printf("Leaving username session\n");
    }
    else if (sessionType == 2)
    {
      if (validAge(content))
        sprintf(outBuffer, "221 User age %s OK", content);
      else
        sprintf(outBuffer, "420 User age error");

      sessionType = 0;
      printf("Leaving user age session\n");
    }

    send(clientFd, outBuffer, strlen(outBuffer), 0);
    if (willQuit)
    {
      printf("Disconnecting client...\n");

      close(clientFd);
      willQuit = 0;
      clientFd = 0;
    }
  }
}