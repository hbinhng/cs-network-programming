/**
 * Author: Hai Binh Nguyen #20020189
 * main.c: This file bootstraps client application.
 */

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>

int Client_Init(char *serverHost, int port);
void *Client_ServerResponseRoutine(void *args);
void *Client_UserInputRoutine(void *args);

int main() {
  char host[15];
  printf("Server host? ");
  scanf("%s", host);

  int clientFd = Client_Init(host, 4620);

  pthread_t userInputThread;
  pthread_t serverResponseThread;

  pthread_create(&userInputThread, NULL, Client_UserInputRoutine,
                 (void *)&clientFd);
  pthread_create(&serverResponseThread, NULL, Client_ServerResponseRoutine,
                 (void *)&clientFd);

  pthread_join(userInputThread, NULL);
  pthread_join(serverResponseThread, NULL);
}