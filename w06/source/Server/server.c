/**
 * Author: Hai-Binh Nguyen #20020189
 * server.c: This file implements server side application.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "server.h"
#include "../common.h"

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

void handleDownload(int clientFd, char *fileName)
{
  if (access(fileName, F_OK) == -1)
  {
    sendMessage(clientFd, "404 FILE not found");
    return;
  }

  sendMessage(clientFd, "201 FILE found;");

  FILE *file = fopen(fileName, "r");

  fseek(file, 0, SEEK_END);
  size_t fileSize = ftell(file);

  printf("Reading file \"%s\" (%lu bytes)\n", fileName, fileSize);

  fseek(file, 0, SEEK_SET);

  const int fbufsiz = 1024;
  char *buffer = (char *)malloc(fbufsiz);
  int readBytes;

  memset(buffer, 0, fbufsiz);

  while (readBytes = fread(buffer, 1, fbufsiz, file))
    write(clientFd, buffer, readBytes);

  write(clientFd, "<<EOF>>", 7);

  printf("File transfer completed\n");

  free(buffer);
  fclose(file);
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
  int passedthru = 0;

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
      sessionType = 0;
      willQuit = 0;
      passedthru = 0;
      clientFd = 0;
      continue;
    }

    char *content = trim(inBuffer);

    printf("Incoming message: \"%s\"\n", content);

    if (sessionType == 0 && !strcmp(content, "DOWNLOAD"))
    {
      sessionType = 1;
      strcpy(outBuffer, "200 DOWNLOAD OK");
      printf("Entering download session\n");
    }
    else if (sessionType == 0 && !strcmp(content, "QUIT"))
    {
      strcpy(outBuffer, "500 bye");
      willQuit = 1;
    }
    else if (sessionType == 1)
    {
      printf("Client has requested \"%s\" file\n", content);
      handleDownload(clientFd, content);
      sessionType = 0;
      passedthru = 1;
    }
    else
    {
      strcpy(outBuffer, "400 UNKNOWN command");
      printf("Client has sent an unknown command\n");
    }

    if (passedthru)
      continue;

    sendMessage(clientFd, outBuffer);
    if (willQuit)
    {
      printf("Disconnecting client...\n");

      close(clientFd);
      willQuit = 0;
      clientFd = 0;
    }
  }
}

#ifndef DEBUG
int main()
{
  startServer();
}
#endif // DEBUG