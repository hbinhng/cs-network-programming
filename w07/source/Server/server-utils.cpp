#include <iostream>
#include <sstream>
#include <string>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "server.hpp"

using namespace std;

int64_t Server::toClientId(sockaddr_in address) {
  int64_t hostPart = ntohl(address.sin_addr.s_addr);
  hostPart <<= 16LL;
  int64_t portPart = ntohs(address.sin_port);

  int64_t clientId = hostPart | portPart;

  return clientId;
}

sockaddr_in Server::toSockAddr(int64_t clientId) { return clients[clientId]; }

void Server::sendMessage(int64_t clientId, const string &message) {
  auto addrIn = toSockAddr(clientId);
  auto addr = (struct sockaddr *)(&addrIn);

  sendto(fd, message.c_str(), message.length(), 0, addr, sizeof(addrIn));
}

void Server::sendMessageRaw(int64_t clientId, char *buf, int n) {
  auto addrIn = toSockAddr(clientId);
  auto addr = (struct sockaddr *)(&addrIn);

  sendto(fd, buf, n, 0, addr, sizeof(addrIn));
}