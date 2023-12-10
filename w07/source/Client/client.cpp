/**
 * Author: Hai-Binh Nguyen #20020189
 * client.cpp: This file implement client side application.
 */

#include <iostream>
#include <string>

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "client.hpp"

using namespace std;

static sockaddr_in serverAddress;
static socklen_t len = sizeof(serverAddress);

void downloadFile(int clientFd, string fileName, string localFilePath) {
  cout << "Download " << fileName << " to local " << localFilePath << endl;

  const int BUFSIZE = 1024;
  char buffer[BUFSIZE];

  sendto(clientFd, "DOWNLOAD", 8, 0, (sockaddr *)(&serverAddress), len);

  buffer[recvfrom(clientFd, buffer, BUFSIZE, 0, (sockaddr *)(&serverAddress),
                  &len)] = 0;

  sendto(clientFd, fileName.c_str(), fileName.length(), 0,
         (sockaddr *)(&serverAddress), len);

  int isFileContent = 0;
  int readBytes;
  int isEof = 0;

  FILE *output = fopen(localFilePath.c_str(), "w");

  while (readBytes = recvfrom(clientFd, buffer, BUFSIZE, 0,
                              (sockaddr *)(&serverAddress), &len)) {
    if (readBytes < 0)
      break;

    buffer[readBytes] = 0;

    if (!isFileContent && (buffer, "201 FILE found;"))
      isFileContent = 1;
    else if (isFileContent) {
      if (!strcmp(buffer + readBytes - 7, "<<EOF>>"))
        isEof = 1;

      fwrite(buffer, 1, readBytes - (isEof * 7), output);
    }

    if (isEof)
      break;
  }

  fflush(output);

  fseek(output, 0, SEEK_END);
  size_t fileSize = ftell(output);

  cout << "File size: " << fileSize << " bytes" << endl;

  fclose(output);
}

int initializeClient(string serverHost, int port) {
  int clientFd = socket(AF_INET, SOCK_DGRAM, 0);

  if (clientFd == -1) {
    cerr << "Cannot create socket";
    exit(EXIT_FAILURE);
  }

  cout << "Socket created successfully" << endl;

  serverAddress.sin_port = htons(port);
  serverAddress.sin_family = AF_INET;

  inet_pton(AF_INET, serverHost.c_str(), &(serverAddress.sin_addr));

  return clientFd;
}

#ifndef DEBUG
string askForHost() {
  cout << "Server host? ";
  string host;
  cin >> host;

  return host;
}

string askForFile() {
  cout << "File name? ";
  string fileName;
  cin >> fileName;

  return fileName;
}

int main() {
  auto host = askForHost();

  int clientFd = initializeClient(host, 4620);

  auto fileName = askForFile();

  downloadFile(clientFd, fileName, fileName + " - copy");

  close(clientFd);
}
#endif // DEBUG