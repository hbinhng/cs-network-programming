/**
 * Author: Hai Binh Nguyen #20020189
 * Worker_HandleSend.c: This file implements `send` command
 * handler.
 */

#include "../Server/Server.h"
#include "../User/User.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void Worker_HandleSend(User *user, int targetUid, char *message,
                       int contentLength) {
  if (user->UserId < 0) {
    Server_Respond(user->SockFd, "You have to log in first");
    return;
  }

  User *targetUser = Server_FindUser(user->Server, targetUid);

  if (targetUser == NULL) {
    Server_Respond(user->SockFd, "Recipient does not exist\n");
    return;
  }

  char *outbuf = (char *)malloc((contentLength + 20) * sizeof(char));
  int printed = 0;

  sprintf(outbuf, "[From %d] %n", user->UserId, &printed);
  memcpy(outbuf + printed, message, contentLength);
  strcat(outbuf + printed + contentLength, "\n");

  send(targetUser->SockFd, outbuf, printed + contentLength + 1, 0);
  Server_Respond(user->SockFd, "Message was sent successfully\n");

  free(outbuf);
}