/**
 * Author: Hai Binh Nguyen #20020189
 * Client_ServerResponseRoutine.c: This file implements thread
 * routine of server response receiver. This is to prevent
 * potential thread blocking experience when using single thread
 * for both user input and server response.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void *Client_ServerResponseRoutine(void *args) {
  int fd = *(int *)args;
  int readBytes;
  char *buffer = (char *)malloc(50 * sizeof(char));

  while ((readBytes = read(fd, buffer, 50)) > 0) {
    printf("%.*s", readBytes, buffer);
  }

  free(buffer);
}