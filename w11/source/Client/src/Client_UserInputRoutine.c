/**
 * Author: Hai Binh Nguyen #20020189
 * Client_UserInputRoutine.c: This file implements thread
 * routine of user input receiver. This is to prevent
 * potential thread blocking experience when using single thread
 * for both user input and server response.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

void *Client_UserInputRoutine(void *args) {
  int fd = *(int *)args;

  char *buffer = (char *)malloc(150 * sizeof(char));
  int isQuited = 0;

  while (1) {
    fgets(buffer, 150, stdin);

    if (strncmp(buffer, "quit", 4) == 0)
      isQuited = 1;

    send(fd, buffer, strlen(buffer), 0);

    if (isQuited)
      break;
  }

  free(buffer);
}