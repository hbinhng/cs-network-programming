/**
 * Author: Hai Binh Nguyen #20020189
 * Worker.c: This file implements:
 * - Worker thread handler (Worker_ThreadRountine)
 * - Worker argument structure initializer (WorkerArg_Init)
 */

#include "Worker.h"
#include "../Server/Server.h"
#include "../User/User.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void Worker_HandleLogin(User *user, int newId);
void Worker_HandleList(User *user);
void Worker_HandleSend(User *user, int targetUid, char *message,
                       int contentLength);
void Worker_HandleQuit(User *user);

int handle(User *user, char *buffer, int length) {
  int readBytes = 0;
  char command[5];
  sscanf(buffer, "%s%n", command, &readBytes);

  if (strcmp("login", command) == 0) {
    int id;
    sscanf(buffer + readBytes, "%d", &id);

    Worker_HandleLogin(user, id);
  } else if (strcmp("list", command) == 0) {
    Worker_HandleList(user);
  } else if (strcmp("send", command) == 0) {
    if (user->UserId < 0) {
      Server_Respond(user->SockFd, "You are not logged in yet\n");

      return 0;
    }

    int id, r;
    char message[100];
    sscanf(buffer + readBytes, "%d %n", &id, &r);

    int messageLength = length - readBytes - r;

    memcpy(message, buffer + readBytes + r, messageLength);

    Worker_HandleSend(user, id, message, messageLength);
  } else if (strcmp("quit", command) == 0) {
    Server_Respond(user->SockFd, "OK bye\n");
    return 1;
  } else {
    Server_Respond(user->SockFd, "Unknown command\n");
  }

  return 0;
}

void *Worker_ThreadRoutine(void *args) {
  printf("Worker thread initialized\n");

  WorkerArg *workerArg = *((WorkerArg **)args);
  User *user = workerArg->User;

  const int bufferSize = 1024;
  char *buffer = (char *)malloc(bufferSize * sizeof(char));

  for (int i = 0; i < user->Server->Allocated; i++)
    printf("User ID: %d\n", user->Server->Users[i]->UserId);

  while (1) {
    int readBytes = read(user->SockFd, buffer, bufferSize);

    if (readBytes == 0) {
      printf("Client unexpectedly closed connection\n");
      break;
    }

    buffer[readBytes] = 0;

    int quited = handle(user, buffer, readBytes);

    if (quited) {
      printf("Client exited\n");
      break;
    }
  }

  Server_UserExit(user->Server, user->UserId);
  WorkerArg_Dispose(workerArg);
}

WorkerArg *WorkerArg_Init(pthread_t *threadId, User *user) {
  WorkerArg *arg = (WorkerArg *)malloc(sizeof(WorkerArg));

  arg->ThreadId = threadId;
  arg->User = user;

  return arg;
}

void WorkerArg_Dispose(WorkerArg *arg) { free(arg); }