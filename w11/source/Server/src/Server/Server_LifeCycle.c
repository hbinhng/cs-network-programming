/**
 * Author: Hai Binh Nguyen #20020189
 * Server_LifeCycle: This file implements life cycle method for
 * server. Life cycle method (Server_LifeCycle) is mainly for
 * accepting new client connection and initalize new worker
 * thread for each connection.
 */

#include "../User/User.h"
#include "../Worker/Worker.h"
#include "Server.h"

#include <arpa/inet.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <unistd.h>

void Server__internal_InitWorker(Server *server, User *user) {
  pthread_t threadId;

  WorkerArg *arg = WorkerArg_Init(&threadId, user);

  pthread_create(&threadId, NULL, Worker_ThreadRoutine, (void *)&arg);

  server->ConnectionThreads[server->Allocated] = &threadId;
  server->Users[server->Allocated] = user;

  ++(server->Allocated);

  printf("Connection pool: %d/%d\n", server->Allocated, server->Capacity);
}

void Server_LifeCycle(Server *server) {
  int connectionCounter = 0;

  printf("Server life cycle method started, client capacity: %d\n",
         server->Capacity);

  while (1) {
    if (server->Allocated >= server->Capacity)
      goto delay;

    struct sockaddr_in clientAddress;
    socklen_t clientAddressLen = sizeof(clientAddress);

    printf("Waiting for new client...\n");

    int fd = accept(server->SockFd, (struct sockaddr *)&clientAddress,
                    &clientAddressLen);

    printf("New client opened a connection\n");

    if (fd < 0) {
      perror("Unable to accept new connection\n");
      goto delay;
    }

    User *user = User_Init(server, fd, clientAddress);
    user->UserId = -(++connectionCounter);

    Server__internal_InitWorker(server, user);

  delay:
    sleep(0.1);
  }
}