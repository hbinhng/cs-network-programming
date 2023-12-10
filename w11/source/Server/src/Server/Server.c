/**
 * Author: Hai Binh Nguyen #20020189
 * Server.c: This file implements Server utility methods:
 * - Find user in Server's user list (Server_FindUser)
 * - Find user and return index in Server's user list (Server_FindUserIndex)
 * - Thread safe disconnection handler (Server_UserExit)
 */

#include "Server.h"
#include "../User/User.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

User *Server_FindUser(Server *server, int targetUid) {
  for (int i = 0; i < server->Allocated; ++i)
    if (server->Users[i]->UserId == targetUid)
      return server->Users[i];

  return NULL;
}

int Server_FindUserIndex(Server *server, int targetUid) {
  for (int i = 0; i < server->Allocated; ++i)
    if (server->Users[i]->UserId == targetUid)
      return i;

  return -1;
}

static pthread_mutex_t Server_AccessLock;

void Server_UserExit(Server *server, int targetUid) {
  pthread_mutex_lock(&Server_AccessLock);

  int index = Server_FindUserIndex(server, targetUid);

  if (index == -1)
    goto release_lock;

  User *user = server->Users[index];

  User_Dispose(user);

  --(server->Allocated);

  for (int i = index; i < server->Allocated; ++i) {
    server->Users[i] = server->Users[i + 1];
    server->ConnectionThreads[i] = server->ConnectionThreads[i + 1];
  }

  printf("Connection pool: %d/%d\n", server->Allocated, server->Capacity);

release_lock:
  pthread_mutex_unlock(&Server_AccessLock);
}

void Server_Respond(int fd, char *content) {
  send(fd, content, strlen(content), 0);
}