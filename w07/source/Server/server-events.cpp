#include <algorithm>
#include <functional>
#include <iostream>
#include <list>
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "../common.hpp"
#include "server.hpp"

using namespace std;

list<function<void(int64_t, const string &)>>::iterator

Server::onMessage(int64_t clientId,
                  function<void(int64_t, const string &)> handler) {
  auto handlers = messageHandlers[clientId];

  if (handlers == nullptr)
    handlers = messageHandlers[clientId] =
        new list<function<void(int64_t, const string &)>>();

  handlers->push_back(handler);

  return --handlers->end();
}

list<function<void(int64_t)>>::iterator
Server::onSession(function<void(int64_t)> handler) {
  sessionHandlers.push_back(handler);

  return --sessionHandlers.end();
}

void Server::closeSession(int64_t clientId) { clients.erase(clientId); }

void Server::offMessage(
    int64_t clientId,
    list<function<void(int64_t, const string &)>>::iterator it) {
  auto handlers = messageHandlers[clientId];

  if (handlers == nullptr)
    return;

  handlers->erase(it);
}