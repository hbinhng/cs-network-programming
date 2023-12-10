/**
 * Author: Hai Binh Nguyen #20020189
 * Worker_HandleLogin.c: This file implements `login` command
 * handler.
 */

#include "../Server/Server.h"
#include "../User/User.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void Worker_HandleLogin(User *user, int newId) {
  char *outbuf = (char *)malloc(sizeof(char) * 50);

  if (newId < 0) {
    sprintf(outbuf, "ID must be positive\n");
    Server_Respond(user->SockFd, outbuf);
    goto cleanup;
  }

  int index = Server_FindUserIndex(user->Server, newId);

  if (index == -1) {
    printf("User changed from ID #%d to #%d\n", user->UserId, newId);

    user->UserId = newId;
    sprintf(outbuf, "Logged in with ID #%d\n", newId);
    Server_Respond(user->SockFd, outbuf);
    goto cleanup;
  }

  sprintf(outbuf, "ID #%d already exists\n", newId);
  Server_Respond(user->SockFd, outbuf);

cleanup:
  free(outbuf);
}