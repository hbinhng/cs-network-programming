/**
 * Author: Hai Binh Nguyen #20020189
 * User.c: This file implements User methods:
 * - User structure initializer (User_Init)
 * - User structure finalizer (User_Dispose)
 * - Send message from user to user (User_SendMessage)
 */

#include "User.h"
#include "../Server/Server.h"

#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

User *User_Init(Server *server, int fd, struct sockaddr_in address) {
  User *user = (User *)malloc(sizeof(User));

  user->Server = server;
  user->Address = address;
  user->SockFd = fd;

  return user;
}

void User_Dispose(User *user) {
  close(user->SockFd);
  free(user);
}

short User_SendMessage(User *user, int targetUid, char *content,
                       int contentLength) {
  User *targetUser = Server_FindUser(user->Server, targetUid);

  if (targetUser == NULL)
    return User_SendMessage_TargetUserNotFound;

  int sent = send(user->SockFd, content, contentLength, 0);

  if (sent == -1)
    return User_SendMessage_SocketError;

  return User_SendMessage_OK;
}