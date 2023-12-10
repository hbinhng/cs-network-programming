#ifndef W11_SERVER_USER_H
#define W11_SERVER_USER_H

#include <arpa/inet.h>
#include <sys/socket.h>

typedef struct User User;
static const short User_SendMessage_OK = 0;
static const short User_SendMessage_TargetUserNotFound = 1;
static const short User_SendMessage_SocketError = 2;

#include "../Server/Server.h"

struct User {
  Server *Server;
  int SockFd;
  struct sockaddr_in Address;
  int UserId;
};

User *User_Init(Server *server, int fd, struct sockaddr_in address);
short User_SendMessage(User *user, int targetUid, char *content,
                       int contentLength);
void User_Dispose(User *user);

#endif // W11_SERVER_USER_H