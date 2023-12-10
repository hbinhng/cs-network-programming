#ifndef NGUYENHAIBINH_6_SERVER_H
#define NGUYENHAIBINH_6_SERVER_H

#include <functional>
#include <list>
#include <string>
#include <unordered_map>

#include <sys/socket.h>

using namespace std;

void startServer();

class Server {
private:
  static const int BUFSIZE = 1024;
  static const int CLIENT_TTL = 30;

  int fd = 0;
  char buffer[Server::BUFSIZE];
  unordered_map<int64_t, struct sockaddr_in> clients;
  unordered_map<int64_t, list<function<void(int64_t, const string &)>> *>
      messageHandlers;
  list<function<void(int64_t)>> sessionHandlers;

  int64_t toClientId(sockaddr_in address);
  sockaddr_in toSockAddr(int64_t clientId);
  void cleanClients();

public:
  Server(string host, int port);
  ~Server();

  void lifeCycle();
  list<function<void(int64_t)>>::iterator
  onSession(function<void(int64_t)> handler);
  list<function<void(int64_t, const string &)>>::iterator
  onMessage(int64_t clientId, function<void(int64_t, const string &)> handler);
  void offMessage(int64_t clientId,
                  list<function<void(int64_t, const string &)>>::iterator it);
  void sendMessage(int64_t clientId, const string &message);
  void sendMessageRaw(int64_t clientId, char *buf, int n);
  void closeSession(int64_t clientId);
};

#endif // NGUYENHAIBINH_6_SERVER_H