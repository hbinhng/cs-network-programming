#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../common.hpp"
#include "server.hpp"

using namespace std;

void Server::lifeCycle() {
  while (true) {
    sockaddr_in addr;
    socklen_t len = sizeof(addr);

    int read = recvfrom(fd, buffer, Server::BUFSIZE, 0,
                        (struct sockaddr *)(&addr), &len);

    buffer[read] = 0;

    string content = string(buffer);
    content = trim(content);

    auto clientId = toClientId(addr);

    if (clients.find(clientId) == clients.end()) {
      clients[clientId] = addr;
      for (auto it = sessionHandlers.begin(); it != sessionHandlers.end(); it++)
        (*it)(clientId);
    }

    auto messageHandlersOfClient = messageHandlers[clientId];

    if (messageHandlersOfClient == nullptr)
      continue;

    auto handlers = *messageHandlersOfClient;

    for (auto it = handlers.begin(); it != handlers.end(); it++)
      (*it)(clientId, content);
  }
}