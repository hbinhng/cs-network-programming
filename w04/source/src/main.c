/**
 * Author: Hai-Binh Nguyen #20020189
 * main.c: This file implements the start point of the program.
 * To run client application, call ./main --client.
 * To run server application, call ./main --server.
 * Target application is required, missing the target results in fatal error.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "client.h"
#include "server.h"

int extractTarget(const char *arg)
{
  if (!strcmp(arg, "--server"))
    return 0;

  if (!strcmp(arg, "--client"))
    return 1;

  return -1;
}

void exitUnexpectedTarget()
{
  perror("Unexpected start target, the target must be specified with --server or --client");
  exit(EXIT_FAILURE);
}

int main(int argv, const char *args[])
{
  if (argv < 2)
    exitUnexpectedTarget();

  int target = extractTarget(args[1]);

  if (target == -1)
    exitUnexpectedTarget();

  target == 0 ? startServer() : startClient();
}