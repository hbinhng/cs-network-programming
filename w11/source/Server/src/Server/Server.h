#ifndef W11_SERVER_H
#define W11_SERVER_H

#include <arpa/inet.h>
#include <pthread.h>

typedef struct Server Server;

#include "../User/User.h"

struct Server {
  int SockFd;
  struct sockaddr_in Address;

  int Capacity;
  int Allocated;
  pthread_t **ConnectionThreads;
  User **Users;
};

Server *Server_Init(char *host, int port, int capacity);
void Server_LifeCycle(Server *server);

User *Server_FindUser(Server *server, int targetUid);
int Server_FindUserIndex(Server *server, int targetUid);
void Server_UserExit(Server *server, int targetUid);
void Server_Respond(int fd, char *content);

#endif // W11_SERVER_H