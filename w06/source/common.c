/**
 * Author: Hai-Binh Nguyen #20020189
 * common.c: This file:
 * - Implements common string methods;
 * - Set PORT constant.
 */

#include <string.h>
#include <sys/socket.h>

#include "common.h"

const int PORT = 4062;

char *ltrim(char *s)
{
  while (isspace(*s))
    s++;
  return s;
}

char *rtrim(char *s)
{
  char *back = s + strlen(s);
  while (isspace(*--back))
    ;
  *(back + 1) = '\0';
  return s;
}

char *trim(char *s)
{
  return rtrim(ltrim(s));
}

void sendMessage(int fd, char *content)
{
  send(fd, content, strlen(content), 0);
}