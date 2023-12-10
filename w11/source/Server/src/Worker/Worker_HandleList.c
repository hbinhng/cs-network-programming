/**
 * Author: Hai Binh Nguyen #20020189
 * Worker_HandleList.c: This file implements `list` command
 * handler.
 */

#include "../Server/Server.h"
#include "../User/User.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void Worker_HandleList(User *user) {
  Server *server = user->Server;
  char digits[18];

  for (int i = 0; i < server->Allocated; ++i) {
    User *iterating = server->Users[i];

    if (iterating->UserId > 0) {
      sprintf(digits, "%d%s\n", iterating->UserId,
              iterating->UserId == user->UserId ? " <- you" : "");
      Server_Respond(user->SockFd, digits);
    }
  }

  Server_Respond(user->SockFd, "#\n");
}