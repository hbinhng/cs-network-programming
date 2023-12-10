/**
 * Author: Hai-Binh Nguyen #20020189
 * server.cpp: This file implements server side application.
 */

#include <iostream>
#include <memory>
#include <string>

#include <arpa/inet.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "../common.hpp"
#include "server.hpp"

using namespace std;

void handleDownload(Server *server, int64_t clientId, const char *fileName) {
  if (access(fileName, F_OK) == -1) {
    server->sendMessage(clientId, "404 FILE not found");
    return;
  }

  server->sendMessage(clientId, "201 FILE found;");

  FILE *file = fopen(fileName, "r");

  fseek(file, 0, SEEK_END);
  size_t fileSize = ftell(file);

  cout << "Reading file \"" << fileName << "\" (" << fileSize << " bytes)"
       << endl;

  fseek(file, 0, SEEK_SET);

  const int fbufsiz = 1024;
  char *buffer = (char *)malloc(fbufsiz);
  int readBytes;

  memset(buffer, 0, fbufsiz);

  while (readBytes = fread(buffer, 1, fbufsiz, file))
    server->sendMessageRaw(clientId, buffer, readBytes);

  server->sendMessage(clientId, "<<EOF>>");

  cout << "File transfer completed" << endl;

  free(buffer);
  fclose(file);
}

void startServer() {
  auto server = new Server("0.0.0.0", 4620);

  auto handleSessionEvent = [&](int64_t clientId) -> void {
    auto willDownload = make_shared<bool>(false);
    auto handlerIt =
        make_shared<list<function<void(int64_t, const string &)>>::iterator>(
            nullptr);

    auto handleMessageEvent =
        [=, &server](int64_t clientId, const string &content) mutable -> void {
      if (!(*willDownload) && content.compare("DOWNLOAD") == 0) {
        server->sendMessage(clientId, "200 DOWNLOAD OK");
        (*willDownload) = true;
        return;
      }

      if (!(*willDownload) && content.compare("QUIT") == 0) {
        cout << "Client exited" << endl;
        server->sendMessage(clientId, "500 bye");
        server->closeSession(clientId);
        server->offMessage(clientId, *handlerIt);
        return;
      }

      if (!(*willDownload)) {
        server->sendMessage(clientId, "400 UNKNOWN command");
        return;
      }

      handleDownload(server, clientId, content.c_str());
      (*willDownload) = false;
    };

    (*handlerIt) = server->onMessage(clientId, handleMessageEvent);
  };

  server->onSession(handleSessionEvent);

  server->lifeCycle();
}

#ifndef DEBUG
int main() { startServer(); }
#endif // DEBUG