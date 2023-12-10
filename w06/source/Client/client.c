/**
 * Author: Hai-Binh Nguyen #20020189
 * client.c: This file implement client side application.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "client.h"
#include "../common.h"

void downloadFile(int clientFd, char *fileName, char *localFilePath)
{
  printf("Download %s to local %s\n", fileName, localFilePath);

  const int BUFSIZE = 1024;
  char buffer[BUFSIZE];

  sendMessage(clientFd, "DOWNLOAD");

  read(clientFd, buffer, BUFSIZE);

  sendMessage(clientFd, fileName);
  memset(buffer, 0, BUFSIZE);

  int isFileContent = 0;
  int readBytes;
  int isEof = 0;

  FILE *output = fopen(localFilePath, "w");

  while (readBytes = read(clientFd, buffer, BUFSIZE))
  {
    if (readBytes < 0)
      break;

    if (!isFileContent && (buffer, "201 FILE found;"))
      isFileContent = 1;
    else if (isFileContent)
    {
      if (!strcmp(buffer + readBytes - 7, "<<EOF>>"))
        isEof = 1;

      fwrite(buffer, 1, readBytes - (isEof * 7), output);
    }

    if (isEof)
      break;

    memset(buffer, 0, BUFSIZE);
  }

  fflush(output);

  fclose(output);
}

int initializeClient(char *serverHost)
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

  inet_pton(AF_INET, serverHost, &(serverAddress.sin_addr));

  if (connect(clientFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
  {
    perror("Cannot connect to server");
    exit(EXIT_FAILURE);
  }

  printf("Connected to server\n");

  return clientFd;
}

#ifndef DEBUG
void askForHost(char *host)
{
  printf("Server host? ");
  scanf("%s", host);
}

void askForFile(char *fileName)
{
  printf("File name? ");
  scanf("%s", fileName);
}

int main()
{
  char fileName[64];
  char host[16];

  askForHost(host);

  int clientFd = initializeClient(host);

  askForFile(fileName);

  downloadFile(clientFd, fileName, fileName);

  close(clientFd);
}
#endif // DEBUG