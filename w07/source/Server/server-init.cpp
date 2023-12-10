#include <iostream>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#include "server.hpp"

using namespace std;

int initializeServer(string host, int port)
{
  struct sockaddr_in serverAddress;

  int serverFd = 0;
  int opt = 1;

  if ((serverFd = socket(AF_INET, SOCK_DGRAM, 0)) == 0)
  {
    cerr << "Cannot create socket" << endl;
    exit(EXIT_FAILURE);
  }

  cout << "Socket created successfully" << endl;

  if (setsockopt(serverFd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
  {
    cerr << "Cannot set socket options failed" << endl;
    exit(EXIT_FAILURE);
  }

  cout << "Socket options set successfully" << endl;

  serverAddress.sin_port = htons(port);
  serverAddress.sin_family = AF_INET;
  inet_pton(AF_INET, host.c_str(), &(serverAddress.sin_addr.s_addr));

  if (bind(serverFd, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0)
  {
    cerr << "Cannot bind to specified address" << endl;
    exit(EXIT_FAILURE);
  }

  cout << "Server is bound to address successfully" << endl;
  cout << "Server is listening on " << host << ":" << port << endl;

  return serverFd;
}

Server::Server(string host, int port)
{
  fd = initializeServer(host, port);
}